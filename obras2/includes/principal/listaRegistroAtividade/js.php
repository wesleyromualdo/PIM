<script type="text/javascript">
	
    $(document).ready(function (){
        $('img[src="/imagens/estrela.png"]').parent().parent().parent().attr('style','background:#FFED32');
    });


	$(document).ready(function (){
		$('.download').click(function(){
			var id = $(this).attr('id');
			DownloadArquivo( id );
		});

	    $('.acoes-<?=$_SESSION['usucpf']?>').show();
	});

	// Função que abre a popup para inclusão
	function popupRegAtividades(){
		open('?modulo=principal/cadRegistroAtividade&acao='+ acao ,'inserirRegistro','width=800,height=500,toolbar=no,scrollbars=yes,status=yes');
	}

	function alterarReg( rgaid ){
		janela = window.open('?modulo=principal/cadRegistroAtividade&acao='+acao+'&rgaid=' + rgaid, 'inserirRegistro', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=500' ); 
		janela.focus();
	}

	function excluirReg( rgaid ){


		if ( confirm('Deseja apagar esta atividade?') ){
	            if(blockEdicao){
	                alert('Você não pode editar os dados da Obra Vinculada.');
	            }else{
			location.href = '?modulo=principal/listaRegistroAtividade&acao='+acao+'&rgaid=' + rgaid + '&requisicao=apagar';
	            }
		}
	}

    function retirarImportancia( rgaid ){
                     location.href = '?modulo=principal/listaRegistroAtividade&acao='+acao+'&rgaid=' + rgaid + '&requisicao=retirarimportancia';
    }
    function tornarImportante( rgaid ){
        location.href = '?modulo=principal/listaRegistroAtividade&acao='+acao+'&rgaid=' + rgaid + '&requisicao=tornarimportante';
    }

    function verReg( rgaid ){
    janela = window.open('?modulo=principal/verRegistroAtividade&acao=A&rgaid=' + rgaid, 'inserirRegistro', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=800,height=500' );
    janela.focus();
	}


	DownloadArquivo = function(arqid){
		window.location = '?modulo=principal/listaRegistroAtividade&acao='+acao+'&requisicao=download&arqid='+arqid;
	}



</script>