<?php
global $arDado;

$renderEntidade = new Demanda_Controller_Entidade();
?>
<form method="post" name="formulario-prefeitura" id="formulario-prefeitura" class="form form-horizontal">
    <input type="hidden" name="ddtid"   id="ddtid"  value="<?php echo $_REQUEST['ddtid']?>"/>
    <input type="hidden" name="requisicao" value=""/>
    <input type="hidden" name="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
                <div class="col-md-10">
        	       <h3 id="entidade">Dados da Prefeitura</h3>
                </div>
            </div>
    	</div>
    	<div class="ibox-content"><?php $renderEntidade->formPessoaJuridica($disabled, (object)$arDado['prefeitura'], null, $display);?></div>
    	<div class="ibox-title">
        	<h3>Endereço da Prefeitura</h3>
    	</div>
    	<div class="ibox-content"><?php $renderEntidade->formEnderecoEntidade($disabled, (object)$arDado['prefeitura'], $arDado['municipio']);?></div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-4 text-center" > <button type="submit" class="btn btn-success btnSalvar" ><i class="fa fa-save"></i> Salvar prefeitura</button> </div>
			</div>
    	</div>
    </div>
</form>
<!--
<div class="ibox">
	<div class="ibox-title">
	    <h3>Prefeitura - Histórico Modificações</h3>
	</div>
    <?php //$controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
</div>
-->
<script>
$(document).ready(function()
{
	$('.next').click(function(){
		var url = window.location.href.toString().replace('prefeitura', 'prefeito');

		if (url.indexOf('prefeito'))
			url = url + '&menu=prefeito';

		window.location.href = url;
	});

	/*$("#enjcnpj").inputmask({
		mask: ['999.999.999-99', '99.999.999/9999-99'],
	    keepStatic: true
	});*/
});

/*$('#enjcpf,#enjcnpj').change(function()
{
    var form = $(this).parent().parent().parent().parent().find('#formulario-prefeitura').val();
    var id   = $(this).attr('id');
console.log(id);
    if (!form) {
        if ($(this).val() != $('[name="'+id+'_old"]').val() && $('[name="'+id+'_old"]').val() != '') {
            $('input[name!="'+id+'"][type=text]').val('');
        }
    } else {
        if (($(this).val() != $('#'+form).find('[name="'+id+'_old"]').val()) && ($('#'+form).find('[name="'+id+'_old"]').val() != '')) {
            $('#'+form).find('input[name!="'+id+'"][type=text]').val('');
        }
    }
});*/

jQuery('.btnSalvar').click(function(){

	jQuery('[name="formulario-prefeitura"]').find('[name="requisicao"]').val('salva_prefeitura');
	jQuery('[name="formulario-prefeitura"]').submit();
});
</script>