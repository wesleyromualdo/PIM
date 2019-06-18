<?php
/**
 * Classe de controle do  iniciativa_iniciativas_anos
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RICARDO MACEDO RANGEL BORGES <ricardoborges@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasAnos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasAnos
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasAnos($_GET['iiano']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasAnosHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {    	
        $acao = Par3_Model_IniciativaIniciativasAnosHistorico::CREATE;
    	 if($arrPost['iiano']){    	 	
    	 	$arrayIniciativa['iiano'] = $arrPost['iiano'];
    		$acao = Par3_Model_IniciativaIniciativasAnosHistorico::UPDATE;
    	} else {
    		$arrPost['iiano'] = null;
    	}
		
    	$arrayIniciativaAnos['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaAnos['iniano'] = $arrPost['iniano'];
    	// Se existe o id da Iniciativa, deverá ser identificado o ano que será inserido, exlcuído (inativado) ou mantido:
		if($arrayIniciativaAnos['iniid']){
			// Irá identificar os anos que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
			// Os anos que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
			$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
			$iniid = $arrayIniciativaAnos['iniid'];
			$anosIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
			// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:			
			foreach($anosIniciativa as $anoExistente){
				if(in_array($anoExistente['iniano'], $arrayIniciativaAnos['iniano'])){
					// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
					if($anoExistente['iianostatus'] == "A"){
						array_push($separacaoAcao['naoAlterado'], $anoExistente);
					} else if($anoExistente['iianostatus'] == "I"){
						array_push($separacaoAcao['ativar'], $anoExistente);
					}
				} else {
					if($anoExistente['iianostatus'] == "A"){
						array_push($separacaoAcao['deleteInativar'], $anoExistente);
					}
				}
			}
			// Verifica o que será inserido
			foreach($arrayIniciativaAnos['iniano'] as $anoNovo){
				$existe = false;
				foreach($anosIniciativa as $anoExistente){
					if($anoNovo == $anoExistente['iniano']){
						$existe = true;
					}
				}
				if(!$existe){
					array_push($separacaoAcao['insert'], $anoNovo);
				}
			}
		}
		//print_r($separacaoAcao); exit();
       	try {       		
       		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaAnos['iniid'] }";
       		//$this->model->executar($query_delete);
       		// "Insere" (ativa) os anos na iniciativa
       		foreach($separacaoAcao['ativar'] as $ano){
       			$this->model = new Par3_Model_IniciativaIniciativasAnos($ano['iiano']);
       			$this->model->popularDadosObjeto(array('iiano'=>$ano['iiano'], 'iianostatus'=>"A", 'iniid'=>$ano['iniid'], 'iniano'=>$ano['iniano']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasAnos = new Par3_Model_IniciativaIniciativasAnos($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasAnosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasAnos, Par3_Model_IniciativaIniciativasAnosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// Insere os anos na iniciativa
       		foreach($separacaoAcao['insert'] as $ano){
       			$this->model = new Par3_Model_IniciativaIniciativasAnos();
       			$this->model->popularDadosObjeto(array('iianostatus'=>"A", 'iniid'=>$arrayIniciativaAnos['iniid'], 'iniano'=>$ano));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasAnos = new Par3_Model_IniciativaIniciativasAnos($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasAnosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasAnos, Par3_Model_IniciativaIniciativasAnosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// "Remove" (desativa) os anos que não existirão mais na iniciativa
       		foreach($separacaoAcao['deleteInativar'] as $ano){
       			$this->model = new Par3_Model_IniciativaIniciativasAnos($ano['iiano']);
       			$this->model->popularDadosObjeto(array('iiano'=>$ano['iiano'], 'iianostatus'=>"I", 'iniid'=>$ano['iniid'], 'iniano'=>$ano['iniano']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasAnos = new Par3_Model_IniciativaIniciativasAnos($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasAnosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasAnos, Par3_Model_IniciativaIniciativasAnosHistorico::DELETE);
       			$this->modelHistorico->commit();     			
       		}      			    		
    	} catch (Simec_Db_Exception $e) {return 'erro';}
    	
    	return $id;
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['iiano'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iiano={$id}";
        $iniciativa_iniciativas_anos = new Par3_Model_IniciativaIniciativasAnos($id);
        try{
             $iniciativa_iniciativas_anos->Xstatus = 'I';
             $iniciativa_iniciativas_anos->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_anos->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_anos->salvar();
             $iniciativa_iniciativas_anos->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>