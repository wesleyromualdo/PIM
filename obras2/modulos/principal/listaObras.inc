<?php

ini_set("memory_limit", "512M");

ob_start();

$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; // $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current($arOrgid));
$orgid = $_SESSION['obras2']['orgid'];

switch ($_REQUEST['req']) {
    case 'abaFotos':
//      $_SESSION['imgparams'] = array("filtro" => "obrid=".$_GET['obrid'],
//                                     "tabela" => "obras2.obras_arquivos");
//      header('Location: /slideshow/slideshow/index.php?pagina=&_sisarquivo=obras2');
        $url = '/slideshow/slideshow/obras2_galeriaGaleriaFotos.php?tipo=abaGaleria&obrid=' . $_GET['obrid'];
        header('Location: ' . $url);
        die();
    case 'abaRestricao':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras();
        $obras->carregarPorIdCache($obrid);
        
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/listaRestricao&acao=O');
        die();
    case 'abaDocumento':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras();
        $obras->carregarPorIdCache($obrid);
        
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/cadObraDocumentos&acao=A');
        die();
//    case 'apagar':
//        $obra = new Obras($_REQUEST['obrid']);
//        if ($obra->obrid) {
//            $obra->obrstatus = 'I';
//            $obra->salvar();
//        }
//        $db->commit();
//        die('<script type="text/javascript">
//              alert(\'Operao realizada com sucesso!\');
//              location.href=\'?modulo=principal/listaObras&acao=A\';
//           </script>');
    case 'pegaHtmlPagamento':
        if ($_REQUEST['preid']) {
            /* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
             * Analista: Daniel Areas Programador Eduardo Duniceu
            * */
            $sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
                    FROM par.empenhoobra emo
                    INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
                    INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                    INNER JOIN par.pagamento pag ON pag.empid = emo.empid
                    WHERE emo.preid = '" . $_REQUEST['preid'] . "' AND pag.pagstatus='A'
                    UNION ALL
                    SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
                    FROM par.empenhoobrapar emo
                    INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
                    INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                    INNER JOIN par.pagamento pag ON pag.empid = emo.empid
                    WHERE emo.preid = '" . $_REQUEST['preid'] . "' AND pag.pagstatus='A'";

            $pagamentoobra = $db->pegaLinha($sql);

            if ($pagamentoobra) {
                echo "Pago<br>
                      Valor pagamento(R$): " . number_format($pagamentoobra['pagvalorparcela'], 2, ",", ".") . "<br>
                      N da Ordem Bancria: " . $pagamentoobra['pagnumeroob'] . "<br>
                      Data do pagamento: " . $pagamentoobra['pagdatapagamento'] . "<br>
                      Banco: " . $pagamentoobra['probanco'] . ", Agncia: " . $pagamentoobra['proagencia'] . "
                        ";
            } else {
                echo "No pago";
            }
        }
        die();
    case 'listaObrasUnidade':
        $obras = new Obras();
        $filtro = $_POST;
        $filtro['not(obridpai)'] = true;
        $cabecalho = $obras->cabecalhoListaSql();
        $sql = ($_POST ? $obras->listaSql($filtro) : array());
        //$cabecalho = array("Ao","Objeto", "Classificao do Objeto", "Tipo do Objeto", "Valor Previsto");
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, "formulario");
        die();
    case 'supervisorFNDE':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $_SESSION['obras2']['empid'] = $_POST['empid'];
        header("Location: obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A");
        exit;
        break;

}

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $estuf = $_REQUEST['estuf'];
        ?>

        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>

        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
    case 'unidade':
        header('content-type: text/html; charset=UTF-8');
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="entid[]"]').chosen();

            });
        </script>

        <select name="entid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php
            $entidade = new Entidade();
            foreach ($entidade->listaComboMulti($_POST['muncod']) as $key) {
                ?>
                <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($entid) && in_array($key['codigo'], $entid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php

        exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgao');
echo montarAbasArray($arAba, "?modulo=principal/listaObras&acao=A&orgid=" . $orgid);

//monta_titulo('Lista de Obras', 'Filtre as Obras.');
monta_titulo_obras('Lista de Obras', 'Filtre as Obras.');

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


//    $('.estados').change(function() {
//        alert(1);
//        // can now use params.selected and params.deselected
//     //   carregarMunicipio();
//    });


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

    //    function excluirObr(obrid) {
    //        if (confirm('Deseja apagar este obra?')) {
    //            $('#obrid').val(obrid);
    //            $('#req').val('apagar');
    //            $('#formListaObra').submit();
    //        }
    //    }

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
                $("#txtPercentualinicial").text("Mnimo: " + $( "#slider-range" ).slider( "values", 0 ));
                $("#txtPercentualfinal").text("Mximo: " + $("#slider-range" ).slider( "values", 1 ));
                $("#percentualinicial").val($( "#slider-range" ).slider( "values", 0 ));
                $("#percentualfinal").val($("#slider-range" ).slider( "values", 1 ));
            }
        });

        $("#txtPercentualinicial").text("Mnimo: " + $( "#slider-range" ).slider( "values", 0 ));
        $("#txtPercentualfinal").text("Mximo: " + $("#slider-range" ).slider( "values", 1 ));
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


<br>
<form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
    <table align="center" style='background-color: #FFFFff' border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="divLabelForm" width="15%">Nome da Obra / ID:</td>
            <td>
                <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <a href="javascript:exibeBuscaAvancada( true );" id="labelBuscaAvancada">[Busca avançada]</a>
            </td>
        </tr>
        <tr>
            <td id="tr_busca_avancada" colspan="2" style="display: none;">


                <!-- Inicio tablela novo layout               -->
            <table align="center" style='background-color: #FFFFff' border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <th colspan="4" style='background-color: #D3E0E0'>
                        Busca avançada
                        <a style="float:right; " onclick="exibeBuscaAvancada(false);">[Fechar]</a>
                    </th>
                </tr>
                <tr>
<!--                    Coluna 1-->
                    <td width="50%">
                        <table width="100%">
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Processo:</td>
                                <td class="tdInputForm">
                                    <?php
                                    echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
                                    ?>
                                    Ano:&nbsp;
                                    <?php
                                    echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Convênio/Termo:</td>
                                <td class="tdInputForm">
                                    <?php
                                    echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
                                    ?>
                                    Ano:&nbsp;
                                    <?php
                                    echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Situação:
                                </td>
                                <td class="tdInputForm">
                                    <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php $situacaoRegistro = new SituacaoRegistro();
                                        foreach ($situacaoRegistro->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($strid) && in_array($key['codigo'], $strid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Tipo de Obra:
                                </td>
                                <td class="tdInputForm">
                                    <select name="tobid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $tipoObra = new TipoObra();
                                        foreach ($tipoObra->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($tobid) && in_array($key['codigo'], $tobid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Classificao da Obra:
                                </td>
                                <td class="tdInputForm">
                                    <select name="cloid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $classificacaoObra = new ClassificacaoObra();
                                        foreach ($classificacaoObra->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($cloid) && in_array($key['codigo'], $cloid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Tipologia:
                                </td>
                                <td class="tdInputForm">
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
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Programa:
                                </td>
                                <td class="tdInputForm">
                                    <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $programa = new ProgramaFonte();
                                        $param = array("orgid" => $_SESSION['obras2']['orgid']);
                                        foreach ($programa->listacombo($param, false) as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Fonte:
                                </td>
                                <td class="tdInputForm">
                                    <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $tipoOrigemObra = new TipoOrigemObra();
                                        $param = array();
                                        foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Modalidade de Ensino:
                                </td>
                                <td class="tdInputForm">
                                    <select name="moeid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $modalidade = new ModalidadeEnsino();
                                        foreach ($modalidade->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($moeid) && in_array($key['codigo'], $moeid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Última atualizao:</td>
                                <td class="tdInputForm">
                                    <?php
                                    $ultatualizacao = $_POST["ultatualizacao"];
                                    $arSel = array(
                                        array("codigo" => 1, "descricao" => "Em at 25 dias"),
                                        array("codigo" => 2, "descricao" => "Entre 25 e 30 dias"),
                                        array("codigo" => 3, "descricao" => "Mais de 30 dias"),
                                    );
                                    $db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "200", "N", "ultatualizacao");
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Tipo Vistoria (Realizado por):
                                </td>
                                <td class="tdInputForm">
                                    <select name="rsuid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php
                                        $realizacaosupervisao = new RealizacaoSupervisao();
                                        foreach ($realizacaosupervisao->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($rsuid) && in_array($key['codigo'], $rsuid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Valor da Obra:</td>
                                <td class="tdInputForm">
                                    De:&nbsp;
                                    <?php
                                    echo campo_texto('obrvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                    ?>
                                    At:&nbsp;
                                    <?php
                                    echo campo_texto('obrvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>

<!--                    Coluna 2-->
                    <td width="50%">
                        <table width="100%">
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Municpios(s):</td>
                                <td class="td_municipio tdInputForm">
                                        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                                            <?php   $municipio = new Municipio();
                                            foreach ($municipio->listaComboMulti() as $key) {
                                                ?>
                                                <option
                                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                            <?php } ?>
                                        </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Unidade:</td>
                                <td id="td_unidade" class="tdInputForm">
                                    <?php if(!empty($muncod)){
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
                                    <?php } ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui foto:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="totalfoto" id=""
                                           value="S" <?= ($_POST["totalfoto"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="totalfoto" id=""
                                           value="N" <?= ($_POST["totalfoto"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="totalfoto" id=""
                                           value=""  <?= ($_POST["totalfoto"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui vistoria:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="possui_vistoria" id=""
                                           value="S" <?= ($_POST["possui_vistoria"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="possui_vistoria" id=""
                                           value="N" <?= ($_POST["possui_vistoria"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="possui_vistoria" id=""
                                           value=""  <?= ($_POST["possui_vistoria"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui restrição:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="res_estado" id=""
                                           value="S" <?= ($_POST["res_estado"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="res_estado" id=""
                                           value="N" <?= ($_POST["res_estado"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="res_estado" id=""
                                           value=""  <?= ($_POST["res_estado"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Restrição superada:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="res_superada" id=""
                                           value="S" <?= ($_POST["res_superada"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="res_superada" id=""
                                           value="N" <?= ($_POST["res_superada"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="res_superada" id=""
                                           value=""  <?= ($_POST["res_superada"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui Aditivo:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="ocraditivado" id=""
                                           value="S" <?= ($_POST["ocraditivado"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="ocraditivado" id=""
                                           value="N" <?= ($_POST["ocraditivado"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="ocraditivado" id=""
                                           value=""  <?= ($_POST["ocraditivado"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui Superviso por O.S:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="responsavel" id=""
                                           value="S" <?= ($_POST["responsavel"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="responsavel" id=""
                                           value="N" <?= ($_POST["responsavel"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="responsavel" id=""
                                           value=""  <?= ($_POST["responsavel"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Listar as Obras Vinculantes?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="obrvinculada" id=""
                                           value="S" <?= ($_POST["obrvinculada"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="obrvinculada" id=""
                                           value="N" <?= ($_POST["obrvinculada"] == "N" || $_POST["obrvinculada"] == "" ? "checked='checked'" : "") ?>/>
                                    No
                                    <input type="radio" name="obrvinculada" id=""
                                           value="T"  <?= ($_POST["obrvinculada"] == "T" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Recebeu o primeiro repasse?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="repasse" id=""
                                           value="S" <?= ($_POST["repasse"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="repasse" id=""
                                           value="N" <?= ($_POST["repasse"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="repasse" id=""
                                           value="T"  <?= ($_POST["repasse"] == "T" || $_POST["repasse"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui checklist administrativo?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="chk_adm" id=""
                                           value="S" <?= ($_POST["chk_adm"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="chk_adm" id=""
                                           value="N" <?= ($_POST["chk_adm"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="chk_adm" id=""
                                           value="T"  <?= ($_POST["chk_adm"] == "T" || $_POST["chk_adm"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui checklist 2 Parcela?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="chk_2pc" id=""
                                           value="S" <?= ($_POST["chk_2pc"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="chk_2pc" id=""
                                           value="N" <?= ($_POST["chk_2pc"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="chk_2pc" id=""
                                           value="T"  <?= ($_POST["chk_2pc"] == "T" || $_POST["chk_2pc"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">% Executado da Obra:</td>
                                <td class="tdInputForm">
                                    <div style="margin: 5px;">
                                        <label id="txtPercentualinicial"></label>
                                        &nbsp;&nbsp;
                                        <label id="txtPercentualfinal"></label>
                                        <input type="hidden" id="percentualfinal" name="percentualfinal" readonly>
                                        <input type="hidden" id="percentualinicial" name="percentualinicial" readonly>
                                        <div style="width: 200px;">
                                        <div id="slider-range"></div>
                                        </div>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">% Executado da Obra:</td>
                                <td class="tdInputForm">
                                    <table>
                                        <tr>
                                            <th>Mnimo</th>
                                            <th>Mximo</th>
                                        </tr>
                                        <tr>
                                            <?php
                                            for ($i = 0; $i <= 100; $i++) {
                                                $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                                            }
                                            $percentualinicial = $_POST['percentualinicial'];
                                            $percentualfinal = $_POST['percentualfinal'];
                                            $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                                            print '<td>';
                                            $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                                            print '</td><td>';
                                            $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                                            print '</td>';
                                            ?>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui Marcador de rgo de Controle?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="chk_org_controle" id=""
                                           value="S" <?= ($_POST["chk_org_controle"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="chk_org_controle" id=""
                                           value="N" <?= ($_POST["chk_org_controle"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="chk_org_controle" id=""
                                           value="T"  <?= ($_POST["chk_org_controle"] == "T" || $_POST["chk_org_controle"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Funcionamento:</td>
                                <td class="tdInputForm">
                                    <table>
                                        <tr>
                                            <td>
                                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 1) ? 'checked' : ''; ?>
                                                       name="funcionamentoflag" id="moefuncionamentoflag_s" value="1"/>
                                                Em Funcionamento:
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 2) ? 'checked' : ''; ?>
                                                       name="funcionamentoflag" id="moefuncionamentoflag_pi" value="2"/>
                                                Previso de Inaugurao:
                                                <?php echo campo_data2('moedtprevinauguracao_i', 'S', true, '', null, '', ' ', '', ''); ?>
                                                &nbsp;at&nbsp;
                                                <?php echo campo_data2('moedtprevinauguracao_f', 'S', true, '', null, '', ' ', '', ''); ?>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 3) ? 'checked' : ''; ?>
                                                       name="funcionamentoflag" id="moefuncionamentoflag_if" value="3"/>
                                                Previso de Início de Funcionamento:
                                                <?php echo campo_data2('moedtpreviniciofunc_i', 'S', true, '', null, '', ' ', '', ''); ?>
                                                &nbsp;at&nbsp;
                                                <?php echo campo_data2('moedtpreviniciofunc_f', 'S', true, '', null, '', ' ', '', ''); ?>
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Mobilirio Empenhado:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="moeempenhadoflag" id=""
                                           value="S" <?= ($_POST["moeempenhadoflag"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="moeempenhadoflag" id=""
                                           value="N" <?= ($_POST["moeempenhadoflag"] == "N" ? "checked='checked'" : "") ?>/> No
                                    <input type="radio" name="moeempenhadoflag" id=""
                                           value=""  <?= ($_POST["moeempenhadoflag"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
                <br><br><br>


<!-- Fim tablela novo layout               -->



            </td>
        </tr>
        <tr>
            <td class="divTituloPadrao" width="15%" colspan="2" align="center">
                <?php
                if (possui_perfil(PFLCOD_SUPER_USUARIO)):
                    ?>
                    <div style="float:left" class="novaObra">
                        <a style="cursor: pointer;">
                            <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Nova obra"> Nova Obra
                        </a>
                    </div>
                <?php endif; ?>
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
            </td>
        </tr>
    </table>
</form>
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

        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', "formulario", '', '', $cache);

    }
} else {
    ?>
    <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
        <tr>
            <td style="text-align: justify;">
                Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for
                escolhido sero apresentados todos os registros que voc pode acessar.
            </td>
        </tr>
    </table>
    <?php
}
?>

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


<?php
function listaSql(Array $param = array())
{
    $coluns = array();
    $join = array();
    $where = array();

    if ($param['obrbuscatexto']) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($param['obrbuscatexto']))));
        $obrbuscatextoTemp = trim($obrbuscatextoTemp);

        if (!strpos($obrbuscatextoTemp, ',')) {
            $param['obrbuscatexto'] = limpaObridSec(trim($param['obrbuscatexto']));
            $where['obrnome'] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%') OR
                        o.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%')) ";
        } else {
            $campos = explode(',', $obrbuscatextoTemp);
            $w = array();
            foreach ($campos as $c) {
                $c = trim($c);
                $d = limpaObridSec(trim($c));
                $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') OR 
                        o.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $d . "%')) ";
            }

            $w = '(' . implode('OR', $w) . ')';
            $where['obrnome'] = $w;
        }

    }

    if ($param['empid'])
        $where['empid'] = "o.empid IN(" . $param['empid'] . ")";


    if ($param['strid'] && $param['strid'][0] != '')
        $where['strid'] = "str.strid IN(" . implode(',', $param['strid']) . ")";
    if ($param['tobid'] && $param['tobid'][0] != '')
        $where['tobid'] = "o.tobid IN(" . implode(',', $param['tobid']) . ")";
    if ($param['tooid'] && $param['tooid'][0] != '')
        $where['tooid'] = "o.tooid IN(" . implode(',', $param['tooid']) . ")";
    if ($param['cloid'] && $param['cloid'][0] != '')
        $where['cloid'] = "o.cloid IN(" . implode(',', $param['cloid']) . ")";
    if ($param['prfid'] && $param['prfid'][0] != '')
        $where['prfid'] = "e.prfid IN(" . implode(',', $param['prfid']) . ")";
    if ($param['moeid'] && $param['moeid'][0] != '')
        $where['moeid'] = "e.moeid IN(" . implode(',', $param['moeid']) . ")";
    if ($param['entid'])
        $where['entid'] = "o.entid IN(" . implode(',', $param['entid']) . ")";
    if ($param['ultatualizacao']) {
        switch ($param['ultatualizacao']) {
            case 1:
                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 25";
                break;
            case 2:
                $where['ultatualizacao'] = "( DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 25 AND
                                  DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 30 )";
                break;
            case 3:
                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 30";
                break;
        }
    }

    if ($param['tpoid'] && $param['tpoid'][0] != '')
        $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $param['tpoid']) . "')";
//    if ($param['estuf'] && $param['estuf'][0] != '')
        $where['estuf'] = "mun.estuf IN ('SP')";
    if ($param['muncod'] && $param['muncod'][0] != '')
        $where['muncod'] = "mun.muncod IN ('" . implode("', '", $param['muncod']) . "')";
    if ($param['empesfera'])
        $where['empesfera'] = "e.empesfera IN('" . $param['empesfera'] . "')";
    if ($param['processo'])
        $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')";
    if ($param['ano_processo'])
        $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'";
    if ($param['convenio'])
        $where['convenio'] = "p_conv.termo_convenio = '{$param['convenio']}'";
    if ($param['ano_convenio'])
        $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$param['ano_convenio']}'";
    if ($param['totalfoto'] == 'S')
        $where['totalfoto'] = "obrsnfotos = true";
    elseif ($param['totalfoto'] == 'N')
        $where['totalfoto'] = "obrsnfotos = false OR obrsnfotos IS NULL";
    if ($param['obrami'] == 'S')
        $where['obrami'] = "o.tpoid IN (104, 105)";
    elseif ($param['obrami'] == 'N')
        $where['obrami'] = "o.tpoid NOT IN (104, 105)";
    if ($param['possui_vistoria'] == 'S')
        $where['possui_vistoria'] = "obrdtultvistoria IS NOT NULL";
    elseif ($param['possui_vistoria'] == 'N')
        $where['possui_vistoria'] = "o.obrdtultvistoria IS NULL";
    if ($param['res_estado'] == 'S')
        $where['res_estado'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0";
    elseif ($param['res_estado'] == 'N')
        $where['res_estado'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) = 0";
    if ($param['res_superada'] == 'S')
        $where['res_superada'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0";
    elseif ($param['res_superada'] == 'N')
        $where['res_superad'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) = 0";
    if ($param['ocraditivado'] == 'S')
        $where['ocraditivado'] = "c.ttaid > 0";
    elseif ($param['ocraditivado'] == 'N')
        $where['ocraditivado'] = " (c.ttaid IS NULL OR c.ttaid=0) ";
    if ($param['responsavel'] == 'S')
        $where['responsavel'] = "e.empdtultvistoriaempresa IS NOT NULL";
    elseif ($param['responsavel'] == 'N')
        $where['responsavel'] = "e.empdtultvistoriaempresa IS NULL";
    if ($param['evomi'] == 'S') {
        $where['evmi'] = "evmi.emidtinclusao IS NOT NULL";
        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
    } elseif ($param['evomi'] == 'N') {
        $where['evmi'] = "evmi.emidtinclusao IS NULL";
        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
    }
    switch ($param['obrvinculada']) {
        case 'S':
            $where['obrvinculada'] = " o.obrstatus = 'P' ";
            break;
        case 'N':
            $where['obrvinculada'] = " o.obrstatus = 'A' ";
            break;
        case 'T':
            $where['obrvinculada'] = " o.obrstatus IN ('A', 'P') ";
            break;
        default:
            $where['obrvinculada'] = " o.obrstatus = 'A' ";
            break;
    }
    if ($param['repasse'] == 'S') {
        $where['repasse'] = "(o.obrid IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')
                                        OR
                                        EXISTS(
                                        select 1 from par3.processoobracomposicao p
                                        JOIN par3.empenhoobracomposicao e ON p.pocid = e.pocid AND eocstatus = 'A'
                                        JOIN par3.pagamentoobracomposicao c ON c.eocid = e.eocid AND pmcstatus = 'A'
                                        WHERE p.pocstatus = 'A' AND p.obrid = o.obrid_par3))";
    } elseif ($param['repasse'] == 'N') {
        $where['repasse'] = "(o.obrid NOT IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')
                                        OR
                                        NOT EXISTS(
                                        select 1 from par3.processoobracomposicao p
                                        JOIN par3.empenhoobracomposicao e ON p.pocid = e.pocid AND eocstatus = 'A'
                                        JOIN par3.pagamentoobracomposicao c ON c.eocid = e.eocid AND pmcstatus = 'A'
                                        WHERE p.pocstatus = 'A' AND p.obrid = o.obrid_par3))";
    }
    if ($param['medidasexcecao'] == 'S')
        $where['medidasexcecao'] = "medidasexcecao = TRUE ";
    elseif ($param['medidasexcecao'] == 'N')
        $where['medidasexcecao'] = "medidasexcecao = FALSE ";
    if ($param['fiscal_ativo'] == 'S')
        $where['fiscal_ativo'] = "e.empid IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";
    elseif ($param['fiscal_ativo'] == 'N')
        $where['fiscal_ativo'] = " e.empid NOT IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";


    if ($param['posvigencia'] == 'S')
        $where['posvigencia'] = "o.obrid IN (select distinct
                                obrid
                                from obras2.supervisao sup where posvigencia = 't' and supstatus = 'A')";
    elseif ($param['posvigencia'] == 'N')
        $where['posvigencia'] = "o.obrid IN (select  distinct
                                sup.obrid
                                from obras2.supervisao sup where posvigencia = 'f' and supstatus = 'A')";

    if ($param['mesid'] == 'S')
        $where['mesid'] = "o.obrid IN (SELECT DISTINCT m.obrid FROM obras2.monitoramento_especial m WHERE m.messtatus = 'A')";
    elseif ($param['mesid'] == 'N')
        $where['mesid'] = "o.obrid IN (SELECT DISTINCT m.obrid FROM obras2.monitoramento_especial m WHERE m.messtatus = 'A')";


    $ck_sql = "SELECT
                        ckf.obrid
                    FROM obras2.checklistfnde                    ckf
                    INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                    WHERE ckf.ckfstatus = 'A' AND que.queid = %s";
    if ($param['chk_adm'] == 'S')
        $where['chk_adm'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM) . ")";
    elseif ($param['chk_adm'] == 'N')
        $where['chk_adm'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM) . ")";
    if ($param['chk_2pc'] == 'S')
        $where['chk_2pc'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_2P) . ")";
    elseif ($param['chk_2pc'] == 'N')
        $where['chk_2pc'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_2P) . ")";
    if ($param['chk_tec'] == 'S')
        $where['chk_tec'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC) . ")";
    elseif ($param['chk_tec'] == 'N')
        $where['chk_tec'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC) . ")";
    if ($param['chk_vinc'] == 'S')
        $where['chk_vinc'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC) . ")";
    elseif ($param['chk_vinc'] == 'N')
        $where['chk_vinc'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC) . ")";

    if ($param['chk_org_controle'] == 'S')
        $where['chk_org_controle'] = " o.obrorgcontrole = 't' ";
    elseif ($param['chk_org_controle'] == 'N')
        $where['chk_org_controle'] = " o.obrorgcontrole = 'f'";

    if ($param['chk_conta_bloqueada'] == 'S')
        $where['chk_conta_bloqueada'] = " o.obrcontabloqueada = 't'";
    elseif ($param['chk_conta_bloqueada'] == 'N')
        $where['chk_conta_bloqueada'] = " o.obrcontabloqueada = 'f'";

    if ($param['chk_obrprocessoanterior'] == 'S')
        $where['chk_obrprocessoanterior'] = " o.obrprocessoanterior = 't'";
    elseif ($param['chk_obrprocessoanterior'] == 'N')
        $where['chk_cobrprocessoanterior'] = " o.obrprocessoanterior = 'f'";

    if ($param['stiid'] && $param['stiid'][0] != '')
        $where['stiid'] = "o.stiid IN (" . implode(', ', $param['stiid']) . ")";

    if ($param['esdid'] && $param['esdid'][0] != '')
        $where['esdid'] = "d.esdid IN (" . implode(', ', $param['esdid']) . ")";

    if ($param['rsuid'] && $param['rsuid'][0] != '')
        $where['rsuid'] = "rsuid IN (" . implode(', ', $param['rsuid']) . ")";

    if ($param['obrvalorprevisto_menor'])
        $where['obrvalorprevisto_menor'] = "o.obrvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_menor']);
    if ($param['obrvalorprevisto_maior'])
        $where['obrvalorprevisto_maior'] = "o.obrvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_maior']);
    switch ($param['nivelpreenchimento']) {
        /* 1 - Verde    -> x < 25              */
        /* 2 - Amarelo  -> 25 > x <= 30         */
        /* 3 - Vermelho -> x > 30              */
        case 1 : //Verde
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) <= 25 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) <= 25 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END  ";
            break;
        case 2 :
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( (DATE_PART('days', NOW() - o.obrdtultvistoria ) > 25) AND DATE_PART('days', NOW() - o.obrdtultvistoria ) <= 30 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( (DATE_PART('days', NOW() - sup.supdata  ) > 25) AND DATE_PART('days', NOW() - sup.supdata ) <= 30  )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END";
            break;
        case 3 :
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) > 30 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) > 30 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END  ";
            break;
    }
    if ($param['nivelpreenchimento'] || $param['rsuid'])
        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";

    if ($param['percentualinicial'] != '')
        $where['percentualinicial'] = "COALESCE(o.obrpercentultvistoria, 0) >= " . $param['percentualinicial'];
    if ($param['percentualfinal'] != '')
        if ($param['percentualfinal'] < 100)
            $where['percentualfinal'] = "COALESCE(o.obrpercentultvistoria, 0) <= " . $param['percentualfinal'];
    if ($param['funcionamentoflag']) {
        switch ($param['funcionamentoflag']) {
            case 1: // em funcionamento
                $queryComplement = 'AND moedtiniciofunc < NOW()';
                break;
            case 2: // previso de inaugurao
                $queryComplement = 'AND moedtprevinauguracao BETWEEN \'' . ajusta_data($param['moedtprevinauguracao_i']) . '\' AND \'' . ajusta_data($param['moedtprevinauguracao_f']) . '\'';
                break;
            case 3: //previso inicio de funcionamento
                $queryComplement = 'AND moedtpreviniciofunc BETWEEN \'' . ajusta_data($param['moedtpreviniciofunc_i']) . '\' AND \'' . ajusta_data($param['moedtpreviniciofunc_f']) . '\'';
                break;
        }

        $where['funcionamentoflag'] = sprintf("CASE WHEN (SELECT
                            CASE WHEN moeid IS NOT NULL THEN TRUE ELSE FALSE END
                            FROM obras2.mobiliarioempenhado
                            WHERE obrid = o.obrid %s
                            ORDER BY moeid DESC
                            LIMIT 1) THEN true
                         ELSE false END", $queryComplement);
    }

    $sqlCondEmp = "
                    SELECT
                           sov.obrid as obra
                           FROM par.subacaoobravinculacao sov
                                   INNER JOIN par.v_saldo_empenho_por_subacao es on es.sbaid = sov.sbaid and es.eobano = sov.sovano
                                  INNER JOIN par.subacao sba ON sba.sbaid = sov.sbaid
                                   INNER JOIN par.propostatiposubacao pts ON pts.ptsid = sba.ptsid
                           WHERE
                           pts.ptsdescricao IN ('EQUIPAMENTOS', 'EQUIPAMENTOS (BNDES)', 'MOBILIRIO', 'MOBILIRIO (BNDES)')
                           AND es.saldo > 0
                    UNION ALL
                    -- Carga de Mobilirio Empenhado por Convênio Ativo
                    SELECT DISTINCT moe.obrid as obra
                        FROM obras2.mobiliarioempenhado moe
                        WHERE moe.moestatus = 'A'
                        AND moe.moeempenhadoflag = 'S'
                        
                    UNION ALL
                    SELECT inp.obrid FROM par3.iniciativa_planejamento inp
                                  INNER JOIN par3.analise ana ON ana.inpid = inp.inpid
                                 INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ppc.anaid = ana.anaid
                                 INNER JOIN par3.processo pro ON pro.proid = ppc.proid
                                 INNER JOIN par3.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
                                 WHERE empstatus = 'A'
                                 AND inp.obrid IS NOT NULL
                    -- Empenho pelo par 3
                    
    ";

    if ($param['moeempenhadoflag'] == 'S') {
        $where['moeempenhadoflag'] = "o.obrid IN ($sqlCondEmp)";
    } elseif ($param['moeempenhadoflag'] == 'N') {
        $where['moeempenhadoflag'] = "o.obrid NOT IN ($sqlCondEmp)";
    }
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR))) {

        $arrObr = array(
            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
            PFLCOD_EMPRESA_VISTORIADORA_GESTOR
        );

        $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = Array(PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC);

        $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

        $resp = array();
        $arPflcod = array();
        $orWhere = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
        }

        if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
            $usuarioResp = new UsuarioResponsabilidade();
            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
        }

    }

    $imgNovoObj = '';

    //string que monta a concatenao do campo obrid + idsecretaria para ser montada na query
    $campoObridSec = montaCampoObridSec('o');

    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
        $imgNovoObj = "' ||
                                    CASE WHEN ed.esdid = " . ESDID_OBJ_PARALISADO . " AND o.obrstatus != 'P' THEN
                                        '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/editar_nome.gif\"
                                                style=\"cursor: pointer\"
                                                onclick=\"javascript: novaObrVinculada(' || o.obrid || ');\"
                                                title=\"Cadastrar nova obra vinculada\">'
                                    ELSE
                                            '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                                    END || '";
    }

    $coluns['acao'] = "'<center><div style=\"width:100px\">
                    {$imgNovoObj}
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/edit_on.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: abreListaSupervisaoFnde(' || o.obrid || ',' || o.empid || ');\"
                            title=\"Superviso FNDE\" />
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/seriehistorica_ativa.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: abreEvolucaoFinan(' || o.obrid || ');\"
                            title=\"Evoluo Financeira\" />
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(' || o.obrid || ');\"
                            title=\"Alterar Obra\" />
              </div></center>' AS acao";

    $coluns['r'] = "
                        CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0 THEN
                            '<img title=\"Restrições pendentes\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_vermelho.png\">'
                        ELSE '<img title=\"Restrições superadas\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_verde.png\">' END as r ";
    $coluns['i'] = "
                         CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0 THEN
                            '<img title=\"Restrições pendentes\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_vermelho.png\">'
                        ELSE '<img title=\"Restrições superadas\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_verde.png\">' END as i";
    $coluns['vinculada'] = "
                        CASE WHEN o.obridvinculado IS NOT NULL THEN
                            '<img title=\"Com obra vinculada\" src=\"/imagens/check_p.gif\"/>'
                        ELSE
                            ''
                        END AS vinculada";


    $coluns['excel_r'] = "CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NO' END as excel_r";

    $coluns['excel_i'] = "CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NO' END as excel_i";

    $coluns['excel_vinculada'] = "
                        CASE WHEN o.obridvinculado IS NOT NULL THEN
                            'SIM'
                        ELSE
                            'NO'
                        END AS vinculada";


    $coluns['obrid'] = $campoObridSec . "as obrid";
    $coluns['preid'] = "COALESCE(o.preid,o.obrid_par3) as preid";
    $coluns['pronumeroprocesso'] = (verificaTecnico()) ? 'p_conv.pronumeroprocesso' : "'<span class=\"processo_detalhe\">'||p_conv.pronumeroprocesso||'</span>'";
    $coluns['excel_pronumeroprocesso'] = "p_conv.pronumeroprocesso";
    $coluns['anoprocesso'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso";
    $coluns['termo_convenio'] = "p_conv.termo_convenio";
    $coluns['ano_termo_convenio'] = "p_conv.ano_termo_convenio";
    $coluns['obrnome'] = "
                        CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                            '<b style=\"color:green\">(EP) </b>'
                        ELSE
                            ''
                        END || '<a href=\"javascript: alterarObr(' || o.obrid || ');\">(' ||".$campoObridSec." || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['excel_obrnome'] = "'(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['ocrdtordemservico'] = "TO_CHAR(oc.ocrdtordemservico, 'dd/mm/YYYY') AS ocrdtordemservico";
    $coluns['mesdsc'] = "meso.mesdsc";
    $coluns['entnome'] = "uni.entnome";
    $coluns['mundescricao'] = "mun.mundescricao";
    $coluns['estuf'] = "mun.estuf";
    $coluns['excel_endereco'] = "trim(edr.endlog) ||', '||  trim(edr.endbai) ||', '|| trim(mun.mundescricao) ||' - '|| trim(mun.estuf) ||' CEP: '|| trim(edr.endcep) as endereco";
    $coluns['inicio'] = "CASE WHEN o.tpoid IN (104, 105) THEN (SELECT TO_CHAR(osmdtinicio, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1) ELSE TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') END AS inicio";
    $coluns['termino'] = "CASE WHEN o.tpoid IN (104, 105) THEN (SELECT TO_CHAR(osmdttermino, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1) ELSE TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') END AS termino";
    $coluns['strdsc'] = "
                    CASE
                        WHEN str.strid = " . STRID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || str.strdsc || '</font>'
                        ELSE str.strdsc
                    END AS situacao
                    ";
    $coluns['excel_strdsc'] = "str.strdsc AS situacao";
    $coluns['percentual_execucao'] = "CASE WHEN
            ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0)) > '99.95' 
        THEN
        ROUND(  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) ) || '%'
        ELSE((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%'
        
        END AS percentual_execucao";
    $coluns['tpldsc'] = "sup.tpldsc";
    $coluns['ultima_atualizacao'] = "
                    -- Obra concluida aplicar cor azul
                    CASE WHEN ed.esdid IN (693) THEN
                        '<span style=\"color:#3276B1\">' || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</span>'
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em at 25 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '</span>'
                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em at 25 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) > 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )' || '</span>'
                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em at 25 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) > 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char((SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))||' dia(s) )' || '</span>'
                    ELSE
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada em at 25 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '</span>'
                    END as ultima_atualizacao";
    $coluns['excel_ultima_atualizacao'] = "to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao";
    $coluns['qtddias'] = " DATE_PART( 'days', NOW() - o.obrdtultvistoria )::text AS qtddias";
    $coluns['obrpercentultvistoria'] = "CASE WHEN
            o.obrpercentultvistoria > '99.95' 
        THEN
        ROUND(  o.obrpercentultvistoria )  || '%'
        ELSE o.obrpercentultvistoria || '%'
        
        END AS obrpercentultvistoria";
    $coluns['empdtultvistoriaempresa'] = "to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa";
    $coluns['emppercentultvistoriaempresa'] = "e.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa";
    $coluns['programa'] = "pf.prfdesc programa";
    $coluns['fonte'] = "too.toodescricao fonte";
    $coluns['esfera'] = "
                    CASE
                        WHEN e.empesfera = 'M' THEN 'Municipal'
                        WHEN e.empesfera = 'E' THEN 'Estadual'
                        WHEN e.empesfera = 'F' THEN 'Federal'
                    ELSE ''
                    END as esfera";
    $coluns['tpodsc'] = "tpo.tpodsc";
    $coluns['ocrvalorexecucao'] = "ocrvalorexecucao";
    $coluns['obrvalorprevisto'] = "o.obrvalorprevisto";


    $coluns['excel_valor_pactuado'] = "v_fim_obr.valor_pactuado";
    $coluns['excel_valor_empenhado'] = "
                                         CASE WHEN peo.valorempenho NOTNULL THEN 
                                               peo.valorempenho
                                             ELSE
                                               p3eo.vlrempenho
                                             END as valor_empenhado ";
    $coluns['excel_valor_receber'] = "(valor_pactuado - valor_repassado) as valor_receber";
    $coluns['excel_saldo_conta'] = "v_fim_obr.saldo";
    $coluns['excel_nr_inep'] = "uni.entcodent";




    if ($param['excel']) {
        $join['mesoregiao'] = "LEFT JOIN territorios.mesoregiao meso ON meso.mescod = mun.mescod";
        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";
        $join['v_financeiro_obras'] = "LEFT JOIN obras2.v_financeiro_obras v_fim_obr  ON v_fim_obr.obrid = o.obrid";
        $join['par_v_saldo_empenho_por_obra'] = "LEFT JOIN par.v_saldo_empenho_por_obra as peo ON peo.obrid = o.obrid";
        $join['par3_v_saldo_empenho_por_obra'] = "LEFT JOIN par3.v_saldo_empenho_por_obra as p3eo ON p3eo.obrid = o.obrid_par3";


        unset($coluns['r']);
        unset($coluns['i']);
        unset($coluns['vinculada']);
        unset($coluns['acao']);
        unset($coluns['obrnome']);
        unset($coluns['ultima_atualizacao']);
        unset($coluns['strdsc']);
        unset($coluns['pronumeroprocesso']);
    } else {
        unset($coluns['ocrdtordemservico']);
        unset($coluns['tipoparalisacao']);
        unset($coluns['mesoregiao']);
        unset($coluns['programa']);
        unset($coluns['fonte']);
        unset($coluns['esfera']);
        unset($coluns['termo']);
        unset($coluns['situacao']);
        unset($coluns['qtddias']);
        unset($coluns['processoobra']);
        unset($coluns['anoprocesso']);
        unset($coluns['qtddias']);
        unset($coluns['mesdsc']);
        unset($coluns['tpldsc']);
        unset($coluns['obrvalorprevisto']);
        unset($coluns['excel_r']);
        unset($coluns['excel_pronumeroprocesso']);
//        unset($coluns['excel_r_pendentes']);
//        unset($coluns['excel_r_atraso']);
        unset($coluns['excel_i']);
//        unset($coluns['excel_i_pendentes']);
//        unset($coluns['excel_i_atraso']);
        unset($coluns['excel_vinculada']);
        unset($coluns['excel_obrnome']);
        unset($coluns['excel_ultima_atualizacao']);
        unset($coluns['excel_strdsc']);
        unset($coluns['excel_endereco']);
        unset($coluns['excel_valor_pactuado']);
        unset($coluns['excel_valor_empenhado']);
        unset($coluns['excel_valor_receber']);
        unset($coluns['excel_saldo_conta']);
        unset($coluns['excel_nr_inep']);

    }


    if (verificaTecnico()) {
        unset($coluns['percentual_execucao']);
    }

    $orWhere = (is_array($orWhere) ? $orWhere : array());
    $sqlBase = "
        WITH  usuarioresponsabilidade AS (
            SELECT entid, pflcod, estuf, orgid, empid FROM obras2.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'
        )
        SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    " . (count($coluns) ? implode(", ", $coluns) : "") . "
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade

                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.v_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                    o.obridpai IS NULL
                    " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
            ) as f
            ORDER BY f.obrid ASC
    ";
//ver($sqlBase, d);
    return $sqlBase;
}

function getCabecalho($xls = false)
{
    $cabecalho = array('Restrições', 'Inconformidades', 'Vinculada', 'ID', 'ID Pr-Obra', 'N Processo', 'Ano Processo', 'Termo/N Convênio', 'Ano Termo/Convênio', 'Obra', 'Data da Ordem de Serviço', 'Mesoregio', 'Unidade Implantadora', 'Município', 'UF','Endereo da Obra' ,'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', '% Executado Instituição Acumulado', 'Tipo de Paralisao', 'Última Vistoria Instituição', 'Qtd Dias Última Vistoria', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Programa', 'Fonte', 'Esfera', 'Tipologia', 'Valor Contrato', 'Valor Previsto','Valores Pactuados','Valores Empenhados','Valores a Receber','Saldo em Conta','N do INEP');

    if (!$xls)
        $cabecalho = array('Ao', 'R', 'I', 'V', 'ID', 'ID Pr-Obra', 'N Processo', 'N Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', '% Executado Instituição Acumulado', 'Última Vistoria Instituição', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');

    if (verificaTecnico()) {
        $cabecalho = array('Restrições', 'Inconformidades', 'Vinculada', 'ID', 'ID Pr-Obra', 'N Processo', 'Ano Processo', 'Termo/N Convênio', 'Ano Termo/Convênio', 'Obra', 'Data da Ordem de Serviço', 'Mesoregio', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', 'Tipo de Paralisao', 'Última Vistoria Instituição', 'Qtd Dias Última Vistoria', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Programa', 'Fonte', 'Esfera', 'Tipologia', 'Valor Contrato', 'Valor Previsto');

        if (!$xls)
            $cabecalho = array('Ao', 'R', 'I', 'V', 'ID', 'ID Pr-Obra', 'N Processo', 'N Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', 'Última Vistoria Instituição', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');

    }

    return $cabecalho;
}

function verificaTecnico()
{
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_CGIMP_GESTOR, PFLCOD_CGIMP_TECNICO, PFLCOD_CALL_CENTER, PFLCOD_ADMINISTRADOR))) {
        return false;
    } else {
        return true;
    }
}

?>

<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }

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


        // $1_11(".estados").chosen().change(function (e, params) {
        //     values = $1_11(".estados").chosen().val();
        //     carregarMunicipio(values);
        // });

        $1_11(".td_municipio").on( "change", ".municipios",function() {
            carregarUnidade($1_11(this).chosen().val());
        });

    });

    number_format = function(){
        return 9999;
    }
</script>
