<?php
set_time_limit(0);
ini_set("memory_limit", "-1");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";

/*echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';*/

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$sql = "SELECT ur.rpuid, ur.usucpf, ur.entid 
        FROM par3.usuarioresponsabilidade ur
        	INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
        WHERE rpustatus = 'A' AND entid IS NOT NULL";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
    
    $entid = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entstatus = 'A' AND entnumcpfcnpj = '{$v['usucpf']}'");
    
    if( empty($entid) ){
        $sql = "INSERT INTO entidade.entidade(entnumcpfcnpj, entnome, entemail, entstatus, entnumrg, entorgaoexpedidor, entsexo, entdatanasc, entdatainclusao)
                (SELECT entcpf, entnome, entemail, 'A', entrg, substring(entorgexpedidor, 1,15) AS entorgexpedidor, entsexo, entdtnascimento, entdtinclusao FROM par3.instrumentounidade_entidade WHERE entcpf = '{$v['usucpf']}' AND entstatus = 'A' ORDER BY entdtinclusao DESC) RETURNING entid;";
        $entid = $db->pegaUm($sql);
    }
    
    if( $entid ){
        $sql = "UPDATE par3.usuarioresponsabilidade SET 
                    entid = $entid
                WHERE rpuid = {$v['rpuid']}";
        
        $db->executar($sql);
        $db->commit();
    }
}
