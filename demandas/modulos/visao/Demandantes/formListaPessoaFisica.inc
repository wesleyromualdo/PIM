<?php
//$renderEntidade = new Demanda_Controller_Entidade();

?>
<div class="ibox">
	<div class="ibox-title">
        <div class="row">
        	<div class="col-md-12" >
    	       <h3 id="entidade">Dados do(a) <?php echo $this->entidades;?>(a)</h3>
            </div>
		</div>
	</div>
    <div class="ibox-content">
    	<div class="row">
    		<div class="col-lg-12">
<?php       if( (int)$this->qtd_ja_cadastrado < (int)$this->qtd_representante ){?>
                    <button type="button" class="btn btn-primary btn-incluir" style="width: 200px;" onclick="abreModalPessoa('');"><i class="fa fa-edit"></i> Incluir</button>
<?php       }?>
            </div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12">
    			<form method="post" name="formulario-lista-pessoa-juridica" id="formulario-lista-pessoa-juridica" class="form form-horizontal">
    			<input type="hidden" name="enfid" value="<?php echo $_REQUEST['enfid'];?>">
    			<input type="hidden" name="tipo" value="<?php echo $_REQUEST['tipo'];?>">
    			<input type="text" name="ejfid" value="<?php echo $this->ejfid;?>">
    			<input type="hidden" name="enrgestor" value="<?php echo $this->enrgestor;?>">
    			<input type="hidden" name="enrtipo"   id="enrtipo"  value="<?php echo $this->enrtipo; ?>"/>
    			
                	<?php
                	
                    $listagemSimec = new Simec_Listagem();
                    $arrayCabecalho = array('CPF', 'Nome', 'E-mail', 'Telefone');
                    $esconderColunas = array('enjid', 'ensid', 'ejfid', 'enrtipo', 'gestor', 'enfid', 'ensstatus', 'enjendcep', 'enjendlogradouro',
                	                           'enjendcomplemento', 'enjendnumero', 'enjendbairro', 'ensdatanascimento', 'ensrg', 'ensorgexpedidor', 'enssexo');
                    
                    $listagemSimec->setCabecalho($arrayCabecalho);
                    $listagemSimec->esconderColunas($esconderColunas);
                    //$listagemSimec->turnOnPesquisator();
                    $listagemSimec->setQuery( $this->sql_pessoafisica );
                    //ver(Simec_Listagem::TOTAL_QTD_REGISTROS,d);
                    $listagemSimec->addAcao('view', array('func' => 'abreModalPessoa', 'extra-params' => array('enrtipo')));
                    $listagemSimec->setFormFiltros('formulario-entidade');
                    $listagemSimec->addCallbackDeCampo('enscpf', 'formata_numero_cnpj_cpf');
                    $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                    $listagemSimec->setTamanhoPagina(50);
                    $listagemSimec->setCampos($arrayCabecalho);                    
                    $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);                    
                    ?>
				</form>
    		</div>
    	</div>
	</div>
</div>

<div class="ibox float-e-margins animated modal conteudo" id="modal-form-pessoa-fisica" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" id="printable">
        <div class="modal-content">
            <div class="ibox-content">            	
                <div class="row">
					<div class="col-lg-12" id="form_pessoa">
							
                    </div>
                </div>
            </div>
            <div class="ibox-footer" style="text-align: center;">
                <button type="button" id="btn-salvar-pessoa" class="btn btn-primary" data-loading-text="Salvando aguarde ..."><i class="fa fa-save"></i> Salvar</button>
                <button type="button" id="btn-fechar-pessoa" data-dismiss="modal" class="btn btn-success"><i class="fa fa-times-circle-o"></i> Fechar</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
function abreModalPessoa( ensid, enrtipo){
	jQuery("#form_pessoa").html('');

	var enfid = jQuery('[name="formulario-lista-pessoa-juridica"]').find('[name="enfid"]').val();
	var ejfid = jQuery('[name="formulario-lista-pessoa-juridica"]').find('[name="ejfid"]').val();
	var tipo = jQuery('[name="formulario-lista-pessoa-juridica"]').find('[name="tipo"]').val();
	var enrgestor = jQuery('[name="formulario-lista-pessoa-juridica"]').find('[name="enrgestor"]').val();
	var enrtipo = jQuery('[name="formulario-lista-pessoa-juridica"]').find('[name="enrtipo"]').val();
	
	var caminho = window.location.href;
	var action = '&requisicao=montar_tela_pessoa&ensid='+ensid+'&ejfid='+ejfid+'&enfid='+enfid+'&tipo='+tipo+'&enrtipo='+enrtipo+'&enrtipo='+enrtipo+'&enrgestor='+enrgestor;
	jQuery.ajax({
	    type: "POST",
	    url: caminho,
	    data: action,
	    async: false,
	    success: function (resp) {
	        jQuery("#form_pessoa").html(resp);
	    }
	});
	jQuery("#modal-form-pessoa-fisica").modal("show");
}

jQuery('#btn-salvar-pessoa').click(function(){	
	jQuery('[name="formulario-pessoa-fisica"]').find('[name="requisicao"]').val('salva_pessoa_fisica');
	jQuery('[name="formulario-pessoa-fisica"]').submit();
});

function getEnderecoByCEP(endcep,tipoendereco) {	
	url = '/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP';
	data = '&endcep=' + endcep;
	$.post(
		url
		, data
		, function(resp){

			var dados = resp.split("||");
			$( '#endlogradouro' ).val(dados[0]);
			$( '#endcomplemento' ).val('');
//			$( '#mundescricao').val(dados[2]);
			$( '#endnumero').val('');
			$( '#endbairro').val(dados[1]); 
		}
	);

}
</script>
