<?php
/**
 * Verificação para exportação das cargas
 */
if ($_REQUEST['exportDifExcel']) {
    $cabecalho = [
        'UO',
        'Nome',
        'Despesas Empenhadas',
        'Despesas Empenhadas (dif)',
        'Depesas Liquidadas',
        'Depesas Liquidadas (dif)',
        'Despesas Liquidadas a Pagar',
        'Despesas Liquidadas a Pagar (dif)',
        'Despesas Pagas',
        'Despesas Pagas (dif)',
        'RAP Processados a Pagar',
        'RAP Processados a Pagar (dif)',
        'RAP Não Process. Liquidados a Pagar',
        'RAP Não Process. Liquidados a Pagar (dif)',
        'RAP Inscritos (Processados e Não Processados)',
        'RAP Inscritos (Processados e Não Processados) (dif)',
        'RAP Pagos (Processados e Não Processados)',
        'RAP Pagos (Processados e Não Processados)(dif)',
        'RAP a Pagar (Processados e Não Processados)',
        'RAP a Pagar (Processados e Não Processados) (dif)',
        'Valores Liquidados a Pagar (Exercício + RAP)',
        'Valores Liquidados a Pagar (Exercício + RAP) (dif)',
    ];
    $tesouroGerencial = new Spoemendas_Model_Tesourogerencial();
    $dados = $tesouroGerencial->getDadosProcessados($_REQUEST['exportDifExcel']);
    $tesouroGerencial->writeExcel($dados, $cabecalho, 'Processado Tesouro Gerencial');
} elseif ($_REQUEST['exportExcel']) {

    $cargaTesouro = new Spoemendas_Model_Cargatesourogerencial();
    $arqid = $cargaTesouro->getCargaByIdCarga($_REQUEST['exportExcel']);
    $file =  new FilesSimec();
    $file->getDownloadArquivo($arqid->arqid);
}
include_once APPRAIZ . 'includes/workflow.php';
/**
 * Objeto para flashmessage para apresentar informações
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/principal/emendas/listaremendas');
/**
 * Filtro da busca das cargas pela data e responsavel
 */
if ($_POST['tesourogerencial']) {
    
    $dataInicial = $_POST['tesourogerencial']['inicio'];
    $dataFinal = $_POST['tesourogerencial']['ate'];
    $responsavel = $_POST['tesourogerencial']['responsavel'];
    
    if (($dataInicial && $dataFinal) && $dataInicial > $dataFinal) {
        $fm->addMensagem("A data inicial não pode ser maior que a final!", 'danger');
    } else {
        if (!empty($dataInicial)) {
            $AND .= "
             AND TO_CHAR(dctdataprocessamento,'DD/MM/YYYY')::date >= '{$_POST['tesourogerencial']['inicio']}'";
        }
        if (!empty($dataFinal)) {
            $AND .= "
             AND TO_CHAR(dctdataprocessamento,'DD/MM/YYYY')::date <= '{$_POST['tesourogerencial']['ate']}'";
        }
        if (!empty($responsavel)) {
            $AND .= "
             AND usucpf = '{$_POST['tesourogerencial']['responsavel']}'";
        }
    }
}

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

$bc = new Simec_View_Breadcrumb('C');
$bc->add('Relatorio', '')
        ->add('Relatorio Carga Tesouro Gerencial')
        ->render();
?>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<link rel="stylesheet" href="/library/font-awesome/css/font-awesome.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
        $(function () {
            $('[data-toggle="tooltip"]').tooltip()
        });

        $('.tabela-listagem th').eq(0).html('Ação:');
    });
    /**
     * Detalhar a Emenda
     */
    function exportDifExcel(idcarga) {
        $(location).attr('href', 'spoemendas.php?modulo=relatorio/RelatorioCargaTesouroGerencial&acao=A&exportDifExcel=' + idcarga);
    }
    function exportExcel(idcarga) {
        $(location).attr('href', 'spoemendas.php?modulo=relatorio/RelatorioCargaTesouroGerencial&acao=A&exportExcel=' + idcarga);
    }

</script>
<style>
    * {
        white-space: normal !important;
    }
</style>
<?php
echo $fm->getMensagens();
$cargaTesouro = new Spoemendas_Model_Cargatesourogerencial();

$form = new Simec_View_Form('tesourogerencial', $method = Simec_View_Form::POST, $action = '');
$form->setTitulo('Filtro');
$form->addData('Período de', 'inicio');
$form->addData('Até', 'ate');
$responsavel = $cargaTesouro->responsavelCarga();
if ($responsavel) {
    $form->addCombo('Responsavel', 'responsavel', $responsavel, ['disabled' => 'disabled']);
}else{
    $form->addCombo('Responsavel', 'responsavel', ['disabled' => 'disabled']);
}
$form->addBotoes(['limpar', 'buscar']);
$form->render();


$arrColunas = [
    'Data da Carga',
    'Responsável'
];
$sql = <<<DML
          SELECT
                  DISTINCT dctidentcarga,
                  TO_CHAR(
                          dct.dctdataprocessamento,
                          'DD/MM/YYYY HH24:MI'
                  ),
                  usunome
          FROM
                  spoemendas.diffcargatesourogerencial dct
          INNER JOIN seguranca.usuario USING(usucpf)
        WHERE 1=1 {$AND}
          ORDER BY
                  dctidentcarga
                    
DML;
$listagem = new Simec_Listagem();
$listagem->addAcao('fileExcel', 'exportExcel');
$listagem->addAcao('stats', 'exportDifExcel');
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['valoremenda', 'valordetalhado', 'edevalordisponivel', 'empenhado', 'limite', 'saldo', 'liquidado', 'pago', 'impedimento']);
$listagem->setFormFiltros('tesouro');
$listagem->turnOnOrdenacao();
$listagem->setCabecalho($arrColunas)
        ->setQuery($sql);
$listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
?>
<script>
    $('#tesourogerencial_inicio').val(<?php echo "'" . $_POST['tesourogerencial']['inicio'] . "'" ?>);
    $('#tesourogerencial_ate').val(<?php echo "'" . $_POST['tesourogerencial']['ate'] . "'" ?>);
    $('#tesourogerencial_responsavel').val(<?php echo "'" . $_POST['tesourogerencial']['responsavel'] . "'" ?>);
    $('#tesourogerencial_inicio').mask('##/##/####');
    $('#tesourogerencial_ate').mask('##/##/####');
</script>