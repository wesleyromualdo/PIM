<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" style="width: 100%">
	<tr>
		<td colspan="2">
			<h4>Filtro - Acesso Rápido</h4>
		</td>
	</tr>
    <tr>
        <td class="SubTituloDireita" width="25%">Esfera:</td>
        <td>
        <?php campo_radio( "itrid_acessorapido", $dados['arrEsfera'], 'h'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Estado:</td>
        <td>
        <?php $db->monta_combo("estuf_acessorapido", $dados['arrEstado'], 'S', 'Selecione...', 'filtrarMunicipio', '', '', '', 'N', 'estuf_acessorapido'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Município:</td>
        <td>
        <?php $db->monta_combo("muncod_acessorapido", $dados['arrMunicipio'], 'S', 'Selecione...', '', '', '', '', 'N', 'muncod_acessorapido'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">&nbsp;</td>
        <td>
        	<input type="button" name="btn-acessar-acesso-rapido" id="btn-acessar-acesso-rapido" class="pesquisar" value="Acessar"/>	
        </td>
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
<style>
#acesso-rapido-tab .chosen-container .chosen-results {
    max-height:170px;
}
</style>
<script type="text/javascript">
var acrid  	= $1_11('.seletorAcessoRapido.ui-tabs-active').attr('id-seletor-tab-acesso-rapido');
var tab 	= $1_11('#tabAcessoRapido-' + acrid)[0];

$1_11('#estuf_acessorapido', tab).chosen({'width': '250px'});
$1_11('#muncod_acessorapido', tab).chosen();

$1_11('[name=itrid_acessorapido]', tab).unbind('change');
$1_11('[name=itrid_acessorapido]', tab).change(function()
{
    if ($1_11(this).val() == 1) {
    	$1_11('#estuf_acessorapido:first', tab).parents('tr:first').show();
        $1_11('#muncod_acessorapido:first', tab).parents('tr:first').hide();
        $1_11('#muncod_acessorapido:first', tab).find('option').prop('selected', false);
        $1_11('#muncod_acessorapido:first', tab).trigger('chosen:updated');
    } else {
//         $1_11('#estuf_acessorapido', tab).find('option').prop('selected', false);
//         $1_11('#estuf_acessorapido', tab).trigger('chosen:updated');
    	$1_11('#estuf_acessorapido:first', tab).parents('tr:first').show();
    	$1_11('#muncod_acessorapido:first', tab).parents('tr:first').show();
    }
    return true;
});
$1_11('[name=itrid_acessorapido]:checked', tab).change();

$1_11('#btn-acessar-acesso-rapido', tab).unbind('click');
$1_11('#btn-acessar-acesso-rapido', tab).click(function ()
{	
	var acrid  = $1_11('.seletorAcessoRapido.ui-tabs-active').attr('id-seletor-tab-acesso-rapido');
	var esfera = $1_11('[name=itrid_acessorapido]:checked', tab).val();
	var dados = {acrid:acrid, itrid:esfera};
	
	if (esfera == 1) {
		dados.estuf = $1_11('#estuf_acessorapido', tab).val();
		if (dados.estuf == '') {
			alert('Selecione algum estado para acessá-lo.');
			return;
		}	
	} else {
		dados.muncod = $1_11('#muncod_acessorapido', tab).val();
		if (dados.muncod == '') {
			alert('Selecione algum município para acessá-lo.');
			return;
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
	
    $1_11.ajax({
    	method		: "POST",
    	url			: location.href,
    	data		: dados,
    	beforeSend	: function() 
    	{
			$1_11('#muncod_acessorapido', tab).find('option').remove();
			$1_11('#muncod_acessorapido', tab).append('<option value="" selected="selected">Aguarde, carregando...</option>');
        },
    	success 	: function (resposta)
    	{
    		try {
    			var obj =  $1_11.parseJSON(resposta);
    			
    			$1_11('#muncod_acessorapido', tab).find('option').remove();
				$1_11('#muncod_acessorapido', tab).append('<option value="" selected="selected">Selecione...</option>');
				
    			for (i = 0; i < obj.arrMunicipio.length; i++) {
    				$1_11('#muncod_acessorapido', tab).append('<option value="'+ obj.arrMunicipio[i].codigo +'">'+ obj.arrMunicipio[i].descricao +'</option>');
        		}
        		
    			$1_11('#muncod_acessorapido', tab).trigger('chosen:updated');
    		} catch {
    			alert(resposta);
    		}
    	},
    	error		: function ()
    	{
    		alert('Falha ao filtrar os municípios.\nTente novamente!');
    	}
    });	
}
</script>
