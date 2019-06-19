<div class="row">
	<div class="col-sm-12 col-md-12 col-lg-12">
		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h4>Filtro - Acesso Rápido</h4>
			</div>
			<div class="ibox-content" style="max-height: 300px; overflow-y: auto; overflow-x: auto;">
    			<div class="form-group ">
                    <label for="obrid_acessorapido" class="col-sm-3 col-md-3 col-lg-3 control-label">Obra: </label>
                    <div class="col-sm-5 col-md-5 col-lg-5 ">
                        <input name="obrid_acessorapido" id="obrid_acessorapido" type="text" value="" class="form-control" maxlength="50" placeholder="Digite o ID da obra" data-inputmask="'mask':'9999999999999', 'placeholder':''">                        
                    </div>
                    <div class="col-sm-4 col-md-4 col-lg-4">
						<button type="button" id="btn-acessar-acesso-rapido" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Acessar</button>
					</div>
                    <div style="clear:both"></div>
                </div>
                <div class="row">
                	<div class="col-sm-12 col-md-12 col-lg-12 listaDetalheObra" style="overflow-x:auto;">
                	
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
var functionObraNaoEncontrada = function (retorno)
{
	var formGroup = jQuery('#obrid_acessorapido', tab).parents('.form-group:first');
	formGroup.append('<div class="acessorapido-msg-obranaoencontrada col-sm-offset-3 col-md-offset-3 col-lg-offset-3 col-sm-7 col-md-7 col-lg-7">' +
						'<label class="bg-danger p-xxs font-bold">' + retorno + '</label>' +
					 '</div>');

// 	setTimeout(function () { jQuery('.acessorapido-msg-obranaoencontrada').remove() }, 3000);
	setTimeout(function () { jQuery('.acessorapido-msg-obranaoencontrada').fadeOut(1500, function(){ jQuery(this).remove();}); }, 3000);
}

var acrid  	= jQuery('.seletorAcessoRapido.active').attr('id-seletor-tab-acesso-rapido');
var tab 	= jQuery('#tabAcessoRapido-' + acrid);

jQuery('[name=obrid_acessorapido]', tab).unbind('keydown');
jQuery('[name=obrid_acessorapido]', tab).keydown(function (e)
{
// 	console.log('Felipe');
	if (e.keyCode == 13 && jQuery('[name=obrid_acessorapido]', tab).val()) {
    	e.preventDefault();//evito o submit do form ao apertar o enter..
		acessoRapidoBuscarObra();
	}
});

jQuery('#btn-acessar-acesso-rapido', tab).unbind('click');
jQuery('#btn-acessar-acesso-rapido', tab).click(function ()
{	
	acessoRapidoBuscarObra();
});

function acessoRapidoBuscarObra()
{
// 	var acrid  = jQuery('.seletorAcessoRapido.active').attr('id-seletor-tab-acesso-rapido');
	var obrid = jQuery('#obrid_acessorapido', tab).val();
	var dados = {acrid:acrid, obrid:obrid};
	
	if (obrid == '') {
		swal({title:'', text:'Digite algum <b>id</b> de obra para acessá-lo.', type:'error', html:true});
		return false;
	}
	 
	aplicarFiltroAcessoRapido(dados, true, functionObraNaoEncontrada);
}

jQuery('#obrid_acessorapido').change(function ()
{
	var obrid = jQuery('#obrid_acessorapido').val();
	if (obrid) {
        jQuery.ajax({
        	method		: "POST",
        	url			: location.href,
        	data		: {requisicao : 'acessoRapidoAplicarAcaoEspecifica', 
            			   obrid : obrid, 
            			   acrid : acrid, 
            			   metodo : 'montarTelaListaObraDetalhada'},
        	beforeSend	: function() 
        	{
            	jQuery('.listaDetalheObra', tab).html('<i class="fa fa-spinner"></i> Carregando...');
            },
        	success 	: function (resposta)
        	{
        		jQuery('.listaDetalheObra', tab).html(resposta);
        		jQuery('#modalFiltroAcessoRapido').find('.modal-dialog').css({
            		'max-width':'90%', 
            		'max-height':'90%', 
            		'width':'auto', 
            		'height':'auto'
                });
        	},
        	error		: function ()
        	{
        		swal({title:'', text:'Falha ao buscar a lista detalhada da obra.<br>Tente novamente!', type:'error', html:true});
        	}
        });
	} else {
		jQuery('.listaDetalheObra', tab).html('');
	}
});
</script>
