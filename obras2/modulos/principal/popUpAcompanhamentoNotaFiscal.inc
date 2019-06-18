<?php
/**
 * @todo ALTERAR LINKS DAS HISTÓRIAS.
 */
header('content-type: text/html; charset=utf-8');

/* Arquivo responsável por utilizar o webservice de consulta de Pessoas Jurídicas à base da Receita Federal. */
require APPRAIZ . "www/includes/webservice/pj.php";

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

        //$somatorioTodosContratos = getSomatorioTodosContratos($tipoEmpresaContratada, $arrObras);

        //$somatorioNotasPagasOutrosContratos = getSomatoriosNotasPagasOutrosContratos($tipoEmpresaContratada, $arrObras, $dataNotaFiscal);
        //$somatorioNotasPagasOutrosContratos += $totalPagoNota;

        //$percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($somatorioNotasPagasOutrosContratos, $somatorioTodosContratos);

        $percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($y, $x);
        $percentualMedicaoAcumulada = number_format($percentualMedicaoAcumulada, 2, ",", ".");

        $arrRetorno["fisica"] = $percentualMedicaoFisica . "%";
        $arrRetorno["acumulada"] = $percentualMedicaoAcumulada . "%";
    }

    echo json_encode($arrRetorno);
    exit;
}

/**
 * @todo Comentar depois.
 * @param $tipoFornecedor
 * @param $arrObras
 * @return bool|mixed|NULL|string
 */
function getSomatoriosNotasPagasOutrosContratos($tipoFornecedor, $arrObras, $dataNotaFiscal) {

    if (is_array($arrObras) && !empty($arrObras)) {

        global $db;

        $strObrids = implode(",", $arrObras);

        if ($tipoFornecedor === "ent") {
            $sql =  "
                SELECT coalesce(SUM(ntm.ntmvlrpago), 0) totalcontratosanteriores
                FROM
                  obras2.notafiscal ntf
                  INNER JOIN
                  obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                  INNER JOIN
                  obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                WHERE
                  ntf.ntfstatus = 'A'
                  --AND ntf.crtid IN 
                  AND mec.crtid IN 
                  (
                    SELECT crt.crtid AS crtid
                    FROM obras2.contrato crt
                      INNER JOIN obras2.obrascontrato ocr ON (crt.crtid = ocr.crtid) OR (ocr.crtid = crt.crtidpai)
                    WHERE ocr.ocrstatus = 'A'
                          AND crt.crtstatus = 'A'
                          AND crt.ttaid IS NULL
                          AND ocr.obrid IN ({$strObrids})
                  )
                  AND ntf.ntfdtnota <= '{$dataNotaFiscal}'; 
            ";

        } elseif ($tipoFornecedor === "cex") {
            $sql = "
                SELECT coalesce(SUM(ntm.ntmvlrpago), 0) totalcontratosanteriores
                FROM
                  obras2.notafiscal ntf
                  INNER JOIN
                  obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                  INNER JOIN
                  obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                WHERE
                  ntf.ntfstatus = 'A'
                  AND ntf.cceid IN (
                    SELECT cce.cceid
                    FROM obras2.contratoconstrutoraextra cce
                      INNER JOIN obras2.construtoraextra cex ON cce.cexid = cex.cexid AND cex.cexstatus = 'A'
                    WHERE cce.ccestatus = 'A'
                      AND cce.cceid_pai ISNULL
                      AND cce.ttaid ISNULL
                      AND cex.obrid IN ({$strObrids})
                  )
                  AND ntf.ntfdtnota <= '{$dataNotaFiscal}';
            ";
        }


        return $db->pegaUm($sql);
    }
}

/**
 * @todo Comentar depois.
 * @param $tipoFornecedor
 * @param $arrObras
 * @return bool|mixed|NULL|string
 */
function getSomatorioTodosContratos($tipoFornecedor, $arrObras) {

    if (is_array($arrObras) && !empty($arrObras)) {

        global $db;

        $strObrids = implode(",", $arrObras);

        if ($tipoFornecedor === "ent") {
            $sql =  "
                SELECT coalesce(sum(ocr.ocrvalorexecucao), 0) totalvalorcontratos
                FROM
                  obras2.obrascontrato ocr
                  INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                WHERE
                  crt.crtstatus = 'A'
                  AND crt.ttaid ISNULL
                  AND ocr.ocrstatus = 'A'
                  AND ocr.obrid IN ({$strObrids})
            ";
;
            return $db->pegaUm($sql);

        } elseif ($tipoFornecedor === "cex") {
            $sql = "
                SELECT
                  coalesce(sum(cce.ccevalor), 0) totalvalorcontratos
                FROM obras2.contratoconstrutoraextra cce
                  INNER JOIN obras2.construtoraextra cex ON cce.cexid = cex.cexid AND cex.cexstatus = 'A'
                WHERE cce.ccestatus = 'A'
                      AND cce.cceid_pai ISNULL
                      AND cce.ttaid ISNULL
                      AND cex.obrid IN ({$strObrids})
            ";

            return $db->pegaUm($sql);
        }
    }
}

/**
 * Função responsável por validar os dados inseridos no formulário de cadastro de notas.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $arrDados
 * @return array
 */
function validarDados($arrDados, $isAtualizacao=false) {

    $arrRetorno = array("error" => false, "message" => null);

    if (is_array($arrDados) && !empty($arrDados)) {

        // Certas validações não são necessárias na tela de edição de nota fiscal.
        $ignorarValidacao = false;
        if ($isAtualizacao) {
            if (!is_array($arrDados["dadosNotaFiscalMedicao"])) {
                $ignorarValidacao = true;
            }
        }

        // Verificando se a empresa contratada foi informada.
        if (empty(trim($arrDados["empresacontratada"]))) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Fornecedor.";
        }

        // Verificando se o contrato foi informado.
        /*if ($ignorarValidacao) {
            if (empty(trim($arrDados["contratos"]))) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "Informe o Contrato ou Aditivo de Valor.";
            }
        }*/

        // Verificando se o número da nota fiscal foi informado.
        if (empty(trim($arrDados["numeroNota"]))) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Número da Nota Fiscal.";
        }

        // Verificando se a data da nota fiscal é uma data válida.
        if (empty(trim($arrDados["dataNota"]))) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe a Data da Nota Fiscal.";
        } else {
            $arrDataNotaFiscal = explode("/", $arrDados["dataNota"]);
            if (!checkdate($arrDataNotaFiscal[1], $arrDataNotaFiscal[0], $arrDataNotaFiscal[2])) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "A Data da Nota Fiscal não tem formato válido.";
            }
        }

        // Verificando se o valor da nota fiscal foi informado.
        if (empty(trim($arrDados["valorNota"]))) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Valor da Nota Fiscal.";
        } else {
            if (!is_numeric($arrDados["valorNota"])) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "O Valor da Nota Fiscal deve ser um número válido.";
            }
        }

        // Verificando os dados que ligam medição e contrato/aditivo.
        if (!$ignorarValidacao) {
            if (!is_array($arrDados["dadosNotaFiscalMedicao"]) || (is_array($arrDados["dadosNotaFiscalMedicao"]) && empty($arrDados["dadosNotaFiscalMedicao"]))) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "Problemas ao iniciar a gravação. Faltam dados para vincular a nota fiscal às medições.";
            }
        }

        // Validando os dados para cada aditivo (Regra 16, TL02).
        $arrValoresPagosNota = array();
        if (is_array($arrDados["dadosNotaFiscalMedicao"]) && !empty($arrDados["dadosNotaFiscalMedicao"])) {
            foreach ($arrDados["dadosNotaFiscalMedicao"] as $notaMed) {
                $vlrPagoNota = $notaMed->valorPagoNota;
                $arrValoresPagosNota[] = (float)$vlrPagoNota;
            }
        } else {
            $ntfid = $arrDados["ntfid"];
            $execucaoFinanceira = new ExecucaoFinanceira();
            $arrValoresPagosNota = $execucaoFinanceira->getValoresPagosNotaMedicao($ntfid);
        }

        //$valorMedicao = $arrDados["vlrMedicao"];
        $valorNotaFiscal = $arrDados["valorNota"];
        $somatorioVlrPagosNota = array_sum($arrValoresPagosNota);

        // Verificando se o somatório dos valores pagos na nota fiscal é igual ao valor da nota fiscal.
        //if (!$ignorarValidacao) {
            if (round($somatorioVlrPagosNota, 2) != round($valorNotaFiscal, 2)) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "O somatório dos valores pagos na nota fiscal deve ser igual ao Valor da Nota Fiscal.";
            }
        //}

    } else {
        $arrRetorno["error"] = true;
        $arrRetorno["message"] = "A nota fiscal não foi gravada, pois houve problemas com os dados informados.";
    }

    return $arrRetorno;
}

/**
 * Função responsável por carregar o elemento select com as empresas contratadas.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @return string
 */
function montaComboEmpresaContratada($obrid, $selectedValue=null) {

    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    /*
     * Na consulta é realizada uma concatenação dos IDs com os prefixos 'ent_' ou 'cex_'. Isso serve para identificar se
     * a empresa foi cadastrada como entidade ou como construtora extra (Cumprimento do Objeto).
     */
    $sql = "
        SELECT
          'ent_' || ent.entid || '_' || c.crtid AS codigo,
          '(' ||
          (SELECT substr(cnpj, 1, 2) || '.' || substr(cnpj, 3, 3) || '.' || substr(cnpj, 6, 3) || '/' || substr(cnpj, 9, 4) ||
                  '-' || substr(cnpj, 13) AS cnpj
           FROM (SELECT cast(ent.entnumcpfcnpj AS VARCHAR) AS cnpj) a) || ') ' ||
          CASE WHEN ent.entnome NOTNULL THEN
            ent.entnome
          ELSE
            '-'
          END
          || ' (Contrato ' ||
          CASE WHEN c.crtnumero NOTNULL THEN
            '- nº ' || c.crtnumero || ' - '
          ELSE
            '- '
          END ||
          to_char(c.crtdtassinatura, 'DD/MM/YYYY') || ')' AS descricao
        FROM obras2.obrascontrato ocr INNER JOIN obras2.contrato c ON (ocr.crtid = c.crtid) OR (ocr.crtid = c.crtidpai)
          INNER JOIN entidade.entidade ent ON ent.entid = c.entidempresa
        WHERE ocr.ocrstatus = 'A' AND c.crtstatus = 'A' AND ent.entstatus = 'A' AND c.ttaid IS NULL AND ocr.obrid IN ({$strObrids})
        
        UNION ALL
        
        SELECT
          'cex_' || cex.cexid || '_' || cce.cceid AS codigo,
          '(' ||
          (
            SELECT substr(cnpj, 1, 2) || '.' || substr(cnpj, 3, 3) || '.' ||
                   substr(cnpj, 6, 3) || '/' || substr(cnpj, 9, 4) || '-' ||
                   substr(cnpj, 13) AS cnpj
            FROM (SELECT cast(cex.cexnumcnpj AS VARCHAR) AS cnpj) a
          ) || ') ' || cex.cexrazsocialconstrutora || ' (Contrato - nº ' || cce.ccenumero || ' - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY') ||
          ')'                 AS descricao
        FROM obras2.construtoraextra cex
          INNER JOIN obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
        WHERE cex.cexstatus = 'A'
              AND cce.ccedataassinatura IS NOT NULL
              AND cce.ttaid IS NULL
              AND
              cex.obrid IN ({$strObrids});
    ";
 

    if ($selectedValue) {
//        $arr = array("name" => "empresacontratada", "value" => $selectedValue);
        return $db->monta_combo("empresaContratada", $sql, "S", "", "montaComboContratos", '', '', '', 'S', 'empresaContratada', '', $selectedValue);
    }

    return $db->monta_combo("empresaContratada", $sql, "S", "Selecione...", "montaComboContratos", '', '', '', 'S', 'empresaContratada');

}

/**
 * Função responsável por carregar o elemento select com os contratos/aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $crtid
 * @param $cceid
 * @param null $medid
 * @return string
 */
function montaComboContratosAditivos($obrid, $crtid, $cceid, $medid=null) {

    global $db;

    $arrContratosAditivos = carregarContratos($obrid, $crtid, $cceid, $medid);

    return $db->monta_combo("contratos", $arrContratosAditivos, "S", "Selecione...", "", '', '', '', 'S', 'contratos');
}

/**
 * @todo Verificar parâmetros.
 * Função responsável por carregar uma lista com os contratos/aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $crtid Id do contrato original quando a empresa for de origem 'Entidade'.
 * @param $cceid Id do contrato original quando a empresa for de origem 'Construtora extra'.
 * @return array|mixed|NULL
 */
function carregarContratos($crtid, $cceid, $medid=null) {

    global $db;

    $arrContratos = array();

    if (!$crtid && !$cceid) {
        return $arrContratos;
    }

    if (ctype_digit($crtid) && !$cceid) {
        $sql = "
				SELECT
					'crt_' || crt.crtid as codigo,
					CASE WHEN ttaid IS NULL THEN
						'Contrato Original' || ' - ' || to_char(crt.crtdtassinatura, 'DD/MM/YYYY')
					WHEN ttaid = 2 THEN
                        'Aditivo de Valor' || ' - ' || to_char(crt.crtdtassinaturaaditivo, 'DD/MM/YYYY')
					ELSE
						'Aditivo de Valor/Prazo' || ' - ' || to_char(crt.crtdtassinaturaaditivo, 'DD/MM/YYYY')
					END as descricao
				FROM 
					obras2.contrato crt
				WHERE 
					(
                        crt.crtid  = {$crtid}
                        or
                        (crt.crtidpai = (select crtidpai from obras2.contrato where crtid = {$crtid} order by crtid limit 1) AND ttaid in (2,3))
                        or
                        (crt.crtid = (select crtidpai from obras2.contrato where crtid = {$crtid} order by crtid limit 1) AND ttaid in (2,3))
                        or
                        (crt.crtidpai_execfinceira = {$crtid})
                    ) AND crtstatus = 'A' order by descricao desc, crt.crtdtassinatura 
        ";
    }
    elseif (ctype_digit($cceid) && !$crtid) {
        $sql = "
			SELECT
				'cex_' ||cce.cceid AS codigo,
				CASE WHEN cce.ttaid IS NULL THEN
					'Contrato Original' || ' - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
				ELSE
					'Aditivo de Valor' || ' - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
				END as descricao
			FROM 
		  		obras2.contratoconstrutoraextra cce
			WHERE 
				ccestatus = 'A'
			AND
			(
				cce.cceid  = {$cceid}
				or
				(cce.cceid_pai = {$cceid} AND ttaid in (2,3))
			) 
		";
    }

    $arrContratos = $sql ? $db->carregar($sql) : false;
    if (!$arrContratos) {
        $arrContratos = array();
    }

    return $arrContratos;
}

/**
 * Função responsável por carregar uma lista com as medições e suas respectivas referências.
 * Função responsável por carregar o elemento select com os contratos ou aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15250
 * @param $crtid Id do contrato de uma entidade.
 * @param $cceid Id do contrato da construtora extra'.
 * @return array|mixed|NULL
 */
function carregarMedicao($crtidPrincipal, $cceidPrincipal, $crtid, $cceid, $acaoEditar=false) {

    global $db;

    $arrContratos = array();

    if ((!$crtid && !$cceid) || (!$crtidPrincipal && !$cceidPrincipal)) {
        return $arrContratos;
    }

    if (ctype_digit($crtidPrincipal) && !$cceidPrincipal) {
        $filtro1 = "AND med.crtid = {$crtidPrincipal}";
        $ligacao = "INNER JOIN obras2.contrato c ON mec.crtid = c.crtid AND c.crtstatus = 'A'";
    } elseif (ctype_digit($cceidPrincipal) && !$crtidPrincipal) {
        $filtro1 = "AND med.cceid = {$cceidPrincipal}";
        $ligacao = "INNER JOIN obras2.contratoconstrutoraextra cce ON mec.cceid = cce.cceid AND cce.ccestatus = 'A'";
    }

    if (ctype_digit($crtid) && !$cceid) {
        $filtro2 = "AND mec.crtid = {$crtid}";
    } elseif (ctype_digit($cceid) && !$crtid) {
        $filtro2 = "AND mec.cceid = {$cceid}";
    }

    $sql = "
        WITH totalpagomedicao AS (
            SELECT
              med.medid,
              sum(ntm.ntmvlrpago) AS totalpago
            FROM obras2.medicoes med
              INNER JOIN obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
              LEFT JOIN obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
            WHERE med.medstatus = 'A'
                  {$filtro1}
                  {$filtro2}
            GROUP BY med.medid
            ORDER BY med.medid
        )
        SELECT
          codigo,
          descricao
        FROM
          (
            SELECT
              mec.mecid                        AS codigo,
              med.mednummedicao                AS descricao,
              cast(med.medvlrmedicao AS FLOAT) AS valortotalmedicao,
              coalesce(tpm.totalpago, 0)       AS valorpagomedicao
            FROM
              obras2.medicoes med
              INNER JOIN
              obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
              {$ligacao}
              INNER JOIN
              totalpagomedicao tpm ON tpm.medid = med.medid
            WHERE
              med.medstatus = 'A'
              {$filtro1}
              {$filtro2}
          ) x \n";

    if (!$acaoEditar) {
        $sql .= "WHERE x.valorpagomedicao < x.valortotalmedicao;";
    }

    $arrMedicoes = $db->carregar($sql);
    if (!$arrMedicoes) {
        $arrMedicoes = array();
    }

    return $arrMedicoes;
}

/**
 * @todo VERIFICAR USO
 * Função responsável por salvar as medições dos contratos/aditivos.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $arrDados
 */
function salvarNota($obrid, $arrDados) {


    $arrDados["dadosNotaFiscalMedicao"] = json_decode(str_replace("\\", "", $arrDados["dadosNotaFiscalMedicao"]));

    $arrDados["valorNota"] = str_replace(".", "", $arrDados["valorNota"]);
    $arrDados["valorNota"] = str_replace(",", ".", $arrDados["valorNota"]);
    $arrDados["valorNota"] = (float)$arrDados["valorNota"];

    $msgRetorno = null;

    if (!empty($_FILES["arquivoNota"]["name"])) {

        // O Boletim de Medição deve ser um arquivo no formato PDF.
        $arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
        if (in_array($_FILES["arquivoNota"]["type"], $arrMimeType)) {

            $validacao = validarDados($arrDados);

            if ($validacao["error"] === true) {

                $errorMessage = $validacao["message"];
                echo "<script>alert('{$errorMessage}');window.close();</script>";
                exit;

            } else {

                $empresaContratada = $arrDados["empresacontratada"];
                $arrEmpresaContratada = explode("_", $empresaContratada);
                $tipoEmpresaContratada = $arrEmpresaContratada[0];
                $idEmpresaContratada = $arrEmpresaContratada[1];
                $idContrato = $arrEmpresaContratada[2];

                // A empresa contratada deve ser 'ent' (Entidade) ou 'cex' (Construtora Extra).
                if ($tipoEmpresaContratada === "ent" || $tipoEmpresaContratada === "cex") {

                    //$medid = $arrDados["medid"];
                    $numeroNota = $arrDados["numeroNota"];
                    $arrNotasMedicao = $arrDados["dadosNotaFiscalMedicao"];
                    $descricaoArquivo = "nota_" . $numeroNota . "_" . $empresaContratada;
                    $numeroNota = $arrDados["numeroNota"];
                    $dataNota = formata_data_sql($arrDados["dataNota"]);
                    $valorNota = $arrDados["valorNota"];


                    foreach ($arrNotasMedicao as $notaMedicao){

                        $valorTotalMedicaoNota = getValorPagoByMedicao((int)$notaMedicao->idMedicaoContrato);
                        $valorTotalMedicao = getValorContratoMedicao((int)$notaMedicao->idMedicaoContrato);
                        $valorPagoNota = (float)$notaMedicao->valorPagoNota;
                        $valorTotalMedicao = (float)$valorTotalMedicao;
                        $total = $valorTotalMedicaoNota + $valorPagoNota;


                        if(round($total, 2) > round($valorTotalMedicao, 2)){
                            echo "<script>
                            alert('Atenção! \\nA soma dos valores declarados como pagos para a medição neste documento não pode ser maior que o valor total da Mediçao indicada.');
                            window.close();
                          </script>";
                            exit;
                        }
                    }


                    $usucpf = $_SESSION["usucpf"];

                    require_once(APPRAIZ . "includes/classes/fileSimec.class.inc");
                    $fileSimec = new FilesSimec();
                    $fileSimec->setUpload($descricaoArquivo, null, false);
                    $arqid = $fileSimec->getIdArquivo();

                    if ($arqid) {

                        global $db;

                        $colunaIdContrato = $tipoEmpresaContratada === "ent" ? "crtid" : "cceid";
                        $colunaIdEmpresa = $tipoEmpresaContratada === "ent" ? "entid" : "cexid";

                        $sql = "
                          INSERT INTO obras2.notafiscal
                            ({$colunaIdEmpresa},{$colunaIdContrato}, arqid, ntfnumnota, ntfdtnota, ntfvalornota, ntfusucpfinclusao, ntfdtinclusao)
                          VALUES 
                            ({$idEmpresaContratada}, {$idContrato}, {$arqid}, {$numeroNota}, '{$dataNota}', {$valorNota}, '{$usucpf}', 'now()')
                          RETURNING ntfid";

                        $ntfid = $db->pegaUm($sql);

                        if ($ntfid) {

                            $sql2 = "
                              INSERT INTO obras2.notamedicao
                                (ntfid, mecid, ntmvlrpago, ntmusucpfinclusao, ntmdtinclusao)
                              VALUES \n";

                            if (!empty($arrNotasMedicao)) {
                                foreach ($arrNotasMedicao as $notaMedicao) {
                                    $arrIdContrato = explode("_", $notaMedicao->idContrato);
                                    //$idContrato = (int)$arrIdContrato[1];
                                    $valorPagoNota = (float)$notaMedicao->valorPagoNota;
                                    $mecid = (int)$notaMedicao->idMedicaoContrato;

                                    $sql2 .= "({$ntfid}, {$mecid}, {$valorPagoNota}, '{$usucpf}', 'now()'),";
                                }

                                $sql2 = rtrim($sql2, "\n,");

                                $db->executar($sql2);

                                if ($db->commit()) {
                                    echo "<script>
                                        alert('Informações gravadas com sucesso.');
                                        window.opener.location.reload();
                                    </script>";
                                }

                            } else {
                                $db->rollback();
                                echo "<script>
                                    alert('Problemas ao gravar as informações. \\nVerifique os dados fornecidos.');
                                    window.close();
                                </script>";
                            }
                        } else {
                            $db->rollback();
                            echo "<script>
                                alert('A nota fiscal não foi salva. \\nVerifique os dados fornecidos.');
                                window.close();
                            </script>";
                        }

                    } else {
                        echo "<script>
                            alert('O arquivo da nota fiscal não foi salvo. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                    }

                } else {
                    echo "<script>
                            alert('Problemas ao verificar o Fornecedor. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                }
            }

        } else {
            $msgRetorno = "A Nota Fiscal deve ser um arquivo no formato PDF.";
        }
    } else {
        $msgRetorno = "A Nota Fiscal não foi enviada.";
    }
    if(!empty($msgRetorno)){
        echo "<script>alert('{$msgRetorno}');window.close();</script>";
        exit;
    }
}

/**
 * @todo VERIFICAR USO.
 * Função responsável por alterar uma medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $medid
 * @param $arrDados
 */
function atualizarNota($obrid, $arrDados) {

    $arrDados["dadosNotaFiscalMedicao"] = json_decode(str_replace("\\", "", $arrDados["dadosNotaFiscalMedicao"]));
    $arrDados["notasRemovidas"] = json_decode(str_replace("\\", "", $arrDados["notasRemovidas"]));

    $arrDados["valorNota"] = str_replace(".", "", $arrDados["valorNota"]);
    $arrDados["valorNota"] = str_replace(",", ".", $arrDados["valorNota"]);
    $arrDados["valorNota"] = (float)$arrDados["valorNota"];

    $msgRetorno = null;

    $validacao = validarDados($arrDados, true);

    if ($validacao["error"] === true) {
        $errorMessage = $validacao["message"];
        echo "<script>alert('{$errorMessage}');window.close();</script>";
        exit;

    } else {

        $arqid = null;

        $ntfid = $arrDados["ntfid"];
        $empresaContratada = $arrDados["empresacontratada"];
        $arrEmpresaContratada = explode("_", $empresaContratada);

        $tipoEmpresaContratada = $arrEmpresaContratada[0];
        $idEmpresaContratada = $arrEmpresaContratada[1];
        $idContratoEmpresaContratada = $arrEmpresaContratada[2];
        $arrContratos = explode("_", $arrDados["contratos"]);
        $tipoContrato = null;
        if ($tipoEmpresaContratada == "ent") {
            $tipoContrato = "crt";
        } elseif ($tipoEmpresaContratada == "cex") {
            $tipoContrato = "cce";
        }

        $idContrato = $arrContratos[1];
        $numeroNota = $arrDados["numeroNota"];
        $valorNota = $arrDados["valorNota"];
        $dataNota = formata_data_sql($arrDados["dataNota"]);
        $descricaoArquivo = "ntf_" . $numeroNota . "_" . $empresaContratada;
        $usucpf = $_SESSION["usucpf"];
        $arrNotasMedicao = $arrDados["dadosNotaFiscalMedicao"];
        $arrNotasRemovidas = $arrDados["notasRemovidas"];

        if (is_array($arrNotasMedicao) && !empty($arrNotasMedicao)) {
            foreach ($arrNotasMedicao as $notaMedicao) {

                $valorTotalMedicaoNota = getValorPagoByMedicao((int)$notaMedicao->idMedicaoContrato, $ntfid);
                $valorTotalMedicao = getValorContratoMedicao((int)$notaMedicao->idMedicaoContrato);
                $valorPagoNota = (float)$notaMedicao->valorPagoNota;

                if (($valorTotalMedicaoNota + $valorPagoNota) > $valorTotalMedicao) {
                    echo "<script>
                            alert('Atenção! \\nA soma dos valores declarados como pagos para a medição neste documento não pode ser maior que o valor total da Mediçao indicada.');
                            window.close();
                          </script>";
                    exit;
                }
            }
        }

        if ($_POST["arqidOld"] && ctype_digit($_POST["arqidOld"])) {

            if (!empty($_FILES["arquivoNota"]["name"])) {

                // O Boletim de Medição deve ser um arquivo no formato PDF.
                $arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
                if (in_array($_FILES["arquivoNota"]["type"], $arrMimeType)) {

                    require_once(APPRAIZ . "includes/classes/fileSimec.class.inc");
                    $fileSimec = new FilesSimec();
                    $fileSimec->setUpload($descricaoArquivo, null, false);
                    $arqid = $fileSimec->getIdArquivo();

                    if (!$arqid) {
                        echo "<script>
                            alert('O arquivo da Nota Fiscal não foi salvo. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                    }

                } else {
                    echo "<script>
                        alert('A Nota Fiscal deve ser um arquivo no formato PDF.');
                        window.close();
                      </script>";
                }
            } else {
                echo "<script>
                        alert('A Nota Fiscal não foi enviada.');
                        window.close();
                      </script>";
            }
        }

        $colunaEmpresaNota = $tipoEmpresaContratada === "ent" ? "entid" : "cexid";
        $colunaContratoNota = $tipoContrato === "crt" ? "crtid" : "cceid";

        global $db;

        // Atualizando os dados da nota.
        $sql1 = "
        UPDATE obras2.notafiscal
        SET ";

        if ($arqid && ctype_digit($arqid)) {
            $sql1 .= "arqid = {$arqid},";
        }

        if ($colunaEmpresaNota === "entid") {
            $sql1 .= "cexid = NULL,";
        } else {
            $sql1 .= "entid = NULL,";
        }

        if ($colunaContratoNota === "crtid") {
            $sql1 .= "cceid = NULL,";
        } else {
            $sql1 .= "crtid = NULL,";
        }

        $sql1 .= "
          {$colunaEmpresaNota} = {$idEmpresaContratada},
          {$colunaContratoNota} = {$idContratoEmpresaContratada},
          ntfnumnota = {$numeroNota},
          ntfdtnota = '{$dataNota}',
          ntfvalornota = {$valorNota}
        WHERE ntfid = {$ntfid}";

        $db->executar($sql1);

        if (is_array($arrNotasRemovidas) && !empty($arrNotasRemovidas)) {

            // Removendo as notas que o usuário solicitou.
            $strntmid = implode(",", $arrNotasRemovidas);
            $sql2 = "UPDATE obras2.notamedicao SET ntmstatus = 'I', ntmdtinativacao = NOW(), ntmcpfinativacao='{$usucpf}' WHERE ntmid IN ({$strntmid});";
            $db->executar($sql2);

        }

        // Vinculando a nota fiscal as novas medições.
        if (is_array($arrNotasMedicao) && !empty($arrNotasMedicao)) {
            $sql3 = "";
            foreach ($arrNotasMedicao as $notaMedicao) {
                if (!$notaMedicao->ntmid) { // Se for novo registro.
                    $mecid = $notaMedicao->idMedicaoContrato;
                    $vlr = $notaMedicao->valorPagoNota;
                    $arrContratos = explode("_", $notaMedicao->idContrato);
                    $idContrato = $arrContratos[1];
                    $sql3 .= "INSERT INTO obras2.notamedicao (ntfid, mecid, ntmvlrpago, ntmusucpfinclusao, ntmdtinclusao) VALUES ({$ntfid}, {$mecid}, {$vlr}, '{$usucpf}', 'now()');\n";
                }
            }
            $db->executar($sql3);
        }

        if ($db->commit()) {
            echo "<script>
                     alert('Informações gravadas com sucesso.');
                     window.opener.location.reload();
                  </script>";
            exit;
        }

        echo "<script>alert({$msgRetorno})</script>";
        exit;
    }
}

/**
 * @todo Comentar depois.
 * @param $mecid
 * @param $ntfid
 * @return bool|int|mixed|NULL|string
 */
function getValorPagoByMedicao($mecid, $ntfid = '') {

    global $db;

    $totalPago = 0;

    if(($ntfid != '') && (ctype_digit($ntfid))){
      $filtro = 'AND ntm.ntfid NOT IN ('.$ntfid.')';
    }
    else{
        $filtro = '';
    }

    if ((ctype_digit($mecid))) {

        $sql = "SELECT
              coalesce(sum(ntm.ntmvlrpago), 0)
            FROM obras2.notamedicao ntm
            WHERE ntm.ntmstatus = 'A'
            AND ntm.mecid = {$mecid}
            {$filtro};";

        $retorno = $db->pegaUm($sql);

        if($retorno){
            $totalPago = $retorno;
        }

    }

    return $totalPago;
}

function getValorContratoMedicao($mecid){
    global $db;

    $sql = "SELECT mecvlrreferencia FROM obras2.medicaocontrato WHERE mecid = {$mecid}";

    $totalPago = $db->pegaUm($sql);

    $totalPago = $totalPago ? $totalPago : 0;

    return $totalPago;
}

/**
 * @todo VERIFICAR USO.
 * Função responsável por pesquisar os dados da medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $medid
 * @return array|mixed|NULL
 */
function getDadosMedicao($obrid, $medid) {

    global $db;

    $sql = "
        SELECT
          med.obrid,
          med.medid,
          CASE WHEN (med.entid IS NOT NULL) AND (med.cexid IS NULL)
            THEN 'ent'
          WHEN (med.entid IS NULL) AND (med.cexid IS NOT NULL)
            THEN 'cex'
          ELSE NULL END                              AS medtipoempresacontratada,
          CASE WHEN (med.entid IS NOT NULL) AND (med.cexid IS NULL)
            THEN med.entid
          WHEN (med.entid IS NULL) AND (med.cexid IS NOT NULL)
            THEN med.cexid
          ELSE NULL END                              AS medidempresacontratada,
          med.obrid,
          med.arqid,
          med.mednummedicao,
          to_char(med.meddtmedicao, 'DD/MM/YYYY')    AS meddtmedicao,
          to_char(med.meddtinicioexec, 'DD/MM/YYYY') AS meddtinicioexec,
          to_char(med.meddtfimexec, 'DD/MM/YYYY')    AS meddtfimexec,
          med.medvlrmedicao
        FROM obras2.medicoes med
        WHERE 
          med.medstatus = 'A'
          AND med.medid = {$medid}
          AND med.obrid = {$obrid};
    ";

    $arrMedicao = $db->carregar($sql);

    $retorno = !$arrMedicao ? array() : $arrMedicao;

    return $retorno;
}

/**
 * Função responsável por pesquisar os dados das medições de determinada nota.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15250
 * @param $medid
 * @return array|mixed|NULL
 */
function getDadosNotaMedicao($ntfid) {

    global $db;

    $arrDadosNotaMedicao = array();

    if (ctype_digit($ntfid)) {

    $sql = "
        SELECT
          ntm.ntmid,
          ntm.ntfid,
          to_char(ntf.ntfdtnota, 'DD/MM/YYYY') AS ntfdtnota,
          ntm.mecid,
          med.mednummedicao,
          to_char(med.meddtmedicao, 'DD/MM/YYYY') AS meddtmedicao,
          to_char(med.meddtinicioexec, 'DD/MM/YYYY') AS meddtinicioexec,
          to_char(med.meddtfimexec, 'DD/MM/YYYY') AS meddtfimexec,
          med.medvlrmedicao,
          ntm.ntmvlrpago,
          med.medid,
          CASE WHEN med.crtid NOTNULL THEN
            'crt_' || med.crtid
          ELSE
            'cce_' || med.cceid
          END as contrato
        FROM
          obras2.notafiscal ntf
        INNER JOIN
          obras2.notamedicao ntm USING (ntfid)
        INNER JOIN
          obras2.medicaocontrato mec USING (mecid)
        INNER JOIN
          obras2.medicoes med USING (medid)
        WHERE
          ntf.ntfstatus = 'A'
        AND
          ntm.ntmstatus = 'A'
        AND
          med.medstatus = 'A'
        AND
          mec.mecstatus = 'A'
        AND
          ntf.ntfid = {$ntfid};
    ";

        $result = $db->carregar($sql);

        if (is_array($result) && !empty($result)) {
            $arrDadosNotaMedicao = $result;
        }
    }

    return $arrDadosNotaMedicao;
}

/**
 * @todo. Comentar.
 * @param $ntfid
 * @return mixed
 */
function getDadosNotaFiscal($ntfid) {

    global $db;

    $arrDadosNotaFiscal = array();

    if (ctype_digit($ntfid)) {

        $sql = "
          SELECT
              CASE
              WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
                THEN 'ent'
              WHEN ntf.entid ISNULL AND ntf.cexid NOTNULL
                THEN 'cex'
              END AS tipoempresa,
              CASE
              WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
                THEN ntf.entid
              WHEN ntf.entid ISNULL AND ntf.cexid NOTNULL
                THEN ntf.cexid
              END AS idempresa,
              CASE
              WHEN ntf.crtid NOTNULL AND ntf.cceid ISNULL
                THEN 'crt'
              WHEN ntf.crtid ISNULL AND ntf.cceid NOTNULL
                THEN 'cce'
              END AS tipocontrato,
              CASE
              WHEN ntf.crtid NOTNULL AND ntf.cceid ISNULL
                THEN ntf.crtid
              WHEN ntf.crtid ISNULL AND ntf.cceid NOTNULL
                THEN ntf.cceid
              END AS idcontrato,
              ntf.arqid,
              ntf.ntfnumnota,
              to_char(ntf.ntfdtnota, 'DD/MM/YYYY') AS ntfdtnota,
              ntf.ntfvalornota,
              xx.obrid
          FROM obras2.notafiscal ntf
            INNER JOIN (
                           SELECT
                             med.obrid,
                             ntf.ntfid
                           FROM obras2.notafiscal ntf
                             INNER JOIN obras2.notamedicao ntm USING (ntfid)
                             INNER JOIN obras2.medicaocontrato mec USING (mecid)
                             INNER JOIN obras2.medicoes med USING (medid)
                           WHERE
                             ntf.ntfstatus = 'A'
                             AND
                             ntm.ntmstatus = 'A'
                             AND
                             med.medstatus = 'A'
                             AND
                             ntf.ntfid = {$ntfid}
                           GROUP BY med.obrid, ntf.ntfid
                         ) xx ON xx.ntfid = ntf.ntfid
          WHERE
            ntf.ntfstatus = 'A'
            AND
            ntf.ntfid = {$ntfid};
        ";

        $result = $db->carregar($sql);

        if (is_array($result) && !empty($result)) {
            $arrDadosNotaFiscal = $result;
        }
    }

    return $arrDadosNotaFiscal;
}

/**
 * @todo COMENTAR.
 * @param $medid
 * @param $valor
 * @param $obrid
 * @param $dataNota
 * @param $valorObj
 * @return array
 */
function calculaPerc($medid, $valor, $obrid, $dataNota, $valorObj){

    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);

    $strObrids = implode(",", $arrObrids);

    $valor = str_replace(".", "", $valor);
    $valor = str_replace(",", ".", $valor);

    $dataNota = formata_data_sql($dataNota);

    $sqlMedFisica = "WITH vtotal AS (SELECT DISTINCT ON (med.medid)
              med.medid,
              CASE WHEN mec.cceid ISNULL AND mec.crtid NOTNULL THEN
                  (SELECT crtvalorexecucao FROM obras2.contrato WHERE crtstatus = 'A' AND ttaid ISNULL AND crtid = mec.crtid)
                ELSE
                  (SELECT ccevalor FROM obras2.contratoconstrutoraextra WHERE ccestatus = 'A' AND cceid_pai ISNULL AND ttaid ISNULL AND cceid = mec.cceid)
              END as valorcontrato
            FROM
              obras2.medicoes med
            INNER JOIN
              obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
            WHERE
              medstatus = 'A'
            AND med.medid = {$medid})
              SELECT ({$valor}*100)/vtotal.valorcontrato as percentfisica
              FROM vtotal;
    ";

    $medicaoFisica = $db->pegaUm($sqlMedFisica);

    $sqlMedAcumulada = "WITH v as (
                          SELECT
                            c.crtvalorexecucao AS valorcontrato,
                            c.crtid as crtid,
                            NULL as cceid
                          FROM obras2.obrascontrato ocr INNER JOIN obras2.contrato c ON (ocr.crtid = c.crtid) OR (ocr.crtid = c.crtidpai)
                            INNER JOIN entidade.entidade ent ON ent.entid = c.entidempresa
                          WHERE ocr.ocrstatus = 'A' AND c.crtstatus = 'A' AND ent.entstatus = 'A' AND c.ttaid IS NULL AND ocr.obrid IN ({$strObrids})
                          GROUP BY c.crtid, valorcontrato

                          UNION ALL

                          SELECT
                            cce.ccevalor AS valorcontrato,
                            NULL as crtid,
                            cce.cceid as cceid
                          FROM obras2.construtoraextra cex
                            INNER JOIN obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
                          WHERE cex.cexstatus = 'A'
                                AND cce.ccedataassinatura IS NOT NULL
                                AND cce.ttaid IS NULL
                                AND
                                cex.obrid IN ({$strObrids})
                          GROUP BY cce.cceid, valorcontrato
                        ),

                        x AS (SELECT
                          sum(ntm.ntmvlrpago) AS vlrpago,
                          1 as codigo
                        FROM v
                        LEFT JOIN
                          obras2.notamedicao ntm ON v.cceid = ntm.cceid OR v.crtid = ntm.crtid
                        LEFT JOIN
                          obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid
                        WHERE
                          ntf.ntfdtnota <= '{$dataNota}'
                        ),

                        y as (
                            SELECT
                             sum(v.valorcontrato) as vlrcontrato,
                             1 as codigo
                            FROM v
                          )

                        SELECT
                            x.vlrpago as valorpago,
                            y.vlrcontrato as valortotal
                        FROM
                          x
                        INNER JOIN y ON x.codigo = y.codigo;
    ";

    $arrMedAculumada = $db->carregar($sqlMedAcumulada);
    $arrMedAculumada = $arrMedAculumada[0];
    $valorFinal = $arrMedAculumada['valorpago'] + $valor + $valorObj;

    $medicaoAcumulada = ($valorFinal*100)/$arrMedAculumada['valortotal'];

    if(!empty($medicaoFisica) && !empty($medicaoAcumulada)){

        $resposta = array(
            "medicaoFisica" => number_format($medicaoFisica, 2, ",", "."),
            "medicaoAcumulada" => number_format($medicaoAcumulada, 2, ",", ".")
        );

    } else{

        $resposta = array(
            "medicaoFisica" => "",
            "medicaoAcumulada" => ""
        );
    }

    return $resposta;

}

/**
 * @todo Comentar
 * @param $mecid
 * @return array|bool|mixed|NULL
 */
function getIdContratoNotaMedicao($mecid) {

    global $db;

    $sql = "
    SELECT
      mec.crtid,
      mec.cceid
    FROM  obras2.medicaocontrato mec
    WHERE
    mec.mecstatus = 'A'
    AND mec.mecid = {$mecid};
    ";

    $retorno = $db->pegaLinha($sql);
    return $retorno;

}

?>

<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/obras2.js?v=3"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<style type="text/css">

    tbody > tr:nth-child(even) {
        background-color: #f2f2f2;
    }

    td {
        vertical-align: middle;
        padding: 5px;
    }

    #tbAcompanhamento td {
        border: 1px solid #000;
        text-align: center;
        font-size: 13px;
    }

    .tbAcompanhamentoTitulo {
        font-weight: bold;
        text-align: center;
        border: 1px solid #000;
    }

    .acaoTbAcompanhamento {
        color: #0000FF;
        text-decoration: underline;
        cursor: pointer;
    }

    .detalheVisualizar {
        font-size: 14px;
        background-color: #DCDCDC;
        font-weight: bold;
    }

</style>

<form id="formulario" method="post" enctype="multipart/form-data" action="#">
    <input type="hidden" name="ntfid" id="ntfid" value="<?php echo $ntfid; ?>" />
    <input type="hidden" name="medid" id="medid" value="<?php echo $medid; ?>" />
    <!-- FORMULÁRIO DE INCLUSÃO DE NOTA FISCAL -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <?php
            $tituloTela = $ntfid ? "Editar Nota Fiscal" : "Incluir Nota FIscal";
            ?>
            <td class="SubTitulocentro" colspan="2"><?php echo $tituloTela; ?></td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Fornecedor:
                        </td>
                        <td>
                            <?php
                            if ($acaoEditar) {
                                $tipoEmpresaContratada = $arrDadosNotaFiscal["tipoempresa"];
                                $idEmpresaContratada = $arrDadosNotaFiscal["idempresa"];
                                $idContrato = $arrDadosNotaFiscal["idcontrato"];
                                $tipoContrato = $arrDadosNotaFiscal["tipocontrato"];
                                $selectedValue = $tipoEmpresaContratada . "_" . $idEmpresaContratada . "_" . $idContrato;
                                /* Criação de campo hidden utilizado para automatizar a seleção da empresa na tela de editar e, consequentemente
                                 * chamar o serviço para a busca da UF. */
                                echo "<input type='hidden' name='idEmpresaContratada' id='idEmpresaContratada' value={$selectedValue} />";
                                echo montaComboEmpresaContratada($obrid, $selectedValue);
                            } else {
                                echo montaComboEmpresaContratada($obrid);
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            UF Fornecedor:
                        </td>
                        <td class="detalheVisualizar" id="ufFornecedor"></td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            if ($acaoEditar) {
                                if (is_array($arquivoNotaFiscal) && !empty($arquivoNotaFiscal)) {
                                    ?>
                                    <div class="divArquivoDownload">
                                        <a class="link"
                                           onclick="excluirArquivo('<?= $arrDadosNotaFiscal['arqid'] ?>');">
                                            <img src="/imagens/excluir.gif"
                                                 title="Excluir arquivo <?= $arquivoNotaFiscal['arqnome'] ?>" />
                                        </a>
                                        &nbsp;
                                        <a class="link"
                                           onclick="downloadArquivo('<?= $arrDadosNotaFiscal['arqid'] ?>');">
                                        <img src="/imagens/salvar.png"
                                             title="Fazer download do arquivo." />
                                        <?= $arquivoNotaFiscal['arqnome'] . '.' . $arquivoNotaFiscal['arqextensao'] ?>
                                        </a>
                                    </div>
                                    <input type="hidden" name="arquivoAtual" id="arquivoAtual" value="<?= $arrDadosNotaFiscal['arqid'] ?>" />
                                    <div class="divArquivoUpload" style="display: none;">
                                        <input type="hidden" name="arqidOld" id="arqidOld" value="" />
                                        <input type="file" name="arquivoNota" id="arquivoNota"/>
                                        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                        <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                    </div>
                                    <?php
                                }
                            } else {
                                ?>
                                <div class="divArquivoUpload">
                                    <input type="file" name="arquivoNota" id="arquivoNota"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                                <?php
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Número da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            $numeroNotaFiscal = $acaoEditar ? $arrDadosNotaFiscal["ntfnumnota"] : null;
                            ?>
                            <input type="text" style="text-align:left;" name="numeroNota"
                                   id="numeroNota" size="15" maxlength="10" value="<?php echo $numeroNotaFiscal; ?>"
                                   onKeyUp="this.value=mascaraglobal('##########',this.value);"
                                   onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                   onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                                   title="Número da Nota Fiscal." class="obrigatorio normal"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." />
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Data da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            $dataNotaFiscal = $acaoEditar ? $arrDadosNotaFiscal["ntfdtnota"] : null;
                            echo campo_data2("dataNota", "N", "S", "Data da Nota Fiscal.", "", null, "calcularPercentualMedicoes();", $dataNotaFiscal, "calcularPercentualMedicoes();");
                            ?>
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>

                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            /* @todo VERFICIAR O USO DESTA VARIÁVEL. */
                            $valorNotaFiscal = $acaoEditar ? number_format($arrDadosNotaFiscal["ntfvalornota"], 2, ",", ".") : null;
                            ?>
                            <input
                                    type="text"
                                    style="text-align:left;"
                                    name="valorNota"
                                    id="valorNota"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);this.select();"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                    maxlength="18"
                                    size="17"
                                    class="obrigatorio normal"
                                    value="<?php echo $valorNotaFiscal; ?>"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE NOTA FISCAL. -->

    <!-- FORMULÁRIO DE BOLETIM DE MEDICAO -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Medições</td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Contrato ou Aditivo de Valor:
                        </td>
                        <td>
                            <?php
                            if ($acaoEditar) {
                                if ($tipoEmpresaContratada === "ent") {
                                    echo montaComboContratosAditivos($obrid, $idContrato, null, $medid);
                                }

                                if ($tipoEmpresaContratada === "cex") {
                                    echo montaComboContratosAditivos($obrid, null, $idContrato, $medid);
                                }
                            } else {
                                ?>
                                <select name="contratos" id="contratos">
                                    <option value="">Selecione...</option>
                                </select>
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                                <?php
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Boletim de Medição:
                        </td>
                        <td>
                            <select name="medicao" id="medicao" onchange="getDadosMedicao(this)">
                                <option value="">Selecione...</option>
                            </select>
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor da Medição:
                        </td>
                        <td class="detalheVisualizar" id="valorMedicao"></td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor Pago na Nota Fiscal:
                        </td>
                        <td>
                            <input
                                    type="text"
                                    style="text-align:left;"
                                    name="valorPagoNota"
                                    id="valorPagoNota"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);this.select();"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                    onblur="calcularPercentualMedicoes();"
                                    maxlength="18"
                                    size="17"
                                    class="obrigatorio normal"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            % Medição Física na NF:
                        </td>
                        <td class="detalheVisualizar"><span id="medicaoFisica"></span></td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            % Medição Física Acumulada:
                        </td>
                        <td class="detalheVisualizar"><span id="medicaoAcumulada"></span></td>
                    </tr>

                    <tr>
                        <td colspan="2" style="text-align: center">
                            <br/>
                            <input type="button" value="Adicionar Nota Fiscal" id="botaoAtualizarLista"
                                   onclick="atualizarListaContratos()"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE BOLETIM DE MEDICAO. -->

    <!-- Div para armazenar os dados da medição -->
    <div id="dadosMedicao" style="display: none">
        <input type="hidden" id="medid" value="">
        <input type="hidden" id="dataMedicao" value="">
        <input type="hidden" id="periodo" value="">
        <input type="hidden" id="vlrMedicao" name="vlrMedicao" value="">
        <input type="hidden" id="vlrReferencia" value="">
    </div>
    <!-- Fim Div para armazenar os dados da medição -->

    <br/>

    <!-- INÍCIO DA TABELA DE ACOMPANHAMENTO. -->
    <?php
        $display = $acaoEditar ? "block" : "none";
    ?>
    <div id="acompanhamento" style="display: <?php echo $display; ?>">

        <table width="95%" align="center" border="1" style="border-collapse: collapse" id="tbAcompanhamento">
            <thead>
            <tr>
                <td class="tbAcompanhamentoTitulo">Ações</td>
                <td class="tbAcompanhamentoTitulo">Numero da Medição</td>
                <td class="tbAcompanhamentoTitulo">Data da Mediçao</td>
                <td class="tbAcompanhamentoTitulo">Período de Execução</td>
                <td class="tbAcompanhamentoTitulo">Valor da Medição</td>
                <td class="tbAcompanhamentoTitulo">Valor Pago na Nota Fiscal</td>
                <td class="tbAcompanhamentoTitulo">% Medição Física na NF</td>
                <td class="tbAcompanhamentoTitulo">% Mediçao Fisica Acumulada</td>
            </tr>
            </thead>
            <?php

            if ($acaoEditar) {
                ?>
                <tbody>
                <?php
                $arrDadosNotaMedicao = getDadosNotaMedicao($ntfid);
                $arrM = array();

                $execucaoFinanceira = new ExecucaoFinanceira();

                foreach ($arrDadosNotaMedicao as $k=>$dadosNotaMedicao) {


                    $ntmid = $dadosNotaMedicao["ntmid"];
                    $mecid = $dadosNotaMedicao["mecid"];
                    $dataNotaFiscal = $dadosNotaMedicao["ntfdtnota"];
                    $numeroMedicao = $dadosNotaMedicao["mednummedicao"];
                    $dataMedicao = $dadosNotaMedicao["meddtmedicao"];

                    $periodoExec = (!empty($dadosNotaMedicao["meddtinicioexec"]) && !empty($dadosNotaMedicao["meddtfimexec"])) ? $dadosNotaMedicao["meddtinicioexec"] . " à " . $dadosNotaMedicao["meddtfimexec"] : "";

                    //$periodoExec = $dadosNotaMedicao["meddtinicioexec"] . " à " . $dadosNotaMedicao["meddtfimexec"];
                    $valorMedicao = $dadosNotaMedicao["medvlrmedicao"];
                    $valorMedicao = number_format($valorMedicao, 2, ",", ".");
                    $valorPagoNota = $dadosNotaMedicao["ntmvlrpago"];
                    $valorPagoNotaFormatado = number_format($valorPagoNota, 2, ",", ".");

                    $arrIdContratoNotaMedicao = getIdContratoNotaMedicao($mecid);
                    $crtid = $arrIdContratoNotaMedicao["crtid"];
                    $cceid = $arrIdContratoNotaMedicao["cceid"];

                    $valorContratoOriginal = $execucaoFinanceira->getValorContratoAtual($crtid, $cceid);

                    // Calculando o percentual da medição física.
                    $percentualMedicaoFisica = $execucaoFinanceira->calcularPercentualMedicao($valorPagoNota, $valorContratoOriginal);
                    $percentualMedicaoFisica = number_format($percentualMedicaoFisica, 2, ",", ".");
                    $percentualMedicaoFisica = $percentualMedicaoFisica . "%";

                    // Calculando o percentual da medição acumulada.
                    $tipoEmpresaContratada = $crtid ? "ent" : "cex";

                    if ($tipoEmpresaContratada === "cex") {
                        $percentualMedicaoFisica = ("Não se aplica.");
                        $percentualMedicaoAcumulada = ("Não se aplica.");

                    } else {

                        $valorContratoPrincipal = (float)$execucaoFinanceira->getValorContratoPrincipal($obrid);

                        $arrObras = $execucaoFinanceira->retornaObrids($obrid);
                        if (count($arrObras) > 1) {
                            $somatorioValoresPagosNotaOutrosContratos = (float)$execucaoFinanceira->getSomatorioValoresPagosNota($obrid, true);
                            $x = $valorContratoPrincipal + $somatorioValoresPagosNotaOutrosContratos;
                        } else {
                            $x = $valorContratoPrincipal;
                        }

                        $y = ((float)$execucaoFinanceira->getSomatorioValoresPagosNota($obrid)) + $valorPagoNota;

                        /*$somatorioNotasPagasOutrosContratos = getSomatoriosNotasPagasOutrosContratos($tipoEmpresaContratada, $arrObras, $dataNotaFiscal);
                        $somatorioNotasPagasOutrosContratos += $valorPagoNota;*/

                        //$percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($somatorioNotasPagasOutrosContratos, $somatorioTodosContratos);
                        $percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($y, $x);
                        $percentualMedicaoAcumulada = number_format($percentualMedicaoAcumulada, 2, ",", ".");
                        $percentualMedicaoAcumulada = $percentualMedicaoAcumulada . "%";
                    }

                    $arrTmp = array(
                        "id" => $k,
                        "ntmid" => $ntmid,
                        "idContrato" => $tipoContrato . '_' . $idContrato,
                        "idMedicaoContrato" => $mecid,
                        "valorPagoNota" => $dadosNotaMedicao["ntmvlrpago"]
                    );

                    array_push($arrM, $arrTmp);

                    echo "<tr>";
                    echo "<td><img src='/imagens/excluir.gif' alt='Excluir' title='Excluir' onclick='removerNotaTabelaAcompanhamento(this, {$k}, {$mecid}, {$numeroMedicao})' style='cursor:pointer;' />";
                               if($periodoExec){
                                 echo " <span class='acaoTbAcompanhamento' alt='Editar Nota Fiscal' title='Editar Nota Fiscal' 
                                        onclick='editarNotaFiscal(this, {$k}, {$mecid}, {$numeroMedicao}, \"" . $dadosNotaMedicao['contrato'] . "\",  \"" . $dadosNotaMedicao['ntmvlrpago'] . "\",  \"" .$valorMedicao. "\", \"" . $percentualMedicaoFisica . "\" , \"" . $percentualMedicaoAcumulada . "\", ".$dadosNotaMedicao['mecid']." )'>
                                        <img src='/imagens/alterar.gif' alt='Editar Nota Fiscal'/></span>";
    							}
    				echo "</td>";
                    echo "<td>{$numeroMedicao}</td>";
                    echo "<td>{$dataMedicao}</td>";
                    echo "<td>{$periodoExec}</td>";
                    echo "<td>{$valorMedicao}</td>";
                    echo "<td>{$valorPagoNotaFormatado}</td>";
                    echo "<td>{$percentualMedicaoFisica}</td>";
                    echo "<td>{$percentualMedicaoAcumulada}</td>";
                    echo "<tr>";
                }

                $arrM = json_encode($arrM);
                echo "<input type='hidden' name='dadosMedicaoNota' id='dadosMedicaoNota' value='{$arrM}' />";

                ?>
                </tbody>
            <?php } else { ?>
                <tbody></tbody>
            <?php } ?>

        </table>

        <br/>

        <div style="text-align: center">
            <input type="submit" id="botaoSalvar" value="Salvar""/>
            <input type="button" id="botaFechar" value="Fechar" onclick="javascript:window.close();"/>
        </div>
    </div>
    </div>
    <!-- FIM DA TABELA DE ACOMPANHAMENTO. -->

    <input type="hidden" id="requisicao" name="requisicao" value="salvarNotaMedicao"/>
    <input type="hidden" id="dadosNotaFiscalMedicao" name="dadosNotaFiscalMedicao" />
    <input type="hidden" id="notasRemovidas" name="notasRemovidas" />

</form>

<script type="text/javascript">

    var listaObjetoNota = [];
    var idControleObjDocumento = 0;
    var listaEmpresasRemovidas = [];
    var listaNotasRemovidas = [];
    var listaMedicoesContrato = [];
    var acaoEditar = false;
    var idEmpresaContratada = $("#idEmpresaContratada").val();
    var dadosMedicao = null;

    //    window.onunload = refreshParent;

    function editarNotaFiscal(elemento, indexObjDocumento, idMedicaoContrato, numeroMedicao,contrato,ntmvlrpago,valorMedicao, percentualMedicaoFisica, percentualMedicaoAcumulada,mecid)
    {

        ntmvlrpago = formatarValorMonetario(ntmvlrpago);
        $('#valorPagoNota').val(ntmvlrpago);

        $("#medicaoFisica").html(percentualMedicaoFisica);
        $("#medicaoAcumulada").html(percentualMedicaoAcumulada);


        $(elemento).closest("tr").remove();

        listaObjetoNota = listaObjetoNota.filter(function(e) {
            if (e.id == indexObjDocumento) {
                listaNotasRemovidas.push(e.ntmid);
            }
            return e.id != indexObjDocumento ;
        });


        $("#notasRemovidas").val(JSON.parse(JSON.stringify(listaNotasRemovidas)));

        var procurarMecid = function (idMedicaoContrato) {
            for(var x=0; x < listaMedicoesContrato.length; x++) {
                if (idMedicaoContrato == listaMedicoesContrato[x]) {
                    listaMedicoesContrato.splice(x, 1);
                }
            }
        };
        procurarMecid(idMedicaoContrato);

        if (listaObjetoNota.length <= 0) {
            $("#acompanhamento").hide();
            $("#dadosNotaFiscalMedicao").val("");
            listaMedicoesContrato = [];
        }

        /*Validação: Uma nota fiscal não pode pagar o mesmo contrato/aditivo mais de uma vez .*/
        var fornecedorSelecionado = $("#empresaContratada").val();

        montaComboContratos(fornecedorSelecionado)
        $("#contratos>[value=" + contrato + "]").attr("selected", true);

        montaComboMedicao(fornecedorSelecionado, contrato);
        $("#medicao>[value=" + mecid + "]").attr("selected", true);

        getDadosMedicao(mecid,true);

        /* Validação: Uma nota fiscal é vinculada aos contratos/aditivos de apenas uma empresa. */
        for (var i=0; i < listaEmpresasRemovidas.length; i++) {
            var empresaRemovida = listaEmpresasRemovidas[i];
            $('#empresaContratada').append($('<option>', {
                value: empresaRemovida.id,
                text: empresaRemovida.text
            }));
        }

        listaEmpresasRemovidas = [];

        // $("#empresaContratada").attr("disabled", false);

        $("#dadosNotaFiscalMedicao").val(JSON.stringify(listaObjetoNota));

    }




    $("#contratos").change(function(){
        var fornecedorSelecionado = $("#empresaContratada").val();
        var contratoSelecionado = $("#contratos").val();

        $('#valorMedicao').text("");
        montaComboMedicao(fornecedorSelecionado, contratoSelecionado);
    });

    $("#empresaContratada").change(function(){
        var opcao = $('#empresaContratada :selected').text();
        var cnpj = opcao.substring(1, 19);
        var cnpjLimpo = cnpj.replace(/\D+/g, "");
        consultaCNPJFornecedor(cnpjLimpo);
    });

    $(document).ready(function() {

        $("#formulario").submit(function () {
            var isValid = validarSubmit();
            if (isValid !== true) {
                alert(isValid);
                return false;
            }
            // É necessário habilitar o campo "Empresa Contratada" no momento que o formulário for submetido.
            $("#empresaContratada").attr("disabled", false);
        });

        acaoEditar = $("#ntfid").val() != "" ? true : false;
        if (acaoEditar) {

            listaNotasRemovidas = [];

            // Automatizando a seleção da empresa automaticamente para que seja feita a busca da UF pelo serviço.
            if (idEmpresaContratada) {
                $("#empresaContratada").val(idEmpresaContratada).trigger("change");
            }

            //listaObjetoNota = JSON.parse(JSON.stringify($("#dadosMedicaoNota").val()));
            listaObjetoNota = JSON.parse($("#dadosMedicaoNota").val());
            for (var x=0; x < listaObjetoNota.length; x++) {
                var o = listaObjetoNota[x];
                listaMedicoesContrato.push(o.idMedicaoContrato);
            }

            $("#dadosNotaFiscalMedicao").val(listaObjetoNota);
            $("#dadosMedicaoNota").remove();
            $("#empresaContratada").attr("disabled", true);
            idControleObjDocumento = listaObjetoNota.length;
        }
    });

    function validarSubmit() {

        var arquivoNota = $("#arquivoNota").get(0).files;
        var nomeArquivoNota = arquivoNota.length > 0 ? arquivoNota[0].name : "";
        var arqidOld = $("#arqidOld").val();

        var valorMedicao = $("#valorMedicao").text();
        valorMedicao = valorMedicao.replace(/[.]/g, "");
        valorMedicao = valorMedicao.replace(/[,]/g, ".");
        valorMedicao = parseFloat(valorMedicao);

        var valorNota = $("#valorNota").val();
        valorNota = valorNota.replace(/[.]/g, "");
        valorNota = valorNota.replace(/[,]/g, ".");
        valorNota = parseFloat(valorNota);

        var valorPagoNota = $("#valorPagoNota").val();
        valorPagoNota = valorPagoNota.replace(/[.]/g, "");
        valorPagoNota = valorPagoNota.replace(/[,]/g, ".");
        valorPagoNota = parseFloat(valorPagoNota);

        if ($("#empresaContratada").val() == "") {
            return "Informe a Empresa Contratada.";
        } else if ($("#ufFornecedor").text() == "") {
            return "Informe a UF do Fornecedor.";
        } else if (arqidOld != "" && acaoEditar) {
            if (arquivoNota.length == 0) {
                return "É necessário anexar a Nota Fiscal.";
            } else if (nomeArquivoNota.substr(nomeArquivoNota.lastIndexOf('.')).toLowerCase() !== ".pdf") {
                return "O arquivo da Nota Fiscal deve ser um arquivo no formato .PDF";
            }
        } else if ($("#numeroNota").val() == "") {
            return "Informe o Número da Nota Fiscal.";
        } else if ($("#dataNota").val() == "") {
            return "Informe a Data da Nota Fiscal.";
        } else if ((valorNota <= 0) || (isNaN(valorNota))) {
            return "O Valor da Nota Fiscal deve ser um número maior que zero.";
        } else if (listaObjetoNota.length < 1) {
            return "A nota fiscal precisa ser vinculada a pelo menos uma medição.";
        }

        var somatorio = 0;
        var somatorioValoresPagos = function (somatorio) {
            $.each(listaObjetoNota, function(k, v) {

                var vlrPagoNota = v.valorPagoNota;
                vlrPagoNota = parseFloat(vlrPagoNota);

                somatorio += vlrPagoNota;

            });
            somatorio = parseFloat(somatorio.toFixed(2));
            return somatorio;
        };

        if (somatorioValoresPagos(somatorio) != valorNota) {
            return "O somatório dos valores pagos na nota fiscal deve ser igual ao Valor da Nota Fiscal.";
        }

        return true;

    }

    /**
     * Função responsável por atualizar a página com a lista de itens na tela de notas fiscais. Esta atualização é feita
     * logo após o cadastro de uma nota fiscal.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     */
    function refreshParent() {
        window.opener.location.reload();
    }

    /**
     * Função para limpar os campos do formulario de medições
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     */
    function limpaFormMedicao() {
        $('#valorMedicao').text("");
        $('#valorPagoNota').val("");
        $('#medicaoFisica').text("");
        $('#medicaoAcumulada').text("");
        $('#vlrReferencia').val("");
    }

    /**
     * Função responsável por fazer a requisição para a montagem da combo 'Contrato ou Aditivo de Valor'.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param entid Id da 'entidade' ou da 'construtora extra'.
     * @returns {boolean}
     */
    function montaComboContratos(idFornecedor) {

        $("#contratos").empty().append("<option value=''>Selecione...</option>");

        if (idFornecedor.length <= 0) {
            return false;
        }

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            dataType: "json",
            data: {
                "requisicao": "carregarContratos",
                "idFornecedor": idFornecedor
            },
            beforeSend: function () {
                divCarregando();
            },
            success: function (data) {

                var selected = null;

                if (data.length > 0) {

                    selected = data.length == 1 ? "selected" : selected;

                    $.each(data, function (chave, valor) {
                        $("#contratos")
                            .append($("<option " + selected + "></option>")
                                .attr("value", valor.codigo)
                                .text(valor.descricao)
                            );
                    });
                }
                var fornecedor = $("#empresaContratada").val();
                var contrato = $("#contratos").val();
                if(fornecedor != "" && contrato != ""){
                    montaComboMedicao(fornecedor, contrato);
                }
                divCarregado();
            }
        });
    }

    /**
     * Função responsável por fazer a requisição para a montagem da combo 'Boletim de Medição'.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param fornecedor ID do contrato de origem da entidade ou de construtora extra'.
     * @param contrato ID do contrato de origem da entidade ou de construtora extra'.
     * @returns {boolean}
     */
    function montaComboMedicao(fornecedor, contrato) {

        $("#medicao").empty().append("<option value=''>Selecione...</option>");

        if (contrato.length <= 0) {
            return false;
        }

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            dataType: "json",
            data: {
                "requisicao": "carregarMedicao",
                "fornecedor": fornecedor,
                "contrato": contrato
            },
            beforeSend: function () {
                divCarregando();
            },
            success: function (data) {

                var selected = null;

                if (data.length > 0) {

                    $.each(data, function (chave, valor) {
                        $("#medicao")
                            .append($("<option " + selected + "></option>")
                                .attr("value", valor.codigo)
                                .text(valor.descricao)
                            );
                    });
                }
                divCarregado();
            }
        });
    }

    /**
     * Função responsável por montar a tabela de acompanhamento das medições e preparar os dados para salvar.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {boolean}
     */
    function atualizarListaContratos() {

        var retornoValidacao = validarDadosFormulario();

        if (retornoValidacao !== true) {
            alert(retornoValidacao);
            return false;
        }

        var idEmpresaContratada = $("#empresaContratada").val();
        //var nomeEmpresaContratada = $("#empresaContratada option:selected").text();
        var idContrato = $("#contratos").val();
        var nomeContrato = $("#contratos option:selected").text();
        var tipoContrato = (nomeContrato.substr(0, 3)).toLowerCase();
        var numeroNota = $("#numeroNota").val();
        var dataNota = $("#dataNota").val();
        var valorNota = $("#valorNota").val();
        valorNota = valorNota.replace(/[.]/g, "");
        valorNota = valorNota.replace(/[,]/g, ".");
        var idMedicaoContrato = $("#medicao option:selected").val();
        var numeroMedicao = $("#medicao option:selected").text();
        var valorMedicao = $("#valorMedicao").text();
        var valorPagoNota = $("#valorPagoNota").val();
        valorPagoNota = valorPagoNota.replace(/[.]/g, "");
        valorPagoNota = valorPagoNota.replace(/[,]/g, ".");

        /* Validação: Uma medição não pode ser paga mais de uma vez na nesma nopta. */
        listaMedicoesContrato.push(idMedicaoContrato);
        var procurarMecid = function (idMedicaoContrato) {
            for(var x=0; x < listaMedicoesContrato.length; x++) {
                if (listaMedicoesContrato[x] == idMedicaoContrato) {
                    return true;
                }
            }
            return false;
        };
        /*if (listaMedicoesContrato.length > 1) {
            if (procurarMecid(idMedicaoContrato)) {
                alert("A mesma medição não pode pode ser paga mais de uma vez.");
                return false;
            }
        }*/
        /* Fim da Validação: Uma medição não pode ser paga mais de uma vez na nesma nota. */

        $("#acompanhamento").show();

        var objNota = {
            "id": idControleObjDocumento, // Este ID não é do banco de dados. Serve apenas para controle no javascript.
            //"idEmpresaContratada": idEmpresaContratada,
            "idContrato": idContrato,
            "tipoContrato": tipoContrato,
            //"numeroNota": numeroNota,
            //"dataNota": dataNota,
            //"valorNota": valorNota,
            "idMedicaoContrato": idMedicaoContrato,
            //"valorMedicao": valorMedicao,
            //"numeroMedicao": numeroMedicao,
            "valorPagoNota": valorPagoNota
        };

        /* Validação: Uma nota fiscal não pode pagar o mesmo contrato/aditivo mais de uma vez . */
        $("#medicao option[value='" + idMedicaoContrato + "']").remove();

        /* Validação: Uma nota fiscal é vinculada aos contratos/aditivos de apenas uma empresa. */
        $("#empresaContratada option:not([value='" + idEmpresaContratada + "'], [value=''])").each(function () {
           listaEmpresasRemovidas.push(
               {
                   "id": $(this).val(),
                   "text": $(this).text()
               }
           );
        });

        $("#empresaContratada option:not([value='" + idEmpresaContratada + "'], [value=''])").remove();
        $("#empresaContratada").attr("disabled", true);

        listaObjetoNota.push(objNota);
        idControleObjDocumento++;

        /* Na tabela de acompanhamento, o "Valor Referente ao Contrato ou Aditivo de Valor" é exibido com máscara. */
        $("#tbAcompanhamento > tbody:last-child").append(
            "<tr>"
            + "<td><img src='/imagens/excluir.gif' alt='Excluir' title='Excluir' onclick='removerNotaTabelaAcompanhamento(this, " + objNota.id + ", " + objNota.idMedicaoContrato + ", \"" + numeroMedicao + "\")' style='cursor:pointer;' /></td>"
            + "<td>" + numeroMedicao + "</td>"
            + "<td>" + dadosMedicao.meddtmedicao + "</td>"
            + "<td>" + dadosMedicao.meddtinicioexec + " à " + dadosMedicao.meddtfimexec + "</td>"
            + "<td>" + valorMedicao + "</td>"
            + "<td>" + $("#valorPagoNota").val() + "</td>"
            + "<td>" + $("#medicaoFisica").text() + "</td>"
            + "<td>" + $("#medicaoAcumulada").text() + "</td>"
            + "</tr>"
        );

        limpaFormMedicao();

        var jsonString = JSON.stringify(listaObjetoNota);

        $("#dadosNotaFiscalMedicao").val(JSON.parse(jsonString));

    }

    /**
     * Função responsável por remover da tabela de acompanhamento determinada medição.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param elemento
     * @param ntmid
     * @param indexObjDocumento
     * @param idMedicao
     * @param numeroMedicao
     */
    function removerNotaTabelaAcompanhamento(elemento, indexObjDocumento, idMedicaoContrato, numeroMedicao) {

        $(elemento).closest("tr").remove();

        listaObjetoNota = listaObjetoNota.filter(function(e) {
            if (e.id == indexObjDocumento) {
                listaNotasRemovidas.push(e.ntmid);
            }
            return e.id != indexObjDocumento ;
        });

        if (acaoEditar) {
            if (listaObjetoNota.length < 1) {
                var arquivoAtual = $("#arquivoAtual").val();
                $("#arqidOld").val(arquivoAtual);
                $(".divArquivoDownload").hide("fast");
                $(".divArquivoUpload").show("fast");
            }
        }

        $("#notasRemovidas").val(JSON.parse(JSON.stringify(listaNotasRemovidas)));

        var procurarMecid = function (idMedicaoContrato) {
            for(var x=0; x < listaMedicoesContrato.length; x++) {
                if (idMedicaoContrato == listaMedicoesContrato[x]) {
                    listaMedicoesContrato.splice(x, 1);
                }
            }
        };
        procurarMecid(idMedicaoContrato);

        if (listaObjetoNota.length <= 0) {
            $("#acompanhamento").hide();
            $("#dadosNotaFiscalMedicao").val("");
            listaMedicoesContrato = [];
        }

        /*Validação: Uma nota fiscal não pode pagar o mesmo contrato/aditivo mais de uma vez .*/
        if(numeroMedicao != null) {
            $("#medicao").append($('<option>', {
                value: idMedicaoContrato,
                text: numeroMedicao
            }));
        }

        /* Validação: Uma nota fiscal é vinculada aos contratos/aditivos de apenas uma empresa. */
        for (var i=0; i < listaEmpresasRemovidas.length; i++) {
            var empresaRemovida = listaEmpresasRemovidas[i];
            $('#empresaContratada').append($('<option>', {
                value: empresaRemovida.id,
                text: empresaRemovida.text
            }));
        }

        listaEmpresasRemovidas = [];

        $("#empresaContratada").attr("disabled", false);

        $("#dadosNotaFiscalMedicao").val(JSON.stringify(listaObjetoNota));
    }

    /**
     * Função responsável por validar os campos do formulário para inclusão das medições.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {*}
     */
    function validarDadosFormulario() {

        var arquivoNota = $("#arquivoNota").get(0).files;
        var nomeArquivoNota = arquivoNota.length > 0 ? arquivoNota[0].name : "";
        var arqidOld = $("#arqidOld").val();

        var valorMedicao = $("#valorMedicao").text();
        valorMedicao = valorMedicao.replace(/[.]/g, "");
        valorMedicao = valorMedicao.replace(/[,]/g, ".");
        valorMedicao = parseFloat(valorMedicao);

        var valorNota = $("#valorNota").val();
        valorNota = valorNota.replace(/[.]/g, "");
        valorNota = valorNota.replace(/[,]/g, ".");
        valorNota = parseFloat(valorNota);

        var valorPagoNota = $("#valorPagoNota").val();
        valorPagoNota = valorPagoNota.replace(/[.]/g, "");
        valorPagoNota = valorPagoNota.replace(/[,]/g, ".");
        valorPagoNota = parseFloat(valorPagoNota);

        if ($("#empresaContratada").val() == "") {
            return "Informe a Empresa Contratada.";
        } else if ($("#ufFornecedor").text() == "") {
            return "Informe a UF do Fornecedor.";
        } else if (arqidOld != "" && acaoEditar) {
            if (arquivoNota.length == 0) {
                return "É necessário anexar a Nota Fiscal.";
            } else if (nomeArquivoNota.substr(nomeArquivoNota.lastIndexOf('.')).toLowerCase() !== ".pdf") {
                return "O arquivo da Nota Fiscal deve ser um arquivo no formato .PDF";
            }
        } else if ($("#numeroNota").val() == "") {
            return "Informe o Número da Nota Fiscal.";
        } else if ($("#dataNota").val() == "") {
            return "Informe a Data da Nota Fiscal.";
        } else if ((valorNota <= 0) || (isNaN(valorNota))) {
            return "O Valor da Nota Fiscal deve ser um número maior que zero.";
        } else if ($("#contratos").val() == "") {
            return "Informe o Contrato ou Aditivo de Valor.";
        } else if ($("#medicao").val() == "") {
            return "Informe o Boletim de Medição.";
        } else if ((valorMedicao <= 0) || (isNaN(valorMedicao))) {
            return "O Valor da Medição deve ser um número maior que zero.";
        } else if ((valorPagoNota <= 0) || (isNaN(valorPagoNota))) {
            return "O Valor Pago na Nota Fiscal deve ser um número maior que zero.";
        } else if ($("#medicaoFisica").text() == "") {
            return "Informe o percentual da Medição Física na Nota Fiscal.";
        } else if ($("#medicaoAcumulada").text() == "") {
            return "Informe o percentual da Medição Física Acumulada.";
        }

        return true;

    }

    /**
     * Função responsável por realizar a falsa exclusão do arquivo da medição.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param arqid
     */
    function excluirArquivo(arqid) {
        if(confirm('Confirma a exclusão do arquivo?')) {
            $("#arqidOld").val(arqid);
            $(".divArquivoDownload").hide("fast");
            $(".divArquivoUpload").show("fast");
        }
    }

    /**
     * Função responsável pelo download do arquivo da nota.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param arqid
     */
    function downloadArquivo(arqid) {
        window.location = window.location.href + "&requisicao=download&arqid=" + arqid;
    }

    /**
     * Função responsável por verificar se o CNPJ fornecido está no formato válido.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15250
     * @param cnpj
     */
    function validarCPNJ(cnpj) {

        var retorno = false;

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            data: {
                "requisicao": "validarCNPJ",
                "cnpjFornecedor": cnpj
            },
            success: function (data) {
                retorno = (data == "true") || false;
            }
        });

        return retorno;
    }

    /**
     * Função para pesquisa de dados de um fornecedor por CNPJ.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15250
     * @param cpnj
     * @returns {boolean}
     */
    function consultaCNPJFornecedor(cnpj) {

        $("#ufFornecedor").text("");

        if (cnpj.trim() == "") {
            alert("É obrigatório informar o CNPJ do fornecedor.");
            $("#cnpjFornecedor").focus();
            return false;
        }

        if (!validarCPNJ(cnpj)) {
            alert("O CNPJ fornecido é inválido.");
            $("#cnpjFornecedor").val("");
            $("#cnpjFornecedor").focus();
            return false;
        }

        //divCarregando();
        setTimeout(function () {
            var pessoaJudirica = new dCNPJ();
            pessoaJudirica.buscarDados(cnpj);
            if (pessoaJudirica.dados.no_empresarial_rf == "") {
                alert("Não há informações sobre o CNPJ informado em nossa base de dados.");
                return false;
            }

            $("#ufFornecedor").text(pessoaJudirica.dados.sg_uf);
        }, 0);
        //divCarregado();

        return false;
    }

    /**
     * @todo Comentar depois
     */
    function getDadosMedicao(objSelecionado,edicao = false) {

        if(edicao ==  true){
            mecid = objSelecionado;
        }else{
            mecid = objSelecionado.value;
        }



        if (mecid.length <= 0) {
            return false;
        }

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            dataType: "json",
            data: {
                "requisicao": "getDadosMedicao",
                "mecid": mecid
            },
            beforeSend: function () {
                divCarregando();
            },
            success: function (data) {

                dadosMedicao = data;

                var valorMedicao = parseFloat(data.medvlrmedicao);
                valorMedicaoFormatado = formatarValorMonetario(valorMedicao);

                $('#valorMedicao').text(valorMedicaoFormatado);

                divCarregado();
            }
        });

    }

    /**
     * Função responsável por fazer a requisição para calcular os percentuais da medição.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15250
     * @param valor Id da medição.
     * @returns {boolean}
     */
    function calcularPercentualMedicoes(){

        var valorPagoNota = $("#valorPagoNota").val();
        var valorPagoNota = $("#valorPagoNota").val();
        valorPagoNota = valorPagoNota.replace(/[.]/g, "");
        valorPagoNota = valorPagoNota.replace(/[,]/g, ".");
        var dataNota = $("#dataNota").val();
        var dadosEmpresaContratada = $("#empresaContratada").val();
        var dadosContratoAditivo = $("#contratos").val();
        var valorMedicao = $("#valorMedicao").text();
        valorMedicao = valorMedicao.replace(/[.]/g, "");
        valorMedicao = valorMedicao.replace(/[,]/g, ".");

        var arrDadosEmpresaContratada = dadosEmpresaContratada.split("_");
        var tipoEmpresaContratada = arrDadosEmpresaContratada[0];
        var idContrato = arrDadosEmpresaContratada[2];

        if (valorPagoNota == "" || dataNota == "") {
            return false;
        }

        var totalPagoNota = 0;
        var getSomatorioValoresPagos = function (totalPagoNota) {
            $.each(listaObjetoNota, function(index, value) {
                if(value.tipoContrato == "con"){
                    var valorPago = value.valorPagoNota;
                    totalPagoNota += parseFloat(valorPago);
                }
            });
            return totalPagoNota;
        };

        totalPagoNota = getSomatorioValoresPagos(totalPagoNota) + parseFloat(valorPagoNota);
//        console.log(getSomatorioValoresPagos(totalPagoNota));
//        console.log(parseFloat(valorPagoNota));

        $.ajax({

            url: window.location.href,
            type: "post",
            async: true,
            dataType: "json",
            data: {
                "requisicao": "calcularPercentuaisMedicao",
                "tipoEmpresaContratada": tipoEmpresaContratada,
                "idContrato": idContrato,
                "dadosContratoAditivo": dadosContratoAditivo,
                "valorPagoNota": valorPagoNota,
                "totalPagoNota": totalPagoNota,
                "dataNota": dataNota,
                "valorMedicao": valorMedicao
            },
            beforeSend: function () {
                $("#medicaoFisica").html("<img src='/imagens/icones/ajax-loader.gif'>&nbsp;Realizando cálculo...");
                $("#medicaoAcumulada").html("<img src='/imagens/icones/ajax-loader.gif'>&nbsp;Realizando cálculo...");
                $("#botaoAtualizarLista").attr("disabled", true);
                $("#dataNota").attr("disabled", true);
                $("#valorPagoNota").attr("disabled", true);
            },
            success: function (data) {
                $("#botaoAtualizarLista").attr("disabled", false);
                $("#dataNota").attr("disabled", false);
                $("#valorPagoNota").attr("disabled", false);
                $('#medicaoFisica').text(JSON.stringify(data));
                $('#medicaoFisica').text(data.fisica);
                $('#medicaoAcumulada').text(data.acumulada);
            }
        });
    }

</script>

</body>