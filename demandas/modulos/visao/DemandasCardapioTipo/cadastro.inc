<?php
global $simec;


?>

<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Cardápio Tipo</h3>
    </div>

    
    <div class="ibox-content">
    
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio-tipo" id="formulario-cardapio-tipo">
            	<input type="hidden" name="requisicao" value="salvarCardapioTipo"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$dctdsc = simec_htmlentities ( $_POST ['dctdsc'] );
            
            				$arrAttr = array (
            				    'placeHolder' => 'Digite o Tipo de Cardápio',
            					'maxlength' => '300'
            				);
            				echo $simec->input ( 'dctdsc', 'Tipo Cardápio', $this->dctdsc, $arrAttr, array('label-size' => '4') );
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <center>
            <button type="reset" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Salvar</button>
        </center>
    </div>
</div>


<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-cardapio-tipo"]').submit();
});

function abrirCardapio(dctid){
	var url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&dctid='+dctid;
    $(location).attr('href', url);
}

</script>