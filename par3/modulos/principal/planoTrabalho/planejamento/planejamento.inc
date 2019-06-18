<?php
checkParamInuid();
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$controleIndicadorINEP = new Par3_Controller_IndicadorINEP();
$controleUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelAuditoriaUnidade 	 = new Par3_Model_AuditoriaUnidade();
$oIndicador = new Par3_Model_Indicador();
$oIndicadorMeta = new Par3_Model_IndicadorMeta();
$oAreaController = new Par3_Controller_Area();
$oPontuacaoController = new Par3_Controller_Pontuacao();
$oPontuacao = new Par3_Model_Pontuacao();
$oPontuacaoMeta = new Par3_Model_PontuacaoMeta();
$modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);

$disabled = '';

$existePendenciaGeral = $_SESSION['visualizar-sintese-diagnostico'] ? FALSE : Par3_Controller_Diagnostico::verificarPendencia($_REQUEST['inuid']);

$verificaInicioDiagnostico = ($modelAuditoriaUnidade->verificarInicioDiagnostico($_REQUEST['inuid']) && !$existePendenciaGeral);

$itrid = $controleUnidade->pegarItrid($_REQUEST['inuid']);
$esdid = $controleUnidadeEntidade->recuperarEstadoWorkflow($_REQUEST['inuid']);

if ($itrid == 2) {
    $nivel = "município";
} else if ($itrid == 3) {
    $nivel = "distrito";
} else {
    $nivel = "estado";
}

if ($_REQUEST['visualizar-sintese-diagnostico'])

    $iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido = '<span class="fa fa-check iconUnidade"' .
    'style="color:green !important;' .
    'background-color: transparent;"></span>';

$a = 0;
$arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid);

require APPRAIZ . 'includes/cabecalho.inc';
?>
<style>
    .colunaIndicadores {
        background: #f3f3f4;
        margin-top: -20px;
        margin-left: -6px;
        padding: 20px;
        height: 100%;
    }

    .btn-circled, .btn-circled:hover, .btn-circled:visited, .btn-circled:active {
        background-color: #347AB8;
        font-size: 30px;
        font-weight: bold;
        width: 100px;
        height: 100px;
        border-radius: 50%;
        -webkit-border-radius: 50%;
        -moz-border-radius: 50%;
        cursor: auto;
    }

    .pontuacao {
        font-size: 20px;
        font-weight: bold;
        color: #347AB8;
    }

    .nav-tabs li a {
        font-size: 12px;
    }

    .form-horizontal .form-group {
        margin: 0px -2px 10px -4px;
    }

    .menuSelecionado {
        cursor: default !important;
        font-weight: bold;
    }

    .esconde-div {
        display: none;
    }

    .panel-heading {
        height: 50px;
    }

    .progress {
        background-color: #969696;
    }

    .pie {
        display: none;
        visibility: hidden;
    }

    .peity {
        vertical-align: bottom;
    }

    .alinhajust {
        text-align: justify !important;
    }

    .no-margin {
        padding: 0px !important;
    }

    .pendencia:hover {
        color: #676a6c !important;
        text-decoration: underline;
    }

    .title-tables {
        color: #333;
    }

    .peity {
        vertical-align: sub !important;
    }

    .progress-bar-success span {
        color: #fff;
    }
</style>

<script src="js/bootstrap-tour.min.js"></script>
<script src="js/tour/indicadoresQualitativos.js"></script>

<script>
</script>

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <input type="hidden" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>

            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <div class="ibox-content">
                <?php $controleUnidade->cabecalhoUnidade(); ?>
                <div class="ibox-content-round-gray ibox-content">
                    <?php
                    $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid, $dimid);

                    $_REQUEST['dimid'] = $_REQUEST['dimid'] ? $_REQUEST['dimid'] : $arDimensoes[0]['dimid'];

                    $aba = $_REQUEST['resumo'] ? "&resumo=1" : "&dimid=" . $_REQUEST['dimid'];

                    $url = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=' . $_REQUEST['inuid'] . $aba;
                    ?>
                    <div class="tabs-container">
                        <div class="tab-content">







                            <div class="wrapper wrapper-content animated fadeIn" id="div_busca">
                                <div class="row">
                                    <div class="ibox float-e-margins">
                                        <div class="ibox-title">
                                            <h5>
                                                Busca de Iniciativas
                                            </h5>
                                        </div>
                                        <div class="ibox-content">
                                            <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                                                <?php
                                                echo $simec->textarea('pindsc', 'Descrição', $dados['inidsc'], array('placeHolder' => '', 'maxlengh' => 1200, $boIndicador));

                                                $tiposA = array(
                                                    "G" => "Global",
                                                    "E" => "Por escola",
                                                );

                                                echo $simec->select('pincronograma', 'Cronograma', $dados['pincronograma'], $tiposA, array('placeHolder' => 'Cronograma', 'maxlengh' => 1200));

                                                $tiposA = array(
                                                    "2017" => "2017",
                                                    "2018" => "2018",
                                                    "2019" => "2019",
                                                );

                                                echo $simec->select('pinano[]', 'Anos', array(), $tiposA, array('placeHolder' => 'Anos', 'maxlengh' => 1200, $disabled));

                                                $sql = "SELECT
                            	obiid  as codigo,
                            	obidesc  as descricao
                            FROM par3.objetoiniciativa
                            where obistatus = 'A'
                            ORDER BY 2";

                                                echo $simec->select('obiid[]', 'Tipo de Objeto', $dados['obiid'], $sql, array('placeHolder' => 'Objeto Iniciativa', 'maxlengh' => 1200));

                                                $sql = "SELECT
                            	aciid as codigo,
                            	acidesc as descricao
                            FROM par3.areascorrelatasiniciativa
                            WHERE acistatus = 'A'
                            ORDER BY 2";

                                                echo $simec->select('aciid[]', 'Areas Relacionadas', $dados['aciid'], $sql, array('placeHolder' => 'Areas Relacionadas', 'maxlengh' => 1200));

                                                $tiposA = array(
                                                    "E" => "Estadual",
                                                    "M" => "Municipal",
                                                    "D" => "Distrital",
                                                );

                                                echo $simec->select('esfera[]', 'Esfera', array(), $tiposA, array('placeHolder' => 'Esfera', 'maxlengh' => 1200, $disabled));

                                                $sql = "SELECT
                            	estuf as codigo,
                            	estdescricao as descricao
                            FROM territorios.estado
                            ORDER BY 2";

                                                echo $simec->select('estados[]', 'Estados', array(), $sql, array('placeHolder' => 'Estados', 'maxlengh' => 1200, $disabled));

                                                $sql = "SELECT
                            	gtmid as codigo,
                            	gtmdsc as descricao
                            FROM territorios.grupotipomunicipio
                            WHERE
                            	gtmstatus = 'A'
                            ORDER BY 2";

                                                echo $simec->select('grupo_municipio[]', 'Grupo de Municipios', array(), $sql, array('placeHolder' => 'Grupo de Municipios', 'maxlengh' => 1200, $disabled));

                                                $sql = "SELECT
                            	dimid as codigo,
                            	dimcod||' - '||dimdsc||' ('||itr.itrdsc||')' as descricao
                            FROM par3.dimensao dim
                            INNER JOIN par3.instrumento itr ON itr.itrid = dim.itrid
                            ORDER BY
                            	itr.itrdsc, dimcod";

                                                echo $simec->select('dimensao[]', 'Dimenssões', array(), $sql, array('placeHolder' => 'Dimenssões', 'maxlengh' => 1200, $disabled));

                                                $sql = "SELECT
                            	dim.dimid as codigo,
                            	dim.dimcod||'.'||are.arecod||' - '||are.aredsc||' ('||itr.itrdsc||')' as descricao
                            FROM par3.dimensao dim
                            INNER JOIN par3.instrumento 	itr ON itr.itrid = dim.itrid
                            INNER JOIN par3.area 		are ON are.dimid = dim.dimid
                            ORDER BY
                            	itr.itrdsc, dim.dimcod, are.arecod
                            LIMIT 30";

                                                echo $simec->select('area[]', 'Areas', array(), $sql, array('placeHolder' => 'Areas', 'maxlengh' => 1200, $disabled));

                                                $sql = "SELECT
                            	dim.dimid as codigo,
                            	dim.dimcod||'.'||are.arecod||'.'||ind.indcod||' - '||ind.indcod||' ('||itr.itrdsc||')' as descricao
                            FROM par3.dimensao dim
                            INNER JOIN par3.instrumento 	itr ON itr.itrid = dim.itrid
                            INNER JOIN par3.area 		are ON are.dimid = dim.dimid
                            INNER JOIN par3.indicador	ind ON ind.areid = are.areid
                            ORDER BY
                            	itr.itrdsc, dim.dimcod, are.arecod, ind.indcod
                            LIMIT 30";

                                                echo $simec->select('indicador[]', 'Indicadores', array(), $sql, array('placeHolder' => 'Indicadores', 'maxlengh' => 1200, $disabled));
                                                ?>
                                                <div class="form-group">
                                                    <div class="col-lg-12 col-sm-offset-3">
                                                        <button type="button" class="btn btn-success">
                                                            <span class="fa fa-search"> </span> Pesquisar
                                                        </button>
                                                    </div>
                                                </div>
                                            </form>
                                        </div>
                                        <div class="ibox-content">
                                            <?php
                                            $sql = "SELECT
                        	pps.ppsid,
                        	CASE ppsid%3
                        		WHEN 0 THEN 'Estadual'
                        		WHEN 1 THEN 'Municipal'
                        		WHEN 2 THEN 'Distrito Federal'
                        	END as esfera,
                            pps.ppsdsc,
                        	CASE ppsid%3
                        		WHEN 0 THEN 'Climatização'
                        		WHEN 1 THEN 'Transporte'
                        		WHEN 2 THEN 'Formação'
                        	END as objeto,
                        	CASE ppscronograma
                        		WHEN 1 THEN 'Global'
                        		ELSE 'Por escola'
                        	END as cronograma
                        FROM par.propostasubacao pps
                        WHERE ppsnovopar = 'S'";

                                            $cabecalho = array('Esfera', 'Iniciativa', 'Objeto da Iniciativa', 'Cronograma');

                                            $listagem = new Simec_Listagem();
                                            $listagem->setCabecalho($cabecalho);
                                            $listagem->setQuery($sql);
                                            $listagem->addAcao('view', 'mostraIniciativa', array('titulo' => 'Visualisar'));

                                            $listagem->setFormFiltros('formulario');
                                            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                                            $listagem->turnOnOrdenacao('estuf');
                                            $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
                                            ?>
                                        </div>
                                    </div>
                                </div>
                            </div>




                            <div class="wrapper wrapper-content animated fadeIn" id="div_iniciativa">
                                <div class="row">
                                    <div class="ibox float-e-margins">
                                        <div class="ibox-title">
                                            <h5>
                                                Iniciativas de Veiculos
                                            </h5>
                                        </div>
                                        <div class="ibox-content">
                                            <div class="row">
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Descrição: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">Iniciativa voltada para planejamento de aquisição de veículos </div>
                                                    </div>
                                                </div>
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Cronograma: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">Global</div>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Areas Relacionadas: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">SEB e FNDE</div>
                                                    </div>
                                                </div>
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Nível: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">Educação Básica</div>
                                                    </div>
                                                </div>
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Etapa: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">Educação Infantil, Educação Fundamental</div>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Modalidade: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">Ensino Regular, Ensino Especial</div>
                                                    </div>
                                                </div>
                                                <div class="col-lg-6">
                                                    <div class="row">
                                                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Desdobramento: </label>
                                                        <div class="col-sm-9 col-md-9 col-lg-9">Urbano, Privado</div>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="ibox-content">
                                            <div class="row">
                                                <div class="col-lg-12">
                                                    <table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;" colspan="6">
                                                                Modalidades Planejadas
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Nível
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Etapa
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Modalidade
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Desdobramento
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Quantidade
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <td rowspan="2">Educação Básica</td>
                                                            <td rowspan="2">Educação Infantil</td>
                                                            <td rowspan="2">Ensino Regular</td>
                                                            <td><a>Urbano</a></td>
                                                            <td>12&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td><a>Privado</a></td>
                                                            <td>10&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td rowspan="2">Educação Básica</td>
                                                            <td rowspan="2">Educação Infantil</td>
                                                            <td rowspan="2">Ensino Especial</td>
                                                            <td><a>Urbano</a></td>
                                                            <td>3&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td><a>Privado</a></td>
                                                            <td>5&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td rowspan="2">Educação Básica</td>
                                                            <td rowspan="2">Educação Fundamental</td>
                                                            <td rowspan="2">Ensino Regular</td>
                                                            <td><a>Urbano</a></td>
                                                            <td>7&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td><a>Privado</a></td>
                                                            <td>1&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td rowspan="2">Educação Básica</td>
                                                            <td rowspan="2">Educação Fundamental</td>
                                                            <td rowspan="2">Ensino Especial</td>
                                                            <td><a>Urbano</a></td>
                                                            <td>5&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                        <tr>
                                                            <td><a>Privado</a></td>
                                                            <td>0&nbsp;<i class="fa fa-search" aria-hidden="true" style="cursor:pointer;"></i></td>
                                                        </tr>
                                                    </table>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="ibox-content">
                                            <div class="row">
                                                <div class="col-lg-8">
                                                    <table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;" colspan="6">
                                                                Planilha de Composição de Itens
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;" colspan="2">
                                                                Definição de Itens
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;" colspan="4">
                                                                Quantidade
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Item
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;width:20%">
                                                                Unidade de Medida
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;width:10%">
                                                                2017
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;width:10%">
                                                                2018
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;width:10%">
                                                                2019
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;width:10%">
                                                                2020
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <td>Ônibus Escolar</td>
                                                            <td>Unidade</td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                        </tr>
                                                        <tr>
                                                            <td>Micro-Ônibus Escolar</td>
                                                            <td>Unidade</td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                        </tr>
                                                        <tr>
                                                            <td>Van Escolar</td>
                                                            <td>Unidade</td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                        </tr>
                                                        <tr>
                                                            <td>Embarcação Escolar</td>
                                                            <td>Unidade</td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                        </tr>
                                                        <tr>
                                                            <td>Bicicleta Escolar</td>
                                                            <td>Unidade</td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                        </tr>
                                                        <tr>
                                                            <td colspan="2">Totais</td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'12'))?></td>
                                                        </tr>
                                                    </table>
                                                </div>
                                                <div class="col-lg-4">
                                                    <table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;" colspan="2">
                                                                &nbsp;
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;" colspan="2">
                                                                Total planejado
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;">
                                                                Total planejado
                                                            </th>
                                                            <th align="center" style="background-color: #E0E0E0;text-align:center;width:50%">
                                                                Média por ano
                                                            </th>
                                                        </tr>
                                                        <tr>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'6'))?>{total}</td>
                                                            <td>{total/qtdanos}</td>
                                                        </tr>
                                                        <tr>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'6'))?>{total}</td>
                                                            <td>{total/qtdanos}</td>
                                                        </tr>
                                                        <tr>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'6'))?>{total}</td>
                                                            <td>{total/qtdanos}</td>
                                                        </tr>
                                                        <tr>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'6'))?>{total}</td>
                                                            <td>{total/qtdanos}</td>
                                                        </tr>
                                                        <tr>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'6'))?>{total}</td>
                                                            <td>{total/qtdanos}</td>
                                                        </tr>
                                                        <tr>
                                                            <td><?php echo $simec->input('qtd', '', $qtd, array('maxlength' => '10', true, ''), array('input-size'=>'6'))?>{total}</td>
                                                            <td>{total/qtdanos}</td>
                                                        </tr>
                                                    </table>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-lg-12 col-sm-offset-8">
                                                    <button type="button" class="btn btn-success">
                                                        <span class="fa fa-floppy-o"> </span> Salvar Planejamento
                                                    </button>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="ibox-content">
                                            <h5> Graficos e indicadores de apoio </h5>
                                        </div>
                                        <div class="ibox-content">
                                            <h5> Oferta de Ações, Programas e Projetos</h5>
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <script>
                                function mostraIniciativa(acao, tipo, id, itrid) {
                                    $('#div_busca').hide();
                                    $('#div_iniciativa').show();
                                }
                            </script>






                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>