<?php

if($_REQUEST['download'] == 'S'){
    /*  $file = new FilesSimec();
     $arqid = $_REQUEST['arqid'];
     $arquivo = $file->getDownloadArquivo($arqid);  */
    ob_clean();
    $dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par3.documentotermoarquivo d where d.dtaid = ".$_REQUEST['dtaid']);
    
    $diretorio = APPRAIZ . 'arquivos/par3/documentoTermo/'.$dtanomearquivo;
    if( is_file($diretorio) ){
        header( 'Content-type: text/plain' );
        header( 'Content-Disposition: attachment; filename='.$dtanomearquivo);
        readfile( $diretorio );
    } else {
        echo "<script>
				alert('Arquivo não encontrado');
				window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentotermogerado';
		</script>";
    }
    exit;
}

if( $_REQUEST['requisicao'] == 'salvar-termo' ){
    $obDocumento = new Par3_Controller_DocumentoTermo();
    
    $dotid = $_REQUEST['dotid'];
    $sql = "SELECT proid, dtatipo FROM par3.documentotermoarquivo WHERE dotid = {$dotid} AND dtastatus = 'A'";
    $arTermo = $db->pegaLinha($sql);
    
    if( $arTermo['proid'] ){
        $_REQUEST['mdoconteudo_t'] = str_replace("\'", '"', $_REQUEST['mdoconteudo_t']);
        $_REQUEST['mdoconteudo_t'] = str_replace('\"', '"', $_REQUEST['mdoconteudo_t']);
        $obDocumento->gravaHtmlDocumento( $_REQUEST['mdoconteudo_t'], $dotid, $arTermo['proid'], $arTermo['dtatipo'] );
        
        simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentotermogerado', 'success', 'Documento Alterado com Sucesso');
    }
    
}
if( $_REQUEST['requisicao'] == 'carregar-termo' ){
    $obDocumento = new Par3_Controller_DocumentoTermo();
    /*if($_REQUEST['numeroprocesso']) {
        $_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
        $_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
        $_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
        $where .= " and p.pronumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
    }*/
    
    $arrDocumentoTermo = $obDocumento->carregaDocumentoTermo($_REQUEST['dotid']);
    $mdoconteudo = $obDocumento->pegaTermoCompromissoArquivo($_REQUEST['dotid'], true);
}

$sqlTipo = array(
    array()
);
?>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Documentos já Gerados</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-gerar-termo" id="form-gerar-termo" class="form form-horizontal" method="post">
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input type="hidden" name="mdoconteudo_t" id="mdoconteudo_t" value="<?=simec_htmlentities($mdoconteudo); ?>" />
			<?php echo $simec->input('dotid', 'Código do Termo', $_REQUEST['dotid'], array('type' => 'number'),array('input-size' => '2')); ?>
			<?php //echo $simec->input('proid', 'Código do Processo', $_REQUEST['proid'], array('type' => 'number'),array('input-size' => '2')); ?>
			<?php
                /*$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
                $numeroprocesso = $filtro;

                $attribs = array('maxlength' => '255',
                    'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                    'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);");

                echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, array('input-size' => '3'));*/
                ?>
			<?php  $arrOptions = array('O' => 'Obras', 'P' => 'PAR'); ?>
			<?php echo $simec->radio('mdotipo', 'Tipo de Processo', $arrDados['mdotipo'], $arrOptions, array(), array()); ?>
			<div class="form-group " style="text-align: right !important;">
    			<label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">&nbsp; </label>
                <div class="col-sm-9 col-md-9 col-lg-9" style="text-align: left !important;">
                	<div>
                    	<button type="button" id="btn-carregar-termo" data-dismiss="modal" class="btn btn-primary btn-carregar-termo" data-loading-text="Carregando, aguarde ..."><i class="fa fa-search"></i> Carregar Termo</button>
    				</div>
    			</div>
    		</div>
    		<?php
    		if( $_REQUEST['dotid'] ){
    		$sql = "SELECT dtaid, dotid, arqid, dtatipo, proid, '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentotermogerado&download=S&dtaid='||dtaid||'\'\">'||dtanomearquivo||'</a>' as nomearquivo, dtastatus
	    			FROM par3.documentotermoarquivo WHERE dotid = {$_REQUEST['dotid']}";
            
    		$listagemSimec = new Simec_Listagem();
    		$arrayCabecalho = array("dtaid", "dotid", "arqid", "dtatipo", "proid", "dtanomearquivo", "dtastatus");
    		$esconderColunas = array();
    		$listagemSimec->setCabecalho($arrayCabecalho);
    		$listagemSimec->setTitulo('Arquivos');
    		$listagemSimec->esconderColunas($esconderColunas);
    		$listagemSimec->setQuery($sql);
    		$listagemSimec->setTamanhoPagina(50);
    		$listagemSimec->setCampos($arrayCabecalho);
    		$listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    		}
    		?>
			<div class="form-group " style="text-align: center !important;">
				<table style="text-align: center; width: 80%">
					<tr>
					<td>
        	            <div class="col-sm-12 col-md-12 col-lg-12" style="text-align: center !important;">
        	            	<div>
                            	<textarea id="mdoconteudo" name="mdoconteudo" rows="30" cols="140" class="minutatinymce"></textarea>
        					</div>
        				</div>
					</td>
				</tr>
				</table>
			</div>
			<div class="form-group " style="text-align: right !important;">
    			<label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">&nbsp; </label>
                <div class="col-sm-9 col-md-9 col-lg-9" style="text-align: left !important;">
                	<div>
                    	<button type="button" id="btn-salvar-termo" data-dismiss="modal" class="btn btn-primary btn-salvar-termo" data-loading-text="Carregando, aguarde ..."><i class="fa fa-search"></i> Salvar Termo</button>
    				</div>
    			</div>
    		</div>
    	</form>
        </div>
	</div>
</div>
<script type="text/javascript">
$(document).ready(function(){
	//$('#texto').val($('#doptexto').val());
	$('#mdoconteudo').val($('#mdoconteudo_t').val());
});

jQuery('#btn-salvar-termo').click(function(){
	var texto = tinyMCE.get('mdoconteudo').getContent();

	jQuery('#mdoconteudo_t').val(texto);
	
	$btn = jQuery(this).button('loading');
	jQuery('[name="requisicao"]').val('salvar-termo');
	jQuery('[name="form-gerar-termo"]').submit();
});

jQuery('#btn-carregar-termo').click(function(){

	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): <br> <br>';
	var validacao = true;
	
	if( jQuery('#dotid').val() == '' ){
		mensagem += 'Codigo do Termo <br>';
		validacao = false;
	}
	/*if( jQuery('.check_acao:checked').length == 0 ){
		mensagem += 'Selecione uma Iniciativa <br>';
		validacao = false;
	}*/

	if( !validacao ){
		alert(mensagem);
		return false;
	}else{
		$btn = jQuery(this).button('loading');
		jQuery('[name="requisicao"]').val('carregar-termo');
		jQuery('[name="form-gerar-termo"]').submit();
	}
});

tinyMCE.init({
	// General options
	mode : "textareas",
	theme : "advanced",
	language: "pt",
	editor_selector : "minutatinymce",
	plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

	// Theme options
	theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
	theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
	theme_advanced_buttons3 : "tablecontrols, pagebreak", //",|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
	//theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	theme_advanced_statusbar_location : "",
	theme_advanced_resizing : true,

	// Example content CSS (should be your site CSS)
	//content_css : "css/content.css",

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




