<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$sql = "SELECT distinct ie.emeid,  ve.emecod, '3' AS resid, '3' AS mdeid, ve.entid
        FROM par3.iniciativa_emenda ie
        	INNER JOIN emenda.v_emendadetalheentidade ve ON ve.edeid = ie.edeid AND ve.emeano = '".date('Y')."'
        	INNER JOIN workflow.documento d ON d.docid = ie.docid
        WHERE ie.inestatus = 'A'
        	AND  d.esdid = 2117
        	AND ie.edeid NOT IN (SELECT edeid FROM emenda.planotrabalho p
        						INNER JOIN emenda.ptemendadetalheentidade pe ON pe.ptrid = p.ptrid and p.ptrexercicio = '".date('Y')."')";
$arrDados = $db->carregar($sql);

foreach ($arrDados as $v) {
    
    $ptridpai = 'null';
    $ptrcod = ($db->pegaUm ( "SELECT max(ptrcod) + 1 FROM emenda.planotrabalho" ));
    
    $resid = $v['resid'];
    $enbid = $v['entid'];
    $mdeid = $v['mdeid'];
    
    $sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrstatus, resid, mdeid, ptridpai, ptrcod, ptrsituacao, sisid )
			VALUES ( {$enbid}, " . date ( 'Y' ) . ", 'A', {$resid}, {$mdeid}, " . $ptridpai . ", " . $ptrcod . ", 'E', 23) RETURNING ptrid";
    $ptrid = $db->pegaUm ( $sql );
    
    $sql = "SELECT distinct ie.emeid, ie.edeid, ve.emecod, '3' AS resid, '3' AS mdeid, ve.entid, ve.edevalor
            FROM par3.iniciativa_emenda ie
            	INNER JOIN emenda.v_emendadetalheentidade ve ON ve.edeid = ie.edeid AND ve.emeano = '".date('Y')."'
            	INNER JOIN workflow.documento d ON d.docid = ie.docid
            WHERE ie.inestatus = 'A'
            	AND  d.esdid = 2117
            	AND ie.emeid = {$v['emeid']} AND ve.entid = {$v['entid']}
            	AND ie.edeid NOT IN (SELECT edeid FROM emenda.planotrabalho p
            						INNER JOIN emenda.ptemendadetalheentidade pe ON pe.ptrid = p.ptrid and p.ptrexercicio = '".date('Y')."')";
    $arrEmenda = $db->carregar($sql);
    $arrEmenda = $arrEmenda ? $arrEmenda : array();
            
    foreach ( $arrEmenda as $arr ) {
        $edeid = $arr['edeid'];
        
        $sql = "INSERT INTO emenda.ptemendadetalheentidade(ptrid, edeid, pedvalor)
				VALUES ($ptrid, $edeid, {$arr['edevalor']})";
        $db->executar ( $sql );
    }
    ob_clean ();
    if ($db->commit ()) {
        include_once APPRAIZ . 'includes/workflow.php';
        // cria o docid (workflow) do PTA
        $tpdid = 8;
        $docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
        $docid = wf_cadastrarDocumento ( $tpdid, $docdsc );
        
        $sql = "UPDATE emenda.planotrabalho SET docid = " . $docid . " WHERE ptrid = " . $ptrid;
        $db->executar ( $sql );
        $db->commit ();
    }
    
}