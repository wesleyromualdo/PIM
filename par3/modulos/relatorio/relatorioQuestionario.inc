<?php
/**
 * relatorioProgramaPnaic.inc
 *
 * @category Cadastro
 * @package  A1
 * @date 14 de abr de 2016
 * @author   Wesley Romualdo<wesleysilva@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	$municipio = new Territorios_Model_Municipio();
	$sql = $municipio->pegarSQLSelect($_REQUEST['estuf']);
	echo $simec->select('mundescricao','Municipio', $_POST['muncod_hid'], $sql);
	exit();
}

require APPRAIZ . 'includes/cabecalho.inc';

$adesaoPrograma	= new Par3_Model_AdesaoPrograma();
$estado 		= new Territorios_Model_Estado();
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
	<input type="hidden" name="muncod_hid" value="<?php echo  $_REQUEST['mundescricao'];?>">
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-10" >
        	       <h3 id="entidade">RelatÃ³rio de Programa PNAIC</h3>
                </div>
    		</div>
    	</div>
    	<div class="ibox-content">
			<div class="row">
				<div class="col-lg-8 text-center">
            		<?php
            		echo $simec->select('estuf','UF', $_POST['estuf'],$estado->pegarSQLSelect( $_REQUEST ));
            		echo '<div id="td_muncod">'.$simec->input('mundescricao', 'Municipio', 'Escolha uma UF.', array('maxlength' => '50', 'disabled')).'</div>';
            		$options = array('S' => 'Sim', 'N' => 'NÃ£o');
            		echo $simec->radio('aceite', 'Aceitou?', $_POST['aceite'], $options, '', '');
            		?>
                </div>
				<div class="col-lg-4 text-center">
				</div>
			</div>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-12 text-center">
					<button type="submit" class="btn btn-success salvar" ><i class="fa fa-search"></i> Pesquisar</button>
				</div>
			</div>
    	</div>
    </div>
</form>
<div class="ibox-content" style="overflow: outo;">
<?php
$adesaoPrograma->listaTermoAdesao($_POST);
?>
</div>
<script>
jQuery(document).ready(function(){

	var muncod_hid = jQuery('[name="muncod_hid"]').val();
	
	if( jQuery('[name="estuf"]').val() != '' ){
		carregarMunicipio( jQuery('[name="estuf"]').val(), muncod_hid );
	}
	jQuery('[name="estuf"]').change(function(){
		if( jQuery('[name="esfera"]').val() != 'E' ){
			var estuf = jQuery(this).val();

			carregarMunicipio( estuf, muncod_hid );
		}
	});
});

function carregarMunicipio( estuf, muncod_hid ){
	if( estuf != '' ){
		jQuery('#aguardando').show();
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaMunicipios&estuf="+estuf+"&muncod_hid="+muncod_hid,
	   		async: false,
	   		success: function(resp){
	   			jQuery('#td_muncod').html(resp);
	   			jQuery('#aguardando').hide();
	   		}
	 	});
	}else{
		jQuery('#td_muncod').html('Escolha uma UF.');
	}
}
</script>