<?php
/**
 * Classe de controle do  iniciativa_iniciativas_areas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasAreas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasAreas
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasAreas($_GET['iiarid']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasAreasHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
	public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaIniciativasAreasHistorico::CREATE;
    	 if($arrPost['iiarid']){
    		$arrayIniciativa['iiarid'] = $arrPost['iiarid'];
    		$acao = Par3_Model_IniciativaIniciativasAreasHistorico::UPDATE;
    	} else {
    		$arrPost['iiarid'] = null;
    	}
    	
    	$arrayIniciativaArea['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaArea['iarid'] = $arrPost['iarid'];
    	
       	// Se existe o id da Iniciativa, deverá ser identificado a área que será inserida, exlcuída (inativada) ou mantida:
		if($arrayIniciativaArea['iniid']){
			// Irá identificar as areas que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
			// As areas que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
			$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
			$iniid = $arrayIniciativaArea['iniid'];
			$areasIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
			// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:			
			foreach($areasIniciativa as $areaExistente){
				if(in_array($areaExistente['iarid'], $arrayIniciativaArea['iarid'])){
					// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
					if($areaExistente['iiarstatus'] == "A"){
						array_push($separacaoAcao['naoAlterado'], $areaExistente);
					} else if($areaExistente['iiarstatus'] == "I"){
						array_push($separacaoAcao['ativar'], $areaExistente);
					}
				} else {
					if($areaExistente['iiarstatus'] == "A"){
						array_push($separacaoAcao['deleteInativar'], $areaExistente);
					}
				}
			}
			// Verifica o que será inserido
			foreach($arrayIniciativaArea['iarid'] as $areaNova){
				$existe = false;
				foreach($areasIniciativa as $areaExistente){
					if($areaNova == $areaExistente['iarid']){
						$existe = true;
					}
				}
				if(!$existe){
					array_push($separacaoAcao['insert'], $areaNova);
				}
			}
		}
		//print_r($separacaoAcao); exit();
       	try {       		
       		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaArea['iniid'] }";
       		//$this->model->executar($query_delete);
       		// "Insere" (ativa) os areas na iniciativa
       		foreach($separacaoAcao['ativar'] as $area){
       			$this->model = new Par3_Model_IniciativaIniciativasareas($area['iiarid']);
       			$this->model->popularDadosObjeto(array('iiarid'=>$area['iiarid'], 'iiarstatus'=>"A", 'iniid'=>$area['iniid'], 'iarid'=>$area['iarid']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasAreas = new Par3_Model_IniciativaIniciativasAreas($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasAreasHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasAreas, Par3_Model_IniciativaIniciativasAreasHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// Insere as áreas na iniciativa
       		foreach($separacaoAcao['insert'] as $area){
       			$this->model = new Par3_Model_IniciativaIniciativasAreas();
       			$this->model->popularDadosObjeto(array('iiarstatus'=>"A", 'iniid'=>$arrayIniciativaArea['iniid'], 'iarid'=>$area));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasAreas = new Par3_Model_IniciativaIniciativasAreas($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasAreasHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasAreas, Par3_Model_IniciativaIniciativasAreasHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// "Remove" (desativa) as áreas que não existirão mais na iniciativa
       		foreach($separacaoAcao['deleteInativar'] as $area){
       			$this->model = new Par3_Model_IniciativaIniciativasAreas($area['iiarid']);
       			$this->model->popularDadosObjeto(array('iiarid'=>$area['iiarid'], 'iiarstatus'=>"I", 'iniid'=>$area['iniid'], 'iarid'=>$area['iarid']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasAreas = new Par3_Model_IniciativaIniciativasAreas($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasAreasHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasAreas, Par3_Model_IniciativaIniciativasAreasHistorico::DELETE);
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
        $id = $_GET['iiarid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iiarid={$id}";
        $iniciativa_iniciativas_areas = new Par3_Model_IniciativaIniciativasAreas($id);
        try{
             $iniciativa_iniciativas_areas->Xstatus = 'I';
             $iniciativa_iniciativas_areas->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_areas->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_areas->salvar();
             $iniciativa_iniciativas_areas->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>