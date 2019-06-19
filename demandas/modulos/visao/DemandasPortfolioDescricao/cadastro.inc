<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Portfólio Descrição</h3>
    </div>

    
    <div class="ibox-content">
    
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-portfolio-descricao" id="formulario-portfolio-descricao">
            	<input type="hidden" name="requisicao" value="salvarPortfolioDescricao"> 
            	<?php
            	$dcddsc = simec_htmlentities ( $_POST ['dcddsc'] );
				echo $simec->input ( 'dcddsc', 'Descrição Portfólio', $this->dcddsc );
				?>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="submit" class="btn btn-primary btn-pesquisar">Salvar</button>
        </div>
    </div>
</div>


<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-portfolio-descricao"]').submit();
});

</script>