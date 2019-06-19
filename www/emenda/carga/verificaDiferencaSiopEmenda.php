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

$sql = "select uo, programa, acao, localizador, emenda, sum(valor_emenda) as valor_emenda, natureza_despesa, fonte from(
					SELECT distinct  uo, programa, acao, localizador, emenda, cast(valor_emenda as numeric(20,2)) as valor_emenda,
		            	natureza_despesa, fonte
					FROM emenda.carga_emenda_siop c
					where momento = '".$_REQUEST['momento']."'
					order by emenda, uo, programa, acao, localizador
				) as foo
		group by uo, programa, acao, localizador, emenda, natureza_despesa, fonte";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrDiferenca = array();
$arrEmendaNexiste = array();
$arCodigoEmenda = array();

foreach ($arrDados as $siop) {
	
	$gnd = substr($siop['natureza_despesa'], 0, 1);
	$mod = substr($siop['natureza_despesa'], 2, 2);
	
	$sql = "SELECT 
				v.acacod, v.loccod, v.unicod, v.prgcod,
				--emenda
				eme.emecod, eme.emeano, eme.autid, 
				--emendaacao
			    eme.acaid, 
			    --emendadetalhe
			    emd.gndcod, emd.emdid, emd.foncod, emd.mapcod, sum(emd.emdvalor) as emdvalor
			FROM emenda.emenda eme
				inner join emenda.v_funcionalprogramatica v on v.acaid = eme.acaid
			    inner join emenda.emendadetalhe emd on emd.emeid = eme.emeid and emd.emdstatus = 'A'
			WHERE
				eme.emeano = '".$_REQUEST['anoemenda']."'
			    and eme.emetipo = 'E'
			    and eme.emecod = '{$siop['emenda']}'
				and v.acacod = '{$siop['acao']}'
				and v.loccod = '{$siop['localizador']}'
				and v.unicod = '{$siop['uo']}'
				and v.prgcod = '{$siop['programa']}'
				and emd.mapcod = '{$mod}'
				and emd.gndcod = '{$gnd}'
				and emd.foncod = '{$siop['fonte']}'
			group by v.acacod, v.loccod, v.unicod, v.prgcod,
					eme.emecod, eme.emeano, eme.autid, eme.acaid, 
				    emd.gndcod, emd.emdid, emd.foncod, emd.mapcod
			order by v.unicod, v.prgcod, v.acacod, v.loccod";
	$arEmenda = $db->pegaLinha($sql);
	
	if( sizeof($arEmenda) > 0 && is_array($arEmenda) ){
		if( (float)$arEmenda['emdvalor'] <> (float)$siop['valor_emenda']  ){
		
			array_push($arrDiferenca, array('acaid' => $arEmenda['acaid'],
											'emenda_siop' => $siop['emenda'],
											'uo' => $siop['uo'],
											'programa' => $siop['programa'],
											'acao' => $siop['acao'],
											'localizador' => $siop['localizador'],
											'natureza_despesa' => $siop['natureza_despesa'],
											'valor_emenda_siop' => $siop['valor_emenda'],
											'valor_emenda_emendas' => $arEmenda['emdvalor'],
											'uo_emenda' => $arEmenda['unicod'],
											'programa_emenda' => $arEmenda['prgcod'],
											'acao_emenda' => $arEmenda['acacod'],
											'localizador_emenda' => $arEmenda['loccod'],
											'gnd' => $gnd,
											'mod' => $mod
			));
		}
	} else {
		array_push($arrDiferenca, array('acaid' => $arEmenda['acaid'],
											'emenda_siop' => $siop['emenda'],
											'uo' => $siop['uo'],
											'programa' => $siop['programa'],
											'acao' => $siop['acao'],
											'localizador' => $siop['localizador'],
											'natureza_despesa' => $siop['natureza_despesa'],
											'valor_emenda_siop' => $siop['valor_emenda'],
											'valor_emenda_emendas' => $arEmenda['emdvalor'],
											'uo_emenda' => $arEmenda['unicod'],
											'programa_emenda' => $arEmenda['prgcod'],
											'acao_emenda' => $arEmenda['acacod'],
											'localizador_emenda' => $arEmenda['loccod'],
											'gnd' => $gnd,
											'mod' => $mod
			));
	}
}

echo '<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>';

monta_titulo('Por Total Geral da Emenda', '');
$cabecalho = array('acaid', 'emenda', 'uo', 'programa', 'acao', 'localizador', 'natureza_despesa', 'valor_emenda_siop', 'valor_emenda_emendas', 'uo_emenda', 'programa_emenda', 'acao_emenda', ' localizador_emenda', 'gnd', 'mod');
$db->monta_lista_simples( $arrDiferenca, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);
//$db->monta_lista_grupo ( $arrDiferenca, $cabecalho, 2000, 5, '', '', '', '', 'emenda', '', true );

$sql = "SELECT distinct uo, programa, acao, localizador, natureza_despesa, fonte, emenda,
			     valor_emenda, cnpj, nome_beneficiario, valor_rcl_apurada, valor_disponivel
			FROM emenda.carga_emenda_siop c
			where momento = '".$_REQUEST['momento']."'
				and cnpj <> ''
                and cnpj is not null
			order by uo, programa, acao, localizador, emenda, cnpj";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrDiferenca = array();
foreach ($arrDados as $siop) {
	$gnd = substr($siop['natureza_despesa'], 0, 1);
	$mod = substr($siop['natureza_despesa'], 2, 2);
	
	$sql = "select v.unicod, v.prgcod, v.acacod, v.loccod, v.gndcod, v.mapcod, v.foncod, v.emecod,
				v.emdvalor, v.enbcnpj, v.enbnome, v.edevalor, v.edevalordisponivel
			from emenda.v_emendadetalheentidade v
			where v.edestatus = 'A'
				and v.emeano = '".$_REQUEST['anoemenda']."'
				and v.emecod = '{$siop['emenda']}'
				and v.acacod = '{$siop['acao']}'
				and v.loccod = '{$siop['localizador']}'
				and v.unicod = '{$siop['uo']}'
				and v.prgcod = '{$siop['programa']}'
				and v.mapcod = '{$mod}'
				and v.gndcod = '{$gnd}'
				and v.foncod = '{$siop['fonte']}'
				and v.enbcnpj = '{$siop['cnpj']}'";		
	$arrEmenda = $db->pegaLinha($sql);
	
	if( sizeof($arrEmenda) > 0 && is_array($arrEmenda) ){
		if( ((float)$siop['valor_rcl_apurada'] <> (float)$arrEmenda['edevalor']) || (float)$siop['valor_disponivel'] <> (float)$arrEmenda['edevalordisponivel'] ){
			array_push($arrDiferenca, array('emenda_siop' => $siop['emenda'],
											'uo' => $siop['uo'],
											'programa' => $siop['programa'],
											'acao' => $siop['acao'],
											'localizador' => $siop['localizador'],
											'natureza_despesa' => $siop['natureza_despesa'],
											'cnpj' => $siop['cnpj'],
											'nome_beneficiario' => $siop['nome_beneficiario'],
											'valor_rcl_apurada' => $siop['valor_rcl_apurada'],
											'valor_disponivel' => $siop['valor_disponivel'],
											'edevalor' => $arrEmenda['edevalor'],
											'edevalordisponivel' => $arrEmenda['edevalordisponivel']
					));
		}
	} else {
		array_push($arrDiferenca, array('emenda_siop' => $siop['emenda'],
				'uo' => $siop['uo'],
				'programa' => $siop['programa'],
				'acao' => $siop['acao'],
				'localizador' => $siop['localizador'],
				'natureza_despesa' => $siop['natureza_despesa'],
				'cnpj' => $siop['cnpj'],
				'nome_beneficiario' => $siop['nome_beneficiario'],
				'valor_rcl_apurada' => $siop['valor_rcl_apurada'],
				'valor_disponivel' => $siop['valor_disponivel'],
				'edevalor' => $arrEmenda['edevalor'],
				'edevalordisponivel' => $arrEmenda['edevalordisponivel']
		));
	}
}

echo '<br><br>';
monta_titulo('Por Beneficiario da Emenda', '');
$cabecalho = array('emenda', 'uo', 'programa', 'acao', 'localizador', 'natureza_despesa', 'cnpj', 'nome_beneficiario', 'valor_rcl_apurada', 'valor_disponivel', 'edevalor', 'edevalordisponivel');
$db->monta_lista_simples( $arrDiferenca, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);


$sql = "select uo, acao, localizador, programa, fonte, emenda, natureza_despesa, sum(valor_emenda) as valor_emenda, 
		        sum(vlrrcl) as vlrrcl, sum(vlrdisponivel) as vlrdisponivel
		from(
		    select s.uo, s.acao, s.localizador, s.programa, s.fonte, s.emenda, s.natureza_despesa, s.valor_emenda, 
		        sum(s.valor_rcl_apurada) as vlrrcl, sum(s.valor_disponivel) as vlrdisponivel
		    from emenda.carga_emenda_siop s 
		    where momento = '".$_REQUEST['momento']."'
		        --and emenda = '16190005'
		    group by s.emenda, s.natureza_despesa, s.fonte, s.uo, s.acao, s.localizador, s.programa, s.fonte, s.valor_emenda
		) as foo
		group by uo, acao, localizador, programa, fonte, emenda, natureza_despesa
		order by emenda, natureza_despesa, fonte, vlrrcl";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrDiferenca = array();
foreach ($arrDados as $siop) {
	
	$gnd = substr($siop['natureza_despesa'], 0, 1);
	$mod = substr($siop['natureza_despesa'], 2, 2);
	
	$sql = "select acacod, unicod, loccod, prgcod, foncod, mapcod, gndcod, emecod, emdid, sum(vlremenda) as vlremenda, sum(vlrrcl) as vlrrcl, sum(vlrdisponivel) as vlrdisponivel
			from( 
				select distinct f.acacod, f.unicod, f.loccod, f.prgcod, ed.foncod, ed.mapcod, ed.gndcod, e.emecod, ed.emdid, 
                ed.emdvalor as vlremenda, sum(ede.edevalor) as vlrrcl, sum(ede.edevalordisponivel) as vlrdisponivel   
				from emenda.emenda e
                	inner join emenda.emendadetalhe ed on ed.emeid = e.emeid and ed.emdstatus = 'A'
                    inner join emenda.v_funcionalprogramatica f on f.acaid = e.acaid and f.acastatus = 'A'
                    left join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
				where e.emeano = '".$_REQUEST['anoemenda']."'
				    and e.emecod = '".$siop['emenda']."'
				    and ed.mapcod = '{$mod}'
					and ed.gndcod = '{$gnd}'
					and ed.foncod = '{$siop['fonte']}'
					and f.acacod = '{$siop['acao']}'
					and f.loccod = '{$siop['localizador']}'
					and f.unicod = '{$siop['uo']}'
					and f.prgcod = '{$siop['programa']}'
				group by f.acacod, f.unicod, f.loccod, f.prgcod, ed.foncod, ed.mapcod, ed.gndcod, e.emecod, ed.emdid, ed.emdvalor
			) as foo
            group by acacod, unicod, loccod, prgcod, foncod, mapcod, gndcod, emecod, emdid, vlrrcl, vlrdisponivel
			order by emecod, gndcod, mapcod, foncod, vlrrcl";	
	$arrEmenda = $db->pegaLinha($sql);
	
	if( ((float)$siop['valor_emenda'] <> (float)$arrEmenda['vlremenda']) || ((float)$siop['vlrrcl'] <> (float)$arrEmenda['vlrrcl']) || (float)$siop['vlrdisponivel'] <> (float)$arrEmenda['vlrdisponivel'] ){
	
		array_push($arrDiferenca, array(
				'emenda_siop' => $siop['emenda'],
				'vlremenda' => $siop['valor_emenda'],
				'valor_rcl_apurada' => $siop['vlrrcl'],
				'valor_disponivel' => $siop['vlrdisponivel'],
				'vlremenda_e' => $arrEmenda['vlremenda'],
				'edevalor' => $arrEmenda['vlrrcl'],
				'edevalordisponivel' => $arrEmenda['vlrdisponivel']
		));
	}
}
echo '<br><br>';
monta_titulo('Por Total Beneficiario e Total Emenda', '');
$cabecalho = array('emenda', 'valor_emenda', 'valor_rcl_apurada', 'valor_disponivel', 'emevalor', 'edevalor', 'edevalordisponivel');
$db->monta_lista_simples( $arrDiferenca, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);
















