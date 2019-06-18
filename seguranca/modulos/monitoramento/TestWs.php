<?php
/**
 * Created by PhpStorm.
 * User: juniosantos
 * Date: 22/08/18
 * Time: 10:07
 */

include_once APPRAIZ . 'includes/classes/Fnde_Webservice_Client.class.inc';
include_once APPRAIZ . 'includes/nusoap/lib/nusoap.php';

class TestWs
{
    const CODIGO_TESTE_CONTA_CORRENTE = 'SIMEC-WS-001';
    const CODIGO_TESTE_CONTA_CORRENTE_PROCESSO = 'SIMEC-WS-002';
    const CODIGO_TESTE_EMPENHO = 'SIMEC-WS-003';
    const CODIGO_TESTE_PAGAMENTO = 'SIMEC-WS-004';
    const CODIGO_TESTE_CPF = 'SIMEC-WS-005';
    const CODIGO_TESTE_SIAFI_PF = 'SIMEC-WS-006';

    private function getXml($arrParam, $solicitacao)
    {
        $data_created = date("c");
        return <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>{$arrParam['wsusuario']}</usuario>
			<senha>{$arrParam['wssenha']}</senha>
		</auth>
		<params>
        {$solicitacao}
		</params>
	</body>
</request>
XML;
    }

    private function getMsg($xml)
    {
        $msg_text = ($xml->status->message->text);
        $msg_error_text = ($xml->status->error->message->text);
        return <<<HTML
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
    <tbody>
        <tr>
            <td align="center" style="color:#cc0000;"> {$xml->status->message->code} - {$msg_text} <br> {$msg_error_text}</td>
        </tr>
    </tbody>    
</table>
HTML;
    }

    private function executeWs($urlWS, $params)
    {
        return Fnde_Webservice_Client::CreateRequest()
            ->setURL($urlWS)
            ->setParams($params)
            ->execute();
    }

    private function getError($parans)
    {
        date_default_timezone_set('America/Sao_Paulo');
        $dataLocal = date('d/m/Y H:i:s', time());

        $e = (isset($parans['exception']) ? $parans['exception'] : null);

        if ($e instanceof Exception) {
            $erroMSG = str_replace(array(chr(13), chr(10)), ' ', $e->getMessage());
            $erroMSG = str_replace("'", '"', $erroMSG);
            $codeError = $e->getCode();
        } else {
            $codeError = $parans['code_error'];
            $erroMSG = $parans['msg_error'];
        }

        return <<<HTML
        <tr>
            <td>{$parans['cod_teste']}</td>
            <td>{$parans['sistemas_afetados']}</td>
            <td>{$parans['responsavel_tecnicos']}</td>
            <td>{$parans['descricao_teste']}</td>
            <td>{$codeError}</td>
            <td>{$erroMSG}</td>
            <td>{$dataLocal}</td>
        </tr>
HTML;
    }

    private function getDadosProgramaFnde($arrParam)
    {
        global $db;
        $sql = "select * from(  
				SELECT pronumeroprocesso as processo, probanco as banco, proagencia as agencia, muncod, protipo as tipo, trim(procnpj) as procnpj, 'PAC' as tipoprocesso FROM par.processoobra WHERE prostatus = 'A'
				union all
				SELECT pronumeroprocesso as processo, probanco as banco, proagencia as agencia, muncod, protipo as tipo, trim(procnpj) as procnpj, 'OBRA' as tipoprocesso FROM par.processoobraspar WHERE prostatus = 'A'
				union all
				SELECT prpnumeroprocesso as processo, prpbanco as banco, prpagencia as agencia, muncod, prptipo as tipo, trim(prpcnpj) as procnpj, 'PAR' as tipoprocesso FROM par.processopar WHERE prpstatus = 'A'
				union all
				SELECT pronumeroprocesso as processo, probanco as banco, proagencia as agencia, muncod, protipo as tipo, trim(procnpj) as procnpj, 'PAR3' as tipoprocesso FROM par3.processo WHERE prostatus = 'A'
			) as foo
			where processo = '{$arrParam['nu_processo']}'";
        $dadoscc = $db->pegaLinha($sql);

        $sql = "select e.empcnpj, e.empprogramafnde from par.empenho e where e.empnumeroprocesso = '{$arrParam['nu_processo']}' and empstatus = 'A' and empcodigoespecie = '01'";
        $arrDados = $db->pegaLinha($sql);

        if (!empty($arrDados['empcnpj'])) {
            $nu_identificador = $arrDados['empcnpj'];
        } else {
            $nu_identificador = $dadoscc['procnpj'];
        }

        if (!empty($arrDados['empprogramafnde'])) {
            $co_programa_fnde = $arrDados['empprogramafnde'];
        } else {
            if ($dadoscc['tipo'] == 'P') {
                $co_programa_fnde = "BW";
            } else {
                $co_programa_fnde = "CN";
            }
        }
        $nu_processo = $dadoscc['processo'];
        return ['co_programa_fnde' => $co_programa_fnde, 'nu_identificador' => $nu_identificador, 'nu_processo' => $nu_processo];
    }

    public function consultaContaCorrente($arrParam = array())
    {
        try {
            $arqXml = $this->getXml($arrParam, "<seq_solic_cr>{$arrParam['seq_conta_corrente']}</seq_solic_cr>");
            $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
            $params = array('xml' => $arqXml, 'method' => 'consultar');
            $xml = $this->executeWs($urlWS, $params);
            $xml = simplexml_load_string(stripslashes($xml));

            $result = (int)$xml->status->result;
            $code = (int)$xml->status->message->code;
            if ($result != 1 && $code == 1) {
                $erros = array('cod_teste' => self::CODIGO_TESTE_CONTA_CORRENTE, 'msg_error' => $result, 'code_error' => $code, 'descricao_teste' => 'teste ws sigef método consultar CR', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br;andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
                return $this->getError($erros);
            }
        } catch (Exception $e) {
            $erros = array('cod_teste' => self::CODIGO_TESTE_CONTA_CORRENTE, 'exception' => $e, 'descricao_teste' => 'teste ws sigef método consultar CR', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br;andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
            return $this->getError($erros);
        }
    }

    public function consultaContaCorrenteProcesso($arrParam = array())
    {
        try {
            $dadosProgramaFnde = $this->getDadosProgramaFnde($arrParam);

            $paransXml = <<<XML
        <nu_identificador>{$dadosProgramaFnde['nu_identificador']}</nu_identificador>
        <nu_processo>{$dadosProgramaFnde['nu_processo']}</nu_processo>
        <co_programa_fnde>{$dadosProgramaFnde['co_programa_fnde']}</co_programa_fnde>
XML;
            $arqXml = $this->getXml($arrParam, $paransXml);

            $urlWS = 'https://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
            $params = array('xml' => $arqXml, 'method' => 'consultarCCAtiva');
            $xml = $this->executeWs($urlWS, $params);

            $xml = simplexml_load_string(stripslashes($xml));

            $result = (int)$xml->status->result;
            $code = (int)$xml->status->message->code;
            if ($result != 1 && $code == 1) {
                $erros = array('cod_teste' => self::CODIGO_TESTE_CONTA_CORRENTE_PROCESSO, 'msg_error' => $result, 'code_error' => $code, 'descricao_teste' => 'teste ws sigef - financeiro CR método consultarCCAtiva', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br;andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
                return $this->getError($erros);
            }
        } catch (Exception $e) {
            $erros = array('cod_teste' => self::CODIGO_TESTE_CONTA_CORRENTE_PROCESSO, 'exception' => $e, 'descricao_teste' => 'teste ws sigef-financeiro CR método consultarCCAtiva', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br;andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
            return $this->getError($erros);
        }
    }

    public function consultarEmpenho($arrParam = array())
    {
        try {
            $nu_seq_ne = $arrParam['nu_seq_ne'];
            $arqXml = $this->getXml($arrParam, "<nu_seq_ne>{$nu_seq_ne}</nu_seq_ne>");

            $params = array('xml' => $arqXml, 'method' => 'consultar');
            $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
            $xml = $this->executeWs($urlWS, $params);

            $result = (int)$xml->status->result;
            $code = (int)$xml->status->message->code;
            if ($result != 1 && $code == 1) {
                $erros = array('cod_teste' => self::CODIGO_TESTE_EMPENHO, 'msg_error' => $result, 'code_error' => $code, 'descricao_teste' => 'teste ws sigef-orcamento NE método consultar', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br;andre.neto@mec.gov.br,thiago.borboleta@mec.gov.br');
                return $this->getError($erros);
            }
        } catch (Exception $e) {
            $erros = array('cod_teste' => self::CODIGO_TESTE_EMPENHO, 'exception' => $e, 'descricao_teste' => 'teste ws sigef-orcamento NE método consultar', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br,andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
            return $this->getError($erros);
        }
    }

    public function consultarPagamento($arrParam = array())
    {

        try {
            $nu_seq_ob = $arrParam['nu_seq_ob'];
            $arqXml = $this->getXml($arrParam, "<nu_seq_ob>{$nu_seq_ob}</nu_seq_ob>");

            $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
            $params = array('xml' => $arqXml, 'method' => 'consultar');
            $xml = $this->executeWs($urlWS, $params);
            $xml = simplexml_load_string(stripslashes($xml));

            $result = (int)$xml->status->result;
            $code = (int)$xml->status->message->code;
            if ($result != 1 && $code == 1) {
                $erros = array('cod_teste' => self::CODIGO_TESTE_PAGAMENTO, 'msg_error' => $result, 'code_error' => $code, 'descricao_teste' => 'teste ws sigef-financeiro OB método consultar', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br,andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
                return $this->getError($erros);
            }
        } catch (Exception $e) {
            $erros = array('cod_teste' => self::CODIGO_TESTE_PAGAMENTO, 'exception' => $e, 'descricao_teste' => 'teste ws sigef-financeiro OB método consultar', 'sistemas_afetados' => 'SIMEC módulos - Obras2, PAR e PAR2.', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br,andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
            return $this->getError($erros);
        }
    }

    public function testarCpf()
    {
        try {
            $client = new SoapClient('http://ws.mec.gov.br/PessoaFisica/wsdl');
            $result = $client->__soapCall('solicitarDadosResumidoPessoaFisicaPorCpf', array('00797370137'), array(
                    'uri' => 'urn:PessoaFisicaServices',
                    'soapaction' => ''
                )
            );

            $code = 0;
            if ($result != 1 && $code == 1) {
                $erros = array('cod_teste' => self::CODIGO_TESTE_CPF, 'msg_error' => $result, 'code_error' => $code, 'descricao_teste' => 'teste ws PessoaFisica método solicitarDadosResumidoPessoaFisicaPorCpf', 'sistemas_afetados' => 'Vários Módulos do SIMEC', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br,andre.neto@mec.gov.br;thiago.borboleta@mec.gov.br');
                return $this->getError($erros);
            }
        } catch (Exception $e) {
            $erros = array('cod_teste' => self::CODIGO_TESTE_CPF, 'exception' => $e, 'descricao_teste' => 'teste ws PessoaFisica método solicitarDadosResumidoPessoaFisicaPorCpf', 'sistemas_afetados' => 'Vários Módulos do SIMEC', 'responsavel_tecnicos' => 'juniosantos@mec.gov.br;andre.neto@mec.gov.br');
            return $this->getError($erros);
        }
    }
}