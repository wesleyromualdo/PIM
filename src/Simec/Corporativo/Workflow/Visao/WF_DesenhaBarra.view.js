var $WfSimec = {
		
	dadoDocumento : {},
	
	__init : function()
	{
//		this.carregarDadosWorkflow();
		this.montarContainerBarraWf();
	},
	
	montarContainerBarraWf : function()
	{
		jQuery('.js-container-barra-wf').each(function()
		{
			var $container = jQuery(this);
			var docid = $container.attr('dado-identificacao');
			$WfSimec.carregarDadosWorkflow(docid);

			jQuery.each($WfSimec.dadoDocumento.todas_acoes_permitidas_perfil, function(idx, acao)
			{
				acao.aedicone[1] = (acao.resultado_validacao_condicao == 1 ? acao.aedicone[1] : 'default');
				
				$divAcao = $container.find('.js-div-externa-acao[dado-identificacao='+ docid +'_'+ acao.aedid +']');
				$divAcao.find('button:first').attr({alt:acao.aeddscrealizar, title:acao.aeddscrealizar})
											 .addClass(acao.aedicone[1])
											 .find('span:first').addClass(acao.aedicone[0]);
			});
			
			
		});
	},

	carregarDadosWorkflow : function(docid)
	{
		this.dadoDocumento = _JS.workflowSimecDocumento[docid];
	},
	
	executarAcao : function(element)
	{
		var identificacao = $WfSimec.pegarDadoIdentificacaoAcao(element);
//		console.log(element);
//		console.log(identificacao);
		$WfSimec.carregarDadosWorkflow(identificacao.docid);
		
		jQuery.each($WfSimec.dadoDocumento.todas_acoes_permitidas_perfil, function(idx, acao)
		{
			if (acao.aedid == identificacao.aedid) {
				if (acao.resultado_validacao_condicao == 1) {
					swal({
						  title				: "",
						  text				: "Deseja realmente " + acao.aeddscrealizar + "?",
						  type				: "warning",
						  showCancelButton	: true,
						  confirmButtonText	: "Tramitar",
						  confirmButtonColor: "#EC4758",
						  closeOnConfirm	: true,
						},
						function(isConfirm)
						{
							if (isConfirm) {
								if (acao.esdsncomentario == 't') {
									$WfSimec.abrirModalComentario(acao);
								} else {
									setTimeout(function () {$WfSimec.tramitarDocumento(identificacao)}, 500);
								}
							}
						});				
				} else {
					if (acao.resultado_validacao_condicao != '' && typeof acao.resultado_validacao_condicao == 'string') {
						swal('', acao.resultado_validacao_condicao, 'error');
					} else {
						swal('', acao.aedobs, 'error');					
					}
				}
			}
		});
	},
	
	abrirModalComentario : function(acao)
	{
		jQuery('#modalComponenteWorkflow').modal('show');
		jQuery('#modalComponenteWorkflow .modal-content').html('');
				
		$modalComentario = jQuery('.tpl-modal-comentario-workflow:first').clone();
		$modalComentario.addClass('js-modal-comentario-workflow')
						.removeClass('tpl-modal-comentario-workflow')
						.show();
		$modalComentario.view('assign', {
			'docdsc' 			: $WfSimec.dadoDocumento.docdsc,
			'estado-atual' 		: acao.esddscorigem,
			'acao' 				: acao.aeddscrealizar,
			'onclick-tramitar' 	: '$WfSimec.executarAcaoComComentario('+ $WfSimec.dadoDocumento.docid +','+ acao.aedid +')'
		});
		
		jQuery('#modalComponenteWorkflow .modal-content').append($modalComentario);
	},
	
	abrirModalHistorico : function (element)
	{
		var docid = jQuery(element).parents('.js-container-barra-wf:first').attr('dado-identificacao');
		jQuery('#modalComponenteWorkflow .modal-content').html('');
		jQuery('#modalComponenteWorkflow').modal('show');
		
		$modalHistorico = jQuery('.tpl-modal-historico-workflow:first').clone();
		$modalHistorico.addClass('js-modal-historico-workflow')
					   .removeClass('tpl-modal-historico-workflow')
				       .show();
		
		jQuery('#modalComponenteWorkflow .modal-content').append($modalHistorico);
		
		jQuery.ajax({
			url			: location.href,
			method		: "GET",
			data		: {docid:docid, historicoDocumentoWorkflow:true},
			beforeSend	: function()
			{
				jQuery('.modal-body', $modalHistorico).html('<center><i class="fa fa-cog fa-spin fa-3x"></i></center>');
			},
			success 	: function (htmlResponse)
			{
				jQuery('.modal-body', $modalHistorico).html(htmlResponse);
//				console.log($modalHistorico.html(), '-------', jQuery('.modal-body', $modalHistorico).html());
			},
			error		: function ()
			{
				jQuery('.modal-body', $modalHistorico).html('<center><i class="fa fa-exclamation-circle fa-2x"></i> Erro ao busar o histórico do documento, tente novamente mais tarde!\nSe o erro persistir contate o administrador do sistema.</center>');
			}
		});
	},

	abirModalInformacao : function (element)
	{
		var identificacao = $WfSimec.pegarDadoIdentificacaoAcao(element);
		$WfSimec.carregarDadosWorkflow(identificacao.docid);
		
		jQuery('#modalComponenteWorkflow .modal-content').html('');
		jQuery('#modalComponenteWorkflow').modal('show');
		
		var $modalInformacaoAcao = jQuery('.tpl-modal-informacao-workflow:first').clone();
		$modalInformacaoAcao.addClass('js-modal-informacao-workflow')
						    .removeClass('tpl-modal-informacao-workflow')
					        .show();
		
		jQuery.each($WfSimec.dadoDocumento.todas_acoes_permitidas_perfil, function(idx, acao)
		{
			if (acao.aedid == identificacao.aedid) {
				$modalInformacaoAcao.view('assign', {
					'descricao-acao' 	 : acao.aeddscrealizar,
					'observacao-tramite' : acao.aeddescregracondicao
				});				
			}
		});
		
		jQuery('#modalComponenteWorkflow .modal-content').append($modalInformacaoAcao);
	},
	
	executarAcaoComComentario: function (docid, aedid)
	{
		var comentario = jQuery.trim(jQuery('.js-modal-comentario-workflow').find('#comentario_acao_wf').val());
		
		if (comentario == '') {
			swal('', 'O campo comentário é obrigatório!', 'error');
		} else {
			jQuery('#modalComponenteWorkflow').modal('hide');
			
			$WfSimec.carregarDadosWorkflow(docid);
			$WfSimec.tramitarDocumento({docid:docid, aedid:aedid, comentario:comentario});
		}
	}, 
	
	tramitarDocumento: function(identificacao)
	{
		var paramDado 									  = identificacao;
		paramDado.tramitarDocumentoWorkflow 			  = true;
		paramDado.parametro_adicional_para_funcao_interna = $WfSimec.dadoDocumento.parametro_adicional_para_funcao_interna;
		paramDado.titulo_caixa_workflow 				  = $WfSimec.dadoDocumento.titulo_caixa_workflow;
		
	    jQuery.ajax({
	        type	: "GET",
	        url		: window.location.href,
	        data	: paramDado,
	        async	: false,
	        success	: function(req)
	        {
	        	try {
	        		var htmlBarra 		= pegaRetornoAjax('<html_barra_wf>', '</html_barra_wf>', req, true);
	        		var sucessoTramite 	= pegaRetornoAjax('<sucesso_tramite_wf>', '</sucesso_tramite_wf>', req, true);
	        		var jsDocumento 	= pegaRetornoAjax('<js_documento_wf>', '</js_documento_wf>', req, true);
	        		
	        		if (sucessoTramite != '1') {
	        			throw false;
	        		}
	        		
	        		jsDocumento = jQuery.parseJSON(jsDocumento);
	        		_JS.workflowSimecDocumento[identificacao.docid] = jsDocumento;
	        		
	        		jQuery('.js-container-barra-wf[dado-identificacao='+identificacao.docid+']').replaceWith(htmlBarra);
	        		
	        		$WfSimec.montarContainerBarraWf(true);
	        		
	        		swal({
	        			title:'', 
	        			text:'Tramite realizado com sucesso', 
	        			type:'success', 
	        			html:true
	        		});
	        	}catch (e) {
	        		swal({
	        			title:'Erro', 
	        			text:'Falha ao tramitar o documento, tente novamente!<br>Se o erro persistir, contacte o administrador do sistema.', 
	        			type:'error', 
	        			html:true
	        		});
	        	}					                	
	        },
	        error	: function (req)
	        {
	        	swal({
	        		title:'Erro', 
	        		text:'Falha ao tramitar o documento, tente novamente!<br>Se o erro persistir, contacte o administrador do sistema.', 
	        		type:'error', 
	        		html:true
	        	});					                	
	        }
	    });
	},
	
	pegarDadoIdentificacaoAcao : function(element)
	{
		var dado = jQuery(element).parents('.js-div-externa-acao:first').attr('dado-identificacao');
		dado = dado.split('_');
		
		return {docid:dado[0], aedid:dado[1]};
	},
}

jQuery(document).ready(function()
{
	$WfSimec.__init();
});