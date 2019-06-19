<?php
/**
 * User: danielfiuza
 * Date: 27/03/17
 * Time: 15:13
 */
class Par3_Controller_IniciativaUnidadeOrcamentaria extends Controle
{
    public function __construct()
    {
        $this->model          = new Par3_Model_IniciativaUnidadeOrcamentaria($_REQUEST['tipid']);
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaDesdobramentosTipoHistorico::CREATE;
        if($arrPost['iuoid']){
            $arrayUnidadeOrcamentaria['iuoid'] = $arrPost['iuoid'];
            $acao = Par3_Model_IniciativaDesdobramentosTipoHistorico::UPDATE;
        }
        $arrayUnidadeOrcamentaria['iuosituacao']    = $arrPost['iuosituacao'];
        $arrayUnidadeOrcamentaria['iuostatus']      = 'A';
        $arrayUnidadeOrcamentaria['iuodsc']         = trim(($arrPost['iuodsc']));
        $arrayUnidadeOrcamentaria['iuocpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        //Valida campos
        $erro = $this->model->validarInput($arrayUnidadeOrcamentaria);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrayUnidadeOrcamentaria);
            $id =  $this->model->salvar();
            $this->model->commit();
            $modelUnidadeOrcamentaria= new Par3_Model_IniciativaDesdobramentosTipo($id);
            $this->modelHistorico->gravarHistorico($modelUnidadeOrcamentaria,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($id) {
            return $id;
        }
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        $mDesdobramentoTipo = new Par3_Model_IniciativaDesdobramentosTipo();
        return simec_json_encode($mDesdobramentoTipo->lista(array('etaid', 'etadsc'), array("nivid = '{$arrPost['nivid']}'")));
    }

    public function pegarSelectCombo()
    {
        return $this->model->pegarSQLSelectCombo();
    }
}