<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Portfólio</h3>
    </div>
    
    <div class="ibox-content">
    
    <?php criaAbaTabelaApoio(); ?>
			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio-itens-descricao" id="formulario-portfolio">
            	<input type="hidden" name="pesquisar" value="1"> 
                    <div class="row">
                        <div class="col-lg-12">
                        	<?php
                        	echo $simec->select('dcdid', 'Descrição', $_POST['dcdid'], $this->sql_combo_portfolio_descricao);
                        	echo $simec->select('dctid', 'Tipo Portfólio', $_POST['dctid'], $this->sql_combo_portfolio_tipo);
                        	echo $simec->select('dddid', 'Ofertante', $_POST['dddid'], $this->sql_combo_demandados);
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
            <button type="reset" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button> 
            <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolio/cadastrar';" class="btn btn-success">Incluir novo</button>
    </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Portfólio', 'Tipo Portfólio', 'Ofertante');
        $esconderColunas = array('dcoid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_portfolio);
        $listagemSimec->addAcao('view', array('func' => 'abrirPortfolio', 'extra-params' => array('dcoid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolio', 'extra-params' => array('dcoid')));
        $listagemSimec->setFormFiltros('formulario-portfolio');
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

function abrirPortfolio(dcoid){
	var url = 'demandas.php?modulo=demandasPortfolio/cadastrar&dcoid='+dcoid;
    $(location).attr('href', url);
}

function deletarPortfolio(dcoid){

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
			if(isConfirm) {
				window.location='demandas.php?modulo=demandasPortfolio/listar&requisicao=deletarPortfolio&dcoid='+dcoid;
			} 
		});

}


</script>