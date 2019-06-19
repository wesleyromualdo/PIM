<?php

$_REQUEST['remover'] = 'N';

$controllerObraTipo = new Par3_Controller_ObraTipo();
$rsObraTipo = $controllerObraTipo->pegarSelectCombo();

$controllerObraTipoVersaoProjeto = new Par3_Controller_ObraTipoVersaoProjeto();
$ctrObraVersaoPlanilha           = new Par3_Controller_ObraTipoVersaoPlanilha();

function retornaRelatorio($arrPost)
{
    $controllerObraTipo = new Par3_Controller_ObraTipo();
    $controllerObraTipo->listar($arrPost);
}

switch ($_REQUEST['requisicao']) {
    case 'pesquisar':
        ob_clean();
        echo $controllerObraTipo->listar($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerObraTipo->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Planilha_Orçamentária.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
}

global $simec;
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Planilha Orçamentária</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <?php
                        echo $simec->input('otpdsc', 'Descrição', $_REQUEST['descriao'], array(), array());
                        ?>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="listagem">
            <?php
            $controllerObraTipo->listar($_REQUEST);
            ?>
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
                    Relatório Planilha Orçamentária<br>
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
<br>
<script>
    $(".xls").on("click", function () {
        window.location.assign(window.location.href + "&requisicao=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = window.location.href;
        var action = '&requisicao=imprimir&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
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
                w.document.title = 'Relatório - Planilha Orçamentaria';
                w.print();
                w.close();
                $("#impressao-content").html("");
                return true;
            }
        });
    });

    $("#pesquisar").click(function (evt) {
        evt.preventDefault();
        var caminho = window.location.href;
        var action = '&requisicao=pesquisar&' + $("#form-filtro").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#listagem").html(resp);
                return true;
            }
        });
    });

    jQuery("#limpar").click(function(){
        $('#otpdsc').val(' ');
    });

    function editar(id) {
        window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cad_planilha_orcamentaria&otpid=' + id;
    }
</script>