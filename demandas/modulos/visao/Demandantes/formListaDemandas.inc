<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="left"><?php echo $this->enjdsc; ?></h2>
            </div>
        </div>
    </div>
    <div class="ibox-title">
        <?php echo montaCabecalhoDemanda($_REQUEST['ddtid']);?>
    </div>    
    <div class="ibox-content">
		<?php echo $simec->tab($this->abasDemanda, 'demandas.php?modulo=demandantes/listarDemanda&acao=A&ddtid='.$_REQUEST['ddtid'] ); ?>
    	<br>
    	<div class="row">
    		<div class="col-lg-12">
            	<form method="post" name="formulario-lista-demanda" id="formulario-lista-demanda">
        			<input type="hidden" name="requisicao" value=""> 
        			
                    <div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	$sql = "SELECT dctid AS codigo, dctdsc AS descricao FROM demandas.demandas_portfolio_tipo;";
                        	echo $simec->select('dctid', 'Tipo Portfólio', $_REQUEST['dctid'], $sql, array(), array('input-size' => '8'));
    						?>
                        </div>
    				</div>
    				<div class="row">
                        <div class="col-lg-6">
                        	<?php
                        	$sql = "SELECT dc.dcoid AS codigo, dd.dcddsc AS descricao
                                    FROM demandas.demandas_portfolio dc
                                    	INNER JOIN demandas.demandas_portfolio_descricao dd ON dd.dcdid = dc.dcdid
                                    WHERE dc.dcostatus = 1";
                        	echo $simec->select('dcoid', 'Portfólio', $_REQUEST['dcoid'], $sql, array(), array('input-size' => '8'));
    						?>
                        </div>
    				</div>
        		</form>
    		</div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12" style="text-align: center;">
    			<button type="button" class="btn btn-primary btn-pesquisar" style="width: 200px;"><i class="fa fa-search"></i> Pesquisar</button>
            </div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12">
			<?php
			if( $this->sql_demandar ){
                $listagemSimec = new Simec_Listagem();
                $arrayCabecalho = array('Código', 'Tipo', 'Portfólio', 'Situação', 'Quem Fez', 'Data');
                $esconderColunas = array('docid');
                $listagemSimec->setCabecalho($arrayCabecalho);
                $listagemSimec->esconderColunas($esconderColunas);
                $listagemSimec->turnOnPesquisator();
                $listagemSimec->setQuery($this->sql_demandar);
                //$listagemSimec->addAcao('plus', array('func' => 'detalharProcesso', 'extra-params' => array('tipo')));
                $listagemSimec->addAcao('view', array('func' => 'abrirDemanda', 'extra-params' => array('demid')));
                //$listagemSimec->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
                $listagemSimec->setFormFiltros('formulario-entidade');
                $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                $listagemSimec->setTamanhoPagina(50);
                $listagemSimec->setCampos($arrayCabecalho);
                
                $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
			}
            ?>
      		</div>
    	</div>
	</div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="requisicao"]').val('pesquisar-demanda');
	jQuery('[name="formulario-lista-demanda"]').submit();
});

function abrirDemanda( demid ){	
	var url = 'demandas.php?modulo=demandantes/cadastarDemanda&acao=A&ddtid=<?php echo $_REQUEST['ddtid'];?>&demid='+demid;
	jQuery(location).attr('href',url);
}

jQuery('[name="dctid"]').change(function(){
	
    var options = jQuery('#dcoid');
    options.empty();
    options.trigger('chosen:updated');
    options.append(new Option('', ''));
    jQuery.ajax({
        type: "GET",
        url: window.location.href,
        data: { requisicao:'carregarCardapio',dctid : jQuery(this).val() },
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
</script>