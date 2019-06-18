<?php


$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];
$medid = null;

if (empty($obrid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
           close();
         </script>");
}

/* Requisição para solicitar o carregamento da combo de contratos.*/
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "carregarContratos") {
    ob_clean();
    $entidInfo = trim($_POST["entid"]);
    $arrEntidInfo = explode("_", $entidInfo);
    $origemEmpresa = $arrEntidInfo[0];

    $crtid = false;
    $cceid = false;

    if ($origemEmpresa == "ent") {
        $crtid = $arrEntidInfo[2];
    } elseif ($origemEmpresa == "cex") {
        $cceid = $arrEntidInfo[2];
    }

    echo json_encode(carregarContratos($crtid, $cceid));
    exit;
}

/* Requisição para verificar se existe 'medid', ou seja, se será carregada a tela de edição da medição. */
if (isset($_GET["medid"]) && ctype_digit($_GET["medid"])) {
    $medid = $_GET["medid"];

    $arrMedicao = getDadosMedicao($obrid, $medid);
    $arrDadosMedicao = !empty($arrMedicao) ? current($arrMedicao) : $arrMedicao;

    $arqid = $arrDadosMedicao["arqid"];

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $arquivoMedicao = $execucaoFinanceira->recuperaDadosArquivo($arqid);

    if (!empty($arrDadosMedicao)) {
        if (!in_array($arrDadosMedicao["obrid"], $arrObrids)) {
            echo "<script>
                    alert('Atenção, você está editando informações de uma obra não permitida.');
                  </script>
                  window.close();
            ";
            exit;
        }
    }
}

/* Requisição para solicitar a gravação das medições. */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "salvarMedicaoContratosAditivos") {
    ob_clean();

    if ($_POST["medid"] && ctype_digit($_POST["medid"])) {
        atualizarMedicao($obrid, $medid, $_POST);
    } else {
        salvarMedicao($obrid, $_POST);
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

/**
 * Função responsável por validar os dados inseridos no formulário de cadastro de medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $arrDados
 * @return array
 */
function validarDados($arrDados)
{

    $arrRetorno = array("error" => false, "message" => null);

    if (is_array($arrDados) && !empty($arrDados)) {
        // Verificando se a empresa contratada foi informada.
        $empresaContratada = trim($arrDados["empresacontratada"]);
        if (empty($empresaContratada)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe a Empresa Contratada.";
        }

        // Verificando se o número da medição foi informado.
        $numeroMedicao = trim($arrDados["numeroMedicao"]);
        if (empty($numeroMedicao)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Número da Medição.";
        }

        // Verificando se a data da medição é uma data válida.
        $dataMedicao = trim($arrDados["dataMedicao"]);
        if (empty($dataMedicao)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe a Data da Medição.";
        } else {
            $arrDataMedicao = explode("/", $arrDados["dataMedicao"]);
            if (!checkdate($arrDataMedicao[1], $arrDataMedicao[0], $arrDataMedicao[2])) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "A Data da Medição não tem formato válido.";
            }
        }

        // Verificando se o Período de Execução é válido
        $periodoExec1 = trim($arrDados["periodoExec1"]);
        $periodoExec2 = trim($arrDados["periodoExec2"]);
        if (empty($periodoExec1) || empty($periodoExec2)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Período de Execução.";
        } else {
            $arrDataMedicao1 = explode("/", $arrDados["periodoExec1"]);
            $arrDataMedicao2 = explode("/", $arrDados["periodoExec2"]);

            $checkPeriodo1 = checkdate($arrDataMedicao1[1], $arrDataMedicao1[0], $arrDataMedicao1[2]);
            $checkPeriodo2 = checkdate($arrDataMedicao2[1], $arrDataMedicao2[0], $arrDataMedicao2[2]);

            if (!$checkPeriodo1 || !$checkPeriodo2) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "O Período de Execução não tem formato válido.";
            }
        }

        // Verificando se o Valor da Medição foi informado.
        $valorMedicao = trim($arrDados["valorMedicao"]);
        if (empty($valorMedicao)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Informe o Valor da Medição.";
        } else {
            if (!is_numeric($arrDados["valorMedicao"])) {
                $arrRetorno["error"] = true;
                $arrRetorno["message"] = "O Valor da Medição deve ser um número válido.";
            }
        }

        // Verificando os dados que ligam medição e contrato/aditivo.
        if (!is_array($arrDados["medicaoContratos"]) || (is_array($arrDados["medicaoContratos"]) && empty($arrDados["medicaoContratos"]))) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "Problemas ao iniciar a gravação. Faltam dados para vincular a medição ao contrato/aditivo.";
        }

        // Validando os dados para cada aditivo (Regra 16, TL02).
        $arrValoresMedicoesContratos = array();
        if (is_array($arrDados["medicaoContratos"]) && !empty($arrDados["medicaoContratos"])) {
            foreach ($arrDados["medicaoContratos"] as $mc) {
                $vca = $mc->valorContratoAditivo;
                $vca = str_replace(".", "", $vca);
                $vca = str_replace(",", ".", $vca);
                $arrValoresMedicoesContratos[] = (float)$vca;
            }
        }

        $valorTotalMedicao = $arrDados["valorMedicao"];
        $somatorio = array_sum($arrValoresMedicoesContratos);

        if (round($somatorio, 2) != round($valorTotalMedicao, 2)) {
            $arrRetorno["error"] = true;
            $arrRetorno["message"] = "O somatório dos valores das medições deve ser igual ao valor total da medição.";
        }
    } else {
        $arrRetorno["error"] = true;
        $arrRetorno["message"] = "A medição não foi gravada, pois houve problemas com os dados informados.";
    }

    return $arrRetorno;
}

/**
 * Função responsável por carregar o elemento select com as empresas contratadas.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @return string
 */
function montaComboEmpresaContratada($obrid, $selectedValue = null)
{

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
          'ent_' || ent.entid || '_' || c.crtid                            AS codigo,
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
          'cex_' || cex.cexid || '_' || cce.cceid  AS codigo,
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
        return $db->monta_combo(
            "empresaContratada",
            $sql,
            "S",
            "Selecione...",
            "montaComboContratos",
            '',
            '',
            '',
            'S',
            'empresaContratada',
            '',
            $selectedValue
        );
    }
    return $db->monta_combo(
        "empresaContratada",
        $sql,
        "S",
        "Selecione...",
        "montaComboContratos",
        '',
        '',
        '',
        'S',
        'empresaContratada'
    );
}

/**
 * Função responsável por carregar o elemento select com os contratos/aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $entid
 * @param $cxeid
 * @param null $medid
 * @return string
 */
function montaComboContratosAditivos($medid)
{

    global $db;

    $sql = "SELECT crtid, cceid FROM obras2.medicoes WHERE medid = {$medid}";

    $dados = $db->pegaLinha($sql);
    $dados = $dados ? $dados : array();

    $arrContratosAditivos = carregarContratos($dados['crtid'], $dados['cceid'], $medid);

    return $db->monta_combo("contratos", $arrContratosAditivos, "S", "Selecione...", "", '', '', '', 'S', 'contratos');
}

/**
 * Função responsável por carregar o elemento select com os contratos ou aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid Id da medição.
 * @param $entid Id da empresa quando a origem da obra for 'Entidade'.
 * @param $cexid Id da empresa quando a origem da obra for 'Construtuora extra (Cumprimento do Objeto)'.
 * @return array|mixed|NULL
 */
function carregarContratos($crtid, $cceid, $medid = null)
{

    global $db;

    $arrContratos = array();

    if (!$crtid && !$cceid) {
        return $arrContratos;
    }

    if (ctype_digit($crtid) && !$cceid) {
        if ($medid) {
            $filtro = "AND crt.crtid NOT IN (SELECT crtid FROM obras2.medicoes WHERE medid = 29396)";
        } else {
            $filtro = "";
        }

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
                    ) AND crtstatus = 'A' {$filtro} order by descricao desc, crt.crtdtassinatura 
        ";
    } elseif (ctype_digit($cceid) && !$crtid) {
        if ($medid) {
            $filtro = "AND cce.cceid NOT IN (SELECT cceid FROM obras2.medicoes WHERE medid = 29396)";
        } else {
            $filtro = "";
        }

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
			) {$filtro} 
		";
    }
    
    $arrContratos = $sql ? $db->carregar($sql) : false;
    if (!$arrContratos) {
        $arrContratos = array();
    }

    return $arrContratos;
}

/**
 * Função responsável por salvar as medições dos contratos/aditivos.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $arrDados
 */
function salvarMedicao($obrid, $arrDados)
{

    $arrDados["medicaoContratos"] = json_decode(str_replace("\\", "", $arrDados["medicaoContratos"]));

    $arrDados["valorMedicao"] = str_replace(".", "", $arrDados["valorMedicao"]);
    $arrDados["valorMedicao"] = str_replace(",", ".", $arrDados["valorMedicao"]);
    $arrDados["valorMedicao"] = (float)$arrDados["valorMedicao"];

    $msgRetorno = null;

    if (!empty($_FILES["arquivoMedicao"]["name"])) {
        // O Boletim de Medição deve ser um arquivo no formato PDF.
        $arrMimeType = array(
            "application/pdf",
            "application/x-pdf",
            "application/acrobat",
            "applications/vnd.pdf",
            "text/pdf",
            "text/x-pdf",
            "application/save",
            "application/force-download",
            "application/download",
            "application/x-download"
        );
        if (in_array($_FILES["arquivoMedicao"]["type"], $arrMimeType)) {
            $validacao = validarDados($arrDados);
            if ($validacao["error"] === true) {
                $errorMessage = $validacao["message"];
                //echo "<script>alert('{$errorMessage}');window.close();</script>";
                echo "<script>alert('{$errorMessage}');</script>";
                exit;
            } else {
                $empresaContratada = $arrDados["empresacontratada"];
                $arrEmpresaContratada = explode("_", $empresaContratada);
                $tipoEmpresaContratada = $arrEmpresaContratada[0];
                $idEmpresaContratada = $arrEmpresaContratada[1];
                $idContratoEmpresaContratada = $arrEmpresaContratada[2];

                // A empresa contratada deve ser 'ent' (Entidade) ou 'cex' (Construtora Extra).
                if ($tipoEmpresaContratada === "ent" || $tipoEmpresaContratada === "cex") {
                    $numeroMedicao = $arrDados["numeroMedicao"];
                    $arrMedicaoContratos = $arrDados["medicaoContratos"];
                    $descricaoArquivo = "medicao_" . $numeroMedicao . "_" . $empresaContratada;
                    $dataMedicao = formata_data_sql($arrDados["dataMedicao"]);
                    $periodoExec1 = formata_data_sql($arrDados["periodoExec1"]);
                    $periodoExec2 = formata_data_sql($arrDados["periodoExec2"]);
                    $valorMedicao = $arrDados["valorMedicao"];
                    $usucpf = $_SESSION["usucpf"];

                    require_once(APPRAIZ . "includes/classes/fileSimec.class.inc");
                    $fileSimec = new FilesSimec();
                    $fileSimec->setUpload($descricaoArquivo, null, false);
                    $arqid = $fileSimec->getIdArquivo();

                    $colunaIdContrato = $tipoEmpresaContratada === "ent" ? "crtid" : "cceid";

                    if ($arqid) {
                        global $db;

                        $sql = "
                          INSERT INTO obras2.medicoes
                            (obrid, {$colunaIdContrato}, arqid, mednummedicao, meddtmedicao, meddtinicioexec, meddtfimexec, medvlrmedicao, usucpfcriacao, meddtcriacao)
                          VALUES 
                            ({$obrid}, {$idContratoEmpresaContratada}, {$arqid}, {$numeroMedicao}, '{$dataMedicao}', '{$periodoExec1}', '{$periodoExec2}', {$valorMedicao}, '{$usucpf}', 'now()')
                          RETURNING medid";

                        $medid = $db->pegaUm($sql);

                        if ($medid) {
                            $sql2 = "
                              INSERT INTO obras2.medicaocontrato
                                (medid, {$colunaIdContrato}, mecvlrreferencia, usucpfcriacao, mecdtcriacao)
                              VALUES \n";

                            if (!empty($arrMedicaoContratos)) {
                                foreach ($arrMedicaoContratos as $mc) {
                                    $arrIdContratoAditivo = explode("_", $mc->idContratoAditivo);
                                    $idContratoAditivo = (int)$arrIdContratoAditivo[1];
                                    $valorContratoAditivo = $mc->valorContratoAditivo;
                                    $valorContratoAditivo = str_replace(".", "", $valorContratoAditivo);
                                    $valorContratoAditivo = str_replace(",", ".", $valorContratoAditivo);

                                    $sql2 .= "({$medid}, {$idContratoAditivo}, {$valorContratoAditivo}, '{$usucpf}', 'now()'),";
                                }

                                $sql2 = rtrim($sql2, "\n,");

                                $db->executar($sql2);

                                if ($db->commit()) {
                                    echo "<script>
                                        alert('Informações gravadas com sucesso.');
                                         window.opener.location.reload();
                                    </script>";
                                    exit;
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
                                alert('A medição não foi salva. \\nVerifique os dados fornecidos.');
                                window.close();
                            </script>";
                        }
                    } else {
                        echo "<script>
                            alert('O arquivo do Boletim de Medição não foi salvo. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                    }
                } else {
                    echo "<script>
                            alert('Problemas ao verificar a Empresa Contratada. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                }
            }
        } else {
            $msgRetorno = "O Boletim de Medição deve ser um arquvo no formato PDF.";
        }
    } else {
        $msgRetorno = "O Boletim de Medição não foi enviado.";
    }

    echo "<script>alert('{$msgRetorno}');</script>";
    exit;
}

/**
 * Função responsável por alterar uma medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $medid
 * @param $arrDados
 */
function atualizarMedicao($obrid, $medid, $arrDados)
{

    $arrDados["medicaoContratos"] = json_decode(str_replace("\\", "", $arrDados["medicaoContratos"]));

    $arrDados["valorMedicao"] = str_replace(".", "", $arrDados["valorMedicao"]);
    $arrDados["valorMedicao"] = str_replace(",", ".", $arrDados["valorMedicao"]);
    $arrDados["valorMedicao"] = (float)$arrDados["valorMedicao"];

    $msgRetorno = null;

    $validacao = validarDados($arrDados);

    if ($validacao["error"] === true) {
        $errorMessage = $validacao["message"];
        //echo "<script>alert('{$errorMessage}');window.close();</script>";
        echo "<script>alert('{$errorMessage}');</script>";
        exit;
    } else {
        $arqid = null;
        $empresaContratada = $arrDados["empresacontratada"];
        $numeroMedicao = $arrDados["numeroMedicao"];
        $arrEmpresaContratada = explode("_", $arrDados["empresacontratada"]);
        $tipoEmpresaContratada = $arrEmpresaContratada[0];
        $idEmpresaContratada = $arrEmpresaContratada[1];
        $idContratoEmpresaContratada = $arrEmpresaContratada[2];

        $colunaIdContratoNovo = $tipoEmpresaContratada === "ent" ? "crtid" : "cceid";
        $colunaIdContratoVelha = $tipoEmpresaContratada === "ent" ? "cceid" : "crtid";

        $colunaIdContrato = $tipoEmpresaContratada === "crt" ? "crtid" : "cceid";
        //$tipoEmpresaContratada = strtok($arrDados["empresacontratada"], "_");
        $descricaoArquivo = "medicao_" . $numeroMedicao . "_" . $empresaContratada;
        $dataMedicao = formata_data_sql($arrDados["dataMedicao"]);
        $periodoExec1 = formata_data_sql($arrDados["periodoExec1"]);
        $periodoExec2 = formata_data_sql($arrDados["periodoExec2"]);
        $valorMedicao = $arrDados["valorMedicao"];
        $arrMedicaoContratos = $arrDados["medicaoContratos"];
        $usucpf = $_SESSION["usucpf"];

        if ($_POST["arqidOld"] && ctype_digit($_POST["arqidOld"])) {
            if (!empty($_FILES["arquivoMedicao"]["name"])) {
                // O Boletim de Medição deve ser um arquivo no formato PDF.
                $arrMimeType = array(
                    "application/pdf",
                    "application/x-pdf",
                    "application/acrobat",
                    "applications/vnd.pdf",
                    "text/pdf",
                    "text/x-pdf",
                    "application/save",
                    "application/force-download",
                    "application/download",
                    "application/x-download"
                );
                if (in_array($_FILES["arquivoMedicao"]["type"], $arrMimeType)) {
                    require_once(APPRAIZ . "includes/classes/fileSimec.class.inc");
                    $fileSimec = new FilesSimec();
                    $fileSimec->setUpload($descricaoArquivo, null, false);
                    $arqid = $fileSimec->getIdArquivo();

                    if (!$arqid) {
                        echo "<script>
                            alert('O arquivo do Boletim de Medição não foi salvo. \\nTente novamente mais tarde.');
                            window.close();
                          </script>";
                    } else {
                        $fileSimec->excluiArquivoFisico($_POST["arqidOld"]);
                    }
                } else {
                    echo "<script>
                        alert('O Boletim de Medição deve ser um arquivo no formato PDF.');
                        window.close();
                      </script>";
                }
            } else {
                echo "<script>
                        alert('O Boletim de Medição não foi enviado.');
                        window.close();
                      </script>";
            }
        }

        /* Evitando que o usuário troque o id do contrato do fornecedor ao editar uma medição.
         * Aqui é verificado se o ID do contrato que o usuário enviou é o mesmo que já está salvo na medição. Se não for,
         * emite o alerta.
         */
        $idContratoFornecedorBanco = verificaContratoExisteNaMedicao($medid);
        if (ctype_digit($idContratoFornecedorBanco)) {
            if ($idContratoFornecedorBanco != $idContratoEmpresaContratada) {
                echo "<script>
                        alert('Não é possível modificar o contrato da medição. Você pode excluí-la e cadastrar novamente.');
                        window.close();
                      </script>";
            }
        }

        global $db;

        $sql1 = "
        UPDATE obras2.medicoes
        SET ";
        if ($arqid && ctype_digit($arqid)) {
            $sql1 .= "arqid = {$arqid},";
        }
        $sql1 .= "
          {$colunaIdContratoNovo} = {$idContratoEmpresaContratada},
          {$colunaIdContratoVelha} = NULL,
          mednummedicao = {$numeroMedicao},
          meddtmedicao = '{$dataMedicao}',
          meddtinicioexec = '{$periodoExec1}',
          meddtfimexec = '{$periodoExec2}',
          medvlrmedicao = {$valorMedicao}
        WHERE medid = {$medid}";

        $db->executar($sql1);

        $sql2 = "
        SELECT
          mec.crtid,
          mec.cceid,
          mec.mecvlrreferencia
        FROM obras2.medicaocontrato mec
        WHERE
          mec.mecstatus = 'A'
          AND
          mec.medid = {$medid};
        ";

        $arrDadosMedicaoContratos = $db->carregar($sql2);

        // Array com IDs dos contratos vindos da tela.
        $arrIdContratosTela = array();
        if (is_array($arrMedicaoContratos) && !empty($arrMedicaoContratos)) {
            foreach ($arrMedicaoContratos as $medicaoContrato) {
                $arrIdContratoAditivo = explode("_", $medicaoContrato->idContratoAditivo);
                $arrIdContratosTela[] = $arrIdContratoAditivo[1];
            }
        }

        // Array com IDs dos contratos vindos do banco.
        $arrIdContratosBanco = array();
        if (is_array($arrDadosMedicaoContratos) && !empty($arrDadosMedicaoContratos)) {
            $colunaMedicaoContratos = $tipoEmpresaContratada === "cex" ? "cceid" : "crtid";
            foreach ($arrDadosMedicaoContratos as $dadosMedicaoContrato) {
                $arrIdContratosBanco[] = $dadosMedicaoContrato[$colunaMedicaoContratos];
            }
        }

        $arrValoresTela = array();

        $sql3 = "";

        if (is_array($arrMedicaoContratos) && !empty($arrMedicaoContratos)) {
            $colunaMedicaoContratos = $tipoEmpresaContratada === "cex" ? "cceid" : "crtid";

            foreach ($arrMedicaoContratos as $medicaoContrato) {
                $valorContratoAditivo = $medicaoContrato->valorContratoAditivo;
                $valorContratoAditivo = str_replace(".", "", $valorContratoAditivo);
                $valorContratoAditivo = str_replace(",", ".", $valorContratoAditivo);
                $valorContratoAditivo = $valorContratoAditivo;

                $arrIdContratoAditivo = explode("_", $medicaoContrato->idContratoAditivo);
                $idContratoAditivo = $arrIdContratoAditivo[1];

                $arrValoresTela[$idContratoAditivo] = $valorContratoAditivo;

                if (is_array($arrDadosMedicaoContratos) && !empty($arrDadosMedicaoContratos)) {
                    foreach ($arrDadosMedicaoContratos as $dadosMedicaoContrato) {
                        if ($dadosMedicaoContrato[$colunaMedicaoContratos] == $idContratoAditivo) {
                            if ($dadosMedicaoContrato["mecvlrreferencia"] == $valorContratoAditivo) {
                                continue;
                            } else {
                                $sql3 .= "UPDATE obras2.medicaocontrato SET mecvlrreferencia = {$valorContratoAditivo} WHERE medid = {$medid} AND {$colunaMedicaoContratos} = {$idContratoAditivo};\n";
                            }
                        }
                    }
                }
            }

            // Contratos que têm no banco, mas não em tela. (INATIVAÇÃO)
            $arrDiff1 = array_diff($arrIdContratosBanco, $arrIdContratosTela);
            if (is_array($arrDiff1) && !empty($arrDiff1)) {
                foreach ($arrDiff1 as $diff) {
                    $sql3 .= "UPDATE obras2.medicaocontrato SET mecstatus = 'I', usucpfinativacao = '{$usucpf}', mecdtinativacao = 'now()' WHERE medid = {$medid} AND {$colunaMedicaoContratos} = {$diff};\n";
                }
            }

            // Contratos que têm na tela, mas não em banco. (NOVO REGISTRO)
            $arrDiff2 = array_diff($arrIdContratosTela, $arrIdContratosBanco);
            if (is_array($arrDiff2) && !empty($arrDiff2)) {
                foreach ($arrDiff2 as $diff) {
                    $vlr = $arrValoresTela[$diff];
                    $sql3 .= "INSERT INTO obras2.medicaocontrato (medid, {$colunaMedicaoContratos}, mecvlrreferencia, usucpfcriacao, mecdtcriacao) VALUES ({$medid}, {$diff}, {$vlr}, '{$usucpf}', 'now()');\n";
                }
            }

            if (!empty($sql3)) {
                $db->executar($sql3);
            }

            if ($db->commit()) {
                echo "<script>
                        alert('Informações gravadas com sucesso.');
                        window.opener.location.reload();
                      </script>";
                exit;
            }
        }

        echo "<script>alert({$msgRetorno})</script>";
        exit;
    }
}

/**
 * Funçnão responsável por verificar se determinado contrato já pertence a uma medição ligada a uma nota fiscal.
 * Esta função foi criada a fim de evitar alteração desnecessária de fornecedor para uma medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid
 * @return bool|mixed|NULL|string
 */
function verificaContratoExisteNaMedicao($medid)
{

    global $db;

    $sql = "
    SELECT
      CASE
        WHEN med.crtid NOTNULL AND med.cceid ISNULL THEN med.crtid
        WHEN med.crtid ISNULL AND med.cceid NOTNULL THEN med.cceid
      END idcontratofornecedor
    FROM obras2.medicoes med
    WHERE med.medstatus = 'A'
          AND med.medid = {$medid};
    ";

    return $db->pegaUm($sql);
}

/**
 * Função responsável por pesquisar os dados da medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid
 * @param $medid
 * @return array|mixed|NULL
 */
function getDadosMedicao($obrid, $medid)
{

    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    $sql = "
        SELECT
          med.obrid,
          med.medid,
          CASE WHEN (med.crtid IS NOT NULL) AND (med.cceid IS NULL)
            THEN 'ent'
          WHEN (med.crtid IS NULL) AND (med.cceid IS NOT NULL)
            THEN 'cex'
          ELSE NULL END                              AS medtipoempresacontratada,
          CASE WHEN (med.crtid IS NOT NULL) AND (med.cceid IS NULL)
            THEN med.crtid
          WHEN (med.crtid IS NULL) AND (med.cceid IS NOT NULL)
            THEN med.cceid
          ELSE NULL END                              AS medidcontratoempresacontratada,
          med.obrid,
          med.arqid,
          med.mednummedicao,
          to_char(med.meddtmedicao, 'DD/MM/YYYY')    AS meddtmedicao,
          to_char(med.meddtinicioexec, 'DD/MM/YYYY') AS meddtinicioexec,
          to_char(med.meddtfimexec, 'DD/MM/YYYY')    AS meddtfimexec,
          med.medvlrmedicao,
          CASE
          WHEN med.crtid NOTNULL AND med.cceid ISNULL THEN (
            SELECT crt.entidempresa FROM obras2.contrato crt WHERE crt.crtstatus = 'A' AND crt.crtid = med.crtid
          )
          WHEN med.crtid ISNULL AND med.cceid NOTNULL THEN (
            SELECT cce.cexid FROM obras2.contratoconstrutoraextra cce WHERE cce.ccestatus = 'A' AND cce.cceid = med.cceid
          ) END medidempresacontratada
        FROM obras2.medicoes med
        WHERE
          med.medstatus = 'A'
          AND med.medid = {$medid}
          AND med.obrid IN ({$strObrids});
    ";

    $arrMedicao = $db->carregar($sql);

    $retorno = !$arrMedicao ? array() : $arrMedicao;

    return $retorno;
}

/**
 * Função responsável por pesquisar os dados dos contratos/aditivos de determinada medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid
 * @return array|mixed|NULL
 */
function getDadosMedicaoContrato($medid)
{

    global $db;

    $sql = "
        SELECT
          mec.medid,
          CASE WHEN (mec.crtid IS NOT NULL) AND (mec.cceid IS NULL)
            THEN 'crt_' || mec.crtid
          WHEN (mec.crtid IS NULL) AND (mec.cceid IS NOT NULL)
            THEN 'cce_' || mec.cceid
          ELSE NULL
          END AS idcontrato,
        
          CASE WHEN (mec.crtid IS NOT NULL) AND (mec.cceid IS NULL)
            THEN (
              SELECT CASE WHEN c.ttaid IS NULL
                THEN 'Contrato Original - ' || to_char(c.crtdtassinatura, 'DD/MM/YYYY')
                     ELSE
                       'Aditivo de Valor - ' || to_char(c.crtdtassinaturaaditivo, 'DD/MM/YYYY')
                     END
              FROM obras2.contrato c
              WHERE c.crtid = mec.crtid
            )
          WHEN (mec.crtid IS NULL) AND (mec.cceid IS NOT NULL)
            THEN (
              SELECT CASE WHEN cce.ttaid IS NULL
                THEN 'Contrato Original - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
                     ELSE
                       'Aditivo de Valor - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
                     END
              FROM obras2.contratoconstrutoraextra cce
              WHERE cce.cceid = mec.cceid
            )
          ELSE NULL
          END AS contrato,
          mec.mecvlrreferencia
        FROM
          obras2.medicaocontrato mec
        WHERE
          mec.mecstatus = 'A'
          AND
          mec.medid = {$medid};
    ";

    $arrDadosContratoMedicao = $db->carregar($sql);

    if (is_array($arrDadosContratoMedicao) && !empty($arrDadosContratoMedicao)) {
        return $arrDadosContratoMedicao;
    } else {
        return array();
    }
}

?>

<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/obras2.js?v=1"></script>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

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

    .detalheVisualizarPagamento {
        font-size: 14px;
    }

</style>

<form id="formulario" method="post" enctype="multipart/form-data" action="#">
    <input type="hidden" name="medid" id="medid" value="<?php echo $medid; ?>"/>
    <!-- FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <?php
            $tituloTela = $medid ? "Editar Medição" : "Incluir Medição";
            ?>
            <td class="SubTitulocentro" colspan="2"><?php echo $tituloTela; ?></td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Empresa Contradada:
                        </td>
                        <td>
                            <?php
                            if ($medid) {
                                /*ver($arrDadosMedicao);*/
                                $tipoEmpresaContratada = $arrDadosMedicao["medtipoempresacontratada"];
                                $idEmpresaContratada = $arrDadosMedicao["medidempresacontratada"];
                                $idContratoEmpresaContratada = $arrDadosMedicao["medidcontratoempresacontratada"];
                                $selectedValue = $tipoEmpresaContratada . "_" . $idEmpresaContratada . "_" . $idContratoEmpresaContratada;

                                if (!$tipoEmpresaContratada) {
                                    echo "<input type='hidden' name='medicaoSemEmpresa' id='medicaoSemEmpresa' value='true' />";
                                    echo montaComboEmpresaContratada($obrid);
                                } else {
                                    echo montaComboEmpresaContratada($obrid, $selectedValue);
                                }
                            } else {
                                echo montaComboEmpresaContratada($obrid);
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Boletim de Medição:
                        </td>
                        <td>
                            <?php
                            if ($medid) {
                                if (is_array($arquivoMedicao) && !empty($arquivoMedicao)) {
                                    ?>
                                    <div class="divArquivoDownload">
                                        <a class="link"
                                           onclick="excluirArquivo('<?= $arrDadosMedicao['arqid'] ?>');">
                                            <img src="/imagens/excluir.gif"
                                                 title="Excluir arquivo <?= $arquivoMedicao['arqnome'] ?>"/>
                                        </a>
                                        &nbsp;
                                        <a class="link"
                                           onclick="downloadArquivo('<?= $arrDadosMedicao['arqid'] ?>');">
                                            <img src="/imagens/salvar.png"
                                                 title="Fazer download do arquivo."/>
                                            <?= $arquivoMedicao['arqnome'] . '.' . $arquivoMedicao['arqextensao'] ?>
                                        </a>
                                    </div>
                                    <div class="divArquivoUpload" style="display: none;">
                                        <input type="hidden" name="arqidOld" id="arqidOld" value=""/>
                                        <input type="file" name="arquivoMedicao" id="arquivoMedicao"/>
                                        <img border="0" title="Indica campo obrigatório."
                                             src="../imagens/obrig.gif"/><br/>
                                        <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                    </div>
                                    <?php
                                } else {
                                    ?>
                                    <div class="divArquivoUpload">
                                        <input type="hidden" name="arqidOld" id="arqidOld" value="001"/>
                                        <input type="file" name="arquivoMedicao" id="arquivoMedicao"/>
                                        <img border="0" title="Indica campo obrigatório."
                                             src="../imagens/obrig.gif"/><br/>
                                        <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                    </div>
                                    <?php
                                }
                            } else {
                                ?>
                                <div class="divArquivoUpload">
                                    <input type="file" name="arquivoMedicao" id="arquivoMedicao"/>
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
                            Número da Medição:
                        </td>
                        <td>
                            <?php
                            $numeroMedicao = $medid ? $arrDadosMedicao["mednummedicao"] : null;
                            ?>
                            <input type="text" style="text-align:left;" name="numeroMedicao"
                                   id="numeroMedicao" size="15" maxlength="10" value="<?php echo $numeroMedicao; ?>"
                                   onKeyUp="this.value=mascaraglobal('##########',this.value);"
                                   onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                   onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                                   title="Número da Medição." class="obrigatorio normal"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Data da Medição:
                        </td>
                        <td>
                            <?php
                            $dataMedicao = $medid ? $arrDadosMedicao["meddtmedicao"] : null;
                            echo campo_data2("dataMedicao", "N", "S", "Data da Medição.", "", null, null, $dataMedicao);
                            ?>
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Período de Execução:
                        </td>
                        <td>
                            <?php
                            $periodoExec1 = $medid ? $arrDadosMedicao["meddtinicioexec"] : null;
                            $periodoExec2 = $medid ? $arrDadosMedicao["meddtfimexec"] : null;
                            echo campo_data2(
                                "periodoExec1",
                                "N",
                                "S",
                                "Período de Execuçã..",
                                "",
                                null,
                                null,
                                $periodoExec1
                            );
                            ?>
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                            &nbsp;&nbsp;à&nbsp;&nbsp;
                            <?php
                            echo campo_data2(
                                "periodoExec2",
                                "N",
                                "S",
                                "Período de Execução.",
                                "",
                                null,
                                null,
                                $periodoExec2
                            );
                            ?>
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>

                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor da Medição:
                        </td>
                        <td>
                            <?php
                            $valorMedicao = $medid ? number_format(
                                $arrDadosMedicao["medvlrmedicao"],
                                2,
                                ",",
                                "."
                            ) : null;
                            ?>
                            <input
                                    type="text"
                                    style="text-align:left;"
                                    name="valorMedicao"
                                    id="valorMedicao"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);this.select();"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                    maxlength="18"
                                    size="17"
                                    class="obrigatorio normal"
                                    value="<?php echo $valorMedicao; ?>"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES. -->

    <!-- FORMULÁRIO DE CONTRATO/ADITIVO DE VALOR. -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Contrato ou Aditivo de Valor</td>
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
                            if ($medid) {
                                echo montaComboContratosAditivos($medid);
                            } else {
                                ?>
                                <select name="contratos" id="contratos">
                                    <option value="">Selecione...</option>
                                </select>
                                <?php
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor Referente ao Contrato ou Aditivo de Valor:
                        </td>
                        <td>
                            <input
                                    type="text"
                                    style="text-align:left;"
                                    name="valorContratoAditivo"
                                    id="valorContratoAditivo"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);this.select();"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                    maxlength="18"
                                    size="17"
                                    class="obrigatorio normal"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: center">
                            <br/>
                            <input type="button" value="Adicionar Medição" id="botaoAtualizarLista"
                                   onclick="atualizarListaContratos()"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE CONTRATO/ADITIVO DE VALOR. -->

    <br/>

    <!-- INÍCIO DA TABELA DE ACOMPANHAMENTO. -->
    <?php
    $display = $medid ? "block" : "none";
    ?>

    <div id="acompanhamento" style="display: <?php echo $display; ?>">

        <table width="95%" align="center" border="1" style="border-collapse: collapse" id="tbAcompanhamento">
            <thead>
            <tr>
                <td class="tbAcompanhamentoTitulo">Ações</td>
                <td class="tbAcompanhamentoTitulo">Contrato ou Aditivo de Valor</td>
                <td class="tbAcompanhamentoTitulo">Valor Referente ao Contrato ou Aditivo de Valor</td>
            </tr>
            </thead>
            <?php if ($medid) {
                ?>
                <tbody>
                <?php
                $arrDadosMedicaoContrato = getDadosMedicaoContrato($medid);
                $arrM = array();
                foreach ($arrDadosMedicaoContrato as $k => $dadosMedicaoContrato) {
                    $idContratoAditivo = $dadosMedicaoContrato["idcontrato"];
                    $nomeContratoAditivo = $dadosMedicaoContrato["contrato"];
                    $valorContratoAditivo = $dadosMedicaoContrato["mecvlrreferencia"];
                    $valorContratoAditivo = number_format($valorContratoAditivo, 2, ",", ".");

                    $arrTmp = array(
                        "id" => $k,
                        "idEmpresaContratada" => $idEmpresaContratada,
                        "numeroMedicao" => $numeroMedicao,
                        "idContratoAditivo" => $idContratoAditivo,
                        "valorContratoAditivo" => $valorContratoAditivo
                    );

                    array_push($arrM, $arrTmp);

                    echo "<tr>";
                    echo "<td><img src='/imagens/excluir.gif' alt='Excluir' title='Excluir' onclick='removerMedicaoTabelaAcompanhamento(this, {$k}, \"{$idContratoAditivo}\", \"{$nomeContratoAditivo}\")' style='cursor:pointer;' />";
                    echo "&nbsp;&nbsp;<img src='/imagens/alterar.gif' alt='Editar' title='Editar' onclick='editarMedicaoTabelaAcompanhamento(this, {$k}, \"{$idContratoAditivo}\", \"{$nomeContratoAditivo}\", \"{$valorContratoAditivo}\")' style='cursor:pointer;' /></td>";
                    echo "<td>{$nomeContratoAditivo}</td>";
                    echo "<td>{$valorContratoAditivo}</td>";
                    echo "<tr>";
                }

                $arrM = json_encode($arrM);
                echo "<input type='hidden' name='dadosMedicaoContrato' id='dadosMedicaoContrato' value='{$arrM}' />";

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

    <input type="hidden" id="requisicao" name="requisicao" value="salvarMedicaoContratosAditivos"/>
    <input type="hidden" id="medicaoContratos" name="medicaoContratos"/>

</form>

<script type="text/javascript">

    var listaObjetoMedicao = [];
    var idControleObjDocumento = 0;
    var listaEmpresasRemovidas = [];
    var acaoEditar = false;

    //    window.onunload = refreshParent;

    $(document).ready(function () {

        $("#formulario").submit(function () {
            var isValid = validarSubmit();
            if (isValid !== true) {
                alert(isValid);
                return false;
            }
            // É necessário habilitar o campo "Empresa Contratada" no momento que o formulário for submetido.
            $("#empresaContratada").attr("disabled", false);
        });

        acaoEditar = $("#medid").val() != "" ? true : false;
        if (acaoEditar) {
            listaObjetoMedicao = JSON.parse($("#dadosMedicaoContrato").val());
            $("#medicaoContratos").val(JSON.stringify(listaObjetoMedicao));
            $("#dadosMedicaoContrato").remove();

            var medicaoSemEmpresa = $("#medicaoSemEmpresa").val();
            if (!medicaoSemEmpresa) {
                $("#empresaContratada").attr("disabled", true);
                $("#medicaoSemEmpresa").remove();
            }

            idControleObjDocumento = listaObjetoMedicao.length;
        }
    });

    function validarSubmit() {

        var arquivoMedicao = $("#arquivoMedicao").get(0).files;
        var nomeArquivoMedicao = arquivoMedicao.length > 0 ? arquivoMedicao[0].name : "";
        var arqiOld = $("#arqidOld").val();

        var valorMedicao = $("#valorMedicao").val();
        valorMedicao = valorMedicao.replace(/[.]/g, "");
        valorMedicao = valorMedicao.replace(/[,]/g, ".");
        valorMedicao = parseFloat(valorMedicao);

        var valorContratoAditivo = $("#valorContratoAditivo").val();
        valorContratoAditivo = valorContratoAditivo.replace(/[.]/g, "");
        valorContratoAditivo = valorContratoAditivo.replace(/[,]/g, ".");
        valorContratoAditivo = parseFloat(valorContratoAditivo);

        var periodoExec1 = $("#periodoExec1").val();
        var periodoExec2 = $("#periodoExec2").val();
        var diferencaPeriodoExecucao = calcularDiferencaEntreDatas(periodoExec1, periodoExec2);

        if ($("#empresaContratada").val() == "") {
            return "Informe a Empresa Contratada.";
        } else if (arqiOld != "" && acaoEditar) {
            if (arquivoMedicao.length == 0) {
                return "É necessário anexar o Boletim de Medição.";
            } else if (nomeArquivoMedicao.substr(nomeArquivoMedicao.lastIndexOf('.')).toLowerCase() !== ".pdf") {
                return "O Boletim de Medição deve ser um arquivo no formato .PDF";
            }
        } else if ($("#numeroMedicao").val() == "") {
            return "Informe o Número da Medição.";
        } else if ($("#dataMedicao").val() == "") {
            return "Informe a Data da Medição.";
        } else if (periodoExec1 == "" || periodoExec2 == "") {
            return "Informe o Período da Medição";
        } else if (diferencaPeriodoExecucao < 0) {
            return "O Período de Execução está irregular.";
        } else if ((valorMedicao <= 0) || (isNaN(valorMedicao))) {
            return "O Valor da Medição deve ser um número maior que zero.";
        } else if (listaObjetoMedicao.length < 1) {
            return "A medição precisa ser vinculada a pelos menos um contrato.";
        }

        var somatorio = 0;
        var somatorioMedicao = function (somatorio) {
            $.each(listaObjetoMedicao, function (k, v) {

                var vlrMedicaoContrato = v.valorContratoAditivo;
                vlrMedicaoContrato = vlrMedicaoContrato.replace(/[.]/g, '');
                vlrMedicaoContrato = vlrMedicaoContrato.replace(/[,]/g, '.');
                vlrMedicaoContrato = parseFloat(vlrMedicaoContrato);

                somatorio += vlrMedicaoContrato;

            });
            //utilizado para formatar o retorno da função que não limitava casas decimaislistaObrasSolicitacao
            somatorio = parseFloat(somatorio.toFixed(2));
            return somatorio;
        };

        if (somatorioMedicao(somatorio) != valorMedicao) {
            return "O somatório dos valores das medições deve ser igual ao valor total da medição.";
        }

        var arquivo = $("#arquivoMedicao").val();
        var extensao = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
        if ((arqiOld != "" && acaoEditar) || (!acaoEditar)) {
            if (extensao != ".pdf") {
                return "O Boletim de Medição deve ser um arquvo no formato PDF.";
            }
        }
        return true;
    }

    /**
     * Função responsável por atualizar a página com a lista de itens na tela de notas fiscais. Esta atualização é feita
     * logo após o cadastro de uma nota fiscal.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     */
    function refreshParent() {
        window.opener.location.reload();
    }

    /**
     * Função responsável por fazer a requisição para a montagem da combo 'Contrato ou Aditivo de Valor'.
     * @param entid Id da 'entidade' ou da 'construtora extra'.
     * @returns {boolean}
     */
    function montaComboContratos(entid) {

        $("#contratos").empty().append("<option value=''>Selecione...</option>");

        if (entid.length <= 0) {
            return false;
        }

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            dataType: "json",
            data: {
                "requisicao": "carregarContratos",
                "entid": entid
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

        $("#acompanhamento").show();

        var idEmpresaContratada = $("#empresaContratada").val();
        var nomeEmpresaContratada = $("#empresaContratada option:selected").text();
        var numeroMedicao = $("#numeroMedicao").val();
        var dataMedicao = $("#dataMedicao").val();
        var periodoExec1 = $("#periodoExec1").val();
        var periodoExec2 = $("#periodoExec2").val();
        var idContratoAditivo = $("#contratos option:selected").val();
        var nomeContratoAditivo = $("#contratos option:selected").text();
        var valorMedicao = $("#valorMedicao").val();
        valorMedicao = valorMedicao.replace(/[.]/g, "");
        valorMedicao = valorMedicao.replace(/[,]/g, ".");
        var valorContratoAditivo = $("#valorContratoAditivo").val();
        /*valorContratoAditivo = valorContratoAditivo.replace(/[.]/g, "");
        valorContratoAditivo = valorContratoAditivo.replace(/[,]/g, ".");*/

        var objMedicao = {
            "id": idControleObjDocumento, // Este ID não é do banco de dados. Serve apenas para controle no javascript.
            "idEmpresaContratada": idEmpresaContratada,
            //"nomeEmpresaContratada": nomeEmpresaContratada,
            "numeroMedicao": numeroMedicao,
            //"dataMedicao": dataMedicao,
            //"periodoExec1": periodoExec1,
            //"periodoExec2": periodoExec2,
            //"valorMedicao": valorMedicao,
            "idContratoAditivo": idContratoAditivo,
            //"nomeContratoAditivo": nomeContratoAditivo,
            "valorContratoAditivo": valorContratoAditivo
        };

        /* Validação: Um contrato/aditivo não pode ter mais de uma medição. */
        $("#contratos option[value='" + idContratoAditivo + "']").remove();

        /* Validação: Uma mesma medição não pode ser gravada para mais de uma empresa. */
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

        listaObjetoMedicao.push(objMedicao);
        idControleObjDocumento++;

        /* Na tabela de acompanhamento, o "Valor Referente ao Contrato ou Aditivo de Valor" é exibido com máscara. */
        $("#tbAcompanhamento > tbody:last-child").append(
            "<tr>"
            + "<td><img src='/imagens/excluir.gif' alt='Excluir' title='Excluir' onclick='removerMedicaoTabelaAcompanhamento(this, " + objMedicao.id + ", \"" + objMedicao.idContratoAditivo + "\", \"" + nomeContratoAditivo + "\")' style='cursor:pointer;' /></td>"
            + "<td>" + nomeContratoAditivo + "</td>"
            + "<td>" + $("#valorContratoAditivo").val() + "</td>"
            + "</tr>"
        );

        $("#medicaoContratos").val(JSON.stringify(listaObjetoMedicao));

        //limpa input valor contrato/aditivo
        $("#valorContratoAditivo").val('');
    }

    /**
     * Função responsável por remover da tabela de acompanhamento determinada medição.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param elemento
     * @param indexObjDocumento
     */
    function removerMedicaoTabelaAcompanhamento(elemento, indexObjDocumento, idContratoAditivo, nomeContratoAditivo) {

        $(elemento).closest("tr").remove();

        listaObjetoMedicao = listaObjetoMedicao.filter(function (e) {
            return e.id != indexObjDocumento;
        });

        if (listaObjetoMedicao.length <= 0) {
            $("#acompanhamento").hide();
            $("#medicaoContratos").val("");
        }

        /* Validação: Um contrato/aditivo não pode ter mais de uma medição. */
        $('#contratos').append($('<option>', {
            value: idContratoAditivo,
            text: nomeContratoAditivo
        }));


        /* Validação: Uma mesma medição não pode ser utilizada em empresas diferentes. */
        for (var i = 0; i < listaEmpresasRemovidas.length; i++) {
            var empresaRemovida = listaEmpresasRemovidas[i];
            $('#empresaContratada').append($('<option>', {
                value: empresaRemovida.id,
                text: empresaRemovida.text
            }));
        }

        listaEmpresasRemovidas = [];

        $("#empresaContratada").attr("disabled", false);

        $("#medicaoContratos").val(JSON.stringify(listaObjetoMedicao));
    }

    /**
     * Função responsável por validar os campos do formulário para inclusão das medições.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {*}
     */
    function validarDadosFormulario() {

        var arquivoMedicao = $("#arquivoMedicao").get(0).files;
        var nomeArquivoMedicao = arquivoMedicao.length > 0 ? arquivoMedicao[0].name : "";
        var arqiOld = $("#arqidOld").val();

        var valorMedicao = $("#valorMedicao").val();
        valorMedicao = valorMedicao.replace(/[.]/g, "");
        valorMedicao = valorMedicao.replace(/[,]/g, ".");
        valorMedicao = parseFloat(valorMedicao);

        var valorContratoAditivo = $("#valorContratoAditivo").val();
        valorContratoAditivo = valorContratoAditivo.replace(/[.]/g, "");
        valorContratoAditivo = valorContratoAditivo.replace(/[,]/g, ".");
        valorContratoAditivo = parseFloat(valorContratoAditivo);

        var periodoExec1 = $("#periodoExec1").val();
        var periodoExec2 = $("#periodoExec2").val();
        var diferencaPeriodoExecucao = calcularDiferencaEntreDatas(periodoExec1, periodoExec2);

        if ($("#empresaContratada").val() == "") {
            return "Informe a Empresa Contratada.";
        } else if (arqiOld != "" && acaoEditar) {
            if (arquivoMedicao.length == 0) {
                return "É necessário anexar o Boletim de Medição.";
            } else if (nomeArquivoMedicao.substr(nomeArquivoMedicao.lastIndexOf('.')).toLowerCase() !== ".pdf") {
                return "O Boletim de Medição deve ser um arquivo no formato .PDF";
            }
        } else if ($("#numeroMedicao").val() == "") {
            return "Informe o Número da Medição.";
        } else if ($("#dataMedicao").val() == "") {
            return "Informe a Data da Medição.";
        } else if (periodoExec1 == "" || periodoExec2 == "") {
            return "Informe o Período da Medição"
        } else if (diferencaPeriodoExecucao < 0) {
            return "O Período de Execução está irregular.";
        } else if ((valorMedicao <= 0) || (isNaN(valorMedicao))) {
            return "O Valor da Medição deve ser um número maior que zero.";
        } else if ($("#contratos").val() == "") {
            return "Informe o Contrato ou Aditivo de Valor";
        } else if ((valorContratoAditivo <= 0) || (isNaN(valorContratoAditivo))) {
            return "O Valor Referente ao Contrato ou Aditivo de Valor deve ser um número maior que zero.";
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
        if (confirm('Confirma a exclusão do arquivo?')) {
            $("#arqidOld").val(arqid);
            $(".divArquivoDownload").hide("fast");
            $(".divArquivoUpload").show("fast");
        }
    }

    /**
     * Função responsável pelo download do arquivo da medição.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param arqid
     */
    function downloadArquivo(arqid) {
        window.location = window.location.href + "&requisicao=download&arqid=" + arqid;
    }

    function editarMedicaoTabelaAcompanhamento(elemento, indexObjDocumento, idContratoAditivo, nomeContratoAditivo, valorContratoAditivo) {

        $(elemento).closest("tr").remove();

        listaObjetoMedicao = listaObjetoMedicao.filter(function (e) {
            return e.id != indexObjDocumento;
        });

        if (listaObjetoMedicao.length <= 0) {
            $("#acompanhamento").hide();
            $("#medicaoContratos").val("");
        }

        /* Validação: Um contrato/aditivo não pode ter mais de uma medição. */
        $('#contratos').append($('<option>', {
            value: idContratoAditivo,
            text: nomeContratoAditivo
        }));


        $("#contratos>[value=" + idContratoAditivo + "]").attr("selected", true);
        $("#valorContratoAditivo").val(valorContratoAditivo);


        /* Validação: Uma mesma medição não pode ser utilizada em empresas diferentes. */
        for (var i = 0; i < listaEmpresasRemovidas.length; i++) {
            var empresaRemovida = listaEmpresasRemovidas[i];
            $('#empresaContratada').append($('<option>', {
                value: empresaRemovida.id,
                text: empresaRemovida.text
            }));
        }

        listaEmpresasRemovidas = [];

        $("#empresaContratada").attr("disabled", false);

        $("#medicaoContratos").val(JSON.stringify(listaObjetoMedicao));
    }

</script>

</body>
