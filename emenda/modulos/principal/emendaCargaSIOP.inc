<?php
include  APPRAIZ."includes/cabecalho.inc";
$db->cria_aba( $abacod_tela, $url, '' );
?> 
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value=""/>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Código:</td>
			<td>
				<?php 
					$emenda = $_REQUEST["emenda"];
					echo campo_texto( 'emenda', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
			<?php
					$cnpj = $_REQUEST['cnpj']; 
					echo campo_texto( 'cnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Momento:</td>
			<td>
			<?php
					$momento = $_REQUEST["momento"];
					echo campo_texto( 'momento', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Natureza:</td>
			<td>
			<?php
					$natureza = $_REQUEST["natureza"];
					echo campo_texto( 'natureza', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
		</tr>
		<!--  <tr>
			<td class="subtitulodireita">Ano:</td>
			<td>
			<?php
					$ano = $_REQUEST["ano"];
					echo campo_texto( 'ano', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
		</tr>-->
		<tr bgcolor="#D0D0D0">
			<td colspan="4" align="center">
				<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisaEmendaOrigem();" style="cursor: pointer;"/>
				<input type="button" value="Todos" id="btnTodos" name="btnTodos" onclick="window.location.href = window.location;" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> 
	<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
	
	<script type="text/javascript">
		function pesquisaEmendaOrigem(){
			$('#requisicao').val('pesquisa');
			$('#formulario').submit();
		}
		
		function alterarTiposEmenda(emecod, tipo){
			$.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/emendaTipoOrigem&acao=A",
			data: "requisicao=alterarTiposEmenda&emecod="+emecod+"&tipo="+tipo,
			async: false,
			success: function(msg){
					$('.labOrigem').css('color', '');
					$('.img_span').html('');
					$('#etoid_'+emecod+'_'+tipo).css('color', 'blue');
					$('#etoid_'+emecod+'_'+tipo).css('font-weight', 'bold');
					$('#span_'+emecod+'_'+tipo).html('<img src="../imagens/check.jpg" border="0">');
				}
			});
		}
	</script>
	<style>
		.divLabel{  
			width: 180px;
		}
	</style>
</form>
<?
$filtro = '';
if( $_POST['requisicao'] == 'pesquisa' ){
	$enbcnpj = str_replace(".", "", $_POST['cnpj']);
	$enbcnpj = str_replace("/", "", $enbcnpj);
	$enbcnpj = str_replace("-", "", $enbcnpj);
	
	if( $_POST['emenda'] ) $filtro .= " and emenda = '".$_POST['emenda']."'";
	if( $_POST['cnpj'] ) $filtro .= " and cnpj = '".$enbcnpj."'";
	if( $_POST['momento'] ) $filtro .= " and momento = '".$_POST['momento']."'";
	if( $_POST['ano'] ) $filtro .= " and ano = ".$_POST['ano'];
	if( $_POST['natureza'] ) $filtro .= " and natureza_despesa = '".$_POST['natureza']."'";

$sql = "SELECT esfera||'&nbsp;' as esfera, uo||'&nbsp;' as uo, funcao||'&nbsp;' as funcao, subfuncao||'&nbsp;' as subfuncao, programa||'&nbsp;' as programa, acao||'&nbsp;' as acao, localizador||'&nbsp;' as localizador, 
		       natureza_despesa||'&nbsp;' as narureza_despesa, rp_atual||'&nbsp;' as rp_atual, fonte||'&nbsp;' as fonte, iduso||'&nbsp;' as iduso, idoc||'&nbsp;' as idoc, emenda||'&nbsp;' as emenda, nome_parlamentar, 
		       partido, uf, valor_emenda, cnpj||'&nbsp;' as cnpj, nome_beneficiario, valor_aprovado, 
		       valor_rcl_apurada, valor_disponivel
		  FROM emenda.carga_emenda_siop
		 WHERE 1=1
			$filtro ";

$tamanho   = array( '390px', '5%', '', '', '', '', '', '', '', '10%');
		    
$cabecalho = array('esfera', 'uo', 'funcao', 'subfuncao', 'programa', 'acao', 'localizador', 
		       'natureza_despesa', 'rp_atual', 'fonte', 'iduso', 'idoc', 'emenda', 'nome_parlamentar', 
		       'partido', 'uf', 'valor_emenda', 'cnpj', 'nome_beneficiario', 'valor_aprovado', 
		       'valor_rcl_apurada', 'valor_disponivel' );

//monta_lista_simples($sql, $cabecalho, $perpage, $pages, $soma = 'N', $largura = '95%', $valormonetario = 'S', $totalregistro = false, $arrHeighTds = false, $heightTBody = false, $boImprimiTotal = false)

$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'S', '', 'S', true, false, false, true);
}
?>