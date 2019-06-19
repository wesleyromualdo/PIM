<?php

if (!$_POST['ajaxPJ']){

 $html =<<<HTML
 	<script type="text/javascript" src="/includes/JQuery/jquery-1.7.2.min.js"></script>
	<script src="/includes/webservice/pj_jQuery.js"></script>
HTML;

	echo $html;
}

include_once 'config.inc';

if ($_POST['ajaxPJ']){

	require_once APPRAIZ . "/includes/corporativo/service/entidades_juridicaService.php";

	$pj = str_replace(array('/', '.', '-'), '', $_POST['ajaxPJ']);
	$svcPJ = new  entidades_juridicaService();
	$pj = $svcPJ->getByCnpj($pj);
	$dadoLgdo = $svcPJ->parseLegado($pj);


	die($dadoLgdo);
}
