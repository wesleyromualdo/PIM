<?php

$objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
$objEscolasEscolaAcessivel = new Par3_Model_EscolasEscolaAcessivel();
$modelProadesao = new Par3_Model_Proadesao();
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

if($_REQUEST['requisicao'] == 'valor_escola_checada'){
    ob_clean();        
    $modelEscolaAcessivel = new Par3_Model_EscolasEscolaAcessivel();
    $dadosSaldo = $modelEscolaAcessivel->recuperaTotalValorEscolasChecadas($_REQUEST);
    echo $dadosSaldo;
    exit();
}
// Necessário porque é ajax
foreach ($_REQUEST as $k => $v) {
    $_REQUEST[$k] = ($v);
}

// Recupera valores post
$inuid = $_REQUEST['inuid'];
$adpid = $_REQUEST['adpid'];
$adpano_ciclo = $_SESSION['par3']['adpano_ciclo'];
$prgid = $_REQUEST['prgid'];
$tipoPesquisa = $_REQUEST['tipopesquisa'];
$apta = $_POST['apta'];
$eeaselecionada = $_POST['eeaselecionada'];

$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid);

if ($inuid) {
    $objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
} else {
    die("erro");
}

$arrParamCarga['inuid'] = $inuid;
$arrParamCarga['adpid'] = $adpid;
$arrParamCarga['prgid'] = $prgid;

$dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($inuid);

$itrid = $dadosUnidade['itrid'];
$estuf = $dadosUnidade['estuf'];
$local = $dadosUnidade['inudescricao'];

//ver($_REQUEST);exit();
$arrParametros = array();
$arrayParams['itrid'] = $itrid;
$arrayParams['estuf'] = $estuf;
$arrayParams['inuid'] = $inuid;
$arrayParams['adpid'] = $adpid;
$arrayParams['adpano_ciclo'] = $adpano_ciclo;
$arrayParams['prgid'] = $prgid;
$arrayParams['inudescricao'] = $local;
$arrayParams['muncod'] = $dadosUnidade['muncod'];
$arrayParams['eeamunicipio'] = $_REQUEST['eeamunicipio'];
$arrayParams['eearede'] = $_REQUEST['eearede'];
$arrayParams['eeacod'] = $_REQUEST['eeacod'];
$arrayParams['eeanome'] = $_REQUEST['eeanome'];
$arrayParams['ordenacao'] = $_REQUEST['ordenacao'];
$arrayParams['checado'] = $_REQUEST['checado'];
$arrayParams['apta'] = $apta;
$arrayParams['eeaselecionada'] = $eeaselecionada;
//
//if ($carga_horaria == 'N') {
//    $listaInfoEscolas = $objEscolasEscolaAcessivel->getListaEscolasNaoElegiveis($arrayParams);
//    $bloqueiaSave = $listaInfoEscolas['bloqueio'];
//    $cabecalho = $listaInfoEscolas['cabecalho'];
//    $dados = $listaInfoEscolas['arrayEscolas'];
//
//    if ($tipoPesquisa == 'xls') {
//        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
//        $listagem->setCabecalho($cabecalho);
//        $listagem->setDados($dados);
//        $listagem->turnOffForm();
//        $listagem->turnOnOrdenacao();
//        $listagem->render();
//        die();
//    } else {
//        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
//        $listagem->setCabecalho($cabecalho);
//        $listagem->setDados($dados);
//        $listagem->turnOffForm();
//        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
//        $listagem->turnOnOrdenacao();
//    }
//
//} else 

if ($tipoPesquisa == 'xls') {

    $listaInfoEscolas = $objEscolasEscolaAcessivel->getListaEscolasElegiveis($arrayParams);
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
    $listaInfoEscolas = $objEscolasEscolaAcessivel->getListaEscolasElegiveis($arrayParams);
    $bloqueiaSave = $listaInfoEscolas['bloqueio'];
    $cabecalho = $listaInfoEscolas['cabecalho'];
    $dados = $listaInfoEscolas['arrayEscolas'];
    $dadosValorChecados = $listaInfoEscolas['valorEscolasChecadas'];

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->setCabecalho($cabecalho);
    $listagem->setDados($dados);
    $listagem->turnOffForm();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->turnOnOrdenacao();
}