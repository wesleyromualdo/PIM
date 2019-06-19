<?php
$controllerProcesso = new Par3_Controller_Processo();


switch ($_GET['requisicao']) {
    case 'listarProcessos':
        ob_clean();
        $resp = $controllerProcesso->listarProcessosJSON($_GET);
        echo simec_json_encode($resp);
        die;
        break;
    case 'montarListaInterna':
        ob_clean();
        $resp = $controllerProcesso->montarListaInterna($_GET);
        echo $resp;
        die;
    break;
    case 'listarComposicao':
        ob_clean();
        $resp = $controllerProcesso->listarComposicao($_GET);
        echo json_encode($resp);
        die;
        break;
    case 'listarAnalise':
        ob_clean();
        $resp = $controllerProcesso->listarAnalise($_GET);
         $toUtf8 = utf8_encode_recursive($resp);
         echo simec_json_encode($toUtf8);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerProcesso->editarProcesso($_POST);
        die;
        break;
    case 'listarDocumento':
        ob_clean();
        $resp = $controllerProcesso->listarDocumento($_GET);
        $toUtf8 = utf8_encode_recursive($resp);
        echo simec_json_encode($toUtf8);
        die;
        break;
    case 'listarEmpenho':
        ob_clean();
        $resp = $controllerProcesso->listarEmpenho($_GET);
        $toUtf8 = utf8_encode_recursive($resp);
        echo simec_json_encode($toUtf8);
        die;
        break;
    case 'listarEmpenhoComposicao':
        ob_clean();
        $resp = $controllerProcesso->listarEmpenhoComposicao($_GET);
        $toUtf8 = utf8_encode_recursive($resp);
        echo simec_json_encode($toUtf8);
        die;
        break;
        case 'listarPagamento':
        ob_clean();
        $resp = $controllerProcesso->listarPagamento($_GET);
        $toUtf8 = utf8_encode_recursive($resp);
        echo simec_json_encode($toUtf8);
        die;
        break;
    case 'listarPagamentoComposicao':
        ob_clean();
        $resp = $controllerProcesso->listarPagamentoComposicao($_GET);
        $toUtf8 = utf8_encode_recursive($resp);
        echo simec_json_encode($toUtf8);
        die;
        break;
    case 'editarAnalisePar':
        ob_clean();
        echo $controllerProcesso->editarAnalisePar($_POST);
        die;
        break;
    case 'editarAnaliseObra':
        ob_clean();
        echo $controllerProcesso->editarAnaliseObra($_POST);
        die;
        break;

    case 'editarDocumento':
        ob_clean();
        echo $controllerProcesso->editarDocumento($_POST);
        die;
        break;
    case 'editarEmpenho':
        ob_clean();
        echo $controllerProcesso->editarEmpenho($_POST);
        die;
        break;
    case 'editarEmpenhoComposicaoPar':
        ob_clean();
        echo $controllerProcesso->editarEmpenhoPar($_POST);
        die;
        break;
    case 'editarEmpenhoComposicaoObra':
        ob_clean();
        echo $controllerProcesso->editarEmpenhoObra($_POST);
        die;
        break;
    case 'editarPagamento':
        ob_clean();
        echo $controllerProcesso->editarPagamento($_POST);
        die;
        break;
    case 'editarPagamentoComposicaoPar':
        ob_clean();
        echo $controllerProcesso->editarPagamentoComposicaoPar($_POST);
        die;
        break;
    case 'editarPagamentoComposicaoObra':
        ob_clean();
        echo $controllerProcesso->editarPagamentoComposicaoObra($_POST);
        die;
        break;
    case 'editarComposicaoPar':
        ob_clean();
        echo $controllerProcesso->editarComposicaoPar($_POST);
        die;
        break;
    case 'editarComposicaoObra':
        ob_clean();
        echo $controllerProcesso->editarComposicaoObra($_POST);
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>

<link rel="stylesheet" type="text/css" media="screen" href="../includes/jqGrid/css/ui.jqgrid-bootstrap.css"/>
<style>
    /** CSS para texto quebrar linha no header e nas linhas da grid **/
    th.ui-th-column div {
        white-space: normal !important;
        height: auto !important;
    }

    .ui-jqgrid tr.jqgrow td {
        white-space: normal !important;
    }
</style>
<script type="text/ecmascript" src="../includes/jqGrid/js/i18n/grid.locale-pt-br.js"></script>
<script type="text/ecmascript" src="../includes/jqGrid/js/jquery.jqGrid.min.js"></script><div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Processos</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="formulario" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-md-3 col-lg-3 control-label">Processo:</label>
                            <div class="col-sm-8 col-md-8">
                                <input type="text" name="pronumeroprocesso" id="pronumeroprocesso" maxlength="255" class="form-control" value="<?php echo ($_GET['pronumeroprocesso']); ?>"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-md-3 col-lg-3 control-label">Proid:</label>
                            <div class="col-sm-8 col-md-8">
                                <input type="text" name="proid" id="proid" maxlength="255" class="form-control" value="<?php echo ($_GET['proid']); ?>"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-md-3 col-lg-3 control-label">Inpid:</label>
                            <div class="col-sm-8 col-md-8">
                                <input type="text" name="inpid" id="inpid" maxlength="255" class="form-control" value="<?php echo ($_GET['inpid']); ?>"/>
                            </div>
                        </div>
                        <div class="col-lg-offset-2">
                            <?php
                            echo $simec->radio(
                                'tipo',
                                'Tipo',
                                'P',
                                array('P' => 'Par', 'O' => 'Obra'),
                                array(),
                                array('label-size' => '1', 'input-size' => '3')
                            );

                            ?>
                        </div>

                        <!--filtros-->
                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label"></label>
                            <div class="col-lg-8">
                                <div class="checkbox checkbox-inline pull-left">
                                    <input type="checkbox" name="analise" id="analise-check" <?php echo ($_GET['analise'] == 'A'?'checked':'');?>>
                                    <label for="analise"> Análise </label>
                                </div>
                                <div class="checkbox checkbox-inline pull-left">
                                    <input type="checkbox" name="documento" id="documento" <?php echo ($_GET['documento'] == 'A'?'checked':'');?>>
                                    <label for="documento"> Documento </label>
                                </div>
                                <div class="checkbox checkbox-inline pull-left">
                                    <input type="checkbox" name="empenho" id="empenho" <?php echo ($_GET['empenho'] == 'A'?'checked':'');?>>
                                    <label for="empenho"> Empenho </label>
                                </div>
                                <div class="checkbox checkbox-inline pull-left">
                                    <input type="checkbox" name="pagamento" id="pagamento" <?php echo ($_GET['pagamento'] == 'A'?'checked':'');?>>
                                    <label for="pagamento"> Pagamento </label>
                                </div>
                            </div>
                        </div>
                        <br>
                    </div>
                </div>
        </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" class="btn btn-primary" id="pesquisar-iniciativa" onclick="listarProcessos()"><i class="fa fa-search"></i> Pesquisar</button>
                    <button type="reset" class="btn btn-success" id="limpar" onclick="limparGrid()"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
        </form>
    </div>
</div>
<div class="ibox float-e-margins">
    <div class="ibox-content">
        <script>
            $.jgrid.defaults.width = 1430;
            $.jgrid.defaults.responsive = true;
            $.jgrid.defaults.styleUI = 'Bootstrap';
        </script>

        <center>
            <div id="divProcesso" style="display: none;">
                <table id="processo"></table>
                <div id="jqGridPager"></div>
            </div>
        </center>
        <div id="divProcessoVazio" style="display: none;">
            <center><h3>Não há processo com os dados informados na pesquisa.</h3></center>
        </div>

    </div>
</div>

<script>
    $("#processo").jqGrid({
        editurl: window.location.href + "&requisicao=" + 'editar' + "&" + $("#formulario").serialize(),
        mtype: "GET",
        datatype: "json",
        page: 1,
        colModel: [
            {
                label: "#",
                name: "actions",
                width: 100,
                formatter: "actions",
                formatoptions: {
                    keys: true,
                    editOptions: {},
                }
            },
            {
                label: 'Processo',
                name: 'pronumeroprocesso',
                width: 200,
            },
            {
                label: 'proid',
                name: 'proid',
                key: true,
            },
            {
                label: 'muncod',
                name: 'muncod',
                editable: true
            },
            {
                label: 'inuid',
                name: 'inuid',
                editable: true
            },
            {
                label: 'proagencia',
                name: 'proagencia',
                editable: true
            },
            {
                label: 'proseqconta',
                name: 'proseqconta',
                editable: true
            },
            {
                label: 'prostatus',
                name: 'prostatus',
                editable: true
            },
            {
                label: 'intaid',
                name: 'intaid',
                editable: true
            },


        ],
        loadonce: true,
        onSelectRow: editRow,
        viewrecords: true,
        subGrid: true,
        subGridRowExpanded: showChildGrid, // javascript function that will take care of showing the child grid
        subGridOptions:{
            expandOnLoad: true,
        },
        height:'auto',
        width:'1150',
        rowNum: 20,
        pager: "#jqGridPager",
        caption: "Processo",
        setGridState:'hidden',
        loadComplete:function() {
            var grid = $("#processo");
            var qtdRegistros = grid.jqGrid("getGridParam", "records");
            if(qtdRegistros == 0){
                $(document).find('#divProcesso').hide();
                $(document).find('#divProcessoVazio').show();
            }else{
                $(document).find('#divProcesso').show();
                $(document).find('#divProcessoVazio').hide();
            }
        }
    });

    function listarProcessos() {
        if($("#pronumeroprocesso").val() == '' && $("#proid").val()=='' && $("#inpid").val() == ''){
            alert('Favor informar ao menos um dos três campos (Processo, Proid, Inpid)');
            return;
        }

        var formValues = window.location.href + "&requisicao=" + 'listarProcessos' + "&" + $("#formulario").serialize();
        var grid = $("#processo");

        grid.jqGrid("clearGridData");
        grid.jqGrid('setGridParam', {datatype: 'json', url: formValues});
        grid.trigger('reloadGrid',[{current:true}]);
        // var reccount = grid.getGridParam('records');
        // console.log(reccount);
    }

    var lastSelection

    function editRow(id) {
        if (id && id !== lastSelection) {
            var grid = $("#processo");
            saveEditedRow(lastSelection);
            grid.jqGrid('restoreRow', lastSelection);
            grid.jqGrid('editRow', id,
                {
                    keys: true,
                    focusField: 4,
                    errorfunc: function (data) {
                        console.log(data);
                        alert('editou');
                    }
                }
            );
            lastSelection = id;
        }
    }
    function saveEditedRow(id) {
        var grid = $("#processo");
        grid.jqGrid('saveRow', id, 'clientArray');
    }

    function showChildGrid(parentRowID, proid) {
        var origemFormulario = $("#formulario").serialize();
        var resultado = origemFormulario.replace(/[^&]+=\.?(?:&|$)/g, '')

        // Composicao
        $.ajax({
            url: window.location.href + "&requisicao=" + 'montarListaInterna' + "&proid=" + proid + '&' + resultado,
            type: "GET",
            success: function (html) {
                $("#" + parentRowID).append(html);
            }
        });
    }

    function limparGrid(){
        $(document).find('#divProcesso').hide();
        $(document).find('#divProcessoVazio').hide();

    }
</script>