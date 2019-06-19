<?php
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
require_once APPRAIZ.'par3/classes/model/WS_Sigarp_FNDE.class.inc';

class Par3_Controller_WS_Sigarp_FNDE extends Controle
{
    public $ws_usuario, $ws_senha, $dotid, $proid, $inuid, $reformular;
    
    public function __construct()
    {
        $this->model = new Par3_Model_WS_Sigarp_FNDE();
        $this->reformular = '';
    }
    
    public function montaXNLItensSigarp()
    {
        $data_created = date("c");
        $arrTermo = $this->retornarDadosTermos();
        $this->proid = $arrTermo['proid'];
        $this->inuid = $arrTermo['inuid'];
        
        $dom = new DOMDocument("1.0", "ISO-8859-1");
        $request = $dom->createElement('request');
        
        $header    = $dom->createElement('header');
        $app       = $dom->createElement('app', 'SIGARP');
        $version   = $dom->createElement('version', '1.0');
        $created   = $dom->createElement('created', date("c"));
        $header->appendChild($app);
        $header->appendChild($version);
        $header->appendChild($created);
        $request->appendChild($header);
        
        $body      = $dom->createElement('body');
        $auth      = $dom->createElement('auth');
        $usuario   = $dom->createElement('usuario', $this->ws_usuario);
        $senha     = $dom->createElement('senha', $this->ws_senha);
        $auth->appendChild($usuario);
        $auth->appendChild($senha);
        $body->appendChild($auth);
        
        $params                = $dom->createElement('params');
        $cnpj                  = $dom->createElement('cnpj', $arrTermo['procnpj']);
        $nu_processo           = $dom->createElement('nu_processo', $arrTermo['pronumeroprocesso']);
        $nu_seq_tipo_pagamento = $dom->createElement('nu_seq_tipo_pagamento', '1');
        $nu_termo_compromisso  = $dom->createElement('nu_termo_compromisso', $arrTermo['dotnumero']);
        $dt_vigencia           = $dom->createElement('dt_vigencia', $arrTermo['dotdatafimvigencia']);
        $params->appendChild($cnpj);
        $params->appendChild($nu_processo);
        $params->appendChild($nu_seq_tipo_pagamento);
        $params->appendChild($nu_termo_compromisso);
        $params->appendChild($dt_vigencia);
        
        $params = $this->retornarItensIniciativa($dom, $params);
        
        $body->appendChild($params);
        $request->appendChild($body);
        
        $dom->appendChild($request);
        $arqXml = $dom->saveXML();
        
        return $arqXml;
    }
    
    public function enviarItensSigarp()
    {
        
        try {
            $arqXml = $this->montaXNLItensSigarp();
            
            /*$arLog = $this->model->pegaLinha("SELECT lw.lgwid, lw.lgwmsgretorno, lw.lgwerro
                                                FROM par3.logwssigarp lw
                                                WHERE lw.lgwservico = 'diponibilizaPregoesItens'
                                                    AND lw.dotid = {$this->dotid}
                                                ORDER BY lgwid desc");

            if($arLog['lgwerro'] == 't'){
                return array('retorno' => $arLog['lgwmsgretorno'], 'erro' => 't');
            } else {*/
            
            if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
                $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/item';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
            }
                
                $xml = Fnde_Webservice_Client::CreateRequest()
                        ->setURL($urlWS)
                        ->setParams(array('xml' => $arqXml, 'method' => 'solicitar'))
                        ->execute();
                
                $xmlRetorno = $xml;
                $xml = simplexml_load_string(stripslashes($xml));
                
                
                $result = (integer) $xml->status->result;
                
            if ($result) {
                $this->model->executar("UPDATE par3.itemadesaosigarp SET adestatus = 'I' WHERE dotid = {$this->dotid}");
                $this->model->commit();
                    
                for ($i=0; $i < sizeof($xml->body->params->adesaopregao); $i++) {
                    $cnpj                       = (string)$xml->body->params->cnpj;
                    $nu_processo                = (string)$xml->body->params->nu_processo;
                    $nu_seq_solicitacao_adesao  = (string)$xml->body->params->adesaopregao[$i]->nu_seq_solicitacao_adesao;
                    $nu_sub_acao                = (string)$xml->body->params->adesaopregao[$i]->nu_sub_acao;
                    $item                       = $xml->body->params->adesaopregao[$i]->item->nu_seq_item;
                    
                    for ($a=0; $a < sizeof($item); $a++) {
                        $sql = "delete from par3.itemadesaosigarp where nu_seq_solicitacao_adesao = {$nu_seq_solicitacao_adesao} and nu_sub_acao = {$nu_sub_acao} and item = ".$item[$a]." and dotid = ".$this->dotid." and proid = ".$this->proid;
                        $this->model->executar($sql);
                            
                        $sql = "INSERT INTO par3.itemadesaosigarp(dotid, proid, cnpj, nu_processo, nu_seq_solicitacao_adesao, nu_sub_acao, item)
    								VALUES (".$this->dotid.", ".$this->proid.", '{$cnpj}', '{$nu_processo}', {$nu_seq_solicitacao_adesao}, {$nu_sub_acao}, ".$item[$a].")";
                                
                        $this->model->executar($sql);
                        $this->model->commit();
                    }
                }
                $lgwerro = 'f';
                $lgwmsgretorno = ($xml->status->message->text);
            } else {
                $lgwerro = 't';
                $lgwmsgretorno = ($xml->status->error->message->text);
            }
                $arrCampos = array();
                $arrCampos['stNomeTabela']      = 'par3.logwssigarp';
                $arrCampos['arAtributosTabela'] = array('proid', 'dotid', 'lgwservico', 'lgwurl', 'lgwrequest', 'lgwresponse', 'lgwerro', 'usucpf', 'lgwmsgretorno', 'lgwdata', 'lgwmethod');
                
                $arrCampos['proid']         = $this->proid;
                $arrCampos['dotid']         = $this->dotid;
                $arrCampos['lgwservico']    = 'enviarItensSigarp';
                $arrCampos['lgwurl']        = $urlWS;
                $arrCampos['lgwmethod']     = 'solicitar';
                $arrCampos['lgwrequest']    = $arqXml;
                $arrCampos['lgwresponse']   = $xmlRetorno;
                $arrCampos['lgwerro']       = $lgwerro;
                $arrCampos['usucpf']        = $_SESSION['usucpf'];
                $arrCampos['lgwmsgretorno'] = $lgwmsgretorno;
                trata_salvar($arrCampos);
                
                return array('retorno' => $lgwmsgretorno, 'erro' => $lgwerro);
            //}
        } catch (Exception $e) {
            # Erro 404 página not found
            if ($e->getCode() == 404) {
            }
            $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
        }
    }
    
    public function enviarItensSigarpReformulacao()
    {
        
        try {
            $arqXml = $this->montaXNLItensSigarp();
            
            /*$arLog = $this->model->pegaLinha("SELECT lw.lgwid, lw.lgwmsgretorno, lw.lgwerro
                                                FROM par3.logwssigarp lw
                                                WHERE lw.lgwservico = 'diponibilizaPregoesItens'
                                                    AND lw.dotid = {$this->dotid}
                                                ORDER BY lgwid desc");

            if($arLog['lgwerro'] == 't'){
                return array('retorno' => $arLog['lgwmsgretorno'], 'erro' => 't');
            } else {*/
                
            if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
                $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/termo';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/termo';
            }
                
                $xml = Fnde_Webservice_Client::CreateRequest()
                        ->setURL($urlWS)
                        ->setParams(array('xml' => $arqXml, 'method' => 'subacao'))
                        ->execute();
                
                $xmlRetorno = $xml;
                $xml = simplexml_load_string(stripslashes($xml));
                
                $result = (integer) $xml->status->result;
                
            if ($result) {
                $this->model->executar("UPDATE par3.itemadesaosigarp SET adestatus = 'I' WHERE dotid = {$this->dotid}");
                $this->model->commit();
                
                for ($i=0; $i < sizeof($xml->body->params->adesaopregao); $i++) {
                    $cnpj                       = (string)$xml->body->params->cnpj;
                    $nu_processo                = (string)$xml->body->params->nu_processo;
                    $nu_seq_solicitacao_adesao  = (string)$xml->body->params->adesaopregao[$i]->nu_seq_solicitacao_adesao;
                    $nu_sub_acao                = (string)$xml->body->params->adesaopregao[$i]->nu_sub_acao;
                    $item                       = $xml->body->params->adesaopregao[$i]->item->nu_seq_item;
                    
                    for ($a=0; $a < sizeof($item); $a++) {
                        $sql = "delete from par3.itemadesaosigarp where nu_seq_solicitacao_adesao = {$nu_seq_solicitacao_adesao} and nu_sub_acao = {$nu_sub_acao} and item = ".$item[$a]." and dotid = ".$this->dotid." and proid = ".$this->proid;
                        $this->model->executar($sql);
                            
                        $sql = "INSERT INTO par3.itemadesaosigarp(dotid, proid, cnpj, nu_processo, nu_seq_solicitacao_adesao, nu_sub_acao, item)
    								VALUES (".$this->dotid.", ".$this->proid.", '{$cnpj}', '{$nu_processo}', {$nu_seq_solicitacao_adesao}, {$nu_sub_acao}, ".$item[$a].")";
                                
                        $this->model->executar($sql);
                        $this->model->commit();
                    }
                }
                $lgwerro = 'f';
                $lgwmsgretorno = ($xml->status->message->text);
            } else {
                $lgwerro = 't';
                $lgwmsgretorno = ($xml->status->error->message->text);
            }
                $arrCampos = array();
                $arrCampos['stNomeTabela']      = 'par3.logwssigarp';
                $arrCampos['arAtributosTabela'] = array('proid', 'dotid', 'lgwservico', 'lgwurl', 'lgwrequest', 'lgwresponse', 'lgwerro', 'usucpf', 'lgwmsgretorno', 'lgwdata', 'lgwmethod');
                
                $arrCampos['proid']         = $this->proid;
                $arrCampos['dotid']         = $this->dotid;
                $arrCampos['lgwservico']    = 'enviarItensSigarpReformulacao';
                $arrCampos['lgwurl']        = $urlWS;
                $arrCampos['lgwmethod']     = 'subacao';
                $arrCampos['lgwrequest']    = $arqXml;
                $arrCampos['lgwresponse']   = $xmlRetorno;
                $arrCampos['lgwerro']       = $lgwerro;
                $arrCampos['usucpf']        = $_SESSION['usucpf'];
                $arrCampos['lgwmsgretorno'] = $lgwmsgretorno;
                trata_salvar($arrCampos);
                
                return array('retorno' => $lgwmsgretorno, 'erro' => $lgwerro);
            //}
        } catch (Exception $e) {
            # Erro 404 página not found
            if ($e->getCode() == 404) {
            }
            $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
        }
    }
    
    public function retornarDadosTermos()
    {
        $sql = "select distinct p.proid, p.procnpj, p.pronumeroprocesso, d.dotnumero, to_char(d.dotdatafimvigencia, 'YYYY-MM') as dotdatafimvigencia, p.inuid
				from par3.documentotermo d
					inner join par3.processo p on p.proid = d.proid
				where d.dotid = ".$this->dotid;
        $arrTermo = $this->model->pegaLinha($sql);
        
        return $arrTermo;
    }
    
    public function retornarItensIniciativa($dom, $params)
    {
        
        $sql = "select distinct i.iniid, ind.inddsc
				from par3.termocomposicao t
					inner join par3.iniciativa_planejamento ip on ip.inpid = t.inpid
					inner join par3.iniciativa i on i.iniid = ip.iniid
					inner join par3.iniciativa_descricao ind on ind.indid = i.indid
				where t.dotid = ".$this->dotid."
					and t.tecstatus = 'A'";
        $arrIniciativa = $this->model->carregar($sql);
        $arrIniciativa = $arrIniciativa ? $arrIniciativa : array();
        
        $xml = '';
        
        foreach ($arrIniciativa as $ini) {
            $sql = $this->model->retornarSQLItensIniciativa($this->dotid, $ini['iniid']);
            $arrDados = $this->model->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            $sub_acao = $dom->createElement("sub_acao");
            $nu_sub_acao = $dom->createElement("nu_sub_acao", $ini['iniid']);
            $id_sistema = $dom->createElement("id_sistema", '231');
            $sub_acao->appendChild($nu_sub_acao);
            $sub_acao->appendChild($id_sistema);
    
            foreach ($arrDados as $v) {
                $arRetorno = $this->diponibilizaPregoesItens($v['seq_sigarp'], $ini['iniid'], $v['itcid']);
                
                if ($arRetorno['erro'] == 'f') {
                    $arRetorno = $arRetorno['retorno'];
                    if (is_array($arRetorno) && $arRetorno['nu_seq_pregao']) {
                        $item = $dom->createElement("item");
                        $nu_seq_pregao  = $dom->createElement("nu_seq_pregao", $arRetorno['nu_seq_pregao']);
                        $nu_seq_item    = $dom->createElement("nu_seq_item", $arRetorno['nu_seq_item']);
                        $qt_item        = $dom->createElement("qt_item", $v['ipiquantidadeaprovada']);
                        $vl_pagamento   = $dom->createElement("vl_pagamento", $v['ipivalorreferencia']);
                        
                        $sql = "delete from par3.itempregaosigarp where iniid = ".$ini['iniid']." and itcid = ".$v['itcid']." and dotid = ".$this->dotid." and proid = ".$this->proid;
                        $this->model->executar($sql);
                        
                        $sql = "INSERT INTO par3.itempregaosigarp(iniid, proid, dotid, itcid, nu_seq_pregao, nu_seq_item, qt_item, vl_pagamento)
                                VALUES(".$ini['iniid'].", ".$this->proid.", ".$this->dotid.", ".$v['itcid'].", ".$arRetorno['nu_seq_pregao'].", ".$arRetorno['nu_seq_item'].", ".$v['ipiquantidadeaprovada'].", ".$v['ipivalorreferencia'].")";
                        
                        $this->model->executar($sql);
                        $this->model->commit();
                        
                        $item->appendChild($nu_seq_pregao);
                        $item->appendChild($nu_seq_item);
                        $item->appendChild($qt_item);
                        $item->appendChild($vl_pagamento);
                        
                        if ($this->reformular == 'S') {
                            $acao = $dom->createElement("acao", 'I');
                            $item->appendChild($acao);
                        }
                        $sub_acao->appendChild($item);
                    }
                } else {
                    /*if( $_REQUEST['inuid'] ){
                        $url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=documentos&inuid='.$_REQUEST['inuid'];
                        simec_redirecionar($url, 'error', $arRetorno['retorno']);
                    }*/
                }
            }
            $params->appendChild($sub_acao);
        }
        return $params;
    }
        
    public function diponibilizaPregoesItens($seq_sigarp, $iniid, $itcid = '')
    {
        try {
            $data_created = date("c");
            
            $sql = "SELECT estuf FROM par3.instrumentounidade WHERE inuid = ".$this->inuid;
            $estuf = $this->model->pegaUm($sql);
            
            $dom = new DOMDocument("1.0", "ISO-8859-1");
            $request = $dom->createElement('request');
            
            $header    = $dom->createElement('header');
            $app       = $dom->createElement('app', 'SIGARP');
            $version   = $dom->createElement('version', '1.0');
            $created   = $dom->createElement('created', date("c"));
            $header->appendChild($app);
            $header->appendChild($version);
            $header->appendChild($created);
            $request->appendChild($header);
            
            $body      = $dom->createElement('body');
            $auth      = $dom->createElement('auth');
            $usuario   = $dom->createElement('usuario', $this->ws_usuario);
            $senha     = $dom->createElement('senha', $this->ws_senha);
            $auth->appendChild($usuario);
            $auth->appendChild($senha);
            $body->appendChild($auth);
            
            $params      = $dom->createElement('params');
            $nu_seq_item = $dom->createElement('nu_seq_item', $seq_sigarp);
            $uf          = $dom->createElement('uf', $estuf);
            
            $params->appendChild($nu_seq_item);
            $params->appendChild($uf);
            
            $body->appendChild($params);
            $request->appendChild($body);
            
            $dom->appendChild($request);
            $arqXml = $dom->saveXML();


            if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
                $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/pregao';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
            }
                
            $xml = Fnde_Webservice_Client::CreateRequest()
                    ->setURL($urlWS)
                    ->setParams(array('xml' => $arqXml, 'method' => 'listar'))
                    ->execute();
                        
            $xmlRetorno = $xml;
            
            $xml = simplexml_load_string(stripslashes($xml));
            $result = (integer) $xml->status->result;
            
            //ver(simec_htmlentities($arqXml), $xml, (string)$xml->nu_seq_pregao,d);
            if (!$result) {
                $lgwerro = 't';
                $lgwmsgretorno = ($xml->status->error->message->text);
                $retorno = array('retorno' => $lgwmsgretorno, 'erro' => $lgwerro);
            } else { //sucesso
                $retorno = array('nu_seq_item' => (string)$xml->body->NU_SEQ_ITEM,
                                 'valor_item' => (string)$xml->body->VL_ITEM,
                                 'nu_seq_pregao' => (string)$xml->body->NU_SEQ_PREGAO,
                                 'sg_uf' => (string)$xml->body->SG_UF
                                );
            
                $lgwerro = 'f';
                $lgwmsgretorno = ($xml->status->message->text);
                $retorno = array('retorno' => $retorno, 'erro' => $lgwerro);
            }
            $arrCampos = array();
            $arrCampos['stNomeTabela']      = 'par3.logwssigarp';
            $arrCampos['arAtributosTabela'] = array('proid', 'dotid', 'iniid', 'itcid', 'lgwservico', 'lgwurl', 'lgwrequest', 'lgwresponse', 'lgwerro', 'usucpf', 'lgwmsgretorno', 'lgwdata', 'seq_sigarp', 'lgwmethod');
            
            $arrCampos['proid']         = $this->proid;
            $arrCampos['dotid']         = $this->dotid;
            $arrCampos['iniid']         = $iniid;
            $arrCampos['itcid']         = $itcid;
            $arrCampos['seq_sigarp']    = $seq_sigarp;
            $arrCampos['lgwservico']    = 'diponibilizaPregoesItens';
            $arrCampos['lgwurl']        = $urlWS;
            $arrCampos['lgwmethod']     = 'listar';
            $arrCampos['lgwrequest']    = $arqXml;
            $arrCampos['lgwresponse']   = $xmlRetorno;
            $arrCampos['lgwerro']       = $lgwerro;
            $arrCampos['usucpf']        = $_SESSION['usucpf'];
            $arrCampos['lgwmsgretorno'] = $lgwmsgretorno;
            trata_salvar($arrCampos);
            
            return $retorno;
        } catch (Exception $e) {
            # Erro 404 página not found
            if ($e->getCode() == 404) {
            }
            $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
            return array('erro'=> $erroMSG);
        }
    }
    
    public function consultarSituacaoAdesao( $processo )
    {
        try {
            $data_created = date("c");
            
            $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
    <header>
        <app>string</app>
        <version>string</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>{$this->ws_usuario}</usuario>
			<senha>{$this->ws_senha}</senha>
        </auth>
        <params>
            <nu_processo>{$processo}</nu_processo>
        </params>
    </body>
</request>
XML;
        
            if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
                $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/solicitacao';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/consultarsituacaoadesao';
            }

            $xml = Fnde_Webservice_Client::CreateRequest()
                    ->setURL($urlWS)
                    ->setParams(array('xml' => $arqXml, 'method' => 'consultarsituacaoadesao'))
                    ->execute();
                        
            $xmlRetorno = $xml;
            $xml = simplexml_load_string(stripslashes($xml));
            ver($xml,d);
            $result = (integer) $xml->status->result;
                
            if ($result) {
                $this->model->executar("delete from par3.s_categoria");
                
                $arrCampos = array();
                $arrCampos['stNomeTabela']      = 'par3.s_categoria';
                $arrCampos['arAtributosTabela'] = array('nu_seq_categoria', 'no_categoria');
                
                foreach ($xml->body as $categoria) {
                    $arrCampos['nu_seq_categoria']  = $categoria->NU_SEQ_CATEGORIA;
                    $arrCampos['no_categoria']      = ((string) $categoria->NO_CATEGORIA);
                    
                    trata_salvar($arrCampos);
                }
            }
        } catch (Exception $e) {
            # Erro 404 página not found
            if ($e->getCode() == 404) {
            }
            $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
        }
    }
    
    public function listarCategoria()
    {
        try {
            $data_created = date("c");
            
            $arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>{$this->ws_usuario}</usuario>
			<senha>{$this->ws_senha}</senha>
		</auth>
		<params>
		</params>
	</body>
</request>
XML;
        
            if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
                $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/categoria';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/categoria';
            }

            $xml = Fnde_Webservice_Client::CreateRequest()
                    ->setURL($urlWS)
                    ->setParams(array('xml' => $arqXml, 'method' => 'listar'))
                    ->execute();
                        
            $xmlRetorno = $xml;
            $xml = simplexml_load_string(stripslashes($xml));
            //ver($xml,d);
            $result = (integer) $xml->status->result;
                
            if ($result) {
                $this->model->executar("delete from par3.s_categoria");
                
                $arrCampos = array();
                $arrCampos['stNomeTabela']      = 'par3.s_categoria';
                $arrCampos['arAtributosTabela'] = array('nu_seq_categoria', 'no_categoria');
                
                foreach ($xml->body as $categoria) {
                    $arrCampos['nu_seq_categoria']  = $categoria->NU_SEQ_CATEGORIA;
                    $arrCampos['no_categoria']      = ((string) $categoria->NO_CATEGORIA);
                    
                    trata_salvar($arrCampos);
                }
            }
        } catch (Exception $e) {
            # Erro 404 página not found
            if ($e->getCode() == 404) {
            }
            $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
        }
    }
    
    public function listarItemCategoria($nu_seq_categoria, $uf)
    {
        try {
            $data_created = date("c");
                
            $arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>{$this->ws_usuario}</usuario>
			<senha>{$this->ws_senha}</senha>
		</auth>
		<params>
			<nu_seq_categoria>$nu_seq_categoria</nu_seq_categoria>
			<uf>$uf</uf>
		</params>
	</body>
</request>
XML;
        
            if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
                $urlWS = 'http://172.20.64.85/webservices/wssigarp/ws/pregao';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
            }

            $xml = Fnde_Webservice_Client::CreateRequest()
                    ->setURL($urlWS)
                    ->setParams(array('xml' => $arqXml, 'method' => 'listarcategoria'))
                    ->execute();
                        
            $xmlRetorno = $xml;
            $xml = simplexml_load_string(stripslashes($xml));
            
            $result = (integer) $xml->status->result;
        
            if ($result) {
                $arrCampos = array();
                $arrCampos['stNomeTabela']      = 'par3.s_item_categoria';
                $arrCampos['arAtributosTabela'] = array('nu_seq_item', 'nu_seq_categoria', 'no_item', 'vl_item', 'nu_seq_pregao', 'sg_uf', 'nu_seq_unidade_medida', 'ds_unidade_medida');
                
                foreach ($xml->body as $categoria) {
                    $this->model->executar("delete from par3.s_item_categoria where nu_seq_item = ".(string) $categoria->NU_SEQ_ITEM." and nu_seq_categoria = ".(string) $nu_seq_categoria." and sg_uf = '".(string) $categoria->SG_UF."'");
                    $arrCampos['nu_seq_item']           = (string) $categoria->NU_SEQ_ITEM;
                    $arrCampos['nu_seq_categoria']      = (string) $nu_seq_categoria;
                    $arrCampos['no_item']               = ((string) $categoria->NO_ITEM);
                    $arrCampos['vl_item']               = (string) $categoria->VL_ITEM;
                    $arrCampos['nu_seq_pregao']         = (string) $categoria->NU_SEQ_PREGAO;
                    $arrCampos['sg_uf']                 = (string) $categoria->SG_UF;
                    $arrCampos['nu_seq_unidade_medida'] = (string) $categoria->NU_SEQ_UNIDADE_MEDIDA;
                    $arrCampos['ds_unidade_medida']     = ((string) $categoria->DS_UNIDADE_MEDIDA);
                        
                    trata_salvar($arrCampos);
                }
            }
        } catch (Exception $e) {
            # Erro 404 página not found
            if ($e->getCode() == 404) {
            }
            $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
        }
    }
}
