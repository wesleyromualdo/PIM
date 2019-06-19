<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
// include_once APPRAIZ . 'par3/classes/controller/IniciativaTiposAssistenciaController.class.inc';
$controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
// $controllerIniciativaPlanejamento = new Par3_Model_IniciativaPlanejamento();

// function retornaRelatorio($arrPost)
// {
//     $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaDescricaoController();
//     $controllerIniciativaTiposAssistencia->listaIniciativaDescricao($arrPost);
// }

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaTiposAssistencia->salvar($_REQUEST);
        echo simec_json_encode($resp);
//        ver($resp,simec_json_encode($resp),d);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->listar($_REQUEST);
//         echo $controllerIniciativaTiposAssistencia->listaIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'editarIntaDesc':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->form($_REQUEST);
//         echo $controllerIniciativaTiposAssistencia->formIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'desvincularIntaDesc':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->desvincular($_REQUEST);
        die;
        break;
    case 'removerIntaDesc':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->listaIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->form($_REQUEST);
//         echo $controllerIniciativaTiposAssistencia->formIniciativaDescricao($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaTiposAssistencia->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Tipos_de_Assistência.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->listar($_REQUEST);
//         retornaRelatorio($_REQUEST);
//        $controllerIniciativaTiposAssistencia->listaIniciativaDescricao($_REQUEST);
        die();
        break;
    case 'remover':
        $controllerIniciativaTiposAssistencia->inativar($_REQUEST);
        break;
    default:
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaTiposAssistencia->recuperar($_REQUEST);
        die;
        break;
    case 'verificaExcluirTipoAssistencia':
        ob_clean();
//         echo "false"; die;
        echo "Não é possível excluir o Tipo de Assistência xxx - xxx, pois esse já foi utilizado no planejamento.";
//         if(isset($_POST['intaid'])){
//             $verificaExcluir = $controllerIniciativaTiposAtendimento->verificaLigacaoPlanejamento($_POST['intaid']);
//             if(isset($verificaExcluir[0])){
//                 $getTiposAtendimento = $controllerIniciativaTiposAtendimento->getTiposAtendimentoById($_POST['intaid']);
//                 echo "Não é possível excluir o Tipo Atendimento ".$getTiposAtendimento['intaid']." - ".$getTiposAtendimento['intadsc'].", pois o mesmo já foi utilizado no planejamento";
//             }else{
//                 echo "false";
//             }
//         }
    	die;
        break;
//     case 'getMesageErrorVerificaIniciativaPlanejamentoExclusao':
//         ob_clean();
//         if(isset($_POST['intaid'])){
//             $iniciativaPlanejamento = $controllerIniciativaPlanejamento->getIniciativaPlanejamentoByIdIniciativaDescricao($_POST['intaid']);
//             echo "Não é possível excluir a Descrição de Iniciativa {$iniciativaPlanejamento[0]['intaid']} - {$iniciativaPlanejamento[0]['intadsc']} pois a mesma já foi utilizada em iniciativa ligada ao planejamento";
//         }
//         die;
//         break;
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
                <h3 class="center">Tipos de Assistência</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-3 col-md-6 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Descrição:</label>
                            <div class="col-sm-7 col-md-8"><input type="text" name="intadsc" id="intadsc" value="<?php echo ($_GET['intadsc']);?>" maxlength="255" class="form-control"/></div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                    <div class="col-sm-7 col-md-8">
                                        <input type="number" name="intaid" id="intaid" value="<?php echo $_GET['intaid'];?>" class="form-control"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label">Situação:</label>
                                    <div class="col-lg-8 col-md-8 " >
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="intasituacao" id="intasituacao1" <?php echo ($_GET['intasituacao'] == 'A'?'checked':'');?> value="A">
                                            <label for="intasituacao1"> Ativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="intasituacao" id="intasituacao2" <?php echo ($_GET['intasituacao'] == 'I'?'checked':'');?> value="I">
                                            <label for="intasituacao2"> Inativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="intasituacao" id="intasituacao3" <?php echo ($_GET['intasituacao'] == ''?'checked':'');?> value="">
                                            <label for="intasituacao3"> Todos </label>
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
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_iniciativaAssistencia">
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
            $controllerIniciativaTiposAssistencia->listar($_REQUEST);
//             $controllerIniciativaTiposAssistencia->listaIniciativaDescricao($_REQUEST);
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
                    Relatório Tipo de Assistência<br>
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
<div class="ibox float-e-margins animated modal" id="modal_iniciativaAssistencia" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formTiposAssistencia" id="formTiposAssistencia" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Tipo de Assistência - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php echo $controllerIniciativaTiposAssistencia->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarDescAssistencia"
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

        jQuery(document).find("#formTiposAssistencia").on('click', "input[name=intasituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

        jQuery('#salvarDescAssistencia').click(function (evt) {
            evt.preventDefault();
            var inprgid = $('#conteudo-modal').find('[name=intaid]').val();
            var msg = 'Incluir';
            if (inprgid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> este Tipo de Assistência?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        salvarTiposAssistencia();
                    }
                    return false;
                }
            );
        });

        function salvarTiposAssistencia() {
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=salvar&' + $("#formTiposAssistencia").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        $("#modal_iniciativaAssistencia").modal("hide");
                        $("#modal_iniciativaAssistencia").find("input").val("");
                        msgSuccess(caminho,"Assistência salva com sucesso");
                        return false;
                    }
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#modal_iniciativaAssistencia").find("input[name=" + err + "]").parent("div");
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
                w.document.title = 'Relatório - Tipo de Assistência';
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
        $("#modal_iniciativaAssistencia").modal("show");
    });

    function editar(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=editarIntaDesc&intaid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Descrição de Iniciativa - Editar");
                $("#modal_iniciativaAssistencia").modal("show");
            }
        });
    }
    $("#modal_iniciativaAssistencia").on("hidden.bs.modal", function () {
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

    function remover(id) {
            var caminho = '<?php echo $urlGuia;?>';
            var action = '&req=recuperar&intaid=' + id;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: true,
                success: function (resp) {
                    var inta = $.parseJSON(resp);
                    var confirmado = false;
                    swal({
                        title: "Remover Tipo de Assistência",
                        text: "Tem certeza que deseja remover: <b>" + inta.intaid + " - " + inta.intadsc + "</b> ?",
                        html: true,
                        type: "warning",
                        showCancelButton: true,
                        cancelButtonText: "Não",
                        confirmButtonText: "Sim",
                        closeOnConfirm: false
                    }, function (isConfirm) {
                        if (isConfirm) {
                            var caminho = window.location.href;
                            var action = '&req=removerIntaDesc&intaid=' + id;
                            $.ajax({
                                type: "POST",
                                url: caminho,
                                data: action,
                                async: false,
                                success: function (resp) {
                                    $("#modal_iniciativaAssistencia").modal("hide");
                                    $("#modal_iniciativaAssistencia").find("input").val("");
                                    if (resp != 'erro') {
                                        msgSuccess(caminho,"Tipo de Assistência removida com sucesso");
                                        return false;
                                    }
                                    swal("Erro.", "Ocorreu um erro ao remover o Tipo de Assistência.", "error");
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
        $("#form-filtro").find('[name=intadsc]').val("");
        $("#form-filtro").find('[name=intaid]').val("");
        $("#form-filtro").find('#intasituacao1').removeAttr("checked");
        $("#form-filtro").find('#intasituacao2').removeAttr("checked");
        $("#form-filtro").find('#intasituacao3').prop('checked',true);
    }

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#intadsc').val('');
        $('#intaid').val('');

        $("#form-filtro").find('#intasituacao1').removeAttr("checked");
        $("#form-filtro").find('#intasituacao2').removeAttr("checked");
        $("#form-filtro").find('#intasituacao3').prop('checked',true);
    });

    /**
     * Verifica se iniciativa não está ligada ao planejamento
     * @param iniid
     */
    function js_verificaRemoverTipoAssitencia(intaid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaExcluirTipoAssistencia&intaid='+intaid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
               if(resp != "false"){
                   swal("Erro.", resp, "error");
               }else{
                   remover(intaid);
               }
            }
        });
    }
    /**
     * Monta a messagem de error quando não a descrição de iniciativa estiver ligada a alguma iniciativa que está no planejamento.
     * @param iniid
     */
//     function js_getMesageErrorVerificaIniciativaPlanejamentoExclusao(intaid){
//        var caminho = '<?php echo $urlGuia;?>';
//         var action = '&req=getMesageErrorVerificaIniciativaPlanejamentoExclusao&intaid='+intaid;
//         $.ajax({
//             type: "POST",
//             url: caminho,
//             data: action,
//             async: false,
//             success: function (resp) {
//                 alert(resp);
//                 //swal("Erro.", resp, "error");
//             },
//             error: function(){
//                 alert("ERROR");
//                 return "ERROR";
//             }
//         });
//     }
</script>