<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/Modalidade.class.inc';
include_once APPRAIZ . 'par3/classes/model/EnsinoNivel.inc';

$controllerModalidade = new Par3_Controller_Modalidade();


function retornaRelatorio($arrPost)
{
    $controllerModalidade = new Par3_Controller_Modalidade();
    $controllerModalidade->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerModalidade->salvar($_REQUEST);
        echo simec_json_encode($resp);
//        ver($resp,simec_json_encode($resp),d);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerModalidade->listar($_REQUEST);
        die;
        break;
    case 'editarModalidade':
        ob_clean();
        echo $controllerModalidade->form($_REQUEST);
        die;
        break;
    case 'desvincular':
        ob_clean();
        echo $controllerModalidade->desvincular($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerModalidade->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerModalidade->listar($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerModalidade->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        echo $controllerModalidade->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Modalidades.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    default:
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerModalidade->recuperar($_REQUEST);
        die;
        break;
    case 'listarEtapas':
        $mModelo = new Par3_Model_EnsinoEtapa();
        ob_clean();
        if ($_REQUEST['nivid']) {
            echo simec_json_encode($mModelo->lista(array('etaid', 'etadsc'), array("nivid in ({$_REQUEST['nivid']})")));
        }
        die;
        break;
    case 'atualizarniveletapa':
        $modelNivel = new Par3_Model_EnsinoNivel();
        $sqlNivel = $modelNivel->pegarSQLSelectCombo();
        ob_clean();
        echo '<div id="nivel-div">';
        echo $simec->select('nivel', 'Nível', '', $sqlNivel, array('maxlength' => '255'), array('input-size' => '5'));
        echo '</div>';
        echo '<div id="etapa-div">';
        echo $simec->select('etapa', 'Etapa', '', array(), array('maxlength' => '255'), array('input-size' => '5'));
        echo '</div>';
        die;
        break;
    case 'verificaRemoverModalidade':
        ob_clean();
            if(isset($_POST['modid'])){
                $verificaExcluir = $controllerModalidade->verificaLigacaoPlanejamento($_POST['modid']);

                if(isset($verificaExcluir[0])){
                    $getModalidade = $controllerModalidade->getModalidadeById($_POST['modid']);
                    //ver($getModalidade);
                    echo "Não é possível excluir a Modalidade ".$getModalidade['modid']." - ".$getModalidade['moddsc'].", pois a mesma já foi utilizada no Planejamento!";
                }else{
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

$modelNivel = new Par3_Model_EnsinoNivel();
$sqlNivel = $modelNivel->pegarSQLSelectCombo();
$modelEtapa = new Par3_Model_EnsinoEtapa();
$sqlEtapa = $modelEtapa->pegarSelectCombo();
global $simec;
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Modalidades</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Modalidades</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="moddsc" id="moddsc" maxlength="255" class="form-control" value="<?php echo ($_GET['moddsc']); ?>"/>
                            </div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                    <div class="col-md-5  col-md-5 col-lg-5">
                                        <input type="number" name="modid" id="modid" class="form-control" value="<?php echo $_GET['modid']; ?>"/>
                                    </div>
                                </div>
                                <div id="nivel-etapa-div">
                                    <div id="nivel-div">
                                        <?php
                                        echo $simec->select('nivel', 'Nível',$_GET['nivel'], $sqlNivel, array('maxlength' => '255'), array('input-size' => '5'));
                                        ?>
                                    </div>
                                    <div id="etapa-div">
                                        <?php
                                        echo $simec->select('etapa', 'Etapa',$_GET['etapa'], $sqlEtapa, array('maxlength' => '255'), array('input-size' => '5'));
                                        ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Situação:</label>
                                    <div class="col-lg-8">
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="modsituacao" id="modsituacao1" <?php echo ($_GET['modsituacao'] == 'A'?'checked':'');?> value="A">
                                            <label for="modsituacao1">Ativo</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="modsituacao" id="modsituacao2" <?php echo ($_GET['modsituacao'] == 'I'?'checked':'');?> value="I">
                                            <label for="modsituacao2">Inativo</label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="modsituacao" id="modsituacao3" <?php echo ($_GET['modsituacao'] == ''?'checked':'');?> value="">
                                            <label for="modsituacao3">Todos</label>
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
                <button type="button" class="btn btn-primary" id="pesquisar-modalidade"><i class="fa fa-search"></i>
                    Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal"
                        data-target="#modal_modalidade">
                    <i class="fa fa-plus"></i> Incluir
                </button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>

            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="modalidadeListagem">
            <?php
            $controllerModalidade->listar($_REQUEST);
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
                    Relatório Modalidades<br>
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
<div class="ibox float-e-margins animated modal" id="modal_modalidade" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formModalidade" id="formModalidade" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Modalidade - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php echo $controllerModalidade->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarModalidade"
                                data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarModalidade"
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

    $(document).on('change click', "#nivid", function () {
        var nivel_id = $("#nivid").val();
        var caminho = window.location.href;
        var options = $("#etaid");
        var action = '&req=listarEtapas&nivid=' + nivel_id;
        $.getJSON(caminho + action, function (dados) {
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                $.each(dados, function (index, etapa) {
                    options.append(new Option(etapa.etadsc, etapa.etaid));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();

    $("#form-filtro").on('change', "#nivel", function () {
        var nivel_id = $("#nivel").val();
        var options = $("#etapa");
        var action = '&req=listarEtapas&nivid=' + nivel_id;
        $.getJSON(caminho + action, function (dados) {
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                options.append(new Option('Todos', '', 'selected'));
                $.each(dados, function (index, etapa) {
                    options.append(new Option(etapa.etadsc, etapa.etaid));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();

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

        jQuery(document).find("#formModalidade").on('click', "input[name=modsituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

        jQuery('#salvarModalidade').click(function (evt) {
            evt.preventDefault();
            var inprgid = $('#conteudo-modal').find('[name=modid]').val();
            var msg = 'Incluir';
            if (inprgid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> esta Modalidade?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarModalidade();
                    }
                    return false;
                }
            );
        });

        function salvarModalidade() {
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=salvar&' + $("#formModalidade").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        $("#modal_modalidade").modal("hide");
                        $("#modal_modalidade").find("input").val("");
                        msgSuccess(caminho,"Modalidade Salva com sucesso");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#modal_modalidade").find("#" + err).parent("div");
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
        window.location.assign(window.location.href + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
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
                w.document.title = 'Relatório - Descrição de Iniciativas';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    $("#btn-incluir").click(function (evt) {
        evt.preventDefault();
        atualizarForm();
        $("#modal_modalidade").modal("show");
    });

    function editarModalidade(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=editarModalidade&modid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Modalidade - Editar");
                $("#modal_modalidade").modal("show");
            }
        });
    }
    $("#modal_modalidade").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Modalidade - Incluir");
    });

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
                $("#modalidadeListagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar-modalidade").click(function (evt) {
        evt.preventDefault();
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#modalidadeListagem").html(resp);
//                return true;
//            }
//        });
    });

    function removerModalidade(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&modid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var mod = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Modalidade",
                    text: "Tem certeza que deseja remover: <b>" + mod.modid + " - " + mod.moddsc + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&modid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_modalidade").modal("hide");
                                $("#modal_modalidade").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho,"Modalidade removida com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover a Modalidade.", "error");
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
        $("#form-filtro").find('[name=moddsc]').val("");
        $("#form-filtro").find('#nivel-etapa-div').load(caminho+action);
        $("#form-filtro").find('[name=nivel]').find("option:selected").removeAttr("selected");
        $("#form-filtro").find('[name=modid]').val("");
        $("#form-filtro").find('#modsituacao1').removeAttr("checked");
        $("#form-filtro").find('#modsituacao2').removeAttr("checked");
        $("#form-filtro").find('#modsituacao3').prop('checked',true);
    }

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#moddsc').val('');
        $('#modid').val('');

        $("#form-filtro").find('#modsituacao1').removeAttr("checked");
        $("#form-filtro").find('#modsituacao2').removeAttr("checked");
        $("#form-filtro").find('#modsituacao3').prop('checked',true);

        $("#nivel").val('').trigger('chosen:updated');
        $("#etapa").val('').trigger('chosen:updated');
    });


    function js_verificaRemoverModalidade(modid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaRemoverModalidade&modid='+modid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if(resp != "false"){
                    swal("Erro.", resp, "error");
                }else{
                    removerModalidade(modid);
                }

                return true;
            }
        });
    }
</script>