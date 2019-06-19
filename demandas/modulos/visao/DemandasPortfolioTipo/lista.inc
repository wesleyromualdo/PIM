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
            	<input type="hidden" name="pesquisar" value="1"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$dctdsc = simec_htmlentities ( $_POST ['dctdsc'] );
            				echo $simec->input ( 'dctdsc', 'Tipo Portfólio', $dctdsc );
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="button" class="btn btn-primary"><i class="fa fa-search"></i> Pesquisar</button> 
            <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioTipo/cadastrar';" class="btn btn-success">Incluir novo</button>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Portfólio Tipo');
        $esconderColunas = array('dctid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_portfolio_tipo);
        $listagemSimec->addAcao('view', array('func' => 'abrirPortfolioTipo', 'extra-params' => array('dctid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolioTipo', 'extra-params' => array('dctid')));
        $listagemSimec->setFormFiltros('formulario-portfolio-tipo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-portfolio-tipo"]').submit();
});

function abrirPortfolioTipo(dctid){
	var url = 'demandas.php?modulo=demandasPortfolioTipo/cadastrar&dctid='+dctid;
    $(location).attr('href', url);
}

function deletarPortfolioTipo(dctid){

	swal({
		  title: "Deletar?",
		  text: "Deseja realmente remover",
		  type: "warning",
		  showCancelButton: true,
		  confirmButtonClass: "btn-success",
		  confirmButtonText: "Sim",
		  closeOnConfirm: false
		},
		function(){
		  window.location='demandas.php?modulo=demandasPortfolioTipo/listar&requisicao=deletarPortfolioTipo&dctid='+dctid;
		});

}


</script>