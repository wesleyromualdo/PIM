<?php /* ****arquivo vazio****  */

require APPRAIZ . "obras2/includes/principal/cadObra/funcoes.php";



$orgid = $_SESSION['obras2']['orgid'];
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int)($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];

$obridVinculado = $_REQUEST['obridVinculado'];
if ($obridVinculado) {
    $obraVinculada = new Obras();
    $obraVinculada->carregarPorIdCache($obridVinculado);


    $esdid = pegaEstadoObra($obraVinculada->docid);

    if ($esdid != ESDID_OBJ_PARALISADO) {
        die("<script>
                    location.href = '?modulo=principal/cadObra&acao=A&obrid=" . $obridVinculado . "'
             </script>");
    }

    $_SESSION['obras2']['empid'] = $obraVinculada->empid;
    $_SESSION['obras2']['obrid'] = '';
}
 
switch ($_REQUEST['requisicao']) {
    case 'obshistoricoparalisacao':
        require_once(APPRAIZ . "obras2/modulos/principal/obshistorico.inc");
        die;
}

if ($_REQUEST['requisicao'] == 'inserirrmarcacaoorgcontrole') {

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);

    $obra->obrorgcontrole = 't';
    $obra->salvar();
}
if ($_REQUEST['requisicao'] == 'retirarmarcacaoorgcontrole') {
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $obra->obrorgcontrole = 'f';
    $obra->salvar();
}
if ($_REQUEST['requisicao'] == 'inserirrmarcacaocontabloqueada') {

    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $obra->obrcontabloqueada = 't';
    $obra->salvar();
}
if ($_REQUEST['requisicao'] == 'retirarmarcacaocontabloqueada') {
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $obra->obrcontabloqueada = 'f';
    $obra->salvar();
}

if ($_REQUEST['requisicao'] == 'inserirrmarcacaoprocessoanterior') {
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $obra->obrprocessoanterior = 't';
    $obra->salvar();
}
if ($_REQUEST['requisicao'] == 'retirarmarcacaoprocessoaterior') {
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    $obra->obrprocessoanterior = 'f';
    $obra->salvar();
}











if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}

// InclusÃ£o de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$_SESSION['obras2']['empid'] = ($_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid']);

if ($_GET['acao'] != 'V') {
    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";

//  $arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );
    $arMenuBlock = array();

    if (!$obrid && !$empid) {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP, $url, $parametros, $arMenuBlock);
    } elseif ($obrid) {
        if ($orgid == ORGID_EDUCACAO_BASICA) {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, $arMenuBlock);
        } else {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros, $arMenuBlock);
        }
    } else {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA, $url, $parametros, $arMenuBlock);
    }

    $habilitado = true;
    $habilita = 'S';
} else {
    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script>jQuery.noConflict();</script>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>


    <?php
    $_SESSION['obras2']['obrid'] = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
    $db->cria_aba($abacod_tela, $url, $parametros);
//  $somenteLeitura = false;
    $habilitado = false;
    $habilita = 'N';
}


//demanda 311298 -- O perfil call center nÃ£o pode alterar/inserir nenhuma informaÃ§Ã£o.
if (possui_perfil(array(PFLCOD_CALL_CENTER))) {
    $habilitado = false;
    $habilita = 'N';
}
$obra = new Obras();
$obra->carregarPorIdCache($_SESSION['obras2']['obrid']);

$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];
$empreendimento = new Empreendimento($_SESSION['obras2']['empid']);
$empid = $empreendimento->empid;

if (empty($_SESSION['obras2']['obrid'])) {
    $dadoEmpreendimento = $empreendimento->getDados();
}

if ($obra->obrid) {
    $habilitado = false;
    $habilita = 'N';
}

$orgid = $_SESSION['obras2']['orgid'];
$orgao = new Orgao($orgid);
$tipologiaObra = new TipologiaObra();
$programaFonte = new ProgramaFonte();
$modalidadeEnsino = new ModalidadeEnsino();
$tipoObra = new TipoObra();
$classificacaoObra = new ClassificacaoObra();
$tipoOrigemObra = new TipoOrigemObra();
$endereco = new Endereco(($obra->endid ? $obra->endid : $dadoEmpreendimento['endid']));
$municipio = new Municipio($endereco->muncod);



