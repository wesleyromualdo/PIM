<?php
$controllerEvento = new Par3_Controller_Evento();
$controllerEventoTipo = new Par3_Controller_EventoTipo();
$controllerTerritorios = new Territorios_Controller_Territorios();

$inuid = $_REQUEST['inuid'];

$modelEventoTipo = new Par3_Model_EventoTipo();
$tiposEvento = $modelEventoTipo->recuperarTodos('*', '', 'evtordem');

switch ($_REQUEST['action']) {
    case 'recuperarEventos':
        ob_clean();
        echo $controllerEvento->recuperarEventos($inuid);
        exit();
    case 'salvar':
        $retorno = $controllerEvento->salvar($_REQUEST);
        ob_clean();
        echo simec_json_encode($retorno);
        exit();
    case 'salvar-tipo':
        $retorno = $controllerEventoTipo->salvar($_REQUEST);
        exit();
    case 'verificarPermissaoEditar':
        echo $controllerEvento->verificarPermissaoEditar($_REQUEST);
        exit();
    case 'formulario':
        $controllerEvento->montarFormulario($_REQUEST);
        exit();
    case 'editar_tipo':
        $controllerEventoTipo->montarFormulario($_REQUEST);
        exit();
    case 'excluir-evento':
        echo $controllerEvento->excluir($_REQUEST['eveid']);
        exit();
    case 'carregarUf':
        ob_clean();
        echo $controllerTerritorios->recuperarUfsPorRegiao($_REQUEST['regcod']);
        exit();
    case 'carregarMunicipio':
        ob_clean();
        echo $controllerTerritorios->recuperarMunicipiosPorUf($_REQUEST['estuf']);
        exit();
    default:
        break;
}

$nivelAcesso = $controllerEvento->recuperarNivelAcesso($inuid);

/**
 * Calendario de Municipio
 *
 * @category Calendario
 * @package  A1
 * @author   Jair Foro <jairsantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="col-lg-3">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Eventos
                        <?php if($nivelAcesso == Par3_Controller_Evento::K_NIVEL_ACESSO_CONFIGURACAO){ ?>
                            <a id="editar-tipo" href="par3.php?modulo=principal/evento/calendario&acao=A&action=editar_tipo" title="Editar Tipos de Evento"><span class="fa fa-pencil" style="color: #ed5666;"></span></a>
                        <?php } ?>
                    </h5>
                </div>
                <div class="ibox-content">
                    <div id='external-events'>
                        <p>Arraste e solte um evento para o calendário.</p>
                        <?php foreach($tiposEvento as $dado) { ?>
                            <div class='external-event btn-danger' style="background-color: <?php echo $dado['evtcor']; ?>" id="tipo_<?php echo $dado['evtid']; ?>" data-evtid="<?php echo $dado['evtid']; ?>"><?php echo $dado['evtdsc']; ?></div>
                        <?php } ?>
<!--                        <p class="m-t">-->
<!--                            <input type='checkbox' id='drop-remove' class="i-checks" /> <label for='drop-remove'>Remover após arrastar.</label>-->
<!--                        </p>-->
                    </div>
                </div>
            </div>
        </div>
        <div class="col-lg-9">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h1>Configuração do Calendário da Rede</h1>
                </div>
                <div class="ibox-content">
                    <div id="calendar"></div>
                </div>
            </div>
        </div>
    </div>
</div>

<!-- Modal -->
<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Evento</h4>
            </div>
            <div class="modal-body" id="formulario-evento">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-primary" id="btn-salvar">Salvar</button>
                <button type="button" class="btn btn-default" data-dismiss="modal" id="btn-fechar-evento">Fechar</button>
            </div>
        </div>
    </div>
</div>

<?php if($nivelAcesso == Par3_Controller_Evento::K_NIVEL_ACESSO_CONFIGURACAO){ ?>
    <!-- Modal Tipo de Evento-->
    <div class="modal fade" id="modal-tipo" tabindex="-1" role="dialog" aria-labelledby="modalTipoLabel">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title" id="modalTipoLabel">Tipo de Evento</h4>
                </div>
                <div class="modal-body" id="formulario-evento-tipo">
    
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-primary" id="btn-salvar-tipo">Salvar</button>
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </div>
    </div>
<?php } ?>

<?php 

	$url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='.$_REQUEST['inuid'];
	$urlFeiraoProgramas = 'par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid='.$_REQUEST['inuid'];
?>
<div id="materialButton" class="click-to-toggle fixed-action-btn" style="bottom: 45px; right: 24px;">
	<a class="btn_par-floating btn_par-large red">
	   	<i class="large material-icons ">view_headline</i>
    </a>
    <ul>
      	<li><a class="btn_par-floating tourn-btn indigo" title="Tour"><i class="material-icons">language</i></a></li>
      	<li><a class="btn_par-floating green" title="Imprimir" href="javascript:window.print();"><i class="material-icons">print</i></a></li>
      	<?php if( $_REQUEST['inuid'] != '' ){ ?>
	      	<li><a class="btn_par-floating blue" title="Programas do MEC" href="<?php echo $urlFeiraoProgramas; ?>"><i class="material-icons">view_module</i></a></li>
	      	<li><a class="btn_par-floating amber" title="Dados da Unidade" href="<?php echo $url; ?>"><i class="material-icons">visibility</i></a></li>
    	<?php } ?>
    </ul>
</div> 
<script>
$(document).ready(function(){

    $('.btn_par-large').click(function(){
        if( $('#materialButton').hasClass("active") ){
        	$('.click-to-toggle').closeFAB();
        }else{
        	$('.click-to-toggle').openFAB();
        }
    });
});

</script>
<style>
    .fc-event-title {
        color: #fff !important;
    }
</style>
<script src="/par3/js/calendario.js"></script>
<script type="text/javascript">
    var inuid = '<?php echo $inuid; ?>';
</script>