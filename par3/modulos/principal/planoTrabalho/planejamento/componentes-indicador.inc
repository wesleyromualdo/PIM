<?php
$oIndicador = new Par3_Model_Indicador();
$oIndicadorMeta = new Par3_Model_IndicadorMeta();
$oAreaController = new Par3_Controller_Area();
$oPontuacaoController = new Par3_Controller_Pontuacao();
$oPontuacao = new Par3_Model_Pontuacao();
$oPontuacaoMeta = new Par3_Model_PontuacaoMeta();
$iniid = $_REQUEST['iniid'] ? '&iniid='.$_REQUEST['iniid']:'';
$arrIni = array();
$arrIni['iniid'] = (isset($_GET['iniid'])) ? $_GET['iniid'] : $_SESSION['iniid'];
$arrIni['dimid'] = (isset($_GET['dimid'])) ? $_GET['dimid'] : $_SESSION['dimid'] ;
$mIidi = new Par3_Model_IniciativaIndicadores();
$arAreas = $mIidi->recuperarIniciativaAreas($arrIni);
if(empty($arAreas)){//Caso não haja área Selecionada são carregadas todas as áreas do diagnóstico
    $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($arrIni['dimid']);
}
$arrAre = array();
$abasGuias = array();
$dimid = $arrIni['dimid'];
$dimensao = new Par3_Model_Dimensao($dimid);

$situacao = recuperaSituacaoPreenchimentoDiagnostico($_REQUEST['inuid']);

//if (is_array($arDimensoes) && (!$existePendenciaGeral || $_SESSION['visualizar-sintese-diagnostico']) && !isset($_GET['resumo'])) {

$ativa = 'active';
$d = 1;

?>
<script language="javascript" src="/estrutura/js/funcoes.js"></script>
<script language="javascript" src="/par/js/javascript.js"></script>
<script src="../includes/funcoes.js"></script>
<style>
    .div_lbl_grfAno {
        float: left;
        margin-top: 11px;
    <?php if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) { ?> margin-left: 16px;
    <?php } else { ?> margin-left: 16px;
    <?php } ?>
    }

    .div_combo_grfAno {
        float: left;
        margin-top: 5px;
    <?php if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) { ?> margin-left: 15px;
    <?php } else { ?> margin-left: 15px;
    <?php } ?> width: 259px;
    }

    .div_btn_NaoInfo {
        margin-left: 10px;
        margin-top: -20px;
    }

    .ui-slider-range-min {
        background-color: #f7b850;
    }

    /*.tabela_box_azul_escuro tr td {*/
        /*margin: 20px;*/
        /*padding: 0px 10px 0px 10px;*/
    /*}*/

    .div_grfMun {
        float: left;
        margin-right: 15px;
    }

    .metanaoinformada {
        margin: 0px -10px;
    }

    table.tabela_painel {
        padding: 19px;
        margin-top: -20px;
        background-color: #f5f5f5;
        border: 1px solid #e3e3e3;
        border-radius: 4px;
        -webkit-box-shadow: inset 0 1px 1px rgba(0, 0, 0, .05);
        box-shadow: inset 0 1px 1px rgba(0, 0, 0, .05);
    }

    .titulo-grfpne {
        background-color: #f3f3f4;
    }

    .btn-primary-active {
        background-color: #808080;
        border-color: #808080;
        color: #FFFFFF;
    }

    <?php
    if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) {
        ?>
    .ui-slider-range {
        background-color: #0101CD !important;
    }

    <?php } else { ?>
    .ui-slider-range {
        background-color: #B8860B !important;
    }

    <?php } ?>
    .ui-state-default, .ui-widget-content .ui-state-default, .ui-widget-header .ui-state-default {
        background: inherit;
        border: 1px solid #B1AFAF !important;
        border-radius: 10px;
    }

    @media print {
        body * {
            visibility: hidden ;
        }
        #print_area, #print_area * {
            visibility: visible;
        }
        #print_area {
            position: absolute;
            left: 0;
            top: 0;
        }
        .noprint {display:none !important;}
    }
</style>
<style>
    .colunaIndicadores {
        background: #f3f3f4;
        margin-top: -20px;
        margin-left: -6px;
        padding: 20px;
        height: 100%;
    }

    .btn-circled, .btn-circled:hover, .btn-circled:visited, .btn-circled:active {
        background-color: #347AB8;
        font-size: 30px;
        font-weight: bold;
        width: 100px;
        height: 100px;
        border-radius: 50%;
        -webkit-border-radius: 50%;
        -moz-border-radius: 50%;
        cursor: auto;
    }

    .pontuacao {
        font-size: 20px;
        font-weight: bold;
        color: #347AB8;
    }

    .nav-tabs li a {
        font-size: 12px;
    }

    .form-horizontal .form-group {
        margin: 0px -2px 10px -4px;
    }

    .menuSelecionado {
        cursor: default !important;
        font-weight: bold;
    }

    .esconde-div {
        display: none;
    }

    .panel-heading {
        height: 50px;
    }

    .progress {
        background-color: #969696;
    }

    .pie {
        display: none;
        visibility: hidden;
    }

    .peity {
        vertical-align: bottom;
    }

    .alinhajust {
        text-align: justify !important;
    }

    .no-margin {
        padding: 0px !important;
    }

    .pendencia:hover {
        color: #676a6c !important;
        text-decoration: underline;
    }

    .title-tables {
        color: #333;
    }

    .peity {
        vertical-align: sub !important;
    }

    .progress-bar-success span {
        color: #fff;
    }
</style>

<script src="js/bootstrap-tour.min.js"></script>
<script src="js/tour/indicadoresQualitativos.js"></script>

<script>
    function verificaRegra(criterio, dimid, areid, indid, crtid, caminho) {
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: '&crtidRegra=' + crtid + '&req=ajaxRegra',
            async: true,
            beforeSend: function () {
                divCarregado();
            },
            complete: function () {
                divCarregado();
            },
            success: function (resp) {
                if (resp) {
                    if (caminho == 2) {
                        if (criterio.attr('checked')) {
                            criterio.checked = true;
                        } else {
                            criterio.checked = false;
                        }
                    }
                    dados = resp.split('=>');
                    var tipo = dados[0];
                    arraycrt = dados[1].split('||');
                    $.each(arraycrt, function (i, val) {
                        if (tipo == 2) { // Não se aplica então bloqueia o resto
                            if (criterio.checked) { // estou marcando
                                $('#indicador-' + dimid + '-' + areid + '-' + indid + '-' + val).attr("disabled", true);
                                $('#indicador-' + dimid + '-' + areid + '-' + indid + '-' + val).parent().parent().hide();
                            } else { // Estou desmarcando
                                $('#indicador-' + dimid + '-' + areid + '-' + indid + '-' + val).removeAttr("disabled");
                                $('#indicador-' + dimid + '-' + areid + '-' + indid + '-' + val).parent().parent().show();
                            }
                        } else { // habilita os que foram passados
                            if (criterio.checked) { // estou marcando
                                $('#indicador-' + dimid + '-' + areid + '-' + indid + '-' + val).removeAttr("disabled");
                            } else { // Estou desmarcando
                                $('#indicador-' + dimid + '-' + areid + '-' + indid + '-' + val).attr("disabled", true);
                            }
                        }
                    });
                }
            }
        });
    }


    $(document).ready(function () {
        $('.tabIndicadoresDemonstrativo').click(function (e) {
            e.preventDefault();
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: '&req=ajaxQuadroDemonstrativo',
                async: true,
                beforeSend: function () {
                    $('#loading').show();
                },
                complete: function () {
                },
                success: function (resp) {
                    $('#loading').hide();
                    $('#quadroDemonstrativo').html(resp);
                }
            });
            $('#quadroDemonstrativo').modal();
        });

        $('.tabIndicadoresGrafico').click(function (e) {
            e.preventDefault();
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: '&req=ajaxGraficoIndicadores',
                async: true,
                beforeSend: function () {
                    $('#loading').show();
                },
                complete: function () {
                },
                success: function (resp) {
                    $('#loading').hide();
                    $('#graficoIndicadores').html(resp);
                }
            });
            $('#graficoIndicadores').modal();
        });

        $("a[data-toggle=collapse]").click(function () {
            var tipo = $(this).attr('tipo');
            if (tipo == '19') {
                $("#botaoBolivia").trigger("click");
            }
        });

        if ($('#tab-30').hasClass('active') && !$('input[id^=areid]:eq(0)').val()) {
            $('.tabIndicadoresDemonstrativo').trigger('click');
        }

        if ($('#tab-32').hasClass('active') && !$('input[id^=areid]:eq(0)').val()) {
            $('.tabIndicadoresGrafico').trigger('click');
        }

        $('.indicador').click(function () {
            var tipo = $(this).attr('tipo');

            if ($('#' + tipo).css('display') == 'none') {
                $('#' + tipo).show();
                $(this).removeClass('content-round');
                $(this).addClass('content-top-round');
            } else {
                $('#' + tipo).hide();
                $(this).addClass('content-round');
                $(this).removeClass('content-top-round');
            }
        });

        $('[id^="indicador-"]').click(function () {
            var valor = $(this).val();
            if ($('#' + valor).css('display') == 'none') {
                $('#' + valor).show();
            } else {
                $('#' + valor).hide();
            }
        });


        $('input[name=nivel]').change(function () {
            if ($(this).val() == "2") {
                $('#divFiltro').show();
                $('#divIndicador').hide();
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: '&escolas=0&req=ajax&nivel=' + 2,
                    async: true,
                    beforeSend: function () {
                        $('#loading').show();
                        $('#divIndicador').hide();

                    },
                    complete: function () {
                    },
                    success: function (resp) {
                        $('#loading').hide();
                        $('#divIndicador').show();
                        $('#divIndicador').html(resp);
                    }
                });

            } else {
                $('#divFiltro').hide();
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: '&escolas=' + $('#comboEscolas').val() + '&req=ajax&nivel=' + 1,
                    async: true,
                    beforeSend: function () {
                        $('#loading').show();
                        $('#divIndicador').hide();

                    },
                    complete: function () {
                    },
                    success: function (resp) {
                        $('#loading').hide();
                        $('#divIndicador').show();
                        $('#divIndicador').html(resp);
                    }
                });
            }
        });

        $('#btFiltrar').click(function () {
            if ("" != $('#comboEscolas').val()) {

                var nivel = $('input[name=nivel]:checked').val()

                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: '&escolas=' + $('#comboEscolas').val() + '&req=ajax&nivel=' + nivel,
                    async: true,
                    beforeSend: function () {
                        $('#loading').show();
                        $('#divIndicador').hide();

                    },
                    complete: function () {
                    },
                    success: function (resp) {
                        $('#loading').hide();
                        $('#divIndicador').show();
                        $('#divIndicador').html(resp);
                    }
                });
            } else {
                swal({
                    title: "Erro!",
                    text: "Escolha pelo menos uma escola para filtrar",
                    type: "error"
                });
            }

        });

        $('.menuArea').click(function (e) {
            var tipo = parseFloat($(this).attr('tipo'));
            if ($.inArray(tipo, [0, 21, 44, 62]) > 0) {
                $('#tabelasSPO').modal();
            }
        });

        $('.menuArea').each(function () {
            var tipo = parseFloat($(this).attr('tipo'));
            if ($.inArray(tipo, [0, 21, 44, 62]) > 0) {
                $('.menuArea[tipo="' + tipo + '"]').parent().parent().addClass('menuAreaTabelaSiope');
            }
        });

        $('[id^="metas["]').textext({
            plugins: 'tags',
        });


        $("span.pie").peity("pie", {
            fill: ['#1c84c6', '#d7d7d7', '#ffffff']
        })

        <?php if ($_REQUEST['tour']) : ?>
        $('.tourn-btn').trigger('click');
        <?php endif; ?>
    });
</script>
<div id="tab-<?php echo $dimensao->dimid; ?>" class="tab-pane <?php echo $ativa; ?>">
    <div class="panel-body">
        <div class="panel-group" id="accordion">
            <?php
            if ($arAreas) {
                $a = 0;
                foreach ($arAreas as $kAre => $area) {
                    $a++;
                    $dadosPercent = $oAreaController->pontuacaoArea($area['areid'], $_REQUEST['inuid']);
                    if ($_REQUEST['areid']) {
                        if ($_REQUEST['areid'] == $area['areid']) {
                            $ativaArea = 'in';
                            $statusArea = 'true';
                        } else {
                            $ativaArea = '';
                            $statusArea = 'false';
                        }
                    } else {
                        if ($a == 1) {
                            $ativaArea = 'in';
                            $statusArea = 'true';
                        } else {
                            $ativaArea = '';
                            $statusArea = 'false';
                        }
                    }
            ?>
            <div class="panel panel-default" id="panelarea<?php echo $area['areid']; ?>">
                <div class="panel-heading">
                    <h5 class="panel-title">
                        <a data-toggle="collapse" data-parent="#accordion" class="menuArea"
                           href="#collapse-<?php echo $area['areid']; ?>"
                           aria-expanded="true" class=""
                           tipo="<?php echo $area['areid']; ?>">
                            <div class="col-md-9"><?php echo $dimensao->dimcod . "." . $area['arecod'] . ' - ' . $area['aredsc']; ?></div>
                            <div class="col-md-3">
                                <div class="progress">
                                    <div style="width: <?php echo $dadosPercent['percent']; ?>%" aria-valuemax="10"
                                         aria-valuemin="0" aria-valuenow="<?php echo $dadosPercent['percent']; ?>"
                                         role="progressbar" class="progress-bar progress-bar-success">
                                        <span><?php echo number_format($dadosPercent['percent'], 0); ?>
                                            % Preenchido</span>
                                    </div>
                                </div>
                            </div>
                        </a>
                    </h5>
                </div>
                <div id="collapse-<?php echo $area['areid']; ?>"
                     class="panel-collapse panelIndicadores collapse <?php echo $ativaArea; ?>"
                     aria-expanded="<?php echo $statusArea; ?>">
                    <div class="panel-body" style="background-color: #f3f3f4;">
                        <form method="post" name="formulario" id="formulario">
                            <input type="hidden" name="req" id="req[<?php echo $area['areid']; ?>]" value=""/>
                            <input type="hidden" name="url" id="url[<?php echo $area['areid']; ?>]" value=""/>
                            <input type="hidden" name="inuid" id="inuid" value="<?php echo $_REQUEST['inuid'] ?>"/>
                            <input type="hidden" name="dimid" id="dimid" value="<?php echo $_REQUEST['dimid'] ?>"/>
                            <input type="hidden" name="areid" id="areid[<?php echo $area['areid']; ?>]"
                                   value="<?php echo $_REQUEST['areid'] ?>"/>
                            <input type="hidden" name="indid" id="indid[<?php echo $area['areid']; ?>]"
                                   value="<?php echo $_REQUEST['indid'] ?>"/>
                            <div class="row">
                                <div class="col-md-3 colunaIndicadores">
                                    <h5>
                                        <b>INDICADORES</b>
                                    </h5>
                                    <?php
                                    $i = 0;
                                    $indid = 0;
                                    $arIndicadores = $mIidi->recuperarIniciativaAreasIndicadores(['iniid' => $arrIni['iniid'], 'dimid' => $dimensao->dimid, 'areid' => $area['areid']]);
                                    if(empty($arIndicadores)){//Caso não haja indicadores vinculados a área informada são listados todos os indicadores
                                        $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']);
                                    }
                                    ?>
                                    <?php if ($arIndicadores) { ?>
                                        <ul class="folder-list m-b-md"
                                            style="padding: 0">
                                            <?php foreach ($arIndicadores as $indicador) {
                                                $i++;
                                                $primeiroSelecao = '';
                                                if ($i == 1) {
                                                    $indid = $indicador['indid'];
                                                    $primeiroSelecao = $_REQUEST['indid'] ? ' ' : 'menuSelecionado';
                                                }

                                                ?>
                                                <li>
                                                    <a
                                                       class="menuIndicadores <?php echo $primeiroSelecao; ?> <?php echo($_REQUEST['indid'] === $indicador['indid'] ? 'menuSelecionado' : '') ?>"
                                                       indid="<?php echo $indicador['indid']; ?>"
                                                       areid="<?php echo $area['areid']; ?>"
                                                       dimid="<?php echo $dimensao->dimid; ?>">
                                                        <?php echo $oPontuacao->buscaPontuacao($_REQUEST['inuid'], $indicador['indid']) ? $iconPreenchido : $iconNaoPreenchido; ?>
                                                        <?php echo $indicador[$indicador['indid']] = $dimensao->dimcod . "." . $area['arecod'] . "." . $indicador['indcod'] . " - " . $indicador['inddsc']; ?>
                                                    </a>
                                                </li>
                                            <?php } ?>
                                        </ul>
                                    <?php } else { ?>
                                        <p class="alert alert-warning">Não existem indicadores cadastrados</p>
                                    <?php } ?>
                                </div>
                                <?php
                                if ($_REQUEST['dimid'] == $dimensao->dimid && $_REQUEST['areid'] == $area['areid'] && $_REQUEST['indid']) {
                                    $indid = $_REQUEST['indid'];
                                }

                                $dadosIndicador = $oIndicador->recuperarDados($indid);
                                $codigoindicador = $oIndicador->recuperaCodigoIndicador($indid);
                                $metasGuia = $oIndicadorMeta->recuperarDadosColunaMeta($indid);
                                $indjustificativa = "";
                                $indmetas = "";
                                $ptopontuacao = "";
                                $dadosPontuacao = $oPontuacaoController->recuperaPontuacao($indid, $_REQUEST['inuid']);
                                $dadosPontuacao = $dadosPontuacao ? $dadosPontuacao : array();
                                $indjustificativa = $dadosPontuacao['ptojustificativa'];
                                $ptopontuacao = $dadosPontuacao['ptopontuacao'];
                                ?>
                                <div class="col-md-9 colunaIndicadores">
                                    <div class="ibox">
                                        <?php if ($dadosIndicador['indid']) : ?>
                                        <div class="ibox-title">
                                            <?php echo $codigoindicador . " - " . $dadosIndicador['inddsc']; ?>
                                        </div>
                                        <div class="ibox-content alinhajust">
                                            <h3>Descrição do Indicador</h3>
                                            <?php
                                            echo $dadosIndicador['indajuda'] . "<br><br>";

                                            if ($dadosIndicador['indlei']) {
                                                echo $dadosIndicador['indlei'] . "<br><br>";
                                            } else {
                                                if ($metasGuia[0]) {
                                                    $arrOrder = asort($metasGuia);
                                                    echo "Metas do PNE relacionadas a este indicador: " . implode(', ', $metasGuia) . " <br><br>";
                                                }
                                            }
                                            $tipoPMEPEE = $itrid == 2 ? "PME" : "PEE";
                                            ?>
                                            <div class="form-group">
                                                <p>
                                                    <label>Metas do <?php echo $tipoPMEPEE; ?> relacionadas a este indicador:</label>
                                                </p>
                                                <div class="row">
                                                    <?php
                                                    $dadosMeta = array();
                                                    for ($i = 1; $i <= 100; $i++) {
                                                        $dadosMeta[$i] = "Meta " . $i;
                                                    }

                                                    $metasM = array();
                                                    $metas = array();
                                                    if ($dadosPontuacao['ptoid']) {
                                                        $metas = $oPontuacaoMeta->recuperarDados($dadosPontuacao['ptoid']);
                                                        if (is_array($metas)) {
                                                            foreach ($metas as $meta) {
                                                                $metasM[] = $meta['pmtmeta'];
                                                            }
                                                            $metas = $metasM;
                                                        }
                                                    }
//                                                    echo $simec->select('metas[' . $dadosIndicador['indid'] . '][]', null, $metas, $dadosMeta, array('placeHolder' => 'Metas','disabled','maxlengh' => 1200, "$disabled"));
                                                    //echo montarAbasMetasComponentes(criarAbasMetasPNEArray($metas), "");
                                                    ?>
                                                    <table style="width: 100%!important;">
                                                        <tr>
                                                            <td colspan="2" id="div-metasaba<?php echo $a ?>">
                                                                <?php
                                                                if(!empty($metas)){
                                                                    echo montarAbasArrayLocal(criarAbasMetasPNEArray($metas),$a,"");
                                                                }else{
                                                                    echo "<div class='alert alert-info col-lg-offset-4 col-lg-4'>
                                                                            <p align='center'>Sem Metas Cadastradas</p>
                                                                         </div>";
                                                                }
                                                                ?>
                                                            </td>
                                                        </tr>
                                                        </tr>
                                                    </table>
                                                    <div id="div-submetas<?php echo $a;?>" div="col-lg-12">
                                                        <?php
                                                        if(!empty($metas)) {
                                                            echo listarSubMetasComponentesIndicador($metas[0], $a);
                                                        }
                                                        ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="ibox-title">
                                            <h3>Componentes do Indicador</h3>
                                        </div>
                                        <div class="ibox-content">
                                            <?php
                                            $arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($dadosIndicador['indid']);
                                            if ($arCriterios) {
                                            $i = 0;
                                            foreach ($arCriterios as $criterio) {
                                            $i++;
                                            $classEscondeDiv = 'esconde-div';
                                            $checked = '';
                                            if ($dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]) {
                                                $classEscondeDiv = '';
                                                $checked = 'checked';
                                                $pontuacao[$criterio['crtid']] = $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]['pontuacao'] ? $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]['pontuacao'] : null;
                                            }
                                            ?>
                                            <div class="form-group">
                                                <?php if ($dadosIndicador['indrequerido'] == 'S' && $criterio['crvvinculo'] == 't') {
                                                $checked = 'checked';
                                                $classEscondeDiv = ''; ?>
                                                <div class="checkbox" style="padding-left: 0px;">
                                                    <input
                                                            type="checkbox" style="display: none;" disabled
                                                            id="indicador-<?php echo $dimensao->dimid; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $criterio['crtid']; ?>"
                                                            name="indicador[<?php echo $dimensao->dimid; ?>][<?php echo $area['areid']; ?>][<?php echo $dadosIndicador['indid']; ?>][]"
                                                            value="<?php echo $criterio['crtid']; ?>" <?php echo $checked; ?>
                                                    >
                                                    <span style=""><?php echo $criterio['crtdsc']; ?></span>
                                                    <?php } else { ?>
                                                    <div class="checkbox">
                                                        <input
                                                                type="checkbox" disabled
                                                                id="indicador-<?php echo $dimensao->dimid; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $criterio['crtid']; ?>"
                                                                name="indicador[<?php echo $dimensao->dimid; ?>][<?php echo $area['areid']; ?>][<?php echo $dadosIndicador['indid']; ?>][]"
                                                                value="<?php echo $criterio['crtid']; ?>" <?php echo $checked; ?>>
                                                        <label for="indicador-<?php echo $dimensao->dimid; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $i; ?>"><?php echo $criterio['crtdsc']; ?></label>
                                                        <?php } ?>
                                                    </div>
                                                    <?php if ($criterio['crtpontuacao'] == 1 || $dadosIndicador['indrequerido'] == 'S' && $criterio['crvvinculo'] == 't') { ?>
                                                        <div class="disabled <?php echo $classEscondeDiv; ?> row"
                                                             style="padding-left: 20px; padding-right: 20px;"
                                                             id="<?php echo $criterio['crtid']; ?>">
                                                            <div id="compIndIonRange_<?php echo $criterio['crtid']; ?>"
                                                                 ctrid="<?php echo $criterio['crtid']; ?>"></div>
                                                            <script>$('#compIndIonRange_<?php echo $criterio['crtid']; ?>').ionRangeSlider({
                                                                    min: 0,
                                                                    max: 100,
                                                                    from: <?php echo $pontuacao[$criterio['crtid']] ? $pontuacao[$criterio['crtid']] : 0; ?>,
                                                                    postfix: " %",
                                                                    from_fixed: true,
                                                                    to_fixed: true,
                                                                    disable: true
                                                                })</script>
                                                            <input disabled id="ptcpontuacao_<?php echo $criterio['crtid']; ?>"
                                                                   name="ptcpontuacao[<?php echo $criterio['crtid']; ?>]"
                                                                   type="hidden"
                                                                   value="<?php echo $pontuacao[$criterio['crtid']]; ?>">
                                                        </div>
                                                    <?php } ?>
                                                </div>
                                                <?php } ?>
                                                <?php } else { ?>
                                                    <p class="alert alert-warning">
                                                        Sem critérios cadastrados
                                                        para o indicador.</p>
                                                <?php } ?>
                                                <div class="row"></div>

                                                <div class="form-group">
                                                    <p>
                                                        <label>Justificativa:</label>
                                                    </p>
                                                    <textarea disabled
                                                              name="indjustificativa[<?php echo $dadosIndicador['indid']; ?>]" <?php echo $disabled; ?>
                                                              id="indjustificativa[<?php echo $dadosIndicador['indid']; ?>]"
                                                              class="form-control"
                                                              required="required"
                                                              label-for="indjustificativa[<?php echo $dadosIndicador['indid']; ?>]"><?php echo $indjustificativa; ?></textarea>
                                                </div>
                                            </div>
                                            <?php else : ?>
                                                <p class="alert alert-warning">Indicador não cadastrado.</p>
                                            <?php endif; ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </form>
                    </div>
                </div>
                <?php
                    }
                }
                ?>
            </div>
        </div>
    </div>
    <?php
//    }

    if ($esdid == PAR3_WORKFLOW_FLUXO_DO_PAR_ELABORACAO && 1 == 0) : ?>
        <p class="text-justify">
            Cada um dos indicadores do diagnóstico, bem como seus componentes, apontam aspectos desejáveis de boas políticas de educação, em suas várias dimensões. Revisar o instrumento diagnóstico, com especial atenção aos componentes não selecionados durante o processo, pode fornecer indicações importantes dos caminhos e estratégias a serem tomados para aprimorar as ofertas em educação no seu estado ou município nos próximos quatro anos.
        </p>
        <p class="text-justify">
            Clique nas dimensões e áreas listadas acima para revisitar o diagnóstico e rever os espaços apontados pelo instrumento para possíveis ações de aperfeiçoamento. Essas indicações, poderão ser utilizadas na fase de Elaboração do Plano de Ações Articuladas, de maneira a torna-lo mais adequado à realidade do seu estado ou município.
        </p>

        <br>

        <?php
        if ($arDimensoes) : $d = 0;
        foreach ($arDimensoes as $dimensao) : $a = 0;
            $pontuacao = recuperaPontuacaoDiagnostico($_REQUEST['inuid'], $dimensao->dimid); $d++;
            $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao->dimid);
        ?>
                <div class="row">
                    <div class="col-lg-12">
                        <table class="table table-bordered">
                            <thead>
                            <tr>
                                <th colspan="3" style="text-align: center;">
                                    <?php echo "Dimensão {$d} - {$dimensao->dimdsc}"; ?>
                                    <span style="padding: 6px;" class="label label-success pull-right">Total Selecionado: <?php echo $pontuacao['pontuacao']; ?></span>
                                </th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php if(is_array($arAreas)){ ?>
                                <?php foreach ($arAreas as $area) : $a++; ?>
                                    <?php $pontuacaoArea = recuperaPontuacaoAreaDiagnostico($_REQUEST['inuid'], $area['areid']); ?>
                                    <?php $item['url'] = "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid={$_REQUEST['inuid']}&dimid={$dimensao->dimid}&areid={$area['areid']}"; ?>
                                    <?php if ($d != 4): ?>
                                        <tr>
                                            <td width="80%" style="vertical-align: middle;"><b><a class="area-link" href="<?php echo $item['url']; ?>"><?php echo "Área {$a}. {$area['aredsc']}"; ?></a></b></td>
                                            <td align="center" width="20%" class="" style="text-align: center; vertical-align: middle;">
                                                <div class="forum-info pull-left" style="margin-left: 40px;">
                                    <span class="views-number">
                                    <?php echo $pontuacaoArea['pontuacao'] ?>
                                    </span>
                                                    <div>
                                                        <small>Selecionado</small>
                                                    </div>
                                                </div>
                                                <small style="text-align: center; margin-top: 12px; display: inline-block;">de</small>
                                                <div class="forum-info pull-right" style="margin-right: 40px;">
                                    <span class="views-number">
                                    <?php echo $pontuacaoArea['total'] ?>
                                    </span>
                                                    <div>
                                                        <small>Total</small>
                                                    </div>
                                                </div>
                                            </td>
                                        </tr>
                                    <?php else : ?>
                                        <?php
                                        $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']);

                                        $arIndicadoresPontuacao = array();
                                        $arIndicadoresPontuacao[0]['descricao'] = 'ate 25%';
                                        $arIndicadoresPontuacao[0]['pontuacao'] = 0;

                                        $arIndicadoresPontuacao[1]['descricao'] = 'entre 25% e 50%';
                                        $arIndicadoresPontuacao[1]['pontuacao'] = 0;

                                        $arIndicadoresPontuacao[2]['descricao'] = 'entre 50% e 75%';
                                        $arIndicadoresPontuacao[2]['pontuacao'] = 0;

                                        $arIndicadoresPontuacao[3]['descricao'] = 'acima de 75%';
                                        $arIndicadoresPontuacao[3]['pontuacao'] = 0;

                                        $i = 0;

                                        foreach($arIndicadores as $indicador) {
                                            $pontuacaoIndicador = recuperaPontuacaoCriteriosDiagnostico($_REQUEST['inuid'], $indicador['indid']);
                                            $pontuacaoIndicador = is_array($pontuacaoIndicador) ? $pontuacaoIndicador : array();

                                            foreach ($pontuacaoIndicador as $pontuacao) {
                                                if ($pontuacao['percentual'] < 25 || empty($pontuacao['percentual'])) {
                                                    $arIndicadoresPontuacao[0]['pontuacao']++;
                                                } else if ($pontuacao['percentual'] < 50 && $pontuacao['percentual'] > 25) {
                                                    $arIndicadoresPontuacao[1]['pontuacao']++;
                                                } else if ($pontuacao['percentual'] < 75 && $pontuacao['percentual'] > 50) {
                                                    $arIndicadoresPontuacao[2]['pontuacao']++;
                                                } else {
                                                    $arIndicadoresPontuacao[3]['pontuacao']++;
                                                }
                                            }
                                        }
                                        ?>
                                        <tr>
                                            <td width="80%" style="vertical-align: middle;"><b><a class="area-link" href="<?php echo $item['url']; ?>"><?php echo "Área {$a}. {$area['aredsc']}"; ?></a></b></td>
                                            <td align="center" width="20%" style="text-align: center; vertical-align: middle; padding: 9px;">
                                                <?php for ($i = 0; $i < 4; $i++) : ?>
                                                    <div class="row" style="padding: 14px 0px 14px 0px; margin: -8px;">
                                                        <div class="forum-info pull-left" style="margin-left: 30px; width: 35%;">
                                    <span class="views-number">
                                        <?php echo $arIndicadoresPontuacao[$i]['pontuacao'] ?>
                                    </span>
                                                            <div>
                                                                <small><?php echo $arIndicadoresPontuacao[$i]['descricao']; ?></small>
                                                            </div>
                                                        </div>
                                                        <small style="margin-right: 30px; text-align: center; margin-top: 12px; display: inline-block;">de</small>
                                                        <div class="forum-info pull-right" style="margin-right: 40px;">
                                    <span class="views-number">
                                        <?php echo $pontuacaoArea['total'] ?>
                                    </span>
                                                            <div>
                                                                <small>Total</small>
                                                            </div>
                                                        </div>
                                                    </div>
                                                <?php endfor; ?>
                                            </td>
                                        </tr>
                                    <?php endif; ?>
                                <?php endforeach; ?>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            <?php endforeach; ?>
            <div class="row">
                <div class="col-lg-12 text-center">
                    <a target="_blank" id="btnImprimir" class="btn btn-primary btn-lg" href="<?php echo "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativosImpressao&acao=A&inuid={$_REQUEST['inuid']}"; ?>">
                        <i class="fa fa-print"></i> Imprimir Diagnóstico
                    </a>
                </div>
            </div>
        <?php endif; ?>
    <?php endif; ?>
