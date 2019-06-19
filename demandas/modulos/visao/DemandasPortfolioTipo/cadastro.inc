<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Portfólio Tipo</h3>
    </div>

    
    <div class="ibox-content">
    
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-portfolio-tipo" id="formulario-portfolio-tipo">
            	<input type="hidden" name="requisicao" value="salvarPortfolioTipo"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$dctdsc = simec_htmlentities ( $_POST ['dctdsc'] );
            				echo $simec->input ( 'dctdsc', 'Tipo Portfólio', $this->dctdsc );
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <center>
            <button type="button" class="btn btn-primary">Salvar</button>
        </center>
    </div>
</div>


<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-portfolio-tipo"]').submit();
});

</script>