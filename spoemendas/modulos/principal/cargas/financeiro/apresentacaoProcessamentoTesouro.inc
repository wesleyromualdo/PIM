<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 */
//include_once APPRAIZ . 'spoemendas/classes/model/CargaTesouroGerencial.inc';
$enderecoCarga = 'spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A';
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/financeiro');
if (isset($_REQUEST['export']) && $_REQUEST['export']) {
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
    $Tesouro = new Spoemendas_Model_Tesourogerencial();
    $dados = $Tesouro->getDadosProcessados($_GET['carga']);
    $Tesouro->writeExcel($dados,$cabecalho,'Resumo do Processamento');
}
include APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
.processar-carga{margin-bottom:10px}
</style>
<script type="text/javascript">
</script>
<div class="col-lg-12">
<?php
(new Simec_View_Breadcrumb())
    ->add('Carga Tesouro Gerencial',$enderecoCarga)
    ->add('Processamento Tesouro Gerencial', $enderecoProcessamento)
    ->add('Apresentação do Processameto Tesouro Gerencial')
    ->render();

echo $fm->getMensagens();
$Tesouro = new Spoemendas_Model_Tesourogerencial();
$dctid = $Tesouro->verificaQtdDiffTesouro($_GET['carga']);
?>
    <form class="form-inline" method="post" name="export">
        <div class="form-group">
            <a href="spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A" class="btn btn-primary processar-carga"><span class="glyphicon glyphicon-new-window"></span> Nova Carga</a>
        </div>
        <?php if($dctid){ ?>
        <div class="form-group">
            <input type="hidden" name="export" value="true">
            <button type="submit" class="btn btn-primary processar-carga"><span class="glyphicon glyphicon-download"></span> Exportar Excel</button>
        </div>
        <?php } ?>
    </form>
 <?php
$query = "
         SELECT 
 	 dct.unicod,
 	 unidsc,
         SUM(case when dcttipoddl = 'I' then 1 else 0 end) dctqtdinseridas,
         SUM(case when dcttipoddl = 'U' then 1 else 0 end)dctqtdatualizados, 
	 SUM(dctdespempenhadas)::text ||' | '|| SUM(dctdespempenhadasdif)::text as dctdespempenhadas,          
         SUM(dctdespliquidas)::text ||' | '||  SUM(dctdespliquidasdif)::text as dctdespliquidas,           
         SUM(dctdespliquidadaspagar)::text ||' | '|| SUM(dctdespliquidadaspagardif)::text as dctdespliquidadaspagar,     
         SUM(dctdesppagas)::text ||' | '|| SUM(dctdesppagasdif)::text as dctdesppagas,              
         SUM(dctrapprocesspagar)::text ||' | '|| SUM(dctrapprocesspagardif)::text as dctrapprocesspagar,         
         SUM(dctrapnprocessliquipagar)::text ||' | '|| SUM(dctrapnprocessliquipagardif)::text as dctrapnprocessliquipagar  ,  
         SUM(dctrapprocnproc)::text ||' | '|| SUM(dctrapprocnprocdif)::text as dctrapprocnproc,          
         SUM(dctpagprocnproc)::text ||' | '|| SUM(dctpagprocnprocdif)::text as dctpagprocnproc,            
         SUM(dctrappagprocnproc)::text ||' | '||  SUM(dctrappagprocnprocdif)::text as dctrappagprocnproc,         
         SUM(dctvalliquidopagar)::text ||' | '|| SUM(dctvalliquidopagardif)::text  as dctvalliquidopagar
        FROM spoemendas.diffcargatesourogerencial dct
        WHERE dctidentcarga = {$_GET['carga']}
        GROUP BY 
        dct.unicod,
        unidsc
        ";
$colunas = 
        [
            'UO',
            'Unidade Orçamentaria',
            'Registros Inseridos',
            'Registros Atualizados',
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

(new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
    ->setQuery($query)
    ->setCabecalho($colunas)
        ->addCallbackDeCampo([
         'dctdespempenhadas',  
         'dctdespliquidas',     
         'dctdespliquidadaspagar'  , 
         'dctdesppagas',    
         'dctrapprocesspagar' ,      
         'dctrapnprocessliquipagar',     
         'dctrapprocnproc',
         'dctpagprocnproc',
         'dctrappagprocnproc',
         'dctvalliquidopagar', 
         
        ], 'apresentacaoDiferencaValores')
        ->addCallbackDeCampo('unidesc', 'formataNomeUnidade')
//    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $totalColunas)
    ->turnOnPesquisator()
    ->render();
?>
</div>