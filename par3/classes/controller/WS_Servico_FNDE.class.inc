<?php
require_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';
require_once APPRAIZ.'par3/classes/model/WS_Servico_FNDE.class.inc';

class Par3_Controller_WS_Servico_FNDE extends Controle
{
	private $ws_usuario, $ws_senha;
	
	public function __construct( $ws_usuario, $ws_senha )
	{
		$this->model = new Par3_Model_WS_Servico_FNDE();
		$this->ws_usuario = $ws_usuario;
		$this->ws_senha = $ws_senha;
	}
	
	public function consultaContaCorrenteProcessoFNDE( $proid, $boGravaLog = true ){
		global $db;
		
		$arrParam = $this->dadosContaCorrente($proid);
		
		$nu_identificador 		= $arrParam['nu_cnpj_favorecido'];
		$ptrnumprocessoempenho 	= $arrParam['nu_processo'];
		$codigo 				= $arrParam['proid'];
		$somente_conta_ativa	= "S";
		$numero_de_linhas		= "200";
		$date = date("c");
		
		if( $boGravaLog ){
			$obProcesso = new ProcessoFNDE( $this->ws_usuario, $this->ws_senha, false );
		}
	
		try {
			$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$date}</created>
	</header>
	<body>
		<auth>
			<usuario>$this->ws_usuario</usuario>
			<senha>$this->ws_senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_identificador</nu_identificador>
			<nu_processo>$ptrnumprocessoempenho</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;
	
			/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'https://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr/';
			} else {*/
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
			//}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
			->execute();
	
			$xmlRetorno = $xml;
	
			$xml = simplexml_load_string( $xml);
			
			if ( (int) $xml->status->result ){
				$obContaCorrenteWS  = $xml->body->row->children();
				$status				= (string)	$obContaCorrenteWS->status;
				$co_status			= substr( $status, 0, 1 );
	
				if( (int)trim($co_status) != 0 ){
					if( $boGravaLog ) $obProcesso->WSHistoricoProcesso($arqXml, $xmlRetorno, 'GERAPROC - AtualizarContaCorrenteFNDE - Sucesso', $proid, ($xml->status->message->text) );
					$obContaCorrenteWS->fase_solicitacao = ($obContaCorrenteWS->fase_solicitacao);
					$obContaCorrenteWS->ds_razao_social = ($obContaCorrenteWS->ds_razao_social);
					
					return $obContaCorrenteWS;
				} else {
					if( $boGravaLog ) $obProcesso->WSHistoricoProcesso( $arqXml, $xmlRetorno, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro', $proid, $status );
					return $status;
					die;
				}
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',($xml->status->error->message->text));
				$erroMSG = str_replace( "'", '"', $erroMSG );
				if( $boGravaLog ) $obProcesso->WSHistoricoProcesso( $arqXml, $xml, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro', $proid, $erroMSG );
				return $erroMSG;
				die;
			}
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erroMSG = "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
			}
			if( $boGravaLog ) $obProcesso->WSHistoricoProcesso( $arqXml, $xml, 'GERAPROC - AtualizarContaCorrenteFNDE - Erro', $proid, $erroMSG );
		}
	}
	
	public function solicitaAberturaContaCorrente( $proid, $boGravaLog = true ){
		global $db;
	
		$arrParam = $this->dadosContaCorrente($proid);
		
		$usuario 			= trim($arrParam['ws_usuario']);
		$senha   			= $arrParam['ws_senha'];
		$co_programa_fnde 	= $arrParam['co_programa_fnde'];
		$nu_sistema			= $arrParam['nu_sistema'];
		$codigo				= $arrParam['proid'];
	
		$agencia = substr(trim($arrParam['nu_agencia']),0,4);
	
		$nu_identificador 	= $arrParam['nu_cnpj_favorecido'];
		$tp_identificador 	= '1';
		$nu_processo 		= $arrParam['nu_processo'];
		$nu_banco 			= $arrParam['nu_banco'];
		$nu_agencia 		= $agencia;
		$nu_conta_corrente 	= null;
		$tp_solicitacao 	= '01';
		$motivo_solicitacao	= '0032';
		$convenio_bb 		= null;
		$tp_conta 			= 'N';
		$date 				= date("c");
		
		if( $boGravaLog ){
			$obProcesso = new ProcessoFNDE( $this->ws_usuario, $this->ws_senha, false );
		}
	
		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$date}</created>
	</header>
	<body>
		<auth>
			<usuario>{$this->ws_usuario}</usuario>
			<senha>{$this->ws_senha}</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;
	
		try {
			/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'https://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr/';
			} else {*/
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
			//}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
			->execute();
			
//			ver(simec_htmlentities($arqXml), simec_htmlentities($xml),d);
			
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
			
			if ( (int) $xml->status->result ){
				$arrRetorno = array('seq_solic_cr' => (string)$xml->body->seq_solic_cr, 'nu_seq_conta' => (string)$xml->body->nu_seq_conta);
	
				if( $boGravaLog ) $obProcesso->WSHistoricoProcesso($arqXml, $xmlRetorno, 'GERAPROC - SolicitarContaCorrente - Sucesso', $proid, str_replace('"', "", ($xml->status->message->text)) );
				return $arrRetorno;
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',($xml->status->error->message->text));
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				if( $boGravaLog ) $obProcesso->WSHistoricoProcesso($arqXml, $xmlRetorno, 'GERAPROC - SolicitarContaCorrente - Erro', $proid, str_replace('"', "", $erroMSG) );
	
				return $erroMSG;
				die;
			}
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				$erro = "Erro-Serviço Solicitar Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				return $erro;
				die;
			}
	
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			$erroMSG = str_replace( '"', '', $erroMSG );
	
			if( $boGravaLog ) $obProcesso->WSHistoricoProcesso($arqXml, $xml, 'GERAPROC - SolicitarContaCorrente - Erro', $proid, $erroMSG );
	
			return $erroMSG;
			die;
		}
	}
	
	public function consultarContaCorrente($dados) {
	
		try {
			$data_created = date("c");
			$usuario = $dados['wsusuario'];
			//$usuario = 'MECTIAGOT';
			$senha   = $dados['wssenha'];
			//$senha   = 'M3135689';
			
			if(!$_SESSION['par_var']['proid']) {
				echo "------ MENSAGEM SIMEC - PROCESSO NÃO ENCONTRADO ------<br>";
				echo "Foram encontrados alguns problemas internos. Feche a tela e clique novamente na lista de processo.";
				exit;
			}
			
	        $proseqconta = $db->pegaUm("SELECT seq_conta_corrente FROM par.processoobraspar WHERE prostatus = 'A' and proid='".$_SESSION['par_var']['proid']."'");
	
		if(!$proseqconta) {
			$existeAndamentoConta = consultarAndamentoContaCorrente($dados);
			if(!$existeAndamentoConta){
			 // RETORNO FALSE - SE NÃO EXISTE CONTA  EM ANDAMENTO PARA SER ABERTA SOLICITA CONTA.
				$r = solicitarContaCorrente($dados);
				if($r){
					return "cc_criado_sucesso";
		       	}else{
		       		return false;
		       	}
			}
	     }

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
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$proseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
			}
	
			if($proseqconta) {
	
				$xml = Fnde_Webservice_Client::CreateRequest()
						->setURL($urlWS)
						->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
						->execute();
						
				$xmlRetorno = $xml;
			    $xml = simplexml_load_string( stripslashes($xml));
	
			    $result = (integer) $xml->status->result;
			    $resultConsultaConta = (integer) $xml->body->row->co_situacao_conta;
	
			    $sql = "INSERT INTO par.historicowsprocessoobrapar(
						    	proid,
						    	hwpwebservice,
						    	hwpxmlenvio,
						    	hwpxmlretorno,
						    	hwpdataenvio,
						        usucpf)
						    VALUES ('".$_SESSION['par_var']['proid']."',
						    		'consultarContaCorrente',
						    		'".addslashes($arqXml)."',
						    		'".addslashes($xmlRetorno)."',
						    		NOW(),
						            '".$_SESSION['usucpf']."');";
	
				$db->executar($sql);
				$db->commit();
	
				if($result != 1 ) { // 1 = sucesso
					
					echo mensagem('CONSULTA DE CONTA CORRENTE', $xml->status->error->message->text);
	
			    	if( $resultConsultaConta == 24 ){
			    		$r = solicitarContaCorrente($dados);
			    		if($r){
							return "cc_criado_sucesso";
				       	}else{
				       		return false;
				       	}
			    		//die("MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.");
				    } elseif( $resultConsultaConta == 25 ){
				    	$r = solicitarContaCorrente($dados);
				    	if($r){
							return "cc_criado_sucesso";
				       	}else{
				       		return false;
				       	}
				    	//die("MSG SIMEC : Conta Corrente Bloqueada Definitivamente.");
				    } elseif( $resultConsultaConta == 14 ){
				    	$r = solicitarContaCorrente($dados);
				    	if($r){
							return "cc_criado_sucesso";
				       	}else{
				       		return false;
				       	}
				    	//die("MSG SIMEC : Conta Corrente Inativa.");
				    }
	
			    	return false;
			    } else {
			    	$statusContaSucessos = array('13','11','09');
					if(in_array($resultConsultaConta, $statusContaSucessos )){
					    if($xml->body->row->seq_conta) {
					    	$db->executar("UPDATE par.processoobraspar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_solic_cr."' WHERE proid='".$_SESSION['par_var']['proid']."'");
					    	$db->commit();
					    }
	
						$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
						$msg.= "*** Detalhes da consulta ***<br>";
						$msg.= "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."<br>";
						$msg.= "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."<br>";
						$msg.= "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")<br>";
						//return $result;
						
						echo mensagem('CONSULTA DE CONTA CORRENTE', $msg);
	
						return true;
					}else{
						$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
						$msg.= "* A conta corrente não está ativa.<br>";
	
				    	if( $resultConsultaConta == 24 ){
				    		$msg.= "MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.";
					    } elseif( $resultConsultaConta == 25 ){
					    	$msg.= "MSG SIMEC : Conta Corrente Bloqueada Definitivamente.";
					    } elseif( $resultConsultaConta == 14 ){
					    	$msg.= "MSG SIMEC : Conta Corrente Inativa.";
					    }
					    echo mensagem('ERRO AO CONSULTAR CONTA CORRENTE', $msg);
						return false;
					}
			    }
	
			} else {
				$msg = iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."<br>";
				$msg.= "*** Erro de integração entre SIMEC e SIGEF ***<br>";
				$msg.= "* Descrição do Erro:O sequencial da conta no SIMEC não foi encontrado.<br>";
	
		    	if( $resultConsultaConta == 24 ){
		    		$msg.= "MSG SIMEC : Conta Corrente Bloqueada Provisoriamente.";
			    } elseif( $resultConsultaConta == 25 ){
			    	$msg.= "MSG SIMEC : Conta Corrente Bloqueada Definitivamente.";
			    } elseif( $resultConsultaConta == 14 ){
			    	$msg.= "MSG SIMEC : Conta Corrente Inativa.";
			    }
				echo mensagem('CONSULTAR CONTA CORRENTE', $msg);
				return false;
			}
	
	
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'<br>';
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			echo mensagem('CONSULTAR CONTA CORRENTE', $erroMSG);
		}
	}
	
	public function dadosContaCorrente( $proid  ){
		global $db;
		
		$sql = "SELECT inuid, procnpj, probanco as banco, proagencia as agencia, pronumeroprocesso as processo, seq_conta_corrente, proseqconta, nu_conta_corrente FROM par3.processo WHERE proid = {$proid} AND prostatus = 'A'";
		$arrParam = $db->pegaLinha( $sql );
		
		$sql = "SELECT sisid, tprid, tipprogramafnde, tipnumerosistemasigef, tiptipoprocessosigef FROM execucaofinanceira.tipoprocesso where tipid = 12 and tipstatus = 'A'";
		$arTipProcesso = $db->pegaLinha($sql);
		
		/*$sql = "select itrid, muncod from par3.instrumentounidade where inuid = {$arrParam['inuid']}";
		$arInstrumento = $db->pegaLinha($sql);
		$itrid = $arInstrumento['itrid'];
		$muncod = $arInstrumento['muncod'];
		
		if( (int)$itrid == 1 || (int)$itrid == 3 ){ #estadual
			$muncod = 'null';
			$entcnpj = $db->pegaUm("select entcnpj from par3.instrumentounidade_entidade e where e.inuid = {$arrParam['inuid']} and tenid = 3 and entstatus = 'A' and entcnpj is not null");
		} else { #municipal
			$entcnpj = $db->pegaUm("select entcnpj from par3.instrumentounidade_entidade e where e.inuid = {$arrParam['inuid']} and tenid = 1 and entstatus = 'A' and entcnpj is not null");
		}*/
		
		$arrParamProcesso = array(
				'an_processo' 			=> date("Y"),
				'nu_processo' 			=> $arrParam['processo'],
				'tp_processo' 			=> $arTipProcesso['tiptipoprocessosigef'],
				'co_programa_fnde' 		=> $arTipProcesso['tipprogramafnde'],
				'nu_sistema'			=> $arTipProcesso['tipnumerosistemasigef'],
				'nu_cnpj_favorecido' 	=> $arrParam['procnpj'],
				'nu_banco' 				=> $arrParam['banco'],
				'nu_agencia'			=> $arrParam['agencia'],
		);
		return $arrParamProcesso;
	}
	
	public function mensagem( $title, $msg ){
		$html = '<div style=" border: 1px solid #B7B7B7; font-size: 11px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					'.$title.'
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >'.$msg.'</div>
				</div>
			 	<br>';
		return $html;
	}
}