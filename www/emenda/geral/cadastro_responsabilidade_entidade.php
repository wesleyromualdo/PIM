<?php
include "config.inc";
header('content-type: text/html; charset=utf-8');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/

if(is_array($_POST['entSelecionadas'])) {
	$sql = "update
			 emenda.usuarioresponsabilidade 
			set
			 rpustatus = 'I' 
			where
			 usucpf = '$usucpf'  
			 and pflcod = $pflcod ";
	$db->executar($sql);
	
	if($_POST['entSelecionadas'][0]){
		foreach($_POST['entSelecionadas'] as $enbid){
			$sql = "insert into emenda.usuarioresponsabilidade (pflcod, usucpf,  rpustatus, rpudata_inc, enbid)
						   								values ($pflcod, '$usucpf', 'A', now(), '$enbid')";
			$db->executar($sql);
		}		
	}
	$db->commit();
?>
	<script>
		window.parent.opener.location.reload();
		self.close();
	</script>
<?
	exit();
}

/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Instituição</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<form name="formulario">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione a(s) Entidade(s)</strong></td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td><b>CNPJ</b></td>
	<td><b>Entidade</b></td>
	<td><b>Ano</b></td>
</tr>
<?
	  $cabecalho = 'Selecione a(s) Entidade(s)';
	  $sql = "SELECT distinct
					eb.enbid,
					eb.enbcnpj,
					eb.enbnome,
					eb.enbano 
				FROM 
				  	emenda.entidadebeneficiada eb				  
				WHERE
					eb.enbstatus = 'A'
				order by eb.enbcnpj, eb.enbano";
	  
	  $RS = @$db->carregar($sql);
	  
	  if($RS){
		  $nlinhas = count($RS)-1;
		  for ($i=0; $i<=$nlinhas;$i++){
				extract($RS[$i]);
				if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
		   ?>
		   		
			   		<tr bgcolor="<?=$cor?>">
					<td align="right"><input type="checkbox" name="usgid" id="<?=$enbid?>" value="<?=$enbid?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="enbnome" value="<?=$enbcnpj.' - '.$enbnome?>"></td>
					<td align="right" style="color:blue;"><?=$enbcnpj?></td>
					<td><?=$enbnome?></td>
					<td><?=$enbano?></td>
					</tr>
		   
		   <?
		  }
	  }else{
	  	?>
	  		<tr>
				<td align="center" style="color: rgb(204, 0, 0);">Não foram encontrados Registros.</td>
			</tr>
	  	<?php
	  }
//xd(789);
?>
</table>
</form>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="entSelecionadas[]" id="entSelecionadas" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "SELECT DISTINCT 
		  e.enbid, 
		  e.enbcnpj as codigo, 
		  e.enbnome as descricao, 
		  e.enbano
		FROM 
		  emenda.usuarioresponsabilidade ur	INNER JOIN emenda.entidadebeneficiada e 
		  ON (ur.enbid = e.enbid)
		WHERE 
		  ur.rpustatus='A' 
		  AND ur.usucpf = '$usucpf' 
		  AND ur.pflcod = $pflcod";

$RS = @$db->carregar($sql);
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$enbid\">$codigo - $descricao - $enbano</option>";		
		}
	}
} else {?>
<option value="">Clique na Entidade.</option>
<?
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

var campoSelect = document.getElementById("entSelecionadas");

if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		document.getElementById(campoSelect.options[i].value).checked = true;
	}
}

function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
}

function retorna(objeto)
{
	//alert(objeto);
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {
		tamanho--;
	}
	//alert(tamanho);
	
	var arUsgid = document.getElementsByName( 'usgid' );
	var arEnbnome = document.getElementsByName( 'enbnome' );
	
	
	
	if (arUsgid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(arEnbnome[objeto].value, arUsgid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (arUsgid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Entidade.', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {

	/*if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();
	}*/
}
</script>