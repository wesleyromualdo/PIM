<?php
/**
 * Classe de controle do  agendamentoemail
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   FELIPE TARCHIANI CERAVOLO CHIAVICATTI <felipe.chiavicatti@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-05-2018
 * @link     no link
 */



/**
 * Seguranca_Controller_Agendamentoemail
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-05-2018
 * @link     no link
 */
class Seguranca_Controller_Agendamentoemail
{
    private $model;
    private $tag;

    public function __construct()
    {
        $this->model = new Seguranca_Model_Agendamentoemail();
        
        $this->tag['data'] 			= date('d/m/Y');
        $this->tag['datahora'] 		= date('d/m/Y H\hi');
        $this->tag['hora'] 			= date('H\h');
        $this->tag['horaminuto'] 	= date('H\hi');
        $this->tag['assinatura'] 	= 'Assinatura do MEC.';
    }
    
    public function preVisualizacaoEmail($arPost){
    	if ($arPost['sql'] && $arPost['conteudo']){
    		$arPost['sql']        = str_replace(["\\'", '\\"', ";"], ["'", '"', ""], $arPost['sql']);
    		$arPost['sql']        = (stripos($arPost['sql'], 'limit') === false  ? $arPost['sql'] . ' LIMIT 1;' : $arPost['sql']);
    		$arPost['conteudo']   = str_replace(["\\'", '\\"', ";"], ["'", '"', ""], $arPost['conteudo']);
    		
    		$dados = $this->model->pegaLinha($arPost['sql']);
    		$dados = ($dados ? $dados : array());
    		$this->adicionarTagLinhaSql($dados);
    		$dados = $this->prepararSubstituicao();
    		
    		return str_replace($dados['tag'], $dados['valor'], $arPost['conteudo']);
    	}
    }
    
    public function enviarEmail($arGet){
    	$this->model->carregarPorId($arGet['aemid']);
    	if ($this->model->aemid && $this->model->aemstatus == 'A'){
    		$dadosQuery = $this->model->carregar($this->model->aemsql);
    		$dadosQuery = ($dadosQuery ? $dadosQuery : array());
    		
    		$continue = false;
    		foreach ($dadosQuery as $dado){
	    		$remetente 		= array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");
	    		$destinatario 	= array($dado['email']);
	    		
	    		if (IS_PRODUCAO == false || $_GET['emaillocal'] == 't') {
	    			$destinatario = array('felipechiavicatti@mec.gov.br', 'thiago.borboleta@mec.gov.br', 'thiagoborboleta@gmail.com');
	    		
	    			if ($continue){
	    				break;
	    			}
	    			$continue = true;
	    		}
	    			
	    		$assunto = $this->model->aemassunto;
	    		$assunto = "=?ISO-8859-1?B?" . base64_encode($assunto) . "?=";
	    		
    			$this->adicionarTagLinhaSql($dado);
    			$dadoTag  = $this->prepararSubstituicao();
    			$conteudo = str_replace($dadoTag['tag'], $dadoTag['valor'], $this->model->aemconteudo);
    			
	    		enviar_email($remetente, $destinatario, $assunto, $conteudo);
    		}
    	}
    }
    
    private function adicionarTagLinhaSql(Array $dado){
    	foreach ($dado as $indice => $valor){
    		$this->tag[$indice] = $valor; 
    	}
    }
    
    private function prepararSubstituicao(){
    	$arDado = array();
    	foreach ($this->tag as $indice => $valor){
    		$arDado['tag'][] 	= "{" . $indice . "}"; 
    		$arDado['valor'][] 	= $valor; 
    	}
    	
    	return $arDado;
    }
    
    public function pegarDadosEdicao($aemid){
    	$dados = $this->model->pegarDadosEdicao($aemid);
    	if ($dados['agsperdetalhes']){
    		$dados['agsperdetalhes'] = explode(";", $dados['agsperdetalhes']);
    		$dados['agsperdetalhes'] = array_map("trim", $dados['agsperdetalhes']);
    		
    		switch ($dados['agsperiodicidade']){
    			case 'mensal':
    				$dados['diames'] = $dados['agsperdetalhes'];
    				break;
    			case 'semanal':
    				$dados['diasemana'] = $dados['agsperdetalhes'];
    				break;
    			case 'diario':
    				$dados['horadia'] = $dados['agsperdetalhes'];
    				break;
    		}
    	}
    	
    	return $dados;
    }
    
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        
        $arDados = $_POST;
        
        $arDados['sisid']       = ($_POST['sisid'] ? $_POST['sisid'] : $_SESSION['sisid']);
        $arDados['usucpf']      = $_SESSION['usucpf'];
        $arDados['aemsql']      = str_replace(["\\'", '\\"', ";"], ["'", '"', ""], $arDados['aemsql']);
        $arDados['aemconteudo'] = str_replace(["\\'", '\\"', ";"], ["'", '"', ""], $arDados['aemconteudo']);
        
        $this->model->popularDadosObjeto($arDados);
        
        $aemid = $this->model->aemid;
        try{
            $aemid = $this->model->salvar();
            
            /*
             * INSERT NA AGENDAMENTO DE SCRIPT
             */
            $agendamentoScript = new Seguranca_Model_Agendamentoscripts();
            
            switch ($_POST['agsperiodicidade']){
            	case 'mensal':
					$agsperdetalhes = implode(";", $_POST['diames']);
            		break;
            	case 'semanal':
					$agsperdetalhes = implode(";", $_POST['diasemana']);
            		break;
            	case 'diario':
					$agsperdetalhes = implode(";", $_POST['horadia']);
            		break;
            }
            
            if ($arDados['aemid']){
				$agendamentoScript->carregarPorAemid($arDados['aemid']);
            }
            
			$arDados = array(
				'aemid' 			=> $aemid,
				'agsdescricao' 		=> $_POST['aemtitulo'],
				'agsperiodicidade' 	=> $_POST['agsperiodicidade'],
				'agsperdetalhes'	=> $agsperdetalhes,
				'agsfile'			=> 'agendamentoEnvioEmail.php?aemid='.$aemid 
			);

			$agendamentoScript->popularDadosObjeto($arDados)->salvar();
            
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        $url .= '&aemid=' . $aemid;
        
        if($aemid){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }
    
    public function atualizarStatus($arPost){
    	$this->model->carregarPorId(($arPost['aemid'] ? $arPost['aemid'] : 0));
		if ( !$this->model->aemid ){
			return false;
		}
		$this->model->aemstatus = ($arPost['status'] == 'true' ? 'A' : 'I');
		$this->model->salvar();
		
		$agendamentoScript = new Seguranca_Model_Agendamentoscripts();
		$agendamentoScript->carregarPorAemid($arPost['aemid']);
		
		if ( !$agendamentoScript->agsid ){
			return false;
		}
		$agendamentoScript->agsstatus = ($arPost['status'] == 'true' ? 'A' : 'I');
		$agendamentoScript->salvar();
		
		$this->model->commit();
		
		return true;
    }

    public function listar($arPost){
    	$sql = $this->model->montaSQLLista($arPost);
    	
    	$listagemSimec = new Simec_Listagem();
    	$arrayCabecalho = array('Ativo?','Título', 'Assunto', "Periodicidade", 'Detalhamento da periodicidade', 'Cadastrante');
    	$listagemSimec->setCabecalho($arrayCabecalho);
    	$listagemSimec->turnOnPesquisator();
    	$listagemSimec->setQuery($sql);
    	$listagemSimec->addAcao('edit', array('func' => 'editarAgendamentoEmail'/*, 'extra-params' => array('aemid')*/));
    	$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    	$listagemSimec->addCallbackDeCampo(['aemstatus'], 'Seguranca_Controller_Agendamentoemail::campoStatusListar');
    	$listagemSimec->addCallbackDeCampo(['agsperdetalhes'], 'Seguranca_Controller_Agendamentoemail::campoDetalhamentoPeriodicidade');
    	$listagemSimec->setFormFiltros('form-lista-agendamento-email');
    	$listagemSimec->setTamanhoPagina(10);
    	$listagemSimec->setCampos($arrayCabecalho);
    	$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    	 
    }
    
    public function campoStatusListar($status, $dadoLinha, $aemid){
	    switch($status){
	        case 'A':
	            return '<input type="checkbox" class="js-switch" agendamento-email-id="'.$aemid.'" checked />';;
	            break;
	        case 'I':
	            return '<input type="checkbox" class="js-switch" agendamento-email-id="'.$aemid.'" />';;
	            break;
	        default:
	            return $status;
	            break;
	    }
	}
	
    public function campoDetalhamentoPeriodicidade($status, $dadoLinha){
    	$det = explode(";",$dadoLinha['agsperdetalhes']);
    	$detalhes = array();
    	
    	switch($dadoLinha['agsperiodicidade']) {
    		case 'Mensal':
    			foreach($det as $t) {
    				$detalhes[] = $t;
    			}
    			break;
    		case 'Diário':
   				foreach($det as $t) {
   					$detalhes[] = "- ".$t."h";
   				}
    			break;
    		case 'Semanal':
   				foreach($det as $t) {
   					switch($t) {
   						case 0: $detalhes[] = "- Domingo";break;
   						case 1: $detalhes[] = "- Segunda-feira";break;
   						case 2: $detalhes[] = "- Terça-feira";break;
   						case 3: $detalhes[] = "- Quarta-feira";break;
   						case 4: $detalhes[] = "- Quinta-feira";break;
   						case 5: $detalhes[] = "- Sexta-feira";break;
   						case 6: $detalhes[] = "- Sábado";break;
   					}
   				}
    			break;
    	}
    	 
    	return implode("<br>", $detalhes);    	
	}
    
	public function criarAbaAgendamentoEmail()
	{
		list(,,$url) = explode("/", $_SERVER['SCRIPT_NAME']);
		$url .= "?modulo={$_GET['modulo']}&acao={$_GET['acao']}";
		
		$abasAg = array(
						0 => array("descricao" => "Lista de Agendamento de Email", "link" => $url),
	    				1 => array("descricao" => "Cadastro de Agendamento de Email", "link" => $url . "&menu=cadastroAgendamento"),
	        	  );
	
	    return $abasAg;
	}
	
	public function validarSql($arPost){
		if ($arPost['sql']){
			try{
				$arPost['sql'] = str_replace(["\\'", '\\"', ";"], ["'", '"', ""], $arPost['sql']);
				$arPost['sql'] = (stripos($arPost['sql'], 'limit') === false  ? $arPost['sql'] . ' LIMIT 1;' : $arPost['sql']);
				$dados = $this->model->pegaLinha($arPost['sql']);
				
				return array_keys($dados);
			}catch(Exception $e){
				return "<b>Error:</b>" . $e->getMessage();	
			}
		}else{
			return false;
		}
	}
	
   /**
     * Função excluir
     * - grava os dados
     *
     */
//     public function inativar()
//     {
//         global $url;
//         $id = $_GET['aemid'];
//         $url = "aspar.php?modulo=principal/proposicao/index&acao=A&aemid={$id}";
//         $agendamentoemail = new Seguranca_Model_Agendamentoemail($id);
//         try{
//              $agendamentoemail->Xstatus = 'I';
//              $agendamentoemail->Xdtinativacao = date('Y-m-d H:i:s');
//              $agendamentoemail->Xusucpfinativacao = $_SESSION['usucpf'];

//              $agendamentoemail->salvar();
//              $agendamentoemail->commit();
//             simec_redirecionar($url, 'success');
//         } catch (Simec_Db_Exception $e) {
//             simec_redirecionar($url, 'error');
//         }
//     }



}
?>