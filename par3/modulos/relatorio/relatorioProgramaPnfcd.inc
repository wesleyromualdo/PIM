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

$adesaoPrograma	= new Par3_Model_AdesaoPrograma();
$estado = new Territorios_Model_Estado();

switch ($_REQUEST['requisicao']) {
	case 'exportar':
		$lista = $adesaoPrograma->listaTermoAdesaoPnfcd($_REQUEST, 'data');
		header("Content-Type: application/vnd.ms-excel; charset=utf-8");
		header("Content-Disposition: attachment; filename=PNFCD.xls");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Cache-Control: private",false);
		require('relatorioProgramaPnfcdXls.inc');
		die;
		break;
	case 'carregaMunicipios':
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'"))); die;
		break;
	case 'mostrarRespostas':
		print $adesaoPrograma->carregarRespostas($_POST['dados'][0]); die;
		break;
	case 'downloadArquivo':
		if (!$_POST) {
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$prp = new Par3_Model_ProadesaoRespostasPnfcd();
			$ppp = $prp->pegarRespostasPorInuid($_GET['inuid']);
			$file = new FilesSimec("proadesao_respostas_pnfcd", array(), "par3");
			$file->getDownloadArquivo($ppp['pppresp1arqid']);
		}
		break;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
	<input type="hidden" name="muncod_hid" value="<?php echo  $_REQUEST['muncod'];?>">
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-10" >
        	       <h3 id="entidade">Relatório de Programa PNFCD</h3>
                </div>
    		</div>
    	</div>
    	<div class="ibox-content">
    		<?php $estados = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 258"; ?>
            <?php echo $simec->select('estuf','UF', $_POST['estuf'], $estado->pegarSQLSelect( $_REQUEST )); ?>
            <?php echo $simec->select('muncod', 'Municipio', $_POST['muncod'], array(), array('data-placeholder' => 'Município')); ?>
            <?php echo $simec->radio('esfera', 'Esfera', $_POST['esfera'] ? $_POST['esfera'] : 2, array('1' => 'Estadual', '2' => 'Municipal')); ?>
            <?php echo $simec->select('situacao', 'Situação', $_POST['situacao'], $estados, array('data-placeholder' => 'Situação')); ?>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-9 col-lg-offset-3">
					<button type="submit" class="btn btn-success salvar" ><i class="fa fa-search"></i> Pesquisar</button>
					<button type="button" class="btn btn-primary export" ><i class="fa fa-list"></i> Exportar XLS</button>
				</div>
			</div>
    	</div>
    </div>
</form>

<div class="ibox-content" style="overflow: outo;" class="text-left">
<?php $adesaoPrograma->listaTermoAdesaoPnfcd($_POST); ?>
</div>

<script>
jQuery(document).ready(function() {
	$('.table').removeClass('table-hover');
	
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
		window.open(window.location.href + "&requisicao=exportar&" + jQuery('#formulario').serialize());
	});
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

function downloadArquivo(inuid) {
	if ( inuid != '' ) {
		window.location.href = 'par3.php?modulo=relatorio/relatorioProgramaPnfcd&acao=A&requisicao=downloadArquivo&inuid=' + inuid;
	}
}
</script>