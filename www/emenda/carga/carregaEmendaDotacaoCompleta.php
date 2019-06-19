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

if( $_REQUEST['emenda'] ){
	$get_emenda = " and emenda = '{$_REQUEST['emenda']}'";
}

$db = new cls_banco();

$sql = "SELECT emenda, codigo_autor, autor, tipo_emenda, tipo_uo, tc, 
		     uo, funcao, subfuncao, programa, acao, localizador, 
		     localizador_nome, rp, substring(fonte, 2,3) as fonte, gnd, modalidade, 
		     sum(dotacao_inicial) as dotacao_inicial, sum(dotacao_atualizada) as dotacao_atualizada, 
		     sum(credito_disponivel) as credito_disponivel, sum(credito_indisponivel) as credito_indisponivel, 
		     sum(despesas_empenhada) as despesas_empenhada, sum(despesas_liquidadas) as despesas_liquidadas, 
		     sum(despesas_pagas) as despesas_pagas, ano_emenda
		FROM emenda.carga_emenda_anual
		WHERE ano_emenda = '".date('Y')."'
		group by emenda, codigo_autor, autor, tipo_emenda, tipo_uo, tc, 
		     uo, funcao, subfuncao, programa, acao, localizador, 
		     localizador_nome, rp, fonte, gnd, modalidade, ano_emenda";

$arrEmendas = $db->carregar($sql);
$arrEmendas = $arrEmendas ? $arrEmendas : array();

$arrAcaid = array();

foreach ($arrEmendas as $v) {
	$sql = "select acaid from monitora.acao a
		where a.acacod = '".trim($v['acao'])."'
				and a.loccod = '".trim($v['localizador'])."'
				and a.prgano = '".trim($v['ano_emenda'])."'
				and a.prgcod = '".trim($v['programa'])."'
				and a.unicod = '".trim($v['uo'])."'";
	$acaid = $db->pegaUm($sql);
	
	$emeid = '';
	if( !empty($acaid) ){
				
		$sql = "select emeid from emenda.emenda where emeano = '".$v['ano_emenda']."' and emecod = '{$v['emenda']}' and acaid = $acaid";
		$emeid = $db->pegaUm($sql);
	
		if( empty($emeid)  ){					
			$sql = "select autid from emenda.autor where autcod = '".$v['codigo_autor']."'";
			$autid = $db->pegaUm($sql);
	
			$sql = "INSERT INTO emenda.emenda(emecod, autid, acaid, resid, emevalor, emetipo, emeano, emedescentraliza, emelibera, emerelator, etoid, ememomentosiop, emepo)
    				VALUES ('".trim($v['emenda'])."', {$autid}, $acaid, null, '".trim($v['dotacao_atualizada'])."', 'E', '".$v['ano_emenda']."', 'N', 'N', 'N', null, null, '".$v['po']."') returning emeid";
			$emeid = $db->pegaUm($sql);
		} else {
			$sql = "UPDATE emenda.emenda SET 
						emevalor = '".trim($v['dotacao_atualizada'])."',
						acaid = $acaid
					WHERE emeid = $emeid";
			$db->executar($sql);
		}
		
		$sql = "select ed.emdid
					from emenda.emenda e
					  	inner join emenda.emendadetalhe ed on ed.emeid = e.emeid and ed.emdstatus = 'A'
					where e.emeano = '".$v['ano_emenda']."'
							and e.emecod = '{$v['emenda']}'
							and ed.gndcod = '{$v['gnd']}'
							and ed.mapcod = '{$v['modalidade']}'
							and ed.foncod = '{$v['fonte']}'
							and e.emeid = $emeid";
		$emdid = $db->pegaUm($sql);
			
		if( empty($emdid) ){
			$sql = "INSERT INTO emenda.emendadetalhe(emeid, gndcod, foncod, mapcod, emdvalor, emdtipo, emdimpositiva, emdstatus, emddotacaoinicial)
					VALUES ($emeid, '{$v['gnd']}', '{$v['fonte']}', '{$v['modalidade']}', '".$v['dotacao_atualizada']."', 'E', ".$v['rp'].", 'A', ".($v['dotacao_inicial'] ? $v['dotacao_inicial'] : 'null').") returning emdid";
			$db->executar($sql);
		} else {
			$sql = "update emenda.emendadetalhe set emddotacaoinicial = ".($v['dotacao_inicial'] ? $v['dotacao_inicial'] : 'null').", emdvalor = '".$v['dotacao_atualizada']."', emdimpositiva = {$v['rp']}, emdstatus = 'A' 
					where emdid = $emdid";
			$db->executar($sql);
		}		
		$db->commit();
	} else {
		array_push($arrAcaid, $v);
	}
}
ver($arrAcaid);