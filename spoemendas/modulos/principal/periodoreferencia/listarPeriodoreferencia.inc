<?php
/**
 * Listagem de períodos de referência.
 *
 * $Id: listarPeriodoreferencia.inc 99325 2015-06-29 14:31:14Z lindalbertofilho $
 */

$where = array("prfano = '{$_SESSION['exercicio']}'");
if (isset($_REQUEST['periodoreferencia']) && $_REQUEST['periodoreferencia']['prfnome']) {
    $where[] = sprintf('prfnome ILIKE %s', "'%{$_REQUEST['periodoreferencia']['prfnome']}%'");
}

$list = new Simec_Listagem();
$list->setCabecalho(array('Título','Atual','Origens Editáveis', 'Perfis Editáveis', 'Abas Visíveis', 'Mensagem', 'Validade' => array('Início', 'Fim')))
    ->addAcao('edit', 'editarPeriodoreferencia')
    ->addAcao('delete', array('func' => 'deletarPeriodoreferencia', 'extra-params' => array('prfnome')))
    ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR)
    ->setQuery($oPerRef->recuperarTodos(
        'prfid, prfnome, prfatual, origenseditaveis, perfiseditaveis, abasvisiveis, prfmensagem, prfdatainicio, prfdatafim',
        $where,
        'prfnome',
        array()
    ))
    ->setListaColunas(['prfid', 'prfnome', 'prfatual', 'origenseditaveis', 'perfiseditaveis', 'abasvisiveis', 'prfmensagem', 'prfdatainicio', 'prfdatafim'])
    ->addCallbackDeCampo('prfnome', 'alinharesquerda')
    ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
?>
<script type="text/javascript">
function deletarPeriodoreferencia(prfid, prfnome)
{
    bootbox.confirm('Tem certeza que deseja excluir o período de referência "' + prfnome + '"?', function(result){
        if (result) {
            window.location.href = 'spoemendas.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A&requisicao=deletar&prfid=' + prfid;
        }
    });
}

function editarPeriodoreferencia(prfid)
{
    window.location.href = 'spoemendas.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A&requisicao=editar&id=' + prfid;
}
$(function(){
    $('.btn-limpar').click(function(){
        window.location.replace(window.location.href);
    });
    $('.btn-adicionar').click(function () {
        window.location.href = 'spoemendas.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A&requisicao=novo';
    });
});
</script>