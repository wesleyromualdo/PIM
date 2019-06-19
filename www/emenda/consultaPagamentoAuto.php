<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

// carrega as funções específicas do módulo
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "emenda/classes/LogErroWS.class.inc";

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

//$_SESSION['sisid'] = '57';
//$_SESSION['suscod'] = 'A';
//$_SESSION['sisdiretorio'] = 'emenda';
//$_SESSION['sisarquivo'] = 'emenda';
//$_SESSION['sisdsc'] = 'Módulo de Emendas';
//$_SESSION['exercicio_atual'] = date('Y');
//$_SESSION['exercicio'] = date('Y');

$db = new cls_banco();

$arDados = array();
$arDados['usuario'] = 'MECPRISCILA';
$arDados['senha']   = '74496385';

$obEmpenho = new WSEmpenho($db);
$obEmpenho->consultarPagamento($arDados, false, true);
unset($obEmpenho);
?>