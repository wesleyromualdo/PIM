<?php 

require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraPagamentos/funcoes.php';

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

if ($empid && empty($obrid)) {
    $obr = new Obras();
    $obrid = $obr->pegaIdObraPorEmpid($empid, array('not(obridpai)' => true));

    $obrid = (is_array($obrid) && count($obrid)) ? $obrid[0] : $obrid;

    if (empty($obrid)) {
        die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
    }
}

$obra = new Obras($obrid);
$contrato = new Contrato($crtid);

$crtid = $obra->pegaContratoPorObra($obrid);
$dados = $contrato->getDados();

if ($dados['crtid']) {
    $empresa = new Entidade($dados['entidempresa']);
    $entnomeempresa = "(" . mascaraglobal($empresa->entnumcpfcnpj, "##.###.###/####-##") . ") " . $empresa->entnome;
}

$empreendimento = new Empreendimento($empid);
$execOrcamento = new ExecucaoOrcamentaria();




include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

switch ($_REQUEST['requisicao']) {

    case 'excluirConstrutora':
        ob_clean();
        $cexid = $_REQUEST['cexid'];

        if (excluirConstrutora($cexid)) {
            echo "Construtora excluída com sucesso.";
            exit;
        }
        echo "Falha ao excluir a construtora";
        exit;
        break;
    case "download":
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
        echo "<script>window.location.href = '" . $_SERVER['HTTP_REFERER'] . "';</script>";
        exit;
        break;
    case "excluirPagamento":
        ob_clean();

        $pgtid = $_REQUEST["pgtid"];

        if ($pgtid) {
            $sql = "UPDATE obras2.pagamentotransacao SET pgtstatus = 'I' WHERE pgtid = {$pgtid}";
            $executou = $db->executar($sql);

            $sql1 = "UPDATE obras2.documentotransacao SET dotstatus = 'I' WHERE pgtid = {$pgtid}";
            $executou1 = $db->executar($sql1);

            if (($db->commit()) && ($executou) && ($executou1)) {
                echo "Registro excluído com sucesso!";
                exit;
            } else {
                echo "Erro ao excluir o registro!";
                exit;
            }
        } else {
            echo "Erro ao excluir o registro!";
            exit;
        }

        break;
}

$param = array("teoid" => TIPO_EXEC_ORCAMENTARIA_OBRA);

if ($_GET['acao'] == "A") {
    verificaSessao('empreendimento');

    extract($execOrcamento->carregaPorEmpid($empid, $param));
    $msgTitle = (!$eorid ? 'Salve o orçamento para o empreendimento e então será liberado o cadastro de detalhamento do orçamento' : '');
    $displayEmpenhado = (!$eorid ? 'none' : '');
    $displayLiquidado = (!$eorid ? 'none' : '');
} else {
    verificaSessao('obra');

    extract($execOrcamento->carregaPorObrid($obrid, $param));
    $msgTitle = (!$eorid ? 'Salve o orçamento para a obra e então será liberado o cadastro de detalhamento do orçamento' : '');
    $displayEmpenhado = (!$eorid ? 'none' : '');
    $displayLiquidado = (!$eorid ? 'none' : '');
}

if ($_GET['acao'] != 'V') {

    include APPRAIZ . "includes/cabecalho.inc";

    echo "<br>";
    if ($_GET['acao'] == 'A') {
        $db->cria_aba(ID_ABA_EMP_CADASTRADO, $url, $parametros);

        $empreendimento->montaCabecalho();
    } else {
        if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
        } else {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
        }

        echo cabecalhoObra($obrid);
    }

    $habilitado = true;
    $habilita = 'S';
} else {
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $db->cria_aba($abacod_tela, $url, $parametros);
    echo cabecalhoObra($obrid);
    $habilitado = false;
    $habilita = 'N';
}

if (possui_perfil(array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) {
    $habilitado = false;
    $habilita = 'N';
    $exclui = "       <img' +
                    '          align=\"absmiddle\"' +
                    '          src=\"/imagens/excluir.gif\"' +
                    '          style=\"cursor: pointer; margin-left: 3px;\"' +
                    '          onclick=\"javascript: excluirExec( ' + dadosEmEdicao.peoid + '  );\"' +
                    '          title=\"Excluir\">";
}
//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if (possui_perfil(PFLCOD_CALL_CENTER)) {
    $habilitado = false;
    $habilita = 'N';
    $exclui = "       <img' +
                    '          align=\"absmiddle\"' +
                    '          src=\"/imagens/excluir_01.gif\"' +
                    '          style=\"cursor: pointer; margin-left: 3px;\"' +
                    '          title=\"Excluir\">";
}

monta_titulo($titulo_modulo, '');
