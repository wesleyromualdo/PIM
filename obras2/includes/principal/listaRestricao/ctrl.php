<?php 
require APPRAIZ . 'obras2/includes/principal/listaRestricao/funcoes.php';


$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];


if($_REQUEST['form'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls'){
    ini_set("memory_limit", "7000M");
    montaLista($_REQUEST['tipo_relatorio']);
    exit;
}


if(empty($_SESSION['obras2']['empid'])){
    $objObr = new Obras();
    $objObr->carregarPorIdCache($obrid);
    $empid =  $objObr->empid;
    $_SESSION['obras2']['empid'] = $empid;
}else{
    $empid =  $_SESSION['obras2']['empid'];
}



