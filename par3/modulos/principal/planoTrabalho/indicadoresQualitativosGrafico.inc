<?php
$IndicadorSimec = new Simec_Indicador();
$infoIndicador = new Painel_Model_Seriehistorica();
$iue = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);

$dadosIndicador1 = $infoIndicador->detalhamentoIndicador(3205);
$titulo          .= $dadosIndicador1[0]['indnome'];
$titulo          .= " X ";

$dadosIndicador2 = $infoIndicador->detalhamentoIndicador(3272);
$dadosIndicador3 = $infoIndicador->detalhamentoIndicador(3207);
$titulo          .= $dadosIndicador2[0]['indnome'];

$abaEsfera       = $iue->itrid == 2 ? 'Municipal' : 'Estadual';
$muncod          = $iue->itrid == 2 ? $iue->muncod : null;
$estuf           = $iue->itrid != 2 ? $iue->estuf  : null;

$painel = new Painel_Model_Seriehistorica();

$ambientes       = $infoIndicador->carregarAmbientesEscolasComboTabelaIndicadorNivel(3207, $muncod, $estuf);
$ambientes       = is_array($ambientes) ? $ambientes : array();

$escolas          = $infoIndicador->carregarEscolasComboTabelaIndicadorNivel(3207, $muncod, $estuf);
$escolas          = is_array($escolas) ? $escolas : array();

$qtdTotalEscolas = $infoIndicador->carregarQuantidadeEscolasEducacenso( $muncod, $estuf );
$percentEscolas = ( count($escolas) / $qtdTotalEscolas ) * 100;
?>
    <div class="row">
        <div class="col-md-4">
            <center>
            <?php if ($iue->itrid == 2) : ?>
                <img
                    src="../imagens/bandeiras/<?php echo strtolower($iue->estuf) ?>/<?php echo strtolower(remove_acentos($iue->inudescricao)) ?>.jpg"
                    style="width: 200px;">
            <?php else : ?>
                <img src="../imagens/bandeiras/bandeira_<?php echo $iue->estuf ?>.jpg" style="width: 250px;">
            <?php endif; ?>
            </center>
        </div>
        <div class="col-md-8">
        	<div class="row">
	        	<div class="ibox-title">
				    <h4><?php echo $dadosIndicador3[0]['indnome']; ?></h4>
				</div>
	            <div class="tabs-container">
	                <ul class="nav nav-tabs">
	                    <li class="active"><a data-toggle="tab" href="#tab-1" aria-expanded="true"><?php echo $abaEsfera; ?></a></li>
	                    <li class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Escolas</a>
	                    </li>
	                </ul>
	                <div class="tab-content">
	                    <div id="tab-1" class="tab-pane active">
	                        <div class="panel-body">
						        <div class="alert alert-success">
						            Os dados se referem a <?php echo count($escolas); ?> escolas municipais da rede de um total de <?php echo $qtdTotalEscolas; ?>, totalizando <?php echo number_format($percentEscolas,2)?>%, que é o percentual daquelas que preencheram o PDDE Interativo do ano anterior.
						        </div>
	                            <?php $IndicadorSimec->gerarTabelaNivelMunicipio(3207, $muncod, $estuf); ?>
	                        </div>
	                    </div>
	                    <div id="tab-2" class="tab-pane">
	                        <div class="panel-body">
						        <div class="alert alert-success">
						            Os dados se referem a <?php echo count($escolas); ?> escolas estaduais da rede de um total de <?php echo $qtdTotalEscolas; ?>, totalizando <?php echo number_format($percentEscolas,2)?>%, que é o percentual daquelas que preencheram o PDDE Interativo do ano anterior.
						        </div>
	                            <?php $IndicadorSimec->gerarTabelaNivel(3207, $muncod, $estuf); ?>
	                        </div>
	                    </div>
	                </div>
	            </div>
			</div>
        </div>
    </div>