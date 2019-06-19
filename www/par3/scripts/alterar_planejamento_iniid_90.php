<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

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

$sql = "SELECT DISTINCT
        	inp.inpid,
            inp.iniid,
            iu.inuid,
            case when iu.muncod is null then est.estuf else mun.estuf end as uf,
            case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
            (inp.iniid||' - '||ind.inddsc) as ds_iniciativa,
            '&inuid='||iu.inuid||'&aba=cadastro&inpid='||inp.inpid
        FROM par3.iniciativa_planejamento as inp
        	INNER JOIN par3.iniciativa_planejamento_item_composicao itc ON itc.inpid = inp.inpid
            inner join par3.instrumentounidade iu on iu.inuid = inp.inuid and iu.inustatus = 'A'
            INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
            INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
            LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
            LEFT JOIN territorios.estado est on est.estuf = iu.estuf
        WHERE inpstatus = 'A'
        	AND ini.iniid = 90";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$sql = "SELECT iig.iigid, itc.itcid, id.itdvalor, itc.itcdsc,itc.itcespecificacao,ctg.ctddsc,und.unidsc,itc.itcsituacao as situacao
        FROM par3.iniciativa_itenscomposicao_grupo iig
        	INNER JOIN par3.itenscomposicao     itc ON itc.itcid = iig.itcid
        	INNER JOIN par3.categoriadespesa    ctg ON ctg.ctdid = itc.ctdid
        	INNER JOIN par3.unidade_medida      und ON und.uniid = itc.uniid
        	INNER JOIN par3.itenscomposicao_detalhamento id ON id.itcid = itc.itcid
        WHERE iig.iigsituacao = 'A' AND iig.iniid = 90 AND itc.itcid IN (465, 464, 469)";
$arrDadosItem = $db->carregar($sql);
$arrDadosItem = $arrDadosItem ? $arrDadosItem : array();

foreach ($arrDados as $v) {
    
    $sql = "SELECT DISTINCT ic.inpid, ic.ipiquantidade, ic.ipiano
            FROM par3.iniciativa_planejamento_item_composicao ic
            WHERE ic.inpid = {$v['inpid']}
            	AND ic.ipistatus = 'A'
            ORDER BY ic.ipiano;";
    $arrItem = $db->carregar($sql);
    $arrItem = $arrItem ? $arrItem : array();
    
    foreach ($arrItem as $item) {
        
        $sql = "SELECT DISTINCT ic.ipiid, COALESCE(ipiquantidade,0) as ipiquantidade, ipivalorreferencia, ipiano
                FROM par3.iniciativa_planejamento_item_composicao ic
                WHERE ic.inpid = {$item['inpid']} AND ipiano = {$item['ipiano']}
                	AND ic.ipistatus = 'A'";
        $arComposicao = $db->pegaLinha($sql);
        
        foreach ($arrDadosItem as $valor) {
            $boTem = $db->pegaUm("SELECT count(ipiid) FROM par3.iniciativa_planejamento_item_composicao ic WHERE ic.inpid = {$item['inpid']} AND ipiano = {$arComposicao['ipiano']} AND iigid = {$valor['iigid']} AND ic.ipistatus = 'A'");
            
            if( (int)$boTem == (int)0 ){
                $sql = "INSERT INTO par3.iniciativa_planejamento_item_composicao(inpid, ipivalorreferencia, ipiquantidade, ipiano, iigid, ipistatus)
                    VALUES({$item['inpid']}, {$valor['itdvalor']}, {$arComposicao['ipiquantidade']}, {$arComposicao['ipiano']}, {$valor['iigid']}, 'A') returning ipiid";
                $ipiid = $db->pegaUm($sql);
                
                $sql = "SELECT ipequantidade, co_entidade, escid FROM par3.iniciativa_planejamento_item_composicao_escola WHERE ipiid = {$arComposicao['ipiid']} and ipestatus = 'A'";
                $arrItemEscola = $db->carregar($sql);
                $arrItemEscola = $arrItemEscola ? $arrItemEscola : array();
                foreach ($arrItemEscola as $escola) {
                    $sql = "INSERT INTO par3.iniciativa_planejamento_item_composicao_escola(ipiid, ipequantidade, co_entidade, escid)
                            VALUES($ipiid, {$escola['ipequantidade']}, {$escola['co_entidade']}, {$escola['escid']})";
                    $db->executar($sql);
                }
            }
        }
    }
    $db->commit();
}