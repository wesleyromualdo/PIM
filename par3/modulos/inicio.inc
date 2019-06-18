<?php

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
$parInicio = new Par3_Model_Inicio();
$url = $parInicio->pegarInicioPorCpf($_SESSION['usucpf']);
header( "Location: ".$url );

?>