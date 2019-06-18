<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'includes/workflow.php';

class Par3_Model_EscolasNovoEnsinoMedio extends Modelo
{
    protected $stNomeTabela = 'par3.nem_escolas';
    
    protected $dbPDDE = '';

    protected $arChavePrimaria = array(
        'nesid'
    );
        
    protected $arAtributos = array(
        'codinep' => null,
        'muncod' => null,
        'estuf' => null,
        'eseid' => null,
        'netid' => null,
        'neiid' => null,
        'nes_descricao' => null,
        'nes_matriculas' => null,
        'nes_tipo_sorteio_cod' => null,
        'nes_tipo_sorteio' => null,
        'nes_carga_horaria_anual' => null,
        'nes_motivo_inaptidao' => null,
    );

    public function antesSalvar()
    {
        return parent::antesSalvar();
    }
    
    public function verificaEscolaVinculada($dados)
    {
        $sql = "SELECT true FROM par3.nem_escolas_escolhidas WHERE adpid = {$dados['adpid']} AND nesid = {$dados['nesid']}";
        
        return $this->pegaUm($sql) == 't';
    }
    
    public function excluirEscolaVinculada($dados)
    {
        $sql = "DELETE FROM par3.nem_escolas_escolhidas WHERE adpid = {$dados['adpid']} AND nesid = {$dados['nesid']}";
        $this->executar($sql);
        $this->commit();
    }
    
    public function vincularEscola($dados)
    {
        $sql = "INSERT INTO par3.nem_escolas_escolhidas(adpid, nesid, usucpf) 
                VALUES ({$dados['adpid']}, {$dados['nesid']}, {$_SESSION['usucpf']});";
        $this->executar($sql);
        $this->commit();
    }
    
    public function buscaQTDEscolas($dadosSobreEscola)
    {
        $sql = $this->buscaSQLListaEscolas($dadosSobreEscola);
        
        $sql = "SELECT count(*) FROM ($sql) as foo";

        return $this->pegaUm($sql);
    }
    
    public function enviarEscolas($dados)
    {
        $mdInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $where[] = "nes.estuf = '{$mdInstrumentoUnidade->estuf}'";
        
        $sql = "SELECT 
                    codinep, muncod, estuf, eseid, netid, neiid, nes_descricao, nes_matriculas,
                    nes_tipo_sorteio_cod, nes_tipo_sorteio, nes_carga_horaria_anual, nes_motivo_inaptidao 
                FROM par3.nem_escolas WHERE eseid = 1 AND estuf = '{$mdInstrumentoUnidade->estuf}'
                UNION ALL
                SELECT 
                    codinep, muncod, estuf, eseid, netid, neiid, nes_descricao, nes_matriculas,
                    nes_tipo_sorteio_cod, nes_tipo_sorteio, nes_carga_horaria_anual, nes_motivo_inaptidao 
                FROM par3.nem_escolas nes
                INNER JOIN par3.nem_escolas_escolhidas  nee ON nee.nesid = nes.nesid AND nee.adpid = {$dados['adpid']}
                WHERE 
                    nes.eseid = 2 AND
                    nes.estuf = '{$mdInstrumentoUnidade->estuf}'";
        
        $arrEscolasSelecionadas = $this->carregar($sql);

        $arrValues      = array();
        $arrEnviadas    = array();
        foreach($arrEscolasSelecionadas as $escola){
            
            $arrEnviadas[] = $escola['codinep'];
            
            $testa = $this->atualizaSituacaoEscolaPDDE($escola);
            
            if($testa){
                
                $escola['netid']                    = $escola['netid'] ? $escola['netid'] : 'NULL';
                $escola['neiid']                    = $escola['neiid'] ? $escola['neiid'] : 'NULL';
                $escola['nes_matriculas']           = $escola['nes_matriculas'] ? $escola['nes_matriculas'] : 'NULL';
                $escola['nes_tipo_sorteio_cod']     = $escola['nes_tipo_sorteio_cod'] ? $escola['nes_tipo_sorteio_cod'] : 'NULL';
                $escola['nes_carga_horaria_anual']  = $escola['nes_carga_horaria_anual'] ? $escola['nes_carga_horaria_anual'] : 'NULL';
                
                $arrValues[] = "('{$escola['codinep']}', '{$escola['muncod']}', '{$escola['estuf']}', {$escola['eseid']}, {$escola['netid']},
                                {$escola['neiid']}, '{$escola['nes_descricao']}', {$escola['nes_matriculas']}, {$escola['nes_tipo_sorteio_cod']},
                                '{$escola['nes_tipo_sorteio']}', '{$escola['nes_carga_horaria_anual']}', '{$escola['nes_motivo_inaptidao']}',
                                '{$_SESSION['usucpf']}')";
            }
        }
        
        if($arrValues[0] != ''){
            $sql = "INSERT INTO nem.escolas_temp(codinep, muncod, estuf, eseid, netid, neiid, nes_descricao, nes_matriculas,
                                                nes_tipo_sorteio_cod, nes_tipo_sorteio, nes_carga_horaria_anual, nes_motivo_inaptidao,
                                                usucpf)
                    VALUES ".implode(', ', $arrValues).";";
            
            adapterConnection::pddeinterativo()->executar($sql);
            adapterConnection::pddeinterativo()->commit();
        }
        
        $sql = "SELECT codinep, 3 as eseid
                FROM par3.nem_escolas
                WHERE estuf = '{$mdInstrumentoUnidade->estuf}' AND codinep NOT IN ('".implode("', '", $arrEnviadas)."')";
        
        $escolasNaoAptas = $this->carregar($sql);
        
        foreach($escolasNaoAptas as $escola){
            
            $this->atualizaSituacaoEscolaPDDE($escola);
        }
    }
    
    public function atualizaSituacaoEscolaPDDE($dados)
    {
        $sql = "SELECT * FROM nem.escolas WHERE codinep = '{$dados['codinep']}'";
        
        $escola = adapterConnection::pddeinterativo()->pegaLinha($sql);
        
        if(!$escola['docid']){
            
            $escola['esdid'] = 638; /*Rede não confirmou no PAR*/
            
            $sql = "INSERT INTO workflow.documento(tpdid, esdid, docdsc)
            		VALUES (109, {$escola['esdid']}, 'Documento escola Novo Ensino Médio')
            		RETURNING docid";
            
            $escola['docid'] = adapterConnection::pddeinterativo()->pegaUm($sql);
            adapterConnection::pddeinterativo()->commit();
            
            $sql = "UPDATE nem.escolas SET docid = {$escola['docid']} WHERE codinep = '{$escola['codinep']}'";
            
            adapterConnection::pddeinterativo()->executar($sql);
            adapterConnection::pddeinterativo()->commit();
        }else{
            
            $sql = "SELECT esdid FROM workflow.documento WHERE docid = {$escola['docid']};";
            
            $escola['esdid'] = adapterConnection::pddeinterativo()->pegaUm($sql);
        }
        
        if($escola['esdid'] == 638){
            
            switch($dados['eseid']){
                case 1: /*Sorteado*/
                    $aedid = 1554;
                    $esdid = 641;
                    break;
                case 2: /*Selecionada pelo PAR*/
                    $aedid = 1553;
                    $esdid = 640;
                    break;
                case 3: /*Não Apta*/
                    $aedid = 1555;
                    $esdid = 642;
                    break;
            }
            
            $sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, htddata)
            		VALUES ( $aedid, {$escola['docid']}, '{$_SESSION['usucpf']}', now() )
            		RETURNING hstid";
            
            $hstid = adapterConnection::pddeinterativo()->pegaUm($sql);
            adapterConnection::pddeinterativo()->commit();
            
            $sql = "UPDATE workflow.documento
            		SET esdid = $esdid, hstid = $hstid
            		WHERE docid = {$escola['docid']}";
            
            adapterConnection::pddeinterativo()->executar($sql);
            adapterConnection::pddeinterativo()->commit();
            
            return true;
        }else{
            
            return false;
        }
    }

    public function buscaSQLListaEscolas($dados)
    {
        $where = array("eseid = {$dados['situacao']}");
        
        if($dados['inuid']){
            $mdInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
            $where[] = "nes.estuf = '{$mdInstrumentoUnidade->estuf}'";
        }
        
        if($dados['estuf'])                 $where[] = "nes.estuf = '{$dados['estuf']}'";
        if($dados['muncod'])                $where[] = "nes.muncod = '{$dados['muncod']}'";
        if($dados['netid'])                 $where[] = "nes.netid = {$dados['netid']}";
        if($dados['codinep'])               $where[] = "nes.codinep = '{$dados['codinep']}'";
        if($dados['nes_descricao'])         $where[] = "nes.nes_descricao ilike '%{$dados['nes_descricao']}%'";
        if($dados['nes_tipo_sorteio_cod'])  $where[] = "nes.nes_tipo_sorteio_cod = {$dados['nes_tipo_sorteio_cod']}";
        if($dados['selecionadas'] == 'S')   $where[] = "nee.nesid IS NOT NULL";
        if($dados['selecionadas'] == 'N')   $where[] = "nee.nesid IS NULL";
        
        $dados['adpid'] = $dados['adpid'] ? $dados['adpid'] : '0';
        
        if(!$dados['somenteLeitura']){
            $colunaChk = "
                    '<input type=checkbox class=\"checkbox-primary seleciona\" '||
                	'nesid='|| nes.nesid ||' '||
                	'adpid={$dados['adpid']} '||
                	CASE 
                		WHEN nee.nesid IS NOT NULL THEN 'checked'
                		ELSE ''
                	END
                	||' \>' as acao,";
            $lerftChk = "LEFT  JOIN par3.nem_escolas_escolhidas  nee ON nee.nesid = nes.nesid AND nee.adpid = {$dados['adpid']}";
        }else if($dados['selecionadas'] != ''){
            $lerftChk = "LEFT  JOIN par3.nem_escolas_escolhidas  nee ON nee.nesid = nes.nesid AND nee.adpid = {$dados['adpid']}";
        }
        
        $colunas    = " nes.estuf,
                        nes.muncod,
                    	mun.mundescricao,
                        nes.codinep,
                        nes.nes_descricao";
        
        switch($dados['situacao']){
            case 1:
                $colunas    .= ",
                            	nes.nes_tipo_sorteio";
                $inner      = "";
                break;
            case 2:
                $colunas    .= ",
                                nes.nes_carga_horaria_anual,
                            	nes.nes_matriculas,
                            	nei.nei_descricao,
                            	net.net_descricao";
                $inner      = "LEFT JOIN par3.nem_escolas_tipo  	net ON net.netid = nes.netid";
                break;
            case 3:
                $colunas    .= ",
                            	nes.nes_motivo_inaptidao";
                $inner      = "LEFT JOIN par3.nem_escolas_tipo  	net ON net.netid = nes.netid";
                break;
        }
        
        $sql = "SELECT 
                	$colunaChk
                	$colunas
                FROM par3.nem_escolas nes
                INNER JOIN territorios.municipio 	mun ON mun.muncod = nes.muncod
                LEFT JOIN par3.nem_escolas_inse	nei ON nei.neiid = nes.neiid
                $inner
                $lerftChk
                WHERE 
                	".implode(' AND ', $where)."
                ORDER BY
                    mundescricao,
                    nes_descricao";
                
        return $sql;
    }
}
