<?php
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

$controllerAnaliseEngenharia = new Par3_Controller_AnaliseEngenhariaObra();

$disabled = '';

switch ($_REQUEST['req']) {
    case 'salvar':
        break;
	case 'listarSubCategorias':
    	ob_clean();
        die;
        break;
    case 'imprimir':
        $_GET['aegid'] = $_REQUEST['aegid'];
        ob_clean();
        include("analise_engenharia/template_impressao_analise.inc");
        die;
        break;
    case 'abrirModal':
        ob_clean();
        $controllerAnaliseEngenharia->montarHtmlPendenciasAnalise($_REQUEST);
        die;
        break;
    default:
        break;
}

?>

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <form method="post" name="formulario" id="formulario" class="form form-horizontal">
            <input type="hidden" name="req" value="salvar"/>
            <div class="ibox">
                <div class="ibox-title">
                    <h3 class="center">Análise de Engenharia</h3>
                    <h3 class="center"><?= (empty($_GET['obrid']) ? '' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
                </div>
                <div class="ibox-content">
                    <?php $controllerAnaliseEngenharia->listar($_GET['obrid']); ?>
                </div>

                <div class="ibox-footer">
                </div>
            </div>
        </form>
    </div>
</div>


<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="form" id="formVisualizarAnaliseEngenharia" class="form form-horizontal" enctype="multipart/form-data">
            <input name='aegid' id='aegid' value='' type='hidden'>
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Análise de Engenharia</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                        <button type="button" id="cancelar" data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Fechar
                        </button>
                        <button type="button" id="salvar"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Imprimir
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<!-- janela de impressão -->
<form id="formImpressao" name="formImpressao" action="" method="POST" target="relatorioImpressao">
    <input type="hidden" id="aegid" name="aegid" value="">
    <input type="hidden" id="req" name="req" value="imprimir">
</form>


<script>

    $(function(){
        $('#salvar').click(function(){
            imprimirAnaliseEngenhariaObra($('#aegid').val());
        })
    });

    function abrirModalInfo(id){
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: 'req=abrirModal&aegid='+id,
            success: function(data) {
                $('#modal').modal();
                $('#conteudo-modal').html(data);
                $('#aegid').val(id);
            }
        });
    }

    function imprimirAnaliseEngenhariaObra(id){

        $('#aegid').val(id);
        var janela = window.open( '', 'relatorioImpressao', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        var formulario = document.formImpressao;
        formulario.target = 'relatorioImpressao';
        janela.focus();
        formulario.submit();
    }

</script>