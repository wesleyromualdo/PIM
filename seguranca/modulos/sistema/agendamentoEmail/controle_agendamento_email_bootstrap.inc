<?php 

$urlPadrao  = "?modulo={$_GET['modulo']}&acao={$_GET['acao']}";
$urlComMenu = ($_GET['menu'] 
				? "?modulo={$_GET['modulo']}&acao={$_GET['acao']}&menu={$_GET['menu']}" 
				: "?modulo={$_GET['modulo']}&acao={$_GET['acao']}");
$urlCompleta  = "?{$_SERVER['QUERY_STRING']}";

switch ($_GET['menu']){
	case 'cadastroAgendamento':
		include APPRAIZ .'seguranca/modulos/sistema/agendamentoEmail/cadastro_agendamento_email_bootstrap.inc';
		break;
	default:
		include APPRAIZ .'seguranca/modulos/sistema/agendamentoEmail/lista_agendamento_email_bootstrap.inc';
		break;;
}
?>
<script type="text/javascript">
jQuery(document).ready(function (){
	jQuery('.nav-tabs li a').each(function (){
		var href = jQuery(this).attr('href');
		var menuVazio = ('<?php echo $_GET['menu']; ?>' == '');
		
		if ( menuVazio == true && href.indexOf('menu') == -1 ){
			jQuery(this).parents('li:first').addClass('active');
		}else if ( menuVazio == false &&  href.indexOf('<?php echo $_GET['menu']; ?>') > -1 ){
			jQuery(this).parents('li:first').addClass('active');
		}else{
			jQuery(this).parents('li:first').removeClass('active');
		}
	});
});
</script>
