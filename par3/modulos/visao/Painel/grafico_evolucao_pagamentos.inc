<div class="row">
<!--    <form action="" id="form-data-evolucao">-->
    <div class="col-lg-offset-4 col-lg-3">
        <div class="" id="data_1">
            <div class="input-daterange input-group m-b" id="datepicker">
                <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;height:34px;"><i class="fa fa-calendar"></i></span>
                <input type="text" class="form-control" name="data-inicio" id="data-inicio" value="<?= ($this->filtro['data-inicio']); ?>">
                <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;height:34px;">a</span>
                <input type="text" class="form-control" name="data-fim" id="data-fim" value="<?= ($this->filtro['data-fim']); ?>">

            </div>
        </div>
    </div>
    <div class="col-lg-2">
        <button class="btn btn-primary" type="button" id="filtrar-periodo-btn">Filtrar</button>
    </div>
<!--    </form>-->
</div>
<div class="row" id="div-chart">
    <div class="">
        <div class="flot-chart" >
            <div class="flot-chart-content" id="flot-dashboard-chart"></div>
        </div>
    </div>
</div>
<br>
<br>
<br>
<script>
    function listarEvolucoesPagamento(form = null) {
        var ibox    = $("#div-chart").parents('.ibox:first');
        var loadDiv = $("#div-chart").parents('.ibox:first').find('.ibox-load-div:first');
        var dtIni = $('#data-inicio').val();
        var dtFim = $('#data-fim').val();
        loadDiv.css('height',ibox.height());
        loadDiv.css('width',ibox.width());
        // loadDiv.fadeIn();

        if(dtIni === '' && dtFim === '') {
            var dtFim    = new Date();
            var dtInicio = new Date();
            if ($(window).width() <= 768) {
                dtInicio.setMonth(dtFim.getMonth() - 1);
            }else {
                dtInicio.setMonth(dtFim.getMonth() - 6);
            }
            var dataFim = (dtFim.getMonth()+1)+'-'+dtFim.getFullYear();
            var dataInicio = (dtInicio.getMonth()+1)+'-'+dtInicio.getFullYear();
            form = '&data-inicio='+dataInicio+'&data-fim='+dataFim;
        }

        $.ajax({
            type: "GET",
            url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=listaevolucoes',
            async: false,
            data : form,
            success: function (resp) {
                var data = JSON.parse(resp);
                montaGraficoEvolucoes(data);
                // loadDiv.fadeOut();
            }
        });
    }

    function montaGraficoEvolucoes(data) {

        if(
            !Array.isArray(data.agrupado.termo_gerado) &&
            !Array.isArray(data.agrupado.termo_validado) &&
            !Array.isArray(data.agrupado.empenho_efetivado) &&
            !Array.isArray(data.agrupado.pagamento_efetivado) &&
            !Array.isArray(data.empenho) &&
            !Array.isArray(data.pagamento)
        ){
            $("#flot-dashboard-chart").html('<div class="alert alert-info"><p align="center">Nenhum registro encontrado</p></div>');
            return false;
        }

        var dataset = [];
        var termo_gerado        = data.agrupado.termo_gerado;
        var termo_validado      = data.agrupado.termo_validado;
        var empenho_efetivado   = data.agrupado.empenho_efetivado;
        var pagamento_efetivado = data.agrupado.pagamento_efetivado;
        validarBarra(dataset,termo_gerado,"Termos gerados",1);
        validarBarra(dataset,termo_validado,"Termos validados",2);
        validarBarra(dataset,empenho_efetivado,"Empenhos efetivados",3);
        validarBarra(dataset,pagamento_efetivado,"Pagamentos efetivados",4);
        validarArrayLinhaPagamento(dataset,data.empenho,"Valor Empenhado",5);
        validarArrayLinhaPagamento(dataset,data.pagamento,"Valor Pago",6);
        var options = {
        xaxis: {
                mode: "time",
                tickSize: [1, "month"],
                timeformat: "%b/%y",
                monthNames : ["Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"],
                axisLabel: "Mês",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 12,
                axisLabelFontFamily: 'Arial',
                axisLabelPadding: 100,
                autoscaleMargin: 0.03
            },
        yaxes: [
            {
                position: "left",
                // max: 1070,
                tickDecimals:0,
                color: "#000",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 10,
                axisLabelFontFamily: 'Arial',
                axisLabelPadding: 100,
            },{
                tickDecimals:2,
                position: "right",
                tickFormatter: function (val, axis) {return "R$"+val.toLocaleString('pt-BR')+',00';},
                color: "#000",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 12,
                axisLabelFontFamily: ' Arial',
                axisLabelPadding: 100
            }
        ],
        legend: {
            noColumns: 3,
            labelBoxBorderColor: "#000000",
            position:"c",
            // axisLegendPadding: 10
        },
        grid: {
            // backgroundColor: { colors: ["#969696", "#5C5C5C"] },
            hoverable: true,
            clickable: true,
            borderWidth: 0
        }
        };

        if (dataset.length > 0) {
            $.plot($("#flot-dashboard-chart"), dataset, options);
        }
    }

    function getDate(year, month) {
        if(!year|| !month) {
            return 0;
        }
        var date = (new Date(year, month,1)).getTime();
        return date;
    }

    $(document).ready(function() {

        $('#data_1 .input-daterange').datepicker({
            format: "mm-yyyy",
            startView: "months",
            minViewMode: "months",
            language: "pt-BR",
        });

        var dataInicio = $('#data-inicio').val();
        var dataFim    = $('#data-fim').val();
        var form = null;
        var dt = new Date();

        if ($(window).width() <= 768) {
            if(dataInicio === '' || dataFim === '') {
                dt = new Date();
                dataFim = (dt.getMonth()+1)+'-'+dt.getFullYear();
                dt.setMonth(dt.getMonth() - 1);
                dataInicio = (dt.getMonth()+1)+'-'+dt.getFullYear();
                form = '&data-inicio='+dataInicio+'&data-fim='+dataFim;
            } else {
                var arDt = dataFim.split('-');
                dt = new Date(arDt[1],arDt[0],1);
                dataFim = (dt.getMonth())+'-'+dt.getFullYear();
                dt.setMonth(dt.getMonth());
                dataInicio = (dt.getMonth())+'-'+dt.getFullYear();
                form = '&data-inicio='+dataInicio+'&data-fim='+dataFim;
            }
        } else {
            if(dataInicio !== '' && dataFim !== '') {
                var arDtFim = dataFim.split('-');
                var arDtIni = dataInicio.split('-');
                var dtFim    = new Date(arDtFim[1],arDtFim[0],1);
                var dtInicio = new Date(arDtIni[1],arDtIni[0],1);
                dataFim = (dtFim.getMonth())+'-'+dtFim.getFullYear();
                dataInicio = (dtInicio.getMonth())+'-'+dtInicio.getFullYear();
                form = '&data-inicio='+dataInicio+'&data-fim='+dataFim;
                console.log(form);
            }else{
                form = $('#form-painel').serialize();
            }
        }

        listarEvolucoesPagamento(form);
    });

    // $("#flot-dashboard-chart").bind("plotclick", function (event, pos, item) {
    //     if (item) {
    //         var x = item.datapoint[0],
    //             y = item.datapoint[1];
    //         if(item.series.label === 'Valor Pago' || item.series.label === 'Valor Empenhado') {
    //             y = y.toLocaleString('pt-BR');
    //         }
    //
    //         alert(item.series.label+" : " + y);
    //     }
    // });

    $("<div id='tooltip'></div>").css({
        position: 'absolute',
        display: 'none',
        border: '1px solid #ccc',
        borderRadius: '5px',
        padding: '8px',
        color: 'rgba(255,255,255,1)',
        'background-color': 'rgba(0, 0, 0, 0.8)'
    }).appendTo("body");

    $("#flot-dashboard-chart").bind("plothover", function (event, pos, item) {

        var str = "(" + pos.x.toFixed(2) + ", " + pos.y.toFixed(2) + ")";
        $("#hoverdata").text(str);
        if (item) {
            var x = item.datapoint[0],
                y = item.datapoint[1];
            if(item.series.label === 'Valor Pago' || item.series.label === 'Valor Empenhado') {
                y = " R$ "+y.toLocaleString('pt-BR');
            }

            $("#tooltip").html(item.series.label+" : " + y)
                .css({top: item.pageY+5, left: item.pageX+5})
                .fadeIn(200);
        } else {
            $("#tooltip").hide();
        }
    });

    $('#filtrar-periodo-btn').on('click',function (evt) {
        evt.preventDefault();
        var form = $('#form-painel').serialize();
        listarEvolucoesPagamento(form);
    });

    function validarBarra(dataset,arr,label,order) {
        var arDados = [0,0];
        if(Array.isArray(arr)) {
            arDados =  arr.map(function(data) {
                var qtd =  data.qtd ? data.qtd : 0;
                var arrdata = [getDate(data.ano,data.mes),qtd];
                return !Array.isArray(arrdata) ? [0,0] :arrdata;
            });
            if(!Array.isArray(arDados)) {
                arDados = [0,0];
            }
            var obj = {
                label: label,
                data: arDados,
                bars: {
                    show: true,
                    barWidth: 60 * 60 * 24 * 1000 * 5,
                    fill: true,
                    lineWidth:1,
                    align: 'center',
                    order: order,
                    fillColor: {
                        colors: [{
                            opacity: 1
                        }, {
                            opacity: 1
                        }]
                    }
                },points: {
                    show: true
                }
            };
            dataset.push(obj);
        }
    }

    function validarArrayLinhaPagamento(dataset,arr,label,order) {
        if(Array.isArray(arr)) {
            if(arr.length === 0) {
                return [0,0];
            }
            var arDados =  arr.map(function(dado) {
                var qtd =  dado.valor ? dado.valor : 0;
                var arrdata = [getDate(dado.ano,dado.mes),qtd];
                if(!Array.isArray(arrdata)) {
                    return [0,0];
                }
                return arrdata;
            });

            if(!Array.isArray(arDados)) {
                return [0,0];
            }
            var obj =  {
                label: label,
                data: arDados,
                yaxis: 2,
                order: order,
                lines: {
                    lineWidth:1,
                    show: true,
                    fill: false,
                    align: 'center'
                },
                points: {
                    show: true
                }
            };
            dataset.push(obj);
        }
    }

    $(window).resize(function() {
        var dataInicio = $('#data-inicio').val();
        var dataFim    = $('#data-fim').val();
        var form = null;
        var dt = new Date();

        if ($(window).width() <= 768) {
            if(dataInicio === '' || dataFim === '') {
                dt = new Date();
                dataFim = (dt.getMonth()+1)+'-'+dt.getFullYear();
                dt.setMonth(dt.getMonth() - 1);
                dataInicio = (dt.getMonth()+1)+'-'+dt.getFullYear();
                $('#data-fim').val(dataInicio);
                $('#data-fim').val(dataFim);
            } else {
                var arDt = dataFim.split('-');
                dt = new Date(arDt[1],arDt[0],1);
                dataFim = (dt.getMonth())+'-'+dt.getFullYear();
                dt.setMonth(dt.getMonth());
                dataInicio = (dt.getMonth())+'-'+dt.getFullYear();
                $('#data-fim').val(dataInicio);
                $('#data-fim').val(dataFim);
            }
            listarEvolucoesPagamento(form);
        }
        else {
            if(dataInicio !== '' && dataFim !== '') {
                var arDtFim = dataFim.split('-');
                var arDtIni = dataInicio.split('-');
                var dtFim    = new Date(arDtFim[1],arDtFim[0],1);
                var dtInicio = new Date(arDtIni[1],arDtIni[0],1);
                dataFim = (dtFim.getMonth())+'-'+dtFim.getFullYear();
                dataInicio = (dtInicio.getMonth())+'-'+dtInicio.getFullYear();
                form = '&data-inicio='+dataInicio+'&data-fim='+dataFim;
            }else {
                form = $('#form-painel').serialize();
                listarEvolucoesPagamento(form);
            }
        }
    });
</script>
