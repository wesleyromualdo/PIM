<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 05/06/2015
 * Time: 10:58
 */

// Dependências
ob_start();
global $db;

$ravid = $_GET['ravid'];
$diretoria = isset($_GET['diretoria']) ? $_GET['diretoria'] : 2;

$title = "Relatório AE Técnico DIVAPE";
$meses = array(
    '01' => 'Janeiro',
    '02' => 'Fevereiro',
    '03' => 'Março',
    '04' => 'Abril',
    '05' => 'Maio',
    '06' => 'Junho',
    '07' => 'Julho',
    '08' => 'Agosto',
    '09' => 'Setembro',
    '10' => 'Outubro',
    '11' => 'Novembro',
    '12' => 'Dezembro'
);

include_once '../../sase/classes/AvaliadorEducacional.class.inc';
include_once '../../sase/classes/RelatorioAvaliadorRedeae.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
// Fim Dependências

$avaliadoreducacional = new AvaliadorEducacional();
$rel = new RelatorioAvaliadorRedeae();

$usucpf = $_SESSION['usucpf'];
$rel->arAtributos['usucpf'] = $usucpf;
$data = new Data();
$pflcod = PFLCOD_SASE_TECNICO;
$ravid = $_GET['ravid'];
if($diretoria == 2){
    $pflcod = PFLCOD_SASE_TECNICO_DIVAPE;
}
$estuf = $rel->pegaEstadoAvaliador('estuf');
$mes = date('n');

/**
 * Carrega as informações do relatório.
 * Dentro da variável '$rel'
 */
if (!empty($ravid) && $ravid != 'undefined'){
    $rel->carregarPorId($ravid);
    //ver($rel, d);
    $perCampos = $rel->retornaDisCampos($_SESSION['usucpf']);

    $dados = $rel->getDados();
    $usucpf = $rel->arAtributos['usucpf'];
    $ratperiodo = $rel->arAtributos['ratperiodo'];
    $ratdata1periodo = $data->formataData($rel->arAtributos['ratdata1periodo']);
    $ratdata2periodo = $data->formataData($rel->arAtributos['ratdata2periodo']);
    $mr = explode("/", $ratdata2periodo);
    $mes = $mr[1];
    $pflcod = $rel->arAtributos['pflcod'];
    $estuf = $rel->pegaEstadoAvaliador('estuf');

}

if($_POST['acao']){
    $retorno = array(
        "return" => null,
        "msg"    => null,
        "ravid"  => null
    );
    switch ($_POST['acao']){
        case 'carregar':
            /**
             * Código encarregado de criar o relatório.
             */
            if ($rel->validaRelatorio()){
                $arAtr = array();
                $arAtr['pflcod']           = $_POST['pflcod'];
                $arAtr['ratdata1periodo']  = $_POST['ratdata1periodo'];
                $arAtr['ratdata2periodo']  = $_POST['ratdata2periodo'];
                $arAtr['ratperiodo']       = $_POST['ratperiodo'];
                $arAtr['usucpf']           = $_POST['usucpf'];
                $arAtr['ravtiporelatorio'] = 2;
                $rel->popularDadosObjeto($arAtr);

                //ver($rel->arAtributos, d);

                $ravid = $rel->salvarRelatorio();

                if ($ravid) {
                    $retorno['return'] = "true";
                    $retorno['msg']    = "Relatório criado com sucesso.";
                    $retorno['ravid']  = $ravid;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                }

            } else {
                $retorno['return'] = "false";
                $retorno['msg']    = $rel->msg;
            }
            echo implode("|",$retorno);
            exit;

        case 'salvar':
            $_POST    = simec_utf8_decode_recursive($_POST);
            $_REQUEST = simec_utf8_decode_recursive($_REQUEST);

            /**
             * Código encarregado de salvar as informações do relatório.
             */
            exit;

        case 'salvar_atividade_cronograma':
            $_POST    = simec_utf8_decode_recursive($_POST);
            $_REQUEST = simec_utf8_decode_recursive($_REQUEST);

            /**
             * Salva as atividades cronograma
             */
            if(!$rel->salvaAtividadeCronograma()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as atividades.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            $retorno['return'] = true;
            $retorno['msg']    = "Dados do relatório salvos com sucesso.";
            $retorno['ravid']  = $ravid;
            echo implode("|",$retorno);

            exit;

        case 'apaga_atividade_cronograma':
            $_POST    = simec_utf8_decode_recursive($_POST);
            $_REQUEST = simec_utf8_decode_recursive($_REQUEST);

            if(!$rel->apagaAtividadeCronograma($_POST['acrid'])){
                if ($rel->msg != ''){
                    echo $rel->msg;
                    exit;
                } else {
                    echo "Erro ao apagar as atividades.";
                    exit;
                }
            }

            echo "Atividade apagada com sucesso.";
            exit;

        case 'edita_atividade_cronograma':
            require_once APPRAIZ . "sase/classes/Atividcronogramaredeae.class.inc";
            $acr = new Atividcronogramaredeae();
            if($_POST['acrid']){
                $acr->carregarPorId($_POST['acrid']);
                $arr = array(
                    $acr->acrid,
                    $acr->acrdsc,
                    $acr->acrperiodo
                );
                echo simec_json_encode($arr);
            }
            exit;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
$prsano = trim($_SESSION['exercicio']);
//$perCampos = false;

//ver($ravid, $rel);
?>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<style>
    table td{
        text-align: center;
    }
</style>

<script>
    function salvar(acao, ajax){
        ajax = typeof ajax !== 'undefined' ? ajax : true;
        jQuery('#acao').val(acao);
        console.log(jQuery('#form-save').serialize());
        if (acao !== 'undefined'){
            if (ajax) {
                if(acao == 'imprimir'){
                    window.location.href = 'sase.php?modulo=relatorio/relPlanoTrabCoor&acao=A&ravid=<?= $_GET['ravid'] ?>';
                } else {
                    jQuery.ajax({
                        url: '',
                        type: 'POST',
                        data: jQuery('#form-save').serialize(),
                        //contentType: 'application/x-www-form-urlencoded; charset=ISO-8859-1',
                        //contentType: 'Content-type: text/plain; charset=iso-8859-1',
                        success: function (e) {
                            if (acao != 'teste') {
                                var a = e;
                                console.log(e);
                                var r = a.split("|");
                                alert(r[1]);
                                console.log(r);
                                if (r[2] != '' && r[2] !== 'undefined') {
//                                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria=<?//= $diretoria ?>//&ravid=' + r[2];
                                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPlanoTrabCoor&acao=A&ravid=' + r[2];
                                }
                            } else {
                                alert(e);
                            }
                        }
                    });
                }
            } else {
                jQuery('[name=acao]').val(acao);
                jQuery('[name=form-save]').submit();
            }
        } else {
            alert('Ação não informada.');
        }
    }

    /**
     *
     */
    function montaPeriodo(){

        switch (jQuery('#ratperiodo').val()) {
            case '1':
                jQuery('#ratdata1periodo').val('01/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val('15/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
            case '2':
                jQuery('#ratdata1periodo').val('16/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val((new Date(2015, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
        }

        console.log(jQuery('#ratdata1periodo').val() + " à " + jQuery('#ratdata2periodo').val());
        jQuery('#hidmes').val(jQuery('#mes').val());
    }

    function apagarAtividade(id, acrid, ravid){
        if(confirm("Deseja apagar esta atividade?")){
            jQuery.ajax({
                url: '',
                type: 'POST',
                data: {acrid: acrid, acao: 'apaga_atividade_cronograma'},
                success: function (e) {
                    alert(e);
                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPlanoTrabCoor&acao=A&ravid=' + ravid;
                }
            });
        }
    }

    function editaAtividade(acrid){
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {acrid: acrid, acao: 'edita_atividade_cronograma'},
            success: function(e){
                var a = JSON.parse(e);
                $('#acrid').val(a[0]);
                $('#acrdsc').val(a[1]);
                $('#acrperiodo').val(a[2]);
                console.log(a);
            }
        });
    }

    function limparAtividade(){
        $('#acrid').val("");
        $('#acrdsc').val("");
        $('#acrperiodo').val("");
    }
</script>


<div id='cabecalhoSistema'>
    <center><h3><?=$titulo?></h3></center>
</div>

<div id="container">
    <div class="row">
        <div class="col-md-12">
            <form id="form-save" method="post" name="form-save" role="form" class="form-horizontal">
                <input type="hidden" id="acao" name="acao" value=""/>
                <input type="hidden" id="ravid" name="ravid" value="<?= $ravid ?>"/>
                <input type="hidden" id="usucpf" name="usucpf" value="<?= $usucpf ?>"/>
                <!--                <input type="hidden" id="ratperiodo" name="ratperiodo" value="1"/>-->
                <input type="hidden" id="pflcod" name="pflcod" value="<?= $pflcod ?>"/>
                <input type="hidden" id="ratdata1periodo" name="ratdata1periodo" value="<?= $ratdata1periodo ?>"/>
                <input type="hidden" id="ratdata2periodo" name="ratdata2periodo" value="<?= $ratdata2periodo ?>"/>
                <input type="hidden" id="hidmes" name="hidmes" value="<?= $mes ?>"/>

                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Informações gerais do relatório                                                -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <div class="col-md-11">
                    <div class="well">
                        <fieldset>
                            <legend>1. Informaçoes gerais do relatório</legend>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Coordenador</label>
                                <div class="col-lg-8 col-md-8">
                                    <label style="font-weight: normal; margin-top: 8px;">
                                        <?php
                                        echo $rel->pegaAvaliador();
                                        ?>
                                    </label>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Estado</label>
                                <div class="col-lg-8 col-md-8">
                                    <label style="font-weight: normal; margin-top: 8px;">
                                        <?php
                                        echo $rel->pegaEstadoAvaliador();
                                        ?>
                                    </label>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label>
                                <div class="col-lg-8 col-md-8">
                                    <select class="form-control chosen" id="ratperiodo" name="ratperiodo" onchange="montaPeriodo()">
                                        <option value="">Selecione...</option>
                                        <option value="1" <?= $ratperiodo == 1 ? 'selected="true"' : '' ?>>1° Quinzena</option>
                                        <option value="2" <?= $ratperiodo == 2 ? 'selected="true"' : '' ?>>2° Quinzena</option>
                                    </select>
                                </div>
                            </div>
                            <?php if ($ratdata1periodo != '' && $ratdata2periodo != ''){ ?>
                                <div class="form-group">
                                    <label class="col-lg-4 col-md-4 control-label">Período em Data</label>
                                    <div class="col-lg-8 col-md-8">
                                        <label style="font-weight: normal; margin-top: 8px;">
                                            <?php
                                            echo $ratdata1periodo . ' à ' . $ratdata2periodo;
                                            ?>
                                        </label>
                                    </div>
                                </div>
                            <?php } ?>
                            <div class="form-group">
                                <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Mês:</label>
                                <div class="col-lg-8 col-md-8 ">
                                    <select class="form-control chosen" id="mes" name="mes" onchange="montaPeriodo()">
                                        <?php
                                        foreach ($meses as $c => $v) {
                                            $s = $mes == $c ? 'selected="true"' : '';
                                            echo <<<HTML
                                                        <option {$s} value="{$c}">$v</option>
HTML;
                                        }
                                        ?>
                                    </select>
                                </div>
                            </div>
                            <div class="text-right">
                                <?php if (empty($ravid)) { ?>
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('carregar')">Carregar
                                    </button>
                                <?php } else { ?>
<!--                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar')">Salvar-->
<!--                                    </button>-->
                                <?php } ?>
                                <?php if (!empty($ravid)) { ?>
                                    <button title="Gerar Relatorio" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('imprimir')">Gera Relatório
                                    </button>
                                <?php } ?>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Informações gerais do relatório                                                -->
                <!-- --------------------------------------------------------------------------------------------- -->

                <?php if($_GET['ravid']){ ?>

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Atividades (cronograma)                                                        -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>ATIVIDADES (CRONOGRAMA)</legend>
                                <div class="form-group">
                                    <label for="acrdsc" class="col-md-4 control-label">Descrição:</label>
                                    <div class="col-md-8">
                                        <input type="hidden" id="acrid" name="acrid" value=""/>
                                        <textarea class="form-control" name="acrdsc" id="acrdsc" cols="30" rows="3" maxlength="120"></textarea>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="acrperiodo" class="col-md-4 control-label">Período:</label>
                                    <div class="col-md-8">
                                        <textarea class="form-control" name="acrperiodo" id="acrperiodo" cols="30" rows="3" maxlength="50"></textarea>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="text-right">
                                        <button title="Salvar" class="btn btn-info" type="button" id="btnSalvar" onclick="salvar('salvar_atividade_cronograma')">Salvar
                                        </button>
                                        <button title="Salvar" class="btn btn-primary" type="button" id="btnLimpar" onclick="limparAtividade()">Limpar
                                        </button>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getAtividadesCronograma(true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Atividades (cronograma)                                                        -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <div class="text-right">
                                    <?php if (empty($ravid)) { ?>
                                        <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('carregar')">Carregar
                                        </button>
                                    <?php } else { ?>
<!--                                        <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar')">Salvar-->
<!--                                        </button>-->
                                    <?php } ?>
                                    <?php if (!empty($ravid)) { ?>
                                        <button title="Gerar Relatorio" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('imprimir')">Gera Relatório
                                        </button>
                                    <?php } ?>
                                </div>
                            </fieldset>
                        </div>
                    </div>

                <?php } ?>
            </form>
        </div>

    </div>
</div>
<script>
    $(document).ready(function(){
        $('.campoCpf').mask("999.999.999-99");
        $('.campoPorcentagem').mask("999");
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();
        // Inicia o período com o valor marcado na combo 'Mês'
        montaPeriodo();
    });
</script>