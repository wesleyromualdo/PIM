<?php /* ****arquivo vazio**** */


$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

switch ($_REQUEST['op']) {
    case 'download':
        $arqid = $_GET['arqid'];
        if ($arqid) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec(null, null, "obras2");
            $file->getDownloadArquivo($arqid);
        }
        die();
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$obra = new Obras();
$obra->carregarPorIdCache($obrid);
$crtid = $obra->pegaContratoPorObra($obrid);
$contrato = new Contrato($crtid);

$dados = $contrato->getDados();

if ($dados['crtid']) {
    $dados['crtdtassinatura'] = formata_data($dados['crtdtassinatura']);
    $dados['dt_cadastro'] = formata_data($dados['dt_cadastro']);
    $dados['crtdttermino'] = formata_data($dados['crtdttermino']);
    $dados['crtvalorexecucao'] = number_format($dados['crtvalorexecucao'], 2, ',', '.');
    $dados['crtpercentualdbi'] = number_format($dados['crtpercentualdbi'], 2, ',', '.');

    extract($dados);

    $obrvalorprevisto = number_format($obra->obrvalorprevisto, 2, ',', '.');

    $empresa = new Entidade($entidempresa);
    if($empresa->entnumcpfcnpj != '' || $empresa->entnome != ''){
        $entnomeempresa = "(" . mascaraglobal($empresa->entnumcpfcnpj, "##.###.###/####-##") . ") " . $empresa->entnome;
    }
      $entidempresa = $empresa->getPrimaryKey();
}

$habilitaAditivo = false;
if ($_GET['acao'] != 'V') {
    // InclusÃ£o de arquivos padrÃ£o do sistema
    include APPRAIZ . 'includes/cabecalho.inc';
    // Cria as abas do mÃ³dulo
    echo '<br>';

    $arMenuBlock = array();

    if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, $arMenuBlock);
    } else {
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros, $arMenuBlock);
    }

    $esdid = pegaEstadoObra($obra->docid);
    if ($esdid == ESDID_OBJ_CONTRATACAO /*|| $esdid == ESDID_OBJ_ADITIVO*/) {
        $habilitado = true;
        $habilita = 'S';
    } else {
        $habilitado = false;
        $habilita = 'N';
    }
    if ($esdid == ESDID_OBJ_EXECUCAO || $esdid == ESDID_OBJ_ADITIVO) {
        $habilitaAditivo = true;
    }
} else {
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>


    <?php
    $db->cria_aba($abacod_tela, $url, $parametros);

    $habilitado = false;
    $habilita = 'N';

}

if (possui_perfil(array(
    PFLCOD_CONSULTA_UNIDADE,
    PFLCOD_CONSULTA_ESTADUAL,
    PFLCOD_CALL_CENTER,
    PFLCOD_CONSULTA_TIPO_DE_ENSINO,
    PFLCOD_GESTOR_MEC
))) {
    $habilitado = false;
    $habilita = 'N';
}

if (!$obra->obrperccontratoanterior) {

    /*Regra de bloqueio retirada a pedido do FERDINANDO FURLANI na ss 4114
     *

    $habilitado = false;
    $habilita   = 'N';
    */
}

$execucaoFinanceira = new ExecucaoFinanceira();
$seloContratoAdicional = ($execucaoFinanceira->existeContratoExtra($obrid)) ? '<div style="position: absolute; top: 540px; right: 130px; z-index:1;">
                    <img border="0" title="Esta obra possui contrato adicional na aba ExecuÃ§Ã£o Financeira." src="../imagens/carimbo-contrato.png">
                </div>' : '';


$docid = pegaDocidObra($obrid);

