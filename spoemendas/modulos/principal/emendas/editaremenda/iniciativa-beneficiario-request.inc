<?php

switch ($requisicao) {
    case 'salvar-iniciativa-beneficiario':
        global $db;
        /*
         * Salvar iniciativas do Beneficiário
         */
        $dados = $_REQUEST['iniciativa-beneficiario'];

        $delete = "DELETE FROM emenda.Iniciativadetalheentidade WHERE edeid = {$dados['edeid']}";

        /*
         * Apaga todas, exceto as subações que não podem ser apagadas (Já vinculadas no PAR)
         */
        apagaSubacoesNaoVinculadas($dados['edeid']);

        $db->executar($delete);
        $db->commit();
        if (is_array($dados['iniid'])) {
            foreach ($dados['iniid'] as $dadoSalvar) {
                if($dadoSalvar){
                $Iniciativadetalheentidade = new Spoemendas_Model_Iniciativadetalheentidade($dados['edeid']);
                $dadosSalvar['iniid'] = $dadoSalvar;
                $dadosSalvar['edeid'] = $dados['edeid'];
                $dadosSalvar['idestatus'] = 'A';
                $Iniciativadetalheentidade->popularDadosObjeto($dadosSalvar)->salvar();
                $Iniciativadetalheentidade->commit();
                }
            }
        }
        cadastrarDocid($informacoes, $infoemenda, $_REQUEST['emeid']);
        $fm->addMensagem('Requisição realizada com sucesso.');
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();

    case 'retornar-iniciativas-beneficiario-ajax':
        $edeid = $_REQUEST['edeid'];
        $sql = "SELECT
                    iniid
                FROM
                    emenda.iniciativadetalheentidade
                WHERE
                    idestatus = 'A'
                AND edeid = {$edeid}";
        $dados = $db->carregar($sql);
        $saida = array();
        if (is_array($dados)) {
            foreach ($dados as $dado) {
                array_push($saida, $dado['iniid']);
            }
        }
        echo simec_json_encode($saida);
        die;
        break;
}
