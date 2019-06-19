<?php
include APPRAIZ . "includes/cabecalho.inc";
$fm = new Simec_Helper_FlashMessage('spoemendas/spoemendas.php?modulo=principal/cargas/cargalimitesempenhos');
if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    switch ($requisicao) {
        case('salvar'):
            $restultado = processarCargaEmpenhosLimites($_REQUEST,$_FILES);
            $fm->addMensagem($restultado['mensagem'],$restultado['tipo']);
            break;
    }
}
$bc = new Simec_View_Breadcrumb('C');
$bc->add('Carga de Empenhos e Limites', 'spoemendas.php?modulo=principal/cargas/cargalimitesempenhos&acao=A')
        ->add('Carga')
        ->render();
$infoModelo = "teste";
$form = new Simec_View_Form('carga');
$form->carregarDados($dados)
        ->addFile('Arquivo .csv', 'anexo', array('button' => array('id' => 'excluirAnexo', 'texto' => 'Apagar Arquivo', 'class' => 'btn btn-danger btn-avancado'), 'info' => "{$infoModelo}"))
        ->addSeparador("<div class=\"col-md-2\" ></div><a href=\"modelo_carga_empenho_limite.csv\" target=_blank class=\"col-md-6\" style=\"font-size: 16px;\">Baixar modelo .csv para Carga de Empenhos e Limites</a>")
        ->addBotoes(array('limpar', 'salvar', 'executar'))
        ->setRequisicao('salvar')
        ->render();
echo $fm->getMensagens();
$listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
$sql = "SELECT
            crg.emeano||crg.emecod AS chave,
            crg.emeano,
            crg.emecod,
            crg.vlrempenho,
            crg.vlrlimiteempenho
        FROM
            spoemendas.cargavalores crg
        ORDER BY
            2,3";
$arrColunas = array('Tramitar em lote', 'Publicado', 'Executar', 'SQL', 'Solicitação', 'Data', 'Solicitante', 'Situação');
$listagem->setCabecalho($arrColunas)
        ->setQuery($sql)
        ->turnOnPesquisator()
        ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
?>