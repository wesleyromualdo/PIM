<?php
// LOCAL
error_reporting(E_ALL ^ E_NOTICE ^ E_DEPRECATED ^ E_STRICT);
ini_set('display_errors', 1);

define('IS_PRODUCAO', false);
define('IS_TREINAMENTO', false);
define('EXECUTAR_AUDITORIA', false);
defined('MAXONLINETIME') || define('MAXONLINETIME', 3600);

/** LAB */
$nome_bd = 'simec_sp';
$servidor_bd = '10.199.5.73';
$porta_bd = 5432;
$usuario_db = 'simec';
$senha_bd = 'pwdsimec';

/** DEV */
if ('simec_espelho_producao' == $_SESSION['baselogin']) {
//    $aConfigHmg = include APPRAIZ.'cecap/dev/app-config-file.php';
//    $aConfigHmg = $aConfigHmg['db_simec'];
//    $nome_bd     = $aConfigHmg['dbname'];
//    $servidor_bd = $aConfigHmg['host'];
//    $porta_bd    = $aConfigHmg['port'];
//    $usuario_db  = $aConfigHmg['user'];
//    $senha_bd    = $aConfigHmg['password'];
}

return [
    'db_pim' => [
        'host'     => $servidor_bd,
        'dbname'   => $nome_bd,
        'user'     => $usuario_db,
        'password' => $senha_bd,
        'port'     => $porta_bd
    ]
];
