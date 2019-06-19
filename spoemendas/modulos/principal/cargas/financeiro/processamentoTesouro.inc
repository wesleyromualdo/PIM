<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 */
//include_once APPRAIZ . 'spoemendas/classes/model/CargaTesouroGerencial.inc';
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/financeiro');
$enderecoCarga = "spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A";

if (isset($_REQUEST['processar']) && $_REQUEST['processar']) {
    $cargaTesouro = new Spoemendas_Model_Cargatesourogerencial();
    $arqid = $cargaTesouro->getArqidByCarga($_GET['carga']);
    $cargaTesouro->saveDifTesouroGerencial($_GET['carga'],$arqid->arqid);
    $tesouroGerencial = new Spoemendas_Model_Tesourogerencial();
    $qtd = $tesouroGerencial->saveOrUpdateTesouroGerencial($_GET['carga']);
    $cargaTesouro->processado($_GET['carga']);
    if ($qtd > 0) {
        $fm->addMensagem($qtd . ' Registros processados com sucesso!');
    } else {
        $fm->addMensagem('Carga de dados não possui diferença com a base de dados!');
    }
    header("Location: spoemendas.php?modulo=principal/cargas/financeiro/apresentacaoProcessamentoTesouro&acao=A&carga=".$_GET['carga']);
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
.processar-carga{margin-bottom:10px}
</style>
<script type="text/javascript">
$(function(){
    $('.processar-carga').click(function(){
        bootbox.confirm('Tem certeza que quer processar essa carga? Ela acarretará modificações na base de dados.', function(){
            window.location.assign(window.location.href + '&processar=1');
        });
    });
});
</script>
<div class="col-lg-12">
<?php
(new Simec_View_Breadcrumb())
    ->add('Carga Tesouro Gerencial',$enderecoCarga)
    ->add('Processamento Tesouro Gerencial')
    ->render();

echo $fm;
?>
    <button class="btn btn-success processar-carga"><span class="glyphicon glyphicon-cog"></span> Iniciar processamento</button>
<?php
$query = "
       SELECT
            ctg.unicod,
            ctg.unidsc,
            SUM(ctgdespempenhadas) as ctgdespempenhadas, 
            SUM(ctgdespliquidas) as ctgdespliquidas, 
            SUM(ctgdespliquidadaspagar) as ctgdespliquidadaspagar,
            SUM(ctgdesppagas) as ctgdesppagas,
            SUM(ctgrapprocesspagar) as ctgrapprocesspagar,
            SUM(ctgrapnprocessliquipagar) as ctgrapnprocessliquipagar,
            SUM(ctgrapprocnproc) as ctgrapprocnproc,
            SUM(ctgpagprocnproc) as ctgpagprocnproc,
            SUM(ctgrappagprocnproc) as ctgrappagprocnproc,
            SUM(ctgvalliquidopagar) as ctgvalliquidopagar

        FROM spoemendas.cargatesourogerencial ctg
        WHERE ctgidentcarga = {$_GET['carga']}
        GROUP BY ctg.unidsc,ctg.unicod";
$colunas = 
        [
            'UO',
            'Unidade Orçamentaria',
            'Despesas Empenhadas', 
            'Despesas Liquidadas', 
            'Despesas Liquidadas a Pagar', 
            'Despesas Pagas',
            'RAP Processados a Pagar',
            'RAP Não Process. Liquidados a Pagar',
            'RAP Inscritos (Processados e Não Processados)',
            'RAP Pagos (Processados e Não Processados)',
            'RAP a Pagar (Processados e Não Processados)',
            'Valores Liquidados a Pagar (Exercício + RAP)',
          ];
$totalColunas = [
    'ctgdespempenhadas',
    'ctgdespliquidas',
    'ctgdespliquidadaspagar',
    'ctgdesppagas',
    'ctgrapprocesspagar',
    'ctgrapnprocessliquipagar',
    'ctgrapprocnproc',
    'ctgpagprocnproc',
    'ctgrappagprocnproc',
    'ctgvalliquidopagar',
    ];
(new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
    ->setQuery($query)
    ->setCabecalho($colunas)
        ->addCallbackDeCampo([
        'ctgdespempenhadas',
        'ctgdespliquidas',
        'ctgdespliquidadaspagar',
        'ctgdesppagas',
        'ctgrapprocesspagar',
        'ctgrapnprocessliquipagar',
        'ctgrapprocnproc',
        'ctgpagprocnproc',
        'ctgrappagprocnproc',
        'ctgvalliquidopagar',
        ], 'mascaraMoeda')
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $totalColunas)
    ->turnOnPesquisator()
    ->render();
?>
</div>