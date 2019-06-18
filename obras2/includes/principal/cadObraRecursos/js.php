<script type="text/javascript">
    $(function () {
        $('#tooid').change(function () {
            if($(this).val() == '2'){
                $('#convenio').show();
            } else {
                $('#convenio').hide();
            }
        });
    });

function abreCamposRecursos(id){
	var tr_convenio  	= document.getElementById( 'convenio' );
	var tr_descentralizacao = document.getElementById( 'descentralizacao' );
	var tr_rec  	        = document.getElementById( 'recurso' );

	if(tr_descentralizacao && tr_rec) {
		if(id == ''){
			if (document.selection){
				tr_descentralizacao.style.display 	= 'none';
				tr_rec.style.display 			= 'none';
			}else{
				tr_descentralizacao.style.display 	= 'none';
				tr_rec.style.display 			= 'none';
			}
		}

		if(id == <?php echo FRPID_DESCENTRALIZACAO ?>){
			if (document.selection){
				tr_descentralizacao.style.display = 'block';
				tr_rec.style.display 		  = 'none';
			}else{
				tr_descentralizacao.style.display = 'table-row';
				tr_rec.style.display 		  = 'none';
			}
		}

		if(id == <?php echo FRPID_RECURSO_PROPRIO ?>){
			if (document.selection){
				tr_descentralizacao.style.display = 'none';
				tr_rec.style.display 		  = 'block';
			}else{
				tr_descentralizacao.style.display = 'none';
				tr_rec.style.display 		  = 'table-row';
			}
		}
	}
}

$(document).ready(function(){

	abreCamposRecursos(<?php echo $frpid ?>);

	$('[type="text"]').keyup();

	$('#salvar').click(function(){
		var stop = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				stop = true;
				alert('Preencha todos os campos obrigatórios.');
				$(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		$('#req').val('salvar');
		$('#formRecursos').submit();
	});

});
</script>


