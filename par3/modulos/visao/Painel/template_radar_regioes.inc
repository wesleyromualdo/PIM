<div id="div-radarpagamentos" style="height: 283px;"></div>
<script>
    renderGrafico();
    function renderGrafico() {
        $.ajax({
            type: "GET",
            url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=listapagamentosregiao',
            async: false,
            success: function (resp) {
                var data = JSON.parse(resp);
                montarGraficoPagamentosRegiao(data);
            }
        });
    }

    function montarGraficoPagamentosRegiao(data) {
        console.log(data);
        var radarData = {
            labels: data.map(function(dt) { return dt.regdescricao}),
            ids : data.map(function(dt){return dt.regcod;}),
            datasets: [
                {
                    label: "Pagamentos",
                    id: 'pagamento',
                    backgroundColor: "rgb(0, 102, 255,0.2)",
                    borderColor: "rgb(0, 102, 255,1)",
                    data: data.map(function(dt) { return dt.vlr_pagamento}),
                },
                {
                    label: "Empenhos",
                    id: 'empenhos',
                    backgroundColor: "rgba(26,179,148,0.2)",
                    borderColor: "rgba(26,179,148,1)",
                    data: data.map(function(dt) { return dt.vlr_empenho}),
                }
            ]
        };

        var radarOptions = {
            responsive: true,
            tooltips: {
                callbacks: {
                    label: function(tooltipItem, data) {
                        var label = data.datasets[tooltipItem.datasetIndex].label || '';
                        var id = data.datasets[tooltipItem.datasetIndex].id || '';
                        if (label) {
                            label += ': ';
                        }
                        if(id === 'pagamento') {
                            label += " R$ "+tooltipItem.yLabel.toLocaleString('pt-BR');
                        }

                        if(id === 'empenhos') {
                            label += " R$ "+tooltipItem.yLabel.toLocaleString('pt-BR');
                        }
                        return label;
                    }
                }
            },
            legend: {
                display: true,
                position: 'bottom'
            },
        };

        var canvas    = document.createElement('canvas');
        canvas.id     = "radarChart";
        canvas.width  = 283;
        canvas.height = 200;
        $('#div-radarpagamentos').html(canvas);
        var ctx = canvas.getContext("2d");
        var chart = new Chart(ctx, {type: 'radar', data: radarData, options:radarOptions});
        chart.update();

        canvas.onclick = function(evt,elem) {
            var activePoints = chart.getElementsAtEvent(evt);
            if(activePoints.length != 0 ){
                console.log(activePoints);
                var dados = listarPagamentosPorRegiao();
                $('#modal_detalhe').find('#titulo-modal').html('Distribuição por Região');
                $('#modal_detalhe').find('#conteudo-modal').html(dados);
                $('#modal_detalhe').modal();
            }
        }
            var activePoints = chart.getElementsAtEvent(evt);
            if (activePoints[0]) {
                var chartData = activePoints[0]['_chart'].config.data;
                var idx = activePoints[0]['_index'];
                var label     = chartData.labels[idx];
                var regcod = chartData.ids[idx];
                listarPagamentosPorRegiao(regcod);
            }
        // };
    }

    function listarPagamentosPorRegiao(regcod = null)
    {
        var dados = {};
        $.ajax({
            type: "GET",
            url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=listapagamentosregiaoestado',
            async: false,
            success: function (resp) {
                // var data = JSON.parse(resp);
                dados = resp;
            }
        });
        return dados;
    }

</script>
