<link rel="stylesheet" type="text/css" href="\library\font-awesome\css\font-awesome.min.css" media="all">

<style>
    #div-central {
        margin-bottom: 20px;
    }

    .btn-equipe-tecnica {
        background-color: #fcd230;
        border-color: #edc32d;
        white-space: pre-wrap;
        color: #000000;
    }

    .btn-equipe-tecnica:hover,
    .btn-equipe-tecnica:active,
    .btn-equipe-tecnica:target,
    .btn-equipe-tecnica:focus {
        background-color: #edc32d;
        border-color: #edc32d;
        color: #000000;
    }

    .botao-inicio .btn-equipe-tecnica {
        height: 90px;
    }

    .btn-group .btn-equipe-tecnica {
        height: 90px;
    }

    .btn-paragraph {
        margin-top: -20px;
    }

    .btn-paragraph .btn-paragraph-number {
        font-weight: bold;
        font-size: 20px;
    }

    #lado-direito > .well,
    #div-graficos > .well,
    #div-central.well,
    #botoes-superior.well {
        background-color: white;
    }

    div.zabuto_calendar .table tr td div.day {
        padding-top: 4px !important;
        padding-bottom: 4px !important;
    }
</style>

<?php
$solicitacaoPedido = new Spoemendas_Model_Solicitacaofinanceirapedido();
$solicitacao = new Spoemendas_Model_Solicitacaofinanceira();

//$totalSolicitacaoPedidos = $solicitacaoPedido->retornaTotalPedidos();
//$totalSolicitacaoPedidosNaoEnviados = $solicitacaoPedido->retornaTotalPedidos(null, ESD_NAO_ENVIADO);
//$totalSolicitacaoAutorizadas = $solicitacaoPedido->retornaTotalPedidos(Spoemendas_Model_Solicitacaofinanceirasituacao::AUTORIZADO, ESD_ENVIADO);

$qtd = $solicitacao->carregar($solicitacao->queryDeSolicitacoes(null));
$totalSolicitacaoPedidos = is_array($qtd) ? count($qtd) : 0;

$qtd = $solicitacao->carregar($solicitacao->queryDeSolicitacoes(['enviado' => ESD_NAO_ENVIADO]));
$totalSolicitacaoPedidosNaoEnviados = is_array($qtd) ? count($qtd) : 0;

$qtd = $solicitacao->carregar($solicitacao->queryDeSolicitacoes(['situacaopedido' => 2]));
$totalSolicitacaoAutorizadas = is_array($qtd) ? count($qtd) : 0;
?>

<div class="row" style="background-color: #f5f5f5;">
    <br>
    <div class="col-sm-12">
        <input type="hidden" id="mescorrente" value="<?= date('m') ?>">

        <div id="div-central">
            <div class="col-sm-12">
                <div id="botoes-superior" class="col-sm-12 well">
                    <div class="col-sm-3 botao-inicio">
                        <button type="button"
                                class="btn btn-success btn-block btn-equipe-tecnica"
                                onclick="openPage('spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A')">
                            <p class="btn-paragraph">Solicitações de Financeiro<br><span class="btn-paragraph-number">- <?= $totalSolicitacaoPedidos; ?> -</span></p>
                        </button>
                        <div style="clear:both;"></div>
                    </div>
                    <div class="col-sm-3 botao-inicio">
                        <button type="button"
                                class="btn btn-success btn-block btn-equipe-tecnica"
                                onclick="openPage('spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A&redirect=dashboard&esdid=<?= ESD_NAO_ENVIADO; ?>')">
                            <p class="btn-paragraph">Solicitações Não Enviadas<br><span class="btn-paragraph-number">- <?= $totalSolicitacaoPedidosNaoEnviados; ?> -</span></p>
                        </button>
                        <div style="clear:both;"></div>
                    </div>
                    <div class="col-sm-3 botao-inicio">
                        <button type="button"
                                class="btn btn-success btn-block btn-equipe-tecnica"
                                onclick="openPage('spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A&redirect=dashboard&esdid=<?= ESD_ENVIADO; ?>&situacao=<?= Spoemendas_Model_Solicitacaofinanceirasituacao::AUTORIZADO; ?>')">
                            <p class="btn-paragraph">Solicitações Autorizadas<br><span class="btn-paragraph-number">- <?= $totalSolicitacaoAutorizadas; ?> -</span></p>
                        </button>
                        <div style="clear:both;"></div>
                    </div>
                    <div class="col-sm-3 botao-inicio">
                        <button type="button"
                                class="btn btn-success btn-block btn-equipe-tecnica"
                                onclick="openPage('spoemendas.php?modulo=relatorio/extratodinamicoFinanceiro&acao=A')">
                            <p class="btn-paragraph">Relatório<br>Dinâmico</p>
                        </button>
                        <div style="clear:both;"></div>
                    </div>
                    <div style="clear:both;"></div>
                </div>
            </div>
        </div>

        <div id="div-body" class="row">
            <div id="div-graficos" class="col-sm-8">
                <div id="div-grafico-solicitacoes-mensal" class="well row form-group box-white">
                </div>

                <div id="div-grafico-solicitacoes-anual" class="well row box-white">
                </div>
            </div>

            <div id="lado-direito" class="col-sm-4">
                <div id="div-calendario" class="row col-sm-12 form-group box-white well">
                    <div id="div-calendario-body">
                        <?php apresentaCalendarioEquipeFinanceira(); ?>
                    </div>
                    <a id="a-calendario-financeiro" title="Configuração/Cadastro dos períodos" href="spoemendas.php?modulo=principal/financeiro/calendario_financeiro&acao=A"><i
                                class="fa fa-cog" aria-hidden="true"></i> Calendário Financeiro</a>
                </div>
                <div style="clear:both"></div>


                <div id="div-mensagens" class="well row col-sm-12 form-group box-white">
                    <?php (new SpoEmendas_Service_QuadroInformacoes())->setHeight(150)->setId('qt-et')->render(); ?>
                </div>

                <div id="div-grafico-pizza" class="well row col-sm-12 form-group box-white">
                    <?php apresentaGraficoPedidoPorSituacao(date('m')); ?>
                </div>
            </div>
        </div>
    </div>
</div>

<div id="div-script">
    <?php apresentaGraficoEmpenhoMensal('div-grafico-solicitacoes-mensal', date('m'), 'Solicitações', 'Nenhuma solicitação encontrada neste mês', 'Mês corrente'); ?></div>
<?php apresentaGraficoMensal('div-grafico-solicitacoes-anual', null, 'Solicitações', 'Nenhuma solicitação encontrada neste exercício', 'Anual') ?>

<script>
    function openPage(url) {
        window.open(url, '_self');
    }

    function openListaSolilcitacoes(situacao, prsmes) {
        window.open('spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A&emendas[situacaopedido]=' + situacao + '&emendas[mesreferencia]=' + prsmes, '_self');
    }

</script>