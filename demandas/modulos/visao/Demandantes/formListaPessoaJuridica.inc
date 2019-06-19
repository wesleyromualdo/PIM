<?php

?>
<div class="ibox">
	<div class="ibox-title">
        <div class="row">
        	<div class="col-md-12" >
    	       <h3 id="entidade">Dados do(a) <?php echo $this->entidade[$this->demandante['enfid']];?>(a)</h3>
            </div>
		</div>
	</div>
    <div class="ibox-content">
    	<div class="row">
    		<div class="col-lg-12">
<?php       if( (int)$this->qtd_ja_cadastrado < (int)$this->qtd_representante ){?>
                    <button type="button" class="btn btn-primary btn-incluir" style="width: 200px;" onclick="abreModalPessoa(<?php echo $this->demandante['ejfid']; ?>, '');"><i class="fa fa-edit"></i> Incluir</button>
<?php       }?>
            </div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12">
    			<input type="hidden" name="enfid" value="<?php echo $this->demandante['enfid'];?>">
                	<?php                	
                    $listagemSimec = new Simec_Listagem();
                    $arrayCabecalho = array('CNPJ', 'Nome', 'E-mail', 'Telefone');
                    $esconderColunas = array('enjid', 'enfid', 'ejfid', 'enjstatus', 'enjrazaosocial', 'enjinscricaoestadual', 'enjendcep', 'enjendlogradouro', 'enjendcomplemento', 'enjendnumero', 'enjendbairro', 'muncod');
                    
                    $listagemSimec->setCabecalho($arrayCabecalho);
                    $listagemSimec->esconderColunas($esconderColunas);
                    //$listagemSimec->turnOnPesquisator();
                    $listagemSimec->setQuery( $this->sql_pessoajuridica );
                    
                    if( !empty($this->verifica_gestor) ){
                        $listagemSimec->addAcao('plus', array('func' => 'abreListaGetor', 'extra-params' => array('enjid')));
                    }
                    $listagemSimec->addAcao('view', array('func' => 'abreModalPessoa', 'extra-params' => array('ejfid')));
                    //$listagemSimec->addAcao('view', 'abreModalPessoa');
                    $listagemSimec->setFormFiltros('formulario-entidade');
                    $listagemSimec->addCallbackDeCampo('enjcnpj', 'formata_numero_cnpj_cpf');
                    $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                    $listagemSimec->setTamanhoPagina(50);
                    $listagemSimec->setCampos($arrayCabecalho);                    
                    $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);                    
                    ?>
    		</div>
    	</div>
	</div>
</div>

<div class="ibox float-e-margins animated modal conteudo" id="modal-form-pessoa-juridica" tabindex="-1" role="dialog"
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
                <button type="button" id="btn-fechar-pessoa" data-dismiss="modal" class="btn btn-success"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-salvar-pessoa" class="btn btn-primary" data-loading-text="Salvando aguarde ..."><i class="fa fa-save"></i> Salvar</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">

function abreModalPessoa( enjid, ejfid ){
	jQuery("#form_pessoa").html('');
	
	var caminho = window.location.href;
	var action = '&requisicao=montar_tela_pessoa&enjid_pai='+enjid+'&enfid_pai='+jQuery('[name="enfid"]').val()+'&ejfid_pai='+ejfid;
	jQuery.ajax({
	    type: "POST",
	    url: caminho,
	    data: action,
	    async: false,
	    success: function (resp) {
	        jQuery("#form_pessoa").html(resp);
	    }
	});
	jQuery("#modal-form-pessoa-juridica").modal("show");
}

jQuery('#btn-salvar-pessoa').click(function(){	
	jQuery('[name="formulario-pessoa-juridica"]').find('[name="requisicao"]').val('salva_pessoa_juridica');
	jQuery('[name="formulario-pessoa-juridica"]').submit();
});

$(document).ready(function()
{
	$('.next').click(function(){
		var url = window.location.href.toString().replace('prefeitura', 'prefeito');

		if (url.indexOf('prefeito'))
			url = url + '&menu=prefeito';

		window.location.href = url;
	});

	/*$("#enjcnpj").inputmask({
		mask: ['999.999.999-99', '99.999.999/9999-99'],
	    keepStatic: true
	});*/
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