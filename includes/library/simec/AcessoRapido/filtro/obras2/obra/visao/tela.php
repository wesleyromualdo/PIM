<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" style="width: 100%">
	<tr>
		<td colspan="3">
			<h4>Filtro - Acesso Rápido</h4>
		</td>
	</tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Obra:</td>
        <td width="40%">
        <?php echo campo_texto('obrid_acessorapido', 'N', 'S', '', 30, 15, '###############', '', 'left', '', 0, 'placeholder="Digite o ID da obra"'); ?>
        </td>
        <td align="left" width="45%">
        	<input type="button" name="pesquisar" id="btn-acessar-acesso-rapido" class="pesquisar" value="Acessar"/>	
        </td>
    </tr>
    <tr>
    	<td colspan="3" class="listaDetalheObra" style="padding-top: 15px;"></td>
    </tr>
</table>
<?php
if (count($dados['menuAcessoDireto']) > 0) {
?>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" style="width: 100%">
	<tr>
		<td colspan="3">
			<h4>Menu - Acesso Direto</h4>
		</td>
	</tr>
	<tr>
		<td colspan="3">
			<div style="overflow:auto; max-height: 300px;">
            <?php
            $mnudscpai = '';
    		foreach ($dados['menuAcessoDireto'] as $k => $menu) {
    		    if ($menu['mnudscpai'] != $mnudscpai) {
    		?>
                	<h4><?php echo $menu['mnudscpai']; ?></h4>	
    			<?php
                    $mnudscpai = $menu['mnudscpai'];
    		    }
                ?>
            		<input type="button" name="btnAcessoRapido_<?php echo $k; ?>" value="<?php echo $menu['mnudsc']; ?>" onclick="abrirLinkAcessoDireto('<?php echo $menu['mnulink'] ?>')">
    		<?php 
                    echo ($dados['menuAcessoDireto'][($k+1)]['mnudscpai'] == $mnudscpai) ? '|' : '';
    		}
            ?>
			</div>
		</td>
	</tr>
</table>
<?php 
}
?>

<script type="text/javascript">
var functionObraNaoEncontrada = function (retorno)
{
	var td = jQuery('[name=obrid_acessorapido]', tab).parents('td:first');
	console.log(td);
	td.append('<div class="acessorapido-msg-obranaoencontrada" style="color:red; font-weight:bold;">' +
			  	retorno +
			  '</div>');

	setTimeout(function () { jQuery('.acessorapido-msg-obranaoencontrada').fadeOut(1500, function(){ jQuery(this).remove();}); }, 3000);
}

var acrid  	= jQuery('.seletorAcessoRapido.ui-tabs-active').attr('id-seletor-tab-acesso-rapido');
var tab 	= jQuery('#tabAcessoRapido-' + acrid);

jQuery('[name=obrid_acessorapido]', tab).unbind('keydown');
jQuery('[name=obrid_acessorapido]', tab).keydown(function (e)
{
	console.log('Felipe');
	if (e.keyCode == 13 && jQuery('[name=obrid_acessorapido]', tab).val()) {
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
	var obrid = jQuery('[name=obrid_acessorapido]', tab).val();
	var dados = {acrid:acrid, obrid:obrid};
	
	if (obrid == '') {
		alert('Digite algum id de obra para acessá-lo.');
		return false;
	}
	 
	aplicarFiltroAcessoRapido(dados, true, functionObraNaoEncontrada);
}

jQuery('[name=obrid_acessorapido]').change(function ()
{
	var obrid = jQuery('[name=obrid_acessorapido]').val();
	if (obrid) {
        $1_11.ajax({
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