<?php
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

class Fnde_WebService_ServicoFNDE{
	private $wsusuario, $wssenha, $boExibiMSG;

	public function __construct( $arrPost = array() ){		
		$this->wsusuario 		= trim($arrPost['wsusuario']);
		$this->wssenha 			= $arrPost['wssenha'];
		$this->boExibiMSG 		= ($arrPost['boExibiMSG'] ? $arrPost['boExibiMSG'] : true);
	}
	
	public function consultaEmpenho( $nu_seq_ne ){
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
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;
	
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	
		$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
		->execute();
	
		$xmlRetorno = $xml;
	
		$xml = simplexml_load_string( stripslashes($xml));
		 ver($xml);
		$result = (integer) $xml->status->result;
		$status = ((string)$xml->body->row->status);
		$co_status			= substr( $status, 0, 1 );
		 
		if( (int)$result == 1 ){
			if( (int)$co_status == 1 ){
		   
			}
		}
	}
	
	public function historicoEmpenho($nu_processo){
	
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
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
			<nu_processo>$nu_processo</nu_processo>
		</params>
	</body>
</request>
XML;

		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'historicoempenho') )
				->execute();
	
		$xml = simplexml_load_string( stripslashes($xml));
		
		if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
			$executa = true;
			$arrXML = $xml->body->children();
			$arrXML2 = $xml->body->row->children();
			
			$arrRegistro = array();
			foreach($arrXML as $chaves => $dados ){
				$cnpj 						= trim((string)$dados->cnpj);
				$programa_fnde 				= trim((string)$dados->programa_fnde);
				$unidade_gestora 			= trim((string)$dados->unidade_gestora);
				$numero_da_proposta_siconv 	= trim((string)$dados->numero_da_proposta_siconv);
				$numero_da_ne 				= trim((string)$dados->numero_da_ne);
				$numero_de_vinculacao_ne 	= trim((string)$dados->numero_de_vinculacao_ne);
				$valor_da_ne 				= trim((string)$dados->valor_da_ne);
				$numero_sequencial_da_ne 	= trim((string)$dados->numero_sequencial_da_ne);
				$nu_seq_mov_ne 				= trim((string)$dados->nu_seq_mov_ne);
				$data_do_empenho 			= trim((string)$dados->data_do_empenho);
				$cpf 						= trim((string)$dados->cpf);
				$nu_id_sistema 				= trim((string)$dados->nu_id_sistema);
				$descricao_do_empenho 		= ( trim((string)$dados->descricao_do_empenho) );
				$ano_do_empenho 			= trim((string)$dados->ano_do_empenho);
				$centro_de_gestao 			= trim((string)$dados->centro_de_gestao);		
				$natureza_de_despesa 		= trim((string)$dados->natureza_de_despesa);
				$fonte_de_recurso 			= trim((string)$dados->fonte_de_recurso);
				$ptres 						= trim((string)$dados->ptres);
				$esfera 					= trim((string)$dados->esfera);
				$pi 						= trim((string)$dados->pi);
				$cod_especie 				= trim((string)$dados->cod_especie);
				$numero_do_processo 		= trim((string)$dados->numero_do_processo);
				$situacao_do_empenho 		= trim((string)$dados->situacao_do_empenho);
				
				if( strpos( strtolower($situacao_do_empenho), strtolower('FETIVADO') ) ){
					$situacao_do_empenho = '2 - EFETIVADO';
				}
				if( strpos( strtolower($situacao_do_empenho), strtolower('APROVADA') ) ){
					$situacao_do_empenho = '8 - SOLICITAÇÃO APROVADA';
				}
				
				array_push($arrRegistro, array(
											'cnpj' => $cnpj,
											'programa_fnde' => $programa_fnde,
											'unidade_gestora' => $unidade_gestora,
											'numero_da_proposta_siconv' => $numero_da_proposta_siconv,
											'numero_da_ne' => $numero_da_ne,
											'numero_de_vinculacao_ne' => $numero_de_vinculacao_ne,
											'valor_da_ne' => ($valor_da_ne ? number_format($valor_da_ne, 2, ',', '.') : '0'),
											'numero_sequencial_da_ne' => $numero_sequencial_da_ne,
											'nu_seq_mov_ne' => $nu_seq_mov_ne,
											'data_do_empenho' => $data_do_empenho,
											'cpf' => $cpf,
											'nu_id_sistema' => $nu_id_sistema,
											'descricao_do_empenho' => $descricao_do_empenho,
											'ano_do_empenho' => $ano_do_empenho,
											'centro_de_gestao' => $centro_de_gestao,
											'natureza_de_despesa' => $natureza_de_despesa,
											'fonte_de_recurso' => $fonte_de_recurso,
											'ptres' => $ptres,
											'esfera' => $esfera,
											'pi' => $pi,
											'cod_especie' => $cod_especie,
											'numero_do_processo' => $numero_do_processo,
											'situacao_do_empenho' => $situacao_do_empenho
										 )
							);
			}
			return $arrRegistro;
		} else {
			return $xml->status->message->code.' - '.($xml->status->message->text).' - '.($xml->status->error->message->text);
		}
	}
	
	public function historicoPagamento( $nu_processo ){
	
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
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
			<nu_processo>$nu_processo</nu_processo>
		</params>
	</body>
</request>
XML;

		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
		$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'historicopagamento') )
			->execute();
		
		$xmlRetorno = $xml;
		$xml = simplexml_load_string( stripslashes($xml));
		
		if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
			$executa = true;
			$arrXML = $xml->body->children();
			$arrXML2 = $xml->body->row->children();
			
			$arrRegistro = array();
			foreach($arrXML as $chaves => $dados ){
				$nu_parcela					= trim((string)$dados->nu_parcela);
				$an_exercicio 				= trim((string)$dados->an_exercicio);
				$vl_parcela 				= trim((string)$dados->vl_parcela);
				$nu_mes 					= trim((string)$dados->nu_mes);
				$nu_documento_siafi_ne 		= trim((string)$dados->nu_documento_siafi_ne);
				$nu_seq_mov_ne 				= trim((string)$dados->nu_seq_mov_ne);
				$ds_username_movimento 		= trim((string)$dados->ds_username_movimento);
				$ds_situacao_doc_siafi 		= trim((string)$dados->ds_situacao_doc_siafi);			
				$dt_movimento 				= trim((string)$dados->dt_movimento);			
				$nu_seq_mov_pag 			= trim((string)$dados->nu_seq_mov_pag);			
				$dt_emissao 				= trim((string)$dados->dt_emissao);
				$nu_documento_siafi 		= trim((string)$dados->nu_documento_siafi);
				$numero_de_vinculacao 		= trim((string)$dados->numero_de_vinculacao);
				
				if( strpos( strtolower($ds_situacao_doc_siafi), strtolower('FETIVADO') ) ){
					$ds_situacao_doc_siafi = '2 - EFETIVADO';
				}
				if( strpos( strtolower($ds_situacao_doc_siafi), strtolower('APROVADA') ) ){
					$ds_situacao_doc_siafi = '8 - SOLICITAÇÃO APROVADA';
				}
				if( strpos( strtolower($ds_situacao_doc_siafi), strtolower('ANCELADO') ) ){
					$ds_situacao_doc_siafi = '9 - CANCELADO';
				}
				if( strpos( strtolower($ds_situacao_doc_siafi), strtolower('UTORIZADO') ) ){
					$ds_situacao_doc_siafi = '0 - AUTORIZADO';
				}
				
				array_push($arrRegistro, array(
											'nu_parcela' 			=> $nu_parcela,
											'an_exercicio' 			=> $an_exercicio,
											'vl_parcela' 			=> $vl_parcela,
											'nu_mes' 				=> $nu_mes,
											'nu_documento_siafi_ne' => $nu_documento_siafi_ne,
											'nu_seq_mov_ne' 		=> $nu_seq_mov_ne,
											'ds_username_movimento'	=> $ds_username_movimento,
											'ds_situacao_doc_siafi' => $ds_situacao_doc_siafi,
											'dt_movimento' 			=> $dt_movimento,
											'nu_seq_mov_pag' 		=> $nu_seq_mov_pag,
											'dt_emissao' 			=> $dt_emissao,
											'nu_documento_siafi' 	=> $nu_documento_siafi,
											'numero_de_vinculacao' 	=> $numero_de_vinculacao
										 )
							);
			}
			
			return $arrRegistro;
		} else {
			return $xml->status->message->code.' - '.($xml->status->message->text).' - '.($xml->status->error->message->text);
		}
	}
	
	public function solicitarPagamentoFNDE( $arrPost = array() ){
		
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
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
			<request_id>{$arrPost['request_id']}</request_id>
			<nu_cgc_favorecido>{$arrPost['nu_cgc_favorecido']}</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>{$arrPost['nu_seq_conta_corrente_favorec']}</nu_seq_conta_corrente_favorec>
			<nu_processo>{$arrPost['nu_processo']}</nu_processo>
			<vl_custeio>{$arrPost['vl_custeio']}</vl_custeio>
			<vl_capital>{$arrPost['vl_capital']}</vl_capital>
			<an_referencia>{$arrPost['an_referencia']}</an_referencia>
			<sub_tipo_documento>{$arrPost['sub_tipo_documento']}</sub_tipo_documento>
			<nu_sistema>{$arrPost['nu_sistema']}</nu_sistema>
			<unidade_gestora>{$arrPost['unidade_gestora']}</unidade_gestora>
			<gestao>{$arrPost['gestao']}</gestao>
			<co_programa_fnde>{$arrPost['co_programa_fnde']}</co_programa_fnde>
			<detalhamento_pagamento>
				<item>
					<nu_parcela>{$arrPost['pagparcela']}</nu_parcela>
					<an_exercicio>{$arrPost['an_exercicio']}</an_exercicio>
					<vl_parcela>{$arrPost['valor']}</vl_parcela>
					<an_parcela>{$arrPost['an_referencia']}</an_parcela>
					<nu_mes>{$arrPost['nu_mes']}</nu_mes>
					<nu_documento_siafi_ne>{$arrPost['nu_documento_siafi_ne']}</nu_documento_siafi_ne>
				</item>
			</detalhamento_pagamento>
		</params>
	</body>
</request>
XML;

			if( $arrPost['requisicao'] == 'visualiza_xml' ){
				return $arqXml;
			} else {
				
				if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
				} else {
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
				}
				
				$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();			
				
				$xmlRetorno = $xml;
				$xml = simplexml_load_string( stripslashes($xml));
				$result = (integer) $xml->status->result;
				
				if($result) {
					return array('mensagem' => ( (string)$xml->status->message->text), 'erro' => 'N', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
				} else {
					$msg = (string)$xml->status->message->code." - ".(string)$xml->status->error->message->text;
					return array('mensagem' => $msg, 'erro' => 'S', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
				}
			}
		} catch (Exception $e){		
			# Erro 404 página not found
			if($e->getCode() == 404){
				$msg = "Erro-Serviço Solicitar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
				return array('mensagem' => $msg, 'erro' => 'S', 'xmlenvio' => '', 'xmlretorno' => '', 'url' => '');
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			return array('mensagem' => $erroMSG, 'erro' => 'S', 'xmlenvio' => '', 'xmlretorno' => '', 'url' => '');
		}
	}
	
	public function consultarPagamentoFNDE( $nu_seq_ob ) {	
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
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();
	
			$xmlRetorno = $xml;
		    $xml = simplexml_load_string( stripslashes($xml));
			$result = (integer) $xml->status->result;
			
			if($result) {
				$nu_seq_ob 			= (string) $xml->body->row->nu_seq_ob;
				$numero_documento 	= (string) $xml->body->row->numero_documento;
				$nu_cnpj 			= (string) $xml->body->row->nu_cnpj;
				$ds_problema 		= (string) $xml->body->row->ds_problema;
				$data_documento		= (string) $xml->body->row->data_documento;
				$valor_ob 			= (string) $xml->body->row->valor_ob;
				$situacao_documento = (string) $xml->body->row->situacao_documento;
				$processo 			= (string) $xml->body->row->processo;
				$nu_favorecido 		= (string) $xml->body->row->nu_favorecido;
				$nu_seq_ob 			= (string) $xml->body->row->nu_seq_ob;
				$status 			= (string) $xml->body->row->status;
				$co_status			= substr( $status, 0, 1 );
				
				if( trim($co_status) == 0 ){
					return array('mensagem' => iconv("UTF-8", "ISO-8859-1", $status), 'erro' => 'N', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
				} else {
					$msg = (string)$xml->status->message->code." - ".(string)$xml->status->message->text;
					return array('mensagem' => $msg, 'erro' => 'N', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
				}
			} else {
				$msg = (string)$xml->status->message->code." - ".(string)$xml->status->error->message->text;
				return array('mensagem' => $msg, 'erro' => 'S', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
			}
		} catch (Exception $e){
			# Erro 404 página not found			
			if($e->getCode() == 404){
				$msg = "Erro-Serviço Consulta Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
				return array('mensagem' => $msg, 'erro' => 'S', 'xmlenvio' => '', 'xmlretorno' => '', 'url' => '');
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
				
			return array('mensagem' => $erroMSG, 'erro' => 'S', 'xmlenvio' => '', 'xmlretorno' => '', 'url' => '');
		}
	}
	
	public function cancelarPagamentoFNDE( $nu_seq_ob ) {

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
			<usuario>{$this->wsusuario}</usuario>
			<senha>{$this->wssenha}</senha>
		</auth>
		<params>
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
					->execute();
	
			$xmlRetorno = $xml;
	
		    $xml = simplexml_load_string( stripslashes($xml));
			$result = (integer) $xml->status->result;
	
			if($result) {
				$status 			= (string) $xml->body->status;
				$co_status			= substr( $status, 0, 1 );
				
				if( trim($co_status) == 0 ){
					return array('mensagem' => iconv("UTF-8", "ISO-8859-1", $status), 'erro' => 'N', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
				} else {
					$msg = (string)$xml->status->message->code." - ".(string)$xml->status->message->text;
					return array('mensagem' => $msg, 'erro' => 'N', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
				}
			} else {	
				$msg = (string)$xml->status->message->code." - ".(string)$xml->status->error->message->text;
				return array('mensagem' => $msg, 'erro' => 'S', 'xmlenvio' => addslashes($arqXml), 'xmlretorno' => addslashes($xmlRetorno), 'url' => $urlWS);
			}
		} catch (Exception $e){
			# Erro 404 página not found
			if($e->getCode() == 404){
				$msg = "Erro-Serviço Cancelar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
				return array('mensagem' => $msg, 'erro' => 'S', 'xmlenvio' => '', 'xmlretorno' => '', 'url' => '');
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			return array('mensagem' => $erroMSG, 'erro' => 'S', 'xmlenvio' => '', 'xmlretorno' => '', 'url' => '');
		}
	}
}