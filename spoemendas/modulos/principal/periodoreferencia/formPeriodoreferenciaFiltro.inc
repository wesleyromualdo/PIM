<?php
/**
 * Formulário de cadastro e de filtros do período de referência.
 *
 * $Id: formPeriodoreferenciaFiltro.inc 96747 2015-04-29 17:43:52Z maykelbraz $
 */

$form = new Simec_View_Form('periodoreferencia');
$form->/*addInputTexto(
    'prfnome',
    $_REQUEST['periodoreferencia']['prfnome'],
    'prfnome',
    250,
    false,
    array('flabel' => 'Título')
    )->*/addBotoes(array(/*'limpar', 'buscar', */'novo' => array('label' => 'Novo período')))
    ->render();
?>
<script type="text/javascript" lang="JavaScript">
$(function(){
    $('.btn-novo').click(function(){
        window.location.href = 'spoemendas.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A&requisicao=novo';
    });
});
</script>