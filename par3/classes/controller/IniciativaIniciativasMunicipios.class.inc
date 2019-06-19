<?php
/**
 * Classe de controle do  iniciativa_iniciativas_municipios
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RICARDO MACEDO RANGEL BORGES <ricardoborges@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasMunicipios
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-07-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasMunicipios
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasMunicipios($_GET['iimunid']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasMunicipiosHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaIniciativasMunicipiosHistorico::CREATE;
    	 if($arrPost['iimunid']){    	 	
    	 	$arrayIniciativa['iimunid'] = $arrPost['iimunid'];
    		$acao = Par3_Model_IniciativaIniciativasMunicipiosHistorico::UPDATE;
    	} else {
    		$arrPost['iimunid'] = null;
    	}
		
    	$arrayIniciativaMunicipios['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaMunicipios['muncod'] = $arrPost['muncod'];
    	// Se existe o id da Iniciativa, deverá ser identificado o municipio que será inserido, exlcuído (inativado) ou mantido:
		if($arrayIniciativaMunicipios['iniid']){
			// Irá identificar os municipios que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
			// Os municipios que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
			$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
			$iniid = $arrayIniciativaMunicipios['iniid'];
			$municipiosIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
			
			// Insere as áspas simples nos códigos dos municípios retornados:
			$arrMunicipiosAspas = array();
			foreach($municipiosIniciativa as $municipio){
				$municipio['muncod'] = "'" .$municipio['muncod'] ."'";
				array_push($arrMunicipiosAspas, $municipio);
			}
			$municipiosIniciativa = $arrMunicipiosAspas;
			
			// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:			
			foreach($municipiosIniciativa as $municipioExistente){
				if(in_array($municipioExistente['muncod'], $arrayIniciativaMunicipios['muncod'])){
					// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
					if($municipioExistente['iimunidstatus'] == "A"){
						array_push($separacaoAcao['naoAlterado'], $municipioExistente);
					} else if($municipioExistente['iimunidstatus'] == "I"){
						array_push($separacaoAcao['ativar'], $municipioExistente);
					}
				} else {
					if($municipioExistente['iimunidstatus'] == "A"){
						array_push($separacaoAcao['deleteInativar'], $municipioExistente);
					}
				}
			}
			// Verifica o que será inserido
			foreach($arrayIniciativaMunicipios['muncod'] as $municipioNovo){
				$existe = false;
				foreach($municipiosIniciativa as $municipioExistente){
					if($municipioNovo == $municipioExistente['muncod']){
						$existe = true;
					}
				}
				if(!$existe){
					array_push($separacaoAcao['insert'], $municipioNovo);
				}
			}
		}
		//print_r($separacaoAcao); exit();
       	try {       		
       		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaMunicipios['iniid'] }";
       		//$this->model->executar($query_delete);
       		// "Insere" (ativa) os municipios na iniciativa
       		foreach($separacaoAcao['ativar'] as $municipio){
       			$this->model = new Par3_Model_IniciativaIniciativasMunicipios($municipio['iimunid']);
       			$this->model->popularDadosObjeto(array('iimunid'=>$municipio['iimunid'], 'iimunidstatus'=>"A", 'iniid'=>$municipio['iniid'], 'muncod'=>str_replace("'", '', $municipio['muncod'])));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasMunicipios = new Par3_Model_IniciativaIniciativasMunicipios($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasMunicipiosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasMunicipios, Par3_Model_IniciativaIniciativasMunicipiosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// Insere os municipios na iniciativa
       		foreach($separacaoAcao['insert'] as $municipio){
       			$this->model = new Par3_Model_IniciativaIniciativasMunicipios();
       			$this->model->popularDadosObjeto(array('iimunidstatus'=>"A", 'iniid'=>$arrayIniciativaMunicipios['iniid'], 'muncod'=>str_replace("'", '', $municipio)));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasMunicipios = new Par3_Model_IniciativaIniciativasMunicipios($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasMunicipiosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasMunicipios, Par3_Model_IniciativaIniciativasMunicipiosHistorico::CREATE);
       			$this->modelHistorico->commit();
       		}
       		// "Remove" (desativa) os municipios que não existirão mais na iniciativa
       		foreach($separacaoAcao['deleteInativar'] as $municipio){
       			$this->model = new Par3_Model_IniciativaIniciativasMunicipios($municipio['iimunid']);
       			$this->model->popularDadosObjeto(array('iimunid'=>$municipio['iimunid'], 'iimunidstatus'=>"I", 'iniid'=>$municipio['iniid'], 'muncod'=>str_replace("'", '', $municipio['muncod'])));
       			$id = $this->model->salvar();
       			$this->model->commit();
       			// Insere o histórico:
       			$modelIniciativaIniciativasMunicipios = new Par3_Model_IniciativaIniciativasMunicipios($id);
       			$this->modelHistorico = new Par3_Model_IniciativaIniciativasMunicipiosHistorico();
       			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasMunicipios, Par3_Model_IniciativaIniciativasMunicipiosHistorico::DELETE);
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
        $id = $_GET['iimunid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iimunid={$id}";
        $iniciativa_iniciativas_municipios = new Par3_Model_IniciativaIniciativasMunicipios($id);
        try{
             $iniciativa_iniciativas_municipios->Xstatus = 'I';
             $iniciativa_iniciativas_municipios->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_municipios->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_municipios->salvar();
             $iniciativa_iniciativas_municipios->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>