<?php
	//Carrega parametros iniciais do simec
	include_once "controleInicio.inc";
	
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "includes/classes/Controle.class.inc";
	include_once APPRAIZ . "includes/classes/Visao.class.inc";
	include_once APPRAIZ . "includes/library/simec/Listagem.php";
	
	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';
	include_once 'autoload.php';
	
	include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
	
	initAutoload();
	
	$simec = new Simec_View_Helper();
	
	$_SESSION['sislayoutbootstrap'] = 'zimec';
	
	// -- Export de XLS automático da Listagem
	Simec_Listagem::monitorarExport($_SESSION['sisdiretorio']);

	verificaServicos();
	
	
	//Carrega as funções de controle de acesso
	include_once "controleAcesso.inc";
	$obReformulacao = new Par3_Controller_Reformulacao();
	if( $_REQUEST['requisicao'] == 'salvar-ciente-reformulaca' ){
	    ob_clean();
	    $obReformulacao->salvarCienciaNotificacao($_REQUEST);
	    die();
	}
	$obReformulacao->verificaNotificacaoReformulacao($_REQUEST['inuid'], true);
	
	include_once APPRAIZ .'par3/classes/controller/Aviso_conteudo.class.inc';
	$avisoConteudo = new Par3_Controller_Aviso_conteudo();
	$avisoConteudo->lancarAviso();
	
?>

<script type="text/javascript" src="js/par3.js"></script>
<link href="css/loading-par3.css" rel="stylesheet"/>
<script>
//script para corrigir o bug da popover
    $('[data-toggle="popover"]').popover();
    $('[data-toggle="popover"]').mouseleave(function(e) {
        e.stopPropagation();
        $(document).find('.popover').remove();
    });

//script para corrigir o bug da tooltip
    $('[data-toggle="tooltip"]').mouseleave(function(e) {
        e.stopPropagation();
        $(document).find('.tooltip').remove();
    });
</script>
<div class="loading-dialog-par3">
    <div class="loading-dialog-content-par3">
        <img src="../library/simec/img/loading.gif">
        <span>O sistema está processando as informações. <br/>Por favor aguarde um momento...</span>
    </div>
</div>