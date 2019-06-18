<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="left">Cadastro de funções</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
		<?php echo $simec->tab($this->arAba, $this->url_pagina); ?>    
		<br/>
    	<div class="row">
    		<div class="col-lg-12">
            	<form method="post" name="formulario-cadastro" id="formulario-cadastro">
        			<input type="hidden" name="requisicao" id="requisicao" value=""> 
                	<?php
                	echo $simec->radio('enftipo', 'Tipo de entidade', $this->edicao['enftipo'], $this->arTipoEntidade, [], ['input-size'=>'8', 'label-size'=>'2']);
                	$arrAttr = array (
                	    'placeHolder' => 'Digite a descrição da função/entidade',
                	    'maxlength' => '200'
                	);
                	echo $simec->input ('enfdsc', 'Descrição função/entidade', $this->edicao['enfdsc'], $arrAttr, ['label-size'=>'2', 'input-size'=>'5']);
                	?>
        		</form>
    		</div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12" style="text-align: center;">
    			<button type="reset" class="btn btn-success js-btn-voltar">
    				<i class="fa fa-arrow-left"></i> Voltar
    			</button>
    			<button type="button" class="btn btn-primary js-btn-salvar">
    				<i class="fa fa-save"></i> Salvar
    			</button>
            </div>
    	</div>
	</div>
</div>


<script type="text/javascript">
var $validacao = new ValidacaoAutomatica();
$validacao.addCampo('name', 'enftipo', 'Tipo de entidade')
		  .addValidaObrigatorio();
$validacao.addCampo('name', 'enfdsc', 'Descrição função/entidade')
		  .addValidaObrigatorio();
		  
jQuery('.js-btn-salvar').click(function ()
{
	jQuery('.js-btn-salvar').prop('disabled', true);
	$validacao.enviarFormulario('formulario-cadastro', 'requisicao', 'salvar-funcao');
	jQuery('.js-btn-salvar').prop('disabled', false);
});

jQuery('.js-btn-voltar').click(function ()
{
	location.href = '?modulo=representante/listar&acao=A';
});
</script>