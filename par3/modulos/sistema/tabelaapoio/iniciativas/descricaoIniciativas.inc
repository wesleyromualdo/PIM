<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/IniciativaDescricaoController.class.inc';
$controllerIniciativaDescricao = new Par3_Controller_IniciativaDescricaoController();
$controllerIniciativaPlanejamento = new Par3_Model_IniciativaPlanejamento();

function retornaRelatorio($arrPost)
{
    $controllerIniciativaDescricao = new Par3_Controller_IniciativaDescricaoController();
    $controllerIniciativaDescricao->listaIniciativaDescricao($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaDescricao->salvar($_REQUEST);
        echo simec_json_encode($resp);
//        ver($resp,simec_json_encode($resp),d);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerIniciativaDescricao->listaIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'editarIndDesc':
        ob_clean();
        echo $controllerIniciativaDescricao->formIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'desvincularIndDesc':
        ob_clean();
        echo $controllerIniciativaDescricao->desvincular($_REQUEST);
        die;
        break;
    case 'removerIndDesc':
        ob_clean();
        echo $controllerIniciativaDescricao->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaDescricao->listaIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerIniciativaDescricao->formIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaDescricao->listaIniciativaDescricao($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Iniciativa_Descricao.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
//        $controllerIniciativaDescricao->listaIniciativaDescricao($_REQUEST);
        die();
        break;
    case 'remover':
        $controllerIniciativaDescricao->inativar($_REQUEST);
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaDescricao->recuperar($_REQUEST);
        die;
        break;
    case 'verificaIniciativaPlanejamentoExclusao':
        ob_clean();
        if(isset($_POST['indid'])){
           //$iniciativaPlanejamento = $controllerIniciativaDescricao->getIniciativaDescricaoById($_POST['indid']);
           $getIniciativaPlanejamentoIniciativaDescricao = $controllerIniciativaPlanejamento->getIniciativaPlanejamentoByIdIniciativaDescricao($_POST['indid']);
          if(isset($getIniciativaPlanejamentoIniciativaDescricao[0])){
               echo "false"; //existe ligação, não pode ser exluída
           }else{
               echo "true"; // não existe ligação, pode ser exluída
           }
        }
        die;
        break;
    case 'getMesageErrorVerificaIniciativaPlanejamentoExclusao':
        ob_clean();
        if(isset($_POST['indid'])){
            $iniciativaPlanejamento = $controllerIniciativaPlanejamento->getIniciativaPlanejamentoByIdIniciativaDescricao($_POST['indid']);
            echo "Não é possível excluir a Descrição de Iniciativa {$iniciativaPlanejamento[0]['indid']} - {$iniciativaPlanejamento[0]['inddsc']} pois a mesma já foi utilizada em iniciativa ligada ao planejamento";
        }
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>
<?php //echo $simec->title('Tabela Apoio - Descrição de Iniciativas', 'Sistema / Tabela de Apoio / Iniciativas do PAR / Descrição de Iniciativas'); ?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Descrição de Iniciativas</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-3 col-md-6 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Descrição:</label>
                            <div class="col-sm-7 col-md-8"><input type="text" name="inddsc" id="inddsc" value="<?php echo ($_GET['inddsc']);?>" maxlength="255" class="form-control"/></div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                    <div class="col-sm-7 col-md-8">
                                        <input type="number" name="indid" id="indid" value="<?php echo $_GET['indid'];?>" class="form-control"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label">Situação:</label>
                                    <div class="col-lg-8 col-md-8 " >
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="indsituacao" id="indsituacao1" <?php echo ($_GET['indsituacao'] == 'A'?'checked':'');?> value="A">
                                            <label for="indsituacao1"> Ativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="indsituacao" id="indsituacao2" <?php echo ($_GET['indsituacao'] == 'I'?'checked':'');?> value="I">
                                            <label for="indsituacao2"> Inativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="indsituacao" id="indsituacao3" <?php echo ($_GET['indsituacao'] == ''?'checked':'');?> value="">
                                            <label for="indsituacao3"> Todos </label>
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
                <button type="button" class="btn btn-primary" id="pesquisar-iniciativa"><i class="fa fa-search"></i>
                    Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_iniciativaDesc">
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
        <div id="iniciativaDescListagem">
            <?php
            $controllerIniciativaDescricao->listaIniciativaDescricao($_REQUEST);
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
<div class="ibox float-e-margins animated modal" id="modal_iniciativaDesc" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formIniciativaDescricao" id="formIniciativaDescricao" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Descrição de Iniciativa - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php echo $controllerIniciativaDescricao->formIniciativaDescricao($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarDescIniciativa"
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

        jQuery(document).find("#formIniciativaDescricao").on('click', "input[name=indsituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

        jQuery('#salvarDescIniciativa').click(function (evt) {
            evt.preventDefault();
            var inprgid = $('#conteudo-modal').find('[name=indid]').val();
            var msg = 'Incluir';
            if (inprgid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> esta Descrição?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarIniciativaDescricao();
                    }
                    return false;
                }
            );
        });

        function salvarIniciativaDescricao() {
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=salvar&' + $("#formIniciativaDescricao").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        $("#modal_iniciativaDesc").modal("hide");
                        $("#modal_iniciativaDesc").find("input").val("");
                        msgSuccess(caminho,"Iniciativa salva com sucesso");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#modal_iniciativaDesc").find("input[name=" + err + "]").parent("div");
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
        $("#modal_iniciativaDesc").modal("show");
    });

    function editarIniciativaDescricao(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=editarIndDesc&indid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Descrição de Iniciativa - Editar");
                $("#modal_iniciativaDesc").modal("show");
            }
        });
    }
    $("#modal_iniciativaDesc").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Descrição de Iniciativa - Incluir");
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
                $("#iniciativaDescListagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar-iniciativa").click(function () {
        //evt.preventDefault();
        //var action = '&req=pesquisar&' + $("#form-filtro").serialize();
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#iniciativaDescListagem").html(resp);
//                return true;
//            }
//        });
    });

    function removerIniciativaDescricao(id) {
            var caminho = '<?php echo $urlGuia;?>';
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
                        title: "Remover Descrição de Iniciativa",
                        text: "Tem certeza que deseja remover: <b>" + ind.indid + " - " + ind.inddsc + "</b> ?",
                        html: true,
                        type: "warning",
                        showCancelButton: true,
                        cancelButtonText: "Não",
                        confirmButtonText: "Sim",
                        closeOnConfirm: false
                    }, function (isConfirm) {
                        if (isConfirm) {
                            var caminho = window.location.href;
                            var action = '&req=removerIndDesc&indid=' + id;
                            $.ajax({
                                type: "POST",
                                url: caminho,
                                data: action,
                                async: false,
                                success: function (resp) {
                                    $("#modal_iniciativaDesc").modal("hide");
                                    $("#modal_iniciativaDesc").find("input").val("");
                                    if (resp != 'erro') {
                                        msgSuccess(caminho,"Iniciativa removida com sucesso");
                                        return false;
                                    }
                                    swal("Erro.", "Ocorreu um erro ao remover a Descrição da Iniciativa.", "error");
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
        $("#form-filtro").find('[name=inddsc]').val("");
        $("#form-filtro").find('[name=indid]').val("");
        $("#form-filtro").find('#indsituacao1').removeAttr("checked");
        $("#form-filtro").find('#indsituacao2').removeAttr("checked");
        $("#form-filtro").find('#indsituacao3').prop('checked',true);
    }

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#inddsc').val('');
        $('#indid').val('');

        $("#form-filtro").find('#indsituacao1').removeAttr("checked");
        $("#form-filtro").find('#indsituacao2').removeAttr("checked");
        $("#form-filtro").find('#indsituacao3').prop('checked',true);
    });

    /**
     * Verifica se iniciativa não está ligada ao planejamento
     * @param iniid
     */
    function js_verificaIniciativaPlanejamentoExclusao(indid){
       // alert(indid);
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaIniciativaPlanejamentoExclusao&indid='+indid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //alert(resp);
                if(resp == "true"){
                    //pode ser excluída, pois ainda não existe nenhum vínculo com o planejamento.
                    //alert("PERMISSÃO PARA EXCLUIR: "+indid);
                    removerIniciativaDescricao(indid);
                    return true;
                }
                else if(resp == "false"){
                    //não pode ser exluída pois já existe um vínculo com o planejamento
                    var action = '&req=getMesageErrorVerificaIniciativaPlanejamentoExclusao&indid='+indid;
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            swal("Erro.", resp, "error");

                            },
                    });
                }
               // return result;
            }
        });
    }
    /**
     * Monta a messagem de error quando não a descrição de iniciativa estiver ligada a alguma iniciativa que está no planejamento.
     * @param iniid
     */
    function js_getMesageErrorVerificaIniciativaPlanejamentoExclusao(indid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=getMesageErrorVerificaIniciativaPlanejamentoExclusao&indid='+indid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                alert(resp);
                //swal("Erro.", resp, "error");
            },
            error: function(){
                alert("ERROR");
                return "ERROR";
            }
        });
    }
</script>