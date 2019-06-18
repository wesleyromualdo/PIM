<?php

header('content-type: text/html; charset=utf-8');

if($_REQUEST["obrid"] != 'undefined' && $_REQUEST["obrid"] != '') {
    $obrid = $_REQUEST["obrid"];
}
if($_REQUEST["entid"] != 'undefined' && $_REQUEST["entid"] != '') {
    $entid = $_REQUEST["entid"];
}

$editar = false;
if($_REQUEST["crtid"] && $_REQUEST["crtid"]!=''){
    $crtid = $_REQUEST["crtid"];
    $editar = true;
}

/* Se não houver 'obrid', o usuário não tem permissão para seguir o processo. */
if (!isset($obrid) || !isset($entid)) {
    echo "<script>alert('Faltam parâmetros para acessar esta tela.');
    window.close();
    </script>";
    exit;
}

/* Requisição para solicitar a gravação das medições. */
if (isset($_POST["requisicao"]) && $_POST["requisicao"] == "salvarAditivo") {
    ob_clean();
//    ver($_POST, d);
    if($_POST['editar'] == 1){
        atualizaAditivo($_POST);
    }
    else{
        salvarAditivo($_POST);
    }
}

/* Requisição para solicitar o download do arquivo da medição. */
if (isset($_POST["requisicao"]) && $_POST["requisicao"] == "download") {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo "<script>window.location.href = '" . $_SERVER['HTTP_REFERER'] . "';</script>";
    exit;
}

/**
 * Função responsável por validar os dados inseridos no formulário de cadastro de aditivo.
 * @author Sérgio Henrique <sergio.hsilva@mec.gov.br>
 * @param $arrDados
 * @return array
 */
function validarDados($arrDados) {

    $arrRetorno = array("error" => false, "message" => null);

    if (is_array($arrDados) && !empty($arrDados)) {

        $denominacao = trim($arrDados["denominacao"]);
        if (empty($denominacao)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe a Denominação do Aditivo.";
        }

        $justificativa = trim($arrDados["justificativa"]);
        if (empty($justificativa)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe a Denominação do Aditivo.";
        }

        $dtassinatura = trim($arrDados["dtassinatura"]);
        if (empty($dtassinatura)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe a data de assinatura do Aditivo.";
        } else {
            $arrData = explode("/", $arrDados["dtassinatura"]);
            if (!checkdate($arrData[1], $arrData[0], $arrData[2])) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "A Data do Aditivo não tem formato válido.";
            }
        }

        $vlraditivo = trim($arrDados["vlraditivo"]);
        if (empty($vlraditivo)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Valor do Aditivo.";
        } else {
            if (!is_numeric($arrDados["vlraditivo"])) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "O Valor do Aditivo deve ser um número válido.";
            }
        }

    } else {
        $arrRetorno["error"] = true;
        $arrRetorno["message"] = "O Aditivo não foi salvo, pois houve problemas com os dados informados.";
    }

    return $arrRetorno;
}

/**
 * Função responsável por salvar os aditivos.
 * @author Sérgio Henrique <sergio.hsilva@mec.gov.br>
 * @param $arrDados
 */
function salvarAditivo($arrDados) {

    $arrDados["vlraditivo"] = str_replace(".", "", $arrDados["vlraditivo"]);
    $arrDados["vlraditivo"] = str_replace(",", ".", $arrDados["vlraditivo"]);
    $arrDados["vlraditivo"] = (float)$arrDados["vlraditivo"];
//ver($arrDados, d);
    $msgRetorno = null;

    if (!empty($_FILES["arquivo"]["name"])) {

        // O Boletim de Medição deve ser um arquivo no formato PDF.
        $arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
        if (in_array($_FILES["arquivo"]["type"], $arrMimeType)) {

            $validacao = validarDados($arrDados);
            if ($validacao["error"] === true) {

                $errorMessage = $validacao["message"];
                echo "<script>alert('{$errorMessage}');</script>";
                exit;

            } else {

                $obrid = $arrDados['obrid'];
                $entid = $arrDados['entid'];
                $crtid = $arrDados['crtid'];
                $denominacao = $arrDados['denominacao'];
                $dtassinatura = formata_data_sql($arrDados['dtassinatura']);
                $vlraditivo = $arrDados['vlraditivo'];
                $justificativa = $arrDados['justificativa'];

                require_once(APPRAIZ . "includes/classes/fileSimec.class.inc");
                $fileSimec = new FilesSimec();
                $fileSimec->setUpload('', null, false);
                $arqid = $fileSimec->getIdArquivo();


                if($arqid){
                    //implementar aqui o salvar do aditivo
                    global $db;

                    $execucaoFinanceira = new ExecucaoFinanceira();
                    $crtid = $execucaoFinanceira->pegaContratoOriginal($obrid);
//                    ver($crtid, d);

                    $sql = "INSERT INTO obras2.contrato (crtdtassinatura, crtvalorexecucao, crtstatus, crtdenominacao, 
                             crtjustificativa, ttaid, arqid, dt_cadastro, crtidpai_execfinceira, crtdtassinaturaaditivo)
                            VALUES ('{$dtassinatura}',{$vlraditivo}, 'A', '{$denominacao}', '{$justificativa}',2, {$arqid}, 'now()', {$crtid}, '{$dtassinatura}');";

                    $db->executar($sql);

                    if ($db->commit()) {
                        echo "<script>
                                        alert('Informações gravadas com sucesso.');
                                         window.opener.location.reload();
                                    </script>";
                        exit;
                    }

                }
                else {
                    echo "<script>
                            alert('O arquivo do Aditivo não foi salvo. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                }
            }

        } else {
            $msgRetorno = "O arquivo referente ao Aditivo deve ser um arquvo no formato PDF.";
        }
    } else {
        $msgRetorno = "O Aditivo não foi salvo.";
    }

    echo "<script>alert('{$msgRetorno}');</script>";
    exit;
}

/**
 * Função responsável por salvar os aditivos.
 * @author Sérgio Henrique <sergio.hsilva@mec.gov.br>
 * @param $arrDados
 */
function atualizaAditivo($arrDados) {

    $arrDados["vlraditivo"] = str_replace(".", "", $arrDados["vlraditivo"]);
    $arrDados["vlraditivo"] = str_replace(",", ".", $arrDados["vlraditivo"]);
    $arrDados["vlraditivo"] = (float)$arrDados["vlraditivo"];

    $msgRetorno = null;

    if($arrDados["editar"] == 1 && $arrDados["arqidOld"] != ''){
        if (!empty($_FILES["arquivo"]["name"])) {

            // O Boletim de Medição deve ser um arquivo no formato PDF.
            $arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
            if (in_array($_FILES["arquivo"]["type"], $arrMimeType)) {

                $validacao = validarDados($arrDados);
                if ($validacao["error"] === true) {

                    $errorMessage = $validacao["message"];
                    echo "<script>alert('{$errorMessage}');</script>";
                    exit;

                } else {

                    $obrid = $arrDados['obrid'];
                    $entid = $arrDados['entid'];
                    $crtid = $arrDados['crtid'];
                    $denominacao = $arrDados['denominacao'];
                    $dtassinatura = formata_data_sql($arrDados['dtassinatura']);
                    $vlraditivo = $arrDados['vlraditivo'];
                    $justificativa = $arrDados['justificativa'];
                    $crtid = $arrDados['crtidAditivo'];

                    require_once(APPRAIZ . "includes/classes/fileSimec.class.inc");
                    $fileSimec = new FilesSimec();
                    $fileSimec->setUpload('', null, false);
                    $arqid = $fileSimec->getIdArquivo();


                    if($arqid){
                        //implementar aqui o salvar do aditivo
                        global $db;

//                    ver($crtid, d);

                        $sql = "UPDATE obras2.contrato SET 
                                  crtdtassinatura = '{$dtassinatura}',
                                  crtvalorexecucao = {$vlraditivo},
                                  crtstatus = 'A',
                                  crtdenominacao = '{$denominacao}',
                                  crtjustificativa = '{$justificativa}',
                                  ttaid = 2,
                                  arqid = {$arqid} WHERE crtid = {$crtid}";

                        $db->executar($sql);

                        if ($db->commit()) {
                            echo "<script>
                                        alert('Informações gravadas com sucesso.');
                                         window.opener.location.reload();
                                    </script>";
                            exit;
                        }

                    }
                    else {
                        echo "<script>
                            alert('O arquivo do Aditivo não foi salvo. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                    }
                }

            } else {
                $msgRetorno = "O arquivo referente ao Aditivo deve ser um arquvo no formato PDF.";
            }
        } else {
            $msgRetorno = "O1 Aditivo não foi salvo.";
        }
    }
    else{
        $validacao = validarDados($arrDados);
        if ($validacao["error"] === true) {

            $errorMessage = $validacao["message"];
            echo "<script>alert('{$errorMessage}');</script>";
            exit;

        } else {

            $obrid = $arrDados['obrid'];
            $entid = $arrDados['entid'];
            $crtid = $arrDados['crtid'];
            $denominacao = $arrDados['denominacao'];
            $dtassinatura = formata_data_sql($arrDados['dtassinatura']);
            $vlraditivo = $arrDados['vlraditivo'];
            $justificativa = $arrDados['justificativa'];
            $crtid = $arrDados['crtidAditivo'];

            global $db;

            $sql = "UPDATE obras2.contrato SET 
                              crtdtassinatura = '{$dtassinatura}',
                              crtvalorexecucao = {$vlraditivo},
                              crtstatus = 'A',
                              crtdenominacao = '{$denominacao}',
                              crtjustificativa = '{$justificativa}',
                              ttaid = 2 WHERE crtid = {$crtid};";

            $db->executar($sql);

            if ($db->commit()) {
                echo "<script>
                                    alert('Informações gravadas com sucesso.');
                                     window.opener.location.reload();
                                </script>";
                exit;
            }
            else{
                $msgRetorno = "O2 Aditivo não foi salvo.";
            }
        }
    }

    echo "<script>alert('{$msgRetorno}');</script>";
    exit;
}

function buscaDadosAditivo($crtid){
    global $db;

    $sql = "SELECT crtid, crtdenominacao, crtdtassinatura, crtvalorexecucao, crtjustificativa, arqid FROM obras2.contrato WHERE crtid = {$crtid}";
    $retorno = $db->pegaLinha($sql);
    $retorno = $retorno ? $retorno :  array();

    return $retorno;
}

if($editar) {
    $dadosAditivo = buscaDadosAditivo($crtid);
}

?>

<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/obras2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<style type="text/css">

    tbody > tr:nth-child(even) {
        background-color: #f2f2f2;
    }

    td {
        vertical-align: middle;
        padding: 5px;
    }

    #tbAcompanhamento td {
        border: 1px solid #000;
        text-align: center;
        font-size: 13px;
    }

    .tbAcompanhamentoTitulo {
        font-weight: bold;
        text-align: center;
        border: 1px solid #000;
    }

    .acaoTbAcompanhamento {
        color: #0000FF;
        text-decoration: underline;
        cursor: pointer;
    }

    .detalheVisualizarPagamento {
        font-size: 14px;
    }

</style>

<form id="formulario" method="post" enctype="multipart/form-data" action="#">
    <input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid; ?>"/>
    <input type="hidden" name="entid" id="entid" value="<?php echo $entid; ?>"/>
    <?php if($editar){?>
    <input type="hidden" name="crtidAditivo" id="crtidAditivo" value="<?php echo $dadosAditivo['crtid']; ?>"/>
    <input type="hidden" name="editar" id="editar" value="1" />
    <?php }?>
    <!-- FORMULÁRIO DE INCLUSÃO DE ADITIVO -->
    <table class="Tabela" align="center" style="border: 0px; width:100%;">
        <tr>
            <td class="SubTitulocentro" colspan="2">Incluir Aditivo</td>
        </tr>
        <tr>
            <th class="SubTituloDireita">Denominação:</th>
            <td>
                <input
                        type="text"
                        style="text-align:left;"
                        name="denominacao"
                        id="denominacao"
                        onmouseout="MouseOut(this);"
                        onfocus="MouseClick(this);this.select();"
                        onmouseover="MouseOver(this);"
                        maxlength="30"
                        size="28"
                        class="obrigatorio normal"
                        value="<?= $dadosAditivo['crtdenominacao'] ?>"
                />
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
            </td>
        </tr>
        <tr>
            <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
            <td>
                <?= campo_data2( 'dtassinatura', 'S', 'S', '', 'S', '', '' , $dadosAditivo['crtdtassinatura']); ?>
            </td>
        </tr>

        <tr>
            <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
            <td>
                <input
                        type="text"
                        style="text-align:left;"
                        name="vlraditivo"
                        id="vlraditivo"
                        onmouseout="MouseOut(this);"
                        onfocus="MouseClick(this);this.select();"
                        onmouseover="MouseOver(this);"
                        onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                        maxlength="18"
                        size="17"
                        class="obrigatorio normal"
                        value="<?= formata_numero_monetario($dadosAditivo['crtvalorexecucao']) ?>"
                />
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
            </td>
        </tr>
        <tr>
        <tr>
            <th class="SubTituloDireita">Justificativa:</th>
            <td>
                <?=campo_textarea( 'justificativa', 'S', 'S', '', '70', '8', 2000, '',
                    '', '', '', '', $dadosAditivo['crtjustificativa']); ?>
            </td>
        </tr>
        <tr>
            <th class="SubTituloDireita">Arquivo:</th>
            <td>
                <?php 
                if ($editar) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $files = new FilesSimec();
                    $arquivo = $files->getDadosArquivo($dadosAditivo['arqid']);
//                    ver($arquivo, d);
                ?>
                <div class="divArquivoDownload">
                    <a class="link"
                       onclick="excluirArquivo('<?= $arquivo['arqid'] ?>');">
                        <img src="/imagens/excluir.gif"
                             title="Excluir arquivo <?= $arquivo['arqnome'] ?>" />
                    </a>
                    &nbsp;
                    <a class="link"
                       onclick="downloadArquivo('<?= $arquivo['arqid'] ?>');">
                        <img src="/imagens/salvar.png"
                             title="Fazer download do arquivo." />
                        <?= $arquivo['arqnome'] . '.' . $arquivo['arqextensao'] ?>
                    </a>
                </div>
                <div class="divArquivoUpload" style="display: none;">
                    <input type="hidden" name="arqidOld" id="arqidOld" value="" />
                    <input type="file" name="arquivo" id="arquivo"/>
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                </div>
                <?php
                } 
                else { 
                    ?>
                <input type="file" name="arquivo" id="arquivo"/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                <?php } ?>
            </td>
        </tr>


    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE ADITVO. -->

    <center>
        <input type="button" id="botaoSalvar" value="Salvar" onclick="salvarDocumento();">
        <input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
    </center>

    <input type="hidden" id="requisicao" name="requisicao" value="salvarAditivo"/>

</form>

<script type="text/javascript">

//    window.onunload = refreshParent;

    function validarDadosFormulario() {

        var valorAditivo = $("#vlraditivo").val();
        valorAditivo = valorAditivo.replace(/[.]/g, "");
        valorAditivo = valorAditivo.replace(/[,]/g, ".");
        valorAditivo = parseFloat(valorAditivo);

        if ($("#denominacao").val() == "") {
            return "Informe a denominação do aditivo.";
        } else if ($("#justificativa").val() == "") {
            return "Informe a justificativa.";
        } else if ((valorAditivo <= 0) || (isNaN(valorAditivo))) {
            return "O Valor do Aditivo deve ser um número maior que zero.";
        } else if ($("#dtassinatura").val() == "") {
            return "Informe a data de assinatura do aditivo.";
        }

        var arquivo = $("#arquivo").val();
        var editar = $("#editar").val();
        if(editar == 1){
            if($("#arqidOld").val() != ''){
                if (arquivo == "") {
                    return "Anexe o arquivo referente ao aditivo de valor.";
                }
                var extensao = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
                if (extensao != ".pdf") {
                    return "O Boletim de Medição deve ser um arquvo no formato PDF.";
                }
            }
        }
        else {
            if (arquivo == "") {
                return "Anexe o arquivo referente ao aditivo de valor.";
            }
            var extensao = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
            if (extensao != ".pdf") {
                return "O Boletim de Medição deve ser um arquvo no formato PDF.";
            }
        }

        return true;
    }

    function salvarDocumento() {

        var retornoValidacao = validarDadosFormulario();

        if (retornoValidacao !== true) {
            alert(retornoValidacao);
            return false;
        }

        $('#formulario').submit();
    }

    function refreshParent() {
        window.opener.location.reload();
    }

    function excluirArquivo(arqid) {
        if(confirm('Confirma a exclusão do arquivo?')) {
            $("#arqidOld").val(arqid);
            $(".divArquivoDownload").hide("fast");
            $(".divArquivoUpload").show("fast");
        }
    }

    function downloadArquivo(arqid) {
        window.location = window.location.href + "&requisicao=download&arqid=" + arqid;
    }

</script>

</body>