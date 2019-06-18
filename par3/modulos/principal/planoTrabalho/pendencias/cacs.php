<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

$pendenciaBox = $_REQUEST['pendencia'];
$boExists = $helperPendencia->consultaPendenciaCACS($_REQUEST['inuid']);

$GLOBALS['nome_bd'] = 'simec_desenvolvimento';
$GLOBALS['servidor_bd'] = '192.168.222.45';
$GLOBALS['porta_bd'] = '5433';
$GLOBALS['usuario_db'] = 'simec';
$GLOBALS['senha_bd'] = 'phpsimecao';
$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, $boExists);
$arAuxHelperPendencia['nomeEntidade'] = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

echo simec_json_encode($arAuxHelperPendencia);
die();
?>