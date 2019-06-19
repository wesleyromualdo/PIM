<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Item Portfólio Descrição</h3>
    </div>

    
    <div class="ibox-content">
    
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-portfolio-descricao" id="formulario-portfolio-descricao">
            	<input type="hidden" name="requisicao" value="salvarPortfolioItemDescricao"> 
                        	<?php
                        	$ciddsc = simec_htmlentities ( $_POST ['ciddsc'] );
            				echo $simec->input ( 'ciddsc', 'Descrição Item Portfólio', $this->ciddsc);
            				?>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="submit" class="btn btn-primary">Salvar</button>
        </div>
    </div>
</div>