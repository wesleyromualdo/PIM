<div class="row">
	<div class="col-sm-12 col-md-12 col-lg-12">
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h4>Filtro - Acesso Rápido</h4>
			</div>
			<div class="ibox-content" style="height: 200px; /*overflow-y: hidden; overflow-x: hidden;*/">
        		<?php echo $simec->radio('itrid_acessorapido', 'Esfera', (1), $dados['arrEsfera'], array(), array('label-size' => 3)); ?>
        		<?php echo $simec->select('estuf_acessorapido', 'Estado', $_REQUEST['estuf'], $dados['arrEstado'], array('onchange'=>'filtrarMunicipio(this.value)'), array('label-size'=>3, 'input-size'=>7)); ?>
        		<?php echo $simec->select('muncod_acessorapido', 'Município', $_REQUEST['muncod'], $dados['arrMunicipio'], array(), array('label-size' => 3, 'input-size' => 7)); ?>
				<div class="row">
					<div class="col-sm-12 col-md-12 col-lg-12 col-sm-offset-3 col-md-offset-3 col-lg-offset-3">
						<button type="button" id="btn-acessar-acesso-rapido" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Acessar</button>
					</div>
				</div>    		
			</div>
		</div>
	</div>
</div>
<?php
if (count($dados['menuAcessoDireto']) > 0) {
?>
<div class="row">
	<div class="col-sm-12 col-md-12 col-lg-12">
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h4>Menu - Acesso Direto</h4>
			</div>
			<div class="ibox-content" style="max-height: 200px; overflow-y: auto; overflow-x: auto;">				
                <div class="row">
                <?php
                $mnudscpai = '';
        		foreach ($dados['menuAcessoDireto'] as $k => $menu) {
        		    if ($menu['mnudscpai'] != $mnudscpai) {
        		?>
                        <div class="row">
                        	<div class="col-sm-12 col-md-12 col-lg-12" style="overflow-x:auto;">
                            	<h4><?php echo $menu['mnudscpai']; ?></h4>	
                        	</div>
                        </div>        		
                        <div class="row">
                        	<div class="col-sm-12 col-md-12 col-lg-12" style="overflow-x:auto;">
        			<?php
                        $mnudscpai = $menu['mnudscpai'];
        		    }
                    ?>
                			<button type="button" id="btnAcessoRapido_<?php echo $k; ?>" class="btn btn-primary" onclick="abrirLinkAcessoDireto('<?php echo $menu['mnulink'] ?>')"><?php echo $menu['mnudsc']; ?></button>
                    <?php 
                            echo ($dados['menuAcessoDireto'][($k+1)]['mnudscpai'] == $mnudscpai) ? '|' : '';
                    if ($dados['menuAcessoDireto'][($k+1)]['mnudscpai'] != $mnudscpai) {
                    ?>        	
                        	</div>
                        </div>
        		<?php
                    }
        		}
                ?>
                </div>
			</div>
		</div>
	</div>
</div>
<?php 
}
?>
<script type="text/javascript">
var acrid  = jQuery('.seletorAcessoRapido.active').attr('id-seletor-tab-acesso-rapido');
var tab    = jQuery('#tabAcessoRapido-<?php echo $dados['acrid']; ?>');

jQuery('[name=itrid_acessorapido]', tab).unbind('change');
jQuery('[name=itrid_acessorapido]', tab).change(function()
{
    if (jQuery(this).val() == 1) {
    	jQuery('#estuf_acessorapido', tab).parents('.form-group:first').show();
        jQuery('#muncod_acessorapido', tab).parents('.form-group:first').hide();
        jQuery('#muncod_acessorapido', tab).find('option').prop('selected', false);
        jQuery('#muncod_acessorapido', tab).trigger('chosen:updated');
    } else {
//         jQuery('#estuf_acessorapido', tab).find('option').prop('selected', false);
//         jQuery('#estuf_acessorapido', tab).trigger('chosen:updated');
//     	jQuery('#estuf_acessorapido', tab).parents('.form-group:first').hide();
    	jQuery('#estuf_acessorapido', tab).parents('.form-group:first').show();
    	jQuery('#muncod_acessorapido', tab).parents('.form-group:first').show();
    }
});
jQuery('[name=itrid_acessorapido]:checked', tab).change();


// jQuery('select[name=estuf_acessorapido]').change(function()
// {
// //     if(!jQuery('#muncod_acessorapido').parents('.form-group:first').is(':visible')) {
// //         return false;
// //     }
//     carregarMunicipio(this.value, 'valueMunicipio', '#muncod_acessorapido');
// });
jQuery('#btn-acessar-acesso-rapido', tab).unbind('click');
jQuery('#btn-acessar-acesso-rapido', tab).click(function ()
{	
	var acrid  = jQuery('.seletorAcessoRapido.active').attr('id-seletor-tab-acesso-rapido');
	var esfera = jQuery('[name=itrid_acessorapido]:checked', tab).val();
	var dados  = {acrid:acrid, itrid:esfera};
	
	if (esfera == 1) {
		dados.estuf = jQuery('#estuf_acessorapido', tab).val();
		if (dados.estuf == '') {
			swal({title:'', text:'Selecione algum <b>estado</b> para acessá-lo.', type:'error', html:true});
			return false;
		}	
	} else {
		dados.muncod = jQuery('#muncod_acessorapido', tab).val();
		if (dados.muncod == '') {
			swal({title:'', text:'Selecione algum <b>município</b> para acessá-lo.', type:'error', html:true});
			return false;
		}	
	}
	 
	aplicarFiltroAcessoRapido(dados, true);
});

function filtrarMunicipio(estuf)
{
	var dados = {
		'acrid'			: acrid, 
		'requisicao'	: 'acessoRapidoAplicarAcaoEspecifica', 
		'metodo' 		: 'getMunicipioFiltrado',
		'estuf'			: estuf
	};
	
    jQuery.ajax({
    	method		: "POST",
    	url			: location.href,
    	data		: dados,
    	beforeSend	: function() 
    	{
			jQuery('#muncod_acessorapido', tab).find('option').remove();
			jQuery('#muncod_acessorapido', tab).append('<option value="" selected="selected">Aguarde, carregando...</option>');
        },
    	success 	: function (resposta)
    	{
    		try {
    			var obj =  jQuery.parseJSON(resposta);
    			
    			jQuery('#muncod_acessorapido', tab).find('option').remove();
				jQuery('#muncod_acessorapido', tab).append('<option value="" selected="selected">Selecione...</option>');
				
    			for (i = 0; i < obj.arrMunicipio.length; i++) {
    				jQuery('#muncod_acessorapido', tab).append('<option value="'+ obj.arrMunicipio[i].codigo +'">'+ obj.arrMunicipio[i].descricao +'</option>');
        		}
        		
    			jQuery('#muncod_acessorapido', tab).trigger('chosen:updated');
    		} catch {
    			swal({title:'', text:resposta, type:'error', html:true});
    		}
    	},
    	error		: function ()
    	{
			swal({title:'', text:'Falha ao filtrar os municípios.\nTente novamente!', type:'error', html:true});
    	}
    });	
}

</script>
