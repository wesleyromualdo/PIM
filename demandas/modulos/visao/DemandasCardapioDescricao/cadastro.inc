<?php
global $simec;


?>

<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Cardápio Descrição</h3>
    </div>

    
    <div class="ibox-content">
    
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio-descricao" id="formulario-cardapio-descricao">
            	<input type="hidden" name="requisicao" value="salvarCardapioDescricao"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$dcddsc = simec_htmlentities ( $_POST ['dcddsc'] );
            
            				$arrAttr = array (
            				    'placeHolder' => 'Digite a Descrição do Cardápio',
            					'maxlength' => '300'
            				);
            				echo $simec->input ( 'dcddsc', 'Descrição Cardápio', $this->dcddsc, $arrAttr, array('label-size' => '4') );
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="submit" class="btn btn-primary btn-pesquisar" style="width: 200px;"><i class="fa fa-search"></i> Salvar</button>
        </div>
    </div>
</div>


<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-cardapio-descricao"]').submit();
});

</script>