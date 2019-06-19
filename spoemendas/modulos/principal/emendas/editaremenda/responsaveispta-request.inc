<?php
switch ($requisicao) {
    case 'consultarcpf':
        echo simec_json_encode(recuperarUsuarioReceita(
            str_replace(['.', '-'], '', $dados['dados']['cpf'])
        ));
        die();

    case 'salvar-beneficiariopta':
        $emendaDetEntidade = new Spoemendas_Model_Emendadetalheentidade();
        $emendaDetEntidade->popularDadosObjeto($dados['responsavelpta']);

        if ($emendaDetEntidade->salvar()) {
            $emendaDetEntidade->commit();
            $fm->addMensagem('Responsável alterado com sucesso.');
        } else {
            $fm->addMensagem('Não foi possível alterar o Responsável TPA', Simec_Helper_FlashMessage::ERRO);
        }
        cadastrarDocid($informacoes, $infoemenda, $_REQUEST['emeid']);
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();
}
