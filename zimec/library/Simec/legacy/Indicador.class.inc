<?php

/*
 * Classe IndicadorSimec
 * Classe para gerar tabelas e graficos dos indicadores
 * @author Marcus Vinícius Louvem Rocha
 * @since 10/04/2014
 */

class Simec_Indicador
{

    /**************************
     *        CONSTANTES       *
     **************************/

    const K_DPEANOREF = '2015';

    /**************************
     *        ATRIBUTOS        *
     **************************/


    /**************************
     *        MÉTODOS          *
     **************************/


    public function gerarGrafico($indid)
    {
        $painel = new Painel_Model_Seriehistorica();
        $dados = $painel->carregarGraficoIndicador($indid);
        $grafico = new Grafico(null, false);
        $grafico->setHeight('400px')->setTipo(Grafico::K_TIPO_COLUNA)->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($dados);
    }

    public function gerarGraficoCruzado($indid, $indid2)
    {
        $painel = new Painel_Model_Seriehistorica();
        $dados = $painel->carregarGraficoIndicadorCruzado($indid, $indid2);
        $grafico = new Grafico(null, false);
        $grafico->setHeight('400px')->setTipo(Grafico::K_TIPO_COLUNA)->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($dados);
    }

    public function gerarGraficoNivel($indid, $nivel)
    {
        $painel = new Painel_Model_Seriehistorica();
        $dadosNivel = $painel->carregarDadosNivel($nivel);
        $dados = $painel->carregarGraficoIndicadorNivel($indid, $nivel);
        $grafico = new Grafico(null, false);
        $grafico->setTitulo($dadosNivel['tiddsc'])->setHeight('400px')->setTipo(Grafico::K_TIPO_PIZZA)->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($dados);
    }

    public function gerarGraficoNivelCombo($indid, $muncod = null, $estuf = null, $escondeFiltros = array())
    {
        global $simec;

        $painel = new Painel_Model_Seriehistorica();
        $dadosNivel = $painel->carregarDadosNivelPorId($indid, $muncod, $estuf);

        $dadosExtra['codigo'] = 0;
        $dadosExtra['descricao'] = "Todos";
        $dadosNivel[1000] = $dadosExtra;
        echo "<script>
            $(document).ready(function(){
                $('#graficoNivel0').show();
                $('.comboGraficoNivel').change(function(){
                    $('.graficos_nivel').hide();
                    var codigoGrafico = $(this).val();
                    $('#graficoNivel'+codigoGrafico).show();
                });
            })
        </script>";

        $arrayDadosNivel = array();
        if (is_array($dadosNivel)) {
            foreach ($dadosNivel as $key) {
                $arrayDadosNivel[$key['codigo']] = $key['descricao'];
            }
        }

        $qtd = count($dadosNivel);

        if(!$escondeFiltros['instalacao']){
            echo $simec->select("comboGraficoNivel", "Instalações", 0, $arrayDadosNivel, array('class'=>' comboGraficoNivel'));
        }


        if (is_array($dadosNivel)) {
            foreach ($dadosNivel as $key) {
                echo "<div style='clear:both'></div>";
                if($key['codigo'] == "0") {
                    echo "<div style='display: none;' class='graficos_nivel' id='graficoNivel" . $key['codigo'] . "'>";
                }else{
                    echo "<div style='display: none;margin-left:-50px' class='graficos_nivel' id='graficoNivel" . $key['codigo'] . "'>";
                }

                if($key['codigo'] == "0"){
                    $dados = $painel->carregarGraficoIndicadorNivel($indid, 'todos', $muncod, $estuf);
                    $grafico = new Grafico(null, false);
                    $grafico->setTitulo($dadosNivel['tiddsc'])->setHeight('300px')->setTipo(Grafico::K_TIPO_PIZZA)->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($dados);
                }else {
                    $dados = $painel->carregarGraficoIndicadorNivel($indid, $key['codigo'], $muncod, $estuf);
                    $grafico = new Grafico(null, false);
                    $grafico->setTitulo($dadosNivel['tiddsc'])->setHeight('300px')->setTipo(Grafico::K_TIPO_PIZZA)->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($dados);
                }
                echo "</div>";
            }
        }


    }

    public function gerarTabela($indid, $indid2)
    {
        $painel = new Painel_Model_Seriehistorica();
        $dados3 = $painel->carregarTabelaIndicadoresMunicipioVinculados($indid, $indid2);?>
        <div class="row" style="margin-top: 0px;">
            <div class="col-md-12">

                <div class="ibox float-e-margins" style="margin: 25px;">
                    <div class="ibox-content">

                        <table class="table table-hover dataTable">
                            <thead>
                            <tr>

                                <th>Equipamento</th>
                                <th>Quantidade existente do equipamento</th>
                                <th>Demanda do equipamento</th>
                            </tr>
                            </thead>

                            <?php
                            if (is_array($dados3)) {
                                foreach ($dados3 as $key) {
                                    ?>
                                    <tr>

                                        <td><?php echo $key['tipo'] ?></td>
                                        <td><?php echo $key['totalequipamento'] ?></td>
                                        <td><?php echo $key['totaldemanda'] ?></td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
                        </table>
                    </div>
                </div>

            </div>
        </div>
    <?php
    }

    public function gerarTabelaEscola($indid, $indid2)
    {
        $painel = new Painel_Model_Seriehistorica();
        $dados3 = $painel->carregarTabelaIndicadoresVinculados($indid, $indid2);?>
        <div class="row" style="margin-top: 0px;">
            <div class="col-md-12">

                <div class="ibox float-e-margins" style="margin: 25px;">
                    <div class="ibox-content">

                        <table class="table table-hover dataTable">
                            <thead>
                            <tr>
                                <th>Escola</th>
                                <th>Equipamento</th>
                                <th>Quantidade existente do equipamento</th>
                                <th>Demanda do equipamento</th>
                            </tr>
                            </thead>

                            <?php
                            if (is_array($dados3)) {
                                foreach ($dados3 as $key) {
                                    ?>
                                    <tr>
                                        <td><?php echo $key['escola'] ?></td>
                                        <td><?php echo $key['tipo'] ?></td>
                                        <td><?php echo $key['totalequipamento'] ?></td>
                                        <td><?php echo $key['totaldemanda'] ?></td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
                        </table>
                    </div>
                </div>

            </div>
        </div>
    <?php
    }

    public function gerarTabelaNivel($indid, $muncod = null, $estuf = null, $escondeFiltros = array())
    {
        $painel           = new Painel_Model_Seriehistorica();
        $categoriasEscola = $painel->carregarEscolasTabelaIndicadorNivel($indid, $muncod, $estuf);

        $escolas          = $painel->carregarEscolasComboTabelaIndicadorNivel($indid, $muncod, $estuf);
        $escolas          = is_array($escolas) ? $escolas : array();

        $ambientes        = $painel->carregarAmbientesEscolasComboTabelaIndicadorNivel($indid, $muncod, $estuf);
        $ambientes        = is_array($ambientes) ? $ambientes : array();
        ?>
        <script>
            $(document).ready(function () {

                $('.filtro').change(function () {

                	<?php if($escondeFiltro['instalacao']){?>
                	var cod_ambiente  = parseFloat($('#filtroAmbiente').val());
                	<?php }else{?>
                	var cod_ambiente  = parseFloat($('#filtroAmbiente2').val());
                	<?php }?>
                	<?php if($escondeFiltro['situacao']){?>
                    var cod_acessivel = $('#filtroAcessivel').val();
                	<?php }else{?>
                    var cod_acessivel = $('#filtroAcessivel2').val();
                	<?php }?>
                	<?php if($escondeFiltro['escolas']){?>
                    var cod_escola   = parseFloat($('#filtroEscola').val());
                	<?php }else{?>
                    var cod_escola   = parseFloat($('#filtroEscola2').val());
                	<?php }?>

                    if (cod_escola > 0 && cod_ambiente > 0 && cod_acessivel != '') {
                        $('.escolas').hide();
                        $('[class*="escola_' + cod_escola + '"][class*="ambiente_' + cod_ambiente + '"][class*="acessivel_' + cod_acessivel + '"]').show();
                    }else if (cod_escola > 0 && cod_ambiente > 0 ) {
                        $('.escolas').hide();
                        $('[class*="escola_' + cod_escola + '"][class*="ambiente_' + cod_ambiente + '"]').show();
                    }else if (cod_escola > 0 && cod_acessivel != '' ) {
                        $('.escolas').hide();
                        $('[class*="escola_' + cod_escola + '"][class*="acessivel_' + cod_acessivel + '"]').show();
                    }else if (cod_ambiente > 0 && cod_acessivel != '' ) {
                        $('.escolas').hide();
                        $('[class*="ambiente_' + cod_ambiente + '"][class*="acessivel_' + cod_acessivel + '"]').show();
                    } else if (cod_escola > 0) {
                        $('.escolas').hide();
                        $('.escola_' + cod_escola).show();
                    } else if (cod_ambiente > 0) {
                        $('.escolas').hide();
                        $('.ambiente_' + cod_ambiente).show();
                    } else if (cod_acessivel != '') {
                        $('.escolas').hide();
                        $('.acessivel_' + cod_acessivel).show();
                    } else {
                        $('.escolas').show();
                    }
                });
            });
        </script>
        <div class="row" style="margin-top: 0px;">
            <div class="col-md-12">
                <div class="ibox float-e-margins" style="margin: 5px;z-index: 9999">
                    <?php if(!$escondeFiltros['escolas'] || !$escondeFiltros['instalacao'] || !$escondeFiltros['situacao']){?>
                    <div style="font-size: 10px;padding: 10px;text-align:left">
                        <h4>Filtros:</h4>
                        <div class="row">
                        <?php if(!$escondeFiltros['escolas']){?>
                            <div class="col-md-4">
                                Escolas:</br>
                                <select class="filtro form-control chosen-select" id="filtroEscola2">
                                    <option value="">Todas</option>
                                    <?php foreach ($escolas as $escola) { ?>
                                        <option
                                            value="<?php echo $escola['cod_escola']; ?>"><?php echo $escola['escola']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        <?php }?>
                        <?php if(!$escondeFiltros['instalacao']){?>
                            <div class="col-md-4">
                                Instalação: </br>
                                <select class="filtro form-control chosen-select" id="filtroAmbiente2">
                                    <option value="">Todas</option>
                                    <?php foreach ($ambientes as $ambiente) { ?>
                                        <option
                                            value="<?php echo $ambiente['cod_ambiente']; ?>"><?php echo $ambiente['ambiente']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                        <?php }?>
                        <?php if(!$escondeFiltro['situacao']){?>
                            <div class="col-md-4">
                                Situação:<br>
                                <select class="filtro form-control chosen-select" id="filtroAcessivel2">
                                    <option value="">Todas</option>
                                    <option value="fa-check">Existe</option>
                                    <option value="fa-times">Não Existe</option>
                                    <option value="fa-thumbs-o-up">Sua falta não prejudica o PPP da escola</option>
                                    <option value="fa-thumbs-o-down">Sua falta prejudica o PPP da escola</option>
                                    <option value="fa-font">Adequada</option>
                                    <option value="fa-wheelchair">Acessível</option>
                                </select>
                            </div>
                        <?php }?>
                        </div>
                    </div>
                    <?php }?>
                    <div class="legendas" style="font-size: 10px;padding: 10px;">
                    	<span class="label"><i class="fa fa-check"></i> Existe</span>
                        <span class="label"><i class="fa fa-times"></i> Não existe</span>
                        <span class="label"><i class="fa fa-thumbs-o-up"></i> Sua falta não prejudica o PPP da escola</span>
                        <span class="label"><i class="fa fa-thumbs-o-down"></i> Sua falta prejudica o PPP da escola</span>
                        <span class="label"><i class="fa fa-font"></i> Adequada</span>
                        <span class="label"><i class="fa fa-wheelchair"></i> Acessível</span>
                    </div>
                    <div>
                        <style>
                            .table_escolas thead, .table_escolas tbody, .table_escolas tr, .table_escolas td, .table_escolas th { display: block; }

	                        .table_escolas tr:after {
	                            content: ' ';
	                            display: block;
	                            visibility: hidden;
	                            clear: both;
	                        }
	
	                        .table_escolas thead th {
	                            height: 30px;
	                        }
	
	                        .table_escolas tbody {
	                            height: 300px;
	                            overflow-y: auto;
	                        }
	
	                        .table_escolas tbody td, .table_escolas thead th {
	                            width: 33.333%;
	                            float: left;
	                        }
	
	                        .icone_grafico{
	                            font-size: 12px;
	                        }
                        </style>
                        <table class="table table-hover table_escolas">
                            <thead>
                            <tr>
                                <th style="text-align: left; width: 36%">Escola</th>
                                <th style="text-align: left; width: 37%">Instalação</th>
                                <th style="text-align: left; width: 25%">Situação</th>
                            </tr>
                            </thead>
                            <?php
                            if (is_array($categoriasEscola)) {
                                foreach ($categoriasEscola as $key) {
                                    ?>
                                    <tr class="escolas escola_<?php echo $key['cod_escola']; ?> ambiente_<?php echo $key['cod_categoria']; ?> <?php
                                            if ($key['id'] == 9481) echo "acessivel_fa-check";
                                            if ($key['id'] == 9482) echo "acessivel_fa-times acessivel_fa-thumbs-o-down";
                                            if ($key['id'] == 9483) echo "acessivel_fa-times acessivel_fa-thumbs-o-up";
                                            if ($key['id'] == 9478) echo "acessivel_fa-check acessivel_fa-font acessivel_fa-wheelchair";
                                            if ($key['id'] == 9480) echo "acessivel_fa-check acessivel_fa-wheelchair";
                                            if ($key['id'] == 9479) echo "acessivel_fa-check acessivel_fa-font";
                                        ?>">
                                        <td style="text-align: left; width: 37%"><?php echo $key['escola'] ?></td>
                                        <td style="text-align: left; width: 38%"><?php echo $key['categoria'] ?></td>
                                        <td style="text-align: left; width: 25%">
                                            <?php if ($key['id'] == 9481) { ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9482) {
                                                ?>
                                                <span class="label"><i class="fa fa-times icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-thumbs-o-down icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9483) {
                                                ?>
                                                <span class="label"><i class="fa fa-times icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-thumbs-o-up icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9478) {
                                                ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-font icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-wheelchair icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9480) {
                                                ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-wheelchair icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9479) {
                                                ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-font icone_grafico" ></i></span>
                                            <?php } ?>
                                        </td>
                                    </tr>
                                <?php
                                }
                            } ?>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    <?php
    }//end gerarTabelaNivel();

    public function gerarTabelaNivelMunicipio($indid, $muncod = null, $estuf = null, $escondeFiltro = array())
    {
        $painel = new Painel_Model_Seriehistorica();
        $categoriasEscola = $painel->carregarListaIndicadorNivel($indid, 'municipio', $muncod, $estuf);

        $ambientes = $painel->carregarAmbientesEscolasComboTabelaIndicadorNivel($indid, $muncod, $estuf);
        $ambientes = is_array($ambientes) ? $ambientes : array();
        ?>
        <script>
            $(document).ready(function () {
                $('.filtro1').change(function () {

                	<?php if($escondeFiltro['instalacao']){?>
                	var cod_ambiente  = parseFloat($('#filtroAmbiente').val());
                	<?php }else{?>
                	var cod_ambiente  = parseFloat($('#filtroAmbiente1').val());
                	<?php }?>
                	<?php if($escondeFiltro['situacao']){?>
                    var cod_acessivel = $('#filtroAcessivel').val();
                	<?php }else{?>
                    var cod_acessivel = $('#filtroAcessivel1').val();
                	<?php }?>

                    if (cod_ambiente > 0 && cod_acessivel != '') {
                        $(".ambiente").hide();
                        $('[class*="ambiente1_'+cod_ambiente+'"][class*="acessivel_'+cod_acessivel+'"]').show();
                    }else if (cod_ambiente > 0) {
                        $(".ambiente").hide();
                        $('.ambiente1_'+cod_ambiente).show();
                    }else if (cod_acessivel != '') {
                        $(".ambiente").hide();
                        $('.acessivel_'+cod_acessivel).show();
                    }else{
                        $(".ambiente").show();
                    }
                });
            });
        </script>
        <div class="row" style="margin-top: 0px;">
            <div class="col-md-12">
                <div class="ibox float-e-margins" style="margin: 5px;z-index: 9999">
                    <?php if(!$escondeFiltro['instalacao']||!$escondeFiltro['situacao']){?>
                    <div style="font-size: 10px;padding: 10px;text-align:left">
                        <h4>Filtros:</h4>

                        <div class="row">
                            <?php if(!$escondeFiltro['situacao']){?>
                            <div class="col-md-6">
                                Instalação:<br>
                                <select class="filtro1 form-control chosen-select" id="filtroAmbiente1">
                                    <option value="">Todas</option>
                                    <?php foreach ($ambientes as $ambiente) { ?>
                                        <option
                                            value="<?php echo $ambiente['cod_ambiente']; ?>"><?php echo $ambiente['ambiente']; ?></option>
                                    <?php } ?>
                                </select>
                            </div>
                            <?php }?>
                            <?php if(!$escondeFiltro['situacao']){?>
                            <div class="col-md-6">
                                Situação:<br>
                                <select class="filtro1 form-control chosen-select" id="filtroAcessivel1">
                                    <option value="">Todas</option>
                                    <option value="fa-check">Existe</option>
                                    <option value="fa-times">Não Existe</option>
                                    <option value="fa-thumbs-o-up">Sua falta não prejudica o PPP da escola</option>
                                    <option value="fa-thumbs-o-down">Sua falta prejudica o PPP da escola</option>
                                    <option value="fa-font">Adequada</option>
                                    <option value="fa-wheelchair">Acessível</option>
                                </select>
                            </div>
                            <?php }?>
                        </div>
                    </div>
                    <?php }?>
                    <div class="legendas" style="font-size: 10px;  padding: 10px;">
                    	<span class="label"><i class="fa fa-check"></i> Existe</span>
                        <span class="label"><i class="fa fa-times"></i> Não existe</span>
                        <span class="label"><i class="fa fa-thumbs-o-up"></i> Sua falta não prejudica o PPP da escola</span>
                        <span class="label"><i class="fa fa-thumbs-o-down"></i> Sua falta prejudica o PPP da escola</span>
                        <span class="label"><i class="fa fa-font"></i> Adequada</span>
                        <span class="label"><i class="fa fa-wheelchair"></i> Acessível</span>
                    </div>
                    <div>
                        <style>
                        .table_municipal thead, .table_municipal tbody, .table_municipal tr, .table_municipal td, .table_municipal th { display: block; }
		
                        .table_municipal tr:after {
                            content: ' ';
                            display: block;
                            visibility: hidden;
                            clear: both;
                        }

                        .table_municipal thead th {
                            height: 30px;
                        }

                        .table_municipal tbody {
                            height: 300px;
                            overflow-y: auto;
                        }

                        .table_municipal thead {
                            /* fallback */
                        }


                        .table_municipal tbody td, .table_municipal thead th {
                            width: 33.33%;
                            float: left;
                        }

                        .legendas span {
                        	margin-bottom: 4px;
                        	padding: 6px;
                        	display: inline-table;
                        }
                        </style>
                        <table class="table table-hover table_municipal">
                            <thead>
                            <tr>
                                <th style="text-align: left; width: 53.5%">Instalação</th>
                                <th style="text-align: left; width: 14.5%">Quantidade</th>
                                <th style="text-align: left; width: 25%">Situação</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            if (is_array($categoriasEscola)) {
                                foreach ($categoriasEscola as $key) {
                                    ?>
                                    <tr
                                        class="ambiente ambiente1_<?php echo $key['cod_ambiente']?> <?php
                                            if ($key['id'] == 9481) echo "acessivel_fa-check";
                                            if ($key['id'] == 9482) echo "acessivel_fa-times acessivel_fa-thumbs-o-down";
                                            if ($key['id'] == 9483) echo "acessivel_fa-times acessivel_fa-thumbs-o-up";
                                            if ($key['id'] == 9478) echo "acessivel_fa-check acessivel_fa-font acessivel_fa-wheelchair";
                                            if ($key['id'] == 9480) echo "acessivel_fa-check acessivel_fa-wheelchair";
                                            if ($key['id'] == 9479) echo "acessivel_fa-check acessivel_fa-font";
                                        ?>">
                                        <td style="text-align: left; width: 55%"><?php echo $key['categoria'] ?></td>
                                        <td style="text-align: left; width: 15%"><?php echo $key['valor'] ?></td>
                                        <td style="text-align: left; width: 25%">
                                            <?php
                                            if ($key['id'] == 9481) { ?>
                                                <span class="label"><i class="fa fa-check icone_grafico"></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9482) {
                                                ?>
                                                <span class="label"><i class="fa fa-times icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-thumbs-o-down icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9483) {
                                                ?>
                                                <span class="label"><i class="fa fa-times icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-thumbs-o-up icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9478) {
                                                ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-font icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-wheelchair icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9480) {
                                                ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-wheelchair icone_grafico" ></i></span>
                                            <?php
                                            }
                                            if ($key['id'] == 9479) {
                                                ?>
                                                <span class="label"><i class="fa fa-check icone_grafico" ></i></span>
                                                <span class="label"><i class="fa fa-font icone_grafico" </i></span>
                                            <?php } ?>
                                        </td>
                                    </tr>
                                <?php
                                }
                            } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    <?php
    }//end gerarTabelaNivel();

}//end Class();

?>