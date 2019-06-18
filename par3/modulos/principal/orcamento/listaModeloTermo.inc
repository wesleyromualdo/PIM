<?php

include_once APPRAIZ .'par3/classes/controller/ModeloDocumento.class.inc';
include_once APPRAIZ .'par3/classes/model/ModeloDocumento.class.inc';

$obDocumento = new Par3_Controller_Modelo_Documento();
$obTermo = new Par3_Controller_DocumentoTermo();

switch ($_REQUEST['requisicao']) {
	case 'excluir':
		$obDocumento->excluirModeloDocumento( $_REQUEST['mdoid'] );
		exit();
	break;
	case 'visualiza_modelo':
		ob_clean();
		$arrDados = $obDocumento->carregaModeloDocumento( $_REQUEST['mdoid'] );
		echo $arrDados['mdoconteudo'];
		exit();
	break;
	case 'imprimir':
		ob_clean();
		$obTermo->imprimirModeloTermoPDF( $_REQUEST['dotid'] );
	exit();
	break;
}

require APPRAIZ . 'includes/cabecalho.inc';

echo $simec->title('Lista Modelo de Termos', '' );

$db->cria_aba( $abacod_tela, $url, '' );

$sqlTipo = "SELECT 
				td.tpdcod as codigo,
				td.tpddsc as descricao
			FROM public.tipodocumento td
			WHERE td.tpdstatus = 'A'
				AND tpdcod IN (21, 102)
			ORDER BY descricao";
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista Modelo de Termos</h2>
            </div>
        </div>
    </div>
	<div class="ibox-content">
		<div class="row">
    		<form name="formDocumento" id="formDocumento" class="form form-horizontal" method="post">
    			<input type="hidden" name="requisicao" id="requisicao" value="">
    			<?php echo $simec->input('mdonome', 'Modelo', $_REQUEST['mdonome'], array(), array('maxlength' => '255', 'input-size' => '7')); ?>
    			<?php echo $simec->select('tpdcod', 'Tipo de Documento', $_REQUEST['tpdcod'], $sqlTipo, array('maxlength' => '255'), array('input-size' => '7')); ?>
    			<?php
				$arrOptions = array('O' => 'Obras', 'P' => 'PAR'); 
				?>
				<? echo $simec->radio('mdoesfera', 'Esfera', $_REQUEST['mdoesfera'], array('E' => 'Estadual', 'M' => 'Municipal')); ?>
				<?php echo $simec->radio('mdotipo', 'Tipo de Processo', $_REQUEST['mdotipo'], $arrOptions, array(), array()); ?>
			</form>
		</div>
	</div>
	<div class="ibox-footer">
		<div class="center">
    		<button type="button" class="btn btn-primary" id="pesquisar-modelo"><i class="fa fa-search"></i> Pesquisar</button>
    		<button type="reset" class="btn btn-success btn-limpar" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
		</div>
	</div>
	<div class="ibox-content">
        <div id="documentoListagem">
            <?php
                $obDocumento->listar($_REQUEST);
            ?>
        </div>
    </div>
</div>

<script>
jQuery(".btn-limpar").click(function () {
	window.location.href = 'par3.php?modulo=principal/orcamento/listaModeloTermo&acao=A';
});

$("#pesquisar-modelo").click(function () {
    jQuery('[name="requisicao"]').val('pesquisar');
    jQuery('[name="formDocumento"]').submit();
});

function editarTermoGerado( mdoid ){
	window.location.href = "par3.php?modulo=principal/orcamento/gerarModeloTermo&acao=A&mdoid="+mdoid;
}

function deletarTermoGerado( mdoid ){
	if ( confirm( "Deseja realmente excluir este Modelo de Documento?" ) ){
		window.location.href = "par3.php?modulo=principal/orcamento/listaModeloTermo&acao=A&requisicao=excluir&mdoid=" + mdoid;
	}
}

jQuery('#btn-fechar-modelo').click(function(){
	$("#modal-visualiza-modelo").modal("hide");
});

function visualizarTermoGerado( mdoid ){
	var caminho = window.location.href;    
    var action  = '&requisicao=visualiza_modelo&mdoid='+mdoid;
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            jQuery("#div_conteudo_termo").html(resp);
            jQuery('[name="dotid"]').val(mdoid);
        }
    });
    $("#modal-visualiza-modelo").modal("show");
}

function imprimirTermo(){
	var dotid = jQuery('[name="dotid"]').val();
	window.location.href = window.location+'&requisicao=imprimir&dotid='+dotid;
}

</script>
<div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-modelo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row">
        			<div class="ibox-content" >
        			<?php echo $obTermo->montaHtmlTermo(); ?>
					</div>
				</div>
			</div>
			<div class="ibox-footer notprint" style="text-align: center;">
				<button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
				<button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
			</div>
		</div>
	</div>
</div>