<?php
$renderEntidade = new Demanda_Controller_Entidade();

?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="ddtid"   id="ddtid"  value="<?php echo $_REQUEST['ddtid']?>"/>
    <input type="hidden" name="req"                 value="salvar"/>
    <input type="hidden" name="menu"                value="<?php echo $_REQUEST['menu']; ?>"/>

    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-12" >
        	       <h3 id="entidade">Dados do(a) Prefeito(a)</h3>
                </div>
    		</div>
    	</div>
    	<div class="ibox-content" id="div_prefeito"><?php $renderEntidade->formPessoaFisica($disabled, $objPessoaFisica, null, $display);?></div>
    	<div class="ibox-title">
        	<h3>Endereço do(a) Prefeito(a)</h3>
    	</div>
    	<div class="ibox-content"><?php $renderEntidade->formEnderecoEntidade($disabled, $objEndereco,$objmunicipio);?></div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-4 text-center"><button type="submit" class="btn btn-success salvar" ><i class="fa fa-save"></i> Salvar prefeito</button></div>
			</div>
    	</div>
    </div>
</form>
<div class="ibox">
	<div class="ibox-title">
	    <h3>Histórico Modificações</h3>
	</div>
    <?php //$controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
</div>
<script>

</script>
