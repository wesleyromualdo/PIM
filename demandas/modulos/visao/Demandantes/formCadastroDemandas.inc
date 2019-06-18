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
    <?php echo $simec->tab($this->abasDemanda, 'demandas.php?modulo=demandantes/cadastarDemanda&acao=A&ddtid='.$_REQUEST['ddtid'] ); ?><br>
    	<div class="row">
    		<div class="col-lg-12">
        	<form method="post" name="formulario-demanda-cadastro" id="formulario-demanda-cadastro">
    			<input type="hidden" name="requisicao" value="">
    			<input type="hidden" name="demid" value="<?php echo $_REQUEST['demid'];?>">
            	<div class="row">
					<div class="col-lg-6">
                    <?php
                        $attrCombo = array();
                        if( $this->demid ){
                            $attrCombo = array('disabled' => 'disabled');
                        }
                        
                        $sql = "SELECT dctid AS codigo, dctdsc AS descricao FROM demandas.demandas_portfolio_tipo;";
                        echo $simec->select('dctid', 'Tipo Portfólio', $this->dctid, $sql, $attrCombo, array('input-size' => '8'));
    				?>
                    </div>   
				</div>
				<div class="row">
                    <div class="col-lg-6">
                    <?php 
                        if( $this->demid ){
                            $sql = "SELECT dc.dcoid AS codigo, dd.dcddsc AS descricao
                                    FROM demandas.demandas_portfolio dc
                                    	INNER JOIN demandas.demandas_portfolio_descricao dd ON dd.dcdid = dc.dcdid
                                    WHERE dc.dcostatus = 1";
                        } else {
                            $sql = array();
                        }
                        echo $simec->select('dcoid', 'Portfólio', $this->dcoid, $sql, $attrCombo, array('input-size' => '8'));
                    ?>
                    </div>
				</div>
<?php           if( $this->demid ){ ?>
				<div class="row">
                    <div class="col-lg-12" id="demanda_itens">
                    	<?php echo $this->html_itens;?>
                    </div>
                </div>
<?php           }?>
			</form>
    		</div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12" style="text-align: center;">
                    <button type="button" class="btn btn-primary btn-salvar" style="width: 200px;"><i class="fa fa-save"></i> Salvar</button>
            </div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12" style="height: 50px;">&nbsp;</div>
    	</div>
	</div>
</div>

<script type="text/javascript">

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

jQuery('.vlrquantidade').focusin(function(){
	if( parseInt( jQuery(this).val() ) == '0' ){
		jQuery(this).val('');
	}
});

$('.vlrquantidade').focusout(function(){
	if( $(this).val() == '' ){
		$(this).val('0');
	}
});

function calculaTotalItens( dciid ){
	var valor_referencia = jQuery('[name="valor_referencia['+dciid+']"]').val();
	var deiquantidade = jQuery('[name="deiquantidade['+dciid+']"]').val();

	var valor = parseFloat(deiquantidade) * parseFloat(valor_referencia);

	jQuery('#valor_total_'+dciid).html(  number_format(valor, 2, ",", ".") );

	var dciid_tot = 0;
	var valor_referencia_tot = 0;
	var deiquantidade_tot = 0;
	var valorTotal = 0;
	jQuery('[name="dciid[]"]').each(function(){
		dciid_tot = jQuery(this).val();

		valor_referencia_tot = jQuery('[name="valor_referencia['+dciid_tot+']"]').val();
		deiquantidade_tot = jQuery('[name="deiquantidade['+dciid_tot+']"]').val();
		valorTotal += parseFloat(deiquantidade_tot) * parseFloat(valor_referencia_tot);
		
	});

	jQuery('#div_valor_geral').html(  number_format(valorTotal, 2, ",", ".") );
}

function incluirItensDemanda(){

	var cidid = jQuery('[name="cidid"]').val();
	var dcoid = jQuery('[name="dcoid"]').val();

	jQuery('[name="requisicao"]').val('incluirLinhaItensDemanda');

	var form = jQuery('[name="formulario-demanda-cadastro"]').serialize();
	
	var caminho = window.location.href;
	var action  = '&cidid='+cidid+'&dcoid='+dcoid+'&'+form;
	jQuery.ajax({
	    type: "POST",
	    url: caminho,
	    data: action,
	    async: false,
	    success: function (resp) {
		    //console.log(resp);
		    jQuery('#demanda_itens').html(resp);
	        //jQuery('#debug').html(resp);
	        /*if(jQuery('#dados-itens-demanda-tbody tr:last').length == 0){
	        	jQuery('#dados-itens-demanda-tbody').append(resp);
	        }else{
	        	jQuery('#dados-itens-demanda-tbody tr:last').after(resp);
	        }*/
	    }
	});
}

jQuery('.btn-salvar').click(function(){

	jQuery('[name="requisicao"]').val('salvar_demanda_itens');
	jQuery('[name="formulario-demanda-cadastro"]').submit();

});

</script>