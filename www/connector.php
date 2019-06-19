<?php
	/* Mudar para FALSE para homologação/teste */
	$GLOBALS['USE_PRODUCTION_SERVICES'] = false;

	if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
		if (!defined("SSD_PATH")) {
			define("SSD_PATH", "./");
		}
	} else {
		if (!defined("SSD_PATH")) {
			define("SSD_PATH", APPRAIZ."/includes/SSD/");
		}
		if (!defined("PER_PATH")) {
			define("PER_PATH", APPRAIZ."www/");
		}
		
	}

//	if (!headers_sent()) {
//		session_start();
//	}

//	error_reporting(E_ALL);
	
	include_once(SSD_PATH . 'SSDWsAuth.class.php');
	include_once(SSD_PATH . 'SSDWSSignDocs.class.php');
	include_once(SSD_PATH . 'SSDWsUser.class.php');
	


	/* Escolhe os certificados de acordo com o ambiente (produção ou homologação/teste) */
	if ($GLOBALS['USE_PRODUCTION_SERVICES']) {  
		$tmpDir              = '/var/www/simec/arquivos';
		$clientCert          = SSD_PATH . 'certificate/certHmg.pem'; // certificado em formato PEM
		$privateKey          = SSD_PATH . 'certificate/chaveHmg.pem'; // chave privada do certificado em formato PEM
		$privateKeyPassword  = SSD_PATH . 'certificate/passHmg.pem'; // password CODIFICADO EM BASE64 (PEM apenas a extensão)
		$trustedCaChain      = SSD_PATH . 'certificate/chainHmg.pem'; // cadeia de certificados em formato PEM
		$marker              = ' ';
	}
	else { /* Arquivos para teste/homologação */
		$tmpDir              = '/var/www/simec/arquivos/';
/*		$clientCert          = SSD_PATH . 'certificate/PHPClientTest.pem.crt'; // certificado em formato PEM
		$privateKey          = SSD_PATH . 'certificate/PHPClientTest.pem.privatekey'; // chave privada do certificado em formato PEM
		$privateKeyPassword  = SSD_PATH . 'certificate/PHPClientTestPass.pem'; // password CODIFICADO EM BASE64 (PEM apenas a extensão)
		$trustedCaChain      = SSD_PATH . 'certificate/PHPClientTest.pem.chain'; // cadeia de certificados em formato PEM
*/		
		$clientCert          = PER_PATH . 'certificate/certSIMEC.pem'; // certificado em formato PEM
		$privateKey          = PER_PATH . 'certificate/chaveSIMEC.pem'; // chave privada do certificado em formato PEM
		$privateKeyPassword  = PER_PATH . 'certificate/passSIMEC.pem'; // password CODIFICADO EM BASE64 (PEM apenas a extensão)
		$trustedCaChain      = PER_PATH . 'certificate/chainSIMEC.pem'; // cadeia de certificados em formato PEM
		
		$marker              = ' ';	
	}
	
	
	function msgOutput($msg) {
		echo date("d/m/Y H:i:s") . "&nbsp;&nbsp;&nbsp;$msg<br />";
		ob_flush();
		flush();
	}

	/*try {
		/*$SSDWs = new SSDWsAuth($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
		$SSDWs->useHomologationSSDServices();*/

		//$SSDWs->useProductionSSDServices();
		//$b = $SSDWs->getIdentifierAppletTicket($marker);
		//$b = $SSDWs->getCertificateAppletTicket($marker);
		//$b = $SSDWs->getIdentifierAndCertificateAppletTicket($marker);

		/*if (isset($_GET['msg']))
			echo "<b>{$_GET['msg']}</b><br>";*/
		/*
	} catch (Exception $e) {
		$erro = $e->getMessage();
		//mail("raul.maia@mec.gov.br, calixto.jorge@mec.gov.br" , "Erro SGB" , "Erro : ". $erro);
		echo $erro;
	}*/
?>
