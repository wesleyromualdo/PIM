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

$sql = "SELECT DISTINCT ve.emecod, ve.emeano, ve.edeid, ve.emeid, e.inpid, d.esdid
        FROM par3.iniciativa_emenda e
        	INNER JOIN workflow.documento d ON d.docid = e.docid
        	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
        	INNER JOIN emenda.v_emendadetalheentidade ve ON ve.edeid = e.edeid
        WHERE e.inestatus = 'A'
        	AND d.esdid = 2117
        	AND ve.edeid NOT IN (SELECT DISTINCT edeid FROM par3.analise WHERE anastatus = 'A' AND edeid IS NOT null)";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();


foreach ($arrDados as $eme) {

    //$sql = $db->sqlTramitePlanejamento($arrPost['edeid'], $arrPost['esdid_atual']);
    $sql = "SELECT DISTINCT COALESCE(eo.tot_obra,0) AS tot_obra, e.inpid, ip.docid, d1.esdid, ae.aedid, eic.qtd
                FROM par3.iniciativa_emenda e
                	INNER JOIN workflow.documento d ON d.docid = e.docid
                	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = e.inpid AND ip.inpstatus = 'A'
                	INNER JOIN workflow.documento d1 ON d1.docid = ip.docid
                	left JOIN workflow.acaoestadodoc ae ON ae.esdidorigem = d1.esdid AND ae.esdiddestino = ".PAR3_ESDID_AGUARDANDO_ANALISE."
                	LEFT JOIN (
                		SELECT count(eo.ieoid) AS tot_obra, eo.inpid FROM par3.iniciativa_emenda_obra eo
                		INNER JOIN par3.obra o ON o.obrid = eo.obrid
                		WHERE o.obrstatus = 'A' AND eo.ieostatus = 'A'
                		GROUP BY eo.inpid
                	) eo ON eo.inpid = e.inpid
                    INNER JOIN(
                		SELECT ineid, sum(qtd) AS qtd FROM(
							SELECT ic.ineid, ic.ieiquantidade AS qtd FROM par3.iniciativa_emenda_item_composicao ic WHERE ic.ieistatus = 'A'
							UNION ALL
							SELECT eo.ineid, eo.ieovalor AS qtd FROM par3.iniciativa_emenda_obra eo WHERE eo.ieostatus = 'A'
						) AS foo GROUP BY ineid
					) eic ON eic.ineid = e.ineid
                WHERE e.inestatus = 'A' AND e.edeid = {$eme['edeid']} and d.esdid = 2117 AND eic.qtd > 0";
    
    $arrPlanejamento = $db->carregar($sql);
    $arrPlanejamento = $arrPlanejamento ? $arrPlanejamento : array();
    
    /*
     * Tramita os planejamentos vinculados a emenda para Aguardando Analise
     * */
    foreach ($arrPlanejamento as $v) {
        if( (int)$v['tot_obra'] == (int)0 ){
            
            if( !empty($v['aedid']) && ($v['esdid'] != PAR3_ESDID_AGUARDANDO_ANALISE) ){
                $arrParamWork = array('inpid' => $v['inpid']);
                wf_alterarEstado( $v['docid'], $v['aedid'], 'Fluxo de Iniciativas Planejamento do PAR3', $arrParamWork );
            }
            
            $tot_analise = $db->pegaUm("SELECT count(anaid) FROM par3.analise WHERE edeid = {$eme['edeid']} AND anastatus = 'A' and inpid = {$v['inpid']}");
            
            if( (int)$tot_analise == (int)0 ){
                $docdsc = "Fluxo de Iniciativas do PAR3 - Análise Planejamento ";
                $docid_analise = wf_cadastrarDocumento(310, $docdsc, '2070');
                
                $sql = "INSERT INTO par3.analise(anaano, docid, anadatacriacao, anastatus, intaid, edeid, inpid)
                                    VALUES('".date('Y')."',  $docid_analise, now(), 'A', 2, ".$eme['edeid'].", {$v['inpid']})";
                echo $sql.'<br>';
                $db->executar($sql);
                $db->commit();
            }
        }
    }
    $db->commit();
    
    $boPTA = $db->pegaUm("SELECT count(pedid) FROM emenda.ptemendadetalheentidade WHERE edeid = {$eme['edeid']}");
    if( (int)$boPTA == (int)0 ){
        $arrEnt = $db->pegaLinha("SELECT enbid, edevalor FROM emenda.emendadetalheentidade WHERE edeid = {$eme['edeid']} AND edestatus = 'A'");
        
        $ptridpai = 'null';
        $ptrcod = ($db->pegaUm ( "SELECT max(ptrcod) + 1 FROM emenda.planotrabalho" ));
        
        $resid = 3;
        $mdeid = 3;
        $enbid = $arrEnt['enbid'];
        $edevalor = $arrEnt['edevalor'];
        
        $ptrid = $db->pegaUm("SELECT pt.ptrid FROM emenda.ptemendadetalheentidade pt
                                                    	INNER JOIN emenda.v_emendadetalheentidade v ON v.edeid = pt.edeid AND v.edestatus = 'A'
                                                    WHERE v.emeid = {$eme['emeid']}");
        
        if( empty($ptrid) && !empty($enbid) ){
            $sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrstatus, resid, mdeid, ptridpai, ptrcod, ptrsituacao, sisid )
        			             VALUES ( {$enbid}, " . date ( 'Y' ) . ", 'A', {$resid}, {$mdeid}, " . $ptridpai . ", " . $ptrcod . ", 'E', 23) RETURNING ptrid";
            $ptrid = $db->pegaUm ( $sql );
        }
        
        if( !empty($ptrid) && !empty($enbid) ){
        $sql = "INSERT INTO emenda.ptemendadetalheentidade(ptrid, edeid, pedvalor)
    				        VALUES ($ptrid, {$eme['edeid']}, {$edevalor})";
        $db->executar ( $sql );
        }
        
        if( $ptrid ){
            if ($db->commit ()) {
                // cria o docid (workflow) do PTA
                $tpdid = 8;
                $docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
                $docid = wf_cadastrarDocumento ( $tpdid, $docdsc );
                
                $sql = "UPDATE emenda.planotrabalho SET docid = " . $docid . " WHERE ptrid = " . $ptrid;
                $db->executar ( $sql );
                $db->commit ();
            }
        }
    }
}