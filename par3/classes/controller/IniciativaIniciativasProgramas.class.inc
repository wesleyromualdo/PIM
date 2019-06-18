<?php
/**
 * Classe de controle do  iniciativa_iniciativas_programas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RICARDO MACEDO RANGEL BORGES <ricardoborges@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasProgramas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasProgramas
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasProgramas($_GET['iiprgid']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
	public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaIniciativasProgramasHistorico::CREATE;
    	 if($arrPost['iiprgid']){
    		$arrayIniciativa['iiprgid'] = $arrPost['iiprgid'];
    		$acao = Par3_Model_IniciativaIniciativasProgramasHistorico::UPDATE;
    	} else {
    		$arrPost['iiprgid'] = null;
    	}
    	
    	$arrayIniciativaProgramas['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaProgramas['prgid'] = $arrPost['prgid'];
    	
       	// Se existe o id da Iniciativa, deverá ser identificado o programa que será inserido, exlcuído (inativado) ou mantido:
		if($arrayIniciativaProgramas['iniid']){
			// Irá identificar os programas que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
			// Os programas que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
			$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
			$iniid = $arrayIniciativaProgramas['iniid'];
			$programasIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
			// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:			
			foreach($programasIniciativa as $programaExistente){
				if(in_array($programaExistente['prgid'], $arrayIniciativaProgramas['prgid'])){
					// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
					if($programaExistente['iiprgstatus'] == "A"){
						array_push($separacaoAcao['naoAlterado'], $programaExistente);
					} else if($programaExistente['iiprgstatus'] == "I"){
						array_push($separacaoAcao['ativar'], $programaExistente);
					}
				} else {
					if($programaExistente['iiprgstatus'] == "A"){
						array_push($separacaoAcao['deleteInativar'], $programaExistente);
					}
				}
			}
			// Verifica o que será inserido
			foreach($arrayIniciativaProgramas['prgid'] as $programaNovo){
				$existe = false;
				foreach($programasIniciativa as $programaExistente){
					if($programaNovo == $programaExistente['prgid']){
						$existe = true;
					}
				}
				if(!$existe){
					array_push($separacaoAcao['insert'], $programaNovo);
				}
			}
		}
		//print_r($separacaoAcao); exit();
       	try {       		
       		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaProgramas['iniid'] }";
       		//$this->model->executar($query_delete);
       		// "Insere" (ativa) os programas na iniciativa
       		foreach($separacaoAcao['ativar'] as $programa){
       			$this->model = new Par3_Model_IniciativaIniciativasProgramas($programa['iiprgid']);
       			$this->model->popularDadosObjeto(array('iiprgid'=>$programa['iiprgid'], 'iiprgstatus'=>"A", 'iniid'=>$programa['iniid'], 'prgid'=>$programa['prgid']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasProgramas = new Par3_Model_IniciativaIniciativasProgramas($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasProgramas, Par3_Model_IniciativaIniciativasProgramasHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// Insere os programas na iniciativa
       		foreach($separacaoAcao['insert'] as $programa){
       			$this->model = new Par3_Model_IniciativaIniciativasProgramas();
       			$this->model->popularDadosObjeto(array('iiprgstatus'=>"A", 'iniid'=>$arrayIniciativaProgramas['iniid'], 'prgid'=>$programa));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasProgramas = new Par3_Model_IniciativaIniciativasProgramas($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasProgramas, Par3_Model_IniciativaIniciativasProgramasHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// "Remove" (desativa) os programas que não existirão mais na iniciativa
       		foreach($separacaoAcao['deleteInativar'] as $programa){
       			$this->model = new Par3_Model_IniciativaIniciativasProgramas($programa['iiprgid']);
       			$this->model->popularDadosObjeto(array('iiprgid'=>$programa['iiprgid'], 'iiprgstatus'=>"I", 'iniid'=>$programa['iniid'], 'prgid'=>$programa['prgid']));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasProgramas = new Par3_Model_IniciativaIniciativasProgramas($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasProgramas, Par3_Model_IniciativaIniciativasProgramasHistorico::DELETE);
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
        $id = $_GET['iiprgid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iiprgid={$id}";
        $iniciativa_iniciativas_programas = new Par3_Model_IniciativaIniciativasProgramas($id);
        try{
             $iniciativa_iniciativas_programas->Xstatus = 'I';
             $iniciativa_iniciativas_programas->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_programas->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_programas->salvar();
             $iniciativa_iniciativas_programas->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>