<?php

include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
$controllerUnidadeMedida = new Par3_Controller_UnidadeMedida();
$controllerClassUnMedida = new Par3_Controller_ClasseUnidadeMedida();

function retornaRelatorio($arrPost)
{
    $controllerClassUnMedida = new Par3_Controller_UnidadeMedida();
    $controllerClassUnMedida->listar($arrPost);
}
switch ($_REQUEST['requisicao']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerUnidadeMedida->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'salvarClasseUnidadeMedida':
        ob_clean();
        $resp = $controllerClassUnMedida->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'formUnidadeMedida':
        ob_clean();
        echo $controllerUnidadeMedida->formUnidadeMedida($_REQUEST);
        die;
        break;
    case 'listarUnidadeMedida':
        ob_clean();
        echo $controllerUnidadeMedida->listar($_REQUEST);
        die;
        break;
    case 'formClasseUnidadeMedida':
        ob_clean();
        echo $controllerClassUnMedida->formClasseUnidadeMedida($_REQUEST);
        die;
        break;
    case 'checkclasse':
        ob_clean();
        $controllerUnidadeMedidaClasse = new Par3_Controller_UnidadeMedidaClasse();
        echo $controllerUnidadeMedidaClasse->salvar($_REQUEST);
        die;
        break;
    case 'marcartodos':
        ob_clean();
        $controllerUnidadeMedidaClasse = new Par3_Controller_UnidadeMedidaClasse();
        echo $controllerUnidadeMedidaClasse->marcarTodos($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerUnidadeMedida->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        $controllerUnidadeMedida->listar($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerUnidadeMedida->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Unidade_de_Medida.xls");
        die;
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerUnidadeMedida->recuperar($_REQUEST);
        die;
        break;
    case 'recuperarclasse':
        ob_clean();
        echo $controllerClassUnMedida->recuperar();die;
        break;
    case 'removerclasse':
        ob_clean();
        $retorno = $controllerClassUnMedida->remover($_REQUEST);
        echo simec_json_encode($retorno);die;
        break;
    default:
        break;
}
?>


<div class="ibox">
    <div class="ibox" id="painelItens">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-12">
                    <h3 class="center">Unidade de Medida</h3>
                </div>
            </div>
        </div>
    <div class="ibox-content">

        <form method="post" name="formUnidadeMedida" id="formUnidadeMedida" class="form form-horizontal">
            <input type="hidden" name="action" id="action" value="salvar"/>
            <input type="hidden" name="pk" id="action" value=""/>
            <div class="row">
                <div class="col-lg-10 col-md-10 center" id="divFormUnidadeMedida">
                <?php echo $controllerUnidadeMedida->formUnidadeMedida($arrPost)?>
                </div>
                <div class="text-center col-md-12">
                    <div class="center">
                        <button type="button" class="btn btn-primary" id="pesquisar-unidademedida"><i class="fa fa-search"></i>Pesquisar</button>
                        <button type="button" class="btn btn-success" id="btn-incluir"><i class="fa fa-plus"></i> Incluir</button>
                        |
                        <button type="button" class="btn btn-primary xls" id="xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> XLS</button>
                        <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                        |
                        <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
<!--                        <button type="button" class="btn btn-danger" id="imprimir" title="Limpar Formulário">-->
<!--                            <i class="fa fa-eraser"></i>-->
<!--                        </button>-->
                    </div>
                </div>
            </div>
        </form>
    </div>
    <div class="ibox-content">
        <button type="button" class="btn btn-success" id="btn-incluirClasseUnidadeMedida" data-toggle="modal" data-target="#modalClasseUnidadeMedida">
            <i class="fa fa-plus"></i> Incluir Classe
        </button>
    </div>
        <div class="ibox-title">
            <div class="ibox-tools left">
                <a id="btn-avancado"><i id="chevron" class="fa fa-chevron-down"></i>Lista de Classes</a>
            </div>
        </div>
    <div class="ibox-footer" id="classeUnidadeMedidaListagem" style="display: none;">
        <?php echo $controllerClassUnMedida->listar($arrPost)?>
    </div>
    <div class="ibox-footer">
        <div id="unidadeMedidaListagem">
            <?php echo $controllerUnidadeMedida->listar($arrPost)?>
        </div>
    </div>
</div>

<!--modal-->
<div class="ibox float-e-margins animated modal" id="modalClasseUnidadeMedida" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formClasseUnidadeMedida" id="formClasseUnidadeMedida" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Classe Unidade de Medida - Incluir</h3>
                </div>
                <div class="modal-body" id="div-body-modalClasseUnidadeMedida">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="btn-cancelarClasseUnidadeMedida" data-dismiss="modal" class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="btn-salvarClasseUnidadeMedida" class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
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
                    Relatórios Ciclos PAR<br>
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
<script src="js/par3.js"></script>
<script>
    //mostra/esconde ibox
    $("#btn-avancado").click(function () {
        $('#classeUnidadeMedidaListagem').slideToggle();
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

    jQuery(function () {
        $('.i-checks').iCheck({//Função para utilizar o checkbox estilizado
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green',
        });
        //confirmação ao salvar ciclo
        jQuery('#btn-incluir').click(function (evt) {
            evt.preventDefault();
            var uniid = $('#formUnidadeMedida').find('[name=uniid]').val();
            var msg = 'Incluir';
            if (uniid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> esta Unidade de Medida?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarUnidadeMedida();
                    }
                    return false;
                }
            );
        });

        function salvarUnidadeMedida() {
            var caminho = window.location.href;
            var action = '&requisicao=salvar&' + $("#formUnidadeMedida").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
//                        swal("Sucesso.", "Unidade de Medida salva com sucesso.", "success");
                        msgSuccess(caminho,"Unidade de Medida salva com sucesso.");
                        atualizarForm();
                        listarUnidadeMedida();
                        $("#btn-incluir").html("<span class='fa fa-plus'></span>Incluir Unidade de Medida");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#formUnidadeMedida").find("[name=" + err + "]").parent("div");
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

        jQuery('#btn-salvarClasseUnidadeMedida').click(function (evt) {
            evt.preventDefault();
            var uniid = $('#formClasseUnidadeMedida').find('[name=cnid]').val();
            var msg = 'Incluir';
            if (uniid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> esta Classe de Unidade de Medida?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarClasseUnidadeMedida();
                    }
                return false;
                }
            );
        });

        function salvarClasseUnidadeMedida() {
            var caminho = window.location.href;
            var action = '&requisicao=salvarClasseUnidadeMedida&' + $("#formClasseUnidadeMedida").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        $("#modalClasseUnidadeMedida").modal("hide");
                        $("#div-body-modalClasseUnidadeMedida").html("");
//                        swal("Sucesso.", "Classe de Unidade de Medida salva com sucesso.", "success");
                        listarUnidadeMedida();
                        carregarformClasseUnidadeMedida();
                        msgSuccess(caminho,"Classe de Unidade de Medida salva com sucesso.");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#formClasseUnidadeMedida").find("[name=" + err + "]").parent("div");
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
    });

    $("#pesquisar-unidademedida").click(function () {
//         evt.preventDefault();
        var caminho = window.location.href;
        var action = '&requisicao=pesquisar&' + $("#formUnidadeMedida").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#unidadeMedidaListagem").html(resp);
                return true;
            }
        });
    });

    $("#xls").click(function(){
        console.log('clicou aqui');
    });
    $(document).on("click","#xls", function () {
        console.log('clicou aqui');
        window.location.assign(window.location.href + "&requisicao=xls&" + jQuery('#formUnidadeMedida').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {//formata e dá print de uma tabela em PDF
        var caminho = window.location.href;
        var action = '&requisicao=imprimir&' + $("#formUnidadeMedida").serialize() + '&' + $('.form-listagem').serialize();
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
                w.document.title = 'Relatório - Unidades de Medida';
                w.print();
                w.close();
                $("#impressao-content").html("");
                listarUnidadeMedida($("#divFormUnidadeMedida").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    function atualizarForm(params = "") {
        if (params) {
            params = '&' + params
        }

        var caminho = window.location.href;
        var action = '&requisicao=formUnidadeMedida' + params;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#divFormUnidadeMedida").html(resp);
                return true;
            }
        });
    }

    $('#btn-incluirClasseUnidadeMedida').click(function(resp){
        carregarformClasseUnidadeMedida();
    });

    function carregarformClasseUnidadeMedida(params ="")
    {
        $('#modalClasseUnidadeMedida').find('#modal_titulo').html('Classe Unidade de Medida - Inserir');
        if (params) {
            params = '&' + params;
            $('#modalClasseUnidadeMedida').find('#modal_titulo').html('Classe Unidade de Medida - Editar');
        }
        var caminho = window.location.href;
        var action = '&requisicao=formClasseUnidadeMedida' + params;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#div-body-modalClasseUnidadeMedida").html(resp);
                return true;
            }
        });
    }

    function salvarUnidadeClasse(cniid,uniid)
    {
        var caminho = window.location.href;
        var action = '&requisicao=checkclasse&cniid=' + cniid+'&uniid='+uniid;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                listarUnidadeMedida();
            }
        });
    }

    function marcarTodos(marcado,uniid)
    {
        console.log(marcado+' '+uniid);
        var marcar = '&desmarcar=T';
        if(marcado == 'f'){
            marcar = '&desmarcar=F';
        }
        var caminho = window.location.href;
        var action = '&requisicao=marcartodos&uniid='+uniid+marcar;
        console.log(caminho+' '+action);
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                listarUnidadeMedida();
            }
        });
    }

    function removerUnidadeMedida(id) {
        var caminho = window.location.href;
        var action = '&requisicao=recuperar&uniid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var uni = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Unidade de Medida",
                    text: "Tem certeza que deseja remover: <b>" + uni.uniid + " - " + uni.unidsc + "</b> ?",
                    type: "warning",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&requisicao=remover&uniid=' + id;
                        $.ajax({
                            type: "GET",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (resp != 'erro') {
//                                    swal("Sucesso.", "Unidade de Medida removido com sucesso.", "success");
                                    atualizarForm();
                                    listarUnidadeMedida();
                                    msgSuccess(caminho,"Unidade de Medida removido com sucesso.", "success");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover Unidade de Medida.", "error");
                            }
                        });
                    }
                })
            }
        });
    }


    function editarUnidadeMedida(id) {
        var caminho = window.location.href;
        var action = '&requisicao=formUnidadeMedida&uniid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#btn-incluir").html("<span class='fa fa-pencil'></span>Editar");
                $("#divFormUnidadeMedida").html(resp);
            }
        });
    }

    function listarUnidadeMedida(params = '')
    {

        if(params){
            params = '&' + params;
        }
        var caminho = window.location.href;
        var action = '&requisicao=listarUnidadeMedida'+params;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (listagem) {
                $('#unidadeMedidaListagem').html(listagem);
                return false;
            }
        });
    }

    function editarClasse(id) {
        var cni = 'cniid='+id;
        carregarformClasseUnidadeMedida(cni);
        var modalClasse  =  $('#modalClasseUnidadeMedida');
        modalClasse.modal('show');
    }

    function removerClasse(id) {
        var caminho = window.location.href;
        var action = '&requisicao=recuperarclasse&cniid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var cni = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Classe da Unidade de Medida",
                    text: "Tem certeza que deseja remover: <b>" + cni.cniid + " - " + cni.cnidsc + "</b> ?",
                    type: "warning",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&requisicao=removerclasse&cniid=' + id;
                        $.ajax({
                            type: "GET",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                var res = $.parseJSON(resp);
                                if (!isNaN(parseInt(res)) && isFinite(res)) {
//                                    swal("Sucesso.", "Unidade de Medida removido com sucesso.", "success");
                                    atualizarForm();
                                    listarUnidadeMedida();
                                    msgSuccess(caminho,"Unidade de Medida removido com sucesso.", "success");
                                    return false;
                                }
                                console.log(res);
                                swal("Erro.", res.erro, "error");
                            }
                        });
                    }
                })
            }
        });
    }

    jQuery("#limpar").click(function(){
        atualizarForm();
        $("#btn-incluir").html("<span class='fa fa-plus'></span>Incluir");
    });
</script>