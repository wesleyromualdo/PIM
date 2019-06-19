<?php
/**
 * Sistema de gestão Emendas
 * $Id: inicio.inc 135598 2017-12-13 20:04:19Z victormachado $
 */

if ($_POST['action']) {
    switch ($_POST['action']) {
        case 'ajaxGraficoMensal' :
            apresentaGrafico('div-grafico-solicitacoes-mensal-ef', $_POST['prsmes'], 'Solicitações por Grupo de Unidade', 'Nenhuma solicitação encontrada neste mês', 'Mês corrente');
            apresentaCalendarioEquipeFinanceira($_POST['prsmes']);
            break;
        case 'ajaxGraficoEmpenhoMensal':
            apresentaGraficoEmpenhoMensal('div-grafico-solicitacoes-mensal', $_POST['prsmes'], 'Solicitações', 'Nenhuma solicitação encontrada neste mês', 'Mês corrente');
            apresentaCalendarioEquipeFinanceira($_POST['prsmes']);
            break;
        case 'ajaxGraficoPizza':
            apresentaGraficoPedidoPorSituacao($_POST['prsmes']);
            break;

        case 'carregaGraficoMensal':
            apresentaGrafico('div-grafico-solicitacoes-mensal-ef', date('m'), 'Solicitações por Grupo de Unidade', 'Nenhuma solicitação encontrada neste mês', 'Mês corrente');
            break;

        case 'carregaGraficoAnual':
            apresentaGrafico('div-grafico-solicitacoes-anual-ef', null, 'Solicitações por Grupo de Unidade', 'Nenhuma solicitação encontrada neste exercício', 'Anual');
            break;
    }
    exit;
}


/**
 * Cabecalho do SIMEC.
 *
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

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
    });
</script>

<div class="col-sm-12 form-group">
    <?php
    if ((in_array(PFL_SPO_EQUIPE_FINANCEIRA, $perfis) && (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis))) || in_array(PFL_SUPER_USUARIO, $perfis)) {
//    if (in_array(PFL_SPO_EQUIPE_FINANCEIRA, $perfis) && in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
        ?>
        <ul class="nav nav-tabs">
            <li role="presentation" class="active"><a href="#equipefinanceira" role="tab" data-toggle="tab" aria-controls="equipefinanceira">SPO/Equipe Financeira</a></li>
            <li role="presentation"><a href="#equipetecnica" role="tab" data-toggle="tab" aria-controls="equipetecnica">UO/Equipe Técnica</a></li>
        </ul>

        <div class="tab-content">
            <div role="tabpanel" class="tab-pane active" id="equipefinanceira" style="border: 1px solid #dddddd;">
                <?php require_once 'inicioEquipeFinanceira.inc'; ?>
            </div>

            <div role="tabpanel" class="tab-pane" id="equipetecnica" style="border: 1px solid #dddddd;">
                <?php require_once 'inicioEquipeTecnica.inc'; ?>
            </div>
        </div>

        <?php
    } elseif (in_array(PFL_SPO_EQUIPE_FINANCEIRA, $perfis)) {
        require_once 'inicioEquipeFinanceira.inc';
    } elseif (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
        require_once 'inicioEquipeTecnica.inc';
    } else {
        header("Location: spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A");
    }
    ?>
</div>

<script>

    function carregaGraficoMensalEf(prsmes) {
        $.ajax({
            type: 'POST',
            data: {
                action: 'ajaxGraficoMensal',
                prsmes: prsmes
            },
            success: function (data) {
                $('#div-calendario-body-ef').html(data);
            }
        });
    }

    function carregaGraficoMensal(prsmes) {
        $.ajax({
            type: 'POST',
            data: {
                action: 'ajaxGraficoEmpenhoMensal',
                prsmes: prsmes
            },
            success: function (data) {
                $('#div-calendario-body').html(data);
            }
        });
    }

    function carregaGraficoPizza(prsmes) {
        $.ajax({
            type: 'POST',
            data: {
                action: 'ajaxGraficoPizza',
                prsmes: prsmes
            },
            success: function (data) {
                $('#div-grafico-pizza').html(data);
            }
        });
    }
</script>

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

