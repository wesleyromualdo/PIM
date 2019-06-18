<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

carregaIniciativaDetalhe();
carregaIniciativaDetalheEntidade();
carregaEmendapariniciativa();

function carregaIniciativaDetalhe(){
	global $db;
	
	$sql = "SELECT DISTINCT
				ee.emecod, ed.emdid
			FROM
				emenda.emenda ee
				inner join emenda.emendadetalhe ed ON ed.emeid = ee.emeid and emdstatus = 'A'
			WHERE
			    ee.emeano = '2017'
			    and ee.emetipo = 'E'
			 order by ee.emecod";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	foreach ($arrDados as $key => $v) {
	
		$sql = "SELECT DISTINCT
					ed.emdid, ie.iniid, ie.iedstatus
				FROM
					emenda.emenda ee
					inner join emenda.emendadetalhe ed ON ed.emeid = ee.emeid and emdstatus = 'A' 
				    inner join emenda.iniciativaemendadetalhe ie on ie.emdid = ed.emdid and ie.iedstatus = 'A'
				WHERE 
				    ee.emecod = '{$v['emecod']}'
				    and ee.emeano in (2016, 2015, 2014, 2013, 2012)
				    and ee.emetipo = 'E'";
		$arrIni = $db->carregar($sql);
		$arrIni = $arrIni ? $arrIni : array();
		
		foreach ($arrIni as $ini) {
			$sql = "select count(iedid) from emenda.iniciativaemendadetalhe where iniid = {$ini['iniid']} and emdid = {$v['emdid']} and iedstatus = 'A'";
			$boTem = $db->pegaUm($sql);
			
			if( (int)$boTem < (int)1 ){
				$sql = "INSERT INTO emenda.iniciativaemendadetalhe(emdid, iniid, iedstatus) values({$v['emdid']}, {$ini['iniid']}, 'A')";
				$db->executar($sql);
			}
		}
		$db->commit();
	}
}

function carregaIniciativaDetalheEntidade(){
	global $db;
	
	$sql = "SELECT DISTINCT 
				ee.emecod, ed.emdid, ede.edeid, eb.enbcnpj
			FROM
				emenda.emenda ee
				inner join emenda.emendadetalhe ed ON ed.emeid = ee.emeid and emdstatus = 'A' 
	            inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
	            inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid and eb.enbstatus = 'A'
			WHERE 
			    ee.emeano = '2017'
			    and ee.emetipo = 'E' 
				--and ee.emecod = '29360007'
			 order by ee.emecod";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();

	foreach ($arrDados as $key => $v) {
		//INSERT INTO emenda.iniciativadetalheentidade(edeid, iniid, idestatus)
		$sql = "SELECT DISTINCT 
				    ede.edeid, iee.iniid, iee.idestatus
				FROM
				    emenda.emenda ee
				    inner join emenda.emendadetalhe ed ON ed.emeid = ee.emeid and emdstatus = 'A'
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid and eb.enbstatus = 'A'
				    inner join emenda.iniciativadetalheentidade iee on iee.edeid = ede.edeid and iee.idestatus = 'A'
				WHERE 
				    ee.emeano in (2016, 2015, 2014, 2013, 2012)
				    and ee.emetipo = 'E'
				    and ee.emecod = '{$v['emecod']}'
				    and eb.enbcnpj = '{$v['enbcnpj']}'";
		$arrIni = $db->carregar($sql);
		$arrIni = $arrIni ? $arrIni : array();
		
		foreach ($arrIni as $ini) {
			$sql = "select count(ideid) from emenda.iniciativadetalheentidade where iniid = {$ini['iniid']} and edeid = {$v['edeid']} and idestatus = 'A'";
			$boTem = $db->pegaUm($sql);
				
			if( (int)$boTem < (int)1 ){
				$sql = "INSERT INTO emenda.iniciativadetalheentidade(edeid, iniid, idestatus) values({$v['edeid']}, {$ini['iniid']}, 'A')";
				$db->executar($sql);
			}
		}
		$db->commit();
	}	
}

function carregaEmendapariniciativa(){
	global $db;
	
	$sql = "SELECT DISTINCT 
				ee.emecod, ed.emdid, ede.edeid, eb.enbcnpj
			FROM
				emenda.emenda ee
				inner join emenda.emendadetalhe ed ON ed.emeid = ee.emeid and emdstatus = 'A' 
	            inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
	            inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid and eb.enbstatus = 'A'
			WHERE 
			    ee.emeano = '2017'
			    and ee.emetipo = 'E'
			 order by ee.emecod";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();

	foreach ($arrDados as $key => $v) {

		$sql = "SELECT DISTINCT 
				    ede.edeid, iee.iniid, iee.ppsid, iee.epistatus
				FROM
				    emenda.emenda ee
				    inner join emenda.emendadetalhe ed ON ed.emeid = ee.emeid and emdstatus = 'A'
				    inner join emenda.iniciativaemendadetalhe ie on ie.emdid = ed.emdid and ie.iedstatus = 'A'
				    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
				    inner join emenda.entidadebeneficiada eb on eb.enbid = ede.enbid and eb.enbstatus = 'A'
				    inner join emenda.emendapariniciativa iee on iee.edeid = ede.edeid and iee.epistatus = 'A'
				WHERE 
				    ee.emeano in (2016, 2015, 2014, 2013)
				    and ee.emetipo = 'E'
				    and ee.emecod = '{$v['emecod']}'
			      	and eb.enbcnpj = '{$v['enbcnpj']}'";
		$arrIni = $db->carregar($sql);
		$arrIni = $arrIni ? $arrIni : array();
		
		foreach ($arrIni as $ini) {
			$sql = "select count(epiid) from emenda.emendapariniciativa where ppsid = {$ini['ppsid']} and iniid = {$ini['iniid']} and edeid = {$v['edeid']} and epistatus = 'A'";
			$boTem = $db->pegaUm($sql);
				
			if( (int)$boTem < (int)1 ){
				$sql = "INSERT INTO emenda.emendapariniciativa(edeid, iniid, ppsid, epistatus) values({$v['edeid']}, {$ini['iniid']}, {$ini['ppsid']}, 'A')";
				$db->executar($sql);
			}
		}
		$db->commit();
	}	
}
