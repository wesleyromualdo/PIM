<?php
set_time_limit(10800);
/**
 * relatorioBandaLarga.inc
 *
 * @category Cadastro
 * @package  A1
 * @date 14 de abr de 2016
 * @author   Fellipe Esteves<fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */
$relatorioBandaLarga = new Par3_Model_Relatorio_BandaLarga();
$estado = new Territorios_Model_Estado();

switch ($_REQUEST['requisicao']) {
	case 'exportar':
		header("Content-Type: application/vnd.ms-excel; charset=utf-8");
		header("Content-Disposition: attachment; filename=QuestionarioBandaLarga.xls");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Cache-Control: private",false);
		$relatorioBandaLarga->gerarPlanilha($_REQUEST);
		die;
		break;
	case 'carregaMunicipios':
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'"))); die;
		break;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
	<input type="hidden" name="muncod_hid" value="<?php echo  $_REQUEST['muncod'];?>">
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-8" >
        	       <h3 id="entidade">Relatório Questionário Banda Larga</h3>
                </div>
            	<div class="col-md-4" >
            	   <b>Legenda:</b><br>
        	       NP: Não Preencheu
        	       PP: Preenchimento Parcial
        	       PT: Preenchimento Total
                </div>
    		</div>
    	</div>
    	<div class="ibox-content">
            <?php echo $simec->select('estuf','UF', $_POST['estuf'], $estado->pegarSQLSelect( $_REQUEST ), array('required'=>'required', 'class'=>' required')); ?>
            <?php echo $simec->select('muncod', 'Município', $_POST['muncod'], array(), array('data-placeholder' => 'Município')); ?>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-9 col-lg-offset-3">
					<button type="button" class="btn btn-primary export" ><i class="fa fa-list"></i> Exportar XLS</button>
				</div>
			</div>
    	</div>
    </div>
</form>

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

	jQuery('.export').click(function() {
		$('#formulario').isValid(geraRelatorio);
	});
});

function geraRelatorio(isValid){
	if(isValid){
    	window.open(window.location.href + "&requisicao=exportar&" + jQuery('#formulario').serialize());
	}
}

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