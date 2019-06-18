<?php global $simec; ?>

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
            	<input type="hidden" name="pesquisar" value="1"> 
                        	<?php
                        	
                        	$dcddsc = simec_htmlentities ( $_POST ['dcddsc'] );
            				echo $simec->input ( 'dctdsc', 'Descrição Portfólio', $dcddsc );
            				?>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="button" class="btn btn-primary"><i class="fa fa-search"></i> Pesquisar</button> 
            <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioDescricao/cadastrar';" class="btn btn-success">Incluir novo</button>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Portfólio Descrição');
        $esconderColunas = array('dctid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_portfolio_descricao);
        $listagemSimec->addAcao('view', array('func' => 'abrirPortfolioDescricao', 'extra-params' => array('dcdid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolioDescricao', 'extra-params' => array('dcdid')));
        $listagemSimec->setFormFiltros('formulario-cardapio-descricao');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-portfolio-descricao"]').submit();
});

function abrirPortfolioDescricao(dcdid){
	var url = 'demandas.php?modulo=demandasPortfolioDescricao/cadastrar&dcdid='+dcdid;
    $(location).attr('href', url);
}

function deletarPortfolioDescricao(dcdid){

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
		  window.location='demandas.php?modulo=demandasPortfolioDescricao/listar&requisicao=deletarPortfolioDescricao&dcdid='+dcdid;
		});

}


</script>