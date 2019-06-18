<?php

if(empty($_GET['protect-carga'])) {
	die("");
}

set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/autoload.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$data = @preg_replace('!s:(\d+):"(.*?)";!e', "'s:'.strlen('$2').':\"$2\";'", file_get_contents('carga_lds/data.serializable'));

$obras = unserialize($data);

$db = new cls_banco();

$ids = array();

foreach ($obras as $key => $obra) {
	if($key == 1) {
		
		$enderecoSql = "select endid from entidade.endereco where endcep = '" . preg_replace('/[^0-9]/', '', $obra['cep']) . "'";
		$endid = $db->pegaUm($enderecoSql);
		
		$entidadeSql = "select entid from entidade.entidade where entnumcpfcnpj = '". preg_replace('/[^0-9]/', '', $obra['unidade']) ."' AND entnome notnull AND entstatus = 'A' order by 1";
		$entid = $db->pegaUm($entidadeSql);
		
		switch ($obra['tipo']) {
			case 102 : $obra['tipoobra'] = '4'; break;
			case 103 : $obra['tipoobra'] = '3'; break;
			case 112 : $obra['tipoobra'] = '1'; break;
		}
		
		$empreendimento				= new Empreendimento();
		$emp						= array();
		$emp['prfid']				= $obra['programa'];
		$emp['moeid']				= $obra['modalidade'];
		$emp['orgid']              	= '3';
		$emp['tpoid']              	= $obra['tipo'];
		$emp['tobid']              	= $obra['tipoobra'];
		$emp['cloid']              	= $obra['classificacao'];
		$emp['endid']              	= $endid;
		$emp['empesfera']           = $obra['esfera'];
		$emp['empdsc']             	= "Construção de um bloco modular com quatro salas de aula que irão compor a Central de Salas de Aula do novo campus da UEPN em Jacarezinho, PR.";//iconv('UTF-8', 'ISO-8859-1', $obra['nome']);
		$emp['empcomposicao']      	= null;
		$emp['empjustsitdominial'] 	= null;
		$emp['empvalorprevisto']   	= $obra['valor'];
		$emp['usucpf']             	= $_SESSION['usucpf'];
		$emp['entidunidade']       	= $entid;
		$empid                     	= $empreendimento->popularDadosObjeto($emp)->salvar();
		
		$newobra					= new Obras();
		$obr						= array();
		$obr['endid']				= $endid;
		$obr['empid']				= $empid;
		$obr['iexid']				= null;
		$obr['entid']				= $entid;
		$obr['tobid']				= $obra['tipoobra'];
		$obr['tpoid']				= $obra['tipo'];
		$obr['cloid']				= $obra['classificacao'];
		$obr['tooid']				= '11';
		$obr['obrnome']				= "Construção de um bloco modular com quatro salas de aula que irão compor a Central de Salas de Aula do novo campus da UEPN em Jacarezinho, PR.";//iconv('UTF-8', 'ISO-8859-1', $obra['nome']);
		$obr['obrvalorprevisto']	= $obra['valor'];
		$obr['obrnumprocessoconv']	= preg_replace('/[^0-9]/', '', $obra['processo']);
		$obr['obranoconvenio']		= preg_replace('/[^0-9]/', '', $obra['ano']);
		$obr['numconvenio']			= preg_replace('/[^0-9]/', '', $obra['convenio']);
		$obr['preid']				= null;
		$obr['obridvinculado']		= null;
		$obrid						= $newobra->popularDadosObjeto($obr)->salvar();
		
		if($empid && $obrid) {
			$db->commit();
		}
		
		$ids['empid'][]				= $empid;
		$ids['obrid'][]				= $obrid;
	}
}

echo "Carga efetuada com sucesso.";

echo "<br>";
echo "<br>";
echo "<pre>";

print_r($ids);