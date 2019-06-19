<?php 
?>
<form method="post" name="formCadAcessoRapido" id="formCadAcessoRapido">
    <input type="hidden" name="requisicao" id="requisicao" value=""/>
    <input type="hidden" name="acrid" id="acrid" value=""/>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" >Nome do filtro:</td>
            <td>
            <?php 
            echo campo_texto('acrdsc','S','S','',49,100,'','');
            ?>
            </td>
		</tr>
        <tr>
            <td class="SubTituloDireita" >Arquivo de filtro:</td>
            <td>
            <?php 
            $db->monta_combo("acrfiltro", $this->comboFiltro, 'S',"-- Selecione o filtro --",'buscarPerfis();','','','400','S');
            ?>
            </td>
		</tr>
        <tr>
            <td class="SubTituloDireita" >Ordem de exibição do filtro:</td>
            <td>
            <?php 
            echo campo_texto('acrordem','S','S','',10,5,'#####','','right');
            ?>
            </td>
		</tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Módulos que exibirão o filtro:</td>
            <td>
            <?php 
            $db->monta_combo_multiplo("sisid", $this->comboModulo, 'S',"-- Selecione o módulo --",'buscarPerfis();','','','1','400');
            ?>
            </td>
		</tr>
        <tr>
            <td class="SubTituloDireita" >Perfis que terão acesso:</td>
            <td id="tdListaPerfil">
            	Selecione um <b>módulo</b> e um <b>arquivo de filtro</b>.
            </td>
		</tr>
        <tr>
            <td class="SubTituloDireita" ></td>
            <td>
            	<input type="button" name="btn_salvar" id="btn_salvar" value="Salvar" onclick="salvarAcessoRapido();">
            	<input type="button" name="btn_limpar" id="btn_limpar" value="Limpar" onclick="location.href = '?modulo=acessoRapido/cadastro&acao=A';">
            </td>
		</tr>		
		<tr id="trListaAcessoRapido">
			<td colspan="2" style="background: #FFFFFF;">
				<fieldset style="border: 1px solid #DFDFDF;">
					<legend>Lista</legend>
					<div id="listaAcessoRapido"></div>
				</fieldset>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
function buscarPerfis()
{
	var sisid  	  = $1_11('[name*=sisid]').val();
	var acrfiltro = $1_11('[name=acrfiltro]').val();
	
	if (sisid && acrfiltro) {
		acrfiltro = acrfiltro.split('/');
		
		$1_11.ajax({
	        method		: "POST",
			url			: location.href,
			data		: {requisicao:'buscarPerfil', sisid:sisid, sisdiretorio:acrfiltro[0]},
			beforeSend	: function() {
// 			    divCarregando();
			},
			success 	: function (resposta)
			{
// 				divCarregado();
				
				try {
					var obj =  $1_11.parseJSON(resposta);

					var selectPflcod = $1_11('<select></select>').attr({multiple:'multiple', name:'pflcod[]', id:'pflcod'});
					var sisdiretorio = '';
					var optgroup = '';
					$1_11.each(obj, function (indice, linha) 
					{
						if (sisdiretorio != linha.sisdiretorio) {
    						optgroup = $1_11('<optgroup></optgroup>').attr({label:linha.sisdiretorio.toUpperCase()});
    						selectPflcod.append(optgroup);
    						sisdiretorio = linha.sisdiretorio;
						}
						
						var option = $1_11('<option></option>').attr({value:linha.pflcod}).html(linha.pfldsc + ' - ' + linha.sisdiretorio);

						_JS.pflcod = _JS.pflcod || [];
						if (_JS.pflcod.indexOf(linha.pflcod) >= 0) {
							option.prop('selected', true);
						}	
						optgroup.append(option);
// 						optgroup.append( $1_11('<option></option>').attr({value:linha.pflcod}).html(linha.pfldsc) );
					});
					
					$1_11("#pflcod").chosen("destroy");
					$1_11('#tdListaPerfil').html('').append( selectPflcod );
					$1_11("#pflcod").chosen({no_results_text: "Não foram encontrados resultados:", placeholder_text_multiple:'-- Selecione os perfis --'});
// 					$1_11("#pflcod").trigger("chosen:updated");
				} catch {
				    alert('Falhar ao buscar os dados da combo perfil.\nTente novamente!');
				    location.href = location.href; 				
				}
			},
			error		: function ()
			{
// 				divCarregado();
				alert('Erro ao buscar os dados da combo perfil.\nTente novamente!');
			    location.href = location.href; 				
			}
		});
	} else {
		$1_11('#tdListaPerfil').html('Selecione um <b>módulo</b> e um <b>arquivo de filtro</b>.');
	}
}

function buscarListaAcessoRapido(sisid)
{
	$1_11.ajax({
        method		: "POST",
		url			: location.href,
		data		: {requisicao:'buscarListaAcessoRapido', sisid:sisid},
		beforeSend	: function() {
		    $1_11('#trListaAcessoRapido').show();
		    $1_11('#listaAcessoRapido').html('').html('<center><img src="../imagens/carregando.gif"></center>')
		},
		success 	: function (resposta)
		{
			try {
				$1_11('#listaAcessoRapido').html(resposta);
			} catch {
			    alert('Falhar ao buscar a listagem de acesso rápido.\nTente novamente!');
			    location.href = location.href; 				
			}
		},
		error		: function ()
		{
			alert('Erro ao buscar a listagem de acesso rápido.\nTente novamente!');
		    location.href = location.href; 				
		}
	});
}
buscarListaAcessoRapido();

validacaoAutomatica = new ValidacaoAutomatica();

validacaoAutomatica.addCampo('name', 'sisid', 'Módulo')
				   .addValidaObrigatorio();

validacaoAutomatica.addCampo('name', 'acrdsc', 'Descrição')
					.addValidaObrigatorio();

validacaoAutomatica.addCampo('name', 'acrfiltro', 'Arquivo filtro')
					.addValidaObrigatorio();

validacaoAutomatica.addCampo('name', 'acrordem', 'Ordem')
                   .addValidaObrigatorio();		

validacaoAutomatica.addCampo('id', 'pflcod', 'Perfis que terão acesso')
                   .addValidaObrigatorio();		

function salvarAcessoRapido()
{
	validacaoAutomatica.enviarFormulario('formCadAcessoRapido', 'requisicao');
}

function alterarAcessoRapido(acrid)
{
	$1_11.ajax({
        method		: "POST",
		url			: location.href,
		data		: {requisicao:'editar', acrid:acrid},
		beforeSend	: function() {
			
		},
		success 	: function (resposta)
		{
			try {
				var obj =  $1_11.parseJSON(resposta);
				
			 	$1_11('#acrid').val(obj.acrid);
			 	for (var i=0; obj.sisid.length > i; i++) {
			 		$1_11('[name*=sisid]').find('option[value=' + obj.sisid[i] + ']').prop('selected', true);	
				}
// 			 	$1_11('[name=sisid]').val(obj.sisid);
			 	$1_11('[name*=sisid]').trigger("chosen:updated");	
			 	$1_11('[name=acrdsc]').val(obj.acrdsc);
			 	$1_11('[name=acrfiltro]').val(obj.acrfiltro);	
			 	$1_11('[name=acrordem]').val(obj.acrordem);	
			 	
			 	_JS.pflcod = obj.pflcod;
			 	buscarPerfis();

			} catch {
			    alert('Falhar ao buscar os dados de edição.\nTente novamente!');
			    location.href = location.href; 				
			}
		},
		error		: function ()
		{
			alert('Erro ao buscar os dados de edição.\nTente novamente!');
		    location.href = location.href; 				
		}
	});
}

function excluirAcessoRapido(acrid)
{
	if (confirm('Deseja excluir este registro?')) {
		$1_11.ajax({
	        method		: "POST",
			url			: location.href,
			data		: {requisicao:'inativar', acrid:acrid},
			beforeSend	: function() {
				
			},
			success 	: function ()
			{
				if ($1_11('#acrid').val()) {
    			 	$1_11('#acrid').val('');	
//     			 	$1_11('[name=sisid]').val('');
    			 	$1_11('[name*=sisid]').find('option').prop('selected', false);
    			 	$1_11('[name*=sisid]').trigger("chosen:updated");	
    			 	$1_11('[name=acrdsc]').val('');
    			 	$1_11('[name=acrfiltro]').val('');	
    			 	$1_11('[name=acrordem]').val('');	
    
    			 	buscarPerfis();
				}
				
				buscarListaAcessoRapido();				
			},
			error		: function ()
			{
				alert('Erro ao excluir o item.\nTente novamente!');
			    location.href = location.href; 				
			}
		});		
	}
}

$1_11("[name*=sisid]").chosen({no_results_text: "Não foram encontrados resultados:", placeholder_text_multiple:'-- Selecione os sistema --'});
$1_11('body').css({'overflow-y':'scroll'});
</script>