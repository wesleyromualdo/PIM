<?php global $simec; ?>

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
            	<input type="hidden" name="pesquisar" value="1"> 
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	
                        	$dctdsc = simec_htmlentities ( $_POST ['dctdsc'] );
            
            				$arrAttr = array (
            					'maxlength' => '300'
            				);
            				echo $simec->input ( 'dctdsc', 'Tipo Cardápio', $dctdsc, $arrAttr, array('label-size' => '4') );
            				?>
                        </div>
            		</div>
            </form>
    
            </div>
            </div>
		
	</div>
	
	<div class="ibox-footer">
        <div class="center">
            <button type="reset" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Cardápio Tipo');
        $esconderColunas = array('dctid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_cardapio_tipo);
        $listagemSimec->addAcao('view', array('func' => 'abrirCardapioTipo', 'extra-params' => array('dctid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarCardapioTipo', 'extra-params' => array('dctid')));
        $listagemSimec->setFormFiltros('formulario-cardapio-tipo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-cardapio-tipo"]').submit();
});

function abrirCardapioTipo(dctid){
	var url = 'demandas.php?modulo=demandasCardapioTipo/cadastrar&acao=A&dctid='+dctid;
    $(location).attr('href', url);
}

function deletarCardapioTipo(dctid){

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
		  window.location='demandas.php?modulo=demandasCardapioTipo/listar&acao=A&requisicao=deletarCardapioTipo&dctid='+dctid;
		});

}


</script>