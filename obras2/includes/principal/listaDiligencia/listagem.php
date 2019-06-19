<?php
$diligencia = new Diligencia();
if ( $_GET['acao'] == 'A' ){
	$param = array('empid' => $_SESSION['obras2']['empid']);
}else{
	$param = array('obrid' => $_SESSION['obras2']['obrid']);
}

if ( $habilitado == false ){
	$param['block_imgs_acao'] = true;
}else{
	$param['block_imgs_acao'] = false;
}
$sql 	   = $diligencia->listaSql( $param );

$cabecalho = array("Ação", "Providência", "ID Item",  "Fase","Tipo", "Data da Inclusão", "Descrição", "Providência", "Previsão da Providência", "Criado Por", "Superação", "Situação Atual", "Superado Por", "Ressalva", "Último Tramite", "Data do Último Tramite");
$db->monta_lista($sql,$cabecalho,100,5,'N','center','N', 'N');
