<?php 
require APPRAIZ . 'obras2/includes/principal/cadObraFotos/funcoes.php';




if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];
if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
	$db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
}elseif( $_SESSION['obras2']['obrid'] ){
	if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
	}
}else{
	$db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
}

$obrasArquivos 	= new ObrasArquivos();
$orgid 			= $_SESSION['obras2']['orgid'];

//$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
//$empreendimento->montaCabecalho();
echo cabecalhoObra($obrid);
echo "<br>";
//monta_titulo( 'Galeria de Fotos', '' );
monta_titulo_obras( 'Galeria de Fotos', '' );

$habilitado = true;
$habilita = 'S';

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita = 'N';
}