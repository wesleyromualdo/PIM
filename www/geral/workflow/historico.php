<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . 'includes/workflow.php';

$db = new cls_banco();

if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao']) && ($_SESSION['sislayoutbootstrap'] == 't' || $_SESSION['sislayoutbootstrap'] == 'zimec') ) {
    include_once 'view_historico_new.php';
}else{
    include_once 'view_historico_old.php';
}
