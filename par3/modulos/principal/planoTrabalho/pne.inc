<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$pne = new Par3_Controller_Pne();
$modelUnidade = new Par3_Model_InstrumentoUnidade();
$cabecalhoUnidadeHelper =  new Par3_Helper_CabecalhoUnidadeHelper();
checkParamInuid();

$inuid = $_REQUEST['inuid'];
$itrid = $controleUnidade->pegarItrid($inuid);
$menu = $_REQUEST['menu'];
$menu = ($menu === 'possuiDiagnostico') ? '' : $menu;
$arrMetas = $pne->retornaArrayMetas();

$unidade = new Par3_Model_InstrumentoUnidade($inuid);

$_SESSION['par']['itrid'] = $itrid;
$_SESSION['par']['estdescricao'] = $controleUnidade->pegarNomeEntidade($inuid);
$_SESSION['par']['mundescricao'] = $controleUnidade->pegarNomeEntidade($inuid);
$_SESSION['par']['estuf'] = $unidade->estuf;
$_SESSION['par']['muncod'] = $unidade->muncod;

$disabled = 'disabled';

$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

if (!$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

if ($arrPermissao['booEscrita']) {
    $disabled = '';
}

$iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido = '<span class="fa fa-check iconUnidade"' .
    'style="color:green !important;' .
    'background-color: transparent;"></span>';

require 'pne/funcoes.php';

require APPRAIZ . 'includes/cabecalho.inc';
?>
<script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
<script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>

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

    .tabela_box_azul_escuro tr td {
        margin: 20px;
        padding: 0px 10px 0px 10px;
    }

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
<?php
#  Verifica se exite Arquivo
$oPendencia = new Par3_Model_Pendencias();
$arqid = $oPendencia->recuperaArquivoSasePne($inuid, $unidade->estuf, $unidade->muncod, $unidade->itrid);
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <input type="hidden" name="menu" id="menu" value="<?php echo $menu; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
                <!--                <div class="ibox-tools">-->
                <!--                    <a class="tourn-btn" data-toggle="tooltip" title="Ajuda?" data-placement="bottom" href="#">-->
                <!--                        <i class="fa fa-question-circle"></i>-->
                <!--                    </a>-->
                <!--                </div>-->
            </div>
            <div class="ibox-content " id="print_area">
                <?php
                if ($_SESSION['par']['itrid'] == 2){
                    $pneText = "PNE";
                }else{
                    $pneText = "PEE";
                }
                $controleUnidade->cabecalhoUnidade(); ?>
                <?php if ($arqid): ?>
                    <div class="col-lg-12 text-center jumbotron">
                        <button class="btn btn-primary btn-lg" type="button" <?= $disabled ?> id="pneArquivo">
                            <?php if ($_SESSION['par']['itrid'] == 2) { ?>
                                <i class="fa fa-check-circle-o"></i> Download do arquivo do PME
                            <?php } else { ?>
                                <i class="fa fa-check-circle-o"></i> Download do arquivo do PEE
                            <?php } ?>
                        </button>
                    </div>
                    <div class="ibox-content-round-gray ibox-content">
                        <?php require_once('grafico.inc'); ?>
                    </div> 
                <?php else: ?>
                    <div class="ibox-content-round-gray ibox-content">
                        <div class="widget red-bg p-lg text-center">
                            <div class="row">
                                <div class="col-md-1">
                                    <i class="fa fa-warning fa-5x"></i>
                                </div>
                                <div class="col-md-11 text-left">
                                    <p><b>Seu plano de educação ainda não foi elaborado ou adequado conforme determina o artigo 8º da Lei 13.005, de 25 de junho de 2014, que Aprova o Plano Nacional de Educação  PNE e dá outras providências, portanto não será possível concluir seu diagnóstico.</b></p>
                                    <p><b>Em caso de dúvidas, entre em contato com a equipe da Diretoria de Cooperação e Planos de Educação/SASE/MEC pelos telefones <u>(61) 2022-7633 / 2022-8998</u> ou e-mail <u>dicope@mec.gov.br</u></b></p>
                                </div>
                            </div>
                        </div>
                    </div>
                <?php endif; ?>
            </div>
        </div>
    </div>
</div>
<?php require_once "pne/infoPreenchimento.php"; ?>
<script type="text/javascript">
    $(document).ready(function(){
        <?php
        //deixar o menu anterior a vista
        echo $cabecalhoUnidadeHelper->menuInicialNavegacaoMostrarAnterior();
        ?>
    });
</script>

<script type="text/javascript">
    $(function () {
        //Sobrescrita da funcionalidade imprimir
        var href = $('.btn-Imprimir-azul').attr('href');
        $('.printMenu').attr('target', '_blank').attr('href', href);

        $('.info-preenchimento').click(function () {
            $('#modalInfoPreenchimento').modal();
        });
    });

    $('#pneArquivo').click(function () {

        var url = 'par3.php?modulo=principal/planoTrabalho/pendencias&acao=A'
            + '&inuid=<?php echo $_REQUEST['inuid'] ?>&acao=download';

        window.open(url);

    });

    function abrirMetasPNE(){

        var url = 'http://www.observatoriodopne.org.br/metas-pne';
        window.open(url);
    }

    $('.btn-menu-meta').click(function () {
        $('.btn-primary').removeClass('btn-primary-active');
        $(this).addClass('btn-primary-active');
    });
</script>
<script src="js/tour/pne.js"></script>
