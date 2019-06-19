$(document).ready(function () {

    $(document).on('click', '.btn-salvar-modal, .btn-salvar', function () {


        $('form input, form textarea, form select').trigger('blur');
        setTimeout(function () {


            if ($('form').find('.has-error').length > 0) {

                return false;
            } else {
                $('form').submit();
                return true;
            }
        }, 500);
        return false;    });



    $(document).on('blur', 'input, textarea, select', function () {
        var validadores = $(this).data('validate');
        var elemento =  $(this);
        var valor = $(this).val();
        var form = $(this).parents('.form-group');
        var label = form.find('.text-danger');
        var msg = form.find('.control-label').html();


   
        $.each(validadores, function (key, validador) {
            switch (key) {
                case 'required':
                    if ((!valor || valor == 'R$ 0,00' || !valor.trim()) && validador) {


                        form.removeClass('has-success').addClass('has-error');
                        label.html('O campo ' + msg.replace(':', '').replace('*', '') + ' é obrigatório.');
                        
                    } else if (elemento.attr('type') == 'radio' && $('input[name="'+elemento.attr('name')+'"').is(':checked') == false) {
                        
                        form.removeClass('has-success').addClass('has-error');
                        label.html('O campo ' + msg.replace(':', '').replace('*', '') + '  é obrigatório.');
                    } else {

                        form.removeClass('has-error').addClass('has-success');
                        label.html('');

                    }
                    break;
                case 'unique':
//                    var _token = $('meta[name="csrf-token"]').attr('content');
//
//                    var url = "ator/validaUniqueAjax";
//                    var valor = $(this);
//
//                    
//                    $.ajax({
//                        type: "POST",
//                        url: url,
//                        data: {'_token': _token},
//                        success: function (r) {
//                           console.log('asfd')
//                           
//                        }
//                    });

                    break;
            }
        });
    });
});



