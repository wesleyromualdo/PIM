<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Portfólio</h3>
    </div>

    
    <div class="ibox-content">
    
	<?php criaAbaTabelaApoio(); ?>    

			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio" id="formulario-cardapio">
            <?php if($this->dcoid) : ?>
            <input type="hidden" name="requisicao" value="adicionarPortfolioItem">
            <?php else : ?>
            <input type="hidden" name="requisicao" value="salvarPortfolio">
            <?php endif; ?>
            	 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	if($this->dcoid) $_att = array('disabled'); 
                        	else $_att = array();
                        	
                        	echo $simec->select('dcdid', 'Descrição', $this->dcdid, $this->sql_combo_portfolio_descricao, $_att);
                        	echo $simec->select('dctid', 'Tipo Cardápio', $this->dctid, $this->sql_combo_portfolio_tipo, $_att);
                        	echo $simec->select('dddid', 'Ofertante', $this->dddid, $this->sql_combo_demandados, $_att);
                        	
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
                                		
                                        $sql = "SELECT cidid AS codigo, ciddsc AS descricao 
                                                FROM demandas.demandas_portfolio_itens_descricao WHERE cidstatus=1 ".(($this->dcoid)?"AND cidid NOT IN(SELECT cidid FROM demandas.demandas_portfolio_itens WHERE dcoid='".$this->dcoid."' AND dcistatus=1)":"");
                                        echo $simec->select('cidid', 'Item descrição', '', $sql, array(), array());
                                        
                                        echo $simec->input ( 'dcivalor', 'Valor', $this->dctdsc, array ('maxlength' => '300'), array() );
                                        
                                        $sql = "( SELECT 'TRUE' AS codigo, 'SIM' AS descricao )
                                                        UNION ALL
                                                        ( SELECT 'FALSE' AS codigo, 'NÃO' AS descricao )";
                                        
                                        echo $simec->select('dcivaloreditavel', 'Item editavel para demandante', '', $sql, array(''), array());
                                        
                                        ?>
                                        <div class="ibox-footer" style="float:right;"><button type="submit" class="btn btn-primary"><i class="fa fa-plus"></i> Adicionar</button></div>
                                                                                
                                        
                                        <table class="table">
                                        	<thead>
                                        		<th></th>
                                        		<th>Item</th>
                                        		<th>Valor</th>
                                        		<th>Item editavel para demandante</th>
                                        	</thead>
                                        
                                            <tbody>
                                            
                                            <? if($this->portfolioItens) : ?>
                                            <? foreach($this->portfolioItens as $it) : ?>
                                            <tr>
                                            	<td><span class="btn btn-danger btn-sm glyphicon glyphicon-trash" onclick="deletarCardapioItem(<?=$it['dciid'] ?>);"></span></td>
                                                <td><?=$it['ciddsc'] ?></td>
                                                <td><?=$it['dcivalor'] ?></td>
                                                <td><?=(($it['dcivaloreditavel']=='t')?'SIM':'NÃO') ?></td>
                                            </tr>
                                            <? endforeach; ?>
                                            
                                            <? else : ?>
                                            <tr>
                                                <td>Não existem itens cadastrados</td>
                                            </tr>
                                            
                                            <? endif; ?>
                                            
                                            </tbody>
                                        </table>
            		
					                                            
                                        </div>

                                    </div>
       				<?php endif; ?>
            		
            </form>
    
            </div>
            </div>
		
	</div>
	<?php if(!$this->dcoid) : ?>
	<div class="ibox-footer">
        <center>
            <button type="submit" class="btn btn-primary">Salvar</button>
        </center>
    </div>
    <?php endif; ?>
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


<?php if($this->dcoid) : ?>

function deletarCardapioItem(dciid) {

	swal({
		  title: "Deletar?",
		  text: "Deseja realmente remover",
		  type: "warning",
		  showCancelButton: true,
		  confirmButtonClass: "btn-success",
		  confirmButtonText: "Sim",
		  closeOnConfirm: false
		},
		function(isConfirm){
			window.location='demandas.php?modulo=demandasCardapio/cadastrar&acao=A&dcoid=<?=$this->dcoid?>&requisicao=deletarCardapioItem&dciid='+dciid;
		});
	
}

<?php endif; ?>

</script>