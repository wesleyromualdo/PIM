<?php 
$param = array("obrid" => $obrid, "not(img)" => true);
if ( $habilitado == false || $blockEdicao){
	$param['block_img_excluir'] = true;
}

$sql = $obrasArquivos->listaSql( $param );

if( $_REQUEST['ordemlista'] == 4 ){ // ordenacao pelo nome do arquivo

	$_SESSION['obras2']['order_extra']['campo'] = 'a.arqnome';

	if( $_SESSION['obras2']['order_extra']['campo'] == 'a.arqnome' && $_SESSION['obras2']['order_extra']['ordem'] == 'ASC' )
		$_SESSION['obras2']['order_extra']['ordem'] = 'DESC';
	else
		$_SESSION['obras2']['order_extra']['ordem'] = 'ASC';

	$_REQUEST['ordemlista'] = $_SESSION['obras2']['order_extra']['campo'];
	$_REQUEST['ordemlistadir'] = $_SESSION['obras2']['order_extra']['ordem'];

}else if( $_REQUEST['ordemlista'] == 5 ){ // ordenacao pela data

	$_REQUEST['ordemlista'] = 'oardtinclusao';
	$_SESSION['obras2']['order_extra']['campo'] = 'oardtinclusao';
	if( $_SESSION['obras2']['order_extra']['campo'] == 'a.arqnome' && $_SESSION['obras2']['order_extra']['ordem'] == 'ASC' )
		$_SESSION['obras2']['order_extra']['ordem'] = 'DESC';
	else
		$_SESSION['obras2']['order_extra']['ordem'] = 'ASC';

	$_REQUEST['ordemlista'] = $_SESSION['obras2']['order_extra']['campo'];
	$_REQUEST['ordemlistadir'] = $_SESSION['obras2']['order_extra']['ordem'];

}


$cabecalho 	= array("Ação", "Tipo Arquivo", "Descrição", "Arquivo", "Data de Inclusão", "Gravado por");
$db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2, "formulario");
