<?php global $simec; ?>

<div class="ibox">
    <div class="ibox-title">
                <h3 class="center">Cardápio</h3>
    </div>
    
    <div class="ibox-content">
    
    <?php criaAbaTabelaApoio(); ?>
			<br/>
        	<div class="row">
    		<div class="col-lg-12">
    
    
            <form method="post" name="formulario-cardapio-itens-descricao" id="formulario-cardapio-itens-descricao">
            	<input type="hidden" name="pesquisar" value="1"> 
                    <div class="row">
                        <div class="col-lg-12">
                        	<?php
                        	
                        	$sql = "SELECT dcdid AS codigo, dcddsc AS descricao FROM demandas.demandas_cardapio_descricao WHERE dcdstatus='A';";
                        	echo $simec->select('dcdid', 'Descrição', $_POST['dcdid'], $sql, array(''));
                        	
                        	$sql = "SELECT dctid AS codigo, dctdsc AS descricao FROM demandas.demandas_cardapio_tipo WHERE dctstatus='A';";
                        	echo $simec->select('dctid', 'Tipo Cardápio', $_POST['dctid'], $sql, array(''), array('input-size' => '8'));
                        	
                        	$sql = "SELECT dddid AS codigo, enjdsc AS descricao FROM demandas.demandados d
                                    INNER JOIN corporativo.entidades_juridica j ON j.enjid = d.enjid;";
                        	echo $simec->select('dddid', 'Ofertante', $_POST['dddid'], $sql, array(''), array('input-size' => '8'));
                        	
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
        $arrayCabecalho = array('Cardápio', 'Tipo Cardápio', 'Ofertante');
        $esconderColunas = array('dcoid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_cardapio);
        $listagemSimec->addAcao('view', array('func' => 'abrirCardapio', 'extra-params' => array('dcoid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarCardapio', 'extra-params' => array('dcoid')));
        $listagemSimec->setFormFiltros('formulario-cardapio');
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

function abrirCardapio(dcoid){
	var url = 'demandas.php?modulo=demandasCardapio/cadastrar&acao=A&dcoid='+dcoid;
    $(location).attr('href', url);
}

function deletarCardapio(dcoid){

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