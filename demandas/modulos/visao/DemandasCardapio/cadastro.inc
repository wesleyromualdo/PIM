<?php
global $simec, $arDado;


?>

<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Cardápio</h3>
    </div>

    
    <div class="ibox-content">
    
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio" id="formulario-cardapio">
            	<input type="hidden" name="requisicao" value="salvarCardapio"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$sql = "SELECT dcdid AS codigo, dcddsc AS descricao FROM demandas.demandas_cardapio_descricao WHERE dcdstatus='A';";
                        	echo $simec->select('dcdid', 'Descrição', $this->dcdid, $sql, array(''));
                        	
                        	$sql = "SELECT dctid AS codigo, dctdsc AS descricao FROM demandas.demandas_cardapio_tipo WHERE dctstatus='A';";
                        	echo $simec->select('dctid', 'Tipo Cardápio', $this->dctid, $sql, array(''), array('input-size' => '8'));
                        	
                        	$sql = "SELECT dddid AS codigo, enjdsc AS descricao FROM demandas.demandados d 
                                    INNER JOIN corporativo.entidades_juridica j ON j.enjid = d.enjid;";
                        	echo $simec->select('dddid', 'Ofertante', $this->dddid, $sql, array(''), array('input-size' => '8'));
                        	
            				?>
                        </div>
            		</div>
            		
            		<?php if($this->dcoid) : ?>
            		
            		
            				<div class="panel panel-default">
                                        <div class="panel-heading">
                                            <strong>Itens</strong>
                                        </div>
                                        
                                        <div class="panel-body">

            		<?php
            		
                    $sql = "SELECT cidid AS codigo, ciddsc AS descricao FROM demandas.demandas_cardapio_itens_descricao WHERE cidstatus='A';";
                    echo $simec->select('cidid', 'Item descrição', '', $sql, array(), array());
                    
                    echo $simec->input ( 'dctdsc', 'Valor', $this->dctdsc, array ('maxlength' => '300'), array() );
                    
                    $sql = "( SELECT 'TRUE' AS codigo, 'SIM' AS descricao )
                                    UNION ALL
                                    ( SELECT 'FALSE' AS codigo, 'NÃO' AS descricao )";
                    
                    echo $simec->select('cidid', 'Item editavel para demandante', '', $sql, array(''), array());
                    
                    ?>
                    
	<div class="ibox-footer" style="float:right;"><button type="button" class="btn btn-primary"><i class="fa fa-plus"></i> Adicionar</button></div>
                                        
                                        
                                        
                                        <?php echo $arDado['htmlItens']; ?>
					                                            
                                        </div>

                                    </div>
       				<?php endif; ?>
            		
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <center>
            <button type="button" class="btn btn-primary"><i class="fa fa-search"></i> Salvar</button>
        </center>
    </div>
</div>


<script type="text/javascript">

jQuery('#inserirItem').click(function(){

	jQuery.ajax({
	    type: "GET",
	    url: window.location.href,
	    data: 'requisicao=inserirCardapioItem&',
	    success: function(retorno){
	        options.append(new Option('', ''));
	        $.each(JSON.parse(retorno), function() {
	            options.append(new Option(this.descricao, this.codigo));
	        });
	        options.val(dcoid);
	        options.trigger('chosen:updated');
	    }
	});

	
});

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-cardapio"]').submit();
});


</script>