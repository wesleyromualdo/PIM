<?php
// LOCAL
error_reporting(E_ALL ^ E_NOTICE ^ E_DEPRECATED ^ E_STRICT);
ini_set('display_errors', 1);

define('IS_PRODUCAO', false);
define('IS_TREINAMENTO', false);
define('IS_GRAVAR_FILE_CONTENT_MANAGER', true);
define('EXECUTAR_AUDITORIA', false);
defined('MAXONLINETIME') || define('MAXONLINETIME', 3600);
$so = strtoupper(substr(PHP_OS, 0, 3));

return [
    'name' => 'simec',
    'env' => 'dev',
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
        'host'     =>  '10.199.201.204',
        'dbname'   => 'simec_sp',
        'user'     => 'simec',
        'password' => 'pwdsimec',
        'port'     => 5432
    ]
];
