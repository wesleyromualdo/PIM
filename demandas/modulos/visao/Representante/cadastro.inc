<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="left">Vínculo de tipo de representante</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
		<?php echo $simec->tab($this->arAba, $this->url_pagina); ?>    
		<br/>
    	<div class="row">
    		<div class="col-lg-12">
            	<form method="post" name="formulario-cadastro" id="formulario-cadastro">
        			<input type="hidden" name="requisicao" value=""> 
        			<input type="hidden" name="enrid" id="enrid" value=""> 
                	<?php
                	echo $simec->radio('enftipo', 'Tipo de entidade', $this->edicao['enftipo'], $this->arTipoEntidade, ['disabled'], ['input-size'=>'8', 'label-size'=>'2']);
                	$arrAttr = array (
                	    'disabled',   
                	    'placeHolder' => 'Digite a descrição da função/entidade',
                	    'maxlength' => '200'
                	);
                	echo $simec->input ('enfdsc', 'Descrição função/entidade', $this->edicao['enfdsc'], $arrAttr, ['label-size'=>'2', 'input-size'=>'5']);
                	?>
                	<div class="row">
                		<div class="offset-2 col-6">
                		    <button type="button" class="btn btn-success js-incluir-representante">
    							<i class="fa fa-plus"></i> Incluir tipo de representante
    						</button>
                		</div>
                	</div>
                	<div class="row">
                		<div class="col-12">
                			<fieldset>
                				<legend>
                					Lista de tipos de representantes 
                				</legend>
                				<?php echo $this->listaRepresentantes->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA); ?>
                			</fieldset>
                		</div>
                	</div>
        		</form>
    		</div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12" style="text-align: center;">
    			<button type="reset" class="btn btn-success js-btn-voltar">
    				<i class="fa fa-arrow-left"></i> Voltar
    			</button>
            </div>
    	</div>
	</div>
</div>
<?php 
echo $this->modalRepresentante;
?>




<script type="text/javascript">

jQuery('.js-incluir-representante').click(function ()
{
	// esta função no arquivo da modal
	limparDadosFormModal();
	atualizarComboRepresentante(_JS.arComboRepresentanteDado);
	jQuery("#modal-representante").modal("show");	
});

function atualizarComboRepresentante(arComboRepresentanteDado, enfidfilho_valor)
{
	enfidfilho_valor = enfidfilho_valor || '';
	
	$enfidfilho = jQuery('#enfidfilho');
	$enfidfilho.find('option').remove();
    for (var i=0; i < arComboRepresentanteDado.length; i++) {
    	$enfidfilho.append('<option value="'+ arComboRepresentanteDado[i].codigo +'">'+ arComboRepresentanteDado[i].descricao +'</option>')
	}
    $enfidfilho.val(enfidfilho_valor).trigger("chosen:updated");
}

function editarRepresentanteEntidade(enrid)
{
	// esta função no arquivo da modal
	limparDadosFormModal();
	
	jQuery.ajax({
	    type	: "POST",
	    url		: location.href,
	    data	: {enrid:enrid, requisicao:'editar-representante'},
	    async	: true,
	    dataType: 'json',
	    success	: function (resp)
	    {
	    	atualizarComboRepresentante(resp.arComboRepresentante, resp.dado.enfidfilho);
	    	jQuery('#enfidfilho').prop('disabled', true);

		    if (resp.dado.enrgestor == 't') {
				jQuery('[name=enrgestor]:eq(0)').click();
			} else {
				jQuery('[name=enrgestor]:eq(1)').click();
			}
			
		    if (resp.dado.enrtipo == 'E') {
				jQuery('[name=enrtipo]:eq(0)').click();
			} else {
				jQuery('[name=enrtipo]:eq(1)').click();
			}

		    jQuery('#enrqtdcadastrados').val(resp.dado.enrqtdcadastrados);

		    jQuery("#modal-representante").modal("show");
	    }
	});
}

function excluirRepresentanteEntidade(enrid)
{
    swal({
        title: "Confirmar",
        text: "Deseja apagar este representante?",
        type: "success",
        html: true,
        showCancelButton: true,
        cancelButtonText: "Não",
        confirmButtonText: "Sim",
        closeOnConfirm: false
    }, function (isConfirm)
    {
        if (isConfirm) {
        	jQuery('[name="requisicao"]').val('excluir-representante-entidade');
        	jQuery('[name="enrid"]').val(enrid);
        	jQuery('[name="formulario-cadastro"]').submit();	
        }
        return false;
    });
}

jQuery('.js-btn-voltar').click(function ()
{
	location.href = '?modulo=representante/listar&acao=A';
});
</script>