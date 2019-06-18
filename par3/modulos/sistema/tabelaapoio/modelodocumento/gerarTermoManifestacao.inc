<?php
include_once APPRAIZ .'par3/classes/controller/ModeloDocumento.class.inc';
include_once APPRAIZ .'par3/classes/model/ModeloDocumento.class.inc';

$obDocumento = new Par3_Controller_DocumentoTermo();

switch ($_REQUEST['requisicao']) {
    case 'salvar-termo':
        
        $db->executar("UPDATE par3.documentotermomanifesto SET domstatus = 'I' WHERE estuf = '{$_REQUEST['estuf']}'");
        
        $sql = "INSERT INTO par3.documentotermomanifesto(mdoid, estuf, domstatus, domdatainclusao, usucpfinclusao)
                VALUES(16, '{$_REQUEST['estuf']}', 'A', now(), '{$_SESSION['usucpf']}') returning domid";
        $domid = $db->pegaUm($sql);
        
        $_REQUEST['mdoconteudo_t'] = str_replace("\'", '"', $_REQUEST['mdoconteudo_t']);
        $_REQUEST['mdoconteudo_t'] = str_replace('\"', '"', $_REQUEST['mdoconteudo_t']);
        
        $doptexto = $_REQUEST['mdoconteudo_t'];
        
        if( $doptexto ){
            if( strpos($doptexto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
                $doptexto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
            } else {
                $doptexto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
            }
        }
        $doptexto = $doptexto ? simec_htmlspecialchars($doptexto) : 'null';
        
        $nomeArquivo 		= 'Minuta_Documento_Manifestacao_'.$domid.'_'.date('YmdHis');
        $diretorio		 	= APPRAIZ . 'arquivos/par3/documentoTermo';
        $diretorioArquivo 	= APPRAIZ . 'arquivos/par3/documentoTermo/'.$nomeArquivo.'.txt';
        
        if( !is_dir($diretorio) ){
            mkdir($diretorio, 0777);
        }
        
        $fp = fopen($diretorioArquivo, "w");
        if ($fp) {
            stream_set_write_buffer($fp, 0);
            fwrite($fp, $doptexto);
            fclose($fp);
        }
        
        $db->executar("update par3.documentotermoarquivo set dtastatus = 'I' where domid IN (SELECT domid FROM par3.documentotermomanifesto WHERE estuf = '{$_REQUEST['estuf']}')");
        
        $sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
				VALUES( '".$nomeArquivo."',
						'txt',
						'".$nomeArquivo."',
						'text/plain',
						'".filesize($diretorioArquivo)."',
						'".date('Y-m-d')."',
						'".date('H:i:s')."',
						'".$_SESSION["usucpf"]."',
						".($_SESSION['sisid'] ? $_SESSION['sisid'] : 'null').",
						'A') RETURNING arqid";
        $arqid = $db->pegaUm($sql);
        
        $sql = "insert into par3.documentotermoarquivo(domid, arqid, dtanomearquivo, dtastatus)
				values($domid, $arqid, '".$nomeArquivo.'.txt'."', 'A')";
        $db->executar($sql);
        $db->commit();
        
        $url = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=gerartermomanifestacao";
        
        if( (int)$domid > 0 ){
            simec_redirecionar($url, 'success', 'Documento Gerado com Sucesso');
        } else {
            simec_redirecionar($url, 'error', 'Erro ao Gerar o Documento');
        }
        exit();
        break;
}

if( $_REQUEST['requisicao'] == 'carregar-termo' ){
    $mdoconteudo = $obDocumento->carregarTermoManifestacao($_REQUEST);
    $mdoconteudo = str_replace('"', "'", $mdoconteudo);
}

$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();

?>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<form name="form-gerar-termo" id="form-gerar-termo" class="form-horizontal" method="post">
<div class="ibox">
	<div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Gerar Termos</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    <div class="row">
    	<div class="col-sm-12 col-md-9 col-lg-6">
    <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'),array('input-size' => '6', 'label-size' => '5')); ?>
    	</div>
    </div>
<?php if( !empty($_REQUEST['estuf']) ){
    
    $sql = "SELECT domid FROM par3.documentotermomanifesto WHERE estuf = '{$_REQUEST['estuf']}' and domstatus = 'A';";
    $domid = $db->pegaUm($sql);
    
    if( $_REQUEST['requisicao'] != 'carregar-termo' ){
        if( $domid ){
            $dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par3.documentotermoarquivo d where d.domid = {$domid} and dtastatus = 'A' order by d.dtaid desc");
            
            if( $dtanomearquivo ){
                $diretorio = APPRAIZ . 'arquivos/par3/documentoTermo/'.$dtanomearquivo;
                if( is_file($diretorio) ){
                    $mdoconteudo = file_get_contents($diretorio);
                }
            }
           // ver($dtanomearquivo, $mdoconteudo,d);
        }
    }
    ?>
    </div>
	<div class="ibox-content">
		<div class="row">
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input type="hidden" name="domid" id="domid" value="<?php echo $domid; ?>">
			<input type="hidden" name="mdoconteudo_t" id="mdoconteudo_t" value="<?=simec_htmlentities($mdoconteudo); ?>" />
				<div class="form-group " style="text-align: right !important;">
					<label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">&nbsp; </label>
		            <div class="col-sm-9 col-md-9 col-lg-9" style="text-align: left !important;">
		            	<div>
                        	<button type="button" id="btn-carregar-termo" data-dismiss="modal" class="btn btn-primary btn-carregar-termo" data-loading-text="Carregando, aguarde ..."><i class="fa fa-search"></i> Carregar Termo</button>
						</div>
					</div>
				</div>
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
		</div>
	</div>
	<div class="ibox-footer">
		<div class="row">
			<div class="center">
        		<button type="button" id="btn-salvar-termo" data-dismiss="modal" class="btn btn-primary btn-salvar-termo" data-loading-text="Salvando aguarde ..."><i class="fa fa-save"></i> Salvar Termo</button>
			</div>
		</div>
	</div>
	 <div class="ibox-content">
        <div id="documentoListagem">
        </div>
    </div>
</div>
<?php }?>
</form>
<script>

jQuery(document).ready(function(){
    
	jQuery('#mdoconteudo').val($('#mdoconteudo_t').val());
});

jQuery('select[name=estuf]').change(function () {
    jQuery('[name="form-gerar-termo"]').submit();
});

jQuery('#btn-carregar-termo').click(function(){
	$btn = jQuery(this).button('loading');
	jQuery('[name="requisicao"]').val('carregar-termo');
	jQuery('[name="form-gerar-termo"]').submit();
});


jQuery('#btn-salvar-termo').click(function(){
	var texto = tinyMCE.get('mdoconteudo').getContent();

	jQuery('#mdoconteudo_t').val(texto);

	if( texto != '' ){
		$btn = jQuery(this).button('loading');
		jQuery('[name="requisicao"]').val('salvar-termo');
		jQuery('[name="form-gerar-termo"]').submit();
	} else {
		alert('É necessário carregar o documento antes de salvar.');
		jQuery(this).button('reset')
		return false;
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