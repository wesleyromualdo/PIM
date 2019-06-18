<?php

error_reporting(E_ALL ^ E_NOTICE ^ E_DEPRECATED ^ E_STRICT);
ini_set('display_errors', 1);

global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $email_sistema;
/**
 * Configuracao PHP
 */
session_start();
date_default_timezone_set('America/Sao_Paulo');

defined('AUTHSSD') || define('AUTHSSD', false);
defined('APPRAIZ') || define('APPRAIZ', realpath(dirname(__FILE__) . '/../') . '/');
defined('DIRFILES') || define('DIRFILES', realpath(dirname(__FILE__) . '/../') . '/arquivos/');
defined('SISRAIZ') || define('SISRAIZ', APPRAIZ . $_SESSION['sisdiretorio'] . '/');

$aConfig = require getenv('APP_CONFIG_FILE') ? getenv('APP_CONFIG_FILE') : APPRAIZ . 'cecap/local/app-config-file.php';

$_SESSION['ambiente'] = IS_PRODUCAO ? 'Ambiente de Producao' : 'Ambiente de Desenvolvimento';
$_SESSION['sisbaselogin'] = $_REQUEST['baselogin'] && $_REQUEST['baselogin'] != "teste" ? $_REQUEST['baselogin'] : $_SESSION['sisbaselogin'];
$_SESSION['baselogin'] = $_REQUEST['baselogin'] ? $_REQUEST['baselogin'] : $_SESSION['baselogin'];

$servidor_bd = $aConfig['db_pim']['host'];
$porta_bd = $aConfig['db_pim']['port'];
$nome_bd = $aConfig['db_pim']['dbname'];
$usuario_db = $aConfig['db_pim']['user'];
$senha_bd = $aConfig['db_pim']['password'];

//$configDbCoorporativo   = new stdClass();
//
//$configDbCoorporativo->host = '10.199.5.73';
//$configDbCoorporativo->port = '5432';
//$configDbCoorporativo->dbname = 'dbcorporativo_sp';
//$configDbCoorporativo->user = 'simec';
//$configDbCoorporativo->password = 'pwdsimec';


include_once APPRAIZ . 'includes/simec_funcoes.inc';
include_once APPRAIZ . 'includes/backtrace/BackTrace.php';
include_once APPRAIZ . 'includes/backtrace/BackTraceExplain.php';
include_once APPRAIZ . 'includes/failure/ErrorHandler.php';
include_once APPRAIZ . 'includes/failure/ExceptionHandler.php';
include_once APPRAIZ . 'includes/connection/adapter-connection.php';

function fechar_conexoes()
{
//    while (pg_ping()) {
//        pg_close();
//    }
}

if (PHP_MAJOR_VERSION >= 7) {
    set_error_handler(function ($errno, $errstr) {
        return strpos($errstr, 'Declaration of') === 0;
    }, E_WARNING);
}

register_shutdown_function('fechar_conexoes');
//ErrorHandler::start();
//ExceptionHandler::start();
ob_start();
?>
