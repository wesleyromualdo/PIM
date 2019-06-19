<?php 
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

if( $_REQUEST['acao'] == 'download'){

    $helperPendencia->recuperaArquivoPNE( $_REQUEST['inuid'] );

}

$pendenciaBox = $_REQUEST['pendencia'];
$boExists = $this->consultaArquivoPNE( $_REQUEST['inuid'] );

$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, $boExists);

$instrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$itrid = $instrumentoUnidade->itrid;
if( $itrid == '2'){
	$arAuxHelperPendencia['description'] = "Plano Municipal de Educação";
}else{
	$arAuxHelperPendencia['description'] = "Plano Estadual de Educação";
}
$arAuxHelperPendencia['nomeEntidade'] = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

echo simec_json_encode($arAuxHelperPendencia);
die();
?>