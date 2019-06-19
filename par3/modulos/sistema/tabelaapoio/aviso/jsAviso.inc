<script type="text/javascript">
$1_11(document).ready(function (){
	var arAviso = <?php echo simec_json_encode($arAviso) ?>;
	var i = 0;
	
	$1_11('#modal_conteudo_aviso').dialog({
		modal : true,
		open : function ()
		{
			$this = $1_11(this);
			setTimeout(function ()
			{			
    			var w = ($1_11('#conteudo-modal-aviso').get(0).scrollWidth + 50);
    			w = (w < 450 ? 450 : w);
    			
    			$btnCiente = $1_11("#btn_ciente", $1_11("#modal_conteudo_aviso").closest('.ui-dialog'));
    			if ($1_11('#actpermiteciente-aviso').val() == 't') {
    				$btnCiente.removeClass('hidden');
    			} else {
    				$btnCiente.addClass('hidden');
    			}
    			
    			$this.dialog({
    	    		resizable	: true,
    				title		: 'Aviso',
    				width 		: w,
    				height		: 'auto',
    				maxHeight 	: 600,
    				dialogClass : "dialog-modal-aviso",
    				position	: ['middle', 60]
    			});
			}, 1);
		},
		close : function () 
		{
			$1_11.post(location.href, {requisicao:'setarSessaoAvisoVisto', actid:$1_11("#actid-aviso", $1_11("#modal_conteudo_aviso")).val()});
			
			if (typeof arAviso[i] == 'object') {
				$1_11('#modal_conteudo_aviso').dialog({width:0, height:0});
// 				$1_11(".modal-dialog", $1_11("#modal_conteudo_aviso")).css({width:0, height:0}).css({visibility:'hidden'});
				
				$1_11("#conteudo-modal-aviso", $1_11("#modal_conteudo_aviso")).html(arAviso[i].actconteudo);
				$1_11("#actid-aviso", $1_11("#modal_conteudo_aviso")).val(arAviso[i].actid);

				$btnCiente = $1_11("#btn_ciente", $1_11("#modal_conteudo_aviso").closest('.ui-dialog'));
				if (arAviso[i].actpermiteciente == 't') {
					$btnCiente.removeClass('hidden');
				} else {
					$btnCiente.addClass('hidden');
				}
				$1_11('#actpermiteciente-aviso').val(arAviso[i].actpermiteciente);	
				
				$1_11("#modal_conteudo_aviso").dialog("open");

				i++;
			}
		},
		buttons : [
			{
    			text : 'Fechar',
//     			icon : 'fa fa-times-circle-o',
    			click: function ()
    			{
					$1_11(this).dialog('close');
    			}
			},
			{
				text 	: 'Ciente',
				id		: 'btn_ciente',
				click	: function ()
				{
					$1_11.post(location.href, {requisicao:'setarCienteAvisoVisto', actid:$1_11("#actid-aviso", $1_11("#modal_conteudo_aviso")).val()});
					$1_11("#modal_conteudo_aviso").dialog('close');
				}
			}
		]
	});

	$1_11("#modal_conteudo_aviso").dialog();
});	
	
</script>
