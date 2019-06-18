<?php

$perfis = pegaPerfilGeral($_SESSION['usucpf']);

// Necessário porque é ajax
foreach ($_REQUEST as $k => $v) {
    $_REQUEST[$k] = ($v);
}

// Recupera valores post
$inuid = $_REQUEST['inuid'];
$adpid = $_REQUEST['adpid'];
$adpano_ciclo = $_SESSION['par3']['adpano_ciclo'];
$carga_horaria = $_REQUEST['carga_horaria'];
$prgid = $_REQUEST['prgid'];
$tipoPesquisa = $_REQUEST['tipopesquisa'];

$objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
$objEscolasMaisAlfabetizacao = new Par3_Model_EscolasMaisAlfabetizacao();
$modelProadesao = new Par3_Model_Proadesao();
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid);

if ($inuid) {
    $objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
} else {
    die("erro");
}

$arrParamCarga['inuid'] = $inuid;
$arrParamCarga['adpid'] = $adpid;
$arrParamCarga['prgid'] = $prgid;
$arrParamCarga['carga_horaria'] = $carga_horaria;

$dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($inuid);

$itrid = $dadosUnidade['itrid'];
$estuf = $dadosUnidade['estuf'];
$local = $dadosUnidade['inudescricao'];


$arrParametros = array();
$arrayParams['carga_horaria'] = $carga_horaria;
$arrayParams['itrid'] = $itrid;
$arrayParams['estuf'] = $estuf;
$arrayParams['inuid'] = $inuid;
$arrayParams['adpid'] = $adpid;
$arrayParams['adpano_ciclo'] = $adpano_ciclo;
$arrayParams['prgid'] = $prgid;
$arrayParams['inudescricao'] = $local;
$arrayParams['muncod'] = $dadosUnidade['muncod'];
$arrayParams['emamunicipio'] = $_REQUEST['emamunicipio'];
$arrayParams['emarede'] = $_REQUEST['emarede'];
$arrayParams['emacod'] = $_REQUEST['emacod'];
$arrayParams['emanome'] = $_REQUEST['emanome'];
$arrayParams['aeacargahoraria'] = $_REQUEST['carga_horaria'];
$arrayParams['ordenacao'] = $_REQUEST['ordenacao'];
$arrayParams['checado'] = $_REQUEST['checado'];

if ($carga_horaria == 'N') {
    $listaInfoEscolas = $objEscolasMaisAlfabetizacao->getListaEscolasNaoElegiveis($arrayParams);
    $bloqueiaSave = $listaInfoEscolas['bloqueio'];
    $cabecalho = $listaInfoEscolas['cabecalho'];
    $dados = $listaInfoEscolas['arrayEscolas'];

    if ($tipoPesquisa == 'xls') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($dados);
        $listagem->turnOffForm();
        $listagem->turnOnOrdenacao();
        $listagem->render();
        die();
    } else {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($dados);
        $listagem->turnOffForm();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
        $listagem->turnOnOrdenacao();
    }

} else if ($tipoPesquisa == 'xls') {

    $listaInfoEscolas = $objEscolasMaisAlfabetizacao->getListaEscolasElegiveis($arrayParams);
    $bloqueiaSave = $listaInfoEscolas['bloqueio'];
    $cabecalho = $listaInfoEscolas['cabecalho'];
    $dados = $listaInfoEscolas['arrayEscolas'];

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    $listagem->setCabecalho($cabecalho);
    $listagem->setDados($dados);
    $listagem->turnOffForm();
    $listagem->turnOnOrdenacao();
    $listagem->render();
    die();
} else {
    $listaInfoEscolas = $objEscolasMaisAlfabetizacao->getListaEscolasElegiveis($arrayParams);

    $bloqueiaSave = $listaInfoEscolas['bloqueio'];
    $cabecalho = $listaInfoEscolas['cabecalho'];
    $dados = $listaInfoEscolas['arrayEscolas'];

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->setCabecalho($cabecalho);
    $listagem->setDados($dados);
    $listagem->turnOffForm();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->turnOnOrdenacao();
}