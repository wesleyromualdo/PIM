<?php

$controllerObradocumentoApoio = new Par3_Controller_ObraDocumentoApoio();

function retornaRelatorio($arrPost)
{
    $controllerObradocumentoApoio = new Par3_Controller_ObraDocumentoApoio();
    $controllerObradocumentoApoio->listaDocumentosApoio($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerObradocumentoApoio->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerObradocumentoApoio->listaDocumentosApoio($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerObradocumentoApoio->form($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerObradocumentoApoio->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerObradocumentoApoio->listaDocumentosApoio($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerObradocumentoApoio->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerObradocumentoApoio->listaDocumentosApoio($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Documento_Apoio.xls");
        die;
        break;
    case 'download':
        ob_clean();
        $arqid = $_GET['arqid'];
        if ( $arqid ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file 		  = new FilesSimec(null, null, "par3");
            $file->getDownloadArquivo($arqid);
        }
        die();
        break;
    default:
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerObradocumentoApoio->recuperar($_REQUEST);
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Documentos de Apoio</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-3 col-md-6 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Descrição:</label>
                            <div class="col-sm-7 col-md-8"><input type="text" name="odadsc" id="odadsc" maxlength="255"
                                                                  class="form-control"/></div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal"><i class="fa fa-plus"></i> Incluir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="Listagem">
            <?php
            $controllerObradocumentoApoio->listaDocumentosApoio($_REQUEST);
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
                    Relatório Documentos de Apoio<br>
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
<div class="ibox float-e-margins animated modal" id="modal" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="form" id="form" class="form form-horizontal" enctype="multipart/form-data">
            <input name="req" id="req" value="salvar" type="hidden">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Documentos de Apoio - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php echo $controllerObradocumentoApoio->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelar" data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvar"
                                class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script>
    jQuery(function () {

        jQuery(document).find("#form").on('click', "input[name=indsituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível.", "warning");
        });

        jQuery('#salvar').click(function (evt) {

            var fileName = '';

            var file = $("#arqid")[0].files[0];
            console.log(file);
            fileName = file.name;

            if ($('#form input[name=odadsc]').val() == '' || fileName == '') {
                alert('Todos os campos são obrigatórios!');
                return false;
            }

            evt.preventDefault();
            var id = $('#conteudo-modal').find('[name=odaid]').val();
            var msg = 'Incluir';
            if (id) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> esse Documentos de Apoio?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        $("#req").val('salvar');
                        $("#form").submit();
                    }
                    return false;
                }
            );
        });
    });

    $("#btn-incluir").click(function (evt) {
        evt.preventDefault();
        atualizarForm();
        $("#modal").modal("show");
    });

    function editar(id) {
        var caminho = window.location.href;
        var action = '&req=editar&odaid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Documentos de Apoio - Editar");
                $("#modal").modal("show");
            }
        });
    }

    $("#modal").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Documentos de Apoio - Incluir");
    });

    function atualizarListagem(params ="") {
        if (params) {
            params = '&' + params
        }
        var caminho = window.location.href;
        var action = '&req=pesquisar' + params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#Listagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar").click(function (evt) {
        console.log($("#form-filtro").serialize());
        evt.preventDefault();
        var caminho = window.location.href;
        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#Listagem").html(resp);
                return true;
            }
        });
    });

    function remover(id) {
        var caminho = window.location.href;
        var action = '&req=recuperar&indid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var ind = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Documentos de Apoio",
                    text: "Tem certeza que deseja remover este Documentos de Apoio?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&odaid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal").modal("hide");
                                $("#modal").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho, "Documento de Apoio removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover a Documentos de Apoio, existe um vínculo com o tipo de obra.", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarForm(params = "") {
        if (params) {
            params = '&' + params
        }

        var caminho = window.location.href;
        var action = '&req=atualizarForm' + params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
                return true;
            }
        });
    }

    jQuery("#limpar").click(function () {

        $('#odadsc').val('');
        $("#pesquisar").click();
    });
</script>