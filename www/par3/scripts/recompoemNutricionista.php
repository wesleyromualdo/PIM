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

$sql = "select distinct int.entid, int.entcpf, int.entnome, int.entemail, int.tenid, int.inuid, iu.muncod, iu.estuf, iu.inudescricao
		from par3.instrumentounidade_entidade int
        	inner join par3.instrumentounidade iu on iu.inuid = int.inuid and iu.inustatus = 'A'
		where int.entstatus = 'A'
			and int.tenid in (7, 8)
		    and int.inuid is not null
		order by int.entnome";

$arrDados = $db->carregar($sql);

$countTotal = 0;
$cpf = array();
$arCpf = array();
foreach ($arrDados as $v) {
	$sql = "select count(v.vnid)
			from par3.vinculacaonutricionista v
			where v.vncpf = '{$v['entcpf']}'
				and v.inuid = {$v['inuid']} 
			    and v.tenid = {$v['tenid']}
				and v.vnstatus = 'A'";
	$total = $db->pegaUm($sql);
	
	if( (int)$total < 1 ){
		$sql = "update par3.instrumentounidade_entidade set entstatus = 'I' where entid = {$v['entid']}";
		$db->executar($sql);
		//$countTotal++;
		//array_push($cpf, array('entid' => $v['entid'], 'cpf' => $v['entcpf']));
		array_push($arCpf, array('entid' => $v['entid'], 
								'cpf' => $v['entcpf'], 
								'entnome' => $v['entnome'], 
								'entemail' => $v['entemail'], 
								'tenid' => $v['tenid'], 
								'inuid' => $v['inuid'],
								'muncod' => $v['muncod'],
								'estuf' => $v['estuf'],
								'inudescricao' => $v['inudescricao'] )
					);
	}
}
$db->commit();
ob_clean();
header('content-type: text/html; charset=utf-8');
$cabecalho = array('entid', 'entcpf', 'entnome', 'entemail', 'tenid', 'inuid', 'muncod', 'estuf', 'inudescricao');
$db->sql_to_excel($arCpf, 'relRelatorioNutricionistas', $cabecalho, '');

//ver($countTotal, $arCpf);
//$db->commit();