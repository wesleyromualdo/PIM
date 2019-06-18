<?php

$controllerIniciativaArea       = new Par3_Controller_IniciativaAreas();
$controllerUnidadeOrcamentaria  = new Par3_Controller_IniciativaUnidadeOrcamentaria();


function retornaRelatorio($arrPost)
{
    $controllerIniciativaArea   = new Par3_Controller_IniciativaAreas();
    $controllerIniciativaArea->listaIniciativaAreas($arrPost);
}
switch ($_REQUEST['req']) {
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaArea->listaIniciativaAreas($_REQUEST);
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'atualizarListagemArea':
        ob_clean();
        echo $controllerIniciativaArea->listaIniciativaAreas($_REQUEST);
        die;
        break;
    case 'form':
        ob_clean();
        $model = $controllerIniciativaArea->recuperar($_REQUEST['iarid']);
        require_once 'areas/formArea.inc';
        die;
        break;
    case 'salvarArea':
        ob_clean();
        $resp = $controllerIniciativaArea->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaArea->listaIniciativaAreas($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Areas.xls");
        die;
        break;
    case 'getarea':
        ob_clean();
        if ($_REQUEST['iuoid']) {
            $mIar = new Par3_Model_IniciativaAreas();
            echo simec_json_encode($mIar->listaAreas($_REQUEST));die;
        }
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaArea->recuperar($_REQUEST);
        die;
        break;
    case 'removerArea':
        ob_clean();
        echo $controllerIniciativaArea->remover($_REQUEST);
        die;
        break;
    case 'verificaRemoverArea':
        ob_clean();
        if(isset($_POST['iarid'])){
            $verificaExclusao =  $controllerIniciativaArea->verificaLigacaoPlanejamentoById($_POST['iarid']);
           if(isset($verificaExclusao[0])){
                $getIniciativaArea = $controllerIniciativaArea->getIniciativaAreaById($_POST['iarid']);
                echo "Não é possível excluir o Desdobramento ".$getIniciativaArea['iarid']." - ".$getIniciativaArea['iardsc'].", pois o mesmo já foi utilizado no planejamento";
            }else{
                echo "false";
            }
        }
        die;
        break;
    default:
        break;
}
global $simec;
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>
<script type="text/javascript" src="js/sistema/tabelaapoio/iniciativas/areas.js"></script>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Áreas</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group"><label class="col-lg-3 col-md-3 control-label">Área:</label>
                            <div class="col-sm-7 col-md-8"><input type="text" name="iardsc" id="iardsc" maxlength="255"  value="<?php echo ($_GET['iardsc']); ?>" class="form-control"/></div>
                        </div>
                        <!--filtros-->
                        <div id="avancado" style="display: none;">
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-md-2 control-label">Código:</label>
                                <div class="col-md-5 col-lg-5">
                                    <input type="number" name="iarid" id="iarid" class="form-control"/>
                                </div>
                            </div>
                            <?php echo $simec->select('iuoid', 'Unidade Orçamentária', '', $controllerUnidadeOrcamentaria->pegarSelectCombo(), array('maxlength' => '255'), array('input-size' => '8')); ?>
                            <?php echo $simec->select('sigla', 'Sigla', '', $controllerIniciativaArea->pegarSQLSelectComboSigla(), array('maxlength' => '255'), array('input-size' => '8')); ?>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label"> Situação:</label>
                                <div class="col-lg-8" >
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="iarsituacao" id="iarsituacao1" value="A">
                                        <label for="iarsituacao1"> Ativo </label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="iarsituacao" id="iarsituacao2" value="I">
                                        <label for="iarsituacao2"> Inativo </label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="iarsituacao" id="iarsituacao3" value=""
                                               checked="checked">
                                        <label for="iarsituacao3"> Todos </label>
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
                <button type="button" class="btn btn-primary" id="pesquisar-area"><i class="fa fa-search"></i>Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal"
                        data-target="#modal_area"><i class="fa fa-plus"></i> Incluir
                </button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                            class="fa fa-file-excel-o"></i> Gerar Excel
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i
                            class="glyphicon glyphicon-print"></i> Imprimir
                </button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>

            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="areaListagem">
            <?php $controllerIniciativaArea->listaIniciativaAreas($_REQUEST); ?>
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
                    MEC/FNDE <br>Relatório Tabela de Apoio: Áreas<br>
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
<div class="ibox float-e-margins animated modal" id="modal_area" tabdesex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formArea" id="formArea" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Área - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal"></div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="button" id="cancelarArea" data-dismiss="modal" class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="salvarArea" class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script>
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
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

        $('#btn-incluir').click(function () {
            form();
        });
    });


    $(".xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    jQuery(document).find("#formArea").on('click', "input[name=iarsituacao ][value=I]", function () {
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
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
                w.document.title = 'Relatório - Áreas';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagemArea($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    jQuery('#salvarArea').click(function () {

        var iarid = $('#conteudo-modal').find('[name=iarid]').val();
        var msg = 'Incluir';
        if (iarid) {
            msg = 'Alterar';
        }
        $('#formArea').isValid(function(isTrue){
            if(isTrue){
                swal({
                        title: "Confirmar",
                        text: "Deseja <b>" + msg + "</b> esta Área?",
                        type: "success",
                        html: true,
                        showCancelButton: true,
                        confirmButtonColor: "#DD6B55",
                        cancelButtonText: "Não",
                        confirmButtonText: "Sim",
                        closeOnConfirm: false
                    }, function (isConfirm) {
                        if (isConfirm) {
                            salvarArea();
                        }
                        return false;
                    }
                );
            }
        });
    });

    $("#pesquisar-area").click(function () {
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#areaListagem").html(resp);
//                return true;
//            }
//        });
    });

    $("#conteudo-modal").on('change', "#iuoid", function () {
        var iuoid = $("#conteudo-modal").find("#iuoid").val();
        var options = $("#conteudo-modal").find("#iarid_pai");
        if(iuoid == ''){
            $("#iarid_pai").empty().trigger('chosen:updated');
            return false;
        }
        var action = '&req=getarea&iuoid=' + iuoid;
        $.getJSON(caminho + action, function (dados) {
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                var areas = $.parseJSON(dados);
                options.append(new Option("Selecione", "",'disabled'));
                $.each(areas, function (index, iar) {
                    options.append(new Option(iar.descricao, iar.codigo));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();

    $("#modal_area").on("hidden.bs.modal", function () {
        $("#modal_titulo").html("Area - Incluir");
    });

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#iardsc').val('');
        $('#iarid').val('');
        $("#form-filtro").find('#iarsituacao1').removeAttr("checked");
        $("#form-filtro").find('#iarsituacao2').removeAttr("checked");
        $("#form-filtro").find('#iarsituacao3').prop('checked',true);
        $("#sigla").val('').trigger('chosen:updated');
        $("#iuoid").val('').trigger('chosen:updated');
    });

    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param desid
     * @description <esta função é utilizada para verificar se o desdobramento já está ligado ao planejamento, inviabilizando sua exclusão caso exista a ligação>
     */
    function js_verificaRemoverArea(iarid){
        var caminho = window.location.href;
        var action = '&req=verificaRemoverArea&iarid='+iarid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if(resp != 'false'){
                    swal("Erro.", resp, "error");
                }else{
                    removerArea(iarid);
                }
            }
        });
    }
</script>