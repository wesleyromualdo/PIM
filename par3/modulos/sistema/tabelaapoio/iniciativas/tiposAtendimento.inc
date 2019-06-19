<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/IniciativaTiposAtendimentoController.class.inc';

$controllerIniciativaTiposAtendimento = new Par3_Controller_IniciativaTiposAtendimentoController();


function retornaRelatorio($arrPost)
{
    $controllerIniciativaTiposAtendimento = new Par3_Controller_IniciativaTiposAtendimentoController();
    $controllerIniciativaTiposAtendimento->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaTiposAtendimento->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerIniciativaTiposAtendimento->listar($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerIniciativaTiposAtendimento->form($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerIniciativaTiposAtendimento->remover($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaTiposAtendimento->listar($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerIniciativaTiposAtendimento->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaTiposAtendimento->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Tipo_Atendimento.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaTiposAtendimento->recuperar();
        die;
        break;
    case 'verificaExcluirTipoAtendimento':
        ob_clean();
        if(isset($_POST['intaid'])){
            $verificaExcluir = $controllerIniciativaTiposAtendimento->verificaLigacaoPlanejamento($_POST['intaid']);
            if(isset($verificaExcluir[0])){
                $getTiposAtendimento = $controllerIniciativaTiposAtendimento->getTiposAtendimentoById($_POST['intaid']);
                echo "Não é possível excluir o Tipo Atendimento ".$getTiposAtendimento['intaid']." - ".$getTiposAtendimento['intadsc'].", pois o mesmo já foi utilizado no planejamento";
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
?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Tipos de Atendimento</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group"><label class="col-md-3 col-lg-3 control-label">Tipo de Atendimento: </label>
                            <div class="col-lg-7 col-md-7">
                                <input type="text" name="intadsc" id="intadsc" maxlength="255" value="<?php echo ($_GET['intadsc']);?>" class="form-control"/>
                            </div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 col-md-2 control-label">Código:</label>
                                    <div class="col-lg-7 col-md-7">
                                        <input type="number" name="intaid" id="intaid" class="form-control" value="<?php echo $_GET['intaid'];?>"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label">Situação:</label>
                                    <div class="col-lg-8">
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="intasituacao" id="intasituacao1" <?php echo ($_GET['intasituacao'] == 'A'?'checked':'');?> value="A" value="A">
                                            <label for="intasituacao1"> Ativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="intasituacao" id="intasituacao2" <?php echo ($_GET['intasituacao'] == 'I'?'checked':'');?> value="I">
                                            <label for="intasituacao2"> Inativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline pull-left">
                                            <input type="radio" name="intasituacao" id="intasituacao3" <?php echo ($_GET['intasituacao'] == ''?'checked':'');?> value="" >
                                            <label for="intasituacao3"> Todos </label>
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
                <button type="button" class="btn btn-primary" id="pesquisar-iniciativa"><i class="fa fa-search"></i> Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_iniciativaDesc">
                    <i class="fa fa-plus"></i> Incluir
                </button> |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="iniciativaDescListagem">
            <?php
            $controllerIniciativaTiposAtendimento->listar($_REQUEST);
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
                    Relatório Tipo de Atendimento<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?=$_SESSION['usunome'];?></b><br/>
                    Hora da Impressão: <?=date( 'd/m/Y - H:i:s' );?><br />
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
        <form method="post" name="form" id="form" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </button>
                    <h3 id="modal_titulo">Tipo de Atendimento - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                        <?php echo $controllerIniciativaTiposAtendimento->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarTipoAtendimento" class="btn btn-success" <?php echo $disabled; ?>>
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

        jQuery('#form').on('click',"input[name=intasituacao ][value=I]",function(){
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

        jQuery('#salvarTipoAtendimento').click(function (evt) {
            evt.preventDefault();
            var inprgid = $('#conteudo-modal').find('[name=intaid]').val();
            var msg = 'Incluir';
            if(inprgid){
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>"+msg+"</b> este Tipo de Atendimento?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if(isConfirm){
                        salvarTipoAtendimento();
                    }
                    return false;
                }
            );
        });

        function salvarTipoAtendimento()
        {
            var caminho = '<?php echo $urlGuia;?>';
                    var action = '&req=salvar&' + $("#form").serialize();
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
                                atualizarFormFiltro();
                                atualizarListagem();
                                msgSuccess(caminho,"Tipo de Atendimento Salvo com sucesso");
                                return false;
                            }
                            swal.close();
                            $.each(res,function(index,erro){//retorna mensagens de erro em json
                                $.each(erro,function(err,errMsg){
                                    if(errMsg < err.length){
                                        return;//se a mensagem for vazia não retorna nada
                                    }
                                    var divFormInput =  $("#form").find("input[name="+err+"]").parent("div");
                                    divFormInput.closest('.form-group').addClass('has-error');
                                    divFormInput.append("<div id='div-"+err+"'></div>");
                                    $("#div-"+err).html("<span class='danger'>"+errMsg+"</span>");
                                });
                            });
                            return false;
                        }
                    });

                return false;
        }
    });

    $(".xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize()+'&'+$('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var action = '&req=imprimir&' + $("#form-filtro").serialize()+'&'+$('.form-listagem').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#impressao-content").html(resp);
                var container =  $("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                table.css('width','100%')
                table.css('border-collapse','collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border','1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height','50px');
                td.css('text-align','center');
                    var w = window.open();
                    w.document.write($('#div-impressao').html());
                    w.document.title = 'Relatório - Tipo de Atendimento';
                    w.print();
                    w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize()+'&'+$('.form-listagem').serialize());
                return true;
            }
        });
    });

    $("#btn-incluir").click(function (evt) {
        evt.preventDefault();
        atualizarForm();
        $("#modal_iniciativaDesc").modal("show");
    });

    function editar(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=editar&intaid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").find("input").val("");
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Tipo de Atendimento - Editar");
                $("#modal_iniciativaDesc").modal("show");
            }
        });
    }

    $("#modal_iniciativaDesc").on("hidden.bs.modal",function(){
        $("#modal_titulo").html("Descrição de Iniciativa - Incluir");
    });

    function atualizarListagem(params ="") {
        if(params){
            params = '&'+params
        }
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagem'+params;
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

    $("#pesquisar-iniciativa").click(function (evt) {
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        evt.preventDefault();
//        var caminho = window.location.href;
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
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
                    html: true,
                    title: "Remover Tipo de Atendimento!",
                    text: "Tem certeza que deseja remover: <strong>" + inta.intaid + " - " + inta.intadsc + "?</strong>",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&intaid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_iniciativaDesc").modal("hide");
                                $("#modal_iniciativaDesc").find("input").val("");
                                if (resp != 'erro') {
                                    atualizarFormFiltro();
                                    atualizarListagem();
                                    msgSuccess(caminho,"Tipo de Atendimento removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover Tipo de Atendimento.", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarForm(params = "") {
        if(params){
            params = '&'+params
        }

        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarForm'+params;
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

    function js_verificaRemoverTipoAtendimento(intaid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaExcluirTipoAtendimento&intaid='+intaid;
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
</script>