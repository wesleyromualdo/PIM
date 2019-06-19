<?php
/**
 * Sistema de gestão Emendas
 * $Id: inicio.inc 135868 2017-12-19 11:56:14Z victormachado $
 */

if ($_POST['action'] == 'ajaxGraficoMensal') {
    apresentaGrafico('div-grafico-solicitacoes-mensal', $_POST['prsmes'], null, 'Nenhuma solicitação encontrada neste mês');
    exit;
}

if ($_POST['action'] == 'ajaxGraficoEmpenhoMensal') {
    apresentaGraficoEmpenhoMensal('div-grafico-solicitacoes-mensal', $_POST['prsmes'], null, 'Nenhuma solicitação encontrada neste mês');
    exit;
}

if ($_POST['action'] == 'ajaxGraficoPizza') {
    apresentaGraficoPedidoPorSituacao($_POST['prsmes']);
    exit;
}


/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

$fm = new Simec_Helper_FlashMessage('spoemendas/inicio');


/*
 * Tratamento para o Período
 */
$permissaoPeriodo = (new Spoemendas_Controller_Permissaoedicaoemenda())->getAutorizacaoEditarPeriodo();
if ($permissaoPeriodo) {
    $msgPeriodo = (new Spoemendas_Controller_Periodopreenchimentoemenda())->getMensagensPeriodo();
    if ($msgPeriodo) {
        foreach ($msgPeriodo as $msg) {
            if ($msg['ppemensagem']) {
                $fm->addMensagem($msg['ppemensagem'], Simec_Helper_FlashMessage::ERRO);
            }
        }
    }
}
$mensagens = $fm->getMensagens(true);
if ($mensagens)
{
    echo "<div class='row' style='line-height: 1.3em'>{$mensagens}</div>";
}

$dadosPeriodoAtual = retornaDadosPeriodoAtual();
$origensEditaveis = [];
$mensagensPostadas = [];

$dados = $_REQUEST['emendas'];
$where = " WHERE true ";

/* Filtros por Perfil */
$perfisLinha = implode(',', $perfis);


$listaExercicios = "SELECT
				prsano AS codigo,
				prsano AS descricao
			FROM
				spoemendas.programacaoexercicio
			WHERE
				prsexercicioaberto
			ORDER BY
				1 DESC";

if (!isset($dados['exercicios']))
{
    $lista[0]['codigo'] = $_SESSION['exercicio'];
    $dados['exercicios'] = [];

    if (is_array($lista))
    {
        foreach ($lista as $chave => $valor)
        {
            array_push($dados['exercicios'], $valor['codigo']);
        }
    }

    $exercicios = implode("','", $dados['exercicios']);

    if (isset($exercicios))
    {
        $whereAno = " WHERE eme.emeano IN ('{$exercicios}')";
    }

    /* Forçando o filtro por exercício */
    if (isset($dados['exercicios']))
    {
        if (is_array($dados['exercicios']) && count($dados['exercicios']) < 2)
        {
            $dados['exercicios'] = explode(',', $dados['exercicios'][0]);
        }
        $exercicios = implode("','", $dados['exercicios']);
        $where .= " AND e.emeano in ('{$exercicios}')";
    }
}
?>
<script>
    var HighchartsCore = Highcharts;
    Highcharts = null;
</script>
<script src="https://code.highcharts.com/highcharts.js"></script>
<script src="https://code.highcharts.com/modules/drilldown.js"></script>
<script src="https://code.highcharts.com/modules/no-data-to-display.js"></script>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
        // $('#aviso-parlamentar').modal();
    });
    function abrirArquivoComunicado(arqid) {
        var uri = window.location.href;
        uri = uri.replace(/\?.+/g, '?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid=' + arqid);
        window.location.href = uri;
    }
</script>
<?php
apresentaInicio($perfis);
?>

<!--<div class="modal fade" id="aviso-parlamentar" tabindex="-1" role="dialog" aria-hidden="true"  >
    <div class="modal-dialog" style="width: 90%; ">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Fechar</span></button>
            AVISO!
            </div>
            <div class="modal-body" style="overflow: auto; text-align: left;">
                Senhor(a) parlamentar, <br/><br/>
                As indicações das emendas <b>2016</b> foram validadas no SIOP. <br/>
                O próximo passo é o preenchimento, até <b>02/03/2016</b> no SIMEC/SPO - Emendas Parlamentares das iniciativas, dos dados do responsável pela elaboração do PTA e, quando se tratar de prefeitura e secretaria estadual, da vinculação da subação. <br/>
                <br/>Qualquer dúvida, tratar com a <b>ASPAR do MEC (2022-7899/7896/7894)</b>
                <br/><br/>
                <b>Alertamos que as emendas que tenham valores indicados inferiores ao necessário para execução da iniciativa serão assinaladas com impedimento técnico.</b>
                <br/><br/>
                <br/><br/>
            </div>
        </div> /.modal-content
    </div> /.modal-dialog
</div> /.modal -->

<div class="modal fade" id="aviso-parlamentar" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog" style="width: 90%; ">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Fechar</span></button>
                AVISO!
            </div>
            <div class="modal-body" style="overflow: auto; text-align: left;">
                Senhor(a) reitor(a) de Universidades e Institutos <b>Federais</b>,<br/><br/>
                Solicitamos o preenchimento do Plano de Trabalho (PTA) até o dia <b>30/03</b>.
                <br/>Caso haja impedimento técnico, favor informar o tipo do impedimento, o objeto do impedimento, o valor impedido e a justificativa.
                <br/><br/><br/><br/><br/><br/>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->


<div class="modal fade" id="fluxo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog" style="width: 90%; ">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
                <h4 class="modal-title">Fluxo das Emendas Parlamentares no MEC</h4>
            </div>
            <div class="modal-body" style="overflow: auto; text-align: center;">
                <img src="fluxo.png" alt="Fluxo"/>
                <br/><br/>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

