<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 05/06/2015
 * Time: 10:58
 */

// Dependências
//ob_start();
global $db;

$ravid = (integer)$_GET['ravid'];
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
include_once '../../sase/classes/Model/Anexorelavaliadorredeae.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";

// Fim Dependências

$avaliadoreducacional = new AvaliadorEducacional();
$rel = new RelatorioAvaliadorRedeae();

$usucpf = $_SESSION['usucpf'];
$rel->arAtributos['usucpf'] = $usucpf;
$data = new Data();
$pflcod = PFLCOD_SASE_TECNICO;
if($diretoria == 2){
    $pflcod = PFLCOD_SASE_TECNICO_DIVAPE;
}
$estuf = $rel->pegaEstadoAvaliador('estuf');
$mes = date('n');

/**
 * Carrega as informações do relatório.
 * Dentro da variável '$rel'
 */
if (!empty($ravid) && $ravid != 'undefined' && is_numeric($ravid)){
    $rel->carregarPorId($ravid);
    if(empty($rel->aveid) && empty($rel->ratdata1periodo) && empty($rel->ratdata2periodo)){
        $html = <<<HTML
            <script>
                alert('Relatório não encontrado.');
                window.location.href = "sase.php?modulo=principal/regrelatorioplanospne&diretoria={$diretoria}&acao=A";
            </script>
HTML;
        die($html);
    }
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

}

if($_POST['request']){
    $retorno = array(
        "return" => null,
        "msg"    => null,
        "ravid"  => null
    );
    switch ($_POST['request']){
        case 'download_arquivo':
            //ob_clean();
            $arqid = $_POST['arqid'];
            if($arqid){
            //$file = new FilesSimec("anexorelavaliadorredeae", $campos, "sase");
            $eschema = "sase";
            $file = new FilesSimec(null,null,$eschema);
            $file->getDownloadArquivo($arqid);

    	die('<script type="text/javascript">
        window.close();
        </script>');
             //$file->getDownloadArquivo($arqid);
            }
            exit;
        break;
        case 'salva_arquivo':
            $diretoria = $_GET['diretoria'];
            $ravid = $_POST['ravid'];
            $arqid = $_POST['arqid'];
            $arqidold = $arqid;
            $araid = $_POST['araid'];
            $aradsc = $_POST['aradscInc'];
            $usucpf = $_SESSION['usucpf'];
            $alt = !empty($arqid) ? true : false;
            
            if(!empty($ravid) || is_numeric($ravid)){
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
                            window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                        </script>
HTML;
                            die($html);
                        }
                    } else {
                        $html = <<<HTML
                        <script>
                            alert('O arquivo deve estar no formato PDF.');
                            window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria={$diretoria}&ravid={$ravid}';
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
                            $msg = "O arquivo selecionado excede o tamanho máximo definido em MAX_FILE_SIZE. Por favor contate o suporte do sistema.";
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
                            window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria={$diretoria}&ravid={$ravid}';
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
                        window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria={$diretoria}&ravid={$ravid}';
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
                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria={$diretoria}&ravid={$ravid}';
                </script>
HTML;
            }
            }else{
                echo <<<HTML
                <script>
                    alert('Relatório não encontrado.');
                    window.location.href = "sase.php?modulo=principal/regrelatorioplanospne&diretoria={$diretoria}&acao=A";
                </script>
HTML;
            }
            exit;

        case 'edita_arquivo':
           
            ob_clean();
			global $db;            $araid = $_POST['araid'];
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
            echo implode("|",$retorno);
            exit;

        case 'salvar':
            $_POST    = simec_utf8_decode_recursive($_POST);
            $_REQUEST = simec_utf8_decode_recursive($_REQUEST);

            /**
             * Código encarregado de salvar as informações do relatório.
             */

            /**
             * Salva o quadro de Atividades Execitadas no Período
             */
            if (!$rel->salvaAtividades()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Atividades Executadas no período.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            /**
             * Salva o quadro de resultados consolidados
             */
            if (!$rel->salvaResultadoConsolidadoPCP()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar os Resultados Consolidados no período.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            if(!$rel->salvaMunComAlteracao('c')){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar os Municipios que apresentaram alteração na sua etapa de trabalho no período.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            if(!$rel->salvaMunSemAlteracao()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar os Munisípios que permaneceram na mesma etapa do período anterior.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            if(!$rel->salvaSituacaoPlanoCarreiraProfessor()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar os Situaçao atual dos municipios, com relação ao plano de carreira e remuneraçao.";
                    echo implode("|",$retorno);
                    exit;
                }
            }


            /**
             * Salva o quadro de comprometimento das receitas com gasto com pessoal
             */
            if(!$rel->salvaComprometimento()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as informações do quadro de comprometimento das receitas no período.";
                    echo implode("|",$retorno);
                    exit;
                }
            }
//
//            /**
//             * Salva o quadro de estrutura do plano de carreira e remuneração
//             */
            if(!$rel->salvaEstruturaPlanoCarreiraRemuneracao()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as informações do quadro de estrutura do plano de carreira e remuneração.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            if(!$rel->salvaMunSemInformacao()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar os Municípios ainda sem informação ou sem trabalho iniciado.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            if(!$rel->salvaAcoesDesenvMun()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Ações a serem desenvolvidas nos municípios.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            if(!$rel->salvaInformacoesComplementares()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Informaçoes complementares.";
                    echo implode("|",$retorno);
                    exit;
                }
            }


            $retorno['return'] = true;
            $retorno['msg']    = "Dados do relatório salvos com sucesso.";
            $retorno['ravid']  = $ravid;
            echo implode("|",$retorno);

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
</style>

<script>
    function salvar(acao, ajax){
        console.log(acao, ajax, jQuery('#ravid').val())
        ajax = typeof ajax !== 'undefined' ? ajax : true;
        console.log(ajax);
        jQuery('#request').val(acao);
        if (acao !== 'undefined'){
            console.log('ops',ajax);
            if (ajax) {
                if(acao == 'imprimir'){
                    var diretoria = <?=$diretoria?>;
                    var _ravid    = jQuery('#ravid').val();
                    
                    if( _ravid > 0 && diretoria > 0 ){
                        window.location.href = 'sase.php?modulo=relatorio/relTecnicoPDC&acao=A&diretoria='+diretoria+'&ravid='+_ravid;
                    }else{
                        alert('Não foi possivél gerar o relatório. Selecione novamente o avaliador e tente novamente!');                        
                        window.location.href = 'sase.php?modulo=principal/regrelatorioplanospne&diretoria=2&acao=A';
                    }
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
                                //console.log(e);
                                var r = a.split("|");
                                //alert(r[1]);
                                console.log(r);
                                //debugger;
                                if (r[2] != '' && r[2] !== 'undefined' &&  r[2] % 1 === 0) {
                                    window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria=<?= $diretoria ?>&ravid=' + r[2];
                                }
                            } else {
                                alert(e);
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
                jQuery('#ratdata2periodo').val((new Date(<?= $prsano ?>, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
            case '3':
                jQuery('#ratdata1periodo').val('01/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val((new Date(<?= $prsano ?>, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
        }
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
                            <legend>Informaçoes gerais do relatório</legend>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Técnico</label>
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
                                    <button title="Gerar Relatorio" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('imprimir')">Gera Relatório
                                    </button>
                                <?php } ?>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="row col-md-1">
                    <?php
                    if (!$rel->docid && $rel->ravid) {
                        $docid = wf_cadastrarDocumento( TPDID_SASE_AVALIADOREDUCACIONAL_DIVAPE, 'Documento Avaliador Educacional - DIVAPE' );
                        //$docid = cadastraDocumentoAE($pflcod, 'Documento Avaliador Educacional');
                        $rel->docid = $docid;
                        // ver($Assessoramento,d);
                        $rel->alterar();
                        $rel->commit();
                    }

                    if ($_GET['ravid'] && $rel->docid)
                        wf_desenhaBarraNavegacao($rel->docid, array('docid' => $rel->docid, 'ravid' => $rel->ravid, 'usucpf' => $rel->arAtributos['usucpf'], 'ratdata1periodo' => $data->formataData($rel->arAtributos['ratdata1periodo']), 'ratdata2periodo' => $data->formataData($rel->arAtributos['ratdata2periodo'])));
                    ?>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Informações gerais do relatório                                                -->
                <!-- --------------------------------------------------------------------------------------------- -->

                <?php if($ravid){ ?>

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
                    <!--                Resultados Consolidados                                                        -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>2. Resultados Consolidados alcançados no período</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php //echo $rel->getResultadosConsolidadosPlanoCarreira($perCampos, true); ?>
                                        <?php //echo $rel->getSituacaoPlanoCarreiraProfessor($perCampos); ?>
                                        <?php echo $rel->getResultadoConsolidadoPCP(); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Resultados Consolidades                                                        -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municipios assistidos com alteração na existência de plano                     -->
                    <!-- --------------------------------------------------------------------------------------------- -->
<!--                    <div class="row col-md-11">-->
<!--                        <div class="well">-->
<!--                            <fieldset>-->
<!--                                <legend>4. Municípios assistidos com alteração na existência de Plano de Carreira</legend>-->
<!--                                <div class="form-group">-->
<!--                                    <div class="col-lg-12 col-md-12">-->
<!--                                        --><?php ////echo $rel->getListaAlteracaoExistPlano(true); ?>
<!--                                        --><?php //echo $rel->getMunicipiosAlteracaoPerioroTecRedeae(); ?>
<!--                                    </div>-->
<!--                                </div>-->
<!--                            </fieldset>-->
<!--                        </div>-->
<!--                    </div>-->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios assistidos com alteração na existência de plano                     -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Comprometimento das receitas com gastos com pessoal                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>3. Municípios do estado que apresentaram alteração na sua etapa de trabalho no período.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getMunComAlteracao(); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Comprometimento das receitas com gastos com pessoal                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municipios que permaneceram na mesma etapa do período anterior                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>4. Municípios assistidos que permaneceram na mesma etapa do período anterior.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getMunSemAlteracao(); ?>
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
                                        <?php echo $rel->getSituacaoPlanoCarreiraProfessor($perCampos); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Resultados Consolidades                                                        -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municipios que permaneceram na mesma etapa do período anterior                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>6. Comprometimento das receitas com gastos com pessoal.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getComprometimento($perCampos, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios que permaneceram na mesma etapa do período anterior                 -->
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
                                        <?php echo $rel->getEstruturaPlanoCarreiraRemuneracao($perCampos, true); ?>
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
                                        <?php echo $rel->getMunSemInformacao($perCampos); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios ainda sem informação ou sem trabalho iniciado                       -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Informações complementares sobre o período avaliado                            -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>9. Ações a serem desenvolvidas nos municípios.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getAcoesDesenvMun($perCampos); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Informações complementares sobre o período avaliado                            -->
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
                                        <button title="Gerar Relatorio" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('imprimir')">Gera Relatório
                                        </button>
                                    <?php } ?>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                </form>

                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Informações complementares sobre o período avaliado                            -->
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
                                            <input type="file" class="btn btn-primary start" name="file" accept="application/pdf" id="file" title="Selecionar arquivo" />
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
                <!--            Fim Informações complementares sobre o período avaliado                            -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Situação atual dos municípios com relação à elaboração/adequação               -->
                <!-- --------------------------------------------------------------------------------------------- -->
<!--                    <div class="row col-md-11">-->
<!--                        <div class="well">-->
<!--                            <fieldset>-->
<!--                                <legend>6. Situação atual dos municípios assistidos com relação à elaboração/adequação dos planos de carreira e remuneração</legend>-->
<!--                                <div class="form-group">-->
<!--                                    <div class="col-lg-12 col-md-12">-->
<!--                                        --><?php //echo $rel->getSituaAtualMunicipioTec($perCampos, 240, $rel->ratdata2periodo); ?>
<!--                                    </div>-->
<!--                                </div>-->
<!--                            </fieldset>-->
<!--                        </div>-->
<!--                    </div>-->
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Situação atual dos municípios com relaçao a elaboração/adequação               -->
                <!-- --------------------------------------------------------------------------------------------- -->


                <div class="col-md-1">
                    <?php

                    ?>
                </div>
            <?php } ?>

<!--            </form>-->
        </div>
    </div>
</div>
<script>
    $(document).ready(function(){
        $('.campoCpf').mask("999.999.999-99");
        $('.campoPorcentagem').mask("999");
        $('.campoHoras').mask("99999");
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();
        // Inicia o período com o valor marcado na combo 'Mês'
        montaPeriodo();
    });
</script>