<?php

class Par3_Ws_PendenciaHabilitacao
{

	private function getDataAtual(){
		return date("c");
	}

	function consultaHabilitaEntidade($cnpj, $boMostraCodigo = false){
		$xmlRetorno = "";
		if(!$cnpj){
			return 1;
		}

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?><request><header><app>string</app><version>string</version><created>{$this->getDataAtual()}</created>
</header>
    <body>
		<params>
			<cnpj>$cnpj</cnpj>
		</params>
    </body>
</request>
XML;
		try {
			$xml = Par3_Ws_Client::CreateRequest()
				//->setURL('http://172.20.200.116/webservices/habilita/base/web/dev.php/entidade/consultar')
				->setURL('http://www.fnde.gov.br/webservices/habilita/index.php/entidade/consultar')
				->setParams( array( 'xml' => $arqXml ) )
				->execute()
			;
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));

			if ( (int) $xml->status->result ){
		        $obHabilita = $xml->body->row->children();
		        $co_situacao_habilita = (int) $obHabilita->co_situacao_habilita;
				$ds_situacao_habilita = (string) $obHabilita->ds_situacao_habilita;

				if( $boMostraCodigo ){
					$arrSituacao = array('codigo' => $co_situacao_habilita, 'descricao' => $ds_situacao_habilita );
					return simec_json_encode($arrSituacao);
				} else {
			       	if($co_situacao_habilita == 3){
			       		return ($ds_situacao_habilita);
			       	} else {
			       		$ds_situacao_habilita = ( $ds_situacao_habilita ? $ds_situacao_habilita : '1'  );
			       		return ($ds_situacao_habilita);
			       }
				}
		    } else {
				return 1;
		    }
	    } catch (Exception $e){
	    	/**
	    	 * @var LogErroWS
	    	 * Bloco que grava o erro em nossa base
	    	 */

	    	return 1;
	    }
	}

	function consultaHabilita($cnpj, $pelaAba = false){
		$xmlRetorno = "";
		if(!$cnpj){
			return 1;
		}

		$ptrid = ($_SESSION['emenda']['ptrid']==''?$_SESSION['emenda']['ptridAnalise']:$_SESSION['emenda']['ptrid']);

		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?><request><header><app>string</app><version>string</version><created>{$this->getDataAtual()}</created>
</header>
    <body>
		<params>
			<cnpj>$cnpj</cnpj>
		</params>
    </body>
</request>
XML;

		try {
			$xml = Fnde_Webservice_Client::CreateRequest()
			//->setURL('http://172.20.200.116/webservices/habilita/base/web/dev.php/entidade/consultar')
			->setURL('http://www.fnde.gov.br/webservices/habilita/index.php/entidade/consultar')
			->setParams( array( 'xml' => $arqXml ) )
			->execute()
			;
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
			if ( (int) $xml->status->result ){
				$obHabilita = $xml->body->row->children();
				$co_situacao_habilita = (int) $obHabilita->co_situacao_habilita;
				$ds_situacao_habilita = (string) $obHabilita->ds_situacao_habilita;
				# Verifica se o está habilitado
				if($pelaAba){
					if($co_situacao_habilita == 3){
						echo "<font size=\"2\">A entidade encontra-se habilitada.</font>";
					} else {
						echo "<font color=\"red\" size=\"2\">Favor entrar em contato com a Coordenação de Habilitação para Projetos Educacionais (COHAP).";
						echo "<br />";
						echo "Telefones: (61) 2022-4291 / 2022-4296</font>";
					}
				} else {
					# Verifica se a é diferente de Habilitada (3)
					if($co_situacao_habilita != 3){
						echo 'Entidade não pode ser alterada pois sua situação encontra-se em: '.$ds_situacao_habilita;
						die;
					}
				}
			} else {
				throw new Exception('A entidade não se encontra habilitada: '.($xml->status->error->message->text));
			}
		} catch (Exception $e){
			/**
			 * @var LogErroWS
			 * Bloco que grava o erro em nossa base
			 */

			$obLogErroWS = new LogErroWS();
			$erro = "XML de Envio: <br />".$arqXml;
			$obLogErroWS->montaTextoErro($erro);
			$erro = "XML de Retorno: <br />".$xmlRetorno;
			$obLogErroWS->montaTextoErro($erro);
			$erro = "Objeto Exception: <br />".$e;
			$obLogErroWS->montaTextoErro($erro);
			$obLogErroWS->ptrid 		  = $ptrid;//$_SESSION['emenda']['ptridAnalise'];
			$obLogErroWS->logtipo 		  = 'H';
			$obLogErroWS->usucpf 		  = $_SESSION['usucpf'];
			$obLogErroWS->logdatainclusao = date('Y-m-d H:i:s');
			$obLogErroWS->logerro 		  = str_replace( "'", '"', $obLogErroWS->logerro );
			$obLogErroWS->salvar();
			$obLogErroWS->commit();
			unset($obLogErroWS);


			# Erro 404 página not found
			if($e->getCode() == 404){
				echo "Erro-Serviço Habilita encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				die;
			}
			if($pelaAba){
				echo "<font color=\"red\" size=\"2\">{$e->getMessage()}</font>";
			} else {
			//echo 'Erro-Operação Habilita falhou.';
				echo 'Erro-WS Consultar Habilita: '.$e->getMessage();
				die;
			}
		}
	}
}