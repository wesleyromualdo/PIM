<?php

checkParamInuid();
$controleUnidade            = new Par3_Controller_InstrumentoUnidade();
$controleIndicadorINEP      = new Par3_Controller_IndicadorINEP();
$controleUnidadeEntidade    = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelAuditoriaUnidade 	    = new Par3_Model_AuditoriaUnidade();
$oIndicador                 = new Par3_Model_Indicador();
$oIndicadorMeta             = new Par3_Model_IndicadorMeta();
$oAreaController            = new Par3_Controller_Area();
$oPontuacaoController       = new Par3_Controller_Pontuacao();
$oPontuacao                 = new Par3_Model_Pontuacao();
$oPontuacaoMeta             = new Par3_Model_PontuacaoMeta();
$modelUnidade               = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$cabecalhoUnidadeHelper     = new Par3_Helper_CabecalhoUnidadeHelper();
$disabled = 'disabled';

$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

if (!$arrPermissao['booVisualizar']) {
    echo "<script>
                alert('Acesso negado.');
                window.location.href = 'par3.php?modulo=inicio&acao=C';
          </script>";
    die();
}

if ($arrPermissao['booEscrita']) {
    $disabled = '';
}

switch ($_POST['req']) {
    case 'salvar':
        $oPontuacao = new Par3_Controller_Pontuacao;
        $indid = $oPontuacao->preparaSalvar();
        $url = "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid={$_POST['inuid']}&dimid={$_POST['dimid']}&areid={$_POST['areid']}&indid=$indid";
        if ($_REQUEST['url']) {
            $url = $_REQUEST['url'];
        }
        simec_redirecionar($url, 'success');
        break;
    case 'ajax':
        $oPontuacao = new Par3_Controller_Pontuacao;
        $escolas = $_REQUEST['escolas'];
        $nivel = $_REQUEST['nivel'];
        if (!empty($escolas)) {
            $oPontuacao->filtroAjax($escolas, $nivel);
        } else {
            $oPontuacao->filtroAjax(null, $nivel);
        }
        die();
        break;
    case 'ajaxRegra':
        $oVinculacaoRegra = new Par3_Controller_CriterioVinculacao();
        echo $oVinculacaoRegra->recuperarRegra($_REQUEST['crtidRegra']);
        die();
        break;
    case 'iniciarDiagnostico':
        $controleUnidadeEntidade->tramitarWorkflowDocumentoNaoFinalizado($_REQUEST['inuid'], PAR3_WORKFLOW_FINALIZAR_FASE_PREPARATORIA_DIAGNOSTIGO);
        $arrayDados['aducpf'] = $_SESSION['usucpf'];
        $arrayDados['inuid'] = $_REQUEST['inuid'];
        $arrayDados['adutipo'] = 2;
        $arrayDados['adudata'] = date('Y-m-d H:i:s');
        $modelAuditoriaUnidade->popularDadosObjeto( $arrayDados );
        $modelAuditoriaUnidade->inserir();
        $modelAuditoriaUnidade->commit();
        die();
    case 'ajaxGraficoIndicadores':
        ?>
        <div class="modal-dialog modal-lg" style="width: 80%;">
            <div class="modal-content animated">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">
                        <span aria-hidden="true">×</span>
                        <span class="sr-only">Fechar</span>
                    </button>
                    <h4 class="modal-title">
                        <?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?>
                    </h4>
                </div>
                <div class="modal-body">
                    <?php require_once "indicadoresQualitativosGrafico.inc"; ?>
                </div>
            </div>
        </div>
        <?php
        die();
        break;
    case 'ajaxQuadroDemonstrativo':
        ?>
        <div class="modal-dialog modal-lg" style="width: 75%;">
            <div class="modal-content animated">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">
                        <span aria-hidden="true">×</span>
                        <span class="sr-only">Fechar</span>
                    </button>
                    <h4 class="modal-title">
                        Adequação da Formação Docente
                    </h4>
                </div>
                <div class="modal-body">
                    <?php require_once "indicadoresQualitativosDemonstrativo.inc"; ?>
                </div>
            </div>
        </div>
        <?php
        die();
        break;
    default:

        $esdid = $controleUnidadeEntidade->recuperarEstadoWorkflow($_REQUEST['inuid']);
        $situacao = recuperaSituacaoPreenchimentoDiagnostico($_REQUEST['inuid']);

        if($esdid == PAR3_ESDID_DIAGNOSTICO_ELABORACAO && $situacao['percent'] >= 100){
            $controleUnidadeEntidade->tramitarWorkflowDocumento($_REQUEST['inuid'], PAR3_WORKFLOW_FINALIZAR_FASE_DE_DIAGNOSTIGO);
        }
}

if ($_REQUEST['finalizar'] && $esdid == PAR3_ESDID_DIAGNOSTICO_FINALIZADO) {
    $controleUnidadeEntidade->tramitarWorkflowDocumento($_REQUEST['inuid'], PAR3_AEDID_ENVIAR_PLANEJAMENTO_NAO_INICIADO);
    die();
}

# ESSE BLOCO NÃO FOI PUBLICADO
# Caso o usuário tenha clicado no botão "Visualizar Sintese diagnostico" força o sistema exibir a tela de Resumo de Dianostico padrão.
if ($_REQUEST['visualizar-sintese-diagnostico']) {
    $_SESSION['visualizar-sintese-diagnostico'] = TRUE;

    // Caso o usuário tenha clicado na tela inicial de Diagnóstico sem ter selecionado nenhuma aba então restaura
    // a navegação original do sistema e verfica pendências.
    // Caso as pêndencias existam o sistema mostra a Tela de Resumo de Diagnóstico com indices pendentes
    // solicitando o preenchimento dele primeiro e caso não existam pêndencias o sistema mostra a tela de
    // resumo de diagnóstico informando os passos finais para concluir essa etapa.
} elseif (!$_REQUEST['resumo'] && !$_REQUEST['dimid']) {
    unset($_SESSION['visualizar-sintese-diagnostico']);
}

$existePendenciaGeral = Par3_Controller_Diagnostico::verificarPendencia($_REQUEST['inuid']);

$verificaInicioDiagnostico = (
        $modelAuditoriaUnidade->verificarInicioDiagnostico($_REQUEST['inuid']) &&
        $esdid != PAR3_ESDID_ETAPA_PREPARATORIA_DO_DIAGNOSTICO
    );

$itrid = $controleUnidade->pegarItrid($_REQUEST['inuid']);
$arrLiberaBtnDiagnostico = array(
            PAR3_ESDID_DIAGNOSTICO_FINALIZADO,
            PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO,
            PAR3_WORKFLOW_FLUXO_DO_PAR_PREPARATORIA_DIAGNOSTIGO,
            );
$diagnosticoFinalizado = 'S'; //finalizado

if(in_array($esdid,$arrLiberaBtnDiagnostico)){
    $diagnosticoFinalizado = 'N';
}

//PAR3_WORKFLOW_FINALIZAR_FASE_DE_DIAGNOSTIGO
if ($itrid == 2) {
    $nivel = "município";
} else if ($itrid == 3) {
    $nivel = "distrito";
} else {
    $nivel = "estado";
}

if ($_REQUEST['visualizar-sintese-diagnostico'])

$iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido = '<span class="fa fa-check iconUnidade"' .
    'style="color:green !important;' .
    'background-color: transparent;"></span>';

$a = 0;
$arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid);
/*ver($esdid);
ver($arrLiberaBtnDiagnostico);
ver($diagnosticoFinalizado);*/
require APPRAIZ . 'includes/cabecalho.inc';
?>
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

        $('#btnFinalizar').click(function () {
            <?php if ($itrid == 1) : ?>
            var messagem = "Ao finalizar o diagnóstico, as informações registradas pelo município permanecerão disponíveis apenas para consulta.";
            messagem += "O sistema não possibilitará alterações nos dados informados nas Seções PNE, Questões Estratégicas e Diagnóstico. ";
            messagem += "Sendo assim, recomendamos que seja feita uma revisão criteriosa, antes de finalizar o diagnóstico, ";
            messagem += "a fim de que as informações representem, de forma fidedigna, a real situação da rede municipal de educação.<br><br>";
            messagem += "Tem certeza de que deseja finalizar o diagnóstico?";
            <?php else :?>
            var messagem = "Ao finalizar o diagnóstico, as informações registradas pelo município permanecerão disponíveis apenas para consulta.";
            messagem += "O sistema não possibilitará alterações nos dados informados nas Seções PNE, Questões Estratégicas e Diagnóstico. ";
            messagem += "Sendo assim, recomendamos que seja feita uma revisão criteriosa, antes de finalizar o diagnóstico, ";
            messagem += "a fim de que as informações representem, de forma fidedigna, a real situação da rede estadual de educação.<br><br>";
            messagem += "Tem certeza de que deseja finalizar o diagnóstico?";
            <?php endif; ?>
            swal({
                title: "Prezado(a) Dirigente",
                text: messagem,
                type: "warning", showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }, function (isConfirm) {
                console.log(isConfirm);
                if (isConfirm) {
                    var url = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=<?php echo $_REQUEST['inuid']?>&finalizar=t';
                    $(location).attr('href', url);
                }
            });
        });

        $('.salvar').click(function () {
            var indid = $(this).attr('indid');
            if ($('[name="indjustificativa[' + indid + ']"]').val() != "") {
                $('#dimid').val($(this).attr('dimid'));
                var areid = $(this).attr('areid');
                $('[id="areid[' + areid + ']"]').val(areid);
                $('[id="indid[' + areid + ']"]').val($(this).attr('indid'));
                $('[id="url[' + areid + ']"]').val($(this).attr('url'));
                $('[id="req[' + areid + ']"]').val('salvar');
                $('#formulario').submit();
            }
        });

        $('.menuArea').click(function (e) {
            var tipo = parseFloat($(this).attr('tipo'));
            if ($.inArray(tipo, [0, 21, 44, 62]) > 0) {
                $('#tabelasSPO').modal();
            }
        });

        $('.menuArea').each(function() {
            var tipo = parseFloat($(this).attr('tipo'));
            if ($.inArray(tipo, [0, 21, 44, 62]) > 0) {
                $('.menuArea[tipo="'+ tipo +'"]').parent().parent().addClass('menuAreaTabelaSiope');
            }
        })

        $('.menuIndicadores').click(function (e) {
            e.preventDefault();
            var dimid = $(this).attr('dimid');
            var areid = $(this).attr('areid');
            var indid = $(this).attr('indid');
            var indidAntigo = $('#indid').val();
            var url = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A'
                + '&inuid=<?php echo $_REQUEST['inuid']?>&dimid=' + dimid + '&areid=' + areid + '&indid=' + indid;

            if (indid != indidAntigo) {
                $(location).attr('href', url);
            }
        });

        $('[id^="ionrange_"]').ionRangeSlider({
            min: 0,
            max: 10,
            from: 5,
            postfix: " %"
        });

        $('[id^="ionrange_"]').on("change", function () {
            var $this = $(this),
                id = $this.attr('id');
            idsplit = id.split('_');
            crtid = idsplit[1];
            value = $this.prop("value");
            $('#ptcpontuacao_' + crtid).val(value);
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

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <input type="hidden" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>

            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <div class="ibox-content">
                <?php if(!$_REQUEST['req']) $controleUnidade->cabecalhoUnidade();  ?>
                <div class="ibox-content-round-gray ibox-content">
                    <?php

                    	$arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid, $dimid);

                    	$_REQUEST['dimid'] = $_REQUEST['dimid'] ? $_REQUEST['dimid'] : $arDimensoes[0]['dimid'];

                        $aba = $_REQUEST['resumo'] ? "&resumo=1" : "&dimid=" . $_REQUEST['dimid'];

                        $url = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=' . $_REQUEST['inuid'] . $aba;

                        $arrDiagnosticoNaoFinalizado = array('', PAR3_ESDID_ETAPA_PREPARATORIA);

                        if (
                            (!$verificaInicioDiagnostico && !$_SESSION['visualizar-sintese-diagnostico']) ||
                            ($existePendenciaGeral && !$_SESSION['visualizar-sintese-diagnostico'])
                        ) {
                        ?>
                        <div class="tabs-container">
                            <div class="tab-content">
                                <?php require_once 'indicadoresQualitativosResumo.inc'; ?>
                            </div>
                        </div>
                    <?php } else if (
                                        $_SESSION['visualizar-sintese-diagnostico'] ||
                                        $_REQUEST['resumo'] ||
                                        (
                                            !count($pendencias) &&
                                            !in_array($esdid, $arrDiagnosticoNaoFinalizado)
                                        )
                                    ) { ?>
                        <?php echo $simec->tab(criaAbaIndicadoresQualitativos($_REQUEST['inuid'], $arDimensoes), $url); ?>
                        <div class="tabs-container">
                            <div class="tab-content">
                                <?php require_once 'indicadoresQualitativosTabs.inc'; ?>
                            </div>
                        </div>
                    <?php } ?>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="ibox animated modal" id="tabelasSPO" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 100%;">
        <div class="modal-content animated" style="padding: 20px">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">×</span>
                    <span class="sr-only">Fechar</span>
                </button>
                <h4 class="modal-title">
                    <?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?>
                </h4>
            </div>
            <div class="modal-body">
                <?php require_once "acompanhamento/listaSiope.inc"; ?>
            </div>
        </div>
    </div>
</div>
<div class="ibox animated modal" id="graficoIndicadores" tabindex="-1" role="dialog" aria-hidden="true"></div>
<?php
//monta array com os estados que o documento deve estar para que possa mostrar o menu planejamento
$mostrar_proximo = false;
$arrDocumentosPodeMostrarProximo = array(PAR3_ESDID_DIAGNOSTICO_FINALIZADO,PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO,PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO);
//se não houver um esdid ainda no arquivo
if($esdid == ''  || is_null($esdid)){
    $esdid = $controleUnidadeEntidade->recuperarEstadoWorkflow($_REQUEST['inuid']);
}
//verifica se o documento está em um dos estados
$mostrar_proximo = (in_array($esdid,$arrDocumentosPodeMostrarProximo));
//ver($arrDocumentosPodeMostrarProximo);
?>
<div class="ibox animated modal" id="quadroDemonstrativo" tabindex="-1" role="dialog" aria-hidden="true"></div>
<script type="text/javascript">
    $( document ).ready(function() {
        <?php
        //caso o $mostrar_proximo seja true, ao carregar a tela, após um segundo, é mostrado o menu planejamento
        echo $cabecalhoUnidadeHelper->menuInicialNavegacaoMostrarProximo($mostrar_proximo);
        ?>
    });
</script>