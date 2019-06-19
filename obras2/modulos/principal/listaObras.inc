<?php

require APPRAIZ . 'obras2/includes/principal/listaObras/ctrl.php';

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

//monta_titulo('Lista de Obras', 'Filtre as Obras.');
monta_titulo_obras('Lista de Obras', 'Filtre as Obras');

/**
 * EXTRAI OS DADOS DA PESQUISA PARA RECARREGAR O FORM
 */
extract($_POST);
?>

<link rel="stylesheet" type="text/css" href="/includes/superTitle.css"/>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/obras2/js/obras2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

    var u = '?modulo=principal/listaObras&acao=A&req=pegaHtmlPagamento&preid=', fieldsValue2 = ['moedtprevinauguracao_i', 'moedtprevinauguracao_f'], fieldsValue3 = ['moedtpreviniciofunc_i', 'moedtpreviniciofunc_f'], OPERATION = 1, INAUGURATION = 2, INITIATION = 3;

    $(document).ready(function () {


        if ($("input:radio[name='funcionamentoflag']").is(":checked")) {

            var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent().parent()[0]);
            var $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent().parent()[0]);

            // $spaceDate1.find("img").css("display", "");
            // $spaceDate2.find("img").css("display", "");

            // switch (parseInt($("input:radio[name='funcionamentoflag']:checked").val())) {
            //     case OPERATION:
            //         $spaceDate1.find("img").css("display", "none");
            //         $spaceDate2.find("img").css("display", "none");
            //         break;
            //     case INAUGURATION:
            //         $spaceDate2.find("img").css("display", "none");
            //         break;
            //     case INITIATION:
            //         $spaceDate1.find("img").css("display", "none");
            //         break;
            // }
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

            // $spaceDate1.find("img").css("display", "");
            // $spaceDate2.find("img").css("display", "");

            switch (parseInt($(this).val())) {
                case OPERATION:
                    // $spaceDate1.find("img").css("display", "none");
                    // $spaceDate2.find("img").css("display", "none");
                    for (var i = 0; i < fieldsValue2.length; i++) {
                        $("input[name='" + fieldsValue2[i] + "']").attr("disabled", true);
                        $("input[name='" + fieldsValue3[i] + "']").attr("disabled", true);
                    }
                    break;
                case INAUGURATION:
                    // $spaceDate2.find("img").css("display", "none");
                    for (var i = 0; i < fieldsValue3.length; i++) {
                        $("input[name='" + fieldsValue2[i] + "']").attr("disabled", false);
                        $("input[name='" + fieldsValue3[i] + "']").attr("disabled", true);
                    }
                    break;
                case INITIATION:
                    // $spaceDate1.find("img").css("display", "none");
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
            $('.tr_busca_avancada').fadeIn(400);
            $('#labelBuscaAvancada').fadeOut(300);
            $('#abreBuscaAvancada').val('t');
        } else {
            $('.tr_busca_avancada').fadeOut(400);
            $('#labelBuscaAvancada').fadeIn(300);
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
                $("#txtPercentualinicial").text("Mínimo: " + $( "#slider-range" ).slider( "values", 0 ));
                $("#txtPercentualfinal").text("Máximo: " + $("#slider-range" ).slider( "values", 1 ));
                $("#percentualinicial").val($( "#slider-range" ).slider( "values", 0 ));
                $("#percentualfinal").val($("#slider-range" ).slider( "values", 1 ));
            }
        });

        $("#txtPercentualinicial").text("Mínimo: " + $( "#slider-range" ).slider( "values", 0 ));
        $("#txtPercentualfinal").text("Máximo: " + $("#slider-range" ).slider( "values", 1 ));
        $("#percentualinicial").val($( "#slider-range" ).slider( "values", 0 ));
        $("#percentualfinal").val($("#slider-range" ).slider( "values", 1 ));
    } );

    $("#slider-range").change(function(){
        $("#slider-idade").slider("value", this.value);
    });

</script>
<style>
    /*#slider-container {width:200px;}*/
    #slider-range #slider-min-value {float:left;margin-top:6px;}
    #slider-range #slider-max-value {float:right;margin-top:6px;}
    /*FUNDO GERAL DO DESLIZADOR*/

    #slider-range .ui-widget-content { border: 1px solid #aaaaaa; background: #dfe5e7;}
    /*ENTRE 2 MARCADOR COLOR*/

    #slider-range .ui-widget-header { border: 1px solid #aaaaaa; background: #E0EEEE; }
    /*MARCADOR*/

    #slider-range .ui-state-default { border: 1px solid #aaaaaa; background: #ffffff; }
</style>

<form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">

    <div class="form-filters">
        <div class="row text-center">
            <div class="col-lg-12 position-relative">
                Nome da Obra / ID: <?= campo_texto('obrbuscatexto', 'N', 'S', '', 50, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
            
                <div onclick="javascript:exibeBuscaAvancada(true);" class="btn btn-primary float-right" id="labelBuscaAvancada">Busca Avançada</div>
            </div>

            
        </div>

            <div  class="row tr_busca_avancada" style="display: none;">
                <div class="row">
                    <div class="col-md-12 text-center">
                        <div onclick="exibeBuscaAvancada(false);" class="btn btn-danger btn-sm">Fechar Busca Avançada</div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-lg-6">
                        <div class="row">
                            <div class="col-md-4">
                                Processo:
                            </div>
                            <div class="col-md-8">
                                <?php
                                echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
                                ?>
                                Ano:&nbsp;
                                <?php
                                echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                                ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Convênio/Termo:
                            </div>
                            <div class="col-md-8">
                                <?php
                                echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
                                ?>
                                Ano:&nbsp;
                                <?php
                                echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                                ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Situação:
                            </div>
                            <div class="col-md-8">
                                <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php $situacaoRegistro = new SituacaoRegistro();
                                    foreach ($situacaoRegistro->listaCombo() as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($strid) && in_array($key['codigo'], $strid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Tipo de Obra:
                            </div>
                            <div class="col-md-8">
                                <select name="tobid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php  $tipoObra = new TipoObra();
                                    foreach ($tipoObra->listaCombo() as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($tobid) && in_array($key['codigo'], $tobid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Classificação da Obra:
                            </div>
                            <div class="col-md-8">
                                <select name="cloid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php  $classificacaoObra = new ClassificacaoObra();
                                    foreach ($classificacaoObra->listaCombo() as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($cloid) && in_array($key['codigo'], $cloid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Tipologia:
                            </div>
                            <div class="col-md-8">
                                <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php   $tipologiaObra = new TipologiaObra();
                                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                                    $dados = $tipologiaObra->listaCombo($param, false);
                                    foreach ($dados as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($tpoid) && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Programa:
                            </div>
                            <div class="col-md-8">
                                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php  $programa = new ProgramaFonte();
                                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                                    foreach ($programa->listacombo($param, false) as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Fonte:
                            </div>
                            <div class="col-md-8">
                                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                                    $param = array();
                                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Modalidade de Ensino:
                            </div>
                            <div class="col-md-8">
                                <select name="moeid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php  $modalidade = new ModalidadeEnsino();
                                    foreach ($modalidade->listaCombo() as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($moeid) && in_array($key['codigo'], $moeid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Última Atualização:
                            </div>
                            <div class="col-md-8">
                                <?php
                                $ultatualizacao = $_POST["ultatualizacao"];
                                $arSel = array(
                                    array("codigo" => 1, "descricao" => "Em até 25 dias"),
                                    array("codigo" => 2, "descricao" => "Entre 25 e 30 dias"),
                                    array("codigo" => 3, "descricao" => "Mais de 30 dias"),
                                );
                                $db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "200", "N", "ultatualizacao");
                                ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Tipo Vistoria (Realizado por):
                            </div>
                            <div class="col-md-8">
                                <select name="rsuid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                    <?php
                                    $realizacaosupervisao = new RealizacaoSupervisao();
                                    foreach ($realizacaosupervisao->listaCombo() as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($rsuid) && in_array($key['codigo'], $rsuid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Valor da Obra:
                            </div>
                            <div class="col-md-8">
                                De:&nbsp;
                                <?php
                                echo campo_texto('obrvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                ?>
                                Até:&nbsp;
                                <?php
                                echo campo_texto('obrvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Municípios:
                            </div>
                            <div class="col-md-8">
                                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                                    <?php   $municipio = new Municipio();
                                    foreach ($municipio->listaComboMulti() as $key) {
                                        ?>
                                        <option
                                                value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <?php if(!empty($muncod)){ ?>
                            <div class="row">
                                <div class="col-md-4">
                                    Unidade:
                                </div>
                                <div class="col-md-8">
                                    <?php
                                        $entidade = new Entidade();
                                        $a = $entidade->listaComboMulti($muncod);
                                        ?>
                                        <select name="entid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                            <?php

                                            foreach ($entidade->listaComboMulti($muncod) as $key) {
                                                ?>
                                                <option
                                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($entid) && in_array($key['codigo'], $entid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                            <?php } ?>
                                        </select>
                                </div>
                            </div>
                        <?php } ?>
                        <div class="row">
                            <div class="col-md-4">
                                Possui foto:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="totalfoto" value="S" <?= ($_POST["totalfoto"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="totalfoto" value="N" <?= ($_POST["totalfoto"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['totalfoto'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="totalfoto" value="" <?= ($_POST["totalfoto"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Possui vistoria:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="possui_vistoria" value="S" <?= ($_POST["possui_vistoria"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="possui_vistoria" value="N" <?= ($_POST["possui_vistoria"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['possui_vistoria'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="possui_vistoria" value="" <?= ($_POST["possui_vistoria"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">
                        <div class="row">
                            <div class="col-md-4">
                                Possui restrição:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="res_estado" value="S" <?= ($_POST["res_estado"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="res_estado" value="N" <?= ($_POST["res_estado"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['res_estado'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="res_estado" value="" <?= ($_POST["res_estado"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Restrição superada:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="res_superada" value="S" <?= ($_POST["res_superada"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="res_superada" value="N" <?= ($_POST["res_superada"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['res_superada'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="res_superada" value="" <?= ($_POST["res_superada"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Possui aditivo:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="ocraditivado" value="S" <?= ($_POST["ocraditivado"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="ocraditivado" value="N" <?= ($_POST["ocraditivado"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['ocraditivado'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="ocraditivado" value="" <?= ($_POST["ocraditivado"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Possui Supervisão por O.S:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="responsavel" value="S" <?= ($_POST["responsavel"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="responsavel" value="N" <?= ($_POST["responsavel"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['responsavel'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="responsavel" value="" <?= ($_POST["responsavel"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Listar as Obras Vinculantes:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="obrvinculada" value="S" <?= ($_POST["obrvinculada"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['obrvinculada'] == "" || $_POST['obrvinculada'] == "N" ? "active" : "") ?>">
                                        <input type="radio" name="obrvinculada" value="N" <?= ($_POST["obrvinculada"] == "" || $_POST['obrvinculada'] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="obrvinculada" value="T" <?= ($_POST["obrvinculada"] == "T" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Recebeu o primeiro repasse:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="repasse" value="S" <?= ($_POST["repasse"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="repasse" value="N" <?= ($_POST["repasse"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['repasse'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="repasse" value="" <?= ($_POST["repasse"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Possui checklist administrativo:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="chk_adm" value="S" <?= ($_POST["chk_adm"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="chk_adm" value="N" <?= ($_POST["chk_adm"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['chk_adm'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="chk_adm" value="" <?= ($_POST["chk_adm"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Possui checklist 2ª parcela:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="chk_2pc" value="S" <?= ($_POST["chk_2pc"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="chk_2pc" value="N" <?= ($_POST["chk_2pc"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['chk_2pc'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="chk_2pc" value="" <?= ($_POST["chk_2pc"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                % Executado da Obra:
                            </div>
                            <div class="col-md-8">
                                <label id="txtPercentualinicial"></label>
                                &nbsp;&nbsp;
                                <label id="txtPercentualfinal"></label>
                                <input type="hidden" id="percentualfinal" name="percentualfinal" readonly>
                                <input type="hidden" id="percentualinicial" name="percentualinicial" readonly>
                                <div style="width: 200px;">
                                <div id="slider-range"></div>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                % Executado da Obra:
                            </div>
                            <div class="col-md-8">
                                Mínimo:
                                <?php
                                    for ($i = 0; $i <= 100; $i++) {
                                        $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                                    }
                                    $percentualinicial = $_POST['percentualinicial'];
                                    $percentualfinal = $_POST['percentualfinal'];
                                    $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                                    $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                                ?>
                                Máximo: 
                                <?php
                                    $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                                ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Possui Marcador de rgo de Controle:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="chk_org_controle" value="S" <?= ($_POST["chk_org_controle"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="chk_org_controle" value="N" <?= ($_POST["chk_org_controle"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['chk_org_controle'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="chk_org_controle" value="" <?= ($_POST["chk_org_controle"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Funcionamento:
                            </div>
                            <div class="col-md-8">
                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 1) ? 'checked' : ''; ?> name="funcionamentoflag" id="moefuncionamentoflag_s" value="1"/> Em Funcionamento
                                <br>
                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 2) ? 'checked' : ''; ?>
                                            name="funcionamentoflag" id="moefuncionamentoflag_pi" value="2"/>
                                    Previsão de Inauguração: (escolha as datas)<br>
                                    <input type="text" class="date" id="moedtprevinauguracao_i" name="moedtprevinauguracao_i">
                                    &nbsp;até&nbsp;
                                    <input type="text" class="date" id="moedtprevinauguracao_f" name="moedtprevinauguracao_f">
                                <br>
                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 3) ? 'checked' : ''; ?>
                                            name="funcionamentoflag" id="moefuncionamentoflag_if" value="3"/>
                                    Previsão de Início de Funcionamento: (escolha as datas)<br>
                                    <input type="text" class="date" id="moedtpreviniciofunc_i" name="moedtpreviniciofunc_i">
                                    &nbsp;até&nbsp;
                                    <input type="text" class="date" id="moedtpreviniciofunc_f" name="moedtpreviniciofunc_f">
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Mobiliário Empenhado:
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="moeempenhadoflag" value="S" <?= ($_POST["moeempenhadoflag"] == "S" ? "checked" : "") ?>> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="moeempenhadoflag" value="N" <?= ($_POST["moeempenhadoflag"] == "N" ? "checked" : "") ?>> Não
                                    </label>
                                    <label class="btn btn-default <?= ($_POST['moeempenhadoflag'] == "" ? "active" : "") ?>">
                                        <input type="radio" name="moeempenhadoflag" value="" <?= ($_POST["moeempenhadoflag"] == "" ? "checked" : "") ?>> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
<!-- Fim tablela novo layout               -->


    <div class="row form-filters text-center">
        <div class="col">
            <?php
            if (possui_perfil(PFLCOD_SUPER_USUARIO)):
                ?>
                <div class="novaObra btn btn-info">
                    <span class="glyphicon glyphicon-plus"></span> Nova Obra
                </div>
            <?php endif; ?>
            <button type="button" name="pesquisar" class="btn btn-success pesquisar">
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>
            <button type="button" name="btnEexcel" class="btn btn-primary btnEexcel">
                <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel
            </button>
        </div>
    </div>
</form>



<!-- <div class="table-responsive"> -->
<?php

if ((array_key_exists('obrid', $_POST) || array_key_exists('esdid', $_POST) || array_key_exists('estuf', $_POST)) || !possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC, PFLCOD_CALL_CENTER, PFLCOD_ADMINISTRADOR))) {

    $obras = new Obras();
    $filtro = $_POST;

    if ($filtro['req'] == 'excel') {
        ini_set("memory_limit", "1024M");
        $filtro['excel'] = true;
        $cabecalho = getCabecalho(true);
        $sql = listaSql($filtro);
        $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
    } else {
        $cabecalho = getCabecalho();
        $sql = listaSql($filtro);
        $cache = (IS_PRODUCAO) ? 600 : null;

        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', "formulario", '', '', $cache, array('classTable' => 'view-table-long', 'table-responsive' => true));

    }
} else {
    ?>
    <div class="row form-filters">
        <div class="col-md-12 text-center">
            Para listar as obras, preencha os filtros desejados e clique em pesquisar. Pesquisar sem preencher nada retornará
            todos os resultados que você pode acessar.
        </div>
    </div>
    <?php
}
?>

<!-- </div> -->

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



<style>
    

    label.btn.active {
        background-image: none;
        outline: 0;
        -webkit-box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        color: #ffffff;
        background-color: #3276b1 !important;
        border-color: #285e8e;
    }

    .tdLabelForm{
        width: 13%;
    }

    .tdInputForm{
        width: 37%;
    }
</style>


<script>
    $1_11(document).ready(function () {
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
      //  $1_11('select[name="ultatualizacao"]').chosen();


        $1_11(".td_municipio").on( "change", ".municipios",function() {
            carregarUnidade($1_11(this).chosen().val());
        });

    });

    number_format = function(){
        return 9999;
    }
</script>
