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

/*
 * ATUALIZA AS EMENDAS QUE SOMENTE ALTERARAM A UO 
 * */
$sql = "select uo, programa, acao, localizador, emenda, sum(valor_emenda) as valor_emenda, totalemenda from(
			SELECT distinct  uo, programa, acao, localizador, emenda, cast(valor_emenda as numeric(20,2)) as valor_emenda,
				(select count(*) from(
                                SELECT distinct  uo, programa, acao, localizador, emenda, valor_emenda
                                FROM emenda.carga_emenda_siop c
                                where momento = '".$_REQUEST['momento']."'
                                order by emenda, uo, programa, acao, localizador
                                ) as foo where emenda = c.emenda) as totalemenda
			FROM emenda.carga_emenda_siop c
			where momento = '".$_REQUEST['momento']."'
			$get_emenda
			order by emenda, uo, programa, acao, localizador
		) as foo
group by uo, programa, acao, localizador, emenda, totalemenda";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrEmendasUnicas = array();
$count = 0;
$dadosAnterior = array();

foreach ($arrDados as $key => $v) {
	
	$sql = "select acaid from monitora.acao a
			where a.acacod = '".trim($v['acao'])."'
				and a.loccod = '".trim($v['localizador'])."'
				and a.prgano = '".$_REQUEST['anoemenda']."'
				and a.prgcod = '".trim($v['programa'])."'
				and a.unicod = '".trim($v['uo'])."'";
	$acaid = $db->pegaUm($sql);
	
	/* $acao = $arrDados[($key+1)]['acao'];
	$unidade = $arrDados[($key+1)]['uo'];
	$localizador = $arrDados[($key+1)]['localizador'];
	$programa = $arrDados[($key+1)]['programa']; */
		
	if( $acaid ){
		$sql = "select emeid from emenda.emenda where emeano = '".$_REQUEST['anoemenda']."' and emecod = '{$v['emenda']}'";
		$emeid = $db->pegaUm($sql);
		
		if( !empty($emeid)  ){
			$total = pegaTotalEmenda($v['emenda']);
			if((int)$total == (int)1){
				//array_push($arrEmendasUnicas, $v);
				$sql = "UPDATE emenda.emenda SET
							ememomentosiop = '{$_REQUEST['momento']}',
							acaid = $acaid,
							emevalor = '".trim($v['valor_emenda'])."'
						WHERE emeid = $emeid";
				 $db->executar($sql);
				 $db->commit();
			}
			/* if( (int)$v['totalemenda'] == (int)1 ){
				
			} */ /*else {
				if( $acao == $v['acao'] && $unidade == $v['uo'] && $localizador == $v['localizador'] && $programa == $v['programa'] ){
					
				} else {
					array_push($arrEmendasUnicas, $dadosAnterior);
				}
				$v['acaid'] = $acaid;
				$dadosAnterior = $v;
				$acao = $v['acao']; 
				$unidade = $v['uo'];
				$localizador = $v['localizador'];
				$programa = $v['programa'];
			}*/
		}
	}
}
//ver(sizeof($arrEmendasUnicas),$arrEmendasUnicas,d);

verificaEmendas( $get_emenda );
verificaDetalheEmenda( $get_emenda );

 $sql = "SELECT uo, programa, acao, localizador, natureza_despesa, fonte, emenda, 
		       nome_parlamentar, partido, uf, rp_atual, valor_emenda, cnpj, nome_beneficiario, 
		       valor_aprovado, valor_rcl_apurada, valor_disponivel,
 				(select count(*) from emenda.carga_emenda_siop where emenda = c.emenda) as totalemenda
		  FROM emenda.carga_emenda_siop c
		  where momento = '".$_REQUEST['momento']."'
		  		$get_emenda
		  		";
 
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$strEmendas = array();
$arrSemAcao = array();
$arrAcaid = array();

foreach ($arrDados as $v) {	
		
	//$arTotalEmenda = pegaValorTotalEmenda($v['emenda']);
	
	array_push($strEmendas, $v['emenda']);
	
	$sql = "select acaid from monitora.acao a
			where a.acacod = '".trim($v['acao'])."'
					and a.loccod = '".trim($v['localizador'])."'
					and a.prgano = '".$_REQUEST['anoemenda']."'
					and a.prgcod = '".trim($v['programa'])."'
					and a.unicod = '".trim($v['uo'])."'";
	$acaid = $db->pegaUm($sql);
	
	$emeid = '';
	if( !empty($acaid) ){
		
		$sql = "select emeid from emenda.emenda where emeano = '".$_REQUEST['anoemenda']."' and emecod = '{$v['emenda']}' and acaid = $acaid";
		$emeid = $db->pegaUm($sql);
				
		$gnd = substr($v['natureza_despesa'], 0, 1);
		$mod = substr($v['natureza_despesa'], 2, 2);
		
		$sql = "select ed.emdid
				from emenda.emenda e
				  	inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				where e.emeano = '".$_REQUEST['anoemenda']."'
					and e.emecod = '{$v['emenda']}'
				    and ed.gndcod = '{$gnd}'
				    and ed.mapcod = '{$mod}'
				    and ed.foncod = '{$v['fonte']}'
				    and e.emeid = $emeid";
		
		$emdid = $db->pegaUm($sql);
		
		/*
		 * VINCULA OS BENEFICIARIOS NO EMENDA DETALHE
		*/
		
		if( (int)strlen($v['cnpj']) == (int)14 ){
			$sql = "select ede.edeid, ede.edevalordisponivel, ede.edevalor
					from emenda.emendadetalheentidade ede
						inner join emenda.entidadebeneficiada enb on enb.enbid = ede.enbid and enb.enbstatus = 'A'
					where
					    ede.emdid = $emdid
						and enb.enbcnpj = '{$v['cnpj']}'";
			$arrEntidade = $db->pegaLinha($sql);
						
			if( empty($arrEntidade['edeid']) ){
				$v['emdid'] = $emdid;
				if( $v['cnpj'] ){
					$sql = "SELECT enbid FROM emenda.entidadebeneficiada WHERE enbstatus = 'A' and enbcnpj = '{$v['cnpj']}' and enbano = '".$_REQUEST['anoemenda']."'";
					$enbid = $db->pegaUm($sql);
					
					if( empty($enbid) ){
						$sql = "SELECT enbnome, muncod, estuf FROM emenda.entidadebeneficiada WHERE enbcnpj = '{$v['cnpj']}'";
						$arEntidade = $db->pegaLinha($sql);
						
						$sql = "INSERT INTO emenda.entidadebeneficiada(enbstatus, enbano, enbdataalteracao, enbnome, enbcnpj, muncod, estuf)
	    						VALUES ('A',
		    						'".$_REQUEST['anoemenda']."',
		    						NOW(),
		    						'".$v['nome_beneficiario']."',
		    						'".$v['cnpj']."',
		    						'".$arEntidade['muncod']."',
		    						'".$arEntidade['estuf']."') RETURNING enbid";
						$enbid = $db->pegaUm($sql); 
					}				
					$sql = "INSERT INTO emenda.emendadetalheentidade(emdid, enbid, edevalor, edestatus, edeobjeto, ededisponivelpta, edevalordisponivel, usucpfalteracao, ededataalteracao)
		    				VALUES ($emdid, $enbid, {$v['valor_rcl_apurada']}, 'A', null, 'S', {$v['valor_disponivel']}, '00000000191', 'now()')";
					$db->executar($sql);
				}
				
			} else {						
				$edejustificativasiop = '';
				$ededisponivelpta = 'S';
				if( (float)$v['valor_rcl_apurada'] < (int)1 ){
					$edejustificativasiop = "A Pedido do parlamentar essa emenda teve remanejamento de recurso.";
					$ededisponivelpta = 'N';
				}
				$sql = "UPDATE emenda.emendadetalheentidade
						SET edevalor = ".(float)$v['valor_rcl_apurada'].",
							edevalordisponivel = ".(float)$v['valor_disponivel'].",
							ededisponivelpta = '$ededisponivelpta',
							edejustificativasiop = '{$edejustificativasiop}',
							edestatus = 'A'
							WHERE edeid = {$arrEntidade['edeid']}";
				$db->executar($sql);
			}
			$db->commit();
		}
	} else {
		array_push($arrSemAcao, $v);
	}
}

inativaBeneficiarioIncorreto( $strEmendas );
atualizaDadosPTA( $strEmendas );

if( $arrSemAcao[0] ){
	ob_clean();
	header('content-type: text/html; charset=utf-8');
	
	$cabecalho = array();
	if( $arrSemAcao[0] ){
		foreach ($arrSemAcao[0] as $cab => $val) {
			array_push($cabecalho, $cab);
			if( strstr( strtoupper($c['label']), strtoupper('valor') ) ) {
				$formato[] = "n";
			} else {
				$formato[] = "";
			}
		}
	}
	$db->sql_to_excel($arrSemAcao, 'rel_dados_carga_emenda', $cabecalho, $formato);
	exit;
}

//$db->close();
function verificaEmendas( $get_emenda ){
	global $db;
	
	$sql = "select uo, programa, acao, localizador, emenda, sum(valor_emenda) as valor_emenda, 
				(select count(*) from emenda.carga_emenda_siop where emenda = foo.emenda) as totalemenda 
			from(
			SELECT distinct uo, programa, acao, localizador, emenda, c.valor_emenda
						FROM emenda.carga_emenda_siop c
						where momento = '".$_REQUEST['momento']."'
							 $get_emenda
			) as foo
			group by uo, programa, acao, localizador, emenda";
	
	$arrEmendas = $db->carregar($sql);
	$arrEmendas = $arrEmendas ? $arrEmendas : array();
	
	foreach ($arrEmendas as $v) {
		$sql = "select acaid from monitora.acao a
			where a.acacod = '".trim($v['acao'])."'
					and a.loccod = '".trim($v['localizador'])."'
					and a.prgano = '".$_REQUEST['anoemenda']."'
					and a.prgcod = '".trim($v['programa'])."'
					and a.unicod = '".trim($v['uo'])."'";
		$acaid = $db->pegaUm($sql);
		
		$emeid = '';
		if( !empty($acaid) ){
					
			$sql = "select emeid from emenda.emenda where emeano = '".$_REQUEST['anoemenda']."' and emecod = '{$v['emenda']}' and acaid = $acaid";
			$emeid = $db->pegaUm($sql);
		
			if( empty($emeid)  ){					
				$sql = "select autid from emenda.autor where autcod = '".substr($v['emenda'], 0, 4)."'";
				$autid = $db->pegaUm($sql);
					
				$arEmenda = $db->pegaLinha("select resid, etoid from emenda.emenda where emeano = '".$_REQUEST['anoemenda']."' and emecod = '{$v['emenda']}'");
		
				$sql = "INSERT INTO emenda.emenda(emecod, autid, acaid, resid, emevalor, emetipo, emeano, emedescentraliza, emelibera, emerelator, etoid, ememomentosiop)
	    				VALUES ('".trim($v['emenda'])."', {$autid}, $acaid, ".$arEmenda['resid'].", '".trim($v['valor_emenda'])."', 'E', '".$_REQUEST['anoemenda']."', 'N', 'N', 'N', ".$arEmenda['etoid'].", '{$_REQUEST['momento']}')";
				$db->executar($sql);
			} else {
				$sql = "UPDATE emenda.emenda SET
							ememomentosiop = '{$_REQUEST['momento']}',
							emevalor = '".trim($v['valor_emenda'])."',
							acaid = $acaid
						WHERE emeid = $emeid";
				$db->executar($sql);
			}
			$db->commit();
		}
	}
	return true;
}

function verificaDetalheEmenda( $get_emenda ){
	global $db;
	
	$sql = "SELECT distinct uo, programa, acao, localizador, natureza_despesa, fonte, emenda, 
			     nome_parlamentar, partido, uf, rp_atual, valor_emenda
			FROM emenda.carga_emenda_siop c
			where momento = '".$_REQUEST['momento']."'
				$get_emenda";	
	
	$arrDetalhe = $db->carregar($sql);
	$arrDetalhe = $arrDetalhe ? $arrDetalhe : array();
	
	/*
	 * Inativa os detalhe da emenda caso não exista plano de trabalho 
	 * */
	foreach ($arrDetalhe as $v) {
		$sql = "select acaid from monitora.acao a
				where a.acacod = '".trim($v['acao'])."'
					and a.loccod = '".trim($v['localizador'])."'
					and a.prgano = '".$_REQUEST['anoemenda']."'
					and a.prgcod = '".trim($v['programa'])."'
					and a.unicod = '".trim($v['uo'])."'";
		$acaid = $db->pegaUm($sql);
		
		if( $acaid ){
			$sql = "select emeid from emenda.emenda where emeano = '".$_REQUEST['anoemenda']."' and emecod = '{$v['emenda']}' and acaid = $acaid";
			$emeid = $db->pegaUm($sql);
			
			$sql = "SELECT 
						emd.emdid, eme.emeano, ede.edeid,
					    (select count(pe.pedid) from emenda.ptemendadetalheentidade pe
					    	inner join emenda.planotrabalho pt on pt.ptrid = pe.ptrid and pt.ptrstatus = 'A'
					    where pe.edeid = ede.edeid and pt.ptrexercicio = eme.emeano) as tem_pta
					FROM emenda.emenda eme
					    inner join emenda.emendadetalhe emd on emd.emeid = eme.emeid and emd.emdstatus = 'A'
					    left join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
					where eme.emeid = $emeid";
			$arView = $db->carregar($sql);
			$arView = $arView ? $arView : array();
			
			foreach ($arView as $view) {
				if( (int)$view['tem_pta'] < (int)1 ){
					$sql = "update emenda.emendadetalhe set emdstatus = 'I' where emdid = ".$view['emdid'];
					$db->executar($sql);
					
					$sql = "update emenda.emendadetalheentidade set edestatus = 'I' where emdid = ".$view['emdid'];
					$db->executar($sql);
				}
			}
		}
	}
	
	foreach ($arrDetalhe as $v) {
		
		$sql = "select acaid from monitora.acao a
				where a.acacod = '".trim($v['acao'])."'
					and a.loccod = '".trim($v['localizador'])."'
					and a.prgano = '".$_REQUEST['anoemenda']."'
					and a.prgcod = '".trim($v['programa'])."'
					and a.unicod = '".trim($v['uo'])."'";
		$acaid = $db->pegaUm($sql);
		
		$emeid = '';
		if( !empty($acaid) ){
			
			$gnd = substr($v['natureza_despesa'], 0, 1);
			$mod = substr($v['natureza_despesa'], 2, 2);
			
			$arrFuncional = array(
					'uo' => $v['uo'],
					'programa' => $v['programa'],
					'acao' => $v['acao'],
					'localizador' => $v['localizador'],
					'emenda' => $v['emenda'],
					'gnd' => $gnd,
					'mod' => $mod,
					'fonte' => $v['fonte']
			);
			
			$arValorTotal = pegaValorTotalEmendaPorFuncional($arrFuncional);
			$valor_emenda = $arValorTotal['valor_emenda']; 
			
			$sql = "select emeid from emenda.emenda where emeano = '".$_REQUEST['anoemenda']."' and emecod = '{$v['emenda']}' and acaid = $acaid";
			$emeid = $db->pegaUm($sql);
			
			$sql = "select ed.emdid
					from emenda.emenda e
					  	inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
					where e.emeano = '".$_REQUEST['anoemenda']."'
						and e.emecod = '{$v['emenda']}'
					    and ed.gndcod = '{$gnd}'
					    and ed.mapcod = '{$mod}'
					    and ed.foncod = '{$v['fonte']}'
					    and e.emeid = $emeid";			
			$emdid = $db->pegaUm($sql);
			
			if( empty($emdid) ){
				$sql = "INSERT INTO emenda.emendadetalhe(emeid, gndcod, foncod, mapcod, emdvalor, emdtipo, emdimpositiva, emdstatus)
						VALUES ($emeid, '{$gnd}', '{$v['fonte']}', '{$mod}', '{$valor_emenda}', 'E', ".$v['rp_atual'].", 'A') returning emdid";
				$db->executar($sql);
			} else {	
				$sql = "update emenda.emendadetalhe set emdvalor = '{$valor_emenda}', emdimpositiva = {$v['rp_atual']}, emdstatus = 'A' where emdid = $emdid";
				$db->executar($sql);
			}
			$db->commit();			
		}
	}
}

function pegaValorTotalEmendaPorFuncional( $arrFuncional = array() ){
	global $db;
	
	$sql = "select uo, programa, acao, localizador, emenda, sum(valor_emenda) as valor_emenda,
				gnd, mod, fonte from(
			SELECT distinct uo, programa, acao, localizador, emenda, valor_emenda,
				substring(natureza_despesa,1,1) as gnd, substring(natureza_despesa,3,2) as mod, fonte
			FROM emenda.carga_emenda_siop c
			where momento = '".$_REQUEST['momento']."'
				and uo = '".$arrFuncional['uo']."'
			    and programa = '".$arrFuncional['programa']."'
			    and acao = '".$arrFuncional['acao']."'
			    and localizador = '".$arrFuncional['localizador']."'
			    and emenda = '".$arrFuncional['emenda']."'
				and substring(natureza_despesa,1,1) = '".$arrFuncional['gnd']."'
				and substring(natureza_despesa,3,2) = '".$arrFuncional['mod']."'
				and fonte = '".$arrFuncional['fonte']."'
			) as foo
			group by uo, programa, acao, localizador, emenda, gnd, mod, fonte";
	
	return $db->pegaLinha($sql);
}

function pegaValorTotalEmenda( $emenda ){
	global $db;
	
	$sql = "select 
				emenda, valor_emenda, valor_rcl_apurada, valor_disponivel
			from(
			select 
				emenda, sum(valor_emenda) as valor_emenda, sum(valor_rcl_apurada) as valor_rcl_apurada, sum(valor_disponivel) as valor_disponivel
			from(
			    select 
			        gnd, mod, fonte, emenda, valor_emenda, sum(valor_rcl_apurada) as valor_rcl_apurada, sum(valor_disponivel) as valor_disponivel
			    from(
			        SELECT substring(natureza_despesa,1,1) as gnd, substring(natureza_despesa,3,2) as mod, fonte, emenda, 
			             valor_emenda, valor_rcl_apurada, valor_disponivel
			        FROM emenda.carga_emenda_siop
					where emenda = '".$emenda."'
						and momento = '".$_REQUEST['momento']."'
			    ) as foo
			    group by gnd, mod, fonte, emenda, valor_emenda
			) as foo1
			group by  emenda
			) foo2";
	
	$arEmenda = $db->pegaLinha($sql);
	return $arEmenda;
}

function pegaValorTotalEmendaPorGND( $emenda, $gnd, $mod, $fonte ){
	global $db;
	
	$sql = "select 
		        gnd, mod, fonte, emenda, valor_emenda, sum(valor_rcl_apurada) as valor_rcl_apurada, sum(valor_disponivel) as valor_disponivel
		    from(
		        SELECT substring(natureza_despesa,1,1) as gnd, substring(natureza_despesa,3,2) as mod, fonte, emenda, 
		             valor_emenda, valor_rcl_apurada, valor_disponivel
		        FROM emenda.carga_emenda_siop
				where emenda = '".$emenda."'
					and momento = '".$_REQUEST['momento']."'
		    ) as foo
		    where gnd = '$gnd'
				and mod = '$mod'
				and fonte = '$fonte'
		    group by gnd, mod, fonte, emenda, valor_emenda";
	$arEmenda = $db->pegaLinha($sql);
	return $arEmenda;
}

function atualizaDadosPTA($arCodigoEmenda){
	global $db;
	
	$strEmenda = "'".implode("', '", $arCodigoEmenda)."'";
	
	$sql = "SELECT distinct emenda FROM emenda.carga_emenda_siop
			where momento = '".$_REQUEST['momento']."'
				and emenda in ($strEmenda)";

	$arrEmenda = $db->carregarColuna($sql);
	$arrEmenda = $arrEmenda ? $arrEmenda : array();

	foreach ($arrEmenda as $emenda) {
		$sql = "select ve.edeid, ve.emdid, eb.enbcnpj, eb.enbnome, ve.emecod, ve.gndcod, ve.mapcod, ve.foncod, 
					ve.emdvalor, ve.edevalor, ve.edevalordisponivel, ve.edecpfresp, ve.edenomerep, ve.ededddresp,
				    ve.edetelresp, ve.edemailresp
				from emenda.v_emendadetalheentidade ve
					inner join emenda.entidadebeneficiada eb on eb.enbid = ve.entid
				where ve.edestatus = 'A'
					and eb.enbano = '".$_REQUEST['anoemenda']."'
					and ve.emeano = '".$_REQUEST['anoemenda']."'
					and ve.emecod = '$emenda'
					and ve.emetipo = 'E'
					and ve.edevalor < 1";
		$arrBenefZerado = $db->carregar($sql);
		$arrBenefZerado = $arrBenefZerado ? $arrBenefZerado : array();
		
		foreach ($arrBenefZerado as $ben){
			$sql = "select ve.edeid, ve.emdid, eb.enbcnpj, eb.enbnome, ve.emecod, ve.gndcod, ve.mapcod, ve.foncod, 
						ve.emdvalor, ve.edevalor, ve.edevalordisponivel, ve.edecpfresp, ve.edenomerep, ve.ededddresp,
					    ve.edetelresp, ve.edemailresp,
						(select count(iedid) from emenda.iniciativaemendadetalhe where emdid = ve.emdid and iedstatus = 'A') as boiniciativaemendadetalhe,
					    (select count(ideid) from emenda.iniciativadetalheentidade where edeid = ve.edeid and idestatus = 'A') as boiniciativadetalheentidade,
					    (select count(ediid) from emenda.emendadetalheimpositivo where edeid = ve.edeid and edistatus = 'A') as boemendadetalheimpositivo,
					    (select count(epiid) from emenda.emendapariniciativa where edeid = ve.edeid and epistatus = 'A') as boemendapariniciativa,
					    (select count(sepid) from par.subacaoemendapta where emdid = ve.emdid and sepstatus = 'A') as bosubacaoemendapta,
					    (select count(pedid) from emenda.ptemendadetalheentidade e where edeid = ve.edeid) as boptemendadetalheentidade
					from emenda.v_emendadetalheentidade ve
						inner join emenda.entidadebeneficiada eb on eb.enbid = ve.entid
					where ve.emeano = '".$_REQUEST['anoemenda']."'
						and ve.edestatus = 'A'
					    and ve.emecod = '".$ben['emecod']."'
					    and ve.enbcnpj = '".$ben['enbcnpj']."'
					    and ve.gndcod = '".$ben['gndcod']."'
					    and ve.mapcod = '".$ben['mapcod']."'
					    and ve.foncod = '".$ben['foncod']."'
					    and ve.edevalor > 0";
			$arrBenefValor = $db->carregar($sql);
			$arrBenefValor = $arrBenefValor ? $arrBenefValor : array();
			
			//ver($arrBenefZerado, $arrBenefValor);
			
			$sql = "update emenda.emendadetalheentidade set ededisponivelpta = 'N' where edeid = {$ben['edeid']}";
			$db->executar($sql);
			
			foreach ($arrBenefValor as $val) {
				$sql = "UPDATE emenda.emendadetalheentidade SET
							edecpfresp = '{$ben['edecpfresp']}', 
					        edenomerep = '{$ben['edenomerep']}', 
					        edemailresp = '{$ben['edemailresp']}', 
					        edetelresp = '{$ben['edetelresp']}', 
					        ededddresp = '{$ben['ededddresp']}',
					       	ededisponivelpta = 'S'
					 WHERE edeid = {$val['edeid']}";
				$db->executar($sql);
				$db->commit();
				
				if( (int)$val['boiniciativaemendadetalhe'] < (int)1 ){
					$sql = "INSERT INTO emenda.iniciativaemendadetalhe(emdid, iniid, iedstatus)
							(select {$val['emdid']}, iniid, iedstatus from emenda.iniciativaemendadetalhe where emdid = {$ben['emdid']})";
					$db->executar($sql);
					
					/* $sql = "update emenda.iniciativaemendadetalhe set iedstatus = 'I' where emdid = {$ben['emdid']}";
					$db->executar($sql); */
				}
				if( (int)$val['boiniciativadetalheentidade'] < (int)1 ){
					$sql = "INSERT INTO emenda.iniciativadetalheentidade(edeid, iniid, idestatus)
							(select {$val['edeid']}, iniid, idestatus from emenda.iniciativadetalheentidade where edeid = {$ben['edeid']})";
					$db->executar($sql);
					
					/* $sql = "update emenda.iniciativadetalheentidade set idestatus = 'I' where edeid = {$ben['edeid']}";
					$db->executar($sql); */
				}
				if( (int)$val['boemendadetalheimpositivo'] < (int)1 ){
					$sql = "INSERT INTO emenda.emendadetalheimpositivo(emdid, edivalor, edidata, edistatus, usucpf, ediimpositivo, edidescricao, edeid)
							(select {$val['emdid']}, edivalor, edidata, edistatus, usucpf, ediimpositivo, edidescricao, {$val['edeid']} from emenda.emendadetalheimpositivo where edeid = {$ben['edeid']})";
					$db->executar($sql);
				}
				if( (int)$val['boemendapariniciativa'] < (int)1 ){
					$sql = "INSERT INTO emenda.emendapariniciativa(ppsid, edeid, iniid)
							(select ppsid, {$val['edeid']}, iniid from emenda.emendapariniciativa where edeid = {$ben['edeid']} and epistatus = 'A')";
					$db->executar($sql);
					
					/* $sql = "update emenda.emendapariniciativa set epistatus = 'I' where edeid = {$ben['edeid']}";
					$db->executar($sql); */
				}
				if( (int)$val['bosubacaoemendapta'] < (int)1 ){
					$sql = "INSERT INTO par.subacaoemendapta(sbdid, emeid, ptrid, sepvalor, emdid, sepstatus)
							(select sbdid, emeid, ptrid, sepvalor, {$val['emdid']}, sepstatus from par.subacaoemendapta where emdid = {$ben['emdid']})";
					$db->executar($sql);
					
					/* $sql = "update par.subacaoemendapta set sepstatus = 'I' where emdid = {$ben['emdid']}";
					$db->executar($sql); */
				}
				if( (int)$val['boptemendadetalheentidade'] < (int)1 ){
					/* $sql = "INSERT INTO emenda.ptemendadetalheentidade(ptrid, edeid, pedvalor)
							(select ptrid, {$val['edeid']}, pedvalor from emenda.ptemendadetalheentidade where edeid = {$ben['edeid']})";
					$db->executar($sql);
					
					$sql = "delete from emenda.ptemendadetalheentidade where edeid = {$ben['edeid']}";
					$db->executar($sql); */
					
					$sql = "update emenda.ptemendadetalheentidade set edeid = {$val['edeid']} where edeid = {$ben['edeid']}";
					$db->executar($sql);
				}
				$db->commit();
			}
		}
	}
}

function inativaBeneficiarioIncorreto($arCodigoEmenda){
	global $db;
	$strEmenda = "'".implode("', '", $arCodigoEmenda)."'";
	$sql = "select ve.acacod, ve.loccod, ve.prgcod, ve.unicod, ve.edeid, eb.enbcnpj, eb.enbnome, ve.emecod, ve.gndcod, ve.mapcod, ve.foncod, ve.emdvalor, ve.edevalor, ve.edevalordisponivel 
			from emenda.v_emendadetalheentidade ve
				inner join emenda.entidadebeneficiada eb on eb.enbid = ve.entid
			where ve.edestatus = 'A'
				and eb.enbano = '".$_REQUEST['anoemenda']."'
				and ve.emeano = '".$_REQUEST['anoemenda']."'
				and ve.emecod in ($strEmenda)
				and ve.emetipo = 'E'";
	$arrEmenda = $db->carregar($sql);
	$arrEmenda = $arrEmenda ? $arrEmenda : array();
	
	$arrCNPJSemPTA = array();
	$arrCNPJComPTA = array();
	
	foreach ($arrEmenda as $v) {
		
		$sql = "select count(gnd)
				from(
					SELECT substring(natureza_despesa,1,1) as gnd, substring(natureza_despesa,3,2) as mod, fonte, emenda,
						valor_emenda, cnpj, nome_beneficiario, valor_rcl_apurada, valor_disponivel
					FROM emenda.carga_emenda_siop
					WHERE emenda = '".$v['emecod']."'
						and momento = '".$_REQUEST['momento']."'
						and acao = '".$v['acacod']."' 
		                and localizador = '".$v['loccod']."'
		                and programa = '".$v['prgcod']."'
		                and uo = '".$v['unicod']."'
				)as foo
				where 
					gnd = '".$v['gndcod']."'
				    and mod = '".$v['mapcod']."'
				    and fonte = '".$v['foncod']."'
				    and cnpj = '".$v['enbcnpj']."'";
		
		$boTem = $db->pegaUm($sql);
		
		/* $valorEmenda = pegaValorTotalEmendaPorGND($v['emecod'], $v['gndcod'], $v['mapcod'], $v['foncod']);
		$valorEmenda = $valorEmenda['valor_emenda'] ? $valorEmenda['valor_emenda'] : '0'; */ 
		
		if( (int)$boTem < 1){
			$sql = "select count(pt.pedid) from emenda.ptemendadetalheentidade pt
						inner join emenda.planotrabalho p on p.ptrid = pt.ptrid
					where p.ptrstatus = 'A'
						and p.ptrexercicio = '".$_REQUEST['anoemenda']."'
									and pt.edeid = {$v['edeid']}";
			$boPTA = $db->pegaUm($sql);
			
			if( (int)$boPTA > (int)0 ){
				/* $sql = "update emenda.emendadetalheentidade set edevalor = 0, edevalordisponivel = 0,edestatus = 'A', edejustificativasiop = 'A Pedido do parlamentar essa emenda teve remanejamento de recurso.' 
						where edeid = {$v['edeid']};";
				$db->executar($sql); */
				
				$sql = "update emenda.planotrabalho set ptrbloqueiosiop = 'S' where ptrid in (select ptrid from emenda.ptemendadetalheentidade where edeid = {$v['edeid']});";
				$db->executar($sql);
				
				/* $sql = "select emdid, sum(naotem) as naotem, sum(tem) as tem from(
						    select emdid, 
						        case when edejustificativasiop is null then count(edeid) else 0 end as naotem,
						        case when edejustificativasiop is not null then count(edeid) else 0 end as tem
						    from emenda.emendadetalheentidade 
						    where emdid in (select emdid from emenda.emendadetalheentidade where edeid = {$v['edeid']}) and edestatus = 'A'
						    group by edejustificativasiop, emdid
						) as foo
						group by emdid";				
				$arTotEntidade = $db->pegaLinha($sql); */
				
				/* if( (int)$arTotEntidade['naotem'] < (int)1 ){
					$sql = "update emenda.emendadetalhe set emdvalor = ".$valorEmenda." where emdid = {$arTotEntidade['emdid']}";
					$db->executar($sql);
				} */				
			} else {
				$sql = "update emenda.emendadetalheentidade set edestatus = 'I', edejustificativasiop = 'Este beneficiario foi inativado pela carga do SIOP.' where edeid = {$v['edeid']};";
				$db->executar($sql);
			}
			$db->commit();
		}
	}
}

function pegaTotalEmenda($emenda){
	global $db;
	
	$sql = "select count(*) from(
			select uo, programa, acao, localizador, emenda, sum(valor_emenda) as valor_emenda 
			from(
			    SELECT distinct  uo, programa, acao, localizador, emenda, cast(valor_emenda as numeric(20,2)) as valor_emenda
			    FROM emenda.carga_emenda_siop c
			    where momento = '".$_REQUEST['momento']."'
			    and emenda = '{$emenda}'
			    order by emenda, uo, programa, acao, localizador
			) as foo
			group by uo, programa, acao, localizador, emenda
			) as foo1";
	return $db->pegaUm($sql);
} 