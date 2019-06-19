<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'emenda.php?modulo=principal/popupAnalideTecnica&acao=A&anaid=".$_GET['anaid']."';</script>";
    exit;
}

if( $_FILES["arquivo"] && !$_POST['arqid']){
	
	if( $_FILES["arquivo"]["name"] ){
		$campos	= array("ptrid"  => $_SESSION['emenda']['ptridAnalise'],
						"anxdsc" => "'Anexo Análise Técnica'",
						"anaid" => "'{$_POST['anaid']}'"
					);
					
		$file = new FilesSimec("anexo", $campos ,"emenda");
		$arquivoSalvo = $file->setUpload();
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
			echo"<script>window.location.href = 'emenda.php?modulo=principal/popupAnalideTecnica&acao=A&anaid=".$_GET['anaid']."';</script>";
			die;	
		}
	}
}
 
if($_GET['arqidDel']){
    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="emenda.php?modulo=principal/popupAnalideTecnica&acao=A&anaid='.$_GET['anaid'].'";
    	  </script>';
    die;
}
switch ( $_REQUEST["requisicao"] ){
	case "salvaranalise":
		if( salvaAnaliseTecnica( $_REQUEST ) ){
			echo "<script>alert('Operação realizada com sucesso!');window.opener.location.reload();window.close();</script>";
			exit();
		} else {
			echo "<script>alert('Falha na operação!');</script>";
		}
	break;
	case "inserirparecer":
		if( inserirParecerAnalise( $_REQUEST ) ){
			echo "<script>alert('Operação realizada com sucesso!');
			window.opener.location.reload();window.close();</script>";
			exit();
		} else {
			echo "<script>alert('Falha na operação!');</script>";
		}
	break;
}


if ( $_REQUEST["anaid"] ){
	
	$sql = "SELECT 
				ana.anaid,			
				ana.anasituacaoparecer, 
				ana.anaparecer,
				ana.anaposterior,
				anx.anxdsc,
				arq.arqid,
				arq.arqnome || '.' || arq.arqextensao as arquivo,
				ana.anadisponibilizardiligencia
			FROM 
				emenda.analise ana
			LEFT JOIN
				emenda.anexo anx ON anx.anaid = ana.anaid
			LEFT JOIN
				public.arquivo arq on anx.arqid = arq.arqid
			WHERE 
				ana.anaid = {$_REQUEST["anaid"]}";
	$dadosAnalise = $db->pegaLinha( $sql );

}
if($dadosAnalise['anasituacaoparecer'] == 'E'){
	$style = "style='display: '";
} else {
	$style = "style='display: none'";
}
$ptrid = $_SESSION["emenda"]["ptridAnalise"];
$anaid = $_REQUEST["anaid"];

?>

<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
	// Salva a análise
	// Salva a análise
	function salvarAnaliseTecnica(){
		
		var form = document.getElementById('formulario');
		var radio = "";
		var marcadoRadio = false;		
		
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == "radio"){
				if(form.elements[i].checked == true ){
					radio = form.elements[i].value;
					marcadoRadio = true
				}
			}
		}

		if (document.getElementById('anaposterior').value == 'P') marcadoRadio = 1;
		
		if(!marcadoRadio){
			alert('Selecione uma análise!');
			return false;
		} else if( tinyMCE.get('anaparecer').getContent() == ''){
			alert('O campo parecer é de preenchimento obrigatório!');
			tinyMCE.get('anaparecer').focus();
			return false;
		} else if(document.getElementById('anxdsc').value != '' && document.getElementById('arquivo') && document.getElementById('arquivo').value == ''){
			alert('Selecione o anexo!');
			document.getElementById('arquivo').focus();
			return false;
		} else {
			form.submit();
		}
	}
	
	function salvarUnidadeParecer(){
		var form = document.getElementById('formulario');
		var radio = "";
		var marcadoRadio = false;		
		
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == "checkbox"){
				if(form.elements[i].checked == true ){
					radio = form.elements[i].value;
					marcadoRadio = true
				}
			}
		}
		if(!marcadoRadio){
			alert('Selecione uma unidade!');
			return false;
		} else {
			document.getElementById('requisicao').value = 'inserirparecer';
			form.submit();
		}
	}

	function excluirAnexoAnaid( anaid, arqid ){
	 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
	 		location.href= window.location+'&arqidDel='+arqid+'&anaid='+anaid;
	 	}
	}
	
	function verificaSituacao( situacao ){
	 	if( situacao == 'E' ){
	 		$('trdisponibilizar').style.display = '';
	 	} else {
	 		$('trdisponibilizar').style.display = 'none';
	 	}
	}
	 
	 
	
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});
</script>

<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="salvaranalise">
	<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid ?>">
	<input type="hidden" name="anaid" id="anaid" value="<?=$anaid ?>">
	<input type="hidden" name="concluir" id="concluir" value="false">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2" class="subtitulocentro">
				Análise Técnica
			</td>
		</tr>
		<?php 
		if( $_REQUEST['acaoTipo'] != 'button' ){
		$sql = "SELECT exfid  
				FROM emenda.execucaofinanceira
				WHERE ptrid = {$_SESSION["emenda"]["ptridAnalise"]}
				and exfstatus = 'A' 
				and exfnumempenhooriginal is null";
		$podeposterior = $db->pegaUm( $sql );


		if($podeposterior["exfid"]){  ?>
			<tr>
				<td class="subtitulodireita" width="25%"><b>Análise Posterior</b></td>
				<td>
					<input type="hidden" name="anaposterior" id="anaposterior" value="<?php if (!$dadosAnalise["anaposterior"]) $dadosAnalise["anaposterior"] = "M"; echo $dadosAnalise["anaposterior"]; ?>"></input>
					<label for="anaposterior"><input type="checkbox" onclick="if (this.checked == true) { document.getElementById('anaposterior').value = 'P'; document.getElementById('anasituacao1').checked = false; document.getElementById('anasituacao2').checked = false; document.getElementById('anasituacao3').checked = false; document.getElementById('anasituacao4').checked = false; document.getElementById('tranatecnica').style.display = 'none'; document.getElementById('titulo_parecer').innerHTML = 'Justificativa'; } else { document.getElementById('anaposterior').value = 'M'; document.getElementById('tranatecnica').style.display = ''; document.getElementById('titulo_parecer').innerHTML = 'Parecer'; }" <?php if($dadosAnalise["anaposterior"] == 'P'){ echo "checked"; } ?>>Análise Posterior</label>
				</td>
			</tr>
		<?php }else{?>
			<input type="hidden" name="anaposterior" id="anaposterior" value="M">
		<?php }?>
				
		<tr id="tranatecnica">
			<td class="subtitulodireita"><b>Análise</b></td>
			<td>
				<label for="anasituacao1"><input type="radio" name="anasituacao" onclick="verificaSituacao(this.value);" id="anasituacao1" value="F" <?php if($dadosAnalise["anasituacaoparecer"] == "F"){ echo "checked"; } ?>>Aprovado</label>
				<label for="anasituacao2"><input type="radio" name="anasituacao" onclick="verificaSituacao(this.value);" id="anasituacao2" value="E" <?php if($dadosAnalise["anasituacaoparecer"] == "E"){ echo "checked"; } ?>>Em diligência</label>
				<label for="anasituacao3"><input type="radio" name="anasituacao" onclick="verificaSituacao(this.value);" id="anasituacao3" value="D" <?php if($dadosAnalise["anasituacaoparecer"] == "D"){ echo "checked"; } ?>>Indeferido</label>
		<?php if( $_SESSION['exercicio'] == 2010 ){?>
				<label for="anasituacao4"><input type="radio" name="anasituacao" onclick="verificaSituacao(this.value);" id="anasituacao4" value="C" <?php if($dadosAnalise["anasituacaoparecer"] == "C"){ echo "checked"; } ?>>Empenho Condicionado</label>
		<?php }?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr id="trdisponibilizar" <?=$style; ?>>
			<td class="subtitulodireita"><b>Disponibilizar essa diligência para convenente?</b></td>
			<td>
				<label for="anadisponibilizardiligencia1"><input type="radio" name="anadisponibilizardiligencia" id="anadisponibilizardiligencia1" value="S" <?php if($dadosAnalise["anadisponibilizardiligencia"] == "S"){ echo "checked"; } ?>>Sim</label>
				<label for="anadisponibilizardiligencia2"><input type="radio" name="anadisponibilizardiligencia" id="anadisponibilizardiligencia2" value="N" <?php if($dadosAnalise["anadisponibilizardiligencia"] == "N"){ echo "checked"; } ?>>Não</label>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<script><?php if($dadosAnalise["anaposterior"] == 'P'){ echo "document.getElementById('tranatecnica').style.display = 'none';"; }?></script>
		<tr>
			<td class="subtitulodireita"><b id="titulo_parecer">Parecer</b></td>
			<td>
				<?php $anaparecer = $dadosAnalise["anaparecer"]; ?>
			 	<?=campo_textarea('anaparecer', 'S', 'S', 'Parecer', 120, 50, 4000, '', '', '', '', 'parecer');?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Anexo</b></td>
			<td>
				<?php if($dadosAnalise["arqid"]){?>
					<div id="div_anexo" >
						<a href="javascript:window.location='?modulo=principal/popupAnalideTecnica&acao=A&download=S&arqid=<?=$dadosAnalise["arqid"]?>'" ><?=$dadosAnalise["arquivo"]?></a>
						<img style="cursor: pointer;vertical-align: middle" onclick="excluirAnexoAnaid('<?=$dadosAnalise["anaid"]?>','<?=$dadosAnalise["arqid"]?>');" title="Excluir Anexo" src="/imagens/excluir.gif"/>
					</div>
				<?php }else{?>
					<input type="file" name="arquivo" id="arquivo" />
				<?php }?> 
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Descrição do Anexo</b></td>
			<td><?php $anxdsc = $dadosAnalise["anxdsc"] ?>
				<?=campo_texto( 'anxdsc', 'N', $habilitado, 'Descrição Anexo', 40, 40, '', '','','','','id="anxdsc"'); ?>
			</td>
		</tr>
		<?} else { ?>
			<tr>
				<td class="subtitulodireita" width="190px"  height="30px">Unidades que realizarão a Análise Técnica do PTA</td>
				<td>
					<?php 
						if( !$boReformulacao )
							$filtro = 'and uniid != 11';
							
						$sql = "select 
									uniid,
									uninome,
									unisigla
								from
									emenda.unidades
								where
									unistatus = 'A'
									$filtro
								order by
									uninome";
						$arrUnidades = $db->carregar($sql);
						$arrUnidades = !$arrUnidades ? array() : $arrUnidades; 
						
						$sql = "select 
									uniid
								from 
									emenda.analise 
								where 
									ptrid = $ptrid 
								and 
									anatipo = 'T'
									and anastatus = 'A'
									and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')";
									
						$arUnidades = $db->carregar($sql);
						$arUnidades = !$arUnidades ? array() : $arUnidades;
						foreach($arUnidades as $unidade){
							$arUnidade[] = $unidade['uniid'];
						}
						$arUnidade = !$arUnidade ? array() : $arUnidade;
						
						?>
						<table>
							<tr>
						<?foreach($arrUnidades as $key => $arrUnidade){ 
							if($key == 3 || $key == $numDiv){
								$numDiv = $numDiv + 3;
								echo "</tr><tr>";
							}
							if( in_array($arrUnidade['uniid'],$arUnidade) ){
								echo '<input type="hidden" name="check[]" id="check" value="'.$arrUnidade['uniid'].'">';
							}
							?>
							<td><input <? echo in_array($arrUnidade['uniid'],$arUnidade) ? "checked='checked'" : "" ?> type="checkbox" value="<?=$arrUnidade['uniid'] ?>" name="ckb_unidade[]" title="<?=$arrUnidade['uninome'] ?>" id="unidade_<?=$arrUnidade['uniid'] ?>" /><label style="margin-right:10px" for="unidade_<?=$arrUnidade['uniid'] ?>" title="<?=$arrUnidade['uninome'] ?>" ><?=$arrUnidade['unisigla'] ?></label></td> 
						<? $cont++; }?>
							</tr>
						</table>
					
				</td>
			</tr>
		<?} ?>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<?if( $_REQUEST['acaoTipo'] != 'button' ){ ?>
					<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" style="cursor: pointer;" <?php if ($habilitaAnalise){ echo 'disabled="disabled"'; }else { echo 'onclick="salvarAnaliseTecnica();"'; } ?> >
				<?} else { ?>
					<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" style="cursor: pointer;" <?php if ($habilitaAnalise){ echo 'disabled="disabled"'; }else { echo 'onclick="salvarUnidadeParecer();"'; } ?> >
				<?} ?>
				<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
			</td>
		</tr>
	</table>