<?php
require APPRAIZ . 'obras2/includes/principal/listaEmpreendimentoEmpresa/funcoes.php';


if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}

$arOrgid = verificaAcessoEmOrgid();
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}

$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid                       = $_SESSION['obras2']['orgid'];
 

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

switch ( $_REQUEST['op'] ){
	case 'apagar':
		$empreendimento = new Empreendimento( $_POST['empid'] );
		if ( $empreendimento->empid ){
			$empreendimento->empstatus = 'I';
			$empreendimento->salvar();
		}
		$db->commit();
		die('<script type="text/javascript">
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/listaEmpreendimentos&acao=A\';
			 </script>');
	case 'abreVistoria':
		$_SESSION['obras2']['empid'] = $_GET['empid'];
		$_SESSION['obras2']['sosid'] = $_GET['sosid'];
		$_SESSION['obras2']['sueid'] = $_GET['sueid'];

		if ( $_SESSION['obras2']['sueid'] ){
			header('location: ?modulo=principal/cadVistoriaEmpresa&acao=E');
		}else{
			header('location: ?modulo=principal/cadVistoriaEmpresa&acao=A');
		}
		die;
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=utf-8');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;
}
 
include  APPRAIZ."includes/cabecalho.inc";

