<?php

$controllerEvento = new Par3_Controller_Evento();
$controllerEventoTipo = new Par3_Controller_EventoTipo();
$controllerTerritorios = new Territorios_Controller_Territorios();

$modelEventoTipo = new Par3_Model_EventoTipo();
$tiposEvento = $modelEventoTipo->recuperarTodos('*', '', 'evtordem');

switch ($_REQUEST['action']) {
    case 'salvar':
        $retorno = $controllerEvento->salvar($_REQUEST);
        ob_clean();
        echo simec_json_encode($retorno);
        exit();
    case 'salvar-tipo':
        $retorno = $controllerEventoTipo->salvar($_REQUEST);
        exit();
    case 'formulario':
        $controllerEvento->montarFormulario($_REQUEST);
        exit();
    case 'editar_tipo':
        $controllerEventoTipo->montarFormulario($_REQUEST);
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

$eventos = $controllerEvento->getEventos();

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
                    <h5>Eventos <a id="editar-tipo" href="par3.php?modulo=principal/calendario&acao=A&action=editar_tipo" title="Editar Tipos de Evento"><span class="fa fa-pencil" style="color: #ed5666;"></span></a></h5>
                </div>
                <div class="ibox-content">
                    <div id='external-events'>
                        <p>Arraste e solte um evento para o calendário.</p>
                        <?php foreach($tiposEvento as $dado) { ?>
                            <div class='external-event btn-danger' style="background-color: <?php echo $dado['evtcor']; ?>" id="tipo_<?php echo $dado['evtid']; ?>" data-evtid="<?php echo $dado['evtid']; ?>"><?php echo $dado['evtdsc']; ?></div>
                        <?php } ?>
                        <p class="m-t">
                            <input type='checkbox' id='drop-remove' class="i-checks" /> <label for='drop-remove'>Remover após arrastar.</label>
                        </p>
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
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<!-- Modal -->
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

<style>
    .fc-event-title {
        color: #fff !important;
    }
</style>
<script src="/par3/js/calendario.js"></script>
<script type="text/javascript">
    $(function(){
        /* initialize the calendar
         -----------------------------------------------------------------*/
        $('#calendar').fullCalendar({
            header: {
                left: 'prev,next today',
                center: 'title',
                //right: 'month,agendaWeek,agendaDay'
                right: 'month'
            },
            axisFormat: 'h(:mm) a',
            lang: 'pt-br',
            editable: true,
            droppable: true,
            drop: function (date) {
                // funcao do arquivo calendario.js do par3
                enviarDados(date, $(this).data('evtid'));

                if ($('#drop-remove').is(':checked')) {
                    $(this).remove();
                }
            },
            eventDrop: function(event, delta, revertFunc) {
                // funcao do arquivo calendario.js do par3
                enviarDados(event, '');
            },
            eventResize: function(event, delta, revertFunc) {
                // funcao do arquivo calendario.js do par3
                enviarDados(event, '');
            },
            eventClick: function(event, jsEvent, view) {
                var eveid = event.id
                var dados = 'action=formulario&eveid=' + eveid;
                $('#formulario-evento').load('par3.php?modulo=principal/calendario&acao=A&' + dados, function(){
                    $('#myModal').modal();
                });

            },
            dayClick: function(date, jsEvent, view) {
                var dataEvento = moment(date).format('YYYY-MM-DD');
                var dados = 'action=formulario&evedatainicio=' + dataEvento + '&evedatafim=' + dataEvento + '&evecor=3899C1';
                $('#formulario-evento').load('par3.php?modulo=principal/calendario&acao=A&' + dados, function(){
                    $('#myModal').modal();
                });
            },
            events: <?php echo $eventos; ?>
        });
    });
</script>

