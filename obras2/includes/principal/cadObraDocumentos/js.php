<script type="text/javascript">
 
$(document).ready(function(){
	$('[type="text"]').keyup();
	$('.incluir').click(function(){
		var stop = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				stop = true;
				alert('Campo obrigatório.');
				$(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		$('#req').val('salvar');
		$('#formObraArquivos').submit();
	});
	$('.download').click(function(){
		$('#req').val('download');
		$('#arqid').val( $(this).attr('id') );
		$('#formObraArquivos').submit();
	});
	$('.excluir').click(function(){
		if ( confirm('Deseja apagar o documento?') ){
			$('#req').val('excluir');
			$('#oarid').val( $(this).attr('id') );
			$('#formObraArquivos').submit();
		}
	});

    $('img[src="/imagens/estrela.png"]').closest('tr').attr('style','background:#FFED32');
});

function retirarImportancia( rgaid ){
    location.href = '?modulo=principal/cadObraDocumentos&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=retirarimportancia';
}
function tornarImportante( rgaid ){
    location.href = '?modulo=principal/cadObraDocumentos&acao=<?php echo $_GET['acao'] ?>&rgaid=' + rgaid + '&requisicao=tornarimportante';
}
</script>
