<?php

//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/IniciativaDesdobramentos.class.inc';
include_once APPRAIZ . 'par3/classes/controller/IniciativaDesdobramentosTipo.class.inc';
include_once APPRAIZ . 'par3/classes/model/IniciativaDesdobramentosTipo.class.inc';

$controllerDesdobramentoTipo = new Par3_Controller_IniciativaDesdobramentosTipo();
$rsTipoDesdobramento = $controllerDesdobramentoTipo->pegarSelectCombo();

$controllerIniciativaDesdobramentos = new Par3_Controller_IniciativaDesdobramentos();


function retornaRelatorio($arrPost)
{
    $controllerIniciativaDesdobramentos = new Par3_Controller_IniciativaDesdobramentos();
    $controllerIniciativaDesdobramentos->listaIniciativaDesdobramentos($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvarDesdobramento':
        ob_clean();
        $resp = $controllerIniciativaDesdobramentos->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagemDesdobramento':
        ob_clean();
        echo $controllerIniciativaDesdobramentos->listaIniciativaDesdobramentos($_REQUEST);
        die;
        break;
    case 'removerDesdobramento':
        ob_clean();
        echo $controllerIniciativaDesdobramentos->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaDesdobramentos->listaIniciativaDesdobramentos($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaDesdobramentos->listaIniciativaDesdobramentos($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Desdobramentos.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
//        $controllerIniciativaDesdobramentos->listaIniciativaDesdobramentos($_REQUEST);
        die();
        break;
    default:
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaDesdobramentos->recuperar($_REQUEST);
        die;
        break;
    case 'form':
        ob_clean();
        $mDesdobramento = $controllerIniciativaDesdobramentos->recuperar($_REQUEST['desid']);
        require_once 'desdobramentos/formDesdobramento.inc';
        die;
        break;
    case 'salvarDesTipo':
        ob_clean();
        $resp = $controllerDesdobramentoTipo->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'recuperarDesTipo':
        ob_clean();
        $mDesdobramentoTipo = $controllerDesdobramentoTipo->recuperar($_REQUEST['tipid']);
        require_once 'desdobramentos/desdobramentos_tipo.inc';
        die;
    case 'recuperarDesTipoObj':
        ob_clean();
        $mDesdobramentoTipo = $controllerDesdobramentoTipo->recuperar($_REQUEST['tipid']);
        echo $mDesdobramentoTipo->tipid;
        die;
        break;
    case 'listar':
        ob_clean();
        echo $controllerDesdobramentoTipo->listar($_REQUEST);
        die;
        break;
    case 'atualizarListagemDesTipo':
        $rsTipoDesdobramento = $controllerDesdobramentoTipo->pegarSelectCombo();
        $tipid = isset($_REQUEST['tipid'])?$_REQUEST['tipid']:'';
        ob_clean();
        echo $simec->select('tipid', '',$tipid, $rsTipoDesdobramento, '',array('input-size'=>12));
        die;
        break;
    case 'atualizarListagemDesTipoPesquisa':
        $rsTipoDesdobramento = $controllerDesdobramentoTipo->pegarSelectCombo();
        ob_clean();
        echo $simec->select('tipodesc', 'Tipo', '', $rsTipoDesdobramento, array('maxlength' => '255'), array('input-size' => '5'));
        die;
        break;
    case 'atualizartipodsc':
        ob_clean();
        echo $simec->select('tipodesc', 'Tipo', '', $rsTipoDesdobramento, array('maxlength' => '255'), array('input-size' => '5'));
        die;
        break;
    case 'verificaRemoverDesdobramentos':
        ob_clean();
        if(isset($_POST['desid'])){
            $verificaExclusao =  $controllerIniciativaDesdobramentos->verificaLigacaoPlanejamentoById($_POST['desid']);
            if(isset($verificaExclusao[0])){
                $getIniciativaDesdobramento = $controllerIniciativaDesdobramentos->getIniciativaDesdobramentoById($_POST['desid']);
                echo "Não é possível excluir o Desdobramento ".$getIniciativaDesdobramento['desid']." - ".$getIniciativaDesdobramento['desdsc'].", pois o mesmo já foi utilizado no planejamento";
            }else{
                echo "false";
            }
        }
        die;
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
                <h3 class="center">Desdobramentos</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group"><label class="col-lg-3 col-md-3 control-label">Desdobramento:</label>
                            <div class="col-sm-7 col-md-8"><input type="text" name="desdsc" id="desdsc" maxlength="255" class="form-control" value="<?php echo ($_GET['desdsc']); ?>"/></div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-md-2 control-label">Código:</label>
                                    <div class="col-md-5 col-lg-5">
                                        <input type="number" name="desid" id="desid" class="form-control" value="<?php echo $_GET['desid'];?>"/>
                                    </div>
                                </div>
                                <div id="tipodesc-div">
                                    <?php
                                    echo $simec->select('tipodesc', 'Tipo', $_GET['tipodesc'], $rsTipoDesdobramento, array('maxlength' => '255'), array('input-size' => '5'));
                                    ?>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label"> Situação:</label>
                                    <div class="col-md-8">
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="dessituacao" id="dessituacao1" <?php echo ($_GET['dessituacao'] == 'A'?'checked':'');?> value="A" >
                                            <label for="dessituacao1"> Ativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="dessituacao" id="dessituacao2" <?php echo ($_GET['dessituacao'] == 'I'?'checked':'');?> value="I" >
                                            <label for="dessituacao2"> Inativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="dessituacao" id="dessituacao3" <?php echo ($_GET['dessituacao'] == ''?'checked':'');?> value="" >
                                            <label for="dessituacao3"> Todos </label>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="ibox-title">
                                <div class="ibox-tools">
                                    <a id="btn-avancado">
                                        Opções Avançadas <i id="chevron" class="fa fa-chevron-down"></i>
                                    </a>
                                </div>
                            </div>
                            <br>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-desdobramento"><i class="fa fa-search"></i>
                    Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal"
                        data-target="#modal_desdobramento">
                    <i class="fa fa-plus"></i> Incluir
                </button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="desdobramentoListagem">
            <?php
            $controllerIniciativaDesdobramentos->listaIniciativaDesdobramentos($_REQUEST);
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
<div class="ibox float-e-margins animated modal" id="modal_desdobramento" tabdesex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formDesdobramento" id="formDesdobramento" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Desdobramento - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal"></div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDesdobramento" data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarDesdobramento" class="btn btn-success" <?php echo $disabled; ?>>
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
    jQuery(function () {

        jQuery(document).find("#formDesdobramento").on('click', "input[name=dessituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

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

        $('#btn-incluir').click(function () {
            form();
        });
    });


    $(".xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
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
                w.document.title = 'Relatório - Desdobramentos';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagemDesdobramento($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    function form(id = null) {
        var desid = '';
        if (id) {
            desid = '&desid=' + id;
            $("#modal_titulo").html("Desdobramento - Editar");
        }

        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=form' + desid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
                $("#modal_desdobramento").modal("show");
                $('#salvarDesdobramento').removeAttr('disabled');
            }
        });
    }

    function salvarDesdobramentoTipo() {
        var caminho = '<?php echo $urlGuia;?>';
        var formTipoDesdobramento = $("#formTipoDesdobramento").serialize();
        var action = '&req=salvarDesTipo&' + formTipoDesdobramento;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = jQuery.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    swal("Sucesso.", "Tipo de Desdobramento salvo com sucesso.", "success");
                    $('#salvarDesdobramento').removeAttr('disabled');
                    $('#formTipoDesdobramento').fadeOut();
                    $('#btnAcaoTipo').addClass();
                    atualizarListagemDesTipo(formTipoDesdobramento);
                    atualizarListagemDesTipoPesquisa();
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formTipoDesdobramento").find("#inputgrouptipid");
                        divFormInput.addClass('has-error');
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                    });
                });
                return false;
            }
        });
        return false;
    }

    jQuery('#salvarDesdobramento').click(function (evt) {
        evt.preventDefault();
        var desid = $('#conteudo-modal').find('[name=desid]').val();
        var msg = 'Incluir';
        if (desid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Desdobramento?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarDesdobramento();
                }
                return false;
            }
        );
    });

    function salvarDesdobramento() {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=salvarDesdobramento&' + $("#formDesdobramento").serialize()+'&'+$('#selectDesdobramentoTipo').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    $("#modal_desdobramento").modal("hide");
                    $("#modal_desdobramento").find("input").val("");
                    msgSuccess(caminho,"Desdobramento salvo com sucesso");
                    //swal("Sucesso.", "Desdobramento salvo com sucesso.", "success");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        if(err == 'tipid'){
                            var tipidlabel = $("#formDesdobramento").find("#tipidlabel");
                            tipidlabel.addClass('has-error');
                            tipidlabel.append("<div id='div-" + err + "'></div>");
                            $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                            tipidlabel.show();
                        }
                        var divFormInput = $("#modal_desdobramento").find("input[name=" + err + "]").parent("div");
                        divFormInput.closest('.form-group').addClass('has-error');
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                    });
                });
                return false;
            }
        });
        return false;
    }

    $("#pesquisar-desdobramento").click(function (evt) {
        evt.preventDefault();
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#desdobramentoListagem").html(resp);
//                return true;
//            }
//        });
    });

    function removerDesdobramento(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&desid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var des = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Desdobramento",
                    text: "Tem certeza que deseja remover: <b>" + des.desid + " - " + des.desdsc + "</b> ?<br> Ao confirmar a operação, o desdobramento não poderá ser utilizado em novas iniciativas.",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=removerDesdobramento&desid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_desdobramento").modal("hide");
                                $("#modal_desdobramento").find("input").val("");
                                if (!isNaN(parseInt(resp)) && isFinite(resp)) {
                                    msgSuccess(caminho,"Desdobramento removido com sucesso");
                                    return false;
                                }
                                swal("Erro.",resp, "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarListagemDesTipo(params = '') {

        if(params){
            params = '&' + params;
        }
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagemDesTipo'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (form) {
                $('#selectDescTipo').html(form);
                return false;
            }
        });
    }

    function atualizarListagemDesTipoPesquisa() {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagemDesTipoPesquisa';
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (form) {
                $('#tipodesc-div').html(form);
                return false;
            }
        });
    }

    function editarDesdobramento(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=form&desid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Desdobramento - Editar");
                $("#modal_desdobramento").modal("show");
            }
        });
    }
    $("#modal_desdobramento").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Desdobramento - Incluir");
    });

    function atualizarListagemDesdobramento(params = '') {
        if (params) {
            params = '&' + params;
        }
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagemDesdobramento' + params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (form) {
                $('#desdobramentoListagem').html(form);
                return false;
            }
        });
    }

    function atualizarFormFiltro()
    {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizartipodsc';
        $("#form-filtro").find('[name=desdsc]').val("");
        $("#form-filtro").find('[name=desid]').val("");
        $("#form-filtro").find('#tipodesc-div').load(caminho+action);
        $("#form-filtro").find('#dessituacao1').removeAttr("checked");
        $("#form-filtro").find('#dessituacao2').removeAttr("checked");
        $("#form-filtro").find('#dessituacao3').prop('checked',true);
    }

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#desdsc').val('');
        $('#desid').val('');

       $("#tipodesc").val('').trigger('chosen:updated');

        $("#form-filtro").find('#dessituacao1').removeAttr("checked");
        $("#form-filtro").find('#dessituacao2').removeAttr("checked");
        $("#form-filtro").find('#dessituacao3').prop('checked',true);

    });

    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param desid
     * @description <esta função é utilizada para verificar se o desdobramento já está ligado ao planejamento, inviabilizando sua exclusão caso exista a ligação>
     */
    function js_verificaRemoverDesdobramento(desid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaRemoverDesdobramentos&desid='+desid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if (resp == 'false') {
                    removerDesdobramento(desid);
                } else {
                    swal("Erro.", resp, "error");
                }
            }
        });
    }
</script>