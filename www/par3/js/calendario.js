$(document).ready(function () {

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
                enviarDados(inuid, date, $(this).data('evtid'));

                if ($('#drop-remove').is(':checked')) {
                    $(this).remove();
                }
            },
            eventDrop: function(event, delta, revertFunc) {
                // funcao do arquivo calendario.js do par3
                enviarDados(inuid, event, '');
            },
            eventResize: function(event, delta, revertFunc) {
                // funcao do arquivo calendario.js do par3
                enviarDados(inuid, event, '');
            },
            eventClick: function(event, jsEvent, view) {

                var eveid = event.id;
                var dados = 'action=verificarPermissaoEditar&eveid=' + eveid;

                $.ajax({
                    url: 'par3.php?modulo=principal/evento/calendario&acao=A',
                    data: dados,
                    type: 'POST',
                    success: function (msgerro) {
                        if(msgerro){
                            swal('', msgerro, 'error');
                        }
                        else {
                            var dados = 'action=formulario&eveid=' + eveid + '&inuid=' + inuid;
                            $('#formulario-evento').load('par3.php?modulo=principal/evento/calendario&acao=A&' + dados, function(){

                                if($('#pode-editar').val()){
                                    $('#btn-salvar').show();
                                } else {
                                    $('#btn-salvar').hide();
                                }

                                $('#myModal').modal();
                            });
                        }
                    },
                    error: function () {
                        swal("", "Houve um erro ao Adicionar o evento!", 'error');
                    }
                });


            },
            dayClick: function(date, jsEvent, view) {
                var dataEvento = moment(date).format('YYYY-MM-DD');
                var dados = 'action=formulario&evedatainicio=' + dataEvento + '&evedatafim=' + dataEvento + '&evecor=3899C1&inuid=' + inuid;
                $('#formulario-evento').load('par3.php?modulo=principal/evento/calendario&acao=A&' + dados, function(){
                    $('#myModal').modal();
                });
            },
            events: '/par3/par3.php?modulo=principal/evento/calendario&acao=A&action=recuperarEventos&inuid=' + inuid,
            eventRender: function(event, element) {
                element.popover({
                    content: event.description || 'Não possuí descrição cadastrada.',
                    trigger: 'hover',
                    placement: 'bottom',
                    delay: {'show': 300, 'hide': 150}
                });
            }
        });
    });

    $('.i-checks').iCheck({
        checkboxClass: 'icheckbox_square-green',
        radioClass: 'iradio_square-green'
    });

    /* initialize the external events
     -----------------------------------------------------------------*/
    $('#external-events div.external-event').each(function () {

        // store data so the calendar knows to render an event upon drop
        $(this).data('event', {
            title: $.trim($(this).text()), // use the element's text as the event title
            stick: true // maintain when user navigates (see docs on the renderEvent method)
        });

        // make the event draggable using jQuery UI
        $(this).draggable({
            zIndex: 1111999,
            revert: true, // will cause the event to go back to its
            revertDuration: 0  //  original position after the drag
        });

    });

    $('#btn-salvar').click(function () {
        var eveid = $('#eveid').val();
        options = {
            dataType: 'json',
            success: function (response) {
                $('#myModal').modal('hide');
                if(eveid){
                    $('#calendar').fullCalendar( 'removeEvents', eveid);
                }
                $('#calendar').fullCalendar('renderEvent', response, false);
            }
        }

        if( !$('#evetitulo').val() ){
            swal("", "É obrigatório o preechimento do Título!", 'error');
        }else if ( !$('#evedatainicio').val() ){
            swal("", "É obrigatório o preechimento da data de início!", 'error');
        }else{
            jQuery("#formulario-modal").ajaxForm(options).submit();
        }

    });


    $('#btn-salvar-tipo').click(function () {
        jQuery("#formulario-modal-tipo").submit();
    });

    $('#editar-tipo').click(function () {
        $('#formulario-evento-tipo').load($(this).attr('href'), function(){
            $('#modal-tipo').modal();
        });
        return false
    });

    $('#formulario-evento').on('change', '#id_regiao', function(){
        carregarSelectPorJson(
            'par3.php?modulo=principal/evento/calendario&acao=A&action=carregarUf&regcod='+ $('#id_regiao').val(),
            '#id_uf', 'estuf', 'estdescricao', null, 'Selecione');
    });

    $('#formulario-evento').on('change', '#id_uf', function(){
        carregarSelectPorJson(
            'par3.php?modulo=principal/evento/calendario&acao=A&action=carregarMunicipio&estuf='+ $('#id_uf').val(),
            '#id_municipio', 'muncod', 'mundescricao', null, 'Selecione');
    });
});

function enviarDados(inuid, event, evtid){
    var data = prepararDados(inuid, event, evtid);

    $.ajax({
        url: 'par3.php?modulo=principal/evento/calendario&acao=A',
        data: data,
        type: 'POST',
        dataType: 'json',
        success: function (response) {
            if(evtid){
                $('#calendar').fullCalendar('renderEvent', response, false);
            }
        },
        error: function () {
            swal("", "Houve um erro ao Adicionar o evento!", 'error');
        }
    });
}

function prepararDados(inuid, event, evtid){
    if(event instanceof Date){
        var dataEvento = moment(event).format('YYYY-MM-DD');
        var data = {
            action: 'salvar',
            evtid: evtid,// Tipo do evento
            evedatainicio: dataEvento,
            evedatafim: dataEvento,
            hrInicio: event,
            inuid: inuid
        };
    } else {
        var datafim = event.end ? event.end : event.start;
        var data = {
            action: 'salvar',
            eveid:  event.id,
            evedatainicio: moment(event.start).format('YYYY-MM-DD HH:mm:ss'),
            evedatafim: moment(datafim).format('YYYY-MM-DD HH:mm:ss'),
            hrInicio: event.start,
            inuid: inuid
        };
    }
    return data;
}