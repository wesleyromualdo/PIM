<?php 
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

$pendenciaBox = $_REQUEST['pendencia'];
$boExists = $this->carregaPendenciaConselhoCAE( $_REQUEST['inuid'] );
$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, $boExists);
$arAuxHelperPendencia['nomeEntidade'] = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

echo simec_json_encode($arAuxHelperPendencia);
die();
?>