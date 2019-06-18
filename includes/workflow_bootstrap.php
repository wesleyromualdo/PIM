<?php
/**
 * Desenha a barra de navegação do workflow.
 *
 * @param int $docid Id do documento do workflow.
 * @param mixed[] $dados Dados extras do registro associado ao documento.
 * @param bool[] $ocultar Oculta área do workflow. ex.: $ocultar['historico'] = true;
 * @param string $titulo Título alternativo para barra do workflow.
 */
function wf_desenhaBarraNavegacaoBootstrap($docid, array $dados, $ocultar = null, $titulo = null, $callbackJs = '')
{
    /*
     * $ocultar - Define quais areas serão ocultadas. ex.: $ocultar['historico'] = true;
     *
     * --- Definidas ---
     * historico       : Oculta linha contendo informações obre o historico
     */
    
    global $db;
    $docid = (integer) $docid;
    
    $ocultar = is_array($ocultar) ? $ocultar : array();
    
    // captura dados gerais
    $documento = wf_pegarDocumento($docid);
    if (!$documento) {
        ?>
    	<span class="badge" style="white-space:normal!important">Documento inexistente.</span>
    	<?php return;
    }

    $estadoAtual = wf_pegarEstadoAtual($docid);

    if (is_array($ocultar) && $ocultar['acoes']) {
        $estados = array();
    } else {
        $estados = wf_pegarProximosEstados($docid, $dados);
        if(isset($ocultar['estados'])) {
            if(count($ocultar['estados']) > 0 && count($estados) > 0) {
                $esd = [];
                foreach($estados as $estado) {
                    if(!in_array($estado['esdid'],$ocultar['estados'])) {
                        $esd[] = $estado;
                    }
                }
                $estados = $esd;
            }
        }
    }


    $modificacao = wf_pegarUltimaDataModificacao($docid);
    $usuario = wf_pegarUltimoUsuarioModificacao($docid);
    $comentario = trim(substr(wf_pegarComentarioEstadoAtual($docid), 0, 50)) . "...";

    $dadosHtml = serialize($dados);

    $sql = "SELECT sisdiretorio FROM seguranca.sistema WHERE sisid = {$_SESSION['sisid']}";
    $sisdiretorio = $db->pegaUm($sql);

    $temPreAcao = false;
    foreach ($estados as $estado) {
        if ($estado['aedpreacao'] != '') {
            $temPreAcao = true;
        }
    }

    if ($temPreAcao): ?>
    <style>
    	.ui-button-text{padding-top:0!important}
    </style>
    <?php
    switch ($_SESSION['sislayoutbootstrap']) {
        case 'zimec':
        break;
        case 't':
                echo <<<HTML
<script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
HTML;
default:
                echo <<<HTML
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
HTML;
}
    endif; ?>
<script type="text/javascript">

	<?php if ($temPreAcao):?>
		jQuery.noConflict();

		jQuery(document).ready(function(){
		jQuery('.preacao').off().click(function(){
		var dados = jQuery(this).attr('id').split(';');
		var docid 			= dados[0];
		var aedid 			= dados[1];
		var aedpreacao 		= dados[2];
		aedpreacao 			= aedpreacao.split('(');
		aedpreacao 			= aedpreacao[0];
		var esdid 			= dados[3];
		var acao 			= dados[4];
		var html			= '';
		var dadosHtml = '<?php echo urlencode(json_encode(unserialize($dadosHtml))); ?>';
		console.log(dadosHtml);
		jQuery.ajax({
		type: 	"POST",
		url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
		data: 	'req_ajax_workflow=form_'+aedpreacao+'&docid='+docid+'&aedid='+aedid+'&esdid='+esdid+'&dados=<?=simec_json_encode($dados) ?>',
		async: 	false,
		success: function(msg){
		html = '<center>'+
		'<div id="aguardando" style="display:none; position: absolute; background-color: white; height:98%; width:95%; opacity:0.4; filter:alpha(opacity=40)" >'+
		'<div style="margin-top:250px; align:center;">'+
		'<img border="0" title="Aguardando" src="../imagens/carregando.gif">Carregando...</div>'+
		'</div>'+
		'</center>'+
		'<form method="POST" id="form'+aedpreacao+'" enctype="multipart/form-data" name="form'+aedpreacao+'">'+
		'<input type="hidden" name="docid" 				value="'+docid+'"/>'+
		'<input type="hidden" name="req_ajax_workflow" 	value="'+aedpreacao+'"/>'+
		'<input type="hidden" name="dadosExtra" value="'+dadosHtml+'" />'+
		msg+
		'</form>';
		$("#div_dialog_workflow_<?=$docid?> .modal-body").html(html);
		$("#div_dialog_workflow_<?=$docid?>").modal({
		'show': true
	});

	$('#div_dialog_workflow_<?=$docid?> .btn-primary').click(function(){
	if (confirm('Deseja prosseguir com a tramitação?')) {
	jQuery('#aguardando').show();
	jQuery.ajax({
	type: 	"POST",
	url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
	data: 	'req_ajax_workflow='+aedpreacao
	+'&'+jQuery('#form'+aedpreacao).serialize(),
	async: 	false,
	dataType: 'json',
	success: function(msg){

	var data = msg;

	if( data.boo == true ){
	if( data.msg != '' ){
	alert(data.msg);
}
var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
'?aedid=' + aedid +
'&docid=' + docid +
'&esdid=' + esdid +
'&verificacao=<?php echo urlencode($dadosHtml); ?>';
var janela = window.open(
url,
'alterarEstado',
'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
);
janela.focus();
}else{
if( data.msg != '' ){
alert(data.msg);
}else{
alert('Erro ao realisar a tramitação.');
}
}
jQuery( this ).dialog( "close" );
}
});
}



});









//                            {
//                        resizable: true,
//                        width: 700,
//                        modal: true,
//                        show: { effect: 'drop', direction: "up" },
//                        buttons: {
//                            "Fechar": function() {
//                                jQuery( this ).dialog( "close" );
//                            },
//                            "Tramitar": function() {
//                                jQuery('select[multiple="multiple"]').children().attr('selected', true);
//
//                                jQuery('select[multiple="multiple"],[class="required"]').each(function(){
//                                    jQuery(this).find('[value=""]').remove();
//                                });
//
//                                jQuery("#form"+aedpreacao).validate();
//
//                                if( !jQuery("#form"+aedpreacao).valid() ){
//                                    alert('Preencha os campos obrigatórios.');
//                                    return false;
//                                }
//                                if( confirm('Deseja prosseguir com a tramitação?') ){
//                                    jQuery('#aguardando').show();
//                                    jQuery.ajax({
//                                        type: 	"POST",
//                                        url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
//                                        data: 	'req_ajax_workflow='+aedpreacao+'&'+jQuery('#form'+aedpreacao).serialize(),
//                                        async: 	false,
//                                        dataType: 'json',
//                                        success: function(msg){
//
//                                            var data = msg;
//
//                                            if( data.boo == true ){
//                                                if( data.msg != '' ){
//                                                    alert(data.msg);
//                                                }
//                                                var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
//                                                            '?aedid=' + aedid +
//                                                            '&docid=' + docid +
//                                                            '&esdid=' + esdid +
//                                                            '&verificacao=<?php echo urlencode($dadosHtml); ?>';
//                                                var janela = window.open(
//                                                    url,
//                                                    'alterarEstado',
//                                                    'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
//                                                );
//                                                janela.focus();
//                                            }else{
//                                                if( data.msg != '' ){
//                                                    alert(data.msg);
//                                                }else{
//                                                    alert('Erro ao realisar a tramitação.');
//                                                }
//                                            }
//                                            jQuery( this ).dialog( "close" );
//                                        }
//                                    });
//                                }
//                            }
//                        }
//                    }
//                            );
}
});
});
});
<?php
switch ($_SESSION['sislayoutbootstrap']) {
    case 't':
    case 'zimec':
                echo <<<HTML
    window.$ = jQuery.noConflict();
HTML;
}
    endif; ?>

function wf_atualizarTela(mensagem, janela)
{
	janela.close();
	alert(mensagem);
	window.location.reload();
}

function wf_alterarEstado(aedid, docid, esdid, acao)
{
	if (acao) {
	var f = acao.charAt(0).toLowerCase();
	acao = f + acao.substr(1);
}

bootbox.confirm({
    message: 'Deseja realmente ' + acao + ' ?',
    buttons: {
        confirm: {
            label: 'Tramitar',
            className: 'btn-success'
        },
        cancel: {
            label: 'Cancelar',
            className: 'btn-danger'
        }
    },
    callback: function (resultado) {
        // ...
    /*}
});
	
bootbox.confirm('Deseja realmente ' + acao + ' ?', function(resultado){*/
if (resultado) {
$.get('/geral/workflow/alterar_estado.php', {
aedid: aedid,
docid: docid,
esdid: esdid,
verificacao: '<?php echo urlencode($dadosHtml); ?>',
bs: 's'
}, function(data){
// -- Tramitação sem comentário
if (-1 !== data.indexOf('Estado alterado com sucesso!')) {
bootbox.alert('Estado alterado com sucesso!', function(){
	<?php if( !empty($callbackJs) ) {
        echo $callbackJs;
      } else {?>
			window.location.reload();
<?php }?>
});
return true;
}

// -- Tramitação com comentário
bootbox.dialog({
title: acao,
message: data,
buttons: {
tramitar: {
label: '<span class="glyphicon glyphicon-ok"></span> Tramitar',
className: 'btn-success',
callback: function(){
if ('' === $('#cmddsc').val()) {
alert('Insira um comentário.');
$('#cmddsc').focus();
return false;
}

// -- submetendo comentário para execução da tramitação
$.post(
$('#form-comentario-workflow').attr('action'),
$('#form-comentario-workflow').serialize(),
function(resultado){
if (-1 !== resultado.indexOf('Estado alterado com sucesso!')) {
bootbox.alert('Estado alterado com sucesso!', function(){
	<?php if( !empty($callbackJs) ) {
        echo $callbackJs;
      } else {?>
			window.location.reload();
<?php }?>
});
return true;
} else {

console.log(resultado);

bootbox.alert('Não foi possível alterar estado do documento.', function(){
return true;
});
}
}
);
return true;
}
},
cancelar: {
label: '<span class="glyphicon glyphicon-remove"></span> Cancelar',
className: 'btn-danger'
}
}
});
});
}
}
});
}

/**
 * Exibe informações sobre a tremitação
 */
 function wf_informacaoTramitacao(aedid, aedsc, docid)
 {
 $('#modal-alert .modal-title').html('Informações sobre o trâmite: ' + aedsc);
 $('#modal-alert .modal-body').html('Não constam informações.');

 var url = '/geral/workflow/historico.php' +
 '?modulo=principal/tramitacao' +
 '&acao=C' +
 '&requisicao=informacaoTramitacao' +
 '&docid=' + docid +
 '&aedid=' + aedid;
 $.ajax({
 url: url,
 context: document.body
}).done(function (result) {
$('#modal-alert .modal-body').html(result);
});
$('#modal-alert').modal();
}

/**
 * Exibe informações sobre a tremitação
 */
 function wf_historicoBootstrap(docid)
 {
 $('#modal-alert .modal-title').html('Histórico de Tramitações');
 $('#modal-alert .modal-body').html('Não constam informações.');
 var url = '/geral/workflow/historico.php' +
 '?modulo=principal/tramitacao' +
 '&acao=C' +
 '&requisicao=historicoBootstrap' +
 '&docid=' + docid;

 $.ajax({
 url: url,
 context: document.body
}).done(function (result) {
<?php
global $aumentaModalHistorico;
    if ($aumentaModalHistorico) {
        ?>
	$('.modal-dialog').attr('style', 'width: 48% !important;');
	$('.modal-content').attr('style', 'width: 750px !important;');
	<?php

    } ?>
$('#modal-alert .modal-body').html(result);
});

$('#modal-alert').modal();
}

$(function(){
$('.aedobs').click(function(){
console.log('hiha');
var aedobs = $(this).attr('data-aedobs');
aedobs && bootbox.alert(aedobs);
});
});
</script>
<div class="modal fade div_dialog_workflow" tabindex="-1" role="dialog" aria-labelledby="gridSystemModalLabel" id="div_dialog_workflow_<?=$docid?>">
	<div class="modal-dialog modal-lg" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
				<h4 class="modal-title" id="gridSystemModalLabel">Pré tramitação</h4>
			</div>
			<div class="modal-body"></div>
			<div class="modal-footer">
				<button type="button" class="btn btn-default" data-dismiss="modal">Cancelar</button>
				<button type="button" class="btn btn-primary">Tramitar</button>
			</div>
		</div>
	</div>
</div>
<div class="panel panel-default">
	<div class="panel-heading"><?= $titulo != null ? $titulo : 'Workflow' ?></div>
	<div class="panel-body">
		<?php if (count($estadoAtual)): ?>
			<dl>
				<dd>Estado atual</dd>
				<dt><?=$estadoAtual['esddsc']?></dt>
			</dl>
			<hr style="margin-top:5px" />
		<?php endif; ?>
		<?php if (count($estados)):
            $nenhumaacao = true;
    foreach ($estados as $estado):
                $action = wf_acaoPossivel2($docid, $estado['aedid'], $dados);

    $classTemPreAcao = '';

    if ($estado['aedpreacao'] != '') {
        $funcao = explode('(', $estado['aedpreacao']);
        $respostaPreAcao = function_exists($funcao[0]);

        if ($respostaPreAcao) {
            $classTemPreAcao = 'preacao';
        }

        /**
         * Verifica se existe uma condição de preação para desabilitar ou habilitar essa preação.
         * @example para usar deve criar uma função com o mesmo nome da sua pré ação porém com o
         * prefixo "cond_" ex: cond_preacao(); e nela colocar sua lógica retornando apenas true ou
         * false para que o workflow execute ou não a pré ação.
         */
        if ((is_callable('cond_'.$funcao[0])) && (call_user_func('cond_'.$funcao[0]))) {
            $classTemPreAcao = '';
            $estado['aedpreacao'] = '';
        }
    }

    // -- Trocando o ícone do workflow pelos icones bootstrap
    list($estado['aedicone'], $tipo) = trocaIcone($estado['aedicone']);

    $iconeInformacao = "";
    if (trim($estado['aeddescregracondicao'])) {
        $iconeInformacao = "
                        <span onclick=\"wf_informacaoTramitacao({$estado['aedid']}, '{$estado['aeddscrealizar']}', {$docid})\"
                              style=\"color:#428bca;cursor:pointer\"
                              class=\"glyphicon glyphicon-info-sign\"></span>";
    }
    
    if ($action === true):
    $nenhumaacao = false; ?>
    	<div style="white-space: nowrap; width: 90%">
    		<button type="button" href="#" alt="<?php echo $estado['aeddscrealizar'] ?>"
    			style="white-space:normal!important;margin-bottom:5px;width:100%;font-size:10px;padding:5px!important"
    			class="btn <?=$classTemPreAcao; ?> <?php echo $tipo; ?>"
    			id="<?=$docid ?>;<?=$estado['aedid'] ?>;<?=$estado['aedpreacao'] ?>;<?php echo $estado['esdid'] ?>;
    			<?php echo $estado['aeddscrealizar'] ?>"
    			<?php if ($estado['aedpreacao'] == '') {
        echo <<<HTML
                                    onclick="wf_alterarEstado('{$estado['aedid']}', '{$docid}', '{$estado['esdid']}', '{$estado['aeddscrealizar']}');"
                                    title="{$estado['aeddscrealizar']}"
HTML;
    } else {
        echo <<<HTML
                                    title="{$respostaPreAcao}"
HTML;
    } ?>
                                >
                                <span class="glyphicon <?php echo $estado['aedicone']; ?> " ></span>
                                <?php echo $estado['aeddscrealizar']; ?>
                            </button>
                            <?php 
                                echo $iconeInformacao;
                            ?>	
                            </div>
                <?php else: // -- Quando uma ação não tem sua condição atendida, ela cai nesse fluxo
                if ($action === false) {
                    #Oculta linha contendo a ação cuja a condição para tramitação não esteja atendida
                    if ($estado['aedcodicaonegativa'] == 't') {
                        $nenhumaacao = false;
                        echo <<<HTML
                    <div style="white-space:nowrap;width:90%">
                        <button type="button" class="btn btn-default aedobs" data-toggle="popover"
                                style="white-space:normal!important;margin-bottom:5px;width:100%;font-size:10px;padding:5px!important"
                                data-aedobs="{$estado['aedobs']}" title="{$action}">
                            <span class="glyphicon {$estado['aedicone']}"></span> {$estado['aeddscrealizar']}
                        </button>
                        {$iconeInformacao}
                    </div>
HTML;
                    }
                } else {
                    $nenhumaacao = false;
                    $popover = true;
                    echo <<<HTML
                    <div style="white-space:nowrap;width:90%">
                        <button type="button" class="btn btn-default aedobs" data-toggle="popover"
                                style="white-space:normal!important;margin-bottom:5px;width:100%;font-size:10px;padding:5px!important"
                                title="{$action}" data-aedobs="{$action}">
                            <span class="glyphicon {$estado['aedicone']}"></span> {$estado['aeddscrealizar']}
                        </button>
                        {$iconeInformacao}
                    </div>
HTML;
                } ?>
            <?php endif; ?>
        <?php endforeach; ?>
        <?php if ($nenhumaacao) : ?>
        	<span class="badge" style="white-space: normal !important; width: 100%">
        		Nenhuma ação disponível para o documento.
        	</span>
        <?php endif; ?>
    <?php else: ?>
    	<span class="badge" style="white-space: normal !important; width: 100%">
    		Nenhuma ação disponível para o documento.
    	</span>
    <?php endif; ?>

    <?php if (is_array($ocultar) && !$ocultar['historico']): ?>
    	<div style="margin-top:7px">
    		<button style="width:100%;font-size:10px;padding:5px!important;"
    		onclick="wf_historicoBootstrap('<?php echo $docid ?>');"
    		type="button" class="btn btn-info">
    		<span class="glyphicon glyphicon-time" aria-hidden="true"></span> Histórico
    	</button>
    </div>
<?php endif; ?>
</div>
</div>
<?php
if ($popover) {
                    echo <<<JS
<script type="text/javascript">
$(function(){
    $('[data-popover="popover"]').popover({
        placement:'top',
        trigger:'hover'
    });
});
</script>
JS;
                }
}

function trocaIcone($icone)
{
    switch ($icone) {
        case '1.png':
        return array(' glyphicon-thumbs-up', "btn-success");
        case '2.png':
        return array(' glyphicon-thumbs-down', "btn-danger");
        case '3.png':
        return array(' glyphicon-share-alt', "btn-warning");
    }
}


function wf_desenhaBotoesNavegacaoBootstrap( $docid, array $dados, $ocultar = null )
{
    /*
     * $ocultar - Define quais areas serão ocultadas. ex.: $ocultar['historico'] = true;
     *
     * --- Definidas ---
     * historico       : Oculta linha contendo informações obre o historico
     */
    
    global $db;
    $docid = (integer) $docid;
    
    $ocultar = is_array($ocultar) ? $ocultar : array();
    
    // captura dados gerais
    $documento = wf_pegarDocumento( $docid );
    if ( !$documento )
    {
        ?>
        Documento inexistente!
        <?php
        return;
    }

    $estadoAtual = wf_pegarEstadoAtual( $docid );
    //$estados = wf_pegarProximosEstadosPossiveis( $docid, $dados );
    $estados = wf_pegarProximosEstados( $docid, $dados );
    $modificacao = wf_pegarUltimaDataModificacao( $docid );
    $usuario = wf_pegarUltimoUsuarioModificacao( $docid );
    $comentario = trim( substr( wf_pegarComentarioEstadoAtual( $docid ), 0, 50 ) ) . "...";

    $dadosHtml = serialize( $dados );
    ?>
    <script type="text/javascript">

        function wf_atualizarTela( mensagem, janela )
        {
            janela.close();
            alert( mensagem );
            window.location.reload();
        }

        function wf_alterarEstado( aedid, docid, esdid, acao )
        {
            if(acao) acao = acao.toLowerCase();
            if ( !confirm( 'Deseja realmente ' + acao + ' ?' ) )
            {
                return;
            }
            var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
                '?aedid=' + aedid +
                '&docid=' + docid +
                '&esdid=' + esdid +
                '&verificacao=<?php echo urlencode( $dadosHtml ); ?>';
            var janela = window.open(
                url,
                'alterarEstado',
                'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
            );
            janela.focus();
        }

        function wf_exibirHistorico( docid )
        {
            var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
                '?modulo=principal/tramitacao' +
                '&acao=C' +
                '&docid=' + docid;
            window.open(
                url,
                'alterarEstado',
                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
            );
        }

    </script>
	<button 
		type="button" 
        disabled="disabled"
		class="btn btn-xs btn-w-m btn-primary"
        href="#"
		>
		<?php echo $estadoAtual['esddsc']; ?>
	</button>
    <?php 
    if ( count( $estados ) ) :
        $nenhumaacao = true;
        foreach ( $estados as $estado ) :
            $action = wf_acaoPossivel2( $docid, $estado['aedid'], $dados );
            if($action === true) :
                $nenhumaacao = false;
                $button = strpos($estado['aedicone'], 'down') ? 'danger' : (strpos($estado['aedicone'], 'down') ? 'success' : 'primary');
    ?>
		<button 
			type="button" 
			class="btn btn-xs btn-w-m btn-<?php echo $button; ?>"
            value="<?php echo $estado['aeddscrealizar'] ?>"
            href="#"
            alt="<?php echo $estado['aeddscrealizar'] ?>"
            title="<?php echo $estado['aeddscrealizar'] ?>"
            onclick="wf_alterarEstado( '<?php echo $estado['aedid'] ?>', '<?php echo $docid ?>', '<?php echo $estado['esdid'] ?>', '<?php echo $estado['aeddscrealizar'] ?>' );"
			>
			<i class="fa <?php echo $estado['aedicone']; ?>"></i> <?php echo $estado['aeddscrealizar'] ?>
		</button>
    <?php 
            else :
                if($action === false) :
                    #Oculta linha contendo a ação cuja a condição para tramitação não esteja atendida
                    if( $estado['aedcodicaonegativa'] == 't') : 
                        $nenhumaacao = false; 
    ?>
		<button 
			type="button" 
            disabled="disabled"
			class="btn btn-xs btn-w-m btn-<?php echo $button; ?>"
            value="<?php echo $estado['aeddscrealizar'] ?>"
            href="#"
            alt="<?php echo $estado['aeddscrealizar'] ?>"
            title="<?php echo $estado['aeddscrealizar'] ?>"
			>
			<i class="fa <?php echo $estado['aedicone']; ?>"></i> <?php echo $estado['aeddscrealizar'] ?>
		</button>
	<? 
        	       endif; 
                else :
                    $nenhumaacao = false; 
    ?>
		<button 
			type="button" 
            disabled="disabled"
			class="btn btn-xs btn-w-m btn-<?php echo $button; ?>"
            value="<?php echo $estado['aeddscrealizar'] ?>"
            href="#"
            alt="<?php echo $estado['aeddscrealizar'] ?>"
            title="<?php echo $estado['aeddscrealizar'] ?>"
            onclick="alert( '<?php echo $action; ?>' )"
			>
			<i class="fa <?php echo $estado['aedicone']; ?>"></i> <?php echo $estado['aeddscrealizar'] ?>
		</button>
	<?php 
                endif;
        	endif;
        endforeach;
        if($nenhumaacao) : ?>
		<button 
			type="button" 
            disabled="disabled"
			class="btn btn-xs btn-w-m btn-warning"
            href="#"
			>
			Nenhuma ação disponível para o documento
		</button>
    <?php 
        endif;
    else: ?>
		<button 
			type="button" 
            disabled="disabled"
			class="btn btn-xs btn-w-m btn-warning"
            href="#"
			>
			Nenhuma ação disponível para o documento
		</button>
	<?php 
    endif;
    if(is_array($ocultar) && !$ocultar['historico']) { 
    ?>
		<button 
			type="button" 
			class="btn btn-xs btn-warning"
            alt="Histórico de tramitações"
            title="Histórico de tramitações"
            onclick="wf_exibirHistorico( '<?php echo $docid ?>' );"
			>
			<i class="fa fa-list-ul"></i> 
		</button>
	<? 
    }
}
