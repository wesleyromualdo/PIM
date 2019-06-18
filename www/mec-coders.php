<?php
defined('APPRAIZ') || define('APPRAIZ', realpath(dirname(__FILE__) . '/../') . '/');
include APPRAIZ . 'src/Simec/Simec.php';

\Simec\Simec::liberarArquivoPublico(isset($_GET['file']) ? $_GET['file'] : '');
