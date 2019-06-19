function vizualizarTermo(id)
{
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "&req=vizualizaDocumento&dopid="+id,
   		async: false,
   		success: function(msg){
			$('#html_modal-form').html(msg);
			$('#html_modal-form').css('overflow', 'auto');
		    $('#modal-form').modal();
   		}
	});
}

function vizualizarTermoPAC(id)
{
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "&req=vizualizaDocumentoPAC&terid="+id,
		async: false,
		success: function(msg){
			$('#html_modal-form').html(msg);
			$('#html_modal-form').css('overflow', 'auto');
			$('#modal-form').modal();
		}
	});
}

function baixarArquivoDopid(dopid)
{
	$(window).attr('location', window.location.href + '&req=baixarArquivoDopid&dopid=' + dopid);
}