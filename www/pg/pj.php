<?php  
	$_POST['ajaxPJ'] =  "46162178000130";
	
	include_once 'config.inc';

	require_once APPRAIZ . "/includes/corporativo/service/entidades_juridicaService.php";

	$pj = str_replace(array('/', '.', '-'), '', $_POST['ajaxPJ']);
	$svcPj = new entidades_juridicaService();
	$pj = $svcPj->getByCnpj($pj);

	$dadoLgdo = $svcPj->parseLegado($pj);
	$dadoJson = $svcPj->parseJSON($pj);

	var_dump($dadoLgdo);
	var_dump($dadoJson);

