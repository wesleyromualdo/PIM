<?php
include_once APPRAIZ .'par3/classes/controller/ModeloDocumento.class.inc';
include_once APPRAIZ .'par3/classes/model/ModeloDocumento.class.inc';

$obDocumento = new Par3_Controller_Modelo_Documento();
//$obPrograma = new Par3_Model_IniciativaProgramas();

switch ($_REQUEST['requisicao']) {
    case 'salvar_modelo':
        $obDocumento->salvar_modelo($_REQUEST);
        die;
    break;
}

if ($_REQUEST['mdoid']) {
    $arrDados = $obDocumento->carregaModeloDocumento($_REQUEST['mdoid']);
}

$sqlTipo = "SELECT
			td.tpdcod as codigo,
			td.tpddsc as descricao
		FROM public.tipodocumento td
		WHERE td.tpdstatus = 'A'
			AND tpdcod IN (21, 102, 99)
		ORDER BY descricao";

$sqlTipoAditivo = "select
				mdoid as codigo,
			    mdoid||' - '||mdonome as descricao
			from
				par3.modelodocumento mdo
			inner join public.tipodocumento tpd ON tpd.tpdcod = mdo.tpdcod
			where
				mdovisivel = 'S'
			    and mdostatus = 'A'
			    AND mdo.tpdcod = 102
			order by mdoid";

$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=gerarmodelodocumento';

?>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
    
    tinyMCE.init({
        // General options
        mode : "textareas",
        theme : "advanced",
        language: "pt",
        plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

        // Theme options
        theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
        theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
        theme_advanced_buttons3 : "tablecontrols, pagebreak", //",|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
        //theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template",
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
<div class="ibox">
<div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Modelo de Documento</h2>
            </div>
        </div>
    </div>
    <?php echo $simec->tab(criaAbaModeloDocumento(), $url);?>
    <div class="ibox-content">
        <div class="row">
            <form name="formDocumento" id="formDocumento" class="form form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value="">
                <input type="hidden" name="mdoid" id="mdoid" value="<?php echo $arrDados['mdoid']; ?>">
                <?php echo $simec->input('mdonome', 'Modelo', $arrDados['mdonome'], array(), array('maxlength' => '255', 'input-size' => '7')); ?>
                <?php echo $simec->select('tpdcod', 'Tipo de Documento', $arrDados['tpdcod'], $sqlTipo, array('maxlength' => '255'), array('input-size' => '7')); ?>
                <div class="form-group ">
                    <label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">Validação Gestor: </label>
                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                        <div id="mdovalidacaogestor" style="text-align: left;">
                            <div class="radio radio-default radio-inline ">
                                <input type='radio' checked name='mdovalidacaogestor' id='mdovalidacaogestor1' value='S'" <?php echo $arrDados['mdovalidacaogestor'] == "S" ? " checked" : ""; ?>>
                                <label for="mdovalidacaogestor1">Sim</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="mdovalidacaogestor" id="mdovalidacaogestor2" value="N" <?php echo (($arrDados['mdovalidacaogestor'] == "N" || empty($arrDados['mdovalidacaogestor'])) ? " checked" : ""); ?>>
                                <label for="mdovalidacaogestor2">Não</label>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-group ">
                    <label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">Validação FNDE: </label>
                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                        <div id="mdovalidacaofnde" style="text-align: left;">
                            <div class="radio radio-default radio-inline ">
                                <input type='radio' checked name='mdovalidacaofnde' id='mdovalidacaofnde1' value='S'" <?php echo $arrDados['mdovalidacaofnde'] == "S" ? " checked" : ""; ?>>
                                <label for="mdovalidacaofnde1">Sim</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="mdovalidacaofnde" id="mdovalidacaofnde2" value="N" <?php echo (($arrDados['mdovalidacaofnde'] == "N" || empty($arrDados['mdovalidacaofnde'])) ? " checked" : ""); ?>>
                                <label for="mdovalidacaofnde2">Não</label>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-group ">
                    <label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">De Ofício: </label>
                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                        <div id="mdoex_oficio" style="text-align: left;">
                            <div class="radio radio-default radio-inline ">
                                <input type='radio' checked name='mdoex_oficio' id='mdoex_oficio1' value='S'" <?php echo $arrDados['mdoex_oficio'] == "S" ? " checked" : ""; ?>>
                                <label for="mdoex_oficio1">Sim</label>
                            </div>
                            <div class="radio radio-default radio-inline ">
                                <input type="radio" name="mdoex_oficio" id="mdoex_oficio2" value="N" <?php echo (($arrDados['mdoex_oficio'] == "N" || empty($arrDados['mdoex_oficio'])) ? " checked" : ""); ?>>
                                <label for="mdoex_oficio2">Não</label>
                            </div>
                        </div>
                    </div>
                </div>
                <?php
                //mdoesfera, mdotipo
                $arrOptions = array('O' => 'Obras', 'P' => 'PAR');
                ?>
                <?php echo $simec->radio('mdoesfera', 'Esfera', $arrDados['mdoesfera'], array('E' => 'Estadual', 'M' => 'Municipal')); ?>
                <?php echo $simec->radio('mdotipo', 'Tipo de Processo', $arrDados['mdotipo'], $arrOptions, array(), array()); ?>
                <div class="form-group " style="text-align: center !important;">
                    <table style="text-align: center; width: 80%">
                        <tr>
                        <td>
                    <div class="col-sm-12 col-md-12 col-lg-12" style="text-align: center !important;">
                        <div>
                            <textarea id="mdoconteudo" name="mdoconteudo" rows="30" cols="140" ><?php echo $arrDados['mdoconteudo']; ?></textarea>
                            <input type="hidden" name="textohidden" id="textohidden" value="">
                        </div>
                    </div>
                        </td>
                    </tr>
                </table>
                </div>
                            <?php
                            $sqlMacro = "SELECT
								teccampo as codigo,
								tecdescricao as descricao
							FROM
								par3.termocampos
							WHERE
								tecstatus = 'A'
							ORDER BY
								descricao";
                            ?>
                <table border="0" width="80%" align="right">
                    <tr>
                        <td valign="top" style="width: 50%; text-align: right;">
                            <?php echo $simec->select('teccampo', 'Macros', '', $sqlMacro, array(), array()); ?>
                        </td>
                        <td valign="top" style="width: 30%; text-align: left;" >
                            <button type="button" class="btn btn-primary" id="adicionar-macro" onclick="adicionaMacro();"><i class="fa fa-edit"></i> Adicionar Macro</button>
                        </td>
                    </tr>
                </table>
            </form>
        </div>
    </div>
    <div class="ibox-footer">
        <div class="center">
            <button type="button" class="btn btn-primary" id="salvar-modelo"><i class="fa fa-save"></i> Salvar</button>
        </div>
    </div>
     <div class="ibox-content">
        <div id="documentoListagem">
            <?php
                //$obDocumento->listar($_REQUEST);
            ?>
        </div>
    </div>
</div>
<script type="text/javascript">
    var tecCampo = document.getElementsByName("teccampo")[0];

    jQuery(document).ready(function(){
        /*if( jQuery('[name="tpdcod"]').val() == 21 ){
        	jQuery('#div_tpdcod_a').show();
        } else {
    		jQuery('#div_tpdcod_a').hide();
        }

        jQuery('[name="tpdcod"]').change(function(){
            if( jQuery(this).val() == 21 ){
            	jQuery('#div_tpdcod_a').show();
            } else {
            	jQuery('#div_tpdcod_a').hide();
            }
        });*/
    });

    jQuery('#salvar-modelo').click(function(){
        var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): <br> <br>';
        var validacao = true;
        
        if( jQuery('[name="mdonome"]').val() == '' ){
            mensagem += 'Nome do Modelo <br>';
            validacao = false;
        }
        if( jQuery('[name="tpdcod"]').val() == '' ){
            mensagem += 'Tipo do Documento <br>';
            validacao = false;
        }
        if( jQuery('[name="mdoesfera"]:checked').length == 0 ){
            mensagem += 'Esfera <br>';
            validacao = false;
        }
        if( jQuery('[name="mdotipo"]:checked').length == 0 ){
            mensagem += 'Tipo de Processo <br>';
            validacao = false;
        }
        
        var mdoconteudo = tinyMCE.get('mdoconteudo').getContent( {format : 'text'} );
        if (mdoconteudo == ''){
            mensagem += 'Termo Modelo <br>';
            validacao = false;
        }
        
        if( !validacao ){
            alert(mensagem);
            return false;
        }else{
            jQuery('[name="requisicao"]').val('salvar_modelo');
            jQuery('[name="formDocumento"]').submit();
        }
    });
    

    function adicionaMacro() {
        var macro = tecCampo.options[tecCampo.selectedIndex].value;
        tinyMCE.execCommand('mceInsertContent', false, macro);
    }
    
</script>
