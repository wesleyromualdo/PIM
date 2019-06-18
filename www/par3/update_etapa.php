<?php
error_reporting(ALL);
ini_set('display_errors', 'On');
date_default_timezone_set('America/Sao_Paulo');

$_REQUEST['baselogin'] = 'simec_espelho_producao';

/* configurações */
ini_set('memory_limit', '10048M');
set_time_limit(0);

require_once 'config.inc';

require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
require_once APPRAIZ . 'www/par3/_funcoes.php';

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once APPRAIZ . "includes/library/simec/Listagem.php";
include_once APPRAIZ . "includes/workflow.php";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
include_once 'autoload.php';

include_once APPRAIZ . 'includes/library/simec/view/Helper.php';

define('FASE_DIAGNOTICO', 3953);

initAutoload();
$simec = new Simec_View_Helper();
$db = new cls_banco();
$alterados=0;
$naoAlterados=0;
$inuids=[];

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 231;

if (!$_GET['page']) {
    $page = 0;
} else {
    $page = $_GET['page'];
}

$offset = (1000 * $page);

$strSQL = <<<DML
    SELECT
        inu.inuid as id, inu.docid, inu.itrid
    FROM
        par3.instrumentounidade inu
        left join workflow.documento doc
            inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
        on doc.docid = inu.docid and doc.tpdid = 243
        left join territorios.municipio mun ON mun.muncod = inu.muncod
    WHERE
       itrid = 2
       AND esd.esdid = 1638
    ORDER BY id ASC LIMIT 1000 OFFSET {$page}
DML;

$rs = $db->carregar($strSQL);
$areas = getAreas();
$totalPagina = count($rs);
if ($totalPagina && $rs) {
    foreach ($rs as $row) {
        if (!empty($row['id'])) {
            $diagnostico = checkPreenchimento($row['id'], $areas);
            if ($diagnostico['percent'] > 0) {
                $alterados++;
                array_push($inuids, $row['id']);
                //$db->executar("UPDATE par3.auditoria_unidade SET adutipo = 2 WHERE inuid = {$row['id']}")
                /*if (wf_alterarEstado($row['docid'], FASE_DIAGNOTICO, $cmd='', ['docid' => $row['docid']])) {
                    $alterados++;
                } else {
                    $naoAlterados++;
                }*/
            } else {
                $naoAlterados++;
            }
            $totalRegistro++;
        }
    }
}

function checkPreenchimento($inuid, $areas) {
    $oAreaController = new Par3_Controller_Area();
    $result = array();

    if (is_array($areas)) {
        foreach ($areas as $area) {
            $dadosPercent = $oAreaController->pontuacaoArea($area, $inuid);
            $indicadores = $indicadores + $dadosPercent['indicadores'];
            $pontuados = $pontuados + $dadosPercent['pontuados'];
        }
        $result['indicadores'] = $indicadores;
        $result['pontuados'] = $pontuados;
        $result['percent'] = $result['indicadores'] > 0 ? ($result['pontuados']*100)/$result['indicadores'] : 0;
        unset($oAreaController);
    }

    return $result;
}

function getAreas() {
    global $db;

    $sql = "
        SELECT DISTINCT
            are.areid
        FROM
            par3.dimensao dim
        INNER JOIN par3.area are ON (are.dimid = dim.dimid AND are.arestatus = 'A')
        WHERE
            dim.dimstatus = 'A'
            AND dim.itrid = 2
        ORDER BY
            are.areid
    ";

    return $db->carregarColuna($sql);
}

if ($page < 4) {
    $page++;
    $_SESSION['alterados'] += $alterados;
    $_SESSION['naoAlterados'] += $naoAlterados;
    $_SESSION['totalLidos'] += $totalRegistro;
    echo "<script type='text/javascript'> location.href= '/par3/update_etapa.php?page=".$page."'; </script>";
} else {
    echo "<h1>Total de registro econtrados: {$_SESSION['totalLidos']}</h1>";
    echo "<h3 style='color: green;'>Total de tramitações: {$_SESSION['alterados']}</h3>";
    echo "<h3 style='color: red;'>Registros não alterados: {$_SESSION['naoAlterados']}</h3>";
    unset($_SESSION['alterados'], $_SESSION['naoAlterados'], $_SESSION['totalLidos']);
    //ver($inuids);
}

