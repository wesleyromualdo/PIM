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

$sql = "SELECT emeid, emecod, autid, acaid, resid, emetipo, emeano, emedescentraliza, emelibera, emevalor, etoid, emerelator, ememomentosiop, emepo, emeaprovacaoreitor, emeeditapar
        FROM emenda.emenda WHERE emeano = '2018' AND etoid = 1";
$arrDados = $db->carregar($sql);

$arrRegistro = array();
foreach ($arrDados as $v) {
    $sql = "SELECT emdid, emeid, gndcod, foncod, mapcod, emdvalor,  emdstatus, emddotacaoinicial FROM emenda.emendadetalhe WHERE emeid = {$v['emeid']} and emdstatus = 'A' ORDER BY emdid ASC";
    $arrDetalhe = $db->carregar($sql);
    $arrDetalhe = $arrDetalhe ? $arrDetalhe : array();
    
    $sql = "SELECT emdid, emeid, gndcod, foncod, mapcod, emdvalor,  emdstatus, emddotacaoinicial FROM emenda.emendadetalhe WHERE emeid = {$v['emeid']} and emdstatus = 'I' ORDER BY emdid ASC";
    $arrDetalheInativo = $db->carregar($sql);
    $arrDetalheInativo = $arrDetalheInativo ? $arrDetalheInativo : array();
    
    if( sizeof($arrDetalheInativo) == (int)1 && sizeof($arrDetalhe) == (int)1 ){
        $emdidInativo = $arrDetalheInativo[0]['emdid'];
        
        foreach ($arrDetalhe as $detalhe) {
            $emdidAtivo = $detalhe['emdid'];
            
            if( (int)$emdidAtivo > (int)$emdidInativo){
                $sql = "SELECT edeid, emdid, enbid, edevalor, edestatus, ededisponivelpta, edevalordisponivel, edejustificativasiop, edeprioridade, edelimiteempenhobeneficiario
                        FROM emenda.emendadetalheentidade WHERE emdid = $emdidAtivo AND edestatus = 'A'";
                $arrEntidadeAtivo = $db->carregar($sql);
                $arrEntidadeAtivo = $arrEntidadeAtivo ? $arrEntidadeAtivo : array();
                
                foreach ($arrEntidadeAtivo as $ent) {
                    $edeidAtivo = $ent['edeid'];
                    
                    $sql = "SELECT edeid FROM emenda.emendadetalheentidade WHERE emdid = $emdidInativo and enbid = {$ent['enbid']} AND edestatus = 'I'";
                    $arrEntInativa = $db->carregar($sql);
                    
                    if( sizeof($arrEntInativa) == (int)1 ){
                        
                        $edeidInativo = $arrEntInativa[0]['edeid'];
                        
                        array_push($arrRegistro, array(
                            'emeid' => $v['emeid'],
                            'emecod' => $v['emecod'],
                            'emdid_ativo' => $emdidAtivo,
                            'emdid_inativo' => $emdidInativo,
                            'edeid_ativo' => $edeidAtivo,
                            'edeid_inativo' => $edeidInativo,
                        ));
                        $sql = "UPDATE par3.iniciativa_emenda SET edeid = $edeidAtivo WHERE edeid = $edeidInativo AND inestatus = 'A';<br>
                                UPDATE par3.analise SET edeid = $edeidAtivo WHERE edeid = $edeidInativo AND anastatus = 'A';<br>
                                UPDATE emenda.ptemendadetalheentidade SET edeid = $edeidAtivo WHERE edeid = $edeidInativo; <br>";
                        echo $sql.'<br>';
                        $db->executar($sql);
                    }
                }
            }
        }
    }
}
$db->commit();
ver($arrRegistro);