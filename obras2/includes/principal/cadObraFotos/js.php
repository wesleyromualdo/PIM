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
		if( confirm('Deseja excluir esta foto?') ){
			$('#req').val('excluir');
			$('#oarid').val( $(this).attr('id') );
			$('#formObraArquivos').submit();
		}
	});
});

function abrirGaleria(arqid, schema)
{
	window.open("../slideshow/slideshow/obras2_galeriaGaleriaFotos.php?tipo=abaGaleria&arqid=" + arqid ,"imagem","width=850,height=600,resizable=yes");
}
</script>


<script type="text/javascript">
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(600,150);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}
</script>


