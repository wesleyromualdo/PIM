<?php
set_time_limit(0);
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

$title = "Relatório AE Supervisor Geral";
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
$pflcod = PFLCOD_SASE_EXECUTIVO;
if($diretoria == 2){
    $pflcod = PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE;
}
$estuf = $rel->pegaEstadoAvaliador('estuf');
$mes = date('n');

/**
 * Carrega as informações do relatório.
 * Dentro da variável '$rel'
 */
if (!empty($ravid) && $ravid != 'undefined' && is_numeric($ravid)){

    $rel->carregarPorId($ravid);
    //ver($rel, d);
    $perCampos = $rel->retornaDisCampos($_SESSION['usucpf'], $diretoria);

    $dados = $rel->getDados();
    $usucpf = $rel->arAtributos['usucpf'];
    $ratperiodo = $rel->arAtributos['ratperiodo'];
    $ratdata1periodo = $data->formataData($rel->arAtributos['ratdata1periodo']);
    $ratdata2periodo = $data->formataData($rel->arAtributos['ratdata2periodo']);
    $mr = explode("/", $ratdata2periodo);
    $mes = $mr[1];
    $pflcod = $rel->arAtributos['pflcod'];
    $estuf = $rel->pegaEstadoAvaliador('estuf');

    if(!$rel->docid){
        //$docid = wf_cadastrarDocumento(TPDID_SASE_AVALIADOREDUCACIONAL_DIVAPE, 'Avaliador Educacional - DIVAPE');
        $docid = cadastraDocumentoAE($pflcod, 'Avaliador Educacional - DIVAPE');
        $rel->docid = $docid;
        $rel->alterar();
        $rel->commit();
    }
}

if($_POST['request']){
    $retorno = array(
        "return" => null,
        "msg"    => null,
        "ravid"  => null
    );
    $_POST    = simec_utf8_decode_recursive($_POST);
    $_REQUEST = simec_utf8_decode_recursive($_REQUEST);
    switch ($_POST['request']){
        case 'salva_arquivo':
            $diretoria = $_GET['diretoria'];
            $ravid = $_POST['ravid'];
            $arqid = $_POST['arqid'];
            $arqidold = $arqid;
            $araid = $_POST['araid'];
            $aradsc = $_POST['aradscInc'];
            $usucpf = $_SESSION['usucpf'];
            $alt = !empty($arqid) ? true : false;

            $campos = array(
                'ravid' => $ravid,
                'aradsc' => "'" . $_POST['aradscInc'] . "'",
                'usucpf' => "'" . trim($_SESSION['usucpf']) . "'"
            );

            $file = new FilesSimec("anexorelavaliadorredeae", $campos, "sase");
            if($_FILES['file']['size'] > 0) {
                if($_FILES['file']['error'] === UPLOAD_ERR_OK) {
                    if ($_FILES['file']['type'] == 'application/pdf') {
                        $file->setPulaTableEschema(true);
                        $arquivoSalvo = $file->setUpload($_FILES['file']['name'], null, false);
                        $arqid = $file->getIdArquivo();
                        if (empty($arqid)) {
                            $html = <<<HTML
                        <script>
                            alert('Erro ao salvar o arquivo, tente novamente');
                            window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                        </script>
HTML;
                            die($html);
                        }
                    } else {
                        $html = <<<HTML
                        <script>
                            alert('O arquivo deve estar no formato PDF.');
                            window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                        </script>
HTML;
                        die($html);
                    }
                } else {
                    switch($_FILES['file']['error']){
                        case UPLOAD_ERR_INI_SIZE:
                            $msg = "O arquivo selecionado excede o tamanho limite definido. Por favor contate o suporte do sistema.";
                            break;
                        case UPLOAD_ERR_FORM_SIZE:
                            $msg = "O arquivo selecionado excede o tamanho máximo definido em MAX_FILE_SIZE. Por favor cntete o suporte do sistema.";
                            break;
                        case UPLOAD_ERR_NO_FILE:
                            $msg = "Nenhum arquivo foi selecionado.";
                            break;
                        default:
                            $msg = "Erro ao salvar o arquivo.";
                            break;
                    }
                    echo <<<HTML
                        <script>
                            alert('{$msg}');
                            window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                        </script>
HTML;
                }
            }

            $ara = new Sase_Model_Anexorelavaliadorredeae();
            if (!empty($araid)) {
                $ara->carregarPorId($araid);
            } else {
                $ara->ravid = $ravid;
                $ara->usucpf = $usucpf;
            }
            $ara->aradsc = $aradsc;
            if(!empty($arqid)) {
                $ara->arqid = $arqid;
            } else {
                if(empty($ara->arqid)){
                    echo <<<HTML
                    <script>
                        alert('Selecione um arquivo.');
                        window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                    </script>
HTML;
                    return false;
                }
            }
            $ara->salvar();
            $msg = "";
            if ($ara->commit()) {
                if ($alt) {
                    $msg = "Arquivo alterado com sucesso!";
                } else {
                    $msg = "Arquivo cadastrado com sucesso!";
                }
                echo <<<HTML
                <script>
                    alert('{$msg}');
                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                </script>
HTML;
            }
            exit;

        case 'edita_arquivo':
            ob_clean();
            global $db;
            $araid = $_POST['araid'];
            if($araid){
                $sql = <<<DML
                    select
                        ara.araid,
                        ara.aradsc,
                        arq.arqid,
                        arq.arqdescricao
                    from sase.anexorelavaliadorredeae ara
                    inner join public.arquivo arq on ara.arqid = arq.arqid
                    where araid = {$araid}
DML;
                $dados = $db->carregar($sql)[0]; 
                echo simec_json_encode($dados);
            }
            exit;

        case 'apaga_arquivo':
            ob_clean();
            $araid = $_POST['araid'];
            $arqid = $_POST['arqid'];

            if($araid && $arqid){
                $campos = array(
                    'araid' => $araid
                );
                $file = new FilesSimec("anexorelavaliadorredeae", $campos, "sase");
                $file->setPulaTableEschema(false);
                if($file->setRemoveUpload($arqid)){
                    $retorno['return'] = "true";
                    $retorno['msg']    = "Arquivo apagado com sucesso.";
                    $retorno['ravid']  = $ravid;
                }
                echo simec_json_encode($retorno);
            }
            exit;

        case 'download_arquivo':
            $file = new FilesSimec("anexorelavaliadorredeae", $campos, "sase");
            $arqid = $_POST['arqid'];
            if($arqid){
                ob_clean();
                $file->getDownloadArquivo($arqid);
            }
            exit;

        case 'carregar':
            /**
             * Código encarregado de criar o relatório.
             */
            if ($rel->validaRelatorio()){
                $arAtr = array();
                $arAtr['pflcod']          = $_POST['pflcod'];
                $arAtr['ratdata1periodo'] = $_POST['ratdata1periodo'];
                $arAtr['ratdata2periodo'] = $_POST['ratdata2periodo'];
                $arAtr['ratperiodo']      = $_POST['ratperiodo'];
                $arAtr['usucpf']          = $_POST['usucpf'];
                $rel->popularDadosObjeto($arAtr);

                //ver($rel->arAtributos, d);

                $ravid = $rel->salvarRelatorio($diretoria);

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
           echo '<resp>'.implode("|",$retorno).'</resp>';
            exit;

        case 'salvar':
//            ver($_POST['iplid'], d);
            /**
             * Salva o quadro de Atividades Execitadas no Período
             */
            if (!$rel->salvaAtividades()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Atividades Executadas no período.";
                    echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            /**
             * Salva os quadros 2, 3 e 8;
             */
            if(!$rel->salvaTabelaInfoPlanoCarreira()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Informações a respeito do Plano de Carreira e Remuneração Estadual.";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            if(!$rel->salvaEstadosComAlteracaoNaEtapaDivapeCoo()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar os Munisípios do Estado que apresentaram alteração na sua etapa de trabalho no período.";
                    echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            if(!$rel->salvaSituacaoAtualMunPlanCarreiraCoo()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao gravar a situaçao atual dos municípios assistidos com relação à existência de plano de carreira e remuneração.";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            /**
             * Salva o quadro de comprometimento das receitas com gasto com pessoal
             */
            if(!$rel->salvaComprMunCoo()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao gravar o comprometimento das receitas com gastos com pessoal(FUNDEB).";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            /**
             * Salva o quadro de estrutura do plano de carreira e remuneração
             */
            if(!$rel->salvaEstruturaPCRDispersaoCoo()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao gravar a estrutura do plano de carreira e remuneração (Dispersão Total).";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            if(!$rel->salvaEstruturaPCRHorasDocentesCoo()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao gravar a estrutura do plano de carreira e remuneração (Relaçao de Horas Docentes)";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            if(!$rel->salvaMunPorEstadosDoc()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao gravar as ações propostas para os municípios.";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            if(!$rel->salvaInformacoesComplementares()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Informaçoes complementares.";
                   echo '<resp>'.implode("|",$retorno).'</resp>';
                    exit;
                }
            }

            $retorno['return'] = true;
            $retorno['msg']    = "Dados do relatório salvos com sucesso.";
            $retorno['ravid']  = $ravid;
           echo '<resp>'.implode("|",$retorno).'</resp>';

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
<link rel="StyleSheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<style>
    table td{
        text-align: center;
    }
    /* ESCONDE O BOTÃO DE IMPRESSÃO DO RELATÓRIO
    .btn-imprimir{
        display: none;
    }*/
</style>

<script>
    function salvar(acao, ajax){
        ajax = typeof ajax !== 'undefined' ? ajax : true;
        jQuery('#request').val(acao);
        var ravid = '<?php echo $rel->ravid;?>';
        if (acao !== 'undefined'){
            if (ajax) {
                if(acao == 'imprimir'){
                    window.location.href = 'sase.php?modulo=relatorio/relCoordenadorPDC&acao=A&ravid='+ravid;
                } else {
                    jQuery.ajax({
                        url: '',
                        type: 'POST',
                        data: jQuery('#form-save').serialize(),
//                        contentType: 'application/x-www-form-urlencoded; charset=ISO-8859-1',
//                        contentType: 'Content-type: text/plain; charset=iso-8859-1',
                        success: function (e) {
                            if (acao != 'teste') {
                                var a = pegaRetornoAjax('<resp>', '</resp>', e, true);//e;
                                console.log(a,e);
                                var r = a.split("|");
                                //console.log(r);
                                if (r[2] != '' && r[2] !== 'undefined') {
                                    alert('Alterado com sucesso!');
                                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria=<?= $diretoria ?>&ravid=' + r[2];
                                }else{
                                    alert(r[1]);
                                }
                            } else {
                                alert('Falha ao salvar' + e);
                                return false;
                            }
                        }
                    });
                }
            } else {
                jQuery('[name=request]').val(acao);
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
            case '3':
                jQuery('#ratdata1periodo').val('01/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val((new Date(<?= $prsano ?>, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
        }

        console.log(jQuery('#ratdata1periodo').val() + " à " + jQuery('#ratdata2periodo').val());
        jQuery('#hidmes').val(jQuery('#mes').val());
    }

    function salvaArquivo(){
        $('#form-arquivo').submit();
    }

    function editaArquivo(araid){
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                araid: araid,
                request: 'edita_arquivo'
            },
            success: function(data){
                var a = JSON.parse(data);
                console.log(a);
                $('#arqid').val(a['arqid']);
                $('#araid').val(a['araid']);
                //$('#file').val(a['arqdescricao']);
                $('#aradscInc').val(a['aradsc']);
            }
        });
    }

    function apagaArquivo(araid, arqid){
        if(confirm('Deseja apagar este anexo?')) {
            jQuery.ajax({
                url: '',
                type: 'POST',
                data: {
                    request: 'apaga_arquivo',
                    araid: araid,
                    arqid: arqid
                },
                success: function (data) {
                    var a = JSON.parse(data);
                    alert(a['msg']);
                    window.location.reload();
                }
            });
        }
    }

    function downloadArquivo(arqid){
        $('#requestArq').val('download_arquivo');
        $('#arqid').val(arqid);
        $('#form-arquivo').submit();
    }

</script>


<div id='cabecalhoSistema'>
    <center><h3><?=$titulo?></h3></center>
</div>

<div id="container">
    <div class="row">
        <div class="col-md-12">
            <form id="form-save" method="post" name="form-save" role="form" class="form-horizontal">
                <input type="hidden" id="request" name="request" value=""/>
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
                            <legend>Informações gerais do relatório</legend>
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
                                        <option value="3" <?= $ratperiodo == 3 ? 'selected="true"' : '' ?>>Mensal</option>
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
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar')">Salvar
                                    </button>
                                <?php } ?>
                                <?php if (!empty($ravid)) { ?>
                                    <button title="Gerar Relatorio" class="btn btn-success btn-imprimir" type="button" id="btnPesquisar" onclick="salvar('imprimir')">Gera Relatório
                                    </button>
                                <?php } ?>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Informações gerais do relatório                                                -->
                <!-- --------------------------------------------------------------------------------------------- -->

                <?php if($ravid){ ?>
                <div class="col-md-1">
                    <?php
                    if($rel->docid){
                        wf_desenhaBarraNavegacao($rel->docid, array('docid'=>$rel->docid, 'ravid'=>$rel->ravid, 'usucpf'=>$usucpf, 'ratdata1periodo'=>$rel->ratdata1periodo, 'ratdata2periodo'=>$rel->ratdata2periodo), null, 'Avaliador Educacional - DIVAPE');
                    }
                    ?>
                </div>

                <div class="row col-md-11">
                    <div class="well">
                        <fieldset>
                            <div class="form-group">
                                <?php echo $rel->getMunicipiosAssistidos(); ?>
                            </div>
                        </fieldset>
                    </div>
                </div>

                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Atividades exutadas no período                                                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <div class="row col-md-11">
                    <div class="well">
                        <fieldset>
                            <legend>1. Atividades Executadas no Período</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $rel->getListaAtividadesDesenvolvidas($perCampos, true, $pflcod, true, $estuf, $diretoria); ?>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Atividades exutadas no período                                                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Municipios que permaneceram na mesma etapa do período anterior                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <div class="row col-md-11">
                    <div class="well">
                        <fieldset>
                            <legend>2. Informações a respeito do Plano de Carreira e Remuneração Estadual.</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $rel->getTabelaInfoPlanoCarreira($ravid,$perCampos); ?>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Municípios que permaneceram na mesma etapa do período anterior                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Municipios que permaneceram na mesma etapa do período anterior                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <div class="row col-md-11">
                    <div class="well">
                        <fieldset>
                            <legend>3. Situaçao atual dos municípios assistidos com relação à elaboração/adequação dos planos de carreira e remuneração.</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $rel->getTabelaMunAssistidos(); ?>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Municípios que permaneceram na mesma etapa do período anterior                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Municipios que permaneceram na mesma etapa do período anterior                 -->
                <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>4. Municípios do Estado que apresentaram alterações na sua etapa de trabalho no período.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getEstadosComAlteracaoNaEtapaDivapeCoo($perCampos); ?>
                                        <?php //echo $rel->getComprometimento($perCampos, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios que permaneceram na mesma etapa do período anterior                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Resultados Consolidados                                                        -->
                <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>5. Situação atual dos municípios assistidos com relação à existência de planos de carreira e remuneração, o cumprimento da Lei 11.738/2008 e a adequação dos planos de carreira ao que estabelece a Meta 18 do PNE.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php //echo $rel->getResultadosConsolidadosPlanoCarreira($perCampos, true); ?>
                                        <?php echo $rel->getSituacaoAtualMunPlanCarreiraCoo($perCampos); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Resultados Consolidades                                                        -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Comprometimento das receitas com gastos com pessoal                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>6. Comprometimento das receitas com gastos com pessoal (FUNDEB)</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getComprMunCoo($perCampos, true); ?>
                                        <?php //echo $rel->getComprometimentoReceitaPessoal($perCampos); ?>
                                        <?php //echo $rel->getComprometimento($perCampos, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Comprometimento das receitas com gastos com pessoal                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Comprometimento das receitas com gastos com pessoal                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>7. Estrutura do plano de carreira e remuneração</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getEstruturaPCRDispersaoCoo($perCampos); ?>
                                        <?php echo $rel->getEstruturaPCRHorasDocentesCoo($perCampos); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Comprometimento das receitas com gastos com pessoal                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municípios ainda sem informação ou sem trabalho iniciado                       -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>8. Municípios ainda sem informação ou sem trabalho iniciado.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getTableMunSemInformacao(); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios ainda sem informação ou sem trabalho iniciado                       -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Situação atual dos municípios com relação à elaboração/adequação               -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>9. Ações a serem desenvolvidas nos municípios.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getMunPorEstadosDoc($perCampos); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Situação atual dos municípios com relaçao a elaboração/adequação               -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Informações complementares sobre o período avaliado                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>10. Informações complementares sobre o período avaliado.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo retornaCampoTextArea($rel->ravinfcomplementar, 'ravinfcomplementar', false, '', 500, 5); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Informações complementares sobre o período avaliado                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
            </form>

            <!-- --------------------------------------------------------------------------------------------- -->
            <!--                                    Anexos do relatório                                        -->
            <!-- --------------------------------------------------------------------------------------------- -->
            <div class="row col-md-11">
                <div class="well">
                    <fieldset>
                        <legend>11. Anexos.</legend>
                        <form id="form-arquivo" method="post" name="form-arquivo" role="form" class="form-horizontal" enctype="multipart/form-data">
                            <input type="hidden" id="requestArq" name="request" value="salva_arquivo"/>
                            <input type="hidden" name="ravid" value="<?php echo $rel->ravid; ?>"/>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <label class="col-lg-4 col-md-4 control-label">Arquivo:</label>
                                    <div class="col-lg-8 col-md-8">
                                        <input type="hidden" name="arqid" id="arqid"/>
                                        <input type="hidden" name="araid" id="araid"/>
                                        <input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
                                    </div>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <label class="col-lg-4 col-md-4 control-label">Descrição:</label>
                                    <div class="col-lg-6 col-md-6">
                                        <textarea name="aradscInc" id="aradscInc" style="width: 337px;" class="form-control" cols="50" maxlength="50" rows="3"></textarea>
                                    </div>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-4 col-md-4"></div>
                                <div class="col-lg-8 col-md-8">
                                    <button title="Salvar" class="btn btn-success" type="button" onclick="salvaArquivo()" id="btnPesquisar">Salvar</button>
                                </div>
                            </div>
                            <div class="col-lg-12 col-md-12">
                                <?php echo $rel->getAnexos($perCampos); ?>
                            </div>
                        </form>
                    </fieldset>
                </div>
            </div>
            <!-- --------------------------------------------------------------------------------------------- -->
            <!--                                Fim Anexos do relatório                                        -->
            <!-- --------------------------------------------------------------------------------------------- -->

            <div class="row col-md-11">
                <div class="well">
                    <fieldset>
                        <div class="text-right">
                            <?php if (empty($ravid)) { ?>
                                <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('carregar')">Carregar
                                </button>
                            <?php } else { ?>
                                <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar')">Salvar
                                </button>
                            <?php } ?>
                            <?php if (!empty($ravid)) { ?>
                                <button title="Gerar Relatorio" class="btn btn-success btn-imprimir" type="button" id="btnPesquisar" onclick="salvar('imprimir')">Gera Relatório
                                </button>
                            <?php } ?>
                        </div>
                    </fieldset>
                </div>
            </div>
        <?php } ?>

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
