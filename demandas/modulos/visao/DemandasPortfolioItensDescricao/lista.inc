<?php global $simec; ?>

<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Itens Portfólio Descrição</h3>
    </div>
    
    <div class="ibox-content">
    
    <?php criaAbaTabelaApoio(); ?>
			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-portfolio-itens-descricao" id="formulario-portfolio-itens-descricao">
            	<input type="hidden" name="pesquisar" value="1"> 
                        	<?php
                        	
                        	$ciddsc = simec_htmlentities ( $_POST ['ciddsc'] );
            				echo $simec->input ( 'ciddsc', 'Descrição Itens Portfólio', $ciddsc );
            				?>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="reset" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button> 
            <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioItensDescricao/cadastrar';" class="btn btn-success">Incluir novo</button>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Itens Portfólio Descrição');
        $esconderColunas = array('cidid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_portfolio_itens_descricao);
        $listagemSimec->addAcao('view', array('func' => 'abrirPortfolioItensDescricao', 'extra-params' => array('cidid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolioItensDescricao', 'extra-params' => array('cidid')));
        $listagemSimec->setFormFiltros('formulario-portfolio-itens-descricao');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-portfolio-itens-descricao"]').submit();
});

function abrirPortfolioItensDescricao(cidid){
	var url = 'demandas.php?modulo=demandasPortfolioItensDescricao/cadastrar&cidid='+cidid;
    $(location).attr('href', url);
}

function deletarPortfolioItensDescricao(cidid){

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
		  window.location='demandas.php?modulo=demandasPortfolioItensDescricao/listar&requisicao=deletarPortfolioItemDescricao&cidid='+cidid;
		});

}


</script>