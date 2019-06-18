<?php
/**
 * @todo ALTERAR LINKS DAS HISTÓRIAS.
 */
header('content-type: text/html; charset=utf-8');

/* Arquivo responsável por utilizar o webservice de consulta de Pessoas Jurídicas à base da Receita Federal. */
//require APPRAIZ . "www/includes/webservice/pj.php";

$empid = $_SESSION["obras2"]["empid"];
$obrid = $_SESSION["obras2"]["obrid"];
$medid = $_REQUEST["medid"];

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

if (empty($obrid) && $empid) {
    $obras = new Obras();
    $arrObrid = $obras->pegaIdObraPorEmpid($empid);
    $obrid = (is_array($arrObrid) && !empty($arrObrid)) ? current($arrObrid) : $obrid;

    if (empty($obrid)) {
        die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
    }
}

/* Se não houver 'obrid', o usuário não tem permissão para seguir o processo. */
if (!isset($obrid)) {
    echo "<script>alert('Faltam parâmetros para acessar esta tela.');</script>";
    exit;
}

/* Requisição para solicitar o download do arquivo da medição. */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "download") {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo "<script>window.location.href = '" . $_SERVER['HTTP_REFERER'] . "';</script>";
    exit;
}

/**
 * Função responsável por pesquisar os dados da medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=16571
 * @param $obrid
 * @param $medid
 * @return array|mixed|NULL
 */
function getDadosMedicao($obrid, $medid) {

    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    $sql = "
        SELECT
          med.obrid,
          med.medid,
          CASE WHEN (med.crtid IS NOT NULL) AND (med.cceid IS NULL)
            THEN 'ent'
          WHEN (med.crtid IS NULL) AND (med.cceid IS NOT NULL)
            THEN 'cex'
          ELSE NULL END                              AS medtipoempresacontratada,
          CASE WHEN (med.crtid IS NOT NULL) AND (med.cceid IS NULL)
            THEN med.crtid
          WHEN (med.crtid IS NULL) AND (med.cceid IS NOT NULL)
            THEN med.cceid
          ELSE NULL END                              AS medidcontratoempresacontratada,
          med.obrid,
          med.arqid,
          med.mednummedicao,
          to_char(med.meddtmedicao, 'DD/MM/YYYY')    AS meddtmedicao,
          to_char(med.meddtinicioexec, 'DD/MM/YYYY') AS meddtinicioexec,
          to_char(med.meddtfimexec, 'DD/MM/YYYY')    AS meddtfimexec,
          med.medvlrmedicao,
          CASE
          WHEN med.crtid NOTNULL AND med.cceid ISNULL THEN (
            SELECT crt.entidempresa FROM obras2.contrato crt WHERE crt.crtstatus = 'A' AND crt.crtid = med.crtid
          )
          WHEN med.crtid ISNULL AND med.cceid NOTNULL THEN (
            SELECT cce.cexid FROM obras2.contratoconstrutoraextra cce WHERE cce.ccestatus = 'A' AND cce.cceid = med.cceid
          ) END medidempresacontratada
        FROM obras2.medicoes med
        WHERE
          med.medstatus = 'A'
          AND med.medid = {$medid}
          AND med.obrid IN ({$strObrids});
    ";

    $arrMedicao = $db->carregar($sql);

    $retorno = !$arrMedicao ? array() : current($arrMedicao);

    return $retorno;
}

/**
 * @todo Comentar depois.
 * @param $obrid
 * @param $tipoFornecedor
 * @param $idFornecedor
 * @return bool|mixed|NULL|string
 */
function getFornecedor($obrid, $tipoFornecedor, $idFornecedor) {

    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    if ($tipoFornecedor == "ent") {

        $sql = "
            SELECT
              '(' ||
              (SELECT substr(cnpj, 1, 2) || '.' || substr(cnpj, 3, 3) || '.' || substr(cnpj, 6, 3) || '/' || substr(cnpj, 9, 4) ||
                      '-' || substr(cnpj, 13) AS cnpj
               FROM (SELECT cast(ent.entnumcpfcnpj AS VARCHAR) AS cnpj) a) || ') ' || ent.entnome || ' (Contrato ' ||
              to_char(c.crtdtassinatura, 'DD/MM/YYYY') || ')' AS descricao
            FROM obras2.obrascontrato ocr INNER JOIN obras2.contrato c ON (ocr.crtid = c.crtid) OR (ocr.crtid = c.crtidpai)
              INNER JOIN entidade.entidade ent ON ent.entid = c.entidempresa
            WHERE ocr.ocrstatus = 'A' AND c.crtstatus = 'A' AND ent.entstatus = 'A' AND c.ttaid IS NULL AND ocr.obrid = {$obrid};
        ";

    } else {

        $sql = "
            SELECT
              '(' ||
              (
                SELECT substr(cnpj, 1, 2) || '.' || substr(cnpj, 3, 3) || '.' ||
                       substr(cnpj, 6, 3) || '/' || substr(cnpj, 9, 4) || '-' ||
                       substr(cnpj, 13) AS cnpj
                FROM (SELECT cast(cex.cexnumcnpj AS VARCHAR) AS cnpj) a
              ) || ') ' || cex.cexrazsocialconstrutora || ' (Contrato ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY') ||
              ')'                 AS descricao
            FROM obras2.construtoraextra cex
              INNER JOIN obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
            WHERE cex.cexstatus = 'A'
                  AND cce.ccedataassinatura IS NOT NULL
                  AND cce.ttaid IS NULL
                  AND
                  cex.obrid = {$obrid};
        ";
    }

    return $db->pegaUm($sql);

}

/**
 * Função responsável por pesquisar os dados dos contratos/aditivos de determinada medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=16571
 * @param $medid
 * @return array|mixed|NULL
 */
function getDadosMedicaoContrato($medid) {

    global $db;

    $sql = "
        SELECT
          mec.medid,
          CASE WHEN (mec.crtid IS NOT NULL) AND (mec.cceid IS NULL)
            THEN mec.crtid
          WHEN (mec.crtid IS NULL) AND (mec.cceid IS NOT NULL)
            THEN mec.cceid
          ELSE NULL
          END AS idcontrato,
        
          CASE WHEN (mec.crtid IS NOT NULL) AND (mec.cceid IS NULL)
            THEN (
              SELECT CASE WHEN c.ttaid IS NULL
                THEN 'Contrato Original - ' || to_char(c.crtdtassinatura, 'DD/MM/YYYY')
                     ELSE
                       'Aditivo de Valor - ' || to_char(c.crtdtassinatura, 'DD/MM/YYYY')
                     END
              FROM obras2.contrato c
              WHERE c.crtid = mec.crtid
            )
          WHEN (mec.crtid IS NULL) AND (mec.cceid IS NOT NULL)
            THEN (
              SELECT CASE WHEN cce.ttaid IS NULL
                THEN 'Contrato Original - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
                     ELSE
                       'Aditivo de Valor - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
                     END
              FROM obras2.contratoconstrutoraextra cce
              WHERE cce.cceid = mec.cceid
            )
          ELSE NULL
          END AS contrato,
          mec.mecvlrreferencia
        FROM
          obras2.medicaocontrato mec
        WHERE
          mec.mecstatus = 'A'
          AND
          mec.medid = {$medid};
    ";

    $arrDadosContratoMedicao = $db->carregar($sql);

    if (is_array($arrDadosContratoMedicao) && !empty($arrDadosContratoMedicao)) {
        return $arrDadosContratoMedicao;
    } else {
        return array();
    }
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

    .detalheVisualizar {
        font-size: 14px;
        background-color: #DCDCDC;
        font-weight: bold;
    }

</style>

<?php
$execucaoFinanceira = new ExecucaoFinanceira();

$arrDadosMedicao = getDadosMedicao($obrid, $medid);

$arrDadosMedicaoContrato = getDadosMedicaoContrato($medid);

if (!is_array($arrDadosMedicao) || empty($arrDadosMedicao)) {
    echo "
        <script type='text/javascript'>
        alert('Não encontramos informações sobre a medição.');
        window.close();
        </script>
    ";
}
?>

<form id="formulario" method="post" enctype="multipart/form-data" action="#">
    <!-- FORMULÁRIO DE VISUALIZAÇÃO DA MEDIÇÃO -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Visualizar Medição</td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Empresa Contradada:
                        </td>
                        <td>
                            <?php
                            $tipoFornecedor = $arrDadosMedicao["medtipoempresacontratada"];;
                            $idFornecedor = $arrDadosMedicao["medidcontratoempresacontratada"];
                            echo getFornecedor($obrid, $tipoFornecedor, $idFornecedor);
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Boletim de Medição:
                        </td>
                        <td>
                            <?php
                            $arquivoMedicao = $execucaoFinanceira->recuperaDadosArquivo($arrDadosMedicao["arqid"]);
                            ?>
                            <a class="link"
                               onclick="downloadArquivo('<?= $arrDadosMedicao['arqid'] ?>');">
                                <img src="/imagens/salvar.png"
                                     title="Fazer download do arquivo." />
                                <?= $arquivoMedicao['arqnome'] . '.' . $arquivoMedicao['arqextensao'] ?>
                            </a>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Número da Medição:
                        </td>
                        <td>
                            <?php
                            echo $arrDadosMedicao["mednummedicao"];
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Data da Medição:
                        </td>
                        <td>
                            <?php
                            echo $arrDadosMedicao["meddtmedicao"];
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Período de Execução:
                        </td>
                        <td>
                            <?php
                            echo $arrDadosMedicao["meddtinicioexec"] . " à " . $arrDadosMedicao["meddtfimexec"];
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor da Medição:
                        </td>
                        <td>
                            <?php
                            echo number_format($arrDadosMedicao["medvlrmedicao"], 2, ",", ".")
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE VISUALIZAÇAO DE MEDIÇÕES. -->

    <br/>

    <!-- INÍCIO DA TABELA DE ACOMPANHAMENTO. -->
    <div id="acompanhamento" style="display: <?php echo $display; ?>">

        <table width="95%" align="center" border="1" style="border-collapse: collapse" id="tbAcompanhamento">
            <thead>
            <tr>
                <td class="tbAcompanhamentoTitulo">Contrato ou Aditivo de Valor</td>
                <td class="tbAcompanhamentoTitulo">Valor Referente ao Contrato ou Aditivo de Valor</td>
            </tr>
            </thead>
                <tbody>
                <?php
                if (is_array($arrDadosMedicaoContrato) && !empty($arrDadosMedicaoContrato)) {
                    foreach ($arrDadosMedicaoContrato as $k => $dadosMedicaoContrato) {

                        $nomeContratoAditivo = $dadosMedicaoContrato["contrato"];
                        $valorContratoAditivo = $dadosMedicaoContrato["mecvlrreferencia"];
                        $valorContratoAditivo = number_format($valorContratoAditivo, 2, ",", ".");

                        echo "<tr>";
                        echo "<td>{$nomeContratoAditivo}</td>";
                        echo "<td>{$valorContratoAditivo}</td>";
                        echo "<tr>";
                    }
                } else {
                    echo "<div style='text-align: center'>Sem informações de contratos.</div>";
                }
                ?>
                </tbody>
        </table>
    </div>
    <!-- FIM DA TABELA DE ACOMPANHAMENTO. -->

    <br/>

</form>

<script type="text/javascript">

    //window.onunload = refreshParent;

    /**
     * Função responsável por atualizar a página com a lista de itens na tela de notas fiscais. Esta atualização é feita
     * logo após o cadastro de uma nota fiscal.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     */
    function refreshParent() {
        window.opener.location.reload();
    }

    /**
     * Função responsável pelo download do arquivo da nota.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param arqid
     */
    function downloadArquivo(arqid) {
        window.location = window.location.href + "&requisicao=download&arqid=" + arqid;
    }

</script>

</body>