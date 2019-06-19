<?php 

if($_SESSION['proinfantil']['mds']['muncod']) unset($_SESSION['proinfantil']['mds']['muncod']);

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
echo "<br/>";

$abacod_tela = 57559;
$arMnuid = array(11931, 11932);

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$titulo_modulo = "Lista de Municípios";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

if($_POST){
	extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	$('.abrirMunicipio').click(function(){
		document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaCreche&acao=A&muncod='+this.id;
	});
	$('#btnPesquisar').click(function(){	
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == ''){
			alert('Preencha pelo menos um filtro!');
			return false;
		}	
		$('#formulario').submit();
	});
	$('#btnLimpar').click(function(){
		document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A';
	});
});
</script>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">UF</td>
			<td>
				<?php 
				$sql = "select 
							estuf as codigo,
							estdescricao as descricao
						from
							territorios.estado
						order by 
							estuf";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td>
				<?php echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>
<?php 

if($estuf){
	$arWhere[] = "mun.estuf = '{$estuf}'";
}

if($mundescricao){
	$arWhere[] = "mun.mundescricao ilike '%{$mundescricao}%'";
}

$sql = "select
			'
			<img src=\"../imagens/alterar.gif\" id=\"' || mun.muncod ||'\" class=\"abrirMunicipio\" style=\"cursor:pointer;\"/>
			' as acao,
			mun.estuf,
			mun.mundescricao 
		from 
			territorios.municipio mun
		inner join
			proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = mun.muncod
		inner join
			proinfantil.procenso pc on pc.muncod = dcm.muncod
		".(is_array($arWhere) ? ' where '.implode(' and ', $arWhere) : '')."
		group by mun.muncod, mun.estuf, mun.mundescricao
		order by 
			mun.estuf, mun.mundescricao
			";

$cabecalho = array('Ação', 'UF', 'Município');
$db->monta_lista($sql, $cabecalho, 50, 10, '', '', '', '', array('60', '40'));

?>