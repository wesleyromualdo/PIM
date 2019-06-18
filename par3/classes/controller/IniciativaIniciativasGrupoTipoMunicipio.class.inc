<?php
/**
 * Classe de controle do  iniciativa_iniciativas_grupo_tipo_municipio
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RICARDO MACEDO RANGEL BORGES <ricardoborges@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_IniciativaIniciativasGrupoTipoMunicipio
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIniciativasGrupoTipoMunicipio
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($_GET['iigtmid']);
        $this->modelHistorico = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
	public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico::CREATE;
    	 if($arrPost['iigtmid']){
    		$arrayIniciativa['iigtmid'] = $arrPost['iigtmid'];
    		$acao = Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico::UPDATE;
    	} else {
    		$arrPost['iigtmid'] = null;
    	}
    	
    	$arrayIniciativaGrupoTipoMunicipio['iniid'] = $arrPost['iniid'];
    	$arrayIniciativaGrupoTipoMunicipio['gtmid'] = $arrPost['gtmid'];
    	// Se existe o id da Iniciativa, deverá ser identificado o grupoTipoMunicipio que será inserido, exlcuído (inativado) ou mantido:
    	if($arrayIniciativaGrupoTipoMunicipio['iniid']){
    		// Irá identificar os grupoTipoMunicipios que já existem e que não existem para saber se gravar no histórico um INSERT ou DELETE.
    		// Os grupoTipoMunicipios que foram removidos, serão desativados na coluna de status e criado um histórico de DELETE. Os que não mudaram não faz anda.
    		$separacaoAcao = array("insert"=>array(), "ativar"=>array(), "deleteInativar"=>array(), "naoAlterado"=>array());
    		$iniid = $arrayIniciativaGrupoTipoMunicipio['iniid'];
    		$grupoTipoMunicipiosIniciativa = $this->model->recuperarTodos('*', array("iniid = $iniid"), null);
    		// Primeiro verifica o que será removido (inativado) - deletarInativar - , ativado ou não alterado:
    		foreach($grupoTipoMunicipiosIniciativa as $grupoTipoMunicipioExistente){
    			if(in_array($grupoTipoMunicipioExistente['gtmid'], $arrayIniciativaGrupoTipoMunicipio['gtmid'])){
    				// Após ter encontrado, verifica se está ativo. Se estiver ativo, não será alterado. Se estiver inativo, será ativado e será CREATE no histórico.
    				if($grupoTipoMunicipioExistente['iigtmstatus'] == "A"){
    					array_push($separacaoAcao['naoAlterado'], $grupoTipoMunicipioExistente);
    				} else if($grupoTipoMunicipioExistente['iigtmstatus'] == "I"){
    					array_push($separacaoAcao['ativar'], $grupoTipoMunicipioExistente);
    				}
    			} else {
    				if($grupoTipoMunicipioExistente['iigtmstatus'] == "A"){
    					array_push($separacaoAcao['deleteInativar'], $grupoTipoMunicipioExistente);
    				}
    			}
    		}
    		// Verifica o que será inserido
    		foreach($arrayIniciativaGrupoTipoMunicipio['gtmid'] as $grupoTipoMunicipioNovo){
    			$existe = false;
    			foreach($grupoTipoMunicipiosIniciativa as $grupoTipoMunicipioExistente){
    				if($grupoTipoMunicipioNovo == $grupoTipoMunicipioExistente['gtmid']){
    					$existe = true;
    				}
    			}
    			if(!$existe){
    				array_push($separacaoAcao['insert'], $grupoTipoMunicipioNovo);
    			}
    		}
    	}
    	//print_r($separacaoAcao); exit();
    	try {
    		//$query_delete = "DELETE FROM {$this->model->stNomeTabela} where iniid = {$arrayIniciativaGrupoTipoMunicipio['iniid'] }";
    		//$this->model->executar($query_delete);
    		// "Insere" (ativa) os grupoTipoMunicipios na iniciativa
    		foreach($separacaoAcao['ativar'] as $grupoTipoMunicipio){
    			$this->model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($grupoTipoMunicipio['iigtmid']);
    			$this->model->popularDadosObjeto(array('iigtmid'=>$grupoTipoMunicipio['iigtmid'], 'iigtmstatus'=>"A", 'iniid'=>$grupoTipoMunicipio['iniid'], 'gtmid'=>$grupoTipoMunicipio['gtmid']));
    			$id = $this->model->salvar();
    			$this->model->commit();
    			// Insere o histórico:
    			$modelIniciativaIniciativasGrupoTipoMunicipio = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($id);
    			$this->modelHistorico = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico();
    			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasGrupoTipoMunicipio, Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico::CREATE);
    			$this->modelHistorico->commit();
    		}
    		// Insere os grupoTipoMunicipios na iniciativa
    		foreach($separacaoAcao['insert'] as $grupoTipoMunicipio){
    			$this->model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio();
    			$this->model->popularDadosObjeto(array('iigtmstatus'=>"A", 'iniid'=>$arrayIniciativaGrupoTipoMunicipio['iniid'], 'gtmid'=>$grupoTipoMunicipio));
    			$id = $this->model->salvar();
    			$this->model->commit();
    			// Insere o histórico:
    			$modelIniciativaIniciativasGrupoTipoMunicipio = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($id);
    			$this->modelHistorico = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico();
    			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasGrupoTipoMunicipio, Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico::CREATE);
    			$this->modelHistorico->commit();
    		}
    		// "Remove" (desativa) os grupoTipoMunicipios que não existirão mais na iniciativa
    		foreach($separacaoAcao['deleteInativar'] as $grupoTipoMunicipio){
    			$this->model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($grupoTipoMunicipio['iigtmid']);
    			$this->model->popularDadosObjeto(array('iigtmid'=>$grupoTipoMunicipio['iigtmid'], 'iigtmstatus'=>"I", 'iniid'=>$grupoTipoMunicipio['iniid'], 'gtmid'=>$grupoTipoMunicipio['gtmid']));
    			$id = $this->model->salvar();
    			$this->model->commit();
    			// Insere o histórico:
    			$modelIniciativaIniciativasGrupoTipoMunicipio = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($id);
    			$this->modelHistorico = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico();
    			$this->modelHistorico->gravarHistorico($modelIniciativaIniciativasGrupoTipoMunicipio, Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico::DELETE);
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
        $id = $_GET['iigtmid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iigtmid={$id}";
        $iniciativa_iniciativas_grupo_tipo_municipio = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($id);
        try{
             $iniciativa_iniciativas_grupo_tipo_municipio->Xstatus = 'I';
             $iniciativa_iniciativas_grupo_tipo_municipio->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_iniciativas_grupo_tipo_municipio->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_iniciativas_grupo_tipo_municipio->salvar();
             $iniciativa_iniciativas_grupo_tipo_municipio->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>