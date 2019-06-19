<?php
/**
 * Variável global de condição para exibição do workflow.
 * False (padrão): Desenha o workflow padrão no formato de tabela.
 * True: Desenha o workflow no formato de lista com botões.
 * @var boolean
 */
$estadoAtualFlag = false;

/**
 * Desenha a barra de navegação do workflow.
 *
 * @param int $docid Id do documento do workflow.
 * @param mixed[] $dados Dados extras do registro associado ao documento.
 * @param bool[] $ocultar Oculta área do workflow. ex.: $ocultar['historico'] = true;
 * @param string $titulo Título alternativo para barra do workflow.
 */
function wf_desenhaBarraNavegacaoBotao($docid, array $dados, $ocultar = null, $titulo = null)
{
	global $db;
	global $estadoAtualFlag;

	$docid = (integer) $docid;

	$ocultar = is_array($ocultar) ? $ocultar : array();

	// captura dados gerais
	$documento = wf_pegarDocumento( $docid );
	if ( !$documento )
	{
		?>
		<table align="center" border="0" cellpadding="5" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; width: 80px;">
			<tr>
				<td style="text-align: center;">
					Documento inexistente!
				</td>
			</tr>
		</table>
		<br/><br/>
		<?php
		return;
	}

	$estadoAtual = wf_pegarEstadoAtual( $docid );
	//$estados = wf_pegarProximosEstadosPossiveis( $docid, $dados );
	if(is_array($ocultar) && $ocultar['acoes'] ){
		$estados = array();
	} else {
		$estados = wf_pegarProximosEstados( $docid, $dados );
	}
	$modificacao = wf_pegarUltimaDataModificacao( $docid );
	$usuario = wf_pegarUltimoUsuarioModificacao( $docid );
	$comentario = trim( substr( wf_pegarComentarioEstadoAtual( $docid ), 0, 50 ) ) . "...";

	$dadosHtml = serialize( $dados );

	$sql = "SELECT sisdiretorio FROM seguranca.sistema WHERE sisid = {$_SESSION['sisid']}";
	$sisdiretorio = $db->pegaUm($sql);

	$temPreAcao = false;
	foreach ( $estados as $estado ){
		if( $estado['aedpreacao'] != '' ){
			$temPreAcao = true;
		}
	}
	if( $temPreAcao ){
	?>
    <style>
    .ui-button-text{padding-top:0!important}
    </style>
    <?php if ('t' != $_SESSION['sislayoutbootstrap']): ?>
	<script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <?php endif; ?>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
	<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
	<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
	<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
	<?php
	}
	?>
	<script type="text/javascript">

	<?php if( $temPreAcao ){?>
		jQuery.noConflict();

		jQuery(document).ready(function(){
			jQuery('.preacao').click(function(){
				var dados = jQuery(this).attr('id').split(';');
				var docid 			= dados[0];
				var aedid 			= dados[1];
				var aedpreacao 		= dados[2];
				aedpreacao 			= aedpreacao.split('(');
				aedpreacao 			= aedpreacao[0];
				var esdid 			= dados[3];
				var acao 			= dados[4];
// 				window.html 		= '';
				var html			= '';
				jQuery.ajax({
			   		type: 	"POST",
			   		url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
			   		data: 	'req_ajax_workflow=form_'+aedpreacao+'&docid='+docid+'&aedid='+aedid+'&esdid='+esdid+'&dados=<?=simec_json_encode($dados) ?>',
			   		async: 	false,
			   		success: function(msg){
// 						window.html = '<center>'+
						html = '<center>'+
									'<div id="aguardando" style="display:none; position: absolute; background-color: white; height:98%; width:95%; opacity:0.4; filter:alpha(opacity=40)" >'+
									'<div style="margin-top:250px; align:center;">'+
										'<img border="0" title="Aguardando" src="../imagens/carregando.gif">Carregando...</div>'+
									'</div>'+
								'</center>'+
								'<form method="POST" id="form'+aedpreacao+'" enctype="multipart/form-data" name="form'+aedpreacao+'">'+
									'<input type="hidden" name="docid" 				value="'+docid+'"/>'+
									'<input type="hidden" name="req_ajax_workflow" 	value="'+aedpreacao+'"/>'+
									msg+
								'</form>';
						jQuery( "#div_dialog_workflow" ).html( html );
			   			jQuery( '#div_dialog_workflow' ).show();
			   			jQuery( "#div_dialog_workflow" ).dialog({
							resizable: true,
							width: 700,
							modal: true,
							show: { effect: 'drop', direction: "up" },
							buttons: {
								"Fechar": function() {
									jQuery( this ).dialog( "close" );
								},
								"Tramitar": function() {
									jQuery('select[multiple="multiple"]').children().attr('selected', true);

									jQuery('select[multiple="multiple"],[class="required"]').each(function(){
										jQuery(this).find('[value=""]').remove();
									});

									jQuery("#form"+aedpreacao).validate();

									if( !jQuery("#form"+aedpreacao).valid() ){
										alert('Preencha os campos obrigatórios.');
										return false;
									}
									if( confirm('Deseja prosseguir com a tramitação?') ){
										jQuery('#aguardando').show();
										jQuery.ajax({
									   		type: 	"POST",
									   		url: 	'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
									   		data: 	'req_ajax_workflow='+aedpreacao+'&'+jQuery('#form'+aedpreacao).serialize(),
									   		async: 	false,
									   		dataType: 'json',
									   		success: function(msg){

									   			//var data = jQuery.parseJSON(msg);
									   			var data = msg;

												if( data.boo == true ){
													if( data.msg != '' ){
												   		alert(data.msg);
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
								}
							}
						});
			   		}
				});
			});
		});
        <?php if ('t' == $_SESSION['sislayoutbootstrap']): ?>
        window.$ = jQuery.noConflict();
        <?php endif; ?>
	<?php }?>

		function wf_atualizarTela( mensagem, janela )
		{
			janela.close();
			alert( mensagem );
			window.location.reload();
		}

		function wf_alterarEstado( aedid, docid, esdid, acao )
		{
			if(acao) {
				var f = acao.charAt(0).toLowerCase();
  				acao = f + acao.substr(1);
			}

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
	<div id="div_dialog_workflow" style="display:none" ></div>

	<style>
		.wf_buttons {
			padding: 0px;
			margin: 0px;
		}
		.wf_buttons li {
			display: inline;
			padding: 0px 2px;
		}
		.wf_buttons li a {
			display: inline-block;
			margin: 2px 0px;
			padding: 5px 8px;
			background: blue;
			font-size: 14px;
			border-radius: 4px;
			white-space: nowrap;
			background-color: #428bca;
			border: 1px solid #357ebd;
			color: #ffffff;
		}
		.wf_buttons li a:hover {
			text-decoration: none;
			background-color: #357ebd;
			color: #ffffff;
		}
		.wf_buttons li .disabled {
			display: inline-block;
			margin: 2px 0px;
			padding: 5px 8px;
			background: blue;
			font-size: 14px;
			border-radius: 4px;
			white-space: nowrap;
			background-color: #d7d7d7;
			border: 1px solid #c7c7c7;
			color: #666;
			cursor: not-allowed;
		}
	</style>

	<table id="wf-container-table" border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: <?php echo empty($estadoAtualFlag) ? '3px solid #c9c9c9' : '0px'; ?>;">

        <?php if($titulo != null && empty($estadoAtualFlag)){ ?>
            <tr style="background-color: #c9c9c9; text-align:center; border-bottom: 2px #f4f4f4 solid;">
                <td style="font-size:7pt; text-align:center;">
					<span title="estado atual">
						<b><?= $titulo ?></b>
					</span>
                </td>
            </tr>
        <?php } ?>
		<?php if ( count( $estadoAtual ) && empty($estadoAtualFlag)) : ?>
			<tr style="background-color: #c9c9c9; text-align:center;">
				<td style="font-size:7pt; text-align:center;">
					<span title="estado atual">
						<b>Estado Atual</b>
					</span>
				</td>
			</tr>
			<tr style="text-align:center;">
				<td style="font-size:7pt; text-align:center;">
					<span title="estado atual">
						<?php echo $estadoAtual['esddsc'] ?>
					</span>
				</td>
			</tr>
		<?php endif; ?>
		<?php if(empty($estadoAtualFlag)) : ?>
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual">
					<b>Ações</b>
				</span>
			</td>
		</tr>
		<?php endif; ?>
		<!-- Ações -->
		<tr>
			<td>
				<ul class="wf_buttons">
					<?php if ( count( $estados ) ) : ?>
						<?php $nenhumaacao = true; ?>
						<?php foreach ( $estados as $estado ) : $action = wf_acaoPossivel2( $docid, $estado['aedid'], $dados );?>

							<?php if($action === true) : ?>
							<?php $nenhumaacao = false; ?>
							<li>
								<a	id="li_acao_<?=$estado['aedid'] ?>"
									href="#"
									alt="<?php echo $estado['aeddscrealizar'] ?>"
									title="<?php echo $estado['aeddscrealizar'] ?>"
									class="btn btn-success preacao"
									<?php if( $estado['aedpreacao'] != '' ){?>
									id="<?=$docid ?>;<?=$estado['aedid'] ?>;<?=$estado['aedpreacao'] ?>;<?php echo $estado['esdid'] ?>;<?php echo $estado['aeddscrealizar'] ?>"
									<?php }else{?>
									onclick="wf_alterarEstado( '<?php echo $estado['aedid'] ?>', '<?php echo $docid ?>', '<?php echo $estado['esdid'] ?>', '<?php echo $estado['aeddscrealizar'] ?>' );"
									<?php }?>
								><?php echo (($estado['aedicone'])?"<img align=absmiddle src=../imagens/workflow/".$estado['aedicone']." border=0 width=14 height=14>":""); ?> <?php echo $estado['aeddscrealizar'] ?></a>
							</li>
							<?php else :?>

								<?php if($action === false) : ?>

									<? #Oculta linha contendo a ação cuja a condição para tramitação não esteja atendida
									if( $estado['aedcodicaonegativa'] == 't') : ?>
									<?php $nenhumaacao = false; ?>
										<li>
											<span id="li_acao_<?=$estado['aedid'] ?>" class="btn btn-default" onclick="alert( '<?php echo $estado['aedobs']; ?>' )" onmouseover="return escape('<? echo $estado['aedobs']; ?>');">
											<?php echo (($estado['aedicone'])?"<img align=absmiddle src=../imagens/workflow/".$estado['aedicone']." border=0 width=14 height=14>":""); ?> <?php echo $estado['aeddscrealizar'] ?>
											</span>
										</li>
									<? endif; ?>

								<?php else: ?>
								<?php $nenhumaacao = false; ?>
								<li>
									<span id="li_acao_<?=$estado['aedid'] ?>" class="btn btn-default" onclick="alert( '<?php echo $action; ?>' )" onmouseover="return escape('<? echo $action; ?>');">
										<?php echo (($estado['aedicone'])?"<img align=absmiddle src=../imagens/workflow/".$estado['aedicone']." border=0 width=14 height=14>":""); ?> <?php echo $estado['aeddscrealizar'] ?>
									</span>
								</li>
								<?php endif; ?>

							<?php endif; ?>
						<?php endforeach; ?>
						<?php if($nenhumaacao) : ?>
							<span style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
								nenhuma ação disponível para o documento
							</span>
						<?php endif; ?>
					<?php else: ?>
						<span style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
							nenhuma ação disponível para o documento
						</span>
					<?php endif; ?>
				</ul>
			</td>
		</tr>
		<!-- Fim: Ações -->
		<?php if (is_array($ocultar) && empty($estadoAtualFlag)) : ?>
			<?php if(!$ocultar['historico']) : ?>
			<tr style="background-color: #c9c9c9; text-align:center;">
				<td style="font-size:7pt; text-align:center;">
					<span title="estado atual">
						<b>Histórico</b>
					</span>
				</td>
			</tr>
			<tr style="text-align:center;">
				<td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
					<img
						style="cursor: pointer;"
						src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/fluxodoc.gif"
						title="<?php echo $usuario['usunome'] . " - " . $modificacao . " - " . simec_htmlentities( $comentario ); ?>"
						onclick="wf_exibirHistorico( '<?php echo $docid ?>' );"
					/>
				</td>
			</tr>
			<?php endif; ?>
		<?php endif; ?>
	</table>
	<br/><br/>
	<?php
}
