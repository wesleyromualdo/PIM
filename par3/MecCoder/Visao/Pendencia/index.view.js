jQuery(document).ready(function ()
{
	ClassePendencia.iniciar();

	jQuery('.next').click(function()
    {
		var url   = new URL(location.href);
		var inuid = url.searchParams.get("inuid");
		
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=' + inuid;
    });

    jQuery('.previous').click(function()
    {
		var url   = new URL(location.href);
		var inuid = url.searchParams.get("inuid");
		
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=' + inuid;
    });	
});

var ClassePendencia = {
	
	iniciar : function ()
	{
		this.eventoDetalharPendencia();
		this.eventoSubDetalhePendencia();
		this.eventoExpandirPendencia();
		this.eventoAtualizarPendencia();
	},
		
	eventoDetalharPendencia : function ()
	{
		jQuery('.box-pendencia').click(function()
		{
			var type = $(this).attr('type');
			var $caixaDetalhePendencia = $('.'+type);
			
            if( $(this).hasClass('active') ) {
                $(this).removeClass('active');
                $caixaDetalhePendencia.animate({opacity : 0}, 'slow', function (){ 
                	var $listPendencias = $caixaDetalhePendencia.closest('.js-list-pendencias');
                	jQuery(this).remove() 
                	
                	if ($listPendencias.find('> div').length == 0) {
                		$listPendencias.closest('.js-lista-caixa-detalhe-pendencia').hide();
                	}
                });
                return;
            }
            $(this).addClass('active');

            ClassePendencia.detalharPendencia(this, type);
		});	
	},
	
	eventoSubDetalhePendencia : function ()
	{
		jQuery('.js-caixa-lista-pendencias').on('click', '.subDetalhePendencia', function ()
		{
			ClassePendencia.subDetalhePendencia(jQuery(this));
		});
	},	
	
	eventoExpandirPendencia : function ()
	{
		jQuery('.ibox:first').on('click', ".abrir-pendencias", function()
		{
			var $this  = jQuery(this);
			var $list  = $this.closest('[class*="js_tpl_pendencia_"]').find('.js-list-pendencias-content');

			$list.slideToggle(1000, function ()
			{
				if ($list.is(':hidden')) {
					$this.text(' Mostrar todas as pendências');
					$this.removeClass('fa-minus-circle');
					$this.addClass('fa-plus-circle');
				} else {
					$this.text(' Esconder todas as pendências');
					$this.addClass('fa-minus-circle');
				}
			});
		});
	},

	subDetalhePendencia : function ($this)
	{
        var funcao = $this.attr('funcao');
        var $divFuncao = $('#div_' + funcao);

        if ($divFuncao.is(':hidden') != true) {
        	$divFuncao.fadeOut(1500, function(){
	    		$divFuncao.html('');
        	});
        	return;
        }
        
    	var url = window.location.href.replace('#', '');
        $.ajax({
            type		: "POST",
            url			: url + '&requisicao=exibirSubDetalhePendencia',
            data		: {funcao:funcao},
            async		: true,
            beforeSend	: function () {
//                $('#loading').show();
            },
            success		: function (resp) {
//            	$('#loading').hide();
				try {
					var content = $.parseJSON(resp);		
				
					if (content.length == 0) {
						$divFuncao.append($('.templates .js_tpl_subDetalhePendencia_vazio').clone());						
					} else {
						$divFuncao.append($('.templates .js_tpl_table_subDetalhePendencia').clone());
						$tableItemDetalhe = $divFuncao.find('.js_tpl_table_subDetalhePendencia');
						
						$tableItemDetalhe.find('.js-grupo-linha')
										 .view('iterate', {
											'template' 		: $tableItemDetalhe.find('.js_tpl_linha_subDetalhePendencia tr'),
											'collection'	: content
										 });
					}	
				} catch(e) {
					$divFuncao.html(resp);							
				}
				
	        	$divFuncao.fadeIn(2000);
            }
        });
	},
	
	detalharPendencia : function (obj, type)
	{
		var caixaTipoAgrupamento = jQuery(obj).parents('.ibox-content:first');
		$('.js-lista-caixa-detalhe-pendencia', caixaTipoAgrupamento).show();
		
		var url = location.href.replace('#', '');
		jQuery.ajax({
			  url: url + '&requisicao=exibirDetalhamentoPendencia',
		      type: "POST",
		      data: {'pendencia' : type},
		      success: function(resp)
		      {
		          var $listaPendencias 	= $('.js-list-pendencias', caixaTipoAgrupamento);
		          
		          try {
		              var content = $.parseJSON(resp);		
		              	
		              var classTemplate = '';
		              if (content.type == 'obras_par') {
		            	  classTemplate = '.js_tpl_pendencia_obras_par';
		              } else if (content.type == 'contas') {
		            	  classTemplate = '.js_tpl_pendencia_prestacao_contas';
		              } else if (content.type == 'monitoramento_par') {
		            	  classTemplate = '.js_tpl_pendencia_monitoramento_par';
		              } else {
		            	  classTemplate = '.js_tpl_pendencia';
		              }
		              
		              classTemplate = ((content.boolean == 1)
		            		  				? classTemplate
		            		  				: '.js_tpl_pendencia_vazio');			
		              
		              var $cloneTemplate = $('.templates ' + classTemplate).clone()
		              													 .removeClass(classTemplate)
		              													 .addClass(content.type)
		              													 .appendTo($listaPendencias);
		              
		              $cloneTemplate.find('.widget div:first').addClass('slim-scroll');
		              $cloneTemplate.find('.slim-scroll i:first').addClass(content.thumb);
		              
		              $cloneTemplate.view('assign', {
		            	  description 	  : content.description,
		            	  widget 		  : content.widget,
		            	  nomeEntidade 	  : content.nomeEntidade,
		            	  msgVazio 	  	  : content.msg, // Quando NÃO há pendências
		            	  msgPendencia 	  : content.msg // Quando há pendências
		              });
		              
		              if (content.type == 'obras_par') {
		            	  var $list 	= $('.js-list-pendencias-content', $cloneTemplate);
		            	  
		            	  /*************
		            	   * LISTA AS PENDÊNCIAS
		            	   */
		            	  $list.view('iterate', {
			                  'template'	: $('.templates .js_tpl_item_pendencia_obras_par'),
			                  'collection'	: content.pendencia,
			                  'callbackItem': function ($item, dados)
			                  {
									$item.view('assign', {
										'titulo-pendencia' : "ID Obras2: " + dados.obrid + ' - ' + dados.obrnome,
										'motivo-pendencia' : "Motivo: " + dados.pendencia
									});
			                  }
		            	  });
		            	  /*************
		            	   * LISTA AS RESTRIÇÕES
		            	   */
		            	  $list.view('iterate', {
			                  'template'	: $('.templates .js_tpl_item_restricao_obras_par'),
			                  'collection'	: content.restricao,
			                  'callbackItem': function ($item, dados)
			                  {
									$item.view('assign', {
										'titulo-restricao' : "RESTRIÇÃO: ID Obras2: " + dados.obrid + ' - ' + dados.obrnome
									});
									
									$item.view('iterate', {
										'template'    : $('.templates .js_tpl_p_detalhe_restricao_obras_par'),
										'collection'  : dados['detalhe-item-restricao'],
										'callbackItem': function ($item2, dados2)
										{
											$item2.view('assign', {
												'detalhe-restricao' : '* ' + dados2
											});	
										}
									});
			                  }
		            	  });
		              } else if (content.type == 'contas') {
		            	  var $list 	= $('.js-list-pendencias-content', $cloneTemplate);
		            	  
		            	  /*************
		            	   * LISTA AS PENDÊNCIAS
		            	   */
		            	  $list.view('iterate', {
			                  'template'	: $('.templates .js_tpl_item_pendencia_prestacao_contas'),
			                  'collection'	: content.pendencia,
			                  'callbackItem': function ($item, dados)
			                  {
									$item.view('assign', {
										'tipoProcesso' 	: dados.tipo,
										'processo' 		: "Processo: " + dados.processo,
										'documento' 	: "Documento: " + dados.documento,
										'motivo' 		: "Motivo: " + dados.situacao
									});
			                  }
		            	  });
		              } else if (content.type == 'monitoramento_par') {
		            	  var arTopicoPendencia = [];
		            	  
		            	  if (content.regraPagamento.length > 0) {
		            		  arTopicoPendencia = [{
		            			  'texto' 	 : 'Processos com pagamento efetivado e sem nota fiscal',
		            			  'callback' : 'monitoramento_par_regraPagamento'
		            		  }];
		            	  }
		            	  
		            	  if (content.saldoConta.length > 0) {
		            		  arTopicoPendencia.push({
		            			  'texto' 	 : 'Processos com saldo em conta e pendências no monitoramento',
		            			  'callback' : 'monitoramento_par_saldoConta'
		            		  });
		            	  }
		            	  
		            	  if (content.regraMonitoramento2010.hasOwnProperty('pendencias')) {
		            		  arTopicoPendencia.push({
		            			  'texto' 	 : 'Existem subações 100% pagas com pendências no monitoramento',
		            			  'callback' : 'monitoramento_par_regraMonitoramento2010'
		            		  });
		            	  }
		            	  
		            	  if (content.regraMonitoramento2010Termo.hasOwnProperty('pendencias')) {
		            		  arTopicoPendencia.push({
		            			  'texto' 	 : 'Há termo(s) de compromisso que necessita(m) validação pelo prefeito',
		            			  'callback' : 'monitoramento_par_regraMonitoramento2010Termo'
		            		  });
		            	  }
		            	  
		            	  var $list = $('.js-list-pendencias-content', $cloneTemplate);
		            	  $list.view('iterate', {
		            		  'template'	: jQuery('.templates .js_tpl_titulo_topico'),
		            		  'collection'	: arTopicoPendencia,
		            		  'callbackItem': function ($item, dados)
		            		  {
		            			  $item.view('assign', {
		            			  	'titulo-topico' : dados.texto
		            			  });
		            			  
		            			  ClassePendencia[dados['callback']]($item, content);
		            		  }
		            	  });
		              }
		              
		              $cloneTemplate.animate({opacity : '1'}, 3000);
		          } catch(e) {
		              $listaPendencias.append(resp);
		          }
		
		      }
		});
	},
	
	monitoramento_par_regraMonitoramento2010 : function ($item, content)
	{
		var i = 0;
		var chavePendencias = Object.keys(content.regraMonitoramento2010.pendencias);
		
		$item.view('iterate', {
			'template'	: $('.templates .js_tpl_regraMonitoramento2010_pendencias'),
			'collection'	: content.regraMonitoramento2010.pendencias,
			'callbackItem': function ($item1, dados)
			{
				$item1.view('assign', {
					'titulo-item' : dados,
					'funcao'	  : chavePendencias[i]
				});
				
				var $tpl_subDetalhe = jQuery('.templates .js_tpl_conteudo_subDetalhePendencia').clone();
				$item.append($tpl_subDetalhe.attr('id', 'div_' + chavePendencias[i]));
				
				i++;
			}
		});
		
	},
	
	monitoramento_par_regraMonitoramento2010Termo : function ($item, content)
	{
		$item.view('iterate', {
			'template'	: $('.templates .js_tpl_regraMonitoramento2010Termo_pendencias'),
			'collection'	: [content.regraMonitoramento2010Termo.pendencias],
			'callbackItem': function ($item1, dados)
			{
				$item1.view('assign', {
					'titulo-item' : dados.termos_n_assinados,
					'funcao'	  : 'termos_n_assinados'
				});
				
				var $tpl_subDetalhe = jQuery('.templates .js_tpl_conteudo_subDetalhePendencia').clone();
				$item.append($tpl_subDetalhe.attr('id', 'div_termos_n_assinados'));
			}
		});
		
	},
	
	monitoramento_par_saldoConta : function ($item, content)
	{
		$item.view('iterate', {
			'template'	: $('.templates .js_tpl_saldoConta'),
			'collection'	: content.saldoConta,
			'callbackItem': function ($item1, dados)
			{
				$item1.view('assign', {
					'processo' : 'Processo: ' + dados.processo,
					'vigencia' : 'Vigência: ' + dados.fimvigencia
				});
			}
		});
	},
	
	monitoramento_par_regraPagamento : function ($item, content)
	{
		$item.view('iterate', {
			'template'	: $('.templates .js_tpl_regraPagamento'),
			'collection'	: content.regraPagamento,
			'callbackItem': function ($item1, dados)
			{
				$item1.view('assign', {
					'processo'   : 'Processo: ' + dados.processo,
					'saldoconta' : 'Saldo em conta: R$' + MascaraMonetario(dados.saldoconta)
				});
			}
		});
		
		
	},
	
	/**
	 * ATUALIZAÇÃO DE PENDÊNCIAS - INÍCIO
	 */
	eventoAtualizarPendencia : function ()
	{
		$('.js-atualizar-pendencias').on('click',function(){
			ClassePendencia.atualizaPendencias(_JS.inuid);
		});
	},
    
    atualizaPendencias : function ( inuid )
    {
        $("#modal-visualiza-pendencia").modal("show");

//        jQuery('[name="inuid"]').val(inuid);
        jQuery('.text-pendencia').html('Atualizando Pendência - Conselho CAE');

        var action  = '&requisicao=atualizaPendencias&tipo=cae&inuid='+inuid;
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="cae"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Obras PAR');
                jQuery('.progress-bar').html('14%');
                jQuery('.progress-bar').css('width', '14%');
                setTimeout(ClassePendencia.pendenciaPAR,1000);
            }
        });
    },

    pendenciaPAR : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid;//jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=par&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="par"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - CACS');
                jQuery('.progress-bar').html('28%');
                jQuery('.progress-bar').css('width', '28%');
                setTimeout(ClassePendencia.pendenciaCACS,1000);
            }
        });
    },

    pendenciaCACS : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid;//jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=cacs&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="cacs"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - PNE');
                jQuery('.progress-bar').html('42%');
                jQuery('.progress-bar').css('width', '42%');
                setTimeout(ClassePendencia.pendenciaPNE,1000);
            }
        });
    },

    pendenciaPNE : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid; //jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=pne&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="pne"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Habilita');
                jQuery('.progress-bar').html('56%');
                jQuery('.progress-bar').css('width', '56%');
                setTimeout(ClassePendencia.pendenciaHabilita,1000);
            }
        });
    },

    pendenciaHabilita : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid; //jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=habilita&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="habilita"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Monitoramento PAR');
                jQuery('.progress-bar').html('70%');
                jQuery('.progress-bar').css('width', '70%');
                setTimeout(ClassePendencia.pendenciaMonitoramento,1000);
            }
        });
    },

    pendenciaMonitoramento : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid; //jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=monitoramento&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="monitoramento"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Contas');
                jQuery('.progress-bar').html('78%');
                jQuery('.progress-bar').css('width', '78%');
                setTimeout(ClassePendencia.pendenciaContas,1000);
            }
        });
    },

    pendenciaContas : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid; //jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=contas&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="contas"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - SIOPE');
                jQuery('.progress-bar').html('84%');
                jQuery('.progress-bar').css('width', '84%');
                setTimeout(ClassePendencia.pendenciaSiope,1000);
            }
        });
    },

    pendenciaSiope : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid; //jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=siope&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            dataType: "json",
            success: function (resp) {
//                jQuery("#div_debug").html(resp);console.log(resp.desc);return false;
                jQuery('[name="siope_desc"]').val(resp.desc);
                jQuery('[name="siope"]').val(resp.cod);
                jQuery('.text-pendencia').html('Atualizando View de Pendências');
                jQuery('.progress-bar').html('100%');
                jQuery('.progress-bar').css('width', '100%');
                setTimeout(ClassePendencia.atualizaPendenciaView,1000);
            }
        });
    },

    atualizaPendenciaView : function ()
    {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = _JS.inuid; //jQuery('[name="inuid"]').val();
        var cae = jQuery('[name="cae"]').val();
        var par = jQuery('[name="par"]').val();
        var cacs = jQuery('[name="cacs"]').val();
        var pne = jQuery('[name="pne"]').val();
        var habilita = jQuery('[name="habilita"]').val();
        var monitoramento = jQuery('[name="monitoramento"]').val();
        var siope = jQuery('[name="siope"]').val();
        var siope_desc = jQuery('[name="siope_desc"]').val();
        var contas = jQuery('[name="contas"]').val();

        var action  = '&requisicao=atualizaPendenciaView&tipo=siope&inuid='+inuid+'&cae='+cae+'&par='+par+'&cacs='+cacs+'&pne='+pne+'&habilita='+habilita+
            '&monitoramento='+monitoramento+'&siope='+siope+'&siope_desc='+siope_desc+'&contas='+contas;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                jQuery('.text-pendencia').html('Pendências Atualizadas com Sucesso!');
                //jQuery("#div_debug").html(resp);
            }
        });
    },
    
    fecharPendencia : function ()
    {
        window.location.reload();
    }

	/**
	 * ATUALIZAÇÃO DE PENDÊNCIAS - FIM
	 */
	
};


