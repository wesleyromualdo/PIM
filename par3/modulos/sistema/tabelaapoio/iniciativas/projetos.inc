<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/Projeto.class.inc';

$controllerProjeto = new Par3_Controller_Projeto();


function retornaRelatorio($arrPost)
{
    $controllerProjeto = new Par3_Controller_Projeto();
    $controllerProjeto->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerProjeto->salvar($_REQUEST);
        echo simec_json_encode($resp);
//        ver($resp,simec_json_encode($resp),d);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerProjeto->listar($_REQUEST);
        die;
        break;
    case 'editarProjeto':
        ob_clean();
        echo $controllerProjeto->form($_REQUEST);
        die;
        break;
    case 'desvincular':
        ob_clean();
        echo $controllerProjeto->desvincular($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerProjeto->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerProjeto->listar($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerProjeto->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        echo $controllerProjeto->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Projetos.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
//        $controllerIniciativaDescricao->listaIniciativaDescricao($_REQUEST);
        die();
        break;
    case 'verificaRemoverProjeto':
        ob_clean();
        if(isset($_POST['proid'])){
            $verificaExclusao =  $controllerProjeto->verificaLigacaoPlanejamentoById($_POST['proid']);
            if(isset($verificaExclusao[0])){
                $getIniciativaProjeto = $controllerProjeto->getIniciativaProjetoById($_POST['proid']);
                echo "Não é possível excluir o Projeto ".$getIniciativaProjeto['proid']." - ".$getIniciativaProjeto['prodsc'].", pois o mesmo já foi utilizado no planejamento";
            }else{
                echo "false";
            }
        }
        die;
        break;
    default:
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerProjeto->recuperar($_REQUEST);
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>

<?php //echo $simec->title('Tabela Apoio - Projetos', 'Sistema / Tabela de Apoio / Projetos'); ?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Projetos</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Projetos:</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="prodsc" id="prodsc" maxlength="255" value="<?php echo ($_GET['prodsc']); ?>" class="form-control"/></div>
                        </div>
                        <!--filtros-->
                        <div id="avancado" style="display: none;">
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                <div class="col-sm-7 col-md-8">
                                    <input type="number" name="proid" id="proid" value="<?php echo $_GET['proid']; ?>" class="form-control"/>
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Sigla:</label>
                                <div class="col-sm-7 col-md-8">
                                    <input type="text" name="prosigla" id="prosigla" class="form-control" value="<?php echo ($_GET['prosigla']); ?>"/>
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Situação:</label>
                                <div class="col-lg-8">
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="prosituacao" id="prosituacao1" <?php echo ($_GET['prosituacao'] == 'A'?'checked':'');?> value="A">
                                        <label for="prosituacao1">Ativo</label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="prosituacao" id="prosituacao2" <?php echo ($_GET['prosituacao'] == 'I'?'checked':'');?> value="I">
                                        <label for="prosituacao2">Inativo</label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="prosituacao" id="prosituacao3" <?php echo ($_GET['prosituacao'] == ''?'checked':'');?> value="">
                                        <label for="prosituacao3">Todos</label>
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
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" class="btn btn-primary" id="pesquisar-projeto"><i class="fa fa-search"></i>
                        Pesquisar
                    </button>
                    <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal"
                            data-target="#modal_projeto">
                        <i class="fa fa-plus"></i> Incluir
                    </button>
                    |
                    <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                                class="fa fa-file-excel-o"></i> Gerar Excel
                    </button>
                    <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i
                                class="glyphicon glyphicon-print"></i> Imprimir
                    </button>
                    |
                    <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar
                    </button>

                </div>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="projetoListagem">
            <?php
            $controllerProjeto->listar($_REQUEST);
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
                    Relatório Projetos<br>
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
<div class="ibox float-e-margins animated modal" id="modal_projeto" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formProjeto" id="formProjeto" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Projeto - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php echo $controllerProjeto->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarProjeto"
                                data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="salvarProjeto"
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

        jQuery(document).find("#formProjeto").on('click', "input[name=prosituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará mais visível para selecinar", "warning");
        });

        //confirmação ao salvar ciclo
        jQuery('#salvarProjeto').click(function (evt) {
            evt.preventDefault();
            var inprgid = $('#conteudo-modal').find('[name=proid]').val();
            var msg = 'Incluir';
            if (inprgid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> este Projeto?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarProjeto();
                    }
                    return false;
                }
            );
        });

        function salvarProjeto() {
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=salvar&' + $("#formProjeto").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        $("#modal_projeto").modal("hide");
                        $("#modal_projeto").find("input").val("");
                        msgSuccess(caminho, "Projeto salvo com sucesso");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#modal_projeto").find("[name=" + err + "]").parent("div");
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
                w.document.title = 'Relatório - Descrição de Iniciativas';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });


    $("#btn-incluir").click(function () {
//         evt.preventDefault();
        atualizarForm();
        $("#modal_projeto").modal("show");
    });

    function editarProjeto(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=editarProjeto&proid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Projeto - Editar");
                $("#modal_projeto").modal("show");
            }
        });
    }

    $("#modal_projeto").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Projeto - Incluir");
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
                $("#projetoListagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar-projeto").click(function () {
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//         evt.preventDefault();
//        var caminho = '<?php //echo $urlGuia;?>//';
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#projetoListagem").html(resp);
//                return true;
//            }
//        });
    });

    function removerProjeto(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&proid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var pro = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Projeto",
                    text: "Tem certeza que deseja remover: <b>" + pro.proid + " - " + pro.prodsc + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&proid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_projeto").modal("hide");
                                $("#modal_projeto").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho, "Programa removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover Projeto.", "error");
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

    function atualizarFormFiltro() {
        $("#form-filtro").find('[name=prodsc]').val("");
        $("#form-filtro").find('[name=proid]').val("");
        $("#form-filtro").find('[name=prosigla]').val("");
        $("#form-filtro").find('#prosituacao1').removeAttr("checked");
        $("#form-filtro").find('#prosituacao2').removeAttr("checked");
        $("#form-filtro").find('#prosituacao3').prop('checked', true);
    }

    jQuery("#limpar").click(function () {
        window.location.assign(caminho);
        $('#prodsc').val('');
        $('#proid').val('');
        $('#prosigla').val('');

        $("#form-filtro").find('#prosituacao1').removeAttr("checked");
        $("#form-filtro").find('#prosituacao2').removeAttr("checked");
        $("#form-filtro").find('#prosituacao3').prop('checked', true);

    });



    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param desid
     * @description <esta função é utilizada para verificar se o projeto já está ligado ao planejamento, inviabilizando sua exclusão caso exista a ligação>
     */
    function js_verificaRemoverProjeto(proid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaRemoverProjeto&proid='+proid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if(resp != 'false'){
                    swal("Erro.", resp, "error");
                }else{
                    removerProjeto(proid);
                }
            }
        });
    }
</script>