<?php

set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";
require_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . 'www/par3/_constantes.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$sql = "SELECT DISTINCT ip.inpid, itc.ipiano
    	FROM par3.iniciativa_planejamento ip
    		INNER JOIN par3.iniciativa_planejamento_item_composicao itc ON itc.inpid = ip.inpid AND itc.ipistatus = 'A'
    		INNER JOIN workflow.documento d ON d.docid = ip.docid
    	WHERE ip.inpstatus = 'A'
    		AND d.esdid in (2069, 2077)";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
    
    $tot_analise = $db->pegaUm("SELECT count(anaid) FROM par3.analise WHERE anaano = {$v['ipiano']} AND anastatus = 'A' and inpid = {$v['inpid']} and intaid = 1");
    
    if( (int)$tot_analise == (int)0 ){
        $docdsc = "Fluxo de Iniciativas do PAR3 - Análise Planejamento ";
        $docid_analise = wf_cadastrarDocumento(310, $docdsc, '2070');
        
        $sql = "INSERT INTO par3.analise(anaano, docid, anadatacriacao, anastatus, intaid, edeid, inpid)
                VALUES('".$v['ipiano']."',  $docid_analise, now(), 'A', 1, null, {$v['inpid']})";
        
        $db->executar($sql);
        $db->commit();
    }
}