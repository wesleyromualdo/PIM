<?php
/**
 * Arquivo de alteração de detalhes da emenda.
 *
 * @version $Id: detalhes.inc 120442 2017-03-17 19:45:04Z wesleysilva $
 */
?>
<script type="text/javascript">
function editarDetalheEmenda(emdid)
{
    $.get(window.location.href, {requisicao:'carregarDetalheEmenda', 'dados[emdid]':emdid}, function(resposta){
        $('#detalheemenda_emdid').val(resposta.emdid);
        $('#detalheemenda_gndcod').val(resposta.gndcod).trigger('chosen:updated');
        $('#detalheemenda_foncod').val(resposta.foncod).trigger('chosen:updated');
        $('#detalheemenda_mapcod').val(resposta.mapcod).trigger('chosen:updated');
        $('#detalheemenda_emdvalor').val(resposta.emdvalor).blur();

        $('#detalheemenda_gndcod').chosen('destroy').attr('disabled', true);
        $('#detalheemenda_foncod').chosen('destroy').attr('disabled', true);
        $('#detalheemenda_emdvalor').attr('readonly', true);
        
        $('#mdldetalheemenda').modal();
    }, 'json');
}

$(function(){
    $('.navbar-listagem .btn-adicionar').click(function(){
        $('#detalheemenda_emdid').val('');
        $('#detalheemenda_gndcod').val('').trigger('chosen:updated');
        $('#detalheemenda_foncod').val('').trigger('chosen:updated');
        $('#detalheemenda_mapcod').val('').trigger('chosen:updated');
        $('#detalheemenda_emdvalor').val('0.00').blur();

        $("#mdldetalheemenda").modal();
    });
});

function onDetalheemendaSubmit()
{
    var campos = new Array();

    if ('' == $('#detalheemenda_gndcod').val()) {
        campos.push('GND');
    }
    if ('' == $('#detalheemenda_mapcod').val()) {
        campos.push('Modalidade');
    }
    if ('' == $('#detalheemenda_foncod').val()) {
        campos.push('Fonte');
    }
    if ('' == $('#detalheemenda_emdvalor').val()) {
        campos.push('Valor');
    }

    if (0 === campos.length) {
        return true;
    }

    var mensagem = 'Os seguintes campos são obrigatórios e não podem ser deixados em branco:<br /><br /><ul><li>'
        + campos.join(';</li><li>')
        + '.</li></ul>';
    bootbox.alert(mensagem);
    return false;
}
</script>
<?php
(new Simec_Listagem())
    ->setCabecalho(['GND', 'Fonte de recurso', 'Modalidade', 'Valor (R$)'])
    ->setQuery(Spoemendas_Model_EmendaDetalhe::getQueryDetalhesDaEmenda($_REQUEST['emeid']))
    ->addAcao('edit', 'editarDetalheEmenda')
    ->addCallbackDeCampo('gndcod', 'formatarGnd')
    ->addCallbackDeCampo('foncod', 'formatarFonte')
    ->addCallbackDeCampo('mapcod', 'formatarModalidade')
    ->addCallbackDeCampo('emdvalor', 'mascaraMoeda')
    ->esconderColunas('gnddsc', 'fondsc', 'mapdsc')
    ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR)
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

$formDetalheEmenda = (new Simec_View_Form('detalheemenda'))
    ->carregarDados($dadosDetalheEmenda)
    ->addHidden('emdid', $dadosDetalheEmenda['emdid'])
    ->addCombo('GND', 'gndcod', Spo_Model_Gruponaturezadespesa::getQueryComboDoPublic())
    ->addCombo('Modalidade', 'mapcod', Public_Model_Modalidadeaplicacao::getQueryCombo())
    ->addCombo('Fonte', 'foncod', Public_Model_Fonterecurso::getQueryCombo())
    ->addMoeda('Valor', 'emdvalor')
    ->setRequisicao('salvardetalheemenda')
    ->render(false);

bootstrapPopup('Detalhes da emenda', 'mdldetalheemenda', $formDetalheEmenda, ['cancelar', 'confirmar'], ['tamanho' => 'lg']);
