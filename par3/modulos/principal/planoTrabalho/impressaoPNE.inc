<?php
ob_start();
//require APPRAIZ . 'includes/cabecalho.inc';
require 'pne/funcoes.php';

#Itrid
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$inuid = (int) $_REQUEST['inuid'];
$itrid = $controleUnidade->pegarItrid($inuid);
?>
<style>
    td,th{padding:0}
    table {
        border-spacing:0;
        border-collapse:collapse;
        font-size: 9pt; 
        width: 100%; 
        font-family: sans-serif;
    }
    th {
        text-align: left;
    }
    .table {
        width: 100%;
        max-width: 100%;
        margin-bottom: 20px;
    }
    .table > thead > tr > th,
    .table > tbody > tr > th,
    .table > tfoot > tr > th,
    .table > thead > tr > td,
    .table > tbody > tr > td,
    .table > tfoot > tr > td {
        padding: 8px;
        line-height: 1.42857143;
        vertical-align: top;
        border-top: 1px solid #ddd;
    }
    .table > thead > tr > th {
        vertical-align: bottom;
        border-bottom: 2px solid #ddd;
    }
    .table > caption + thead > tr:first-child > th,
    .table > colgroup + thead > tr:first-child > th,
    .table > thead:first-child > tr:first-child > th,
    .table > caption + thead > tr:first-child > td,
    .table > colgroup + thead > tr:first-child > td,
    .table > thead:first-child > tr:first-child > td {
        border-top: 0;
    }
    .table > tbody + tbody {
        border-top: 2px solid #ddd;
    }
    .table .table {
        background-color: #fff;
    }
    .table-condensed > thead > tr > th,
    .table-condensed > tbody > tr > th,
    .table-condensed > tfoot > tr > th,
    .table-condensed > thead > tr > td,
    .table-condensed > tbody > tr > td,
    .table-condensed > tfoot > tr > td {
        padding: 5px;
    }
    .table-bordered {
        border: 1px solid #ddd;
    }
    .table-bordered > thead > tr > th,
    .table-bordered > tbody > tr > th,
    .table-bordered > tfoot > tr > th,
    .table-bordered > thead > tr > td,
    .table-bordered > tbody > tr > td,
    .table-bordered > tfoot > tr > td {
        border: 1px solid #ddd;
    }
    .table-striped > tbody > tr:nth-of-type(odd) {
        background-color: #f9f9f9;
    }
    table col[class*="col-"] {
        position: static;
        display: table-column;
        float: none;
    }
    table td[class*="col-"],
    table th[class*="col-"] {
        position: static;
        display: table-cell;
        float: none;
    }
    .table > thead > tr > td.active,
    .table > tbody > tr > td.active,
    .table > tfoot > tr > td.active,
    .table > thead > tr > th.active,
    .table > tbody > tr > th.active,
    .table > tfoot > tr > th.active,
    .table > thead > tr.active > td,
    .table > tbody > tr.active > td,
    .table > tfoot > tr.active > td,
    .table > thead > tr.active > th,
    .table > tbody > tr.active > th,
    .table > tfoot > tr.active > th {
        background-color: #f5f5f5;
    }
    .table > thead > tr > td.success,
    .table > tbody > tr > td.success,
    .table > tfoot > tr > td.success,
    .table > thead > tr > th.success,
    .table > tbody > tr > th.success,
    .table > tfoot > tr > th.success,
    .table > thead > tr.success > td,
    .table > tbody > tr.success > td,
    .table > tfoot > tr.success > td,
    .table > thead > tr.success > th,
    .table > tbody > tr.success > th,
    .table > tfoot > tr.success > th {
        background-color: #dff0d8;
    }
    .table > thead > tr > td.info,
    .table > tbody > tr > td.info,
    .table > tfoot > tr > td.info,
    .table > thead > tr > th.info,
    .table > tbody > tr > th.info,
    .table > tfoot > tr > th.info,
    .table > thead > tr.info > td,
    .table > tbody > tr.info > td,
    .table > tfoot > tr.info > td,
    .table > thead > tr.info > th,
    .table > tbody > tr.info > th,
    .table > tfoot > tr.info > th {
        background-color: #d9edf7;
    }
    .table > thead > tr > td.warning,
    .table > tbody > tr > td.warning,
    .table > tfoot > tr > td.warning,
    .table > thead > tr > th.warning,
    .table > tbody > tr > th.warning,
    .table > tfoot > tr > th.warning,
    .table > thead > tr.warning > td,
    .table > tbody > tr.warning > td,
    .table > tfoot > tr.warning > td,
    .table > thead > tr.warning > th,
    .table > tbody > tr.warning > th,
    .table > tfoot > tr.warning > th {
        background-color: #fcf8e3;
    }
    .table > thead > tr > td.danger,
    .table > tbody > tr > td.danger,
    .table > tfoot > tr > td.danger,
    .table > thead > tr > th.danger,
    .table > tbody > tr > th.danger,
    .table > tfoot > tr > th.danger,
    .table > thead > tr.danger > td,
    .table > tbody > tr.danger > td,
    .table > tfoot > tr.danger > td,
    .table > thead > tr.danger > th,
    .table > tbody > tr.danger > th,
    .table > tfoot > tr.danger > th {
        background-color: #f2dede;
    }
    .tituloMeta{
         width: 100px; text-align: center; background-color: #18A689; color: #FFF; font-weight: bold
    }
</style>
<div class="space-30"></div>
<table class="table table-bordered">
    <thead>
        <tr>
            <th colspan="2" style="border-top: 1px solid #ddd; font-size: 18px; font-weight: normal;">
                Metas PNE: <?php echo $controleUnidade->pegarNomeEntidade($inuid); ?>
            </th>
        </tr>
    </thead>
    <tbody>
        <?php
        #@TODO verificar as classes - refatorar utilizar Classes Metas.class.inc no Sase
        $sql = "select metid, mettitulo from sase.meta where metstatus = 'A' order by metid";
        $arrMetas = $db->carregar($sql);

        foreach ($arrMetas as $meta):
            ?>
            <tr>
                <td class="tituloMeta" style="vertical-align: middle;">
                    <?php echo 'Meta ' . $meta['metid'] ?>
                </td>
                <td>
                    <table class="table table-bordered">
                        <thead>
                            <tr>
                                <th colspan="2" style="width: 100%;border-top: 1px solid #ddd">
                                    <?php echo $meta['mettitulo']; ?>
                                </th>
                            </tr>
                            <tr>
                                <th style="width: 70%;border-top: 1px solid #ddd; color: #333;">Indicador</th>
                                <th style="width: 30%;border-top: 1px solid #ddd; color: #333;">Valores</th>
                            </tr>
                        </thead>
                        <tbody>
                            <?php
                            $arrIndicador = carregarMetas($meta['metid'], $itrid);
                            $muncod = $_SESSION['par']['muncod'];
                            $estuf = $_SESSION['par']['estuf'];
                            $valor = ($meta['metid'] == 11 || $meta['metid'] == 14) ? '0' : '1';

                            foreach ($arrIndicador as $indicador) {
                                $dadosResultado = array();
                                if ($itrid == 1 || $itrid == 3) {
                                    $pneanoUf = '';
                                    if (!in_array($meta['metid'], array(19, 20)) && !empty($indicador['subiddial'])) {
                                        $pneanoUf = selecionaAno($indicador['subiddial'], 'E');
                                    }
                                    $dadosResultado = retornaDadosEst($valor, $indicador['subiddial'], $meta['metid'], $estuf, $pneanoUf, $itrid);
                                } else {
                                    $pneanoMunicipio = '';
                                    if (!in_array($meta['metid'], array(13, 14, 17, 19, 20))) {
                                        $pneanoMunicipio = selecionaAno($indicador['subiddial'], 'M');
                                    }
                                    $dadosResultado = retornaDadosMun($valor, $indicador['subiddial'], $meta['metid'], $muncod, $pneanoMunicipio, $itrid);
                                }

                                if (!verificaMetas($meta['metid'])) {
                                    ?>   
                                    <tr>
                                        <td><?php echo $indicador['subtitulo'] ?></td>
                                        <td>
                                            <strong>Meta <?php echo ($itrid == 1) ? 'Estadual' : 'Municipal' ?>:</strong> <?php echo (!empty($dadosResultado['pnevalormeta'])) ? $dadosResultado['pnevalormeta'] : '0' ?> %<br />
                                            <strong>Ano Previsto:</strong> <?php echo (!empty($dadosResultado['pneanoprevisto'] || $dadosResultado['pneanoprevisto'] != '0')) ? $dadosResultado['pneanoprevisto'] : '-' ?>
                                        </td>
                                    </tr>
                                    <?php
                                } else if ($meta['metid'] == 18) {

                                    $dados = retornaDadosPne($valor, $meta['metid'], $indicador['subiddial'], '', $itrid, '');
                                    ?>
                                    <tr>
                                        <td style="width: 70%"><?php echo $indicador['subtitulo'];
                                    ?></td>
                                        <td style="width: 30%">
                                            <strong><?php echo ($dados[0]['pnepossuiplanoremvigente'] == 'f' || $dados[0]['pnepossuiplanoremvigente'] == '') ? 'NÃO' : 'SIM' ?></strong><br />
                                            <?php if ($dados[0]['pnepossuiplanoremvigente'] == 'f' || $dados[0]['pnepossuiplanoremvigente'] == '') { ?>
                                                <strong>Ano Previsto:</strong> <?php echo (!empty($dados[0]['pneanoprevisto'] || $dados[0]['pneanoprevisto'] != '0')) ? $dados[0]['pneanoprevisto'] : '-' ?>
                                            <?php } ?>
                                        </td>
                                    </tr>
                                    <?php if ($dados[0]['pnepossuiplanoremvigente'] != 'f' && $dados[0]['pnepossuiplanoremvigente'] != '') { ?>
                                        <tr>
                                            <td style="width: 70%">Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18? </td>
                                            <td style="width: 30%">
                                                <strong><?php echo ($dados[0]['pneplanorefcaput'] != 'f' && $dados[0]['pneplanorefcaput'] != '') ? 'SIM' : 'NÃO' ?></strong><br />
                                                <?php if ($dados[0]['pneplanorefcaput'] == 'f' || $dados[0]['pneplanorefcaput'] == '') { ?>
                                                    <strong>Ano Previsto:</strong> <?php echo (!empty($dados[0]['pneanoprevisto'] || $dados[0]['pneanoprevisto'] != '0')) ? $dados[0]['pneanoprevisto'] : '-' ?>
                                                <?php } ?>
                                            </td>
                                        </tr>
                                        <?php
                                    }
                                } else {
                                    $dados = retornaDadosPne($valor, $meta['metid'], $indicador['subiddial'], $pneano, $itrid, '');
                                    ?>
                                    <tr>
                                        <td style="width: 50%">
                                            <?php echo $indicador['subtitulo'] ?>
                                        </td>
                                        <td style="width: 50%">
                                            <?php echo $dados[0]['micinfcomplementar'] ?>
                                        </td>
                                    </tr>
                                    <?php
                                }
                            }
                            ?>
                        </tbody>
                    </table>
                </td>
            </tr>
        <?php endforeach; ?>
    </tbody>
    <tfoot>
        <tr>
            <td colspan="2">
                Gerado por <?php echo $_SESSION['usunome'] ?> em : <?php echo date('d/m/Y G:i:s') ?>
            </td>
        </tr>
    </tfoot>
</table>
<?php
$content = ob_get_contents();
ob_end_clean();
html2Pdf($content);
/*
function html2Pdf($content) {
    // -- Preparando a requisição ao webservice de conversão de HTML para PDF do MEC.
    $content = http_build_query(
            array('conteudoHtml' => ($content))
    );

    $context = stream_context_create(
            array(
                'http' => array(
                    'method' => 'POST',
                    'content' => $content
                )
            )
    );

    // -- Fazendo a requisição de conversão
    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);

    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=relatorio-pne.pdf");
    echo $contents;
    exit();
}
*/
exit;
