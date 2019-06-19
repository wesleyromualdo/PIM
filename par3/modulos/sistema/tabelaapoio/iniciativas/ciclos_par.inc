<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/IniciativaDescricaoController.class.inc';

$controllerCiclo = new Par3_Model_CicloParController();


function retornaRelatorio($arrPost)
{
    $controllerCiclo = new Par3_Model_CicloParController();
    $controllerCiclo->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerCiclo->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerCiclo->listar($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerCiclo->form($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerCiclo->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerCiclo->listar($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerCiclo->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerCiclo->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Ciclo_do_PAR.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'remover':
        $controllerCiclo->inativar($_REQUEST);
        break;
    default:
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerCiclo->recuperar($_REQUEST);
        die;
        break;
        case 'verificaLigacaoComPlanejamento':
        ob_clean();
            if(isset($_POST['cicid'])){
                $resultVerifica = $controllerCiclo->verificaLigacaoPlanejamento($_POST['cicid']);
                if(isset($resultVerifica[0])){
                    //não pode apagar pois já está vinculado ao planejamento
                    echo "Não é possível excluir o Ciclo: ".$resultVerifica[0]['cicid']." - ".$resultVerifica[0]['cicdsc'].", pois o mesmo já foi utilizado no planejamento";
                }else{
                    //pode ser excluído pois ainda não existe nenhum vinculo com o planejamento
                    echo "false";
                }
            }
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>

<?php //echo $simec->title('Tabela Apoio - Ciclos do PAR', 'Sistema / Tabela de Apoio / Iniciativas do PAR / Ciclos do PAR'); ?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Ciclos PAR</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Ciclo:</label>
                            <div class="col-sm-7 col-md-8"><input type="text" name="cicdsc" id="cicdsc" value="<?php echo ($_GET['cicdsc']);?>" maxlength="10" class="form-control"/></div>
                        </div>
                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Ano:</label>
                            <div class="col-sm-7 col-md-5"><input type="number" name="ano" id="ano" max="4" value="<?php echo $_GET['ano']?>" class="form-control"/></div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                    <div class="col-sm-7 col-md-5">
                                        <input type="number" name="cicid" id="cicid" <?php echo $_GET['cicid']?> class="form-control"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Vigência:</label>
                                    <div class="col-lg-8">
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicvigencia" id="cicvigencia1" <?php echo ($_GET['cicvigencia'] == 'VIGENTE'?'checked':'');?> value="VIGENTE">
                                            <label for="cicvigencia1">Vigente</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicvigencia" id="cicvigencia2" <?php echo ($_GET['cicvigencia'] == 'FECHADO'?'checked':'');?> value="FECHADO">
                                            <label for="cicvigencia2">Fechado</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicvigencia" id="cicvigencia3" <?php echo ($_GET['cicvigencia'] == 'NAO_INICIADO'?'checked':'');?> value="NAO_INICIADO">
                                            <label for="cicvigencia3">Não Iniciado</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicvigencia" id="cicvigencia4" <?php echo ($_GET['cicvigencia'] == ''?'checked':'');?> value="">
                                            <label for="cicvigencia4">Todos</label>
                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Situação:</label>
                                    <div class="col-md-5 col-lg-5">
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicsituacao" id="cicsituacao1" <?php echo ($_GET['cicsituacao'] == 'A'?'checked':'');?> value="A">
                                            <label for="cicsituacao1">Ativo</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicsituacao" id="cicsituacao2" <?php echo ($_GET['cicsituacao'] == 'I'?'checked':'');?> value="I">
                                            <label for="cicsituacao2">Inativo</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="cicsituacao" id="cicsituacao3" <?php echo ($_GET['cicsituacao'] == ''?'checked':'');?> value="" >
                                            <label for="cicsituacao3">Todos</label>
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
                <button type="button" class="btn btn-primary" id="pesquisarCiclo"><i class="fa fa-search"></i> Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal"
                        data-target="#modal_ciclo">
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
        <div id="listagemCiclo">
            <?php
            $controllerCiclo->listar($_REQUEST);
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
<br>
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_ciclo" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formCiclo" id="formCiclo" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Ciclo do PAR - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php echo $controllerCiclo->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarCiclo"
                                data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarCiclo"
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
    var caminho = '<?php echo $urlGuia;?>';

    jQuery(function () {

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

        jQuery(document).find("#formCiclo").on('click', "input[name=cicsituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

        //confirmação ao salvar ciclo
        jQuery('#salvarCiclo').click(function (evt) {
            evt.preventDefault();
            var inprgid = $('#conteudo-modal').find('[name=cicid]').val();
            var msg = 'Incluir';
            if (inprgid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> este Ciclo?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarCiclo();
                    }
                    return false;
                }
            );
        });

        function salvarCiclo() {
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=salvar&' + $("#formCiclo").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        $("#modal_ciclo").modal("hide");
                        $("#modal_ciclo").find("input").val("");
                        msgSuccess(caminho,"Ciclo salvo com sucesso");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#modal_ciclo").find("[name=" + err + "]").parent("div");
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
    $(".xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {//formata e dá print de uma tabela em PDF
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
                w.document.title = 'Relatório - Ciclos do PAR';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    $("#modal_ciclo").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Ciclo do PAR - Incluir");
    });

    $("#btn-incluir").click(function (evt) {
        evt.preventDefault();
        atualizarForm();
        $("#modal_ciclo").modal("show");
    });

    function editarCiclo(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=editar&cicid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Ciclo do PAR - Editar");
                $("#modal_ciclo").modal("show");
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
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#listagemCiclo").html(resp);
                return true;
            }
        });
    }

    $("#pesquisarCiclo").click(function (evt) {
        evt.preventDefault();
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        var caminho = window.location.href;
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#listagemCiclo").html(resp);
//                return true;
//            }
//        });
    });

    function removerCiclo(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&cicid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var ind = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Ciclo PAR",
                    text: "Tem certeza que deseja remover: <b>" + ind.cicid + " - " + ind.cicdsc + "</b> ?",
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
                        var action = '&req=remover&cicid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_ciclo").modal("hide");
                                $("#modal_ciclo").find("input").val("");
                                if (resp != 'erro') {
                                    swal("Sucesso.", "Ciclo PAR removido com sucesso.", "success");
                                    msgSuccess(caminho,"Ciclo removida com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover Ciclo PAR.", "error");
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

        var caminho = '<?php echo $urlGuia;?>';
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

    function atualizarFormFiltro()
    {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarniveletapa';
        $("#form-filtro").find('[name=cicdsc]').val("");
        $("#form-filtro").find('[name=ano]').val("");
        $("#form-filtro").find('[name=cicid]').val("");
        $("#form-filtro").find('#cicsituacao1').removeAttr("checked");
        $("#form-filtro").find('#cicsituacao2').removeAttr("checked");
        $("#form-filtro").find('#cicsituacao3').prop('checked',true);
        $("#form-filtro").find('#cicvigencia1').removeAttr("checked");
        $("#form-filtro").find('#cicvigencia2').removeAttr("checked");
        $("#form-filtro").find('#cicvigencia3').removeAttr("checked");
        $("#form-filtro").find('#cicvigencia4').prop('checked',true);
    }

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#cicdsc').val('');
        $('#ano').val('');
        $('#cicid').val('');

        $("#form-filtro").find('#cicvigencia1').removeAttr("checked");
        $("#form-filtro").find('#cicvigencia2').removeAttr("checked");
        $("#form-filtro").find('#cicvigencia3').removeAttr("checked");
        $("#form-filtro").find('#cicvigencia4').prop('checked',true);

        $("#form-filtro").find('#cicsituacao1').removeAttr("checked");
        $("#form-filtro").find('#cicsituacao2').removeAttr("checked");
        $("#form-filtro").find('#cicsituacao3').prop('checked',true);

    });

    //função que verifica se o ciclo está ligado ao planejamento
    function js_verificaRemoverCiclo(cicid){
        if(cicid != ''){
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=verificaLigacaoComPlanejamento&cicid='+cicid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                   if(resp != 'false'){
                       swal("Erro.", resp, "error");
                   }else{
                       removerCiclo(cicid);
                   }
                    return true;
                }
            });
        }
    }

</script>