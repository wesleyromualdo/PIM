<script type="text/javascript">
        $(document).ready(function() 
        {   
			if( $("#pgtid").val() != '' )
			{
				montaComboContratosEdicao();

				if( $("#tem_anexos").val() == 1 )
				{
					$("#acompanhamento").show();
            		

            		var total = ( $("#total_aditivos").val() != '' ) ? $("#total_aditivos").val() : 0;
            		
            		atualizaObjeto(total);

                    $("input.[name=listaNtfId[]]").each(function () {
                        arrIdNotas.push($(this).val());
                    });

				}
			}
        });
</script>


<script type="text/javascript">

    var listaObjDocumentos = [];
    var idControleObjDocumento	= 0;
    var excluiEdicao 			= false;
    var acaoVisualizar = $("#acaoVisualizar").val();
    var arrIdNotas = [];

    /**
     * Função responsável por atualizar o objeto para montar os anexos
     * idControleObjDocumento++;
     */
	function atualizaObjeto(total) 
 	{
     	for (i = 0; i < total; i++) 
		{	
			objDocumento = {
				"id": i
			};
			listaObjDocumentos.push(objDocumento);
			
		}
 		
    }
     /**
      * Função responsável por fazer a requisição para a montagem da combo 'Contrato ou Aditivo de Valor'.
      * @param entid Id da 'entidade' ou da 'construtora extra'.
      * @returns {boolean}
      */
	function retornaValorNota( ntfid ) 
	{
    		$("#valor_nota").html("");
       		$("#dotvalordocumento").val("");
       		
  		$.ajax({
  			url: window.location.href,
  			type: "post",
  			async: false,
  			dataType: "json",
  			data: {
  				"requisicao": "carregaValorNota",
  				"ntfid": ntfid
  			},
  			beforeSend: function () {
  				divCarregando();
  			},
  			success: function (data) 
  			{
  				if(data.ntfvalornota != '')
  				{
  					var ntfvalornota = formatarValorMonetario(data.ntfvalornota)
  					$("#valor_nota").html(ntfvalornota);
               		$("#dotvalordocumento").val(data.ntfvalornota);
  					
  				}
  				else
  				{
  					$("#valor_nota").html("");
               		$("#dotvalordocumento").val("");
  				}
  				divCarregado();
  			}
  		});
	}

  	function montaComboContratosEdicao() 
	{
  		entid = $("#empresaContratada").val();

  		if( entid == '' )
  		{
			return false;
  		}

  		$("#empresaContratada").attr("disabled", true);
  		
		$("#contratos").empty().append("<option value=''>Selecione...</option>");
		$("#ntfid").empty().append("<option value=''>Selecione...</option>");
		$("#valor_nota").text("");
		$("#dotvalordocumento").val("");
		
		if (entid.length <= 0) {
			return false;
		}
		$.ajax({
			url: window.location.href,
			type: "post",
			async: false,
			dataType: "json",
			data: {
				"requisicao": "carregarContratos",
				"entid": entid
			},
			beforeSend: function () {
				divCarregando();
			},
			success: function (data) {

				var selected = null;

				if (data.length > 0) {

					//selected = data.length == 1 ? "selected" : selected; //@todo

					$.each(data, function (chave, valor) {
						$("#contratos")
							.append($("<option " + selected + "></option>")
							.attr("value", valor.codigo)
							.text(valor.descricao)
							);
					});
				}
				divCarregado();
			}
		});
	}

    	
	function montaComboContratos(entid) 
	{
		$("#contratos").empty().append("<option value=''>Selecione...</option>");
		$("#ntfid").empty().append("<option value=''>Selecione...</option>");
		$("#valor_nota").text("");
		$("#dotvalordocumento").val("");
		
		if (entid.length <= 0) {
			return false;
		}
		$.ajax({
			url: window.location.href,
			type: "post",
			async: false,
			dataType: "json",
			data: {
				"requisicao": "carregarContratos",
				"entid": entid
			},
			beforeSend: function () {
				divCarregando();
			},
			success: function (data) {

				var selected = null;

				if (data.length > 0) {

					//selected = data.length == 1 ? "selected" : selected; //@todo

					$.each(data, function (chave, valor) {
						$("#contratos")
							.append($("<option " + selected + "></option>")
							.attr("value", valor.codigo)
							.text(valor.descricao)
							);
					});
				}
				divCarregado();
			}
		});
	}
	
	function montaNumeroDocumento(id) 
	{

		$("#ntfid").empty().append("<option value=''>Selecione...</option>");
		$("#valor_nota").text("");
		$("#dotvalordocumento").val("");

		if (id.length <= 0) {
			return false;
		}
		$.ajax({
			url: window.location.href,
			type: "post",
			async: false,
			dataType: "json",
			data: {
				"requisicao": "carregarNotas",
				"id": id,
                "arrNotas": arrIdNotas
			},
			beforeSend: function () {
				divCarregando();
			},
			success: function (data) 
			{
				var selected = null;

				if (data.length > 0) {

					//selected = data.length == 1 ? "selected" : selected; //@todo
					
					$.each(data, function (chave, valor) {
						$("#ntfid")
							.append($("<option " + selected + "></option>")
							.attr("value", valor.codigo)
							.text(valor.descricao)
							);
					});
				}
				divCarregado();
			}
		});
	}
	
    /* Se for a tela de visualização, não precisa recarregar a tela pai. */
    if (typeof(acaoVisualizar) === "undefined") {
        //window.onunload = refreshParent;
    }

    /**
     * Função responsável por atualizar a página com a lista de itens na tela de notas fiscais. Esta atualização é feita
     * logo após o cadastro de uma nota fiscal.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     */
    function refreshParent() {
        window.opener.location.reload();
    }
     
    function excluirArquivo(arqid){
         if(confirm('Você deseja realmente excluir este arquivo?')) {
        	 $("#edicao_arquivo").val(0);
             $("#arqidOld").val(arqid);
             $(".divArquivoDownload").hide("fast");
             $(".divArquivoUpload").show("fast");
         }
     }
    /**
     * Função responsável por atualizar a tabela de acompanhamento à medida que um novo documento é incluído.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {boolean}
     */
    function atualizarListaDocumentos() 
	{
		var retornoValidacao = validarDadosFormulario(true);
		//var retornoValidacao = true;

		if (retornoValidacao !== true) {
			alert(retornoValidacao);
			return false;
		}

		$("#empresaContratada").attr("disabled", true);
   		
		$("#acompanhamento").show();
		var contrato			= $("#contratos").val();
		var tdtid				= $("#tdtid").val();
		var ntfid 				= $("#ntfid").val();
		var valor_ntfid 		= $("#ntfid :selected").text();
		var valor_tdtid 		= $("#tdtid :selected").text();
		var valor_tdtid 		= valor_tdtid.trim();
		var valor_ntfid 		= valor_ntfid.trim();
		var valor_nota_label	= $("#valor_nota").html();
		var dotvalordocumento 	= $("#dotvalordocumento").val();
		var dotvalorpagodoc 	= $("#dotvalorpagodoc").val();

		$("#ntfid").val('');
		
  		// Trata a questão do arquivo do aditivo
        // Obj para tratar a questão das linhas da tabela
        objDocumento = {
			"id": idControleObjDocumento
		};
		
		listaObjDocumentos.push(objDocumento);
		arrIdNotas.push(ntfid + "-" + contrato);
		
		idControleObjDocumento++;
  		
		$("#tbAcompanhamento > tbody:last-child").append(
				"<tr>"
					+ "<td><span class='acaoTbAcompanhamento' onclick='removerDocumentoTabelaAcompanhamento(this, " + objDocumento.id + "," + ntfid + ", \"" + contrato + "\")'>" +
                    "<img src='/imagens/excluir.gif' title='Excluir Aditivo' alt='Excluir Aditivo'/></span></td>"
					+ "<td> <input type='hidden' name='contrato[]' 		value='"+contrato+"' > <input type='hidden' name='ntfid[]' 		value='"+ntfid+"' > " + valor_ntfid + "</td>"
					+ "<td> <input type='hidden' name='tdtid[]' 		value='"+tdtid+"' > " + valor_tdtid + "</td>"
					+ "<td> <input type='hidden' name='dotvalordocumento[]' value='"+dotvalordocumento+"' > " + valor_nota_label  + "</td>"
					+ "<td> <input type='hidden' name='dotvalorpagodoc[]' 		value='"+dotvalorpagodoc+"' > " + dotvalorpagodoc + "</td>"
				+ "</tr>"
		);

  		$("#contratos").val('');
        $("#ntfid").empty();
        $("#ntfid").append('<option  value="">Selecione...</option>');
  		$("#valor_nota").text('');
  		$("#dotvalordocumento").val('');
  		$("#dotvalorpagodoc").val('');
  		
		/*
				Contrato ou Aditivo de Valor:
		
		
		Tipo de Documento
		"tdtid"
		
		Nº do Documento:
		"ntfid" 
		
		Valor do Documento:
		"valor_nota" //span
		"dotvalordocumento" /hidden
		
		Valor Pago na Transação:
		"dotvalorpagodoc" // text
		*/
			
    }

    /**
     * Função responsável por remover da tabela de acompanhamento determinado documento.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param elemento
     * @param indexObjDocumento
     */
    function removerDocumentoTabelaAcompanhamento(elemento, indexObjDocumento, ntfid, contrato) {

        $(elemento).closest("tr").remove();
        listaObjDocumentos = listaObjDocumentos.filter(function(e) {
            return e.id != indexObjDocumento ;
        });

        if (listaObjDocumentos.length <= 0) {
            $("#acompanhamento").hide();
            $("#empresaContratada").attr("disabled", false);
        }

        var removeItem = ntfid + "-" + contrato;
        arrIdNotas.splice( $.inArray(removeItem, arrIdNotas), 1 );
    }
     
    function removerAditivoEdicao(elemento, indexObjDocumento, dotid, ntfid, contrato)
    {

    	$("#lista_excluidas_edicao").append(
                "<input type='hidden' name='documentos_excluidos[]' id='documentos_excluidos[]' value='"+dotid+"'> "
		);
    	excluiEdicao = true;
        $(elemento).closest("tr").remove();
        listaObjDocumentos = listaObjDocumentos.filter(function(e) {
            return e.id != indexObjDocumento ;
        });

        if (listaObjDocumentos.length <= 0) {
            $("#acompanhamento").hide();
            $("#empresaContratada").attr("disabled", false);
        }

        var removeItem = ntfid + "-" + contrato;
        arrIdNotas.splice( $.inArray(removeItem, arrIdNotas), 1 );
    }

    function editarAditivoEdicao(elemento, indexObjDocumento, dotid, numeroDoc, valorDoc, valorPago,  ntfid, contrato)
    {

        $("#lista_excluidas_edicao").append(
            "<input type='hidden' name='documentos_excluidos[]' id='documentos_excluidos[]' value='"+dotid+"'> "
        );
        excluiEdicao = true;
        $(elemento).closest("tr").remove();
        listaObjDocumentos = listaObjDocumentos.filter(function(e) {
            return e.id != indexObjDocumento ;
        });

        var removeItem = ntfid + "-" + contrato;
        arrIdNotas.splice( $.inArray(removeItem, arrIdNotas), 1 );

        montaComboContratosEdicao();
        $("#contratos>[value=" + contrato + "]").attr("selected", true);

        montaNumeroDocumento(contrato);
        $("#ntfid>[value=" + ntfid + "]").attr("selected", true);

        retornaValorNota(ntfid);


        valorPago = formatarValorMonetario(valorPago);
        $('#dotvalorpagodoc').val(valorPago);

        if (listaObjDocumentos.length <= 0) {
            $("#acompanhamento").hide();
            $("#empresaContratada").attr("disabled", false);
        }

    }

    /**
     * Função responsável por validar os campos do formulário para inclusão de pagamentos.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {*}
     */
    function validarDadosFormulario(validaAditivo) {

        var valorTransacao = $("#pgtvalortransacao").val();
        valorTransacao = valorTransacao.replace(".", "");
        valorTransacao = valorTransacao.replace(",", ".");
        
        var valorPagoDocumento = $("#dotvalorpagodoc").val();
        valorPagoDocumento = valorPagoDocumento.replace(".", "");
        valorPagoDocumento = valorPagoDocumento.replace(",", ".");
        
        	
        if ($("#pgtdtpagamento").val() == "") {
            return "Informe a Data da Transação Bancária.";
        }
        else if ($("#pgtnumtransacao").val() == "") {
            return "Informe o Nº da Transação Bancária.";
        }
        else if ($("#tptid").val() == "") {
            return "Selecione o Tipo de Pagamento.";
        }
        else if ($("#arquivo").val() == "" && ($("#edicao_arquivo").val() != 1)) 
        {
            return "Insira o arquivo da Transação Bancária.";
        }
        else if ($("#empresaContratada").val() == "") 
        {
            return "Selecione o Favorecido.";
        }
        else if (parseFloat(valorTransacao) <= 0 || ($("#pgtvalortransacao").val() == "")) 
		{
            return "O valor da transação deve ser maior que zero.";
        }

        if( validaAditivo )
        {
	        if ($("#contratos").val() == "") 
	        {
	        	return "Selecione o Contrato ou Aditivo de Valor.";
	        }
	        else if ($("#tdtid").val() == "") 
	        {
	        	return "Selecione o Tipo de Documento";
	        }
	        else if (parseFloat(valorPagoDocumento) <= 0 || ($("#dotvalorpagodoc").val() == "")) 
	        {
	        	return "Insira o Valor do Aditivo";
	        }
	        else if ($("#ntfid").val() == "") 
	        {
	        	return "Selecion o Nº do Documento";
	        }
	        else if ($("#dotvalordocumento").val() == "") 
	        {
	            return "Insira o Valor do Documento";
	        }
        }

        return true;
    }

    function validaValoresTotais()
    {
    	if( $("#edicao").val() == '1' )
		{
			return true;
		}
		
    	if(jQuery("input.[name=dotvalorpagodoc[]]").length > 0)
		{
        	total = 0
 			jQuery("input.[name=dotvalorpagodoc[]]").each(function()
 		 	{
 	 		 	var valor = $(this).val();
 	 		 	//alert(valor);
 				var valor = valor.replace(/\./gi,"");
 				var formatado = valor.replace(",",".");
 				formatado = parseFloat(formatado);
 				total  = total + formatado;
 				
 					
 			});
        	total = total.toFixed(2);
 			
 			var valor_transacao = $("#pgtvalortransacao").val();
 			valor_transacao = valor_transacao.replace(/\./gi,"");
 			valor_transacao = valor_transacao.replace(",",".");
 			valor_transacao = parseFloat(valor_transacao).toFixed(2);
 			
 			if( total != valor_transacao )
 			{
				alert('Falha ao cadastrar! O valor da Transação deve ser totalmente distribuido nos documentos adicionados.');
				return false;
 			}
 			else
 			{
				return true;
 			}
 			
 		}
    }

    /**
     * Função responsável por requisitar a gravação dos documentos da transação.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {boolean}
     */
    function salvarDocumento() {
		
      	
		if( idControleObjDocumento < 1  )
		{
			if( $("#edicao").val() == '1'  )
			{
				if ( jQuery("input.[name=dotid[]]").length <= 0 ) 
				{
					alert("É necessário inserir no mínimo um documento");
	            	return false;
				}
			}
			else
			{
				alert("É necessário inserir no mínimo um documento");
            	return false;
			}
		}
		
		if( ! validaValoresTotais() )
    	{
			return false;
    	}
		var confirmacao = true;
      	if(excluiEdicao)
    	{
	    	if( confirm('Foram excluídos documentos, confirma as modificações?') ){
	    		confirmacao = true;
	    	}
	    	else
	    	{
	    		confirmacao = false;
	    	}
    	}
		
		if(confirmacao)
		{
			if( $("#edicao").val() == '1' )
			{
				$("#requisicao").val("editar");
			}
	
	        var retornoValidacao = validarDadosFormulario(false);
			//var retornoValidacao = true;
			
	        if (retornoValidacao !== true) {
	            alert(retornoValidacao);
	            return false;
	        }
	        $("#empresaContratada").attr("disabled", false);
	        $('#formulario').submit();
		}
        return false;

    }

    /**
     * Funçao responsavel por verificar se o CNPJ fornecido está no formato válido.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=12078
     */
    function validarCPNJ(cnpj) {

        var retorno = false;

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            data: {
                "requisicao": "validarCNPJ",
                "cexnumcnpj": cnpj
            },
            success: function (data) {
                retorno = (data == "true") || false;
            }
        });

        return retorno;
    }

 	function downloadArquivo( arqid )
	{

		window.location='?modulo=principal/popUpConstrutoraExtraExecFinanceira&acao=O&download=S&arqid=' + arqid

	}

 	function esvaziaDadosCnpj()
	{
		$("#uf_fornecedor").html('');
		$("#razao_construtora").html('');
		$("#cexrazsocialconstrutora").val('');
        return true;
	}
	
    /**
     * Função para pesquisa de dados de um fornecedor por CNPJ.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=12078
     * @param pesquisa(boolean), quando acesso for de visualição passa true para pega atributo text do campo cnpj
     * caso inclusão passa false para pegar atributo val do campo cnpj
     * @returns {boolean}
     */
    function consultaCNPJFornecedor(pesquisa) {

        $("#razao_construtora").text("");
        $("#cexrazsocialconstrutora").val("");
        $("#uf_fornecedor").text("");

        if(pesquisa)
		{
            var cnpj = $("#cexnumcnpj").val();
            cnpj = cnpj.trim();
        }
        else
		{
            var cnpj = $("#cexnumcnpj").val();
        }

        if (cnpj.trim() == "") {
            alert("É obrigatório informar o CNPJ do fornecedor.");
            $("#cexnumcnpj").focus();
            return false;
        }

        if (!validarCPNJ(cnpj)) {
            alert("O CNPJ fornecido é inválido.");
            $("#cexnumcnpj").val("");
            $("#cexnumcnpj").focus();
            return false;
        }

        divCarregando();
        setTimeout(function () {
            var pessoaJudirica = new dCNPJ();
            pessoaJudirica.buscarDados(cnpj);
            if (pessoaJudirica.dados.no_empresarial_rf == "") {
                alert("Não há informações sobre o CNPJ informado em nossa base de dados.");
                return false;
            }
            $("#cexrazsocialconstrutora").val(pessoaJudirica.dados.no_empresarial_rf);
            $("#razao_construtora").text(pessoaJudirica.dados.no_empresarial_rf);
            $("#uf_fornecedor").text(pessoaJudirica.dados.sg_uf);
        }, 0);
        divCarregado();

        return false;
    }


</script>
