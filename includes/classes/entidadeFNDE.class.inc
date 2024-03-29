<?php
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

class entidadeFNDE extends cls_banco{
	
	public function __construct(){
		parent::__construct();
	}
	
	public function __destruct(){
		if( isset($_SESSION['transacao'] ) ){
			pg_query( $this->link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}
	}
	
	public function pegaCnpj($entid = null){
		if($entid){
			return parent::pegaUm("SELECT enbcnpj FROM emenda.entidadebeneficiada WHERE enbid = ".$entid);
		}
	}
	
	public function verificaEntidadeBaseFNDE( $enbid = null, $ptrid = '' ){		
		$entnumcnpjcpf = $this->pegaCnpj( $enbid );
		
		$obEntidade  = $this->buscaEntidadeBaseFNDE_WS( $entnumcnpjcpf );
		$obDirigente = $this->buscaDirigenteBaseFNDE_WS( $entnumcnpjcpf );

		if( is_object( $obEntidade ) ){
			$obEntidade->enbid = $enbid;
			$obEntidade->entnumcnpjcpf = $entnumcnpjcpf;
			//$this->atualizaEntidadeBase( $obEntidade );
			$this->atualizaEntidadeBeneficiada( $obEntidade );
				
		}
		if( is_object( $obDirigente ) ){	
			$obDirigente->ptrid = $ptrid;
			$obDirigente->entnumcnpjcpf = $entnumcnpjcpf;
			$this->atualizaDirigenteBase( $obDirigente );
		}
		return true;
	}
	
	public function atualizaEntidadeBeneficiada( $obDados ){
		$obDados = (array)$obDados;
		
		$arRegistro = array();
		foreach ($obDados as $key => $v) {
			if( empty( $v ) ){
				$v = '';
			}
			$arRegistro[$key] = $v;
		}
		$obDados = (object)$arRegistro;
		
		$registro = array();
		if($obDados->entnumcnpjcpf) $registro[] = "enbcnpj = '".$obDados->entnumcnpjcpf."'";
		if($obDados->razao_social) $registro[] = "enbnome = '".str_ireplace( "'", "\'", ($obDados->razao_social))."'";
		if($obDados->ddd){
			$registro[] = "enbnumdddcomercial = '".substr($obDados->ddd, -2)."'";
		}
		if($obDados->telefone) $registro[] = "enbnumcomercial = '".$obDados->telefone."'";
		if($obDados->fax) $registro[] = "enbnumfax = '".$obDados->fax."'";
		if($obDados->email) $registro[] = "enbemail = '".$obDados->email."'";
		if($obDados->endereco) $registro[] = "enblog = '".str_ireplace( "'", "\'", ($obDados->endereco))."'";
		if($obDados->numero) $registro[] = "enbnum = '".$obDados->numero."'";
		if(($obDados->bairro)) $registro[] = "enbbai = '".str_ireplace( "'", "\'", ($obDados->bairro))."'";
		if($obDados->uf) $registro[] = "estuf = '".$obDados->uf."'";
		if($obDados->co_municipio_ibge) $registro[] = "muncod = '".$obDados->co_municipio_ibge."'";
		if($obDados->cep) $registro[] = "enbcep = '".$obDados->cep."'";
		
		if( !empty($registro[0]) ){
			$sql = "UPDATE emenda.entidadebeneficiada SET ".implode(', ',$registro)." WHERE enbid = $obDados->enbid";
			parent::executar( $sql );
		}
		return parent::commit();
	} 
	
	/*public function atualizaEntidadeBase( $obDados ){
		$obDados = (array)$obDados;
		
		$arRegistro = array();
		foreach ($obDados as $key => $v) {
			if( empty( $v ) ){
				$v = '';
			}
			$arRegistro[$key] = $v;
		}
		$obDados = (object)$arRegistro;
		$obDados->ddd = $this->pegaNumeroString($obDados->ddd);
		// caso exista a entidade, verifica se a mesma possui endereço
		$sql   = "SELECT endid FROM entidade.endereco WHERE entid = {$obDados->entid}";
		$endid = parent::pegaUm( $sql );
		
		if ( empty($endid) ){
			// se a entidade não possuir endereço, cria-se um para ela
			$sql = "INSERT INTO entidade.endereco (entid, endlog, endnum, endcom, endbai, estuf, muncod, endcep, endstatus)
					VALUES ( {$obDados->entid}, '".($obDados->endereco)."', '{$obDados->numero}', '{$obDados->complemento}', '".($obDados->bairro)."', '{$obDados->uf}',
							 '{$obDados->co_municipio_ibge}', '{$obDados->cep}', 'A' )";
			parent::executar( $sql );
			
		}else{
			// caso já existe o endereço, atualiza a UF com o dado informado
			$sql = "UPDATE entidade.endereco SET estuf = '{$obDados->uf}', muncod = '{$obDados->co_municipio_ibge}' WHERE endid = {$endid}";
			parent::executar( $sql );
		}
		
		$registro = array();
		if($obDados->entnumcnpjcpf) $registro[] = "entnumcpfcnpj = '".$obDados->entnumcnpjcpf."'";
		if($obDados->razao_social) $registro[] = "entnome = '".($obDados->razao_social)."'";
		if($obDados->ddd){
			$registro[] = "entnumdddcomercial = '".substr($obDados->ddd, -2)."'";
			$registro[] = "entnumdddfax = '".substr($obDados->ddd, -2)."'";
		}
		if($obDados->telefone) $registro[] = "entnumcomercial = '".$obDados->telefone."'";
		if($obDados->fax) $registro[] = "entnumfax = '".$obDados->fax."'";
		if($obDados->email) $registro[] = "entemail = '".$obDados->email."'";

		$sql = "UPDATE entidade.entidade SET ".implode(', ',$registro)." WHERE entid = $obDados->entid";
		parent::executar( $sql );
		
		//pega o id da função
		$funid = parent::pegaUm("SELECT funid FROM entidade.funcao WHERE funstatus = 'A' and fundsc ilike '%".$obDados->tipo."%'");
		if( !empty($funid) ){
			//verifica se a entidade possui alguma função 
			$bFuncao = parent::pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = $obDados->entid and fuestatus = 'A' and funid = $funid");
			if( empty($bFuncao) ){
				
			}
		}
		
		return parent::commit();
	}*/
	
	public function atualizaDirigenteBase( $obDados ){
		$obDados = (array)$obDados;
		
		$arRegistro = array();
		foreach ($obDados as $key => $v) {
			if( empty( $v ) ){
				$v = '';
			}
			$arRegistro[$key] = $v;
		}
		$obDados = (object)$arRegistro;
		
		$obDados->complemento = str_replace( "'", '"', $obDados->complemento );
			
		$obDados->ddd = $this->pegaNumeroString($obDados->ddd);
		
		$entidDirigente = parent::pegaUm("SELECT entid FROM entidade.entidade where entnumcpfcnpj = '".$obDados->cpf."'") ;
		/*if( $_SESSION['usucpf'] == '' ){
			echo '<pre>';
			print_r($entidDirigente);
		}*/
		
		if ( empty($entidDirigente) ){
			if($obDados->dt_nascimento && $obDados->dt_nascimento != '0000-00-00' && $obDados->dt_nascimento != '00/00/0000'){
				$dataNascimento = "'".formata_data_sql($obDados->dt_nascimento)."'";
			} else {
				$dataNascimento = 'NULL';
			}
			// caso não existe a entidade, cria-se a mesma e gera um endereço à ela
			$sql = "INSERT INTO entidade.entidade( entnumcpfcnpj, entnome, entsexo, entdatanasc, entnumdddcomercial, entnumcomercial, entnumdddfax, 
							entnumfax, entemail, entnumrg, entorgaoexpedidor, entstatus )
					VALUES	('{$obDados->cpf}', '".addslashes($obDados->nome)."','{$obDados->sexo}', {$dataNascimento}, '".substr($obDados->ddd, -2)."', '{$obDados->telefone}', '".substr($obDados->ddd, -2)."',
							'{$obDados->fax}', '{$obDados->email}', '{$obDados->rg}', '".addslashes(substr($obDados->orgao_emissor,0,10))."', 'A') returning entid";
			
			$entid = parent::pegaUm( $sql );

			$sql = "INSERT INTO entidade.endereco (entid, endlog, endnum, endcom, endbai, estuf, muncod, endcep, endstatus)
					VALUES ( {$entid}, '".addslashes(($obDados->endereco))."', '{$obDados->numero}', '{$obDados->complemento}', '".($obDados->bairro)."', '".pg_escape_string($obDados->uf)."',
							 '{$obDados->co_municipio_ibge}', '{$obDados->cep}', 'A' )";
			
			parent::executar( $sql );
			
			$funid = parent::pegaUm("SELECT funid FROM entidade.funcao WHERE funstatus = 'A' and fundsc ilike '%".$obDados->cargo."%'");
			
			if( !empty( $funid ) ){				
				//verifica se a entidade possui alguma função 
				$fueid = parent::pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = $entid and fuestatus = 'A' and funid = $funid");
				//caso não exista função vinculada a entidade, cria-se uma para ela
				if( empty($fueid) ){
					$sql = "INSERT INTO entidade.funcaoentidade(funid, entid, fuestatus) 
								VALUES ($funid, $entid, 'A') RETURNING fueid";					
					$fueid = parent::pegaUm( $sql );
					
					/*$sql = "INSERT INTO entidade.funentassoc(entid, fueid) VALUES ($obDados->entid, $fueid)";
					parent::executar( $sql );*/
				} else {
					/*$sql = "SELECT feaid FROM entidade.funentassoc WHERE entid = $obDados->entid and fueid = $fueid";
					$feaid = parent::pegaUm( $sql );
					//caso exista função vinculada a entidade, mas não uma função associativa de dirigente, cria-se uma para ela
					if( empty($feaid) ){
						$sql = "INSERT INTO entidade.funentassoc(entid, fueid) VALUES ($obDados->entid, $fueid)";
						parent::executar( $sql );
					}*/
				}
				$carid = parent::pegaUm("SELECT carid FROM emenda.cargos WHERE funid = ".$funid);
				/*if( $_SESSION['usucpf'] == '' ){
					echo '<pre>';
					print_r('Insert Cargo: ' . $carid);
					print_r('Ptrid: ' . $obDados->ptrid);
				}*/
				if( !empty( $carid ) && !empty($obDados->ptrid) ){
					$sql = "UPDATE emenda.planotrabalho SET carid =	".$carid.", entiddirigente = '".$entid."' WHERE ptrid = ".$obDados->ptrid;
					/*if( $_SESSION['usucpf'] == '' ){
						echo '<pre>';
						print_r($sql);
						die;
					}*/
					parent::executar( $sql );
				}
			}			
		}else{
			// caso exista a entidade, verifica se a mesma possui endereço
			$sql   = "SELECT endid FROM entidade.endereco WHERE entid = {$entidDirigente}";
			$endid = parent::pegaUm( $sql );
			
			$registro = array();
			if($obDados->cpf) $registro[] = "entnumcpfcnpj = '".$obDados->cpf."'";
			if($obDados->nome) $registro[] = "entnome = '".pg_escape_string($obDados->nome)."'";
			if($obDados->sexo) $registro[] = "entsexo = '".$obDados->sexo."'" ;
			if($obDados->dt_nascimento && $obDados->dt_nascimento != '0000-00-00' && $obDados->dt_nascimento != '00/00/0000') $registro[] = "entdatanasc = '". formata_data_sql($obDados->dt_nascimento)."'" ;
			if($obDados->ddd){
				$registro[] = "entnumdddcomercial = '".substr($obDados->ddd, -2)."'";
				$registro[] = "entnumdddfax = '".substr($obDados->ddd, -2)."'";
			}
			if($obDados->telefone) $registro[] = "entnumcomercial = '".$obDados->telefone."'";
			if($obDados->fax) $registro[] = "entnumfax = '".$obDados->fax."'";
			if($obDados->email) $registro[] = "entemail = '".pg_escape_string($obDados->email)."'";
			if($obDados->rg) $registro[] = "entnumrg = '".trim($obDados->rg)."'";
			if($obDados->orgao_emissor) $registro[] = "entorgaoexpedidor = '".trim(substr($obDados->orgao_emissor,0,10))."'";
			
			if( !empty($registro[0]) ){
				$sql = "UPDATE entidade.entidade SET ".implode(', ',$registro)." WHERE entid = $entidDirigente";
				parent::executar( $sql );
			}
			
			if ( !$endid ){				
				// se a entidade não possuir endereço, cria-se um para ela
				$sql = "INSERT INTO entidade.endereco (entid, endlog, endnum, endcom, endbai, estuf, muncod, endcep, endstatus)
						VALUES ( {$entidDirigente}, '".addslashes(($obDados->endereco))."', '".substr($obDados->numero,0,10)."', '{$obDados->complemento}', '".($obDados->bairro)."', '{$obDados->uf}',
								 '{$obDados->co_municipio_ibge}', '{$obDados->cep}', 'A' )";
				parent::executar( $sql );
				
			}else{
				$arEndereco = array();
				
				if($obDados->endereco) $arEndereco[] = "endlog = '".pg_escape_string(($obDados->endereco))."'";
				if($obDados->numero) $arEndereco[] = "endnum = '".$obDados->numero."'";
				if($obDados->complemento) $arEndereco[] = "endcom = '".$obDados->complemento."'";
				if($obDados->bairro) $arEndereco[] = "endbai = '".($obDados->bairro)."'";
				if($obDados->uf) $arEndereco[] = "estuf = '".$obDados->uf."'";
				if($obDados->co_municipio_ibge) $arEndereco[] = "muncod = '".$obDados->co_municipio_ibge."'";
				if($obDados->cep) $arEndereco[] = "endcep = '".$obDados->cep."'";
				
				if( $arEndereco[0] ){
					// caso já existe o endereço, atualiza a UF com o dado informado
					$sql = "UPDATE entidade.endereco SET ".implode(', ',$arEndereco)." WHERE endid = {$endid}";
					parent::executar( $sql );
				}
				
			}
			
			$funid = parent::pegaUm("SELECT funid FROM entidade.funcao WHERE funstatus = 'A' and fundsc ilike '%".$obDados->cargo."%'");
			
			if( !empty( $funid ) ){
				//verifica se a entidade possui alguma função 
				$fueid = parent::pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = $entidDirigente and fuestatus = 'A' and funid = $funid");
				//caso não exista função vinculada a entidade, cria-se uma para ela
				if( empty($fueid) ){
					$sql = "INSERT INTO entidade.funcaoentidade(funid, entid, fuestatus) 
								VALUES ($funid, $entidDirigente, 'A') RETURNING fueid";					
					$fueid = parent::pegaUm( $sql );
					
					/*$sql = "INSERT INTO entidade.funentassoc(entid, fueid) VALUES ($obDados->entid, $fueid)";
					parent::executar( $sql );*/
				} else {
					/*$sql = "SELECT feaid FROM entidade.funentassoc WHERE entid = $obDados->entid and fueid = $fueid";
					$feaid = parent::pegaUm( $sql );
					//caso exista função vinculada a entidade, mas não uma função associativa de dirigente, cria-se uma para ela
					if( empty($feaid) ){
						$sql = "INSERT INTO entidade.funentassoc(entid, fueid) VALUES ($obDados->entid, $fueid)";
						parent::executar( $sql );
					}*/
				}
				$carid = parent::pegaUm("SELECT carid FROM emenda.cargos WHERE funid = ".$funid);
				/*if( $_SESSION['usucpf'] == '' ){
					echo '<pre>';
					print_r('Cargo: ' . $carid);
					print_r('Ptrid: ' . $obDados->ptrid);
				}*/
			
				if( !empty( $carid ) && !empty($obDados->ptrid) ){
					$sql = "UPDATE emenda.planotrabalho SET carid =	".$carid.", entiddirigente = '".$entidDirigente."' WHERE ptrid = ".$obDados->ptrid;
					/*if( $_SESSION['usucpf'] == '' ){
						echo '<pre>';
						print_r($sql);
						die;
					}*/
					parent::executar( $sql );
				}				
			}			
		}
		return parent::commit();
	}

	public function pegaNumeroString($texto){
		$arNumero = array('0','1','2','3','4','5','6','7','8','9');
		$string = '';
		for($i=0; $i<strlen($texto); $i++){
			if( in_array( $texto{$i}, $arNumero ) ){
				$string = $string.$texto{$i};
			}
		}
		return $string;
	}
	
	public function buscaEntidadeBaseFNDE_WS( $entnumcnpjcpf = '' ){
		
	    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request><header><app>string</app><version>string</version><created>{$this->getDataAtual()}</created>
</header>
	<body>
	<params>
			<cnpj>$entnumcnpjcpf</cnpj>
	</params>
	</body>
</request>
XML;

		/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = "http://172.20.200.116/webservices/corp/integracao/web/dev.php/entidade/consultarEntidade";						
		} else {*/
			$urlWS = "http://www.fnde.gov.br/webservices/corp/index.php/entidade/consultarEntidade";
		//}
		
		try{
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml) )
					->execute();
			$xmlRetorno = $xml;
			
			if(strpos($xml, '</response>')){
				$xml = simplexml_load_string( stripslashes($xml));
				
			    if ( (int) $xml->status->result ){	    	
			         return $xml->body->children();			    
			    } else {
			    	throw new Exception(($xml->status->error->message->text));
			    }
			}
		} catch (Exception $e){			
			return 'Erro-WS Consultar Entidade: '.$e->getMessage().'<br>';
		}
	}
	public function buscaDirigenteBaseFNDE_WS( $entnumcnpjcpf = '' ){
		
	    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request><header><app>string</app><version>string</version><created>{$this->getDataAtual()}</created>
</header>
	<body>
	<params>
			<cnpj>$entnumcnpjcpf</cnpj>
	</params>
	</body>
</request>
XML;

		/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = "http://172.20.200.116/webservices/corp/integracao/web/dev.php/entidade/consultarDirigente";						
		} else {*/
			$urlWS = "http://www.fnde.gov.br/webservices/corp/index.php/entidade/consultarDirigente";
		//}
		try{
			$xml = Fnde_Webservice_Client::CreateRequest()
						->setURL($urlWS)
						->setParams( array('xml' => $arqXml) )
						->execute();
				$xmlRetorno = $xml;
			if(strpos($xml, '</response>')){
				$xml = simplexml_load_string( stripslashes($xml));
		
			    if ( (int) $xml->status->result ){		    	
			         return $xml->body->children();			    
			    } else {
			    	throw new Exception(($xml->status->error->message->text));
			    }
			}
		} catch (Exception $e){
			return 'Erro-WS Consultar Dirigente: '.$e->getMessage();
		}		
	}
	
	private function getDataAtual(){
		return date("c");
	}
}
?>