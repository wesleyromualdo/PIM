<?php
set_time_limit(10800);

$obEmenda = new Par3_controller_EmendasParlamentares();
$obModelEmenda = new Par3_Model_EmendasParlamentares();

switch ($_REQUEST['requisicao']) {
	case 'exportar':
	    ini_set('memory_limit', '-1');
	    
	    $sql = $obModelEmenda->sqlCarregaRelatorioParlamentar($_REQUEST);
	    
	    $arrayCabecalho = array('Emenda','Ano', 'Partido', 'Parlamentar', 'CNPJ', 'Beneficiário', 'UF', 'Objeto' , 'Planejamento','Valor da Emenda', 'Valor Designado', 'Valor Indicado', 'Processo', 'Termo', 'Valor Processo', 'Contrapartida', 'Valor Empenhado', 'Nota de empenho', 'Valor Pago', 'Situação');
	    $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
	    $list->setCabecalho($arrayCabecalho);
	    $list->setQuery( $sql );
	    $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
		die;
		break;
	case 'carregaMunicipios':
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'"))); die;
		break;
}

$sqlPartido = $obModelEmenda->sqlPartido();
$sqlAutor = $obModelEmenda->sqlAutor();
$sqlAnoExercicio = $obModelEmenda->sqlAnoExercicio();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$estado = new Territorios_Model_Estado();
$sqlSituacao = $obModelEmenda->sqlSituacao();
$sqlObjeto = $obModelEmenda->sqlObjeto();


require APPRAIZ . 'includes/cabecalho.inc';
?>
<form method="post" name="formulario-relatorio" id="formulario-relatorio" class="form form-horizontal">
	<input type="hidden" name="muncod_hid" value="<?php echo  $_REQUEST['muncod'];?>">
	<input type="hidden" name="requisicao" value="<?php echo $_REQUEST['requisicao'] ?>">
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-lg-8" >
        	       <h3 id="entidade">Relatório Parlamentar</h3>
                </div>
    		</div>
    	</div>
    	<div class="ibox-content">
    		<div class="row">
                <div class="col-lg-6">
                    <?php echo $simec->select('estuf','UF', $_POST['estuf'], $estado->pegarSQLSelect( $_REQUEST ), array()); ?>
        			<?php echo $simec->select('muncod', 'Município', $_POST['muncod'], $arrMunicipio, array('data-placeholder' => 'Município')); ?>
                    <?php echo $simec->input('emeid', 'Emenda', $_REQUEST['emeid'], array('type' => 'number'),array('input-size' => '9')); ?>
                    <?php echo $simec->select('esdid', 'Situação', $_POST['esdid'], $sqlSituacao, array('data-placeholder' => 'Situação')); ?>
                </div>
                <div class="col-lg-6">
                    <?php echo $simec->select('parid', 'Partido', $_REQUEST['parid'], $sqlPartido, array('maxlength' => '255'), array('input-size' => '8')); ?>
                    <?php echo $simec->select('autid', 'Parlamentar', $_REQUEST['autid'], $sqlAutor, array('maxlength' => '255'), array('input-size' => '8')); ?>
                    <?php echo $simec->select('prsano', 'Ano', $_REQUEST['prsano'], $sqlAnoExercicio, array('maxlength' => '255'), array('input-size' => '8')); ?>
                    <?php echo $simec->select('intoid', 'Objeto', $_REQUEST['intoid'], $sqlObjeto, array('maxlength' => '255'), array('input-size' => '8')); ?>
                </div>
			</div>
			<div class="row">
				<div class="col-lg-6">
    				<div class="form-group" >
                        <label class=" col-sm-3 control-label">Iniciativas Indicadas pela Entidade:</label>
                        <div class="col-sm-12 col-md-9 col-lg-4">
                            <div id="iniciativa_entidade">
                                <div class="radio radio-default radio-inline ">
                                    <input type='radio' name='iniciativa_entidade' id='iniciativa_entidade_s' value="S" <?= $_REQUEST['iniciativa_entidade'] == 'S'?'checked':'';?>>
                                    <label for="iniciativa_entidade_s">Sim</label>
                                </div>
                                <div class="radio radio-default radio-inline ">
                                    <input type="radio" name="iniciativa_entidade" id="iniciativa_entidade_n" value="N"  <?= $_REQUEST['iniciativa_entidade'] == 'N'?'checked':'';?>>
                                    <label for="iniciativa_entidade_n">Não</label>
                                </div>
                                <div class="radio radio-default radio-inline ">
                                    <input type="radio" name="iniciativa_entidade" id="iniciativa_entidade_t" value="T"  <?= $_REQUEST['iniciativa_entidade'] != 'S' && $_REQUEST['iniciativa_entidade'] != 'N'?'checked':'';?>>
                                    <label for="iniciativa_entidade_t">Todos</label>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
			</div>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-12" style="text-align: center;">
					<button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
					<button type="button" class="btn btn-primary btn-export" ><i class="fa fa-list"></i> Exportar Excel</button>
					<button type="button" class="btn btn-success" id="btn-limpar"><i class="fa fa-eraser"></i> Limpar</button>
				</div>
			</div>
    	</div>
    </div>
</form>
<div class="float-e-margins" id="listagem-parlamentar">
    <div class="ibox-content">
        <div class="table-responsive" style="overflow: auto;">
                <?php
                if ($_REQUEST['requisicao'] == 'pesquisar') {
                    $obEmenda->carregaRelatorioParlamentar($_REQUEST);
                }
                ?>
        </div>
    </div>
</div>

<script>
jQuery(document).ready(function() {
	var muncod_hid = jQuery('[name="muncod_hid"]').val();

	jQuery('#muncod').append(new Option('', ''));

	if (jQuery('[name="estuf"]').val() != '') {
		carregarMunicipio( jQuery('[name="estuf"]').val(), muncod_hid );
	}

	jQuery('[name="estuf"]').change(function(){
		if ( jQuery('[name="esfera"]').val() != 'E' ) {
			var estuf = jQuery(this).val();
			carregarMunicipio( estuf, muncod_hid );
		}
	});

});

jQuery(".btn-export").click(function () {
	//$btn = jQuery(this).button('loading');
    jQuery('[name="requisicao"]').val('exportar');
    jQuery('[name="formulario-relatorio"]').submit();
});

jQuery(".btn-pesquisar").click(function () {
    $btn = jQuery(this).button('loading');
    jQuery('[name="requisicao"]').val('pesquisar');
    jQuery('[name="formulario-relatorio"]').submit();
});

jQuery("#btn-limpar").click(function () {
    window.location.href = 'par3.php?modulo=relatorio/relatorioParlamentar&acao=A';
});

function carregarMunicipio( estuf, muncod_hid ){
	if ( estuf != '' ) {
   		var options = jQuery('#muncod');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaMunicipios&estuf="+estuf,
	   		async: false,
	   		success: function(result) {
                options.empty();
                options.append(new Option('', ''));
                var result = JSON.parse(result);
                $.each(result, function() {
                    options.append(new Option(this.mundescricao, this.muncod));
                });
                options.focus();
                if (muncod_hid) {
                	options.val(muncod_hid);
                }
                options.trigger('chosen:updated');
	   		}
	 	});
	}
}

</script>