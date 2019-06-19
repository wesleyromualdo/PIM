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

$sql = "SELECT exercicio, emenda, uo, partido, gnd, fonte, mod, valor_emenda, 
		       cnpj, valor_rcl, valor_disponivel
		FROM carga.base_siop_2525";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrRegistroEmenda = array();
$arrRegistroEntidade = array();

$totalCNPJ = array();
$totalValor = array();
foreach ($arrDados as $v) {
	
	$sql = "select ed.emdid
			from emenda.emenda e
			  inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
			where e.emeano = '".date('Y')."'
				and e.emecod = '{$v['emenda']}'
			    and ed.gndcod = '{$v['gnd']}'
			    and ed.mapcod = '{$v['mod']}'
			    and ed.foncod = '{$v['fonte']}'";
	$emdid = $db->pegaUm($sql);
	
	if( empty($emdid) ){
		$emeid = $db->pegaUm("select emeid from emenda.emenda where emeano = '".date('Y')."' and emecod = '{$v['emenda']}'");
		$sql = "INSERT INTO emenda.emendadetalhe(emeid, gndcod, foncod, mapcod, emdvalor, emdtipo, emdimpositiva)
				VALUES ($emeid, '{$v['gnd']}', '{$v['fonte']}', '{$v['mod']}', '{$v['valor_emenda']}', 'E', null) returning emdid";
		$emdid = $db->pegaUm($sql);
		$db->commit();
		array_push($arrRegistroEmenda, $v);
	}
	
	if( (int)strlen($v['cnpj']) == (int)14 ){
		$sql = "select ede.edeid, ede.edevalordisponivel
				from emenda.emendadetalheentidade ede
					inner join emenda.entidadebeneficiada enb on enb.enbid = ede.enbid and enb.enbstatus = 'A'
				where
				    ede.edestatus = 'A'
				    and ede.emdid = $emdid
					and enb.enbcnpj = '{$v['cnpj']}'";
		$arrEntidade = $db->pegaLinha($sql);
		
		$db->executar("update emenda.emenda set ememomentosiop = 2526 where emecod = '{$v['emenda']}' and emeano = '".date('Y')."'");
		
		if( (float)$arrEntidade['edevalordisponivel'] <> (float)$v['valor_disponivel'] ){
			
			if( !empty($arrEntidade['edeid']) ){
				$sql = "UPDATE emenda.emendadetalheentidade
						SET edevalor = ".(float)$v['valor_rcl'].",
							edevalordisponivel = ".(float)$v['valor_disponivel']."
						WHERE edeid = {$arrEntidade['edeid']}";
				$db->executar($sql);
				$db->commit();
				array_push($totalValor, array('emdid' => $emdid, 'emenda' => $v['emenda'], 'valorEntidade' => $arrEntidade['edevalordisponivel'], 'valor_disponivel' => $v['valor_disponivel'], 'cnpj' => $v['cnpj'], 'sql' => $sql));
			}
		}
		
		if( empty($arrEntidade['edeid']) ){
			$v['emdid'] = $emdid;
			array_push($arrRegistroEntidade, $v);
			if( $v['cnpj'] ){
				$sql = "SELECT enbid FROM emenda.entidadebeneficiada WHERE enbstatus = 'A' and enbcnpj = '{$v['cnpj']}' and enbano = '".date('Y')."'";
				$enbid = $db->pegaUm($sql);
				
				if( empty($enbid) ){
					$sql = "SELECT enbnome, muncod, estuf FROM emenda.entidadebeneficiada WHERE enbcnpj = '{$v['cnpj']}'";
					$arEntidade = $db->pegaLinha($sql);
					
					$sql = "INSERT INTO emenda.entidadebeneficiada(enbstatus, enbano, enbdataalteracao, enbnome, enbcnpj, muncod, estuf)
    						VALUES ('A',
	    						'".date('Y')."',
	    						NOW(),
	    						'".$arEntidade['enbnome']."',
	    						'".$v['cnpj']."',
	    						'".$arEntidade['muncod']."',
	    						'".$arEntidade['estuf']."') RETURNING enbid";
					$enbid = $db->pegaUm($sql);
				}
				
				$sql = "INSERT INTO emenda.emendadetalheentidade(emdid, enbid, edevalor, edestatus, edeobjeto, ededisponivelpta, edevalordisponivel)
	    				VALUES ($emdid, $enbid, {$v['valor_rcl']}, 'A', null, 'N', {$v['valor_disponivel']})";
				$db->executar($sql);
			}
		} 
		$db->commit();
	}
}

$sql = "SELECT distinct emenda FROM carga.base_siop_2525 WHERE cnpj <> '(vazio)'";
$arrEmenda = $db->carregarColuna($sql);
$arrEmenda = $arrEmenda ? $arrEmenda : array();
$arrCNPJ = array();
foreach ($arrEmenda as $emenda) {
	
	$sql = "select eb.enbcnpj, ve.* from emenda.v_emendadetalheentidade ve 
				inner join emenda.entidadebeneficiada eb on eb.enbid = ve.entid
			where ve.edestatus = 'A'
				and eb.enbano = '".date('Y')."'
			    and ve.emeano = '".date('Y')."'
			    and ve.emetipo = 'E'
			    and ve.emecod = '{$emenda}'
			    and eb.enbcnpj not in (SELECT cnpj
			                            FROM carga.base_siop_2525
			                            WHERE emenda = '{$emenda}')";
	$arrEntidadeEmenda = $db->carregar($sql);
	$arrEntidadeEmenda = $arrEntidadeEmenda ? $arrEntidadeEmenda : array();
	
	foreach ($arrEntidadeEmenda as $v) {
		$sql = "select count(pt.pedid) from emenda.ptemendadetalheentidade pt
					inner join emenda.planotrabalho p on p.ptrid = pt.ptrid
				where p.ptrstatus = 'A'
					and p.ptrexercicio = '".date('Y')."'
				    and pt.edeid = {$v['edeid']}";
		$boPTA = $db->pegaUm($sql);
		if( (int)$boPTA < (int)1 ){
			$sql = "update emenda.emendadetalheentidade set edestatus = 'I' where edeid = {$v['edeid']}";
			$db->executar($sql);
			$db->commit();
			
			array_push($arrCNPJ, array('emenda'=>$emenda, 'cnpj' => $v['enbcnpj']));
		}
	}
}
ver($arrRegistroEmenda, sizeof($arrRegistroEntidade), sizeof($totalValor), $arrRegistroEntidade, $totalValor, $arrCNPJ);

$db->close();











