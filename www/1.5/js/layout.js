$(document).ready(function () {
    attachInit();
});


function attachInit() {

    $('.mudar_sistema').on('change', function () {
        location.href = "/muda_sistema.php?sisid=" + $(this).val();
    })

    $.fn.serializeFormJSON = function () {

        var o = {};
        var a = this.serializeArray();
        $.each(a, function () {
            if (o[this.name]) {
                if (!o[this.name].push) {
                    o[this.name] = [o[this.name]];
                }
                o[this.name].push(this.value || '');
            } else {
                o[this.name] = this.value || '';
            }
        });
        return o;
    };

    $('.datepicker').datepicker({
        changeYear: true,
        yearRange: "1900:2030",
        format: "dd/mm/yyyy",
        minDate: 0, // 0 days offset = today
        language: "pt-BR",
        orientation: "top auto",
        autoclose: true,
        todayHighlight: true,
        container: 'body'
    });

    $('.money').maskMoney({prefix: 'R$ ', allowNegative: true, thousands: '.', decimal: ',', affixesStay: false});
    $('.telefone_com_ddd').mask('(00) 0000-0000');
    $('.numCPF').mask('999.999.999-99');
    $('input.numTelefoneCel').mask('(99) 99999-9999');

    $.ajaxSetup({
        headers: {            
            'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
        }
    });

    $.extend(true, $.fn.dataTable.defaults, {
        orderCellsTop: true,
        fixedHeader: true,
        processing: true,
        serverSide: true,
        language: {
            "sEmptyTable": "Nenhum registro encontrado",
            "sInfo": "Mostrando de _START_ até _END_ de _TOTAL_ registros",
            "sInfoEmpty": "Mostrando 0 até 0 de 0 registros",
            "sInfoFiltered": "(Filtrados de _MAX_ registros)",
            "sInfoPostFix": "",
            "sInfoThousands": ".",
            "sLengthMenu": "_MENU_ resultados por página",
            "sLoadingRecords": "Carregando...",
            "sProcessing": "Processando...",
            "sZeroRecords": "Nenhum registro encontrado",
            "sSearch": "Pesquisar",
            "oPaginate": {
                "sNext": "Próximo",
                "sPrevious": "Anterior",
                "sFirst": "Primeiro",
                "sLast": "Último"
            },
            "oAria": {
                "sSortAscending": ": Ordenar colunas de forma ascendente",
                "sSortDescending": ": Ordenar colunas de forma descendente"
            }
        }
    });

    // Chosen
    var config = {
        '.chosen-select': {allow_single_deselect: true},
        '.chosen-select-deselect': {allow_single_deselect: true},
        '.chosen-select-no-single': {disable_search_threshold: 10},
        '.chosen-select-no-results': {no_results_text: 'Oops, nothing found!'},
        '.chosen-select-width': {width: "95%"}
    }
    for (var selector in config) {
        $(selector).chosen(config[selector]);
    }

    $('.act_bt_delete').on('click', function (e) {
        e.preventDefault();
        var btn = $(this);
        alertaDePergunta([], function (isConfirm) {

        }).then(function(isConfirm) {
            if (isConfirm) {
                let action = btn.attr('href');
                let token = $('meta[name="csrf-token"]').attr('content');

                $('<form method="POST" action="' + action + '" accept-charset="UTF-8">')
                    .append('<input name="_method" type="hidden" value="DELETE"> <input name="_token" type="hidden" value="' + token + '">')
                    .appendTo('body').submit();
            }
        });
    });


    $(document).on('blur', 'input.emailValido', function (e) {
        var email = $(this).val();
        if (email) {
            var regex = /^([a-zA-Z0-9_.+-])+\@(([a-zA-Z0-9-])+\.)+([a-zA-Z0-9]{2,4})+$/;
            if(!regex.test(email) ){
                swal("Atenção", "Por favor insira um e-mail válido!", "warning");
                $(this).val('');
            }
        }
    });

    //$('[data-toggle="tooltip"]').tooltip({});
}


/**** MENSAGENS  *****/
function mensagemSucesso(titulo, mensagem) {
    swal(titulo, mensagem, "success");
}

function mensagemError(titulo, mensagem) {
    swal(titulo, mensagem, "error");
}

function alertaDePergunta($options, callback) {

   return  swal({
       title: ($options['title'] ? $options['title'] : 'Exclusão'),
       text: ($options['text'] ? $options['text'] : 'Tem certeza que deseja excluir este registro?'),
       cancelButtonText: 'Cancelar',
        icon: "warning",
        buttons: true,
        dangerMode: true,
    });
}

// -- Substituindo o alert do browser.
window.alert = function (texto) {
    jQuery('#modal-alert .modal-body').html(texto);
    jQuery('#modal-alert').modal();
};

/**** FIM MENSAGENS  *****/


/**** LOADING  *****/

/** Funcao de fazer com que o sistema informe que esta havendo uma requisicao ajax */
$(document).ajaxSend(function (e, jqxhr, settings) {
    $("#loading").fadeIn();
}).ajaxStop(function () {
    $("#loading").fadeOut();
    attachInit();
    // attachEvents();
});

/** Mensagem de carregando quando houver requisicaes em ajax */
$.ajaxSetup({
    timeout: 0,
    error: function (xhr, status, error) {
        console.log("Ocorrencia de erro no retorno AJAX: " + status + "\nError: " + error);
        jQuery("#loading").fadeOut();
        jQuery("#loading").fadeIn();

        setTimeout(function () {
            jQuery("#loading").fadeOut();
        }, 1300);
    }
});

$('#btntperfilmenumobile').on({
    "click": function(evt) {
        evt.preventDefault();
        $('#dropdownperfilmenumobile').toggle();
    },
});