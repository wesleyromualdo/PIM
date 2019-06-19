<?php
// LOCAL
error_reporting(E_ALL ^ E_NOTICE ^ E_DEPRECATED ^ E_STRICT);
ini_set('display_errors', 1);

define('IS_PRODUCAO', false);
define('IS_TREINAMENTO', false);
define('IS_GRAVAR_FILE_CONTENT_MANAGER', true);
define('EXECUTAR_AUDITORIA', false);
defined('MAXONLINETIME') || define('MAXONLINETIME', 3600);

/** LAB */
$nome_bd = 'simec_sp';
$servidor_bd = '10.199.5.73';
$porta_bd = 5432;
$usuario_db = 'simec';
$senha_bd = 'pwdsimec';
$so = strtoupper(substr(PHP_OS, 0, 3));

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
    'name' => 'simec',
    'env' => 'lab',
    'url' => 'http://simec-dsv/',
    'timezone' => 'America/Sao_Paulo',
    'locale' => 'pt-br',
    'fallback_locale' => 'pt_br',
    'key' => 'base64:CJNBSMnbHiagk5Qi0TtMWJEKqQLAdWGQMlUshhhTNBA=',
    'cipher' => 'AES-256-CBC',
    'debug' => true,
    'log' => 'single',
    'log_level' => 'debug',
    'log_max_files' => 1,
    'cache_driver'=> 'file',
    'cache_dir'=> ($so === 'WIN' ?  'C:\\tmp\\cache' : '/tmp/cache' ),
    'session_dir'=> ($so === 'WIN' ?  'C:\\tmp\\session' : '/tmp/session' ),
    'view_dir'=> ($so === 'WIN' ?  'C:\\tmp\\view' : '/tmp/view' ),
    'lifetime'=> 120,
    'JWT_SECRET'=> 'WSfEN6A84RVVQIucvXcAqv6ITOD2wL9W',
    'JWT_BLACKLIST_GRACE_PERIOD'=> 30,
    'db_pim' => [
        'host'     => $servidor_bd,
        'dbname'   => $nome_bd,
        'user'     => $usuario_db,
        'password' => $senha_bd,
        'port'     => $porta_bd
    ]
];
