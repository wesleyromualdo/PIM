<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

require_once APPRAIZ . "includes/funcoesspo_componentes.php";

include_once APPRAIZ . 'par3/classes/controller/ItensComposicaoPregaoSigarp.class.inc';
include_once APPRAIZ . 'par3/classes/model/ItensComposicaoPregaoSigarp.class.inc';

$controllerItemPregaoSicarp= new Par3_Controller_ItensComposicaoPregaoSigarp();


function retornaRelatorio($arrPost)
{
    $controllerPregaoSircap = new Par3_Controller_ItensComposicaoPregaoSigarp();
    $controllerPregaoSircap->listar($arrPost);
}


if($_REQUEST['requisicao'] == 'carregarDadosPregao'){
    $sprid = $_POST['dados'][0];
    ob_clean();
    $itd = new Par3_Controller_ItensComposicaoDetalhamento();
    $itd->listarDadosPregao($sprid);
    die;
}


switch ($_REQUEST['req']) {
    case 'pesquisar':
        ob_clean();
        echo $controllerItemPregaoSicarp->listar($_REQUEST);
        die;
        break;

    case 'xls':
        ob_clean();
        $controllerItemPregaoSicarp->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=grupo_itens_composicao.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerItemPregaoSicarp->listar($_REQUEST);
        die;
        break;
}
$mInto = new Par3_Model_IniciativaTiposObjeto();
$rsInto = $mInto->recuperarTodos('intoid as codigo,intodsc as descricao ', array("intostatus = 'A' "), 'intodsc ASC');

?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Pregão - SIGARP</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-3 col-md-6 center">


                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="sprid" maxlength="255"
                                       id="sprid" class="form-control" onkeypress='return SomenteNumero(event)'/></div>
                        </div>

                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Número:</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="sprnupregao" maxlength="255"
                                       id="sprnupregao" class="form-control"/></div>
                        </div>


                        <!--filtros-->
                        <div class="ibox float-e-margins">
                            <div class="ibox-content" id="avancado" style="display: none;">
                                <div class="form-group"><label
                                            class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Descrição:</label>
                                    <div class="col-sm-7 col-md-8"><input type="text" name="asdasd" id="asdsad"
                                                                          maxlength="255" class="form-control"/></div>
                                </div>

                            </div>

                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-grupoitem"><i class="fa fa-search"></i>Pesquisa
                </button>

                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                            class="fa fa-file-excel-o"></i> XLS
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i
                            class="glyphicon glyphicon-print"></i>Imprimir
                </button>
                |
                <button type="reset" class="btn btn-success" id="limpar-formfiltro"><i class="fa fa-eraser"></i> Limpar
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="grupositenslistagem">
            <?php echo $controllerItemPregaoSicarp->listar($_REQUEST) ?>
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
                    Relatório Descrição de Iniciativas<br>
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
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_grupoitem" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="formGrupoItem" id="formGrupoItem" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Grupo de Item de Composição - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarGrupoItem" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarGrupoItem" class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    //mostra/esconde ibox
    $("#btn-avancado").click(function () {
        $('#avancado').slideToggle();
        //chevron up/down
        $('#chevron').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $('#chevron').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#chevron').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //    $("#btn-incluir").click(function () {
    //        $("#modal_titulo").html("Formulário Grupo de Itens - Incluir");
    //        carregarForm();
    //        $("modal_grupoitem").modal('show');
    //    });

    $("#btn-incluir").click(function () {
        window.location.assign(window.location.href + "&submenu=form");
    });

    function editarItemComposicaoGrupo(id) {
        window.location.assign(window.location.href + "&submenu=form&igrid=" + id);
    }

    //    function editarGrupoItemComposicao(id)
    //    {
    //        window.location.assign(window.location.href + "&submenu=form&gitid="+id);
    //    }

    $(".xls").on("click", function () {
        window.location.assign(window.location.href + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = window.location.href;
        var action = '&req=imprimir&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
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
                w.document.title = 'Relatório - Grupo de Itens de Composição';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    function removerGrupoItemComposicao(id) {
        var caminho = window.location.href;
        var action = 'req=recuperar&igrid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var igr = $.parseJSON(resp);
                swal({
                    title: "Remover Grupo de Item de Composição",
                    text: "Tem certeza que deseja remover: <b>" + igr.igrid + " - " + igr.igrnome + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&igrid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (resp != 'erro') {
                                    msgSuccess(caminho, "Grupo de Item de Composição removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover o Grupo de Item de Composição.", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarListagem(params ="") {
        if (params) {
            params = '&' + params
        }
        var caminho = window.location.href;
        var action = '&req=atualizarListagem' + params;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#grupositenslistagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar-grupoitem").click(function (evt) {
        evt.preventDefault();
        var caminho = window.location.href;
        var action = 'req=pesquisar&' + $("#form-filtro").serialize();
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#grupositenslistagem").html(resp);
                return true;
            }
        });
    });

    function carregarForm(id = null) {
        var caminho = window.location.href;
        var requisicao = '&req=form';
        var igrid = '';
        if (id != null) {
            var igrid = '&igrid=' + id;
        }
        $('#conteudo-modal').load(caminho + requisicao + igrid);
    }


    //    function editarItemComposicaoGrupo(id) {
    //        $("#modal_grupoitem").modal("show");
    //        $("#modal_titulo").html("Grupo de Itens de Composição - Editar");
    //        carregarForm(id);
    //    }

    function removerItemComposicaoGrupo(id) {
        var caminho = window.location.href;
        var action = 'req=recuperar&igrid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var igr = $.parseJSON(resp);
                swal({
                    title: "Remover Grupo de Item de Composição",
                    text: "Tem certeza que deseja remover: <b><br/><center>" + igr.igrid + " - " + igr.igrnome + "?<center>",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&igrid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (resp != 'erro') {
                                    msgSuccess(caminho, "Grupo de Itens de Composição removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover o Grupo de Itens de Composição.", "error");
                            }
                        });
                    }
                })
            }
        });
    }
    function js_resetaPagina(){
        window.location.href = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=pregaoSigarp";
    }

    function SomenteNumero(e){
        var tecla = (window.event) ? event.keyCode : e.which;
        if ((tecla > 47 && tecla < 58 || tecla === 44 || tecla === 45 || tecla === 13))
            return true;
        else {
            if (tecla === 8 || tecla === 0)
                return true;
            else
                alert('Informe apenas números!')
                return false;
        }
    }

    $(document).ready(function() {

        $("#sprid").bind('paste', function(e) {
            e.preventDefault();
        });

    });

    jQuery("#limpar-formfiltro").click(function () {

        $('#sprid').val('');
        $('#sprnupregao').val('');

        js_resetaPagina();

    });
</script>
