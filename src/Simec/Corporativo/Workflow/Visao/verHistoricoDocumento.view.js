function historicoBootstrapComentario(hstid)
{
    var id = '#arow-' + hstid;
    var $parentTr = jQuery(id).closest('tr');

    if (jQuery(id + ' span').hasClass('open')) {
        $parentTr.next().remove();
        jQuery(id + ' span').removeClass('open').removeClass('btn-default').addClass('btn-info');
    } else {
        jQuery(id + ' span').addClass('open').removeClass('btn-info').addClass('btn-default');;
        var numCols = jQuery('td', $parentTr).length;
        numAcao = $parentTr.parents('table').attr('data-qtd-acoes');
        td_acao = '<td colspan="'+ numAcao +'">&nbsp;</td>';
        $parentTr.after('<tr>'+td_acao+'<td colspan="' + numCols + '"><blockquote style="text-align:left">' + jQuery('#hstid_' + hstid).attr('data-comentario') + '</blockquote></td></tr>');
    }
}

jQuery(function()
{
    jQuery('#modal-alert .modal-dialog').css('width', '70%');
//    jQuery('[data-toggle="popover"]').popover({placement:'top',trigger:'hover'});
    jQuery('#modalComponenteWorkflow').find(".navbar-listagem").remove();
});
