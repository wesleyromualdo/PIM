<?php
include_once APPRAIZ . 'par3/classes/controller/ItensComposicao.class.inc';
include_once APPRAIZ . 'par3/classes/model/ItensComposicao.class.inc';
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

$controllerItemComposicao = new Par3_Controller_ItensComposicao();

function retornaRelatorio($arrPost)
{
    $controllerItem = new Par3_Controller_ItensComposicao();
    $controllerItem->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'recuperar':
        ob_clean();
        echo $controllerItemComposicao->recuperar($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerItemComposicao->listar($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerItemComposicao->remover($_REQUEST);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerItemComposicao->listar($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerItemComposicao->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=itens_composicao.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    default:
        break;
}
$mInto = new Par3_Model_IniciativaTiposObjeto();
$rsInto = $mInto->recuperarTodos('intoid as codigo,intodsc as descricao ', array("intostatus = 'A' "), 'intodsc ASC');

$mCtd = new Par3_Model_CategoriaDespesa();
$mUni = new Par3_Model_UnidadeMedida();
$sqlCtd = $mCtd->selectCombo();
$sqlUni = $mUni->selectCombo();
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Itens de Composição</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-3 col-md-6 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Descrição:</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="itcdsc" id="itcdsc" maxlength="255" class="form-control" value="<?php echo ($_GET['itcdsc']); ?>"/></div>
                        </div>
                        <!--filtros-->
                        <div id="avancado" style="display: none;">
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                <div class="col-sm-8 col-md-8 col-lg-8">
                                    <input type="text" name="codigo" id="codigo" class="form-control" value="<?php echo $_GET['codigo']; ?>"/>
                                </div>
                            </div>
                            <?php
                            echo $simec->select('intoid[]', 'Tipo de Objeto',$_GET['intoid'], simec_preparar_array($rsInto), array('maxlength' => '255', 'required', 'title' => 'Selecione um Tipo de Objeto', 'multiple' => 'multiple', 'placeholder' => 'Selecione'), array('input-size' => '8'));
                            echo $simec->select('ctdid', 'Categoria de Despesa',$_GET['ctdid'], $sqlCtd, array('maxlength' => '255', 'required', 'title' => 'Selecione uma Categoria de Despesa'), array('input-size' => '8'));
                            echo $simec->select('uniid', 'Unidade de Medida',$_GET['uniid'], $sqlUni, array('maxlength' => '255', 'required', 'title' => 'Selecione uma Unidade de Medida'), array('input-size' => '8'));
                            ?>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label">Situação:</label>
                                <div class="col-lg-8">
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="itcsituacao" id="itcsituacao1" <?php echo ($_GET['itcsituacao'] == 'A'?'checked':'');?> value="A">
                                        <label for="itcsituacao1"> Ativo </label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="itcsituacao" id="itcsituacao2" <?php echo ($_GET['itcsituacao'] == 'I'?'checked':'');?> value="I">
                                        <label for="itcsituacao2"> Inativo </label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="itcsituacao" id="itcsituacao3" <?php echo ($_GET['itcsituacao'] == ''?'checked':'');?> value="">
                                        <label for="itcsituacao3"> Todos </label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="ibox-title">
                            <div class="ibox-tools">
                                <a id="btn-avancado">Opções Avançadas <i id="chevron" class="fa fa-chevron-down"></i></a>
                            </div>
                        </div>
                        <br>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-item"><i class="fa fa-search"></i>Pesquisar</button>
                <button type="button" class="btn btn-success" id="btn-incluir"><i class="fa fa-plus" title="Incluir Item de Composição"></i>Incluir</button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar-formfiltro"><i class="fa fa-eraser"></i> Limpar</button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="itensComposicaoListagem">
            <?php echo $controllerItemComposicao->listar($_REQUEST) ?>
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
<div class="ibox float-e-margins animated modal" id="modal_iniciativaDesc" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formIniciativaDescricao" id="formIniciativaDescricao" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Descrição de Iniciativa - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarDescIniciativa"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    var caminho = '<?php echo $urlGuia;?>';
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

    $("#btn-incluir").click(function () {
        window.location.assign(caminho + "&submenu=form");
    });

    function editarItemComposicao(id) {
        window.location.assign(caminho + "&submenu=form&itcid=" + id);
    }

    $(".xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
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
                w.document.title = 'Relatório - Itens de Composição';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    function removerItemComposicao(id) {
        console.log(id);
        var caminho = '<?php echo $urlGuia;?>';
        var action = 'req=recuperar&itcid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var itc = $.parseJSON(resp);
                swal({
                    title: "Remover Item de Composição",
                    text: "Tem certeza que deseja remover: <b>" + itc.itcid + " - " + itc.itcdsc + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = '<?php echo $urlGuia;?>';
                        var action = '&req=remover&itcid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (resp != 'erro') {
                                    msgSuccess(caminho, "Item de Composição removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover o Item de Composição.", "error");
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
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagem' + params;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#itensComposicaoListagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar-item").click(function (evt) {
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        evt.preventDefault();
//        var caminho = '<?php //echo $urlGuia;?>//';
//        var action = 'req=pesquisar&' + $("#form-filtro").serialize();
//        $.ajax({
//            type: "GET",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#itensComposicaoListagem").html(resp);
//                return true;
//            }
//        });
    });

    jQuery("#limpar-formfiltro").click(function () {
        window.location.assign(caminho);
        $('#itcdsc').val('');
        $('#codigo').val('');
        $("#intoid").val('null').trigger('chosen:updated');
        $("#ctdid").val('').trigger('chosen:updated');
        $("#uniid").val('').trigger('chosen:updated');
        $("#form-filtro").find('#itcsituacao1').removeAttr("checked");
        $("#form-filtro").find('#itcsituacao2').removeAttr("checked");
        $("#form-filtro").find('#itcsituacao3').prop('checked', true);

    });
</script>
