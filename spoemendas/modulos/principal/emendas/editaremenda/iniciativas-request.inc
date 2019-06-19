<?php
require_once APPRAIZ . 'spoemendas/classes/model/Iniciativaemendadetalhe.class.inc';
global $alterandoEmenda;

if (!$requisicao) {
    $requisicao = $_REQUEST['requisicao'];
}

switch ($requisicao) {

    case 'nova-iniciativa-emenda':
        $model = new Spoemenda_Model_Iniciativaemendadetalhe();
        $model->emdid = $_POST['cadastroiniciativa']['emdid'];
        $model->iedstatus = 'A';

        foreach ($_REQUEST['cadastroiniciativa']['iniid'] as $iniid) {
            $model->iniid = $iniid;
            $model->inserir();
        }

        if ($model->commit()) {
            $fm->addMensagem("Registro inserido com sucesso.", Simec_Helper_FlashMessage::SUCESSO);
        } else {
            $fm->addMensagem("Falha ao tentar inserir novo registro.", Simec_Helper_FlashMessage::ERRO);
        }

        if ($infoemenda) {
            cadastrarDocid($informacoes, $infoemenda, $dados['emeid']);
        }

        header("Location: {$_SERVER['REQUEST_URI']}");
        die();

    case 'apagar-emenda':

        header("Content-type:application/json");
        $model = new Spoemenda_Model_Iniciativaemendadetalhe();
        $model->iniid = (int) $_GET['iniid'];
        if ($model->deleteIniciativas((int) $_GET['emeid'])) {
            $fm->addMensagem("Registro excluido com sucesso.", Simec_Helper_FlashMessage::SUCESSO);
            echo simec_json_encode(['success' => 'ok']);
        } else {
            $fm->addMensagem("Falha ao tentar excluir registro.", Simec_Helper_FlashMessage::ERRO);
            echo simec_json_encode(['fail' => 'ok']);
        }
        die;
}
