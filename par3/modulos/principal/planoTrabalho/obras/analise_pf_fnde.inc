<?php
// include_once APPRAIZ . "includes/workflow.php";
// $mObra = new Par3_Controller_Obra($_GET['inuid'], $_GET['obrid']);
$mObra = new Par3_Controller_ObraDocumentos();
// if ($_REQUEST['requisicao'] == 'estadoatual') {
//     ob_clean();
//     $estadoAtual = wf_pegarEstadoAtual($mObra->model->docid);
//     echo simec_json_encode($estadoAtual);
//     die;
// }

if ($_REQUEST['requisicao'] == 'download') {
	ob_clean();
	$arqid = $_GET['arqid'];
	if ($arqid) {
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec(null, null, "par3");
		$file->getDownloadArquivo($arqid);
	}
	die;
}

if ($_REQUEST['requisicao'] == 'imprimir') {
    ob_clean();
    echo ($mObra->listaDocumentosFNDE($_REQUEST));
    die;
}

// if ($_REQUEST['requisicao'] == 'recuperarcomentario') {
//     ob_clean();
//     $comentario = $db->recuperar("SELECT cmddsc FROM workflow.comentariodocumento WHERE hstid = {$_REQUEST['hstid']}");
//     echo simec_json_encode($comentario['cmddsc']);
//     die;
// }
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Histórico de Análise - PF-FNDE</h3>
                <h3 class="center"><?= (empty($_GET['obrid']) ? '' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
                <div class="col-lg-2">
                    <button class="btn btn-warning" id="imprimir"><i class="fa fa-print"></i> Imprimir</button>
                </div>
                <div class="col-lg-12">
                    <?php echo $mObra->listaDocumentosFNDE($_REQUEST); ?>
                </div>
        </div>
    </div>
</div>

<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório - Histórico de Análise - PF-FNDE<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content">

    </div>
</div>

<script>
//     $(document).ready(function () {
//         var caminho = window.location.href;
//         var action = '&requisicao=estadoatual';
//         $.ajax({
//             type: "GET",
//             url: caminho,
//             data: action,
//             async: false,
//             success: function (resp) {
//                 var valor = $.parseJSON(resp);
//                 $('#historicotramitacao').find('tfoot').prepend('<tr><td style="text-align:right" colspan="7"><strong>Estado Atual: </strong>' + valor.esddsc + '</td></tr>');
//             }
//         });
//     });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=imprimir&' + $('.form-listagem').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#impressao-content").html(resp);
                var container = $("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                table.css('width', '100%')
                table.css('border-collapse', 'collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border', '1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height', '50px');
                td.css('text-align', 'center');
                var w = window.open();
                w.document.write($('#div-impressao').html());
                w.document.title = 'Relatório - Histórico de Análise - PF-FNDE';
                w.print();
                w.close();
                $("#impressao-content").html("");
                return true;
            }
        });
    });

//     function historicoBootstrapComentario(hstid) {
//         var id = '#arow-' + hstid;
//         console.log(hstid);
//         var parentTr = $(id).closest('tr');

//         if ($(id + ' span').hasClass('open')) {
//             parentTr.next().remove();
//             $(id + ' span').removeClass('open').removeClass('btn-default').addClass('btn-info');
//         } else {
//             $(id + ' span').addClass('open').removeClass('btn-info').addClass('btn-default');
//             ;
//             var numCols = $('td', parentTr).length;
//             numAcao = parentTr.parents('table').attr('data-qtd-acoes');
//             td_acao = '<td colspan="' + numAcao + '">&nbsp;</td>';

//             var caminho = window.location.href;
//             var action = '&requisicao=recuperarcomentario&hstid=' + hstid;
//             $.ajax({
//                 type: "GET",
//                 url: caminho,
//                 data: action,
//                 async: false,
//                 success: function (resp) {
//                     var comentario = $.parseJSON(resp);
//                     console.log(comentario);
//                     parentTr.after('<tr>' + td_acao + '<td colspan="' + numCols + '"><blockquote style="text-align:left">' + comentario + '</blockquote></td></tr>');
//                 }
//             });
//         }
//     }
</script>