<?php
global $simec;

if( $_REQUEST['enfid'] ){
    $url = 'demandas.php?modulo=demandantes/listar&acao=A&enfid='.$_REQUEST['enfid'];
} else {
    $url = 'demandas.php?modulo=demandantes/listar&acao=A&enfid=1';
}
echo <<<HTML

    <!-- inicio --->
    {$simec->tab ( $this->abas, $url )}
    <!-- fim --->     
HTML;

?> 

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Filtros de pesquisa</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    	<form method="post" name="formulario-entidade" id="formulario-entidade">
			<input type="hidden" name="pesquisar" value="1"> 
			<input type="hidden" id="exporta" name="exporta" value="false">
        	<div class="row">
                <div class="col-lg-6">
                	<?php
					$mundescricao = simec_htmlentities ( $_POST ['enjdsc'] );

					$arrAttr = array (
					    'placeHolder' => $this->enfdsc,
						'maxlength' => '100'
					);
					echo $simec->input ( 'enjdsc', 'Informe a '.$this->enfdsc, $mundescricao, $arrAttr, array('label-size' => '4') );
					?>
                </div>
			</div>
		</form>
	</div>
	<div class="ibox-footer">
        <div class="center">
            <button type="reset" class="btn btn-primary btn-pesquisar" style="width: 200px;"><i class="fa fa-search"></i> Pesquisar</button>
        </div>
    </div>
</div>

<?php if(array_key_exists('pesquisar', $_REQUEST)){ ?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista de <?php echo $this->enfdsc; ?></h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    	<?php
                
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Entidade');
        $esconderColunas = array('ddtid', 'ejfid', 'enfid', 'enjid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_demandantes);
         
        $listagemSimec->addAcao('view', 'abrirEntidades'); 
        $listagemSimec->setFormFiltros('formulario-entidade');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>
<?php } ?>
 
<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-entidade"]').submit();
});

function abrirEntidades(ddtid){
	var url = 'demandas.php?modulo=demandantes/verPainel&acao=A&ddtid='+ddtid;
	jQuery(location).attr('href', url);
}
</script>