<?php global $simec; ?>

<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Itens Cardápio Descrição</h3>
    </div>
    
    <div class="ibox-content">
    
    <?php criaAbaTabelaApoio(); ?>
			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio-itens-descricao" id="formulario-cardapio-itens-descricao">
            	<input type="hidden" name="pesquisar" value="1"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$ciddsc = simec_htmlentities ( $_POST ['ciddsc'] );
            
            				$arrAttr = array (
            				    'placeHolder' => 'Descrição',
            					'maxlength' => '300'
            				);
            				echo $simec->input ( 'ciddsc', 'Descrição Itens Cardápio', $ciddsc, $arrAttr, array('label-size' => '4') );
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="reset" class="btn btn-primary btn-pesquisar" style="width: 200px;"><i class="fa fa-search"></i> Pesquisar</button>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Itens Cardápio Descrição');
        $esconderColunas = array('cidid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_cardapio_itens_descricao);
        $listagemSimec->addAcao('view', array('func' => 'abrirCardapioItensDescricao', 'extra-params' => array('cidid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarCardapioItensDescricao', 'extra-params' => array('cidid')));
        $listagemSimec->setFormFiltros('formulario-cardapio-itens-descricao');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-cardapio-itens-descricao"]').submit();
});

function abrirCardapioItensDescricao(cidid){
	var url = 'demandas.php?modulo=demandasCardapioItensDescricao/cadastrar&acao=A&cidid='+cidid;
    $(location).attr('href', url);
}

function deletarCardapioItensDescricao(cidid){

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
		  window.location='demandas.php?modulo=demandasCardapioItensDescricao/listar&acao=A&requisicao=deletarCardapioItemDescricao&cidid='+cidid;
		});

}


</script>