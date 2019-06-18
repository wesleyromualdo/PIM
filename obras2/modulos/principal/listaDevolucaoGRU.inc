<?php
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
    $obrid = $obr->pegaIdObraPorEmpid($empid);

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

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$execucaoFinanceira = new ExecucaoFinanceira();

$verificarExecucaoFinanceira = $execucaoFinanceira->verificarExecucaoFinanceira($obrid);

$verificarExecucaoFinanceiraComposicao = $execucaoFinanceira->verificarExecucaoFinanceiraComposicao($obrid);

$processo = $execucaoFinanceira->retornarProcesso($obrid);

$retornaExecucaoFinanceira = $execucaoFinanceira->retornaExecucaoFinanceira($processo);

$execucaoFinanceiraAberto = empty($verificarExecucaoFinanceiraComposicao) || $verificarExecucaoFinanceiraComposicao['excfinalizada'] == 'f' ? true : false;

$quantidadeObras = $execucaoFinanceira->verificarQuantidadeObrasProcesso($obrid, $processo);

$estadoObra = $obra->pegaEstadoObra($obrid);

$docid = $execucaoFinanceira->retornaExecucaoFinanceira($processo);
$docid = $docid['docid'];

$disabled = $verificarExecucaoFinanceira < 1 || $estadoObra != ESDID_OBJ_CONCLUIDO  ||  $estadoObra == ESDID_OBJ_CANCELADO? "disabled='disabled'" : "" ;



if ($_REQUEST['excluir'] == 'S') {

    $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
    $questaoCumprimentoArquivo->excluir($_REQUEST['arqid']);
    echo "<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
    exit;
}

if ($_REQUEST['excluir_exc'] == 'S') {

    $execucaoFinanceira->excluirArquivoDevolucaoGruExecucaoFinanceira($_REQUEST['arqid']);
    echo "<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
    exit;
}

if ($_REQUEST['download'] == 'S') {

    $execucaoFinanceira->downloadDevolucaoGruExecucaoFinanceira($_REQUEST['arqid']);
    echo "<script>window.location.href = 'obras2.php?modulo=principal/clistaDevolucaoGRU&acao=O';</script>";
    exit;
}



if (isset($_POST['salvar']) && !empty($_POST['salvar'])) {

    $extensao = str_replace('.', '', strstr($_FILES['arq']['name'], '.'));
    $maxUploadSize = 10485760;

    /*if (empty($_REQUEST['devvalor']) && empty($_FILES['arq']['name'])) {
        echo "<script>alert('Anexe o documento e preencha o valor!'); window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
        exit;
    }
    if (!empty($_REQUEST['devvalor']) && empty($_FILES['arq']['name'])) {
        echo "<script>alert('Deve inserir o documento!'); window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
    }

    if($extensao != 'pdf' || $_FILES['arq']['size'] > $maxUploadSize){
        echo "<script>alert('Apenas arquivos PDF com tamanho até 10 Megabytes!'); window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
        exit;
    }*/

    $arrDados = array(
        'obrid' => $obrid,
        'file' => $_FILES,
        'req' => $_REQUEST,
        'exeprocesso' => $processo,
        'usucpf' => $_SESSION['usucpf'],
        'exeid' => $retornaExecucaoFinanceira['exeid'],
        'excid' => $retornaExecucaoFinanceira['excid']
    );

    if ($retornaExecucaoFinanceira == null) {

        $execucaoFinanceira->salvarExecucaoFinanceira($arrDados);
        echo "<script>alert('Arquivo incluído com sucesso!'); window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
        exit;

    } else {

        if (!$verificarExecucaoFinanceiraComposicao) {

            $execucaoFinanceira->salvarExecucaoFinanceiraComposicao($arrDados);
            echo "<script>alert('Arquivo incluído com sucesso!'); window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
            exit;

        } else {

            $execucaoFinanceira->salvarDevolucaoGruExecucaoFinanceira($arrDados);
            echo "<script>alert('Arquivo incluído com sucesso!'); window.location.href = 'obras2.php?modulo=principal/listaDevolucaoGRU&acao=O';</script>";
            exit;
        }

    }

}

//if ($_GET['acao'] != 'V') {

    include APPRAIZ . "includes/cabecalho.inc";

    echo "<br>";

    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);

    echo cabecalhoObra($obrid);

    $habilitado = true;
    $habilita = 'S';
//} else {
//    ?>
<!--    <script language="JavaScript" src="../includes/funcoes.js"></script>-->
<!--    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>-->
<!--    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>-->
<!--    --><?php
//    $db->cria_aba($abacod_tela, $url, $parametros);
//    echo cabecalhoObra($obrid);
//    $habilitado = false;
//    $habilita = 'N';
//}


monta_titulo($titulo_modulo, '');
?>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript"
            src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <script type="text/javascript">
        function excluirArquivo(arqid) {
            if (confirm('Você deseja realmente excluir este arquivo?')) {
                window.location = '?modulo=principal/listaDevolucaoGRU&acao=O&excluir=S&arqid=' + arqid;
            }
        }

        function excluirArquivoExecucaoFinanceira(arqid) {
            if (confirm('Você deseja realmente excluir este arquivo?')) {
                window.location = '?modulo=principal/listaDevolucaoGRU&acao=O&excluir_exc=S&arqid=' + arqid;
            }
        }

        function salvar() {
            var devvalor = document.getElementById('devvalor').value;
            var arq = document.getElementById('arq').value;
            if(arq != ''){
                var tamanhoArq = parseInt(document.getElementById("arq").files[0].size);
                var extensaoArq = document.getElementById("arq").files[0].type;
            }

            if (devvalor == '' && arq == '') {
                alert('Anexe o documento e preencha o valor!');
                return false;
            }

            if (devvalor != '' && arq == '') {
                alert('Deve inserir o documento!');
                return false;
            }

            if(extensaoArq != 'application/pdf' || tamanhoArq > 10485760){
                alert('Apenas arquivos PDF com tamanho até 10 Megabytes!');
                return false;
            }

            document.formulario_devolucao.submit();

        }


    </script>
<?php echo $execucaoFinanceira->criaSubAba($url, $habilitado, $obrid); ?>
    <style type="text/css">

        table {
            border-collapse: collapse;
            border-spacing: 0;
        }

        th {
            text-align: left;
        }

        .table {
            width: 100%;
            margin-bottom: 20px;
        }

        .table thead > tr > th, .table tbody > tr > th, .table tfoot > tr > th, .table thead > tr > td, .table tbody > tr > td, .table tfoot > tr > td {
            padding: 8px;
            line-height: 1.428571429;
            vertical-align: top;
            border-top: 1px solid #dddddd;
        }

        .table thead > tr > th {
            vertical-align: bottom;
            border-bottom: 2px solid #dddddd;
        }

        .table caption + thead tr:first-child th, .table colgroup + thead tr:first-child th, .table thead:first-child tr:first-child th, .table caption + thead tr:first-child td, .table colgroup + thead tr:first-child td, .table thead:first-child tr:first-child td {
            border-top: 0;
        }

        .table tbody + tbody {
            border-top: 2px solid #dddddd;
        }

        .table .table {
            background-color: #ffffff;
        }

        .table-condensed thead > tr > th, .table-condensed tbody > tr > th, .table-condensed tfoot > tr > th, .table-condensed thead > tr > td, .table-condensed tbody > tr > td, .table-condensed tfoot > tr > td {
            padding: 5px;
        }

        .table-bordered {
            border: 1px solid #dddddd;
        }

        .table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
            border: 1px solid #dddddd;
        }

        .table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
            border-bottom-width: 2px;
        }

        .table-striped > tbody > tr:nth-child(odd) > td, .table-striped > tbody > tr:nth-child(odd) > th {
            background-color: #f9f9f9;
        }

        .table-hover > tbody > tr:hover > td, .table-hover > tbody > tr:hover > th {
            background-color: #f5f5f5;
        }

        table col[class*="col-"] {
            float: none;
            display: table-column;
        }

        table td[class*="col-"], table th[class*="col-"] {
            float: none;
            display: table-cell;
        }

        .table > thead > tr > td.active, .table > tbody > tr > td.active, .table > tfoot > tr > td.active, .table > thead > tr > th.active, .table > tbody > tr > th.active, .table > tfoot > tr > th.active, .table > thead > tr.active > td, .table > tbody > tr.active > td, .table > tfoot > tr.active > td, .table > thead > tr.active > th, .table > tbody > tr.active > th, .table > tfoot > tr.active > th {
            background-color: #f5f5f5;
        }

        .table > thead > tr > td.success, .table > tbody > tr > td.success, .table > tfoot > tr > td.success, .table > thead > tr > th.success, .table > tbody > tr > th.success, .table > tfoot > tr > th.success, .table > thead > tr.success > td, .table > tbody > tr.success > td, .table > tfoot > tr.success > td, .table > thead > tr.success > th, .table > tbody > tr.success > th, .table > tfoot > tr.success > th {
            background-color: #468847;
            border-color: #46773d;
        }

        .table-hover > tbody > tr > td.success:hover, .table-hover > tbody > tr > th.success:hover, .table-hover > tbody > tr.success:hover > td {
            background-color: #3d773e;
            border-color: #3c6635;
        }

        .table > thead > tr > td.danger, .table > tbody > tr > td.danger, .table > tfoot > tr > td.danger, .table > thead > tr > th.danger, .table > tbody > tr > th.danger, .table > tfoot > tr > th.danger, .table > thead > tr.danger > td, .table > tbody > tr.danger > td, .table > tfoot > tr.danger > td, .table > thead > tr.danger > th, .table > tbody > tr.danger > th, .table > tfoot > tr.danger > th {
            background-color: #b94a48;
            border-color: #af4353;
        }

        .table-hover > tbody > tr > td.danger:hover, .table-hover > tbody > tr > th.danger:hover, .table-hover > tbody > tr.danger:hover > td {
            background-color: #a74240;
            border-color: #9c3c4a;
        }

        .table > thead > tr > td.warning, .table > tbody > tr > td.warning, .table > tfoot > tr > td.warning, .table > thead > tr > th.warning, .table > tbody > tr > th.warning, .table > tfoot > tr > th.warning, .table > thead > tr.warning > td, .table > tbody > tr.warning > td, .table > tfoot > tr.warning > td, .table > thead > tr.warning > th, .table > tbody > tr.warning > th, .table > tfoot > tr.warning > th {
            background-color: #c09853;
            border-color: #bc7e48;
        }

        .table-hover > tbody > tr > td.warning:hover, .table-hover > tbody > tr > th.warning:hover, .table-hover > tbody > tr.warning:hover > td {
            background-color: #b78c43;
            border-color: #ab713f;
        }

        .table > thead > tr > td {
            text-align: center;
            font-weight: bold;
            vertical-align: middle;
            font-size: 12px;
        }

        .table > tbody > tr > td {
            vertical-align: middle;
            text-align: center;
            font-size: 12px;
        }

        .abaContainer {
            width: 98%;
            text-align: center;
            margin: 0 auto;
        }

        .abaContent {
            background-color: #f5f5f5;
            padding: 15px;
            border-left: 1px solid #dcdcdc;
            border-right: 1px solid #dcdcdc;
            border-bottom: 1px solid #dcdcdc;
            border-bottom-left-radius: 5px;
            border-bottom-right-radius: 5px;
            font-size: 13px;
        }

        .construtoraContent {
            text-align: left;
        }


    </style>
<?php

$cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
$arquivos = $cumprimentoObjetoDocumentacao->recuperarArquivosGru($obrid);

$valorGruDevolvido = new QuestionarioCumprimentoObjeto();
$valorGruDevolvido = $valorGruDevolvido->recuperarGruDevolvida($obrid);


?>

    <div class="abaContent">
        <div class="construtoraContent">
            <table class="table table-bordered">
                <thead>
                <?php

                if (!empty($arquivos)) { ?>
                <tr>
                    <td colspan="2">Lista de GRU (Cumprimento do Objeto)</td>
                </tr>
                <tr>
                    <td>Ações</td>
                    <td>Arquivos</td>
                </tr>
                </thead>
                <tbody>

                <?php

                foreach ($arquivos as $arquivo) {

                    ?>
                    <tr>
                        <td width="25%">
                            <div align="center">
                                <a class="link" onclick="excluirArquivo('<?= $arquivo['arqid'] ?>');">
                                    <img src="/imagens/excluir.gif" border='0'
                                         title="Excluir arquivo <?= $arquivo['arqnome'] ?>">
                                </a>

                                <a class="link"
                                   onclick="window.location='?modulo=principal/cadCumprimentoObjeto&acao=A&download=S&arqid=<?= $arquivo['arqid']; ?>'">
                                    <img src='/imagens/salvar.png' border='0'
                                         title="Download arquivo <?= $arquivo['arqnome'] ?>">
                                </a>
                            </div>
                        </td>
                        <td>
                            <div align="left"><?= $arquivo['arqnome'] . '.' . $arquivo['arqextensao'] ?></div>
                        </td>

                    </tr>
                    <?php

                }


                ?>

                <tr>
                    <td>
                        <div><b>Valor dos recursos devolvidos ao FNDE</b></div>
                    </td>
                    <td>
                        <div align="left"><b>R$ <?= number_format($valorGruDevolvido['devvalor'], 2, ',', '.'); ?></b>
                        </div>
                    </td>

                </tr>
        </div>

        </tbody>
        <?php } else { ?>
            <tr>
                <td width="25%">
                    <div><b>Valor dos recursos devolvidos ao FNDE</b></div>
                </td>
                <td>
                    <div align="left"><b>R$ <?= number_format($valorGruDevolvido['devvalor'], 2, ',', '.'); ?></b></div>
                </td>
            </tr>
        <?php } ?>


        <!--        <input type="hidden" name="req" value="salvar">-->
        <div class="construtoraContent">
            <table class="table table-bordered">
                <thead>
                <tr>
                    <td colspan="3">Lista de GRU Devolução</td>
                </tr>
                <?php
                if ($execucaoFinanceiraAberto) { ?>
                    <form method="post" id="formulario_devolucao" name="formulario_devolucao"
                          enctype="multipart/form-data">
                        <input type="hidden" name="salvar" value="salvar">
                        <tr>
                            <td width="25%"><input type="file" name="arq" id="arq"></td>
                            <td colspan="2">
                                <div align="left">
                                    R$ <?php echo campo_texto('devvalor', 'N', 'false', '', 10, 15, '###.###.###,##', '', 'left', '', 0, 'id="devvalor"', '', ''); ?></div>
                            </td>
                        </tr>
                    </form>
                <?php } ?>
                <tr>
                    <td>Ações</td>
                    <td>Arquivos</td>
                    <td>Valor</td>
                </tr>
                </thead>
                <tbody>
                <?php
                $devolucoes = $execucaoFinanceira->retornarDevolucaoGruExecucaoFinanceira($obrid);
                if (!empty($devolucoes)) {
                    foreach ($devolucoes as $devolucao) {
                        ?>
                        <tr>
                            <td>
                                <div align="center">
                                    <?php if ($execucaoFinanceiraAberto) { ?>
                                        <a class="link"
                                           onclick="excluirArquivoExecucaoFinanceira('<?= $devolucao['arqid'] ?>');">
                                            <img src="/imagens/excluir.gif" border='0'
                                                 title="Excluir arquivo <?= $devolucao['arqnome'] ?>">
                                        </a>
                                    <?php } ?>
                                    <a class="link"
                                       onclick="window.location='?modulo=principal/listaDevolucaoGRU&acao=O&download=S&arqid=<?= $devolucao['arqid']; ?>'">
                                        <img src='/imagens/salvar.png' border='0'
                                             title="Download arquivo <?= $devolucao['arqnome'] ?>">
                                    </a>
                                </div>
                            </td>
                            <td>
                                <div align="left"><?= $devolucao['arqnome'] . '.' . $devolucao['arqextensao'] ?></div>
                            </td>

                            <td width="40%">
                                <div align="left">
                                    R$ <?= number_format($devolucao['devvalor'], 2, ',', '.'); ?>
                            </td>

                        </tr>
                    <?php }
                } ?>
                <tr>
                    <td colspan="2">
                        <div align="right"><b>Total</b></div>
                    </td>
                    <td>
                        <div align="left">
                            <?php $valorGruDevolvidoExc = $execucaoFinanceira->retornarValorDevolucaoGruExecucaoFinanceira($obrid); ?>
                            R$ <?= number_format($valorGruDevolvidoExc, 2, ',', '.'); ?>
                    </td>

                </tr>
                <tr>
                    <?php if ($execucaoFinanceiraAberto) {?>
                        <td colspan="3">
                            <button onclick="salvar()">Salvar</button>
                        </td>
                    <?php } ?>
                </tr>
                <tr>

                    <td colspan="3"><b>

                            Total devolvido:
                            R$ <?= number_format($valorGruDevolvido['devvalor'] + $valorGruDevolvidoExc, 2, ',', '.'); ?></b>

                    </td>

                </tr>

        </div>

        </tbody>

    </div>
<?php

?>