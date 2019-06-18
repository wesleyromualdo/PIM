<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";
include_once APPRAIZ . "www/par3/_funcoes.php";
include_once APPRAIZ . "par3/classes/controller/Processo.class.inc";
include_once APPRAIZ . "par3/classes/model/Processo.class.inc";

include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';
include_once APPRAIZ.'par3/classes/controller/WS_Servico_FNDE.class.inc';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$sql = "select p.proid, p.probanco, p.proagencia, p.pronumeroprocesso, p.procnpj from par3.processo p
		where p.pronumeroprocesso not in (select empnumeroprocesso from par3.empenho where empstatus = 'A')";
$arrProcesso = $db->carregar($sql);
$arrProcesso = $arrProcesso ? $arrProcesso : array();

$sql = "SELECT sisid, tprid, tipprogramafnde, tipnumerosistemasigef, tiptipoprocessosigef FROM execucaofinanceira.tipoprocesso where tipid = 12 and tipstatus = 'A'";
$arTipProcesso = $db->pegaLinha($sql);
	
$obProcesso = new ProcessoFNDE( 'juliov', 'Mudamud0', false );

foreach ($arrProcesso as $key => $v) {
	
	$arrParamProcesso = array(
			'an_processo' 			=> date("Y"),
			'nu_processo' 			=> $v['pronumeroprocesso'],
			'tp_processo' 			=> $arTipProcesso['tiptipoprocessosigef'],
			'co_programa_fnde' 		=> $arTipProcesso['tipprogramafnde'],
			'nu_sistema'			=> $arTipProcesso['tipnumerosistemasigef'],
			'nu_cnpj_favorecido' 	=> $v['procnpj'],
			'proid' 				=> $v['proid'],
			'ws_usuario'			=> 'juliov',
			'ws_senha' 				=> 'Mudamud0',
			'nu_banco' 				=> $v['probanco'],
			'nu_agencia'			=> $v['proagencia'],
	);
	
	$res_sp = $obProcesso->solicitarProcesso($arrParamProcesso);
}