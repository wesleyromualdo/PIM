<?php
/**
 * Listagem de emendas para edição.
 *
 * @version $Id: listaremendas.inc 109643 2016-03-31 19:49:39Z werteralmeida $
 */
/**
 * Cabeçalho do SIMEC.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
    $(document).ready(function () {
        console.log("ready!");
    });
    function editarEmenda(emeid)
    {
        window.location.assign(
                '/spoemendas/spoemendas.php?modulo=principal/alteracaoemendas/editaremenda&acao=A'
                + '&emeid=' + emeid
                );
    }
</script>
<style>
    *{
        white-space: normal !important;
    }
</style>
<div class="col-md-12">
    <?php
    (new Simec_View_Breadcrumb())
            ->add('Alteração de emendas')
            ->render();

    $form = new Simec_View_Form('filtroemendas');
    $form->carregarDados($_REQUEST['filtroemendas'])
            ->addTexto('Código', 'emecod', 8, ['masc' => '########'])
            ->addCombo('Autor', 'autid', (new Spoemendas_Model_Autor())->recuperarTodosFormatoInput('autnome', [], 'autnome'))
            ->addCombo('Unidade Orçamentária', 'unicod', Spo_Model_Unidade::queryCombo())
            ->addCombo('Responsável', 'resid', (new Spoemendas_Model_Responsavel())->recuperarTodosFormatoInput('resdsc', [], 'resdsc'))
            ->addBotoes(['buscar', 'limpar'])
            ->render();

    $list = new Simec_Listagem();
    $list->setQuery((new Spoemendas_Model_Emenda)->queryListaEmendas($_REQUEST['filtroemendas'] ? $_REQUEST['filtroemendas'] : []))
            ->setCabecalho(['Código', 'Autor', 'UO', 'Funcional', 'Subtítulo', 'Responsável', 'Origem', 'Impositivo'])
            ->addCallbackDeCampo('unicod', 'formatarUnicod')
            ->addCallbackDeCampo('impositivo', 'formatarImpositivo')
            ->esconderColunas('unidsc')
            ->addAcao('edit', 'editarEmenda')
            ->turnOnPesquisator()
            ->setFormFiltros('filtroemendas')
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>
</div>