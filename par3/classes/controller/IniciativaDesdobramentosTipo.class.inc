<?php

/**
 * User: danielfiuza
 * Date: 27/03/17
 * Time: 15:13
 */
class Par3_Controller_IniciativaDesdobramentosTipo extends Controle
{
    public function __construct()
    {
        $this->model          = new Par3_Model_IniciativaDesdobramentosTipo($_REQUEST['tipid']);
        $this->modelHistorico = new Par3_Model_IniciativaDesdobramentosTipoHistorico();
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaDesdobramentosTipoHistorico::CREATE;
        if($arrPost['tipid']){
            $arrayDesTipo['tipid'] = $arrPost['tipid'];
            $acao = Par3_Model_IniciativaDesdobramentosTipoHistorico::UPDATE;
        }
        $arrayDesTipo['tipsituacao']    = $arrPost['tipsituacao'];
        $arrayDesTipo['tipstatus']      = 'A';
        $arrayDesTipo['tipdsc']         = trim(($arrPost['tipdsc']));
        $arrayDesTipo['tipcpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        //Valida campos
        $erro = $this->model->validarInput($arrayDesTipo);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrayDesTipo);
            $id =  $this->model->salvar();
            $this->model->commit();
            $modelDesTipo= new Par3_Model_IniciativaDesdobramentosTipo($id);
            $this->modelHistorico->gravarHistorico($modelDesTipo,$acao);
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

    public function pegarSelectCombo($tipid = null)
    {
        return $this->model->pegarSQLSelectCombo($tipid);
    }
}