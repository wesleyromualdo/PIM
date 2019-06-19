<?php

require APPRAIZ . "www/includes/webservice/pj.php";

require APPRAIZ . "obras2/includes/principal/popUpAcompanhamentoNotaFiscal/funcoes.php";


$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

$medid = null;
$ntfid = null;
$acaoEditar = false;

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

if (empty($obrid) && $empid) {
    $obras = new Obras();
    $arrObrid = $obras->pegaIdObraPorEmpid($empid);
    $obrid = (is_array($arrObrid) && !empty($arrObrid)) ? current($arrObrid) : $obrid;

    if (empty($obrid)) {
        die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
    }
}

/* Se não houver 'obrid', o usuário não tem permissão para seguir o processo. */
if (!isset($obrid)) {
    echo "<script>alert('Faltam parâmetros para acessar esta tela.');</script>";
    exit;
}


/*
 * Requisição para verificar se a ação da tela é a de edição.
 */
if ((isset($_GET["ntfid"]) && ctype_digit($_GET["ntfid"])) && (isset($_GET["tipo"]) && $_GET["tipo"] == "E")) {
    $acaoEditar = true;
    $ntfid = $_GET["ntfid"];

    $dadosNotaFiscal = getDadosNotaFiscal($ntfid);
    $arrDadosNotaFiscal = !empty($dadosNotaFiscal) ? current($dadosNotaFiscal) : $dadosNotaFiscal;

    $arqid = $arrDadosNotaFiscal["arqid"];

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $arquivoNotaFiscal = $execucaoFinanceira->recuperaDadosArquivo($arqid);

    if (!empty($arrDadosNotaFiscal)) {
        if (!in_array($arrDadosNotaFiscal["obrid"], $arrObrids)) {
            echo "<script>
                    alert('Atenção, você está editando informações de uma obra não permitida.');                  
                  window.close();
                  </script>
            ";
            exit;
        }
    }
}

/* Requisição para solicitar o carregamento da combo de contratos.*/
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "carregarContratos") {
    ob_clean();
    $idFornecedorInfo = trim($_POST["idFornecedor"]);
    $arrIdFornecedorInfo = explode("_", $idFornecedorInfo);
    $origemEmpresa = $arrIdFornecedorInfo[0];

    $crtid = false;
    $cceid = false;

    if ($origemEmpresa === "ent") {
        $crtid = $arrIdFornecedorInfo[2];
    } elseif ($origemEmpresa === "cex") {
        $cceid = $arrIdFornecedorInfo[2];
    }

    echo json_encode(carregarContratos($crtid, $cceid));
    exit;
}

/* Requisição para solicitar o carregamento da combo de medições.*/
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "carregarMedicao") {

    ob_clean();

    $fornecedorInfo = trim($_POST["fornecedor"]);
    $arrFornecedorInfo = explode("_", $fornecedorInfo);
    $tipoFornecedor = $arrFornecedorInfo[0];
    $idContratoPrincipal = $arrFornecedorInfo[2];

    $crtidPrincipal = $tipoFornecedor === "ent" ? $idContratoPrincipal : null;
    $cceidPrincipal = $tipoFornecedor === "cex" ? $idContratoPrincipal : null;

    $contratoInfo = trim($_POST["contrato"]);
    $arrContratoInfo = explode("_", $contratoInfo);
    $tipoContrato = $arrContratoInfo[0];
    $idContrato = $arrContratoInfo[1];

    $crtid = $tipoContrato == "crt" ? $idContrato : null;
    $cceid = $tipoContrato == "cex" ? $idContrato : null;

    $arrMedicoes = carregarMedicao($crtidPrincipal, $cceidPrincipal, $crtid, $cceid, $acaoEditar);

    echo json_encode($arrMedicoes);
    exit;
}

/* Requisição para solicitar a gravação das medições. */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "salvarNotaMedicao") {

    ob_clean();

    if ($_POST["ntfid"] && ctype_digit($_POST["ntfid"]) && $acaoEditar) {
        atualizarNota($obrid, $_POST);
    } else {
        salvarNota($obrid, $_POST);
    }
}

/* Requisição para solicitar o download do arquivo da medição. */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "download") {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo "<script>window.location.href = '" . $_SERVER['HTTP_REFERER'] . "';</script>";
    exit;
}

/* Requisição para validar o CNPJ. */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == 'validarCNPJ') {
    ob_clean();
    $cnpjFornecedor = trim($_POST["cnpjFornecedor"]);
    echo json_encode(validarCnpj($cnpjFornecedor));
    exit;
}

/* Requisição para solicitar o valor de determinada medição.*/
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "getDadosMedicao") {
    ob_clean();
    $mecid = (int)trim($_POST["mecid"]);
    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrDadosMedicao = $execucaoFinanceira->getDadosMedicao($mecid);

    echo json_encode($arrDadosMedicao);
    exit;
}

/* Requisição para solicitar o cálculo do percentual das medições.*/
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "calcularPercentuaisMedicao") {

    ob_clean();

    $valorPagoNota = $_POST["valorPagoNota"];
    $totalPagoNota = $_POST["totalPagoNota"];
    $tipoEmpresaContratada = $_POST["tipoEmpresaContratada"];
    $idEmpresaContratada = $_POST["idEmpresaContratada"];
    $idContrato = $_POST["idContrato"];
    $dadosContratoAditivo = $_POST["dadosContratoAditivo"];
    $dataNotaFiscal = formata_data_sql($_POST["dataNota"]);
    $valorMedicao = $_POST["valorMedicao"];

    $crtid = $tipoEmpresaContratada === "ent" ? $idContrato : null;
    $cceid = $tipoEmpresaContratada === "cex" ? $idContrato : null;

    $arrRetorno = array("fisica"=>null, "acumulada"=>null);

    if (!$crtid && !$cceid) {
        $arrRetorno["fisica"] = ("Não foi possível obter os percentuais da medição.");
        $arrRetorno["acumulada"] = ("Não foi possível obter os percentuais da medição.");

        echo json_encode($arrRetorno);
        exit;
    }

    if ($cceid) {
        $arrRetorno["fisica"] = ("Não se aplica.");
        $arrRetorno["acumulada"] = ("Não se aplica.");
        echo json_encode($arrRetorno);
        exit;
    }

    $execucaoFinanceira = new ExecucaoFinanceira();

    $arrDadosContratoAditivo = explode("_", $dadosContratoAditivo);
    $origemContrato = $arrDadosContratoAditivo[0];
    $idContratoAditivo = $arrDadosContratoAditivo[1];

    $tipoContratoAditivo = $execucaoFinanceira->getTipoContrato($origemContrato, $idContratoAditivo);

    if ($tipoContratoAditivo === "aditivo") {

        $arrRetorno["fisica"] = ("Não há cálculo do Percentual de Medição Física para os aditivos.");
        $arrRetorno["acumulada"] = ("Não há cálculo do Percentual de Medição Acumulada para os aditivos.");

    } else {

        // Obtendo o valor do contrato atual.
        $valorContratoAtual = (float)$execucaoFinanceira->getValorContratoAtual($crtid, $cceid);

        // Calculando percentual da medição física.
        $percentualMedicaoFisica = $execucaoFinanceira->calcularPercentualMedicao($valorPagoNota, $valorContratoAtual);
        $percentualMedicaoFisica = number_format($percentualMedicaoFisica, 2, ",", ".");

        // Calculando o percentual da medição acumulada.
        $arrObras = $execucaoFinanceira->retornaObrids($obrid);

        $valorContratoPrincipal = (float)$execucaoFinanceira->getValorContratoPrincipal($obrid);

        /*
         * % Medição Física Acumulada: Cálculo: (Y*100) / X, onde:
         * X = Soma de todos os contratos (RN04, RN05, RN06)
         * Y = Soma dos valores pagos das notas fiscais até a data da nota fiscal (RN04, RN05), somado aos pagamentos
         * realizados na própria nota, somado ao campo "Valor Pago na Nota Fiscal".
         *
         * [RN04] - As medições podem ser de um contrato ou um aditivo ou para os dois (parte da medição referente ao
         * contrato e parte a um ou mais aditivo).
         * [RN05] - Para calcular a "% Medição Física" e "% Medição Física Acumulada" não serão considerados os valores
         * das medições referentes a aditivos, nem os valores de contratos das empresas cadastradas manualmente nas
         * abas Medições ou Cumprimento do Objeto.
         * [RN06] - Para calcular a "% Medição Física Acumulada" os contratos serão totalizados da seguinte forma:
         * Quando a obra foi realizada por apenas uma empresa, será utilizado o valor do contrato atual;
         * Quando a obra foi realizada por mais de uma empresa, será utilizado o valor do contrato atual somado as
         * notas fiscais das medições dos contratos anteriores.
         */

        // Verificando se a obra foi realizada com mais de um contrato.
        if (count($arrObras) > 1) {
            $somatorioValoresPagosNotaOutrosContratos = (float)$execucaoFinanceira->getSomatorioValoresPagosNota($obrid, true);
            $x = $valorContratoPrincipal + $somatorioValoresPagosNotaOutrosContratos;
        } else {
            $x = $valorContratoPrincipal;
        }

        $y = ((float)$execucaoFinanceira->getSomatorioValoresPagosNota($obrid)) + (float)$totalPagoNota;


        $percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($y, $x);
        $percentualMedicaoAcumulada = number_format($percentualMedicaoAcumulada, 2, ",", ".");

        $arrRetorno["fisica"] = $percentualMedicaoFisica . "%";
        $arrRetorno["acumulada"] = $percentualMedicaoAcumulada . "%";
    }

    echo json_encode($arrRetorno);
    exit;
}


