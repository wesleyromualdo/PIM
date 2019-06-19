<?php
/**
 * Classe de controle do  iniciativa_iniciativas_projetos
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RICARDO MACEDO RANGEL BORGES <ricardoborges@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasProjetos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasProjetos
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasProjetos($_GET['iiproid']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasProjetosHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
	public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaIniciativasProjetosHistorico::CREATE;
    	 if($arrPost['iiproid']){
    		$arrayIniciativa['iiproid'] = $arrPost['iiproid'];
    		$acao = Par3_Model_IniciativaIniciativasProjetosHistorico::UPDATE;
    	} else {
    		$arrPost['iiproid'] = null;
    	}
    	
    	$arrayIniciativaProjetos['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaProjetos['proid'] = $arrPost['proid'];
    	//$arrayIniciativaProjetos['iniid'] = 1;
    	
       	// Se existe o id da Iniciativa, deverá ser identificado o projeto que será inserido, exlcuído (inativado) ou mantido:
		if($arrayIniciativaProjetos['iniid']){
			// Irá identificar os projetos que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
			// Os projetos que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
			$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
			$iniid = $arrayIniciativaProjetos['iniid'];
			$projetosIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
			// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:			
			foreach($projetosIniciativa as $projetoExistente){
				if(in_array($projetoExistente['proid'], $arrayIniciativaProjetos['proid'])){
					// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
					if($projetoExistente['iiprostatus'] == "A"){
						array_push($separacaoAcao['naoAlterado'], $projetoExistente);
					} else if($projetoExistente['iiprostatus'] == "I"){
						array_push($separacaoAcao['ativar'], $projetoExistente);
					}
				} else {
					if($projetoExistente['iiprostatus'] == "A"){
						array_push($separacaoAcao['deleteInativar'], $projetoExistente);
					}
				}
			}
			// Verifica o que será inserido
			foreach($arrayIniciativaProjetos['proid'] as $projetoNovo){
				$existe = false;
				foreach($projetosIniciativa as $projetoExistente){
					if($projetoNovo == $projetoExistente['proid']){
						$existe = true;
					}
				}
				if(!$existe){
					array_push($separacaoAcao['insert'], $projetoNovo);
				}
			}
		}
		//print_r($separacaoAcao); exit();
       	try {       		
       		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaProjetos['iniid'] }";
       		//$this->model->executar($query_delete);
       		// "Insere" (ativa) os projetos na iniciativa
       		foreach($separacaoAcao['ativar'] as $projeto){
       			$this->model = new Par3_Model_IniciativaIniciativasProjetos($projeto['iiproid']);
       			$this->model->popularDadosObjeto(array('iiproid'=>$projeto['iiproid'], 'iiprostatus'=>"A", 'iniid'=>$projeto['iniid'], 'proid'=>$projeto['proid']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasProjetos = new Par3_Model_IniciativaIniciativasProjetos($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasProjetosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasProjetos, Par3_Model_IniciativaIniciativasProjetosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// Insere os projetos na iniciativa
       		foreach($separacaoAcao['insert'] as $projeto){
       			$this->model = new Par3_Model_IniciativaIniciativasProjetos();
       			$this->model->popularDadosObjeto(array('iiprostatus'=>"A", 'iniid'=>$arrayIniciativaProjetos['iniid'], 'proid'=>$projeto));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasProjetos = new Par3_Model_IniciativaIniciativasProjetos($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasProjetosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasProjetos, Par3_Model_IniciativaIniciativasProjetosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// "Remove" (desativa) os projetos que não existirão mais na iniciativa
       		foreach($separacaoAcao['deleteInativar'] as $projeto){
       			$this->model = new Par3_Model_IniciativaIniciativasProjetos($projeto['iiproid']);
       			$this->model->popularDadosObjeto(array('iiproid'=>$projeto['iiproid'], 'iiprostatus'=>"I", 'iniid'=>$projeto['iniid'], 'proid'=>$projeto['proid']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasProjetos = new Par3_Model_IniciativaIniciativasProjetos($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasProjetosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasProjetos, Par3_Model_IniciativaIniciativasProjetosHistorico::DELETE);
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
        $id = $_GET['iiproid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iiproid={$id}";
        $iniciativa_iniciativas_projetos = new Par3_Model_IniciativaIniciativasProjetos($id);
        try{
             $iniciativa_iniciativas_projetos->Xstatus = 'I';
             $iniciativa_iniciativas_projetos->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_projetos->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_projetos->salvar();
             $iniciativa_iniciativas_projetos->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>