<?php

$obDocumento = new Par3_Controller_DocumentoTermo();

if ($_REQUEST['dotid'] && empty($_REQUEST['requisicao'])) {
    $arrDocumentoTermo = $obDocumento->carregaDocumentoTermo($_REQUEST['dotid']);
    $mdoconteudo = $obDocumento->pegaTermoCompromissoArquivo($_REQUEST['dotid'], true);
}

switch ($_REQUEST['requisicao']) {
    case 'salvar-termo':
        $mdoconteudo = $obDocumento->salvar_termo($_REQUEST);
        break;
    case 'carregar-termo':        
        $_REQUEST['bloqueado'] = 'S';
        $dotid = $obDocumento->salvar_termo($_REQUEST);
        $_REQUEST['dotid'] = $dotid;
        $mdoconteudo = $obDocumento->alteraMacroDocumento($_REQUEST);
        unset($_REQUEST['requisicao']);
        $_REQUEST['mdoconteudo_t'] = $mdoconteudo;
        $obDocumento->salvar_termo($_REQUEST);
        $mdoconteudo = str_replace('"', "'", $mdoconteudo);
        
        $url_ = "par3.php?modulo=principal/orcamento/gerarTermo&acao=A&proid=".$_REQUEST['proid']."&dotid=".$_REQUEST['dotid'];
        
        echo "<script>
        	window.location.href = '".$url_."';
		</script>";
        die;
    break;
    case 'visualiza_iniciativa':
        echo $obDocumento->listarIniciativasProcesso($_REQUEST['proid']);
        exit();
    break;
    case 'carregaModelos':
        $sql = "select
    				mod.mdoid,
    				mod.mdonome
				from par3.modelodocumento mod
		     	where mod.mdostatus = 'A' AND mod.mdovisivel <> 'N'
					and mod.mdoesfera = '{$_REQUEST['esfera']}'
					and mod.mdotipo = '{$_REQUEST['tipo_processo']}'
		     		and mod.tpdcod = {$_REQUEST['tpdcod']}
    			order by mod.mdonome asc";
        
        $retorno = $db->carregar($sql);
        
        print simec_json_encode($retorno);
        die;
        break;
}

if (empty($_REQUEST['proid'])) {
    echo "<script>
			alert('Dados Insuficientes. Selecione um processo');
        	window.location.href = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A';
		</script>";
    die;
} else {
    $sql = "SELECT count(rd.refid) FROM par3.reformulacao_documento rd
            	INNER JOIN workflow.documento d ON d.docid = rd.docid
            WHERE proid = {$_REQUEST['proid']} AND refidpai IS NOT NULL AND d.esdid = ".PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO;
    $boReformulacao = $db->pegaUm($sql);
    $obProcesso = new Par3_Controller_Processo();
    $arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($_REQUEST['proid']);
    if (empty($_REQUEST['dotid'])) {
        $obDocumento->verificaTermoAtivo($_REQUEST['proid'], $arrEntidade['tipo_processo']);
    }
}

$_REQUEST['mdoid'] = ($_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $arrDocumentoTermo['mdoid']);
$_REQUEST['tpdcod'] = ($_REQUEST['tpdcod'] ? $_REQUEST['tpdcod'] : $arrDocumentoTermo['tpdcod']);

require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . '/includes/workflow.php';

$db->cria_aba($abacod_tela, $url, '');

//$boEmenda = $db->pegaUm("SELECT count(proid) FROM par3.processo WHERE intaid = 2 and proid = {$_REQUEST['proid']}");

$_REQUEST['inuid'] = $arrEntidade['inuid'];
$helperPendencia = new Par3_Helper_PendenciasEntidade();
$pendenciasImpeditivas = $helperPendencia->controlePendenciasBoxImpeditivas();

/*$obPendencia = new Par3_Controller_Pendencia();
$boHabilitaPorEntidade = $obPendencia->pegaPendenciaInstrumentoUnidade( $_REQUEST['inuid'] );
$boHabilitaPorIniciativa = $obPendencia->pegaValidaPendenciaPorInicitivaProcesso( $_REQUEST['proid'] );*/

$obPendenciaBloqueio = new Par3_Controller_Pendencia();
$temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('termo', $_REQUEST['inuid'], '', $_REQUEST['proid'], false);

# Verifica se o municipio ou o estado não precisa validar as pendecias para finalizar a iniciativa
if ($temPendencia == false /*|| ((int)$boEmenda > (int)0)*/) {
    $pendenciasImpeditivas = 0;
}

$filtroTipo = ' 21, 102 ';
if( $_REQUEST['aditivo'] == 'S'){
    $filtroTipo = ' 21 ';
}
$sqlTipo = "SELECT
			td.tpdcod as codigo,
			td.tpddsc as descricao
		FROM public.tipodocumento td
		WHERE td.tpdstatus = 'A'
			AND tpdcod IN ($filtroTipo)
		ORDER BY descricao";

$mdoconteudo = $mdoconteudo ? $mdoconteudo : "";

?>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
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
            <div class="form-group " style="text-align: right;">
                <label for="mdonome" class="col-sm-3 col-md-3 col-lg-3 control-label">Processo: </label>
                <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                    <?php echo $arrEntidade['processo'];?>
                </div>
                <div style="clear:both"></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group" style="text-align: right;">
                <label for="mdonome" class="col-sm-3 col-md-3 col-lg-3 control-label">Entidade: </label>
                <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                    <?php echo $arrEntidade['entidade'].' / '.$arrEntidade['uf'];?>
                </div>
                <div style="clear:both"></div>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-gerar-termo" id="form-gerar-termo" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value="">
                <input type="hidden" name="cpf_logado" id="cpf_logado" value="<?php echo $_SESSION['usucpf']; ?>">
                <input type="hidden" name="pendencias" id="pendencias" value="<?php echo $pendenciasImpeditivas;?>"/>
                <input type="hidden" name="esfera" id="esfera" value="<?php echo $arrEntidade['esfera'];?>">
                <input type="hidden" name="tipo_processo" id="tipo_processo" value="<?php echo $arrEntidade['tipo_processo'];?>">
                <input type="hidden" name="arr_iniciativa" id="arr_iniciativa" value="<?php echo ( is_array($_REQUEST['check_acao']) ? implode(',', $_REQUEST['check_acao']) : '') ; ?>">
                <input type="hidden" name="proid" id="proid" value="<?php echo $_REQUEST['proid']; ?>">
                <input type="hidden" name="dotvalortermo" id="dotvalortermo" value="<?php echo ($_SESSION['par']['termo']['dotvalortermo'] ? $_SESSION['par']['termo']['dotvalortermo'] : $arrDocumentoTermo['dotvalortermo']); ?>">
                <input type="hidden" name="dotid" id="dotid" value="<?php echo $_REQUEST['dotid']; ?>">
                <input type="hidden" name="mdoconteudo_t" id="mdoconteudo_t" value="<?=simec_htmlentities($mdoconteudo); ?>" />
                <div id="div_mdoconteudo" style="display: none"><?=($mdoconteudo); ?></div>
<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
	<tr>
		<td style="width: 90%">
                <?php echo $simec->select('tpdcod', 'Tipo de Documento', $_REQUEST['tpdcod'], $sqlTipo, array('maxlength' => '255'), array('input-size' => '7'));?>
                <?php 
                    $arrOpcoesTermo = array('maxlength' => '255',  'onchange' => 'carregarTermo(this.value)' );                
                ?>
                <?php echo $simec->select('mdoid', 'Modelo de documento', $_REQUEST['mdoid'], array(), $arrOpcoesTermo, array('input-size' => '7')); ?>
		</td>
		<td style="text-align: center;" valign="center"><?php
		if( $_REQUEST['exoficio'] == 'S' ){
            /*echo wf_desenhaBarraNavegacaoBootstrap(
                    $arrDocumentoTermo['docid'],
                    array('dotid' => $arrDocumentoTermo['dotid'],
                    'proid' => $_REQUEST['proid']),
                    array(),
                    'Fluxo'
            );*/
		}
        ?></td>
	</tr>
</table>
                    <div class="form-group" style="text-align: right !important;">
                        <label class="col-sm-2 col-md-2 col-lg-2 control-label  control-label">Iniciativas: </label>
                        <div class="col-sm-10 col-md-10 col-lg-10" style="text-align: center !important;">
                            <?php echo $obDocumento->listarIniciativasProcesso($_REQUEST['proid'], $arrEntidade['tipo_processo']);?>
                        </div>
                    </div>
                    <div class="form-group " style="text-align: right !important;">
                        <label class="col-sm-2 col-md-2 col-lg-2 control-label  control-label">&nbsp; </label>
                        <div class="col-sm-10 col-md-10 col-lg-10" style="text-align: left !important;">
                            <div>
                                <button type="button" id="btn-carregar-termo" data-dismiss="modal" class="btn btn-primary btn-carregar-termo" data-loading-text="Carregando, aguarde ..."><i class="fa fa-search"></i> Carregar Termo</button>
                            </div>
                        </div>
                    </div>
                    <div style="clear:both; height: 10px;"></div>
                    <div class="form-group " style="text-align: right !important;">
                        <label class="col-sm-2 col-md-2 col-lg-2 control-label  control-label">Modelo: </label>
                        <div class="col-sm-10 col-md-10 col-lg-10" style="text-align: center !important;">
                            <div>
                                <textarea id="mdoconteudo" name="mdoconteudo" rows="30" cols="140" class="minutatinymce"></textarea>
                            </div>
                        </div>
                    </div>
            </form>
        </div>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="center">
<?php       if ($_REQUEST['requisicao'] == 'carregar-termo' || $_REQUEST['dotid']) {?>
                <button type="button" id="btn-salvar-termo" data-dismiss="modal" class="btn btn-primary btn-salvar-termo" data-loading-text="Salvando aguarde ..."><i class="fa fa-save"></i> Salvar Termo</button>
<?php       }?>
            </div>
        </div>
    </div>
     <div class="ibox-content">
        <div id="documentoListagem">
        </div>
    </div>
</div>
<?php
unset($_SESSION['par']['termo']['dotvalortermo']);
?>
<script type="text/javascript">

    function js_travaPendencias( empid ) {
        divCarregando();
        if( jQuery('[name="cpf_logado"]').val() == '' ){
            swal("Pendencias do PAR", "Não é possível Gerar Termo. Para Gerar o Termo, será necessário resolver as Pendências do PAR", "error");
            divCarregado();
        } else {
            swal({
                title: "Pendencias do PAR",
                text: "Não é possível Gerar Termo. Para Gerar o Termo, será necessário resolver as Pendências do PAR",
                html: true,
                type: "error",
                showCancelButton: false,
                confirmButtonText: "OK",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    window.location.href = 'par3.php?modulo=principal/orcamento/listaTermosGerados&acao=A&proid='+jQuery('[name="proid"]').val();
                }
            });
        }
    }

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

    function carregarTermo( valor ){        
        if(valor ){
            $('.loading-dialog-par3').show();
            $btn = jQuery('#btn-carregar-termo').button('loading');
            jQuery('.loading-dialog-par3').show();
            jQuery('[name="requisicao"]').val('carregar-termo');
            jQuery('[name="form-gerar-termo"]').submit();
        }
    }
    
    jQuery('#btn-carregar-termo').click(function(){

        var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): <br> <br>';
        var validacao = true;
        
        if( jQuery('#tpdcod').val() == '' ){
            mensagem += 'Tipo de Documento <br>';
            validacao = false;
        }
        if( jQuery('#mdoid').val() == '' ){
            mensagem += 'Modelo de documento <br>';
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
        	jQuery('.loading-dialog-par3').show();
            $btn = jQuery(this).button('loading');
            jQuery('[name="requisicao"]').val('carregar-termo');
            jQuery('[name="form-gerar-termo"]').submit();
        }
    });

    $(document).ready(function(){
        //renderizeMunicipio();
        
        if( parseInt(jQuery('[name="pendencias"]').val()) > 0 ){
            if( jQuery('[name="cpf_logado"]').val() != '' ){
                jQuery('#btn-carregar-termo').remove();
                jQuery('#btn-salvar-termo').remove();
            }
            js_travaPendencias();
        }

        jQuery('#mdoconteudo_t').val( jQuery('#div_mdoconteudo').html() );
        $('#mdoconteudo').val($('#mdoconteudo_t').val());

        if(jQuery('[name="tpdcod"]').val() != '') {
            carregarModeloDocumento(jQuery('[name="tpdcod"]').val(), <?=$_REQUEST['mdoid']?>);
        }

        jQuery("input:radio[name=itrid], select[name=tpdcod]").change(function() {
            //renderizeMunicipio();
        });

        jQuery('select[name=tpdcod]').change(function(){
            carregarModeloDocumento(this.value, '');
        });

        /*function renderizeMunicipio() {
            var filtroMunicipio = jQuery("select[name=mdoid]").parents("div.form-group");
            if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=tpdcod]').val()) {
                filtroMunicipio.slideUp();
            } else {
                filtroMunicipio.slideDown();
            }
        }*/

        /*var retorno = jQuery('[name="arr_iniciativa"]').val();
        retorno = retorno.split(',');
        
        for(var i=0; i<retorno.length; i++ ){
            jQuery('#check_acao_'+retorno[i]).attr('checked', true);
        }*/
    });
    
    function carregarModeloDocumento(tpdcod, mdoid) {
        if(tpdcod != '') {

            var esfera = jQuery('[name="esfera"]').val();
            var tipo_processo = jQuery('[name="tipo_processo"]').val();
            
            var options = jQuery('#mdoid');
            options.empty();
            options.append(new Option('', ''));
            jQuery.post('', 'requisicao=carregaModelos&tpdcod='+tpdcod+'&esfera='+esfera+'&tipo_processo='+tipo_processo, function(retorno) {
                
                options.append(new Option('', ''));
                if( retorno != 'false' ){
                    $.each(JSON.parse(retorno), function() {
                        options.append(new Option(this.mdonome, this.mdoid));
                    });
                }
                options.focus();
                options.val(mdoid);
                options.trigger('chosen:updated');
            });
        }
    }

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
