/*mascaras do sistema*/


$(".real").inputmask('decimal', {
                                'alias': 'numeric',
                                'groupSeparator': ',',
                                'autoGroup': true,
                                'digits': 2,
                                'radixPoint': ".",
                                'digitsOptional': false,
                                'allowMinus': false,
                                'prefix': 'R$ ',
                                'placeholder': ''
                    });





$(".data").inputmask({ mask: "99/99/99999"});

$(".cnpj").inputmask({ mask: "99.999.999/9999-99"});



