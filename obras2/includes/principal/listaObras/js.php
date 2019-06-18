<script type="text/javascript">

    var u = '?modulo=principal/listaObras&acao=A&req=pegaHtmlPagamento&preid=', fieldsValue2 = ['moedtprevinauguracao_i', 'moedtprevinauguracao_f'], fieldsValue3 = ['moedtpreviniciofunc_i', 'moedtpreviniciofunc_f'], OPERATION = 1, INAUGURATION = 2, INITIATION = 3;

    $(document).ready(function () {

        if ($("input:radio[name='funcionamentoflag']").is(":checked")) {

            var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent().parent()[0]), $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent().parent()[0]);

            $spaceDate1.find("img").css("display", "");
            $spaceDate2.find("img").css("display", "");

            switch (parseInt($("input:radio[name='funcionamentoflag']:checked").val())) {
                case OPERATION:
                    $spaceDate1.find("img").css("display", "none");
                    $spaceDate2.find("img").css("display", "none");
                    break;
                case INAUGURATION:
                    $spaceDate2.find("img").css("display", "none");
                    break;
                case INITIATION:
                    $spaceDate1.find("img").css("display", "none");
                    break;
            }
        }

        $('.pesquisar').click(function () {
            $('#req').val('');


            if ($("input:radio[name='funcionamentoflag']:checked").val() == 2) {
                if (!$("input[name='moedtprevinauguracao_i']").val() || !$("input[name='moedtprevinauguracao_f']").val()) {
                    alert("Voce precisa preencher o intervalo com as datas");
                    $("input[name='moedtprevinauguracao_i']").focus();
                    return false;
                }
            }

            if ($("input:radio[name='funcionamentoflag']:checked").val() == 3) {
                if (!$("input[name='moedtpreviniciofunc_i']").val() || !$("input[name='moedtpreviniciofunc_f']").val()) {
                    alert("Voce precisa preencher o intervalo com as datas");
                    $("input[name='moedtpreviniciofunc_i']").focus();
                    return false;
                }
            }

            $('#formListaObra').submit();
        });

        $('.btnEexcel').click(function () {
            $('#req').val('excel');
            $('#formListaObra').submit();
        });

        $('.novaObra').click(function () {
            window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A';
        });

        <?php
        if ($abreBuscaAvancada) {
            echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";
        }
        ?>

        $("input:radio[name='funcionamentoflag']").on("click", function (e) {

            for (var i = 0; i < fieldsValue2.length; i++) {
                $("input[name='" + fieldsValue2[i] + "']").val("");
                $("input[name='" + fieldsValue3[i] + "']").val("");
            }

            var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent()[0]), $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent()[0]);

            $spaceDate1.find("img").css("display", "");
            $spaceDate2.find("img").css("display", "");

            switch (parseInt($(this).val())) {
                case OPERATION:
                    $spaceDate1.find("img").css("display", "none");
                    $spaceDate2.find("img").css("display", "none");
                    for (var i = 0; i < fieldsValue2.length; i++) {
                        $("input[name='" + fieldsValue2[i] + "']").attr("disabled", true);
                        $("input[name='" + fieldsValue3[i] + "']").attr("disabled", true);
                    }
                    break;
                case INAUGURATION:
                    $spaceDate2.find("img").css("display", "none");
                    for (var i = 0; i < fieldsValue3.length; i++) {
                        $("input[name='" + fieldsValue2[i] + "']").attr("disabled", false);
                        $("input[name='" + fieldsValue3[i] + "']").attr("disabled", true);
                    }
                    break;
                case INITIATION:
                    $spaceDate1.find("img").css("display", "none");
                    for (var i = 0; i < fieldsValue3.length; i++) {
                        $("input[name='" + fieldsValue2[i] + "']").attr("disabled", true);
                        $("input[name='" + fieldsValue3[i] + "']").attr("disabled", false);
                    }
                    break;
            }
        });


    });

    function exibeBuscaAvancada(visivel) {
        if (visivel == true) {
            $('#tr_busca_avancada').show();
            $('#labelBuscaAvancada').hide();
            $('#abreBuscaAvancada').val('t');
        } else {
            $('#tr_busca_avancada').hide();
            $('#labelBuscaAvancada').show();
            $('#abreBuscaAvancada').val('f');
        }
    }

    function carregarMunicipio(estuf) {


        var td = $1_11('.td_municipio');
        if (estuf != '') {
            var url = location.href;
            $1_11.ajax({
                url: url,
                type: 'post',
                data: {
                    ajax: 'municipio',
                    estuf: values
                },
                dataType: "html",
                async: false,
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                    alert(2);
                },
                success: function (data) {
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                .attr('disabled', true);
        }
    }

    function carregarUnidade(muncod) {
        var td = $1_11('#td_unidade');

        if (muncod != '') {
            var url = location.href;
            $1_11.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'unidade', muncod: muncod},
                dataType: "html",
                async: false,
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                },
                success: function (data) {
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                .attr('disabled', true);
        }
    }

    function abreEvolucaoFinan(obrid) {
        janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=' + obrid, 800, 650);
    }

    function alterarObr(obrid) {
        location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
    }

    function novaObrVinculada(obrid) {
        location.href = '?modulo=principal/cadObra&acao=A&obridVinculado=' + obrid;
    }

    function abreDocumentoObjeto(obrid) {
        $('#req').val('abaDocumento');
        $('#obrid').val(obrid);

        $('#formListaObra').submit();
    }

    function abreRestricao(obrid) {
        $('#req').val('abaRestricao');
        $('#obrid').val(obrid);

        $('#formListaObra').submit();
    }

    function abreFotoObjeto(obrid) {
        janela('?modulo=principal/listaObras&acao=A&req=abaFotos&obrid=' + obrid, 800, 650);
    }
    /**
     * Alterar visibilidade de um campo.
     *
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo(campo) {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none') {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>

<script>
    $( function() {
        $( "#slider-range" ).slider({
            range: true,
            min: 0,
            max: 100,
            values: [ 40, 60 ],
            slide: function( event, ui ) {
                $("#txtPercentualinicial").text("MÃ­nimo: " + $( "#slider-range" ).slider( "values", 0 ));
                $("#txtPercentualfinal").text("MÃ¡ximo: " + $("#slider-range" ).slider( "values", 1 ));
                $("#percentualinicial").val($( "#slider-range" ).slider( "values", 0 ));
                $("#percentualfinal").val($("#slider-range" ).slider( "values", 1 ));
            }
        });

        $("#txtPercentualinicial").text("MÃ­nimo: " + $( "#slider-range" ).slider( "values", 0 ));
        $("#txtPercentualfinal").text("MÃ¡ximo: " + $("#slider-range" ).slider( "values", 1 ));
        $("#percentualinicial").val($( "#slider-range" ).slider( "values", 0 ));
        $("#percentualfinal").val($("#slider-range" ).slider( "values", 1 ));
    } );

    $("#slider-range").change(function(){
        $("#slider-idade").slider("value", this.value);
    });

</script>



<script type='text/javascript'>
    $(function () {
        $('#esdid').change(function () {
            var val = $('#esdid').val();
            if (val == '<?= ESDID_OBJ_PARALISADO ?>' || val == '<?= ESDID_OBJ_EXECUCAO ?>') {
                $('#nivelpreenchimento').removeAttr("disabled");
            } else {
                $('#nivelpreenchimento').attr("disabled", "disabled");
                $('#nivelpreenchimento').val(0);
            }
        });
    });
</script>







<script>
    $1_11(document).ready(function () {
        setTimeout(function(){

        $1_11('select[name="strid[]"]').chosen();
        $1_11('select[name="tobid[]"]').chosen();
        $1_11('select[name="cloid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="moeid[]"]').chosen();
        $1_11('select[name="estuf[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="entid[]"]').chosen();
        $1_11('select[name="stiid[]"]').chosen();
        $1_11('select[name="esdid[]"]').chosen();
        $1_11('select[name="rsuid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();


        },  100)

        $1_11(".td_municipio").on( "change", ".municipios",function() {
            carregarUnidade($1_11(this).chosen().val());
        });

    });


</script>
