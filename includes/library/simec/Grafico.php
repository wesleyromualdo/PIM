<?php

/*
 * Classe Grafico
 * Classe para gerar gráficos
 * @author Orion Teles de Mesquita
 * @since 10/04/2014
 */
class Grafico {

    /**************************
     *        CONSTANTES       *
     **************************/
    // Tipo de Gráfico
    const K_TIPO_PIZZA  = 'P';
    const K_TIPO_AREA   = 'A';
    const K_TIPO_LINHA  = 'L';
    const K_TIPO_BARRA  = 'B';
    const K_TIPO_COLUNA = 'C';
    const K_TIPO_COLUNA_IMAGEM = 'I';

    // Formatos
    const K_DECIMAL_0 = "function () { return number_format(this.value, 0, ',', '.'); }";
    const K_DECIMAL_2 = "function () { return number_format(this.value, 2, ',', '.'); }";
    const K_TOOLTIP_DECIMAL_0 = "function() { return '<span>' + this.x + '</b><br /><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }";
    const K_TOOLTIP_DECIMAL_2 = "function() { return '<span>' + this.x + '</b><br /><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }";
    const K_TOOLTIP_PIE_DECIMAL_0 = "function() { return '<b>' + this.point.name + '</b><br />Quantidade: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + this.point.percentage.toFixed(2) + '%</b>'; }";
    const K_TOOLTIP_PIE_DECIMAL_2 = "function() { return '<b>' + this.point.name + '</b><br />Valor: <b>' + number_format(this.y, 2, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }";
    const K_TOOLTIP_PIE_DECIMAL_VALOR_MONETARIO = "function() { return '<b>' + this.point.name + '</b><br />Valor R$: <b>' + number_format(this.point.valor_monetario, 2, ',', '.') + '</b><br />Quantidade: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + this.point.percentage.toFixed(2) + '%</b>'; }";

    /**************************
     *        ATRIBUTOS        *
     **************************/
    private $tipo;
    private $id;
    private $dados;
    private $height;
    private $titulo;
    private $subtitulo;
    private $agrupamentoManual;
    private $formatoX;
    private $formatoY;
    private $formatoTooltip;
    private $labelX;
    private $agrupadores;
    private $legenda;
    private $colors;
    private $event;
    private $pointEvent;
    private $innerSize;

    /**
     * @return mixed
     */
    public function getPointEvent()
    {
        return $this->pointEvent;
    }

    /**
     * @param mixed $pointEvent
     */
    public function setPointEvent($pointEvent)
    {
        $this->pointEvent = $pointEvent;
        return $this;

    }
    
    /**
     * @return mixed
     */
    public function getInnerSize()
    {
        return $this->innerSize;
    }

    /**
     * @param mixed $innerSize
     */
    public function setInnerSize($innerSize)
    {
        $this->innerSize = $innerSize;
        return $this;
    }

    /**************************
     *        MÉTODOS          *
     **************************/
    public function __construct($tipo = self::K_TIPO_PIZZA , $incluirBiblioteca = true)
    {
        $this->resetValores();
        $this->tipo = $tipo;
//        if($tipo != self::K_TIPO_PIZZA){
//            $this->formatoTooltip = self::K_TOOLTIP_DECIMAL_2;
//        }

        if($incluirBiblioteca){
            $this->incluirBiblioteca();
        }
    }


    /**************************
     *    GETTERS E SETTERS    *
     **************************/
    /**
     * @param boolean $agrupamentoManual
     */
    public function setAgrupamentoManual($agrupamentoManual)
    {
        $this->agrupamentoManual = $agrupamentoManual;
        return $this;
    }

    /**
     * @return boolean
     */
    public function getAgrupamentoManual()
    {
        return $this->agrupamentoManual;
    }

    /**
     * @param mixed $dados
     */
    public function setDados($dados)
    {
        $this->dados = $dados;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getDados()
    {
        return $this->dados;
    }

    /**
     * @param string $formatoTooltip
     */
    public function setFormatoTooltip($formatoTooltip)
    {
        $this->formatoTooltip = $formatoTooltip;
        return $this;
    }

    /**
     * @return string
     */
    public function getFormatoTooltip()
    {
        return $this->formatoTooltip;
    }

    /**
     * @param mixed $formatoX
     */
    public function setFormatoX($formatoX)
    {
        $this->formatoX = $formatoX;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getFormatoX()
    {
        return $this->formatoX;
    }

    /**
     * @param string $formatoY
     */
    public function setFormatoY($formatoY)
    {
        $this->formatoY = $formatoY;
        return $this;
    }

    /**
     * @return string
     */
    public function getFormatoY()
    {
        return $this->formatoY;
    }

    /**
     * @param string $height
     */
    public function setHeight($height)
    {
        $this->height = $height;
        return $this;
    }

    /**
     * @return string
     */
    public function getHeight()
    {
        return $this->height;
    }

    /**
     * @param string $width
     */
    public function setWidth($width)
    {
        $this->width = $width;
        return $this;
    }

    /**
     * @return string
     */
    public function getWidth()
    {
        return $this->width;
    }

    /**
     * @param mixed $id
     */
    public function setId($id)
    {
        $this->id = $id;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * @param array $labelX
     */
    public function setLabelX($labelX)
    {
        $this->labelX = $labelX;
        return $this;
    }

    /**
     * @return array
     */
    public function getLabelX()
    {
        return $this->labelX;
    }

    /**
     * @param string $subtitulo
     */
    public function setSubtitulo($subtitulo)
    {
        $this->subtitulo = $subtitulo;
        return $this;
    }

    /**
     * @return string
     */
    public function getSubtitulo()
    {
        return $this->subtitulo;
    }

    /**
     * @param mixed $tipo
     *
     * Setar o tooltip sempre após o tipo, caso queira outro tooltip sem ser o padrão para cada tipo.
     */
    public function setTipo($tipo)
    {
        if($tipo != self::K_TIPO_PIZZA){
            $this->formatoTooltip = self::K_TOOLTIP_DECIMAL_2;
        }

        $this->tipo = $tipo;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getTipo()
    {
        return $this->tipo;
    }

    /**
     * @param string $titulo
     */
    public function setTitulo($titulo)
    {
        $this->titulo =  $titulo ;
        return $this;
    }

    /**
     * @return string
     */
    public function getTitulo()
    {
        return $this->titulo;
    }

    /**
     * @param array $agrupadores
     */
    public function setAgrupadores($agrupadores)
    {
        $this->agrupadores = $agrupadores;
        return $this;
    }

    /**
     * @return array
     */
    public function getAgrupadores()
    {
        return $this->agrupadores;
    }

    public function setConfigs($configs)
    {
        foreach($configs as $atributo => $valor){
            if (property_exists( get_class( $this ),$atributo )) {
                $this->{$atributo} = $valor;
            }
        }
        return $this;
    }

    /**
     * @param mixed $legenda
     */
    public function setLegenda($legenda)
    {
        $this->legenda = $legenda;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getLegenda()
    {
        return $this->legenda;
    }

    /**
     * @param mixed $colors
     */
    public function setColors($colors)
    {
        $this->colors = $colors;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getColors()
    {
        return $this->colors;
    }

    /**
     * @return array
     */
    public function getEvent() {
        return $this->event;
    }

    /**
     * @param array $event
     * @return \Grafico
     */
    public function setEvent($event) {
        $this->event = $event;
        return $this;
    }

        
    public function gerarGrafico($dados,$numgrafic=null)
    {
        switch($this->tipo){

            case (self::K_TIPO_COLUNA_IMAGEM):
                $this->montarGraficoLinhaimagem($dados , $numgrafic);
                break;

            case (self::K_TIPO_PIZZA):
                $this->montarGraficoPizza($dados);
                break;

            default:
                $this->montarGraficoLinha($dados);
                break;
        }
    }

    public function montarGraficoLinha($dados)
    {
        $this->dados = $this->agrupamentoManual ? $dados : $this->agruparDadosGrafico($dados);
        if(!$this->id){
            $this->id = $this->gerarIdGrafico();
        }

        $cor = "
                    '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro
                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro
                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro
                    , '#008000' // Verde
                    , '#CD0000' // Vermelho
                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                ";

        ?>

        <div style=" <?php echo ($this->width)? "width: {$this->width};": ''; ?>; height: <?php echo $this->height; ?> " id="<?php echo $this->id; ?>" ></div>

        <script type="text/javascript">
            jQuery(function () {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map([ <?php echo $this->colors; ?> ] , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });

                var title     = '<?php echo $this->titulo; ?>';
                var subtitle  = '<?php echo $this->subtitulo; ?>';
                jQuery('#<?php echo $this->id; ?>').highcharts({
                    chart: {
                        type: '<?php echo $this->getTipoGrafico($this->tipo); ?>',
                        plotBackgroundColor: null,
                        plotBorderWidth: null,
                        plotShadow: true,
                        backgroundColor:'rgba(255, 255, 255, 0.0)'
                    },
                    credits: {
                        enabled: false
                    },
                    exporting: {
                        enabled: false
                    },
                    title: {
                        text: title
                    },
                    subtitle: {
                        text: subtitle
                    },
                    xAxis: {
                        categories: <?php echo json_encode($this->dados['categories']); ?>,

                        <?php if($this->labelX){ ?>
                        labels: <?php echo json_encode($this->labelX); ?>,
                        <?php } ?>

                        <?php if($this->formatoX){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoX; ?>,
                        },
                        <?php } ?>
                    },
                    yAxis: {
                        title: {
                            text: ''
                        },

                        <?php if($this->formatoY){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoY; ?>
                        },
                        <?php } ?>

                        plotLines: [{
                            value: 0,
                            width: 1,
                            color: '#808080'
                        }]
                    },
                    <?php if($this->formatoTooltip){ ?>
                    tooltip: {
                        formatter: <?php echo $this->formatoTooltip; ?>
                    },
                    <?php } ?>

                    <?php if($this->legenda){ ?>
                    legend: <?php echo json_encode($this->legenda); ?>,
                    <?php } ?>

                    <?php if($this->tipo == self::K_TIPO_PIZZA){ ?>
                    plotOptions: {
                        pie: {
                            allowPointSelect: true,
                            cursor: 'pointer',
                            dataLabels: {
                                enabled: true,
                                color: '#000000',
                                distance: 30,
//                                    formatter: function() {
//                                        return this.percentage +' %'; //'<b>'+ this.point.name +'</b>: '+
//                                    },
                                formatter: function () { return this.point.name + ' (' + this.percentage.toFixed(2) + '%)'; },
                                format: '{point.percentage:.2f}%</b>',
                                showInLegend: true
                            },showInLegend: true,
                            
                             <?php if($this->event): ?>
                                events: {
                                    <?php foreach ($this->event as $key => $value) :?>
                                        <?php echo $key ?>: function (event) {
                                            <?php echo $value ?>
                                        }
                                    <?php endforeach; ?>
                                }
                            <?php endif; ?>
                        }
                    },
                    <?php } ?>

                    series: <?php echo json_encode($this->dados['series']);
                    
//                    ver( $this->dados['series'] );
//                    echo 'aqui';
                    ?>

                });
            });
        </script>

        <?php
        // Apagando o valor do id em caso de inserir outro gráfico com id gerado automaticamente
        $this->resetValores();
    }


    public function montarGraficoPizza($dados)
    {
        $this->dados = $this->agrupamentoManual ? $dados : $this->agruparDadosGrafico($dados);
        if(!$this->id){
            $this->id = $this->gerarIdGrafico();
        }

        $cor = "
                    '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro
                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro
                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro
                    , '#008000' // Verde
                    , '#CD0000' // Vermelho
                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                ";

        ?>

        <div style=" <?php echo ($this->width)? "width: {$this->width};": ''; ?>; height: <?php echo $this->height; ?> " id="<?php echo $this->id; ?>" ></div>

        <script type="text/javascript">
            jQuery(function () {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map([ <?php echo $this->colors; ?> ] , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });

                var title     = '<?php echo $this->titulo; ?>';
                var subtitle  = '<?php echo $this->subtitulo; ?>';
                jQuery('#<?php echo $this->id; ?>').highcharts({
                    chart: {
                        type: '<?php echo $this->getTipoGrafico($this->tipo); ?>',
                        plotBackgroundColor: null,
                        plotBorderWidth: null,
                        plotShadow: true,
                        backgroundColor:'rgba(255, 255, 255, 0.0)'                        
                    },
                    credits: {
                        enabled: false
                    },
                    exporting: {
                        enabled: false
                    },
                    title: {
                        text: title
                    },
                    subtitle: {
                        text: subtitle
                    },
                    xAxis: {
                        categories: <?php echo json_encode($this->dados['categories']); ?>,

                        <?php if($this->labelX){ ?>
                        labels: <?php echo json_encode($this->labelX); ?>,
                        <?php } ?>

                        <?php if($this->formatoX){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoX; ?>,
                        },
                        <?php } ?>
                    },
                    yAxis: {
                        title: {
                            text: ''
                        },

                        <?php if($this->formatoY){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoY; ?>
                        },
                        <?php } ?>

                        plotLines: [{
                            value: 0,
                            width: 1,
                            color: '#808080'
                        }]
                    },
                    <?php if($this->formatoTooltip){ ?>
                    tooltip: {
                        formatter: <?php echo $this->formatoTooltip; ?>
                    },
                    <?php } ?>

                    <?php if($this->legenda){ ?>
                    legend: <?php echo json_encode($this->legenda); ?>,
                    <?php } ?>

                    <?php if($this->tipo == self::K_TIPO_PIZZA){ ?>
                    plotOptions: {
                        pie: {
                            allowPointSelect: true,
                            cursor: 'pointer',
                            <?php if (($this->innerSize) && ($this->innerSize) > 0): ?>
                                innerSize: '<?php echo $this->getInnerSize(); ?>%',
                            <?php endif; ?>
                            <?php if($this->pointEvent): ?>

                            point: {
                                events: {
                                    <?php foreach ($this->pointEvent as $key => $value) :?>
                                    <?php echo $key ?>: function (event) {
                                        <?php echo $value ?>
                                    }
                                    <?php endforeach; ?>
                                }
                            },
                            <?php endif;?>
                            dataLabels: {
                                enabled: true,
                                color: '#000000',
                                distance: 30,
//                                    formatter: function() {
//                                        return this.percentage +' %'; //'<b>'+ this.point.name +'</b>: '+
//                                    },
                                formatter: function () { return this.point.name + ' (' + this.percentage.toFixed(2) + '%)'; },
                                format: '{point.percentage:.2f}%</b>',
                                showInLegend: true
                            },showInLegend: true,

                            <?php if($this->event): ?>
                            events: {
                <?php foreach ($this->event as $key => $value) :?>
                <?php echo $key ?>: function (event) {
                    <?php echo $value ?>
                }
                <?php endforeach; ?>
            }
                <?php endif; ?>
            }
            },
                <?php } ?>

                series: <?php echo json_encode($this->dados['series']);

                    //                    ver( $this->dados['series'] );
                    //                    echo 'aqui';
                    ?>

                    });
            });
        </script>

        <?php
        // Apagando o valor do id em caso de inserir outro gráfico com id gerado automaticamente
        $this->resetValores();
    }



    public function montarGraficoLinhaimagem($dados, $numgrafic)
    {
        $this->dados = $this->agrupamentoManual ? $dados : $this->agruparDadosGrafico($dados);
        if(!$this->id){
            $this->id = $this->gerarIdGrafico();
        }

        $cor = "
                    '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro
                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro
                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro
                    , '#008000' // Verde
                    , '#CD0000' // Vermelho
                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                ";

        ?>



        <script type="text/javascript">
            jQuery(function () {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map([ <?php echo $this->colors; ?> ] , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });


                var subtitle  = '<?php echo $this->subtitulo; ?>';
                var options = {
                    chart: {
                        type: '<?php echo $this->getTipoGrafico('C'); ?>'
                    },
                    credits: {
                        enabled: false
                    },
                    exporting: {
                        enabled: false
                    },
                    title: {
                        text: subtitle
                    },
                    subtitle: {
                        text: ''
                    },
                    xAxis: {
                        categories: <?php echo json_encode($this->dados['categories']); ?>,

                        <?php if($this->labelX){ ?>
                        labels: <?php echo json_encode($this->labelX); ?>,
                        <?php } ?>

                        <?php if($this->formatoX){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoX; ?>,
                        },
                        <?php } ?>
                    },
                    yAxis: {
                        title: {
                            text: ''
                        },

                        <?php if($this->formatoY){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoY; ?>
                        },
                        <?php } ?>

                        plotLines: [{
                            value: 0,
                            width: 1,
                            color: '#808080'
                        }]
                    },
                    <?php if($this->formatoTooltip){ ?>
                    tooltip: {
                        formatter: <?php echo $this->formatoTooltip; ?>
                    },
                    <?php } ?>

                    <?php if($this->legenda){ ?>
                    legend: <?php echo json_encode($this->legenda); ?>,
                    <?php } ?>

                    <?php if($this->tipo == self::K_TIPO_PIZZA){ ?>
                    plotOptions: {
                        pie: {
                            allowPointSelect: true,
                            cursor: 'pointer',
                            dataLabels: {
                                enabled: true,
                                color: '#000000',
                                distance: 30,
//                                    formatter: function() {
//                                        return this.percentage +' %'; //'<b>'+ this.point.name +'</b>: '+
//                                    },
                               /* formatter: function () { return this.point.name + ' (' + this.percentage.toFixed(2) + '%)'; },
                                format: '{point.percentage:.2f}%</b>',*/
                                showInLegend: true
                            },showInLegend: true,

                            <?php if($this->event): ?>
                            events: {
                <?php foreach ($this->event as $key => $value) :?>
                <?php echo $key ?>: function (event) {
                    <?php echo $value ?>
                }
                <?php endforeach; ?>
            }
                <?php endif; ?>
            }
            },
                <?php } ?>

                series: <?php echo json_encode($this->dados['series']);


                    ?>



                    };




                // URL to Highcharts export server
                var exportUrl = 'http://export.highcharts.com/';

                // POST parameter for Highcharts export server
                var object = {
                    options: JSON.stringify(options),
                    type: 'image/png',
                    async: true,
                    width: 800
                };

                // Ajax request
                $.ajax({
                    type: 'post',
                    url: exportUrl,
                    data: object,
                    success: function (data) {
                        // Update "src" attribute with received image URL
                        $('#chart_<?=$numgrafic?>').attr('src', exportUrl + data);
                    }
                });
            });
        </script>

        <?php
        // Apagando o valor do id em caso de inserir outro gráfico com id gerado automaticamente
        $this->resetValores();
    }



    public function utf8Encode(&$dados)
    {
        if(is_array($dados)){
            foreach($dados as &$dado ){
                if(is_array($dado)){
                    $this->utf8Encode($dado);
                } else {
                    $dado = ($dado);
                }
            }
        }
    }


    /*
     * Função que agrupa os dados de uma consulta para serem usados em gráficos agrupados.
     * O agrupamento obedece ao formato exigido pelo componente de gráfico.
     *
     * @param array $dados - Dados carregados do banco
     * @param string $categoria - Nome do campo que representará as divisões (eixo x)
     * @param string $name - Nome do campo que representará os grupos dentro da divisão
     * @param string $valor - Nome do campo que representará o valor de cada grupo
     *
     * @return array - Retorna um array com duas informações: series e categories, sendo o primeiro com os dados agrupados e o último com todas as divisões únicas
     * @author Orion Teles de Mesquita
     */
    public function agruparDadosGrafico($dados)
    {
        $categoria = $this->agrupadores['categoria'];
        $name = $this->agrupadores['name'];
        $valor = $this->agrupadores['valor'];
        $id = $this->agrupadores['id'];

        if(is_string($dados)){
//            $db = new cls_banco();
            global $db;
            $dados = $db->carregar($dados);
        }

        $categories = array();
        $grupos = array();
        $dadosAgrupados = array();

        if(is_array($dados)){

            $this->utf8Encode($dados);

            if ($this->tipo == self::K_TIPO_PIZZA) {
                if($this->getFormatoTooltip() == self::K_TOOLTIP_PIE_DECIMAL_VALOR_MONETARIO) {
                    $valorMonetario = $this->agrupadores['valor_monetario'];
                    $series[0]['name'] = '';
                    foreach($dados as $dado) {
                        $series[0]['data'][] = (object) array('name' => $dado[$name], 'y' => (float)$dado[$valor],$valorMonetario => (float)$dado[$valorMonetario],$id => $dado[$id]);
                    }
                } else {
                    $series[0]['name'] = '';
                    foreach($dados as $dado) {
                        $series[0]['data'][] = array('name' => $dado[$name], 'y' => (float)$dado[$valor],$id =>$dado[$id]);
                    }
                }

            } else {

                foreach ($dados as $dado) {
//                ver($dados, d);
                    $dadosAgrupados[$dado[$categoria]][$dado[$name]] = (float) $dado[$valor];
                    $categories[$dado[$categoria]] = $dado[$categoria];
                    $grupos[$dado[$name]] = $dado[$name];
                }

                $dadosFinais = array();
                foreach ($grupos as $grupo) {
                    foreach ($categories as $divisao) {
                        if(!isset($dadosAgrupados[$divisao][$grupo])){
                            $dadosFinais[$grupo][] = 0;
                        } else {
                            $dadosFinais[$grupo][] = $dadosAgrupados[$divisao][$grupo];
                        }
                    }
                }
                $series = array();
                foreach ($dadosFinais as $divisao => $aDado) {
                    $series[] = array('name' => (string)$divisao, 'data'=>$aDado);
                }
            }
        }
        return array('series'=>$series, 'categories'=>array_values($categories));
    }

    private function gerarIdGrafico()
    {
        return 'grafico_' . rand() . '_' . rand();
    }

    private function getTipoGrafico($tipo)
    {
        $tipos = array(
            self::K_TIPO_PIZZA  => 'pie',
            self::K_TIPO_AREA   => 'area',
            self::K_TIPO_LINHA  => 'line',
            self::K_TIPO_BARRA  => 'bar',
            self::K_TIPO_COLUNA => 'column',
        );

        return $tipos[$tipo];
    }

    private function resetValores()
    {
        $this->id = null;
        $this->height = '300px';
        $this->titulo = '';
        $this->subtitulo = '';
        $this->agrupamentoManual = false;
        $this->formatoX = null;
        $this->formatoY = self::K_DECIMAL_0;
        $this->labelX = array('rotation'=>-45, 'align'=>'right');
        $this->agrupadores = array('categoria' => 'categoria', 'name' => 'descricao', 'valor' => 'valor','id' => 'id');

        // Exemplo de Legenda
        // $this->legenda = array('layout' => 'vertical', 'align' => 'right', 'verticalAlign' => 'middle', 'borderWidth'=>'0');

        $this->formatoTooltip = self::K_TOOLTIP_PIE_DECIMAL_2;

        $this->colors =  "
                              '#00BFFF' // Azul claro
                            , '#55BF3B' // Verde
                            , '#FFD700' // Amarelo
                            , '#FF6A6A' // Vermelho claro
                            , '#eeaaee' // Rosa claro
                            , '#aaeeee' // Cinza claro
                            , '#7798BF' // Roxo claro
                            , '#DDDF0D' // Verde claro
                            , '#7CCD7C' // Amarelo um pouco mais claro
                            , '#DF5353' // Vermelho rosa escuro
                            , '#008000' // Verde
                            , '#CD0000' // Vermelho
                            , '#FF4500' // Laranja
                            , '#ff0066' // Rosa choque
                            , '#4B0082' // Roxo
                            , '#808000' // Verde oliva
                            , '#800000' // Marrom
                            , '#2F4F4F' // Cinza escuro
                            , '#006400' // Verde escuro
                            , '#FFA500' // Amarelo quemado
                        ";

    }

    private function incluirBiblioteca()
    {
        echo '
            <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
            <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
            <script language="javascript" src="/estrutura/js/funcoes.js"></script>

        ';
    }
}
