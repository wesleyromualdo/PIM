<?php
header("content-type: text/html; charset=ISO-8859-1");

include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/library/simec/BasicWS.php";
include_once APPRAIZ . "includes/library/vendor/phpoffice/phpexcel/Classes/PHPExcel.php";
include_once APPRAIZ . "siconv/Siconv.php";
include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";


$usucpf = $_SESSION['usucpf'];
$arrCpf = array();

if (!in_array($usucpf, $arrCpf)) {

    echo "<script>
            alert('Acesso negado!');
            window.location.href = '/obras2/obras2.php?modulo=principal/listaObras&acao=A';
        </script>";
    exit;
}


/* Requisição para realizar a carga no banco. */
if ($_POST['salvar'] == '1') {
    ob_clean();

    global $db;

    $obridVinculante = $_POST['obridVinculante'];
    $obridNovo = $_POST['obridNovo'];
    $tipo = $_POST['tipoCarga'];

    if ($obridVinculante == "") {
        echo "<script>
            alert('É necessário informar o ID da Obra Vinculante.');
                window.location.href = window.location.href;
        </script>";
        exit;
    }
    if ($obridNovo == "") {
        echo "<script>
            alert('É necessário informar o ID da Nova Obra.');
                window.location.href = window.location.href;
        </script>";
        exit;
    }

    if (!verificaVinculoObra($obridVinculante, $obridNovo)) {
        echo "<script>
            alert('As obras informadas não possuem vinculo.');
                window.location.href = window.location.href;
        </script>";
        exit;
    }


    // Migra os registros de validação
    $sql = "SELECT * FROM obras2.validacao WHERE obrid = {$obridVinculante}";
    $val = $db->pegaLinha($sql);
    if ($val) {
        $val['vldid'] = null;
        $val['obrid'] = $obridNovo;
        $validação = new Validacao();
        $validação->popularDadosObjeto($val);
        $validação->salvar();
    }

    // Migra os registros de atividades
    $sql = "SELECT * FROM obras2.registroatividade WHERE obrid = {$obridVinculante}";
    $ras = $db->carregar($sql);

    if ($ras) {
        foreach ($ras as $ra) {
            $registro = new RegistroAtividade();
            $ra['rgaid'] = null;
            $ra['obrid'] = $obridNovo;
            $registro->popularDadosObjeto($ra);
            $registro->salvar();
        }
    }

    // Migra os registros de restrições e inconformidades
    $sql = "SELECT * FROM obras2.restricao WHERE obrid = {$obridVinculante}";
    $res = $db->carregar($sql);

    if ($res) {
        foreach ($res as $re) {
            $restricao = new Restricao();
            $re['rstid'] = null;
            $re['obrid'] = $obridNovo;
            $restricao->popularDadosObjeto($re);
            $restricao->salvar();
        }
    }

    $sql = "SELECT * FROM obras2.filarestricao WHERE obrid = {$obridVinculante}";
    $fres = $db->carregar($sql);

    if ($fres) {
        foreach ($fres as $fre) {
            $filaRestricao = new FilaRestricao();
            $fre['frtid'] = null;
            $fre['obrid'] = $obridNovo;
            $filaRestricao->popularDadosObjeto($fre);
            $filaRestricao->salvar();
        }
    }

    $sql = "SELECT * FROM obras2.solicitacao_vinculada WHERE obrid = {$obridVinculante}";
    $slvs = $db->carregar($sql);

    if ($slvs) {
        foreach ($slvs as $slv) {
            $s = new SolicitacaoVinculada($slv['slvid']);
            $s->obrid = $obridNovo;
            $s->salvar();
        }
    }

    if($tipo == 1) {
        $idObraTmp = '9' . $obridNovo;

        $sql_update_id = "
            -- Troca os IDs
            UPDATE obras2.obras SET obrid = $idObraTmp, obrstatus = 'P' WHERE obrid =  $obridVinculante;
            UPDATE obras2.obras SET obrid = $obridVinculante, obrstatus  = 'A' WHERE obrid = $obridNovo;
            UPDATE obras2.obras SET obrid = $obridNovo WHERE obrid = $idObraTmp;

            UPDATE obras2.obras SET obridvinculado = $obridVinculante WHERE obrid =  $obridNovo;
        ";

        $db->executar($sql_update_id);
    }

    try {
        $db->commit();
    } catch (Exception $ex) {
        $db->rollback();
        echo "<script>
            alert('Erro ao relizar copia dos dados!');
                window.location.href = window.location.href;
        </script>";
        exit;
    }


    // Migra os Documentos
    $sql = " SELECT oa.*, a.*
             FROM
                    obras2.obras_arquivos oa
             JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
             JOIN public.arquivo      a ON a.arqid = oa.arqid
             WHERE oa.oarstatus = 'A'
               AND oa.obrid = {$obridVinculante}
               AND (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')";

    $docs_originais = $db->carregar($sql);

    if (!empty($docs_originais)) {
        $erro = false;
        foreach ($docs_originais as $key => $value) {
            //obras2.obras_arquivos
            $oardata = !empty($value['oardata']) ? "'" . $value['oardata'] . "'" : 'NULL';
            $oardtinclusao = !empty($value['oardtinclusao']) ? "'" . $value['oardtinclusao'] . "'" : 'NULL';
            $sql_ioa = "INSERT INTO obras2.obras_arquivos( obrid, tpaid, arqid, oardesc, oardata, oardtinclusao, oarstatus)
                        VALUES ( $obridNovo, " . $value['tpaid'] . ", {$value['arqid']}, '" . addslashes($value['oardesc']) . "', $oardata, $oardtinclusao, '" . $value['oarstatus'] . "' )RETURNING oarid;";
            try {
                $oarid = $db->pegaUm($sql_ioa);
            } catch (Exception $ex) {
                $erro = true;
                break;
            }
        }
        if (!$erro) {
            $db->commit();
            echo "<script>
            alert('Operação realizada com sucesso!');
                window.location.href = window.location.href;
        </script>";
            exit;
        } else {
            $db->rollback();
            echo "<script>
            alert('Não foi possivel realizar a carga dos arquivos.');
                window.location.href = window.location.href;
        </script>";
            exit;
        }
    } else {
        echo "<script>
            alert('Operação realizada com sucesso!');
                window.location.href = window.location.href;
        </script>";
        exit;
    }

    exit;
}

function verificaVinculoObra($obridVinculante, $obridNovo)
{
    global $db;

    $sql = "SELECT obrid FROM obras2.obras WHERE obrid = {$obridNovo} AND obridvinculado = {$obridVinculante}";

    $resultado = $db->pegaUm($sql);

    if ($resultado) {
        return true;
    } else {
        return false;
    }
}

?>

<style type="text/css">
    .labelFiltro {
        font-weight: bold;
        font-size: 13px;
    }

    .labelCheckbox {
        font-size: 14px;
    }

    .divisor {
        border: 1px solid #DCDCDC;
    }

    #cabecalhoInfoRelatorio {
        text-align: left;
        font-size: 13px;
        line-height: 170%;
    }

    #cabecalhoBotoesRelatorio {
        text-align: right;
        font-size: 13px;
        line-height: 170%;
    }

    #relatorio, #HTMLtoPDF {
        width: 95%;
        text-align: center;
        margin: 0 auto;
    }

    /* O arquivo bootstrap.min-simec.css é utilizado no layout antigo. Nesse arquivo estão comentadas as classes que o
     * Bootstrap utiliza para tabelas. Portanto, para utilizar a formatação de tabelas do bootstrap é necessário o trecho
     * a seguir.
     */
    table {
        border-collapse: collapse;
        border-spacing: 0;
    }

    th {
        text-align: left;
    }

    .table {
        width: 100%;
        margin-bottom: 20px;
    }

    .table thead > tr > th, .table tbody > tr > th, .table tfoot > tr > th, .table thead > tr > td, .table tbody > tr > td, .table tfoot > tr > td {
        padding: 8px;
        line-height: 1.428571429;
        vertical-align: top;
        border-top: 1px solid #dddddd;
    }

    .table thead > tr > th {
        vertical-align: bottom;
        border-bottom: 2px solid #dddddd;
    }

    .table caption + thead tr:first-child th, .table colgroup + thead tr:first-child th, .table thead:first-child tr:first-child th, .table caption + thead tr:first-child td, .table colgroup + thead tr:first-child td, .table thead:first-child tr:first-child td {
        border-top: 0;
    }

    .table tbody + tbody {
        border-top: 2px solid #dddddd;
    }

    .table .table {
        background-color: #ffffff;
    }

    .table-condensed thead > tr > th, .table-condensed tbody > tr > th, .table-condensed tfoot > tr > th, .table-condensed thead > tr > td, .table-condensed tbody > tr > td, .table-condensed tfoot > tr > td {
        padding: 5px;
    }

    .table-bordered {
        border: 1px solid #dddddd;
    }

    .table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
        border: 1px solid #dddddd;
    }

    .table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
        border-bottom-width: 2px;
    }

    .table-striped > tbody > tr:nth-child(odd) > td, .table-striped > tbody > tr:nth-child(odd) > th {
        background-color: #f9f9f9;
    }

    .table-hover > tbody > tr:hover > td, .table-hover > tbody > tr:hover > th {
        background-color: #f5f5f5;
    }

    table col[class*="col-"] {
        float: none;
        display: table-column;
    }

    table td[class*="col-"], table th[class*="col-"] {
        float: none;
        display: table-cell;
    }

    .table > thead > tr > td.active, .table > tbody > tr > td.active, .table > tfoot > tr > td.active, .table > thead > tr > th.active, .table > tbody > tr > th.active, .table > tfoot > tr > th.active, .table > thead > tr.active > td, .table > tbody > tr.active > td, .table > tfoot > tr.active > td, .table > thead > tr.active > th, .table > tbody > tr.active > th, .table > tfoot > tr.active > th {
        background-color: #f5f5f5;
    }

    .table > thead > tr > td.success, .table > tbody > tr > td.success, .table > tfoot > tr > td.success, .table > thead > tr > th.success, .table > tbody > tr > th.success, .table > tfoot > tr > th.success, .table > thead > tr.success > td, .table > tbody > tr.success > td, .table > tfoot > tr.success > td, .table > thead > tr.success > th, .table > tbody > tr.success > th, .table > tfoot > tr.success > th {
        background-color: #468847;
        border-color: #46773d;
    }

    .table-hover > tbody > tr > td.success:hover, .table-hover > tbody > tr > th.success:hover, .table-hover > tbody > tr.success:hover > td {
        background-color: #3d773e;
        border-color: #3c6635;
    }

    .table > thead > tr > td.danger, .table > tbody > tr > td.danger, .table > tfoot > tr > td.danger, .table > thead > tr > th.danger, .table > tbody > tr > th.danger, .table > tfoot > tr > th.danger, .table > thead > tr.danger > td, .table > tbody > tr.danger > td, .table > tfoot > tr.danger > td, .table > thead > tr.danger > th, .table > tbody > tr.danger > th, .table > tfoot > tr.danger > th {
        background-color: #b94a48;
        border-color: #af4353;
    }

    .table-hover > tbody > tr > td.danger:hover, .table-hover > tbody > tr > th.danger:hover, .table-hover > tbody > tr.danger:hover > td {
        background-color: #a74240;
        border-color: #9c3c4a;
    }

    .table > thead > tr > td.warning, .table > tbody > tr > td.warning, .table > tfoot > tr > td.warning, .table > thead > tr > th.warning, .table > tbody > tr > th.warning, .table > tfoot > tr > th.warning, .table > thead > tr.warning > td, .table > tbody > tr.warning > td, .table > tfoot > tr.warning > td, .table > thead > tr.warning > th, .table > tbody > tr.warning > th, .table > tfoot > tr.warning > th {
        background-color: #c09853;
        border-color: #bc7e48;
    }

    .table-hover > tbody > tr > td.warning:hover, .table-hover > tbody > tr > th.warning:hover, .table-hover > tbody > tr.warning:hover > td {
        background-color: #b78c43;
        border-color: #ab713f;
    }

    .table > thead > tr > td {
        text-align: center;
        font-weight: bold;
        vertical-align: middle;
        font-size: 13px;
    }

    .table > tbody > tr > td {
        vertical-align: middle;
        text-align: center;
        font-size: 13px;
    }
</style>

<input type="hidden" name="pagina" id="pagina" value=""/>

<div id="filtros">

    <?php monta_titulo("Popula Dados da Obra Vinculada", ""); ?>

    <form name=formulario id=formulario method=post enctype="multipart/form-data">
        <input type="hidden" name="salvar" value="0">
        <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
               align="center">
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">ID Obra Vinculante:
                </td>
                <td width='50%'>
                    <input
                            type="text"
                            name="obridVinculante"
                            id="obridVinculante"
                            value=""
                            onKeyUp="this.value=mascaraglobal('##########',this.value);"
                            class=""
                            style="text-align: left; width: 100px"
                            title="Obrid da obra vinculante."
                    />
                    <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">ID Nova Obra:</td>
                <td width='50%'>
                    <input
                            type="text"
                            name="obridNovo"
                            id="obridNovo"
                            value=""
                            onKeyUp="this.value=mascaraglobal('##########',this.value);"
                            class=""
                            style="text-align: left; width: 100px"
                            title="Obrid da nova obra."
                    />
                    <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                </td>
            </tr>
                        <tr>
                            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo de Migração:</td>
                            <td width='50%'>
                                <select id="tipoCarga" name="tipoCarga">
                                    <option value="1">Completa (Dados + ID)</option>
                                    <option value="2">Somente Dados</option>
                                </select>
                            </td>
                        </tr>
        </table>
        <div class="row" style="text-align:center">
            <div class="col-md-12">
                <button class="btn btn-default" onclick="enviar()" id="btnPesquisar">Popular Dados</button>
            </div>
        </div>
    </form>

</div>


<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../par/js/jspdf.js"></script>
<script type="text/javascript" src="../par/js/pdfFromHTML.js"></script>
<script type="text/javascript">

    function refreshParent() {
        window.opener.location.reload();
    }

    function enviar() {

        if ($("#obridVinculante").val().trim() == "") {
            alert('É necessário informar o ID da Obra Vinculante.');
            return false;
        }
        if ($("#obridNovo").val().trim() == "") {
            alert('É necessário informar o ID da Nova Obra.');
            return false;
        }

        document.formulario.salvar.value = 1;
        document.formulario.tipo.value = carga;
        window.onunload = refreshParent;
        document.formulario.submit();
    }


</script>