<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
global $simec;
$disabled = 'disabled';
$mPrograma = new Par3_Model_IniciativaProgramas();

$controllerUnidadeOrcamentaria = new Par3_Controller_IniciativaUnidadeOrcamentaria();
$iar                           = new Par3_Model_IniciativaAreas($_REQUEST['iarid']);
$mPrograma                     = new Par3_Model_IniciativaProgramas();
$rsPrograma                    = $mPrograma->recuperarTodos('prgid as codigo, prgdsc as descricao',array("prgstatus = 'A'"),'2 ASC');
$controllerIniciativaProgramas = new Par3_Controller_IniciativaAreasRelacionadasProgramas();
$mIar                          = new Par3_Model_IniciativaAreas();
$rsIar = array();

if($iar->iarid){
    $rsIar = $mIar->pegarSQLSelectComboSigla(array('iuoid'=> $iap->iuoid));
}

function retornaRelatorio($arrPost)
{
    $controllerIniciativaProgramas = new Par3_Controller_IniciativaAreasRelacionadasProgramas();
    $controllerIniciativaProgramas->listarProgramas($arrPost);
}

$rsPrograma = $mPrograma->pegarSQLSelectComboSigla();
$controllerUnidadeOrcamentaria  = new Par3_Controller_IniciativaUnidadeOrcamentaria();
$mIar  = new Par3_Model_IniciativaAreas();
$rsIar = $mIar->pegarSQLSelectComboSigla();

switch ($_REQUEST['req']) {
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaProgramas->listarProgramas($_REQUEST);
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'xls':
        ob_clean();
        header("Content-Disposition: attachment; filename=Areas_Relacionadas.xls");
        echo $controllerIniciativaProgramas->listarProgramas($_REQUEST);
        die;
        break;
    case 'atualizarListagemArea':
        ob_clean();
        die;
        break;
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaProgramas->salvar($_REQUEST);
        echo simec_json_encode($resp);die;
        break;
    case 'getarea':
        ob_clean();
        if ($_REQUEST['iuoid']) {
            echo simec_json_encode($mIar->listaAreas($_REQUEST));die;
        }
        break;
    case 'recuperarprograma':
        ob_clean();
        $resp = $controllerIniciativaProgramas->recuperarPrograma($_REQUEST);
        echo simec_json_encode($resp);die;
        break;
    case 'desvincular':
        ob_clean();
        $resp = $controllerIniciativaProgramas->desvincular($_REQUEST['iapid']);
        echo simec_json_encode($resp);die;
        break;
    case 'marcaropcao':
        ob_clean();
        $resp = $controllerIniciativaProgramas->marcarOpcao($_REQUEST['esdid'],$_REQUEST['iapid']);
        echo simec_json_encode($resp);die;
        die;
        break;
    default:
        break;
}

?>
<script type="text/javascript" src="js/sistema/tabelaapoio/iniciativas/areasrelacionadas.js"></script>
<div class="ibox" id="painelItens">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Áreas Relacionadas</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <form name="form-item" id="form-arearelacionada" class="form-horizontal">
            <div class="row">
                <?php if($iar->iarid):?>
                    <input type="hidden" name="iarid" value="<?php echo $iar->iarid; ?>"/>
                    <input type="hidden" name="iuoid" value="<?php echo $iar->iuoid; ?>"/>
                <? endif; ?>
                <?php
                echo $simec->select('iuoid', 'Unidade Orçamentária', $iar->iuoid, $controllerUnidadeOrcamentaria->pegarSelectCombo(), array('maxlength' => '255', 'required','title' => 'Selecione uma Unidade Gestora'), array('input-size' => '8'));//,array('class' => 'col-md-3')
                echo $simec->select('iarid', 'Área', $iar->iarid,$rsIar, array('maxlength' => '255', 'required','title' => 'Selecione uma Área'), array('input-size' => '8'));//,array('class' => 'col-md-3')
                echo $simec->select('prgid[]', 'Programa', '', $rsPrograma, array('maxlength' => '255','required','title' => 'Selecione um programa','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '8'));
                ?>
            </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" class="btn btn-primary" id="pesquisar-arearelacionada"><i class="fa fa-search"></i> Pesquisar</button>
                    <button type="button" class="btn btn-success" id="btn-vincularprogramas"><i class="fa fa-link"></i> Vincular</button>
                    |

                    <button type="button" class="btn btn-primary" id="btn-xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                     <button type="button" class="btn btn-warning" id="imprimir"><i class="fa fa-print"></i> Imprimir</button>
                    |
                    <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
        </form>
    </div>

    <div class="ibox-content" id="listaprogramasvinculados" style="overflow: auto;">
        <div class="row" >
            <?php echo $controllerIniciativaProgramas->listarProgramas($_REQUEST) ;?>
        </div>
    </div>

<script type="text/javascript" src="js/sistema/tabelaapoio/iniciativas/areasrelacionadas.js"></script>
<script>
    var caminho = '<?php echo $urlGuia;?>';
    $(document).on('click', '#cancelar-arearelacionada', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas');
    });
    jQuery('#salvar-arearelacionada').click(function (evt) {
        evt.preventDefault();
        var iapid = $('#form-arearelacionada').find('[name=iapid]').val();
        var msg = 'Incluir';
        if (iapid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> esta Área Relacionada?",
                type: "success",
                html: true,
                showCancelButton:  true,
                cancelButtonText:  "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarAreaRelacionada();
                }
                return false;
            }
        );
    });

    jQuery('#btn-vincularprogramas').click(function (evt) {
        evt.preventDefault();
        swal({
                title: "Confirmar",
                text: "Confirma a <b>Vinculação</b> de Programas a Área Informada?",
                type: "success",
                html: true,
                showCancelButton:  true,
                cancelButtonText:  "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    vicularProgramas();
                }
                return false;
            }
        );
    });

    $('#btn-vincularprogramas').click(function(evt){
        evt.preventDefault();

    });

    $("#form-arearelacionada").on('change', "#iuoid", function () {
        var iuoid = $('#form-arearelacionada').find("#iuoid").val();
        var options = $('#form-arearelacionada').find("#iarid");
        var action = '&req=getarea&iuoid=' + iuoid;
        $.getJSON(caminho + action, function (dados) {
            options.empty();
            if (!dados) {
                options.append(new Option("", ""));
            } else {
                var areas = $.parseJSON(dados);
                console.log(areas);
                $.each(areas, function (index, iar) {
                    options.append(new Option(iar.descricao, iar.codigo));
                });
            }
            options.focus();
            options.trigger('chosen:updated');
        });
    }).change();
</script>
<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório Tabela de Apoio: Áreas<br>
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
<script>
    var caminho = '<?php echo $urlGuia;?>';
    $("#btn-xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-arearelacionada').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=imprimir&' + $("#form-arearelacionada").serialize() + '&' + $('.form-listagem').serialize();
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
                w.document.title = 'Relatório - Áreas Relacionadas';
                w.print();
                w.close();
                $("#impressao-content").html("");
                return true;
            }
        });
    });

    $("#pesquisar-arearelacionada").click(function (evt) {
        evt.preventDefault();
        pesquisarAreaRelacionada();
    });

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

    $("#btn-incluir").click(function () {
        window.location.assign('<?php echo $urlGuia;?>');
    });

    function editarAreaRelacionada(id)
    {
            window.location.assign('<?php echo $urlGuia;?>' + "&iarid="+id);
    }

    jQuery("#limpar").click(function(){
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas');
//        $("#iarid").val('').trigger('chosen:updated');
//        $("#prgid").val('').trigger('chosen:updated');
//        $("#iuoid").val('').trigger('chosen:updated');
//        $('.search-choice').remove();
    });
</script>