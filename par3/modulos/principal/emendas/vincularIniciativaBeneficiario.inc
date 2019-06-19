<?php
$obEmenda = new Par3_Controller_EmendasParlamentares();

switch ( $_REQUEST['requisicao'] ) {
    case 'salvar':
        $obEmenda->salvar( $_REQUEST );
        die;
    break;
    case 'verificaValorEmpenho':
        echo $obEmenda->verificaValorEmpenho( $_REQUEST['inpid'], $_REQUEST['edeid'] );
        die;
    break;
    case 'enviar-iniciativa':
        require_once APPRAIZ.'/includes/workflow.php';
        $_REQUEST['tramitar'] = 'S';
        $obEmenda->salvar( $_REQUEST );
        $obEmenda->enviarIniciativa( $_REQUEST );
        die;
    break;
}

$boValida = $obEmenda->validaResponsabilidadeEmenda($_REQUEST['emeid']);

require APPRAIZ . 'includes/cabecalho.inc';
$db->cria_aba( $abacod_tela, $url, '' );

$arDadosEmenda = $obEmenda->pegaDadosEmendas($_REQUEST['emeid']);
$arrPerfilRetorno = $obEmenda->verificaPerfilEmendas($_REQUEST['emeid'], $_REQUEST['edeid']);
?>
<style>
.panel-title {
    font-size: 15px;
}
.panel-little {
    font-size: 12px;
}

.table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td{
    vertical-align: middle;
}
.style_h5 {
  font-size: 14px;
  margin: 0 0 7px;
  padding: 0;
  text-align: left;
}
</style>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Vincular Iniciativas ao Beneficiário</h2>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12">
            	<table>
                	<tbody>
                		<tr>
                			<td class="style_h5" style="text-align: right !important;"><b>Parlamentar:</b></td>
                			<td class="style_h5">&nbsp;<?php echo $arDadosEmenda['autor']; ?></td>
                		</tr>
                		<tr>
                			<td class="style_h5" style="text-align: right !important;"><b>Ação:</b></td>
                			<td class="style_h5">&nbsp;<?php echo $arDadosEmenda['acao']; ?></td>
                		</tr>
                	</tbody>
                </table>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
        	<div class="col-lg-12">
                	<div class="row">
                		<div class="col-lg-6">
                            <div class="panel panel-default" style="text-align: left;">
                            	<div class="panel-heading"> <h3 class="panel-title">Detalhes da Emenda</h3></div>
                            	<div class="panel-body"><?php echo $obEmenda->carregaDetalheEmenda($_REQUEST['emeid']); ?></div>
                            </div>
                        </div>
                        <div class="col-lg-6">
                            <div class="panel panel-info">
                            	<div class="panel-heading"> <h3 class="panel-title">Beneficiários da Emenda</h3> </div>
                            	<div class="panel-body" style="overflow: auto; height: 270px !important; "> <?php echo $obEmenda->carregaBeneficiarioEmenda($_REQUEST['emeid']); ?> </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="row">
                	<div class="col-lg-12">
                        <div class="panel panel-success" style="text-align: left;">
                        	<div class="panel-heading"> <h3 class="panel-title">Beneficiário Selecionado</h3> </div>
                        	<div class="panel-body"> <?php echo $obEmenda->pegaBeneficiarioSelecionado($_REQUEST['edeid']); ?> </div>
                        </div>
                    </div>
                </div>
            <form name="form-lista-emenda" id="form-lista-emenda" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao" id="requisicao" value=""/>
            	<input type="hidden" name="edeid" id="edeid" value="<?php echo $_REQUEST['edeid']?>"/>
            	<input type="hidden" name="emeid" id="emeid" value="<?php echo $_REQUEST['emeid']?>"/>
                <div class="row">
                	<div class="col-lg-12">
                        <div class="panel panel-warning" style="text-align: left;">
                        	<div class="panel-heading"> <h3 class="panel-title">Vincular Iniciativa</h3> <span class="panel-little">*Somente as iniciativas planejadas pelo ente serão apresentadas</span></div>
                        	<div class="panel-body"> <?php echo $obEmenda->carregaIniciativaBeneficiario($_REQUEST['edeid'], $_REQUEST['emeid'])?> </div>
                        </div>
                    </div>
                </div>
            	<div class="row">
            		<div class="col-lg-12">
                    	<div class="ibox-footer">
                        	<div class="center">
<?php                       if( $arrPerfilRetorno['vincular'] == 'S' ){?>
                        		<button type="button" id="btn-salvar" data-dismiss="modal" class="btn btn-primary btn-salvar" data-loading-text="Salvando, aguarde ..."><i class="fa fa-search"></i> Salvar</button>
<?php                           if( $arrPerfilRetorno['tramitar'] == 'S' ){?>
                        			<button type="button" id="btn-enviar" data-dismiss="modal" class="btn btn-primary btn-enviar" data-loading-text="Enviando, aguarde ..."><i class="fa fa-search"></i> Enviar para o Município</button>
<?php                           }?>
<?php                       } ?>
                            	<button type="reset" class="btn btn-success" id="btn-voltar"><i class="fa fa-eraser"></i> Voltar</button>
                        	</div>
                    	</div>
                	</div>
            	</div>
            </form>
        </div>
    </div>
    <div class="ibox-content">
        <div id="documentoListagem">
        </div>
    </div>
</div>

<script>
jQuery(document).ready(function(){
	
	/*jQuery('[name="inpid[]"]').each(function( index ) {

		if( jQuery( this ).is(':checked') == true ){
			var inpid = jQuery( this ).val();
			var valordestinado = jQuery('#valordestinado_'+inpid).val();

			jQuery('#valordestinado_'+inpid).val( number_format(valordestinado, 2, ",", ".") );
		}
	});*/

	pegaValorTotalInformado();
	
});

jQuery('#btn-voltar').click(function(){
	window.location.href = "par3.php?modulo=principal/emendas/listaEmendas&acao=A";
});

jQuery('#btn-enviar').click(function(){

	swal({
        title: "Confirmar",
        text: "Deseja enviar as <b>Iniciativas Vinculadas às Emendas com a Situação \"Em Cadastramento\" </b> para a análise do município?<br><p style='color: red;'>Obs.: As informações não salvas serão perdidas.</p>",
        type: "success",
        html: true,
        showCancelButton: true,
        cancelButtonText: "Não",
        confirmButtonText: "Sim",
        closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                swal.close();
                enviarAnaliseMunicipio();
            }
            return false;
        }
    );
	
});

function enviarAnaliseMunicipio(){
	var valor_designado = jQuery('[name="valor_designado"]').val();
	/*var valor = pegaValorTotalInformado();

	if( parseFloat(valor) != parseFloat(valor_designado) ){
		swal("Ação não Permitida!", "O Valor Total Destinado(R$ "+number_format(valor, 2, ",", ".")+") deve ser igual ao Valor Designado ao município(R$ "+number_format(valor_designado, 2, ",", ".")+")!", "error");
		return false;
	} else {*/
    	jQuery('[name="requisicao"]').val('enviar-iniciativa');
    	jQuery('[name="form-lista-emenda"]').submit();
	//}
}

jQuery('#btn-salvar').click(function(){
	var valor_designado = jQuery('[name="valor_designado"]').val();

	var boTrue = false;
	jQuery('[name="inpid[]"]:checked').each(function( index ) {
		if( jQuery( this ).is(':disabled') == false ){
			boTrue = true;
		}
	});

	if( boTrue == false ){
		swal("Ação não Permitida!", "As iniciativas somente poderão ser vinculadas a emenda na situação em cadastramento!", "error");
		return false;
	}
	
	if( jQuery('[name="inpid[]"]:checked').length == 0 ){
		swal("Ação não Permitida!", "Selecione pelo menos uma iniciativa!", "error");
		return false;
	}

	/*var valor = pegaValorTotalInformado();

	if( parseFloat(valor) != parseFloat(valor_designado) ){
		swal("Ação não Permitida!", "O Valor Total Destinado(R$ "+number_format(valor, 2, ",", ".")+") deve ser igual ao Valor Designado ao município(R$ "+number_format(valor_designado, 2, ",", ".")+")!", "error");
		return false;
	} else {*/
    	jQuery('[name="requisicao"]').val('salvar');
    	jQuery('[name="form-lista-emenda"]').submit();
	//}
	
});

jQuery('[name="inpid[]"]').click(function(){
	var codigo = this.value;
	
	if( jQuery('[name="tem_analise['+codigo+']"]').val() > 0 ){
		swal("Ação não Permitida!", "Não é possível desvincular esse planejamento, pois já existe uma análise aprovada vinculada!", "error");
		jQuery('#chkiniciativa_'+codigo).prop('checked', true);
	} else {
    	if( jQuery('#chkiniciativa_'+codigo+':checked').length > 0 ){
    		jQuery('#valordestinado_'+codigo).attr('disabled', false);
    		jQuery('#valordestinado_'+codigo).val('0,00');
    	} else {
    		jQuery('#valordestinado_'+codigo).attr('disabled', true);
    		jQuery('#valordestinado_'+codigo).val('');
    		jQuery('#valordestinado_'+codigo).focus();
    	}
	}
});

jQuery('.valordestinado').on('focus', function(){
	if( parseInt( jQuery(this).val() ) == '0' ){
		jQuery(this).val('');
	}
});

jQuery('.valordestinado').on('blur', function(){
	if( jQuery(this).val() == '' ){
		jQuery(this).val('0,00');
	}
});

function pegaValorTotalInformado(){
	var valor = 0;
	jQuery('.valordestinado').each(function( index ) {
		//if( jQuery( this ).is(':disabled') == false ){
		  valor = parseFloat(valor) + parseFloat(  retiraPontosPAR3Emenda(jQuery( this ).val()) );
		//}
	});
	//console.log(valor);
	valor = number_format(valor, 2, ",", ".");
	jQuery('#total_destinado').html(valor);
	valor = retiraPontosPAR3Emenda(valor);

	return valor;
}

function calcularValorDestinado( inpid ){
	var valor_designado = jQuery('[name="valor_designado"]').val();
	var valor_planejado = jQuery('#valor_planejado_'+inpid).val();
	//var valordestinado = retiraPontosPAR3Emenda( jQuery('#valordestinado_'+inpid).val() );
	
	/*if( parseFloat(valordestinado) > parseFloat(valor_planejado) ){
		swal("Ação não Permitida!", "O Valor Destinado informado de R$ "+number_format(valordestinado, 2, ",", ".")+" não pode ser maior que o Valor Planejado: R$ "+number_format(valor_planejado, 2, ",", ".")+"!", "error");
		jQuery('#valordestinado_'+inpid).val('0,00');
	}*/
	
	var valor = pegaValorTotalInformado();
	if( parseFloat(valor) > parseFloat(valor_designado) ){
		swal("Ação não Permitida!", "O(s) valor(es) informado(s) de R$ "+number_format(valor, 2, ",", ".")+" não pode(m) ser maior que o Valor Designado: R$ "+number_format(valor_designado, 2, ",", ".")+"!", "error");
		jQuery('#valordestinado_'+inpid).val('0,00');
	}

	verificaValorEmpenho(inpid);
}

function verificaValorEmpenho(inpid){
	var edeid 			= jQuery('[name="edeid"]').val();
	var valordestinado 	= retiraPontosPAR3Emenda( jQuery('#valordestinado_'+inpid).val() );
	
	var caminho = window.location.href;
    var action  = '&requisicao=verificaValorEmpenho&inpid='+inpid+'&edeid='+edeid;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	//jQuery("#debug").html(resp+'-'+valordestinado);
        	if( parseFloat(valordestinado) <  parseFloat(resp) ){
        		swal("Ação não Permitida!", "Já existe empenho para este planejamento. O valor informado de R$ "+number_format(valordestinado, 2, ",", ".")+" não pode ser menor que o Valor Empenhado: R$ "+number_format(resp, 2, ",", ".")+"!", "error");
        		jQuery('#valordestinado_'+inpid).val(number_format(resp, 2, ",", "."));
            }
        }
    });
}

function retiraPontosPAR3Emenda(v){
	if( v != 0 ){
		var valor = v.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");
	} else {
		var valor = v;
	}
	
	return valor;
}

function carregarBeneficiarioEmenda(edeid, emeid){
	window.location.href = "par3.php?modulo=principal/emendas/vincularIniciativaBeneficiario&acao=A&emeid="+emeid+"&edeid="+edeid;
}

function selecionaTodos() {

	var selecionaTodos 	= document.getElementById("seleciona_todos");
	
	jQuery('[name="inpid[]"]').each(function(){
		var codigo = this.value;

		if( jQuery('#chkiniciativa_'+codigo).prop('disabled') == false ){
    		if(selecionaTodos.checked) {
    			jQuery('#chkiniciativa_'+codigo).prop('checked', true);
    			jQuery('#valordestinado_'+codigo).attr('disabled', false);
    			if( jQuery('[name="tem_analise['+codigo+']"]').val() == 0 ){
					jQuery('#valordestinado_'+codigo).val('0,00');
    			}
    		} else {
    			if( jQuery('[name="tem_analise['+codigo+']"]').val() > 0 ){
    				jQuery('#chkiniciativa_'+codigo).prop('checked', true);
    				jQuery('#valordestinado_'+codigo).attr('disabled', false);
    			} else {
    				jQuery('#chkiniciativa_'+codigo).prop('checked', false);
    				jQuery('#valordestinado_'+codigo).attr('disabled', true);
        			jQuery('#valordestinado_'+codigo).val('');
    			}
    		}
		}
	});
}
</script>