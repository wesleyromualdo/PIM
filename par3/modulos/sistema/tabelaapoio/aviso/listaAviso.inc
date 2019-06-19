<?php 
include_once APPRAIZ .'par3/classes/controller/Aviso_conteudo.class.inc';

$controllerAvisoConteudo = new Par3_Controller_Aviso_conteudo();

if ( $_REQUEST['requisicao'] ){
	ob_clean();
	
	echo $controllerAvisoConteudo->{$_REQUEST['requisicao']}( $_REQUEST );
	die;
}

$dados = $controllerAvisoConteudo->carregarAvisoConteudo();
extract($dados);

$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=aviso';
?>
<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Lista de aviso</h2>
			</div>
		</div>
	</div>
	<?php echo $simec->tab ( criaAbaPainelAviso(), $url );?>
	<div class="ibox-content">
		<?php $controllerAvisoConteudo->listarAvisoConteudo($arrPost); ?>		
	</div>
	<div class="ibox-footer">
	</div>
</div>
<?php 
$controllerAvisoConteudo->modalAvisoConteudo(false, true);
?>
<script type="text/javascript">
function editarAviso(actid){
	location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroAviso&actid=' + actid;
}

function excluirAviso(actid){
	swal({
		  title: "Confirmação",
		  text: "Deseja realmente apagar este aviso?",
		  type: "warning",
		  showCancelButton: true,
		  confirmButtonClass: "btn-danger",
		  confirmButtonText: "Apagar",
		  closeOnConfirm: false
		},
		function(){
		    jQuery('<form action="" method="POST"/>')
	        	.append(jQuery('<input type="hidden" name="actid" value="' + actid + '">'))
	        	.append(jQuery('<input type="hidden" name="requisicao" value="excluirAvisoConteudo">'))
	        	.appendTo(jQuery(document.body))
	        	.submit();
		});
}

function visualizarAviso(actid){
	jQuery('#conteudo-modal', jQuery("#modal_conteudo_aviso_exemplo")).html('');           
	jQuery("#modal_conteudo_aviso_exemplo").modal("show");
	
	jQuery.ajax({
        type: 	"POST",
        url: 	location.href,
        data: 	{requisicao:'pegarConteudoAviso', actid: actid},
        async: 	true,
        success: function (resp) {
 			jQuery('#conteudo-modal-aviso', jQuery("#modal_conteudo_aviso_exemplo")).html(resp);           
    	},
    	error: function(){
	        	swal({title: "Erro", text: "Falha ao buscar o aviso, tente novamente!", type: "error"},
				   function(){ 
						location.href = location.href;
				   }
				);
		}
    });
}


jQuery(document).ready(function (){
	jQuery('#modal_conteudo_aviso_exemplo').on('show.bs.modal', function () {
		setTimeout(function(){
			var w = (jQuery('#conteudo-modal-aviso', jQuery('#modal_conteudo_aviso_exemplo')).get(0).scrollWidth + 40);
			w = (w < 450 ? 450 : w);
		    jQuery('#modal_conteudo_aviso_exemplo').find('.modal-dialog').css({
		    	width		: w + 'px', //probably not needed
		        height		: 'auto', //probably not needed 
		        'max-height': '500px',
		        visibility	: 'visible'
			});
		}, 300);
	});
	
	jQuery("#modal_conteudo_aviso_exemplo").on('hidden.bs.modal', function (e) {
		jQuery(".modal-dialog", jQuery("#modal_conteudo_aviso_exemplo")).css({width:0, height:0}).css({visibility:'hidden'});
	});
});	
</script>


