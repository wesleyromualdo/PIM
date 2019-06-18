<?php
$inuid = $_REQUEST['inuid'];
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . '/includes/workflow.php';

$obReformulacao = new Par3_Controller_Reformulacao();
if( $_REQUEST['requisicao_ajax'] == 'validaFormulario' ){
    ob_clean();
    $resp = $obReformulacao->validaFormularioDocumentoReformulacao($_REQUEST);
    echo simec_json_encode($resp);
    exit();
}
switch ($_REQUEST['requisicao']) {
    case 'salvarDocumento':
        ob_clean();
        $obReformulacao->salvarDocumentoReformulacao($_REQUEST);
    die;
    break;
    case 'gravaParecerTecnico':
        ob_clean();
        echo $obReformulacao->gravaParecerTecnicoPrazo($_REQUEST);
    die;
    break;
    case 'verificaDataAprovada':
        ob_clean();
        $sql = "SELECT CASE WHEN refdata_solicitada::date = to_char('".$_REQUEST['data']."'::date, 'YYYY-MM-DD')::date THEN 'I' ELSE 
                	CASE WHEN refdata_solicitada::date > to_char('".$_REQUEST['data']."'::date, 'YYYY-MM-DD')::date THEN 'M' ELSE 'T' END
                	END
                FROM par3.reformulacao_documento WHERE refid = ".$_REQUEST['refid']."";
        $str_data = $db->pegaUm($sql);
        if( $str_data == 'M' ){
            echo 'A data informada é MENOR que a data solicitada.';
        }
        if( $str_data == 'T' ){
            echo 'A data informada é MAIOR que a data solicitada.';
        }
    die;
    break;
    case 'salvar-reformulacao':
        ob_clean();
        $refid = $obReformulacao->salvarReformulacaoDocumento($_REQUEST);
        $msg = "Prorrogação salva com sucesso.<br>É necessário clicar em <b>Enviar para análise</b> para que a solicitação seja analisada pelo FNDE.";
        
        simec_redirecionar('par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'], 'success', $msg);
    die;
    break;
    case 'salvar-reformulacao-analise':
        ob_clean();
        $refid = $obReformulacao->salvarReformulacaoPrazoDocumentoAnalise($_REQUEST);
        $msg = "Análise da Prorrogação salva com Sucesso!";
        
        if( $_REQUEST['esdid'] == PAR3_REFORMULACAO_PRAZO_ESDID_EM_AGUARDANDO_ANALISE ){
            require_once APPRAIZ . 'includes/workflow.php';
            
            $docid = $db->pegaUm("SELECT docid FROM par3.reformulacao_documento WHERE refid = {$_REQUEST['refid']}");
            
            wf_alterarEstado( $docid, 5777, 'Fluxo de Reformulação', array('refid' => $_REQUEST['refid'], 'dotid' => $_REQUEST['dotid'], 'proid' => $_REQUEST['proid']) );
        }
        
        simec_redirecionar('par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'], 'success', $msg);
    die;
    break;
    case 'download':
        ob_clean();
        $file = new FilesSimec();
        $arqid = $_REQUEST['arqid'];
        $arquivo = $file->getDownloadArquivo($arqid);
        echo '<script type="text/javascript">
                window.location.href="par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'].'&refid='.$_REQUEST['refid'].'";
                </script>';
        exit;
    die;
    break;
    case 'delarquivo':
        ob_clean();
        $sql = "UPDATE par3.reformulacao_anexo SET axrstatus = 'I' where axrid = ".$_REQUEST['axrid'];
        $db->executar($sql);
        $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = ".$_REQUEST['arqid'];
        $db->executar($sql);
        $db->commit();
        
        $file = new FilesSimec();
        $file->excluiArquivoFisico($_REQUEST['arqid']);
        
        echo '<script type="text/javascript">
        		alert("Operação realizada com sucesso!");
        		window.location.href="par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'].'&refid='.$_REQUEST['refid'].'";
        	  </script>';
    die;
    break;
}

/*$itrid = $obInstrumento->pegarItrid($_REQUEST['inuid']);*/

$arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $_REQUEST['refid'], PAR3_REFORMULACAO_PRAZO);

if( $_REQUEST['gerar'] == 'S'){
    if( empty($arrReformulacao['refid']) ){
        $refid = $obReformulacao->gerarBackup($_REQUEST['dotid'], $_REQUEST['proid'], PAR3_REFORMULACAO_PRAZO);
        $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $refid, PAR3_REFORMULACAO_PRAZO);
        $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
    } else {
        $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
        if( in_array($arEstado['esdid'], array(PAR3_REFORMULACAO_PRAZO_ESDID_EM_FINALIZADA, PAR3_REFORMULACAO_PRAZO_ESDID_EM_CANCELADA, PAR3_REFORMULACAO_PRAZO_ESDID_EM_PRORROGACAO_OFICIO)) ){
            $refid = $obReformulacao->gerarBackup($_REQUEST['dotid'], $_REQUEST['proid'], PAR3_REFORMULACAO_PRAZO);
            $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $refid, PAR3_REFORMULACAO_PRAZO);
            $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
        } else {
            $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $refid, PAR3_REFORMULACAO_PRAZO);
            $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
        }
    }
} else {
    //$arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $_REQUEST['refid'], PAR3_REFORMULACAO_PRAZO);
    $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
}

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$obReformulacao->verificaPermissaoTelaReprogramacaoPrazo($arEstado['esdid'], $arrReformulacao['refid']);

// foreach ($arrPermissao[$arEstado['esdid']] as $key => $arConfig) {    
//     foreach ($arConfig as $chave => $v) {
//         if( $chave == 'attribs' ) ${'attr_'.$key} = $v;
//         if( $chave == 'config' ) ${'config_'.$key} = $v;
//     }
// }

require APPRAIZ.'includes/cabecalho.inc';
?>
<style>
.danger {
    color: #EF5352 !important;
    font-weight: bold;
}
.btn-reprovar {
    background-color: red !important;
    border-color: red !important;
}
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    <?php
        if (!$_REQUEST['requisicao']&&!$_REQUEST['req']) {
            $controleUnidade->cabecalhoUnidade();
        }?>
	</div>
</div>
<?php
echo '<br>';

$url = "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid={$_REQUEST['dotid']}&inuid={$_REQUEST['inuid']}&proid={$_REQUEST['proid']}";
echo $simec->tab ( criaAbaReformulacao($_REQUEST['inuid'], PAR3_REFORMULACAO_PRAZO), $url );

$obProcesso = new Par3_Controller_Processo();
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Prorrogação de Prazo</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    <div class="row">
            <div class="col-md-12">
                <?php
                $emecod = 'N';
                if ( $obProcesso->verificaProcessoEmenda($_REQUEST['proid']) == 'S') {
                    $edeid = $db->pegaUm("SELECT a.edeid FROM par3.analise a
        						INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = a.inpid AND pp.ppcstatus = 'A'
        					WHERE a.anastatus = 'A'
        						AND pp.proid = {$_REQUEST['proid']}");
                    $obEmenda = new Par3_Controller_EmendasParlamentares();
                    echo $obEmenda->pegaBeneficiarioSelecionado($edeid, $_REQUEST['inpid'], $_REQUEST['inuid']);
                    $emecod = 'S';
                } ?>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12">
                <?php
                if ( $_REQUEST['proid'] ) {
                    echo $obProcesso->carregaDadosOrcamentario($_REQUEST['inuid'], $_REQUEST['inpid'], $_REQUEST['anaid'], $_REQUEST['proid']);
                } ?>
            </div>
            <div class="col-md-12">
                <?php
                if ( $_REQUEST['proid'] ) {
                    echo carregaDadosObraLista($_REQUEST['proid'], false);
                } ?>
            </div>
        </div>
        </div>
<div class="ibox-content">
      <div class="row">
      	<div class="col-md-12">
	<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
        <tr>
        <td style="width: 100%">
    	<div class="row">
            <form name="form-solicita-reformulacao" id="form-solicita-reformulacao" class="form-horizontal" method="post" enctype="multipart/form-data">
			<input type="hidden" name="requisicao" id="requisicao" value="">
        	<input type="hidden" name="emecod" value="<?php echo $emecod; ?>"/>
        	<input type="hidden" name="refid" value="<?php echo $arrReformulacao['refid']; ?>"/>
        	<input type="hidden" name="esdid" value="<?php echo $arEstado['esdid']; ?>"/>
        	<input type="hidden" name="refidpai" value="<?php echo $arrReformulacao['refidpai']; ?>"/>
        	<input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
        	<input type="hidden" name="dotid" value="<?php echo $_REQUEST['dotid']; ?>"/>
        	<input type="hidden" name="proid" value="<?php echo $_REQUEST['proid']; ?>"/>
        	<input type="hidden" name="tirid" value="1"/>
                <div class="row">
                    <div class="col-lg-12">
                    	<?php echo $simec->data('refdata_solicitada', 'Data Prorrogação', $arrReformulacao['refdata_solicitada'], $obReformulacao->getPermissaoAttr('refdata_solicitada'), $obReformulacao->getPermissaoConfig('refdata_solicitada')); ?>
						<?php echo $simec->data('refdata_aprovada', 'Data Aprovada', $arrReformulacao['refdata_aprovada'], $obReformulacao->getPermissaoAttr('refdata_aprovada'), $obReformulacao->getPermissaoConfig('refdata_aprovada')); ?>
						<div class="form-group div_msg_data" style="display: none;" >
                            <label for="" class="col-sm-2 col-md-2 col-lg-2 control-label">&nbsp;</label>
                            <div id="div_msg_data"></div>
                        </div>
                    	<?php echo $simec->textarea('refjustificativa', 'Justificativa', $arrReformulacao['refjustificativa'], $obReformulacao->getPermissaoAttr('refjustificativa'), $obReformulacao->getPermissaoConfig('refjustificativa'));?>
                    	<?php echo $simec->textarea('refacoestomadas', 'Ações Tomadas', $arrReformulacao['refacoestomadas'], $obReformulacao->getPermissaoAttr('refacoestomadas'), $obReformulacao->getPermissaoConfig('refacoestomadas'));?>
                    	<?php echo $simec->textarea('refoutrasinformacoes', 'Outras Informações', $arrReformulacao['refoutrasinformacoes'], $obReformulacao->getPermissaoAttr('refoutrasinformacoes'), $obReformulacao->getPermissaoConfig('refoutrasinformacoes'));?>
                    	<?php echo $simec->textarea('refconsideracoes', 'Parecer técnico', $arrReformulacao['refconsideracoes'], $obReformulacao->getPermissaoAttr('refconsideracoes'), $obReformulacao->getPermissaoConfig('refconsideracoes'));?>
                    	<?php echo $simec->radio('refsituacao', 'Parecer', $arrReformulacao['refsituacao'], array('A' => 'Aprovar', 'R' => 'Reprovar'), $obReformulacao->getPermissaoAttr('refsituacao'), $obReformulacao->getPermissaoConfig('refsituacao'));?>
                    </div>
				</div>
				<div class="row">
					<div class="col-lg-12">
					<div class="form-group">
                        <label class="col-lg-2 control-label">Anexo de cronograma de execução atualizado (pdf)*</label>
                        <div class="col-lg-8">
                            <button type="button" id="add_field" class="btn btn-success" <?php echo $obReformulacao->getPermissaoAttr('btnAnexo')['disabled'];?>><i class="fa fa-plus-square-o"></i> Adicionar Anexo</button>
						<div id="div-anexos"></div>
                        </div>
                    </div>
                    </div>
				</div>
				
				<div class="row">
					<div class="col-lg-12">
					<div class="form-group">
                        <label class="col-lg-2 control-label">&nbsp;</label>
                        <div class="col-lg-9">
                            <div id="div-arquivos">
                    		<?php
                    			echo $obReformulacao->getListaArquivosReformulacao($arrReformulacao['refid'], $arEstado['esdid']);
                    		?>
                    		</div>
                        </div>
                    </div>
                    </div>
				</div>
				
        	</form>
		</div>
		</td>
		<td style="text-align: center;" valign="center"><?php
        echo wf_desenhaBarraNavegacaoBootstrap(
                $arrReformulacao['docid'],
                array('refid' => $arrReformulacao['refid'], 'dotid' => $_REQUEST['dotid'], 'proid' => $_REQUEST['proid']), 
                $ocultar,
                'Fluxo',
            'gravaParecerTecnico('.$arrReformulacao['refid'].', '.$arEstado['esdid'].');'
        );
        ?></td>
		</tr>
		</table>
		</div></div>
		<div class="ibox float-e-margins">
                </div>
    </div>
    <div class="ibox-content" >
    	<div class="row" style="text-align: center;">
<?php if( $obReformulacao->getPermissaoAttr('btnSalvar')['disabled'] == '' ){?>
    	<button type="button" id="btn-salvar" data-dismiss="modal" class="btn btn-success"><i class="fa fa-plus"></i> Salvar</button>
<?php }?>
<?php if( $obReformulacao->getPermissaoAttr('btnSalvarAnalise')['disabled'] == '' ){?>
		<button type="button" id="btn-salvarAnalise" data-dismiss="modal" class="btn btn-success"><i class="fa fa-plus"></i> Salvar</button>
<?php }?>
        </div>
    </div>
    <div class="ibox float-e-margins"> </div>
</div>

<div id="debug"></div>
<br>
<br>
<script>
function gravaParecerTecnico( refid, esdid ){

	if( esdid == 2457 || esdid == 2476 ){
    	var caminho = window.location.href;
        var action  = '&requisicao=gravaParecerTecnico&refid='+refid+'&esdid='+esdid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //jQuery('#debug').html(resp);
            }
        });
	}
	window.location.href = window.location;
}

jQuery(document).ready(function() {
    /*$("#form-solicita-reformulacao" ).validate({
        rules: {
        	refdata_solicitada:{required:true},
            refjustificativa:{required:true},
    		refacoestomadas:{required:true},
            refoutrasinformacoes:{required:true}
        },messages: {
        	refdata_solicitada:{required:"Adicione um arquivo."},
            refjustificativa:{required:"Adicione um arquivo."},
        	refacoestomadas:{required:"Adicione um arquivo teste."},
            refoutrasinformacoes:{required:"Adicione um arquivo teste."}
        },
        onfocusout: function(element) {
            this.element(element); //< inside 'validate()' method, this is like 'valid()'
        },
        submitHandler: function(form) {
            form.submit();
        }
    });*/

    jQuery("[name=refsituacao]").on('change', function() {
    	if( jQuery(this).val() == 'R' ){
    		swal("Atenção", "Tem certeza que deseja reprovar a prorrogação?", "info");

    		/*jQuery(this).parent("label").closest('.btn-campo').addClass('btn-reprovar');
    		jQuery(this).parent("label").closest('.btn-campo').removeClass('active');*/
        }
    });

    jQuery("[name=refdata_aprovada]").on('change', function() {
    	$(document).find('#loading').remove();
    	var caminho = window.location.href;
        var action  = '&requisicao=verificaDataAprovada&data='+jQuery(this).val()+'&refid='+jQuery('[name="refid"]').val();
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //jQuery('#debug').html(resp);

                if( resp != '' ){
                    jQuery(".div_msg_data").show();
                    jQuery("#div_msg_data").html("<span class='danger'>"+resp+"</span>");
                } else {
                	jQuery(".div_msg_data").hide();
                }
            }
        });
        
    });
	
    var campos_max = 10;   //max de 10 campos
    var x = 1; // campos iniciais
    jQuery('#add_field').click (function(e) {
		jQuery("#modal_anexo_arquivos").modal("show");
    });
});

function download_arquivo( arqid ){
	window.location.href = window.location.href+'&requisicao=download&arqid='+arqid;
}
function deletarAnexo( axrid, arqid ){
	window.location.href = window.location.href+'&requisicao=delarquivo&axrid='+axrid+'&arqid='+arqid;
}

jQuery('#btn-salvar').click(function(){	
	
	var retorno = validaInputFormulario('form-solicita-reformulacao', 'validaFormulario');
	
	if( jQuery('#div-arquivos').html().length < 500 ){
		jQuery('#div-anexos').html('<span class="danger">Campo Obrigatório</span>');
		retorno = false;
	} else {
		jQuery('#div-anexos').html('');
	}
	if( retorno == true ){
		jQuery('.loading-dialog-par3').show();
    	jQuery('[name="requisicao"]').val('salvar-reformulacao');
    	jQuery('[name="form-solicita-reformulacao"]').submit();
	}
});

jQuery('#btn-salvarAnalise').click(function(){

	jQuery("#form-solicita-reformulacao").find('div').removeClass('has-error');
    jQuery("#form-solicita-reformulacao").find('.danger').remove();

    var erro = false;
    if( jQuery('[name="refdata_aprovada"]').val() == '' && jQuery('[name="refsituacao"]:checked').val() == 'A' ){
		var divFormInput = jQuery("#form-solicita-reformulacao").find("[name=refdata_aprovada]").parent("div");
        divFormInput.closest('.form-group').addClass('has-error');
        divFormInput.append("<div id='div-refdata_aprovada'></div>");
        jQuery("#div-refdata_aprovada").html("<span class='danger'>Campo Obrigatório!</span>");
        erro = true;
	}
	if( jQuery('[name="refconsideracoes"]').val() == '' ){
		var divFormInput = jQuery("#form-solicita-reformulacao").find("[name=refconsideracoes]").parent("div");
        divFormInput.closest('.form-group').addClass('has-error');
        divFormInput.append("<div id='div-refconsideracoes'></div>");
        jQuery("#div-refconsideracoes").html("<span class='danger'>Campo Obrigatório!</span>");
        erro = true;
	}
	
	if( jQuery('[name="refsituacao"]:checked').length == 0 ){ 
		var divFormInput = jQuery("#form-solicita-reformulacao").find("[name=refsituacao]").parent("div");
        divFormInput.closest('.form-group').addClass('has-error');
        divFormInput.append("<div id='div-refsituacao'></div>");
        jQuery("#div-refsituacao").html("<span class='danger'>Campo Obrigatório!</span>");
        erro = true;
	}
	/*if( erro == true){

		swal({
                title: "Confirmar",
                text: "Existem campos obriga",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    js_verificaExistencia();
                }
                return false;
            }
        );*/
		jQuery('.loading-dialog-par3').show();
		jQuery('[name="requisicao"]').val('salvar-reformulacao-analise');
		jQuery('[name="form-solicita-reformulacao"]').submit();
	//}
});

function salvarDocumentoReformulacao() {

    var tamanhoMaximo = 10 * 1024 * 1024;
    var axrdescricao = $('#axrdescricao').val();

    if($('#arqid').val() == '') {
        alert('Adicione um arquivo');
        return false;
    }

    var file = $("#arqid")[0].files[0];

    if(file.size > tamanhoMaximo){
        alert("O arquivo não pode ser maior que 10MB!");
        return false;
    }

    var formData = new FormData();
    formData.append('arqid', file);
    formData.append('refid', jQuery('#form-solicita-reformulacao').find('[name="refid"]').val() );
    formData.append('esdid', jQuery('#form-solicita-reformulacao').find('[name="esdid"]').val() );
    formData.append('axrdescricao', axrdescricao);
    formData.append('requisicao', 'salvarDocumento');

    jQuery.ajax({
        url: window.location.href,
        data: formData,
        cache: false,
        contentType: false,
        processData: false,
        method: 'POST',
        type: 'POST', // For jQuery < 1.9
        success: function (data) {
            if(trim(data) === 'emptyfile'){
                swal("Erro", "Adicione um arquivo!", "error");
            }else if(trim(data) === 'maxsize'){
                swal("Erro", "Por favor, adicione um arquivo de no máximo 5MB.", "error");
            }else if(trim(data) === 'error'){
                swal("Erro", "Erro ao salvar arquivo, tente novamente ou contate o administrador do sistema.", "error");
            }else{
                $("#modal_anexo_arquivos").modal('hide');
                $('#arqid').val('');
                $('#axrdescricao').val('');
                $('#div-arquivos').html(data);
                //swal("Sucesso", "Arquivo salvo com sucesso!", "success");
            }
        }
    });
}

</script>
<div class="ibox float-e-margins animated modal" id="modal_anexo_arquivos" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo">Documentos Anexo - Incluir</h3>
            </div>
            <div class="ibox-content" id="conteudo-modal">
                <?php
                    echo $simec->input('arqid', 'Anexo', null, array('type' => 'file', 'accept' => 'application/pdf'));
                    echo $simec->input('axrdescricao', 'Descrição', null, array('maxlength'=>100));
                ?>
            </div>
            <div class="ibox-footer">
                <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                    <button type="button" id="cancelar" data-dismiss="modal"
                            class="btn btn-default">
                        <i class="fa fa-times-circle-o"></i> Cancelar
                    </button>
                    <button type="button" id="salvarArquivo"
                            class="btn btn-success" onclick="salvarDocumentoReformulacao()">
                        <i class="fa fa-plus-square-o"></i> Salvar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>
