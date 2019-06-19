<script type="text/javascript">
jQuery(document).ready(function (){
	jQuery('#modal_conteudo_aviso').on('show.bs.modal', function () {
		setTimeout(function(){
			var w = (jQuery('#conteudo-modal-aviso', jQuery('#modal_conteudo_aviso')).get(0).scrollWidth + 50);
			w = (w < 450 ? 450 : w);
// 		    jQuery('#modal_conteudo_aviso').find('.modal-dialog').style.width = w + 'px';
		    jQuery('#modal_conteudo_aviso').find('.modal-dialog').css({
		    	width		: w + 'px', //probably not needed
		        height		: 'auto', //probably not needed 
		        'max-height': '520px',
		        visibility	: 'visible'
			});
		}, 300);
	});
	
// 	$('#modal_conteudo_aviso').on('show.bs.modal', function () {
// 		var w = jQuery("#conteudo-modal-aviso", jQuery("#modal_conteudo_aviso")).innerWidth();
		
// 	       $(this).find('.modal-dialog').css({
// 	              width: w + 'px', //probably not needed
// 	              height:'auto', //probably not needed 
// 	              'max-height':'100%'
// 	       });
// 	});	
	
	var arAviso = <?php echo simec_json_encode($arAviso) ?>;
	var i = 0;
	jQuery("#modal_conteudo_aviso").on('hidden.bs.modal', function (e) {

		jQuery.post(location.href, {requisicao:'setarSessaoAvisoVisto', actid:jQuery("#actid-aviso", jQuery("#modal_conteudo_aviso")).val()});
		
		if (typeof arAviso[i] == 'object'){
			jQuery(".modal-dialog", jQuery("#modal_conteudo_aviso")).css({width:0, height:0}).css({visibility:'hidden'});
			
			jQuery("#conteudo-modal-aviso", jQuery("#modal_conteudo_aviso")).html(arAviso[i].actconteudo);
			jQuery("#actid-aviso", jQuery("#modal_conteudo_aviso")).val(arAviso[i].actid);

			if ( arAviso[i].actpermiteciente == 't' ){
				jQuery("#bnt-ciente", jQuery("#modal_conteudo_aviso")).removeClass('hidden');
			}else{
				jQuery("#bnt-ciente", jQuery("#modal_conteudo_aviso")).addClass('hidden');
			}
			
			jQuery("#modal_conteudo_aviso").modal("show");

			i++;
		}

	});

	jQuery("#bnt-ciente", jQuery("#modal_conteudo_aviso")).click(function (){
		jQuery.post(location.href, {requisicao:'setarCienteAvisoVisto', actid:jQuery("#actid-aviso", jQuery("#modal_conteudo_aviso")).val()});
		jQuery("#modal_conteudo_aviso").modal('hide');
	});
	
	jQuery("#modal_conteudo_aviso").modal("show");
});	
	
</script>
