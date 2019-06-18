<?php
/**
 * Classe de controle do  iniciativa_iniciativas_estados
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RICARDO MACEDO RANGEL BORGES <ricardoborges@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasEstados
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasEstados
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasEstados($_GET['iiestufid']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasEstadosHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
	public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaIniciativasEstadosHistorico::CREATE;
    	 if($arrPost['iiestufid']){
    		$arrayIniciativa['iiestufid'] = $arrPost['iiestufid'];
    		$acao = Par3_Model_IniciativaIniciativasEstadosHistorico::UPDATE;
    	} else {
    		$arrPost['iiestufid'] = null;
    	}
    	
    	$arrayIniciativaEstados['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaEstados['estuf'] = $arrPost['estuf'];
    	
       	// Se existe o id da Iniciativa, deverá ser identificado o estado que será inserido, exlcuído (inativado) ou mantido:
		if($arrayIniciativaEstados['iniid']){
			// Irá identificar os estados que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
			// Os estados que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
			$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
			$iniid = $arrayIniciativaEstados['iniid'];
			$estadosIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
			// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:			
			foreach($estadosIniciativa as $estadoExistente){
				if(in_array($estadoExistente['estuf'], $arrayIniciativaEstados['estuf'])){
					// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
					if($estadoExistente['iiestufstatus'] == "A"){
						array_push($separacaoAcao['naoAlterado'], $estadoExistente);
					} else if($estadoExistente['iiestufstatus'] == "I"){
						array_push($separacaoAcao['ativar'], $estadoExistente);
					}
				} else {
					if($estadoExistente['iiestufstatus'] == "A"){
						array_push($separacaoAcao['deleteInativar'], $estadoExistente);
					}
				}
			}
			// Verifica o que será inserido
			foreach($arrayIniciativaEstados['estuf'] as $estadoNovo){
				$existe = false;
				foreach($estadosIniciativa as $estadoExistente){
					if($estadoNovo == $estadoExistente['estuf']){
						$existe = true;
					}
				}
				if(!$existe){
					array_push($separacaoAcao['insert'], $estadoNovo);
				}
			}
		}
		//print_r($separacaoAcao); exit();
       	try {       		
       		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaEstados['iniid'] }";
       		//$this->model->executar($query_delete);
       		// "Insere" (ativa) os estados na iniciativa
       		foreach($separacaoAcao['ativar'] as $estado){
       			$this->model = new Par3_Model_IniciativaIniciativasEstados($estado['iiestufid']);
       			$this->model->popularDadosObjeto(array('iiestufid'=>$estado['iiestufid'], 'iiestufstatus'=>"A", 'iniid'=>$estado['iniid'], 'estuf'=>$estado['estuf']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasEstados = new Par3_Model_IniciativaIniciativasEstados($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasEstadosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasEstados, Par3_Model_IniciativaIniciativasEstadosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// Insere os estados na iniciativa
       		foreach($separacaoAcao['insert'] as $estado){
       			$this->model = new Par3_Model_IniciativaIniciativasEstados();
       			$this->model->popularDadosObjeto(array('iiestufstatus'=>"A", 'iniid'=>$arrayIniciativaEstados['iniid'], 'estuf'=>$estado));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasEstados = new Par3_Model_IniciativaIniciativasEstados($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasEstadosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasEstados, Par3_Model_IniciativaIniciativasEstadosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// "Remove" (desativa) os estados que não existirão mais na iniciativa
       		foreach($separacaoAcao['deleteInativar'] as $estado){
       			$this->model = new Par3_Model_IniciativaIniciativasEstados($estado['iiestufid']);
       			$this->model->popularDadosObjeto(array('iiestufid'=>$estado['iiestufid'], 'iiestufstatus'=>"I", 'iniid'=>$estado['iniid'], 'estuf'=>$estado['estuf']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasEstados = new Par3_Model_IniciativaIniciativasEstados($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasEstadosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasEstados, Par3_Model_IniciativaIniciativasEstadosHistorico::DELETE);
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
        $id = $_GET['iiestufid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iiestufid={$id}";
        $iniciativa_iniciativas_estados = new Par3_Model_IniciativaIniciativasEstados($id);
        try{
             $iniciativa_iniciativas_estados->Xstatus = 'I';
             $iniciativa_iniciativas_estados->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_estados->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_estados->salvar();
             $iniciativa_iniciativas_estados->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>