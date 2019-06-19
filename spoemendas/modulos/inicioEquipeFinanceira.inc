<link rel="stylesheet" type="text/css" href="\library\font-awesome\css\font-awesome.min.css" media="all">

<style>
    #div-central {
        margin-bottom: 20px;
    }

    .btn-equipe-financeira {
        background-color: #40AAAA !important;
        border-color: #94CFCF !important;
        white-space: pre-wrap;
    }

    .btn-equipe-financeira:hover,
    .btn-equipe-financeira:active,
    .btn-equipe-financeira:target,
    .btn-equipe-financeira:focus {
        background-color: #409999 !important;
        border-color: #94CFCF !important;
    }

    .botao-inicio .btn-equipe-financeira {
        height: 90px !important;
    }

    .btn-group .btn-equipe-financeira {
        height: 90px !important;
    }

    #lado-direito > .well,
    #div-graficos > .well,
    #div-central.well,
    #botoes-superior.well {
        background-color: white;
    }

    div.zabuto_calendar .table tr td div.day {
        padding-top: 9px !important;
        padding-bottom: 9px !important;
    }

    #div-grafico-solicitacoes-mensal-ef,
    #div-grafico-solicitacoes-anual-ef {
        min-height: 365.976px;
    }

    .loading-dialog-grafico {
        position: fixed;
        background-color: #3c3c3c;
        filter: alpha(opacity = 75);
        opacity: 0.75;
        -moz-opacity: 0.75;
        bottom: 0;
        cursor: default;
        display: none;
        overflow: hidden;
        padding: 0 0;
        text-align: center;
        top: 62px;
        z-index: 9999999;
    }
</style>

<div class="row" style="background-color: #f5f5f5;">
    <br>
    <div class="col-sm-12">

        <div id="div-central">
            <div class="col-sm-12">
                <div id="div-central" class="col-sm-12 well">
                    <div class="col-sm-3 botao-inicio text-center">
                        <input type="button" class="btn btn-success btn-block btn-equipe-financeira" value="Solicitações de Financeiro"
                               onclick="openPage('spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A')">
                    </div>
                    <div class="col-sm-3 botao-inicio text-center">
                        <input type="button" class="btn btn-success btn-block btn-equipe-financeira" value="Exportar Solicitações"
                               onclick="openPage('spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A')">
                    </div>
                    <div class="col-sm-3">
                        <div class="row btn-group" style="width: 100%;">
                            <div class="btn-group" style="width: 50%;">
                                <button class="btn btn-success btn-block btn-equipe-financeira" onclick="openPage('spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&amp;acao=A')"
                                        style="height: 70px;">Carga Tesouro Gerencial</button>
                            </div>
                            <div class="btn-group" style="width: 50%;">
                                <button class="btn btn-success btn-block btn-equipe-financeira" onclick="openPage('spoemendas.php?modulo=principal/cargas/segov/carga&amp;acao=A')" style="height: 70px;">Carga SEGOV</button>
                            </div>
                        </div>
                    </div>
                    <div class="col-sm-3 botao-inicio text-center">
                        <input type="button" class="btn btn-success btn-block btn-equipe-financeira" value="Relatório Dinâmico"
                               onclick="openPage('spoemendas.php?modulo=relatorio/extratodinamicoFinanceiro&acao=A')">
                    </div>
                    <div style="clear:both"></div>
                </div>
                <div style="clear:both"></div>
            </div>
            <div style="clear:both"></div>
        </div>

        <div id="div-body" class="row">
            <div id="div-graficos" class="col-sm-8">
                <div id="div-grafico-solicitacoes-mensal-ef" class="col-sm-12 form-group well"></div>
                <div id="div-grafico-solicitacoes-anual-ef" class="col-sm-12 form-group well"></div>
            </div>

            <div id="lado-direito" class="col-sm-4">
                <div id="div-calendario-ef" class="col-sm-12 form-group well">
                    <div id="div-calendario-body-ef">
                        <?php apresentaCalendarioEquipeFinanceira(); ?>
                    </div>
                    <a id="a-calendario-financeiro" title="Configuração/Cadastro dos períodos" href="?modulo=principal/financeiro/calendario_financeiro&acao=A"><i
                                class="fa fa-cog" aria-hidden="true"></i> Calendário Financeiro</a>
                </div>
                <div id="div-mensagens-ef" class="col-sm-12 form-group well">
                    <?php (new SpoEmendas_Service_QuadroInformacoes())
                        ->setHeight(318)
                        ->setId('qi-ef')
                        ->render(); ?>
                </div>
            </div>
        </div>
    </div>
</div>

<div id="div-grafico-mensal">
    <?php //apresentaGrafico('div-grafico-solicitacoes-mensal-ef', date('m'), 'Solicitações por Grupo de Unidade', 'Nenhuma solicitação encontrada neste mês', 'Mês corrente'); ?>
</div>

<div id="div-grafico-anual">
    <?php //apresentaGrafico('div-grafico-solicitacoes-anual-ef', null, 'Solicitações por Grupo de Unidade', 'Nenhuma solicitação encontrada neste exercício', 'Anual') ?>
</div>

<script>
    $( document ).ready(function(){
        carregaGraficos();
    });

    function divCarregando(id) {
        div = $('#loading').clone();
        $(div).css({
            'display'  : 'block',
            'position' : 'static',
            'height'   : '365.976px'
        });

        $('#'+id).html(div);
    }

    function carregaGraficos() {
        // Carrega Gráfico Mensal
        console.log('Carregando gráfico mensal...');
        divCarregando('div-grafico-solicitacoes-mensal-ef');
        $.ajax({
            type: 'POST',
            data: {
                action: 'carregaGraficoMensal'
            },
            success: function(data) {
                $('#div-grafico-mensal').html(data);
                console.log('Gráfico mensal carregado com sucesso');
            }
        });

        // Carregam Gráfico Anual
        console.log('Carregando gráfico anual...');
        divCarregando('div-grafico-solicitacoes-anual-ef');
        $.ajax({
            type: 'POST',
            data: {
                action: 'carregaGraficoAnual'
            },
            success: function(data) {
                $('#div-grafico-anual').html(data);
                console.log('Gráfico anual carregado com sucesso');
            }
        });
    }

    function openPage(url) {
        window.open(url, '_self');
    }

</script>