<?php

/* Arquivo responsável por utilizar o webservice de consulta de Pessoas Jurídicas à base da Receita Federal. */
require APPRAIZ . "www/includes/webservice/pj.php";

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

/*$execucaoFinanceira = new ExecucaoFinanceira();
var_dump($execucaoFinanceira->retornaObrids(1006315));
exit;*/

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
if ((isset($_GET["ntfid"]) && ctype_digit($_GET["ntfid"])) )
{
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
else
{
	$errorMessage = "Faltam dados!";
	echo "<script>alert('{$errorMessage}');window.close();</script>";
	exit;

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

/**
 * Função responsável por carregar uma lista com os contratos/aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid Id da obra.
 * @param $medid Id da medição.
 * @param $entid Id da empresa quando a origem da obra for 'Entidade'.
 * @param $cexid Id da empresa quando a origem da obra for 'Construtuora extra (Cumprimento do Objeto)'.
 * @return array|mixed|NULL
 */
function carregarContratos($obrid, $crtid, $cceid, $medid=null) {

    global $db;

    $arrContratos = array();

    if (!$crtid && !$cceid)
    {
        return $arrContratos;
    }

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    if (ctype_digit($crtid) && !$cceid)
    {
        $sql = "
				SELECT
					'crt_' || crt.crtid as codigo,
					CASE WHEN ttaid IS NULL THEN
						'Contrato Original' || ' - ' || to_char(crt.crtdtassinatura, 'DD/MM/YYYY')
					ELSE
						'Aditivo de Valor' || ' - ' || to_char(crt.crtdtassinatura, 'DD/MM/YYYY')
					END as descricao
				FROM 
					obras2.contrato crt
				WHERE 
					(
						crt.crtid  = {$crtid}
						or
						(crt.crtidpai = {$crtid} AND ttaid in (2,3))
					)
            ";

        if ($medid) { //@todo
            $sql .= "AND c.crtid NOT IN (SELECT mec.crtid
			FROM obras2.medicaocontrato mec
			WHERE mec.mecstatus = 'A' AND mec.medid = {$medid})";
        }
    }
    elseif (ctype_digit($cceid) && !$crtid)
    {
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

        if ($medid) { //@todo
            $sql .= "
			AND cce.cceid NOT IN (
			SELECT mec.cceid
			FROM obras2.medicaocontrato mec
			WHERE mec.mecstatus = 'A' AND mec.medid = {$medid}
			)"
            ;
        }
    }

    $arrContratos = $sql ? $db->carregar($sql) : false;
    if (!$arrContratos) {
        $arrContratos = array();
    }

    return $arrContratos;
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
			AND ntf.crtid IN
			(
			SELECT c.crtid AS crtid
			FROM obras2.obrascontrato ocr
			INNER JOIN obras2.contrato c ON (ocr.crtid = c.crtid) OR (ocr.crtid = c.crtidpai)
			WHERE ocr.ocrstatus = 'A'
			AND c.crtstatus = 'A'
			AND c.ttaid IS NULL
			AND ocr.obrid IN ({$strObrids})
			)
			AND ntf.ntfdtnota < '{$dataNotaFiscal}';
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
					AND ntf.ntfdtnota < '{$dataNotaFiscal}';
			";
		}

		return $db->pegaUm($sql);
	}
}

function getSomatorioTodosContratos($tipoFornecedor, $arrObras) {

	if (is_array($arrObras) && !empty($arrObras)) {

		global $db;

		$strObrids = implode(",", $arrObras);

		if ($tipoFornecedor === "ent") {
			$sql =  "
			SELECT
			coalesce(sum(ocr.ocrvalorexecucao), 0) totalvalorcontratos
			FROM
			obras2.obrascontrato ocr
			INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
			WHERE
			crt.crtstatus = 'A'
			AND crt.ttaid ISNULL
			AND ocr.obrid IN ({$strObrids});
			";

			return $db->pegaUm($sql);

		} elseif ($tipoFornecedor === "cex") {
		$sql = "
			SELECT
			coelesce(sum(cce.ccevalor), 0) totalvalorcontratos
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
 * Função responsável por carregar uma lista com as medições e suas respectivas referências.
 * Função responsável por carregar o elemento select com os contratos ou aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15250
 * @param $crtid Id do contrato de uma entidade.
 * @param $cceid Id do contrato da construtora extra'.
 * @return array|mixed|NULL
 */
function carregarMedicao($crtid, $cceid) {

    global $db;

    $arrContratos = array();

    if (!$crtid && !$cceid) {
        return $arrContratos;
    }

    if (ctype_digit($crtid) && !$cceid) {
        $filtro = "AND mec.crtid = {$crtid}";
        $filtro2 = "AND mec.crtid = {$crtid}";
    } elseif (ctype_digit($cceid) && !$crtid) {
        $filtro = "AND mec.cceid = {$cceid}";
        $filtro2 = "AND mec.cceid = {$cceid}";
    }

    $sql = "
        WITH totalpagomedicao AS (
        SELECT
          ntm.mecid,
          sum(ntm.ntmvlrpago) AS totalpago
        FROM obras2.notamedicao ntm
        INNER JOIN obras2.medicaocontrato mec ON mec.mecid = ntm.mecid  AND mec.mecstatus = 'A'
        WHERE ntm.ntmstatus = 'A'
              {$filtro2}
        GROUP BY ntm.mecid
        )
        SELECT 
            codigo,
            descricao
        FROM
          (SELECT
             mec.mecid                        AS codigo,
             mednummedicao                    AS descricao,
             cast(med.medvlrmedicao AS FLOAT) AS valortotalmedicao,
             coalesce(tpm.totalpago, 0)       AS valorpagomedicao
           FROM
             obras2.medicaocontrato mec
             INNER JOIN
             obras2.medicoes med ON mec.medid = med.medid AND med.medstatus = 'A'
             LEFT JOIN
             totalpagomedicao tpm ON tpm.mecid = mec.mecid
           WHERE
             mec.mecstatus = 'A'
             AND
             med.medstatus = 'A'
             {$filtro}) x
        WHERE x.valorpagomedicao < x.valortotalmedicao;
    ";

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
                                        window.close();
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

    echo "<script>alert('{$msgRetorno}');window.close();</script>";
    exit;
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
        $tipoContrato = $arrContratos[0];
        $idContrato = $arrContratos[1];
        $numeroNota = $arrDados["numeroNota"];
        $valorNota = $arrDados["valorNota"];
        $dataNota = formata_data_sql($arrDados["dataNota"]);
        $descricaoArquivo = "ntf_" . $numeroNota . "_" . $empresaContratada;
        $usucpf = $_SESSION["usucpf"];
        $arrNotasMedicao = $arrDados["dadosNotaFiscalMedicao"];
        $arrNotasRemovidas = $arrDados["notasRemovidas"];

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
                     window.close();
                  </script>";
        }

        echo "<script>alert({$msgRetorno})</script>";
        exit;
    }
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
          med.arqid as arquivo_medicao
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
				WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
				THEN 
					(select entnumcpfcnpj from entidade.entidade where entid = ntf.entid)
				WHEN 
					ntf.entid ISNULL AND ntf.cexid NOTNULL
                THEN (select cexnumcnpj from obras2.construtoraextra  where cexid = ntf.cexid )
			END AS cnpj,

			CASE WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
			THEN 
				(
					SELECT 
					 substr(entnumcpfcnpj, 1, 2) || '.' || substr(entnumcpfcnpj, 3, 3) || '.' || substr(entnumcpfcnpj, 6, 3) || '/' || substr(entnumcpfcnpj, 9, 4) ||
					  '-' || substr(entnumcpfcnpj, 13) || ' - ' ||ent.entnome ||  ' (' || 'Contrato - ' || 
					 (select to_char(crtdtassinatura, 'DD/MM/YYYY') || ')'
					from obras2.contrato where crtid = ntf.crtid)
					from entidade.entidade ent where entid = ntf.entid
				) 
			WHEN ntf.entid ISNULL AND ntf.cexid NOTNULL
			THEN 
		       (
					SELECT 
					 substr(cexnumcnpj, 1, 2) || '.' || substr(cexnumcnpj, 3, 3) || '.' || substr(cexnumcnpj, 6, 3) || '/' || substr(cexnumcnpj, 9, 4) ||
					  '-' || substr(cexnumcnpj, 13) || ' - ' || cexrazsocialconstrutora  || ' (Contrato - ' || to_char(ccedataassinatura, 'DD/MM/YYYY') || ')'
					 from obras2.construtoraextra cex 
					 inner join obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
					where cce.cceid = ntf.cceid

		       )
			END AS empresa,
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
 * @todo COMENTAR.
 * @param $medid
 * @param $crtid
 * @param $cceid
 * @return bool|string
 */
function buscaValorReferencia($medid, $crtid, $cceid) {

    global $db;

    if (!$crtid && !$cceid) {
        return false;
    }

    if (ctype_digit($crtid) && !$cceid) {
        $sql = "SELECT mecvlrreferencia FROM obras2.medicaocontrato WHERE mecstatus = 'A' AND medid = {$medid} AND crtid = {$crtid}";
    } elseif (ctype_digit($cceid) && !$crtid) {
        $sql = "SELECT mecvlrreferencia FROM obras2.medicaocontrato WHERE mecstatus = 'A' AND medid = {$medid} AND cceid = {$cceid}";
    }

    $vlrReferencia = $db->pegaUm($sql);
    if ($vlrReferencia) {
        return number_format($vlrReferencia, 2, ",", ".");
    } else {
        return false;
    }

}

/**
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
<script type="text/javascript" src="js/obras2.js"></script>
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
            $tituloTela = "Visualizar Nota Fiscal" ;
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
                           		echo $arrDadosNotaFiscal['empresa'];
                           		
                            ?>
                            <input type="hidden" name="cnpj_visualizacao" id="cnpj_visualizacao" value="<?= $arrDadosNotaFiscal['cnpj'] ?>" />
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
                            echo $numeroNotaFiscal ;
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Data da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            $dataNotaFiscal = $acaoEditar ? $arrDadosNotaFiscal["ntfdtnota"] : null;
                            echo $dataNotaFiscal;
                            ?>
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
                            echo $valorNotaFiscal;
                            ?>
                            
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE NOTA FISCAL. -->



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
                <td class="tbAcompanhamentoTitulo">ação</td>
                <td class="tbAcompanhamentoTitulo">Número da Medição</td>
                <td class="tbAcompanhamentoTitulo">Data da Mediçao</td>
                <td class="tbAcompanhamentoTitulo">Período de Execução</td>
                <td class="tbAcompanhamentoTitulo">Valor da Medição</td>
                <td class="tbAcompanhamentoTitulo">Valor Pago na Nota Fiscal</td>
                <td class="tbAcompanhamentoTitulo">% Medição Física na NF</td>
                <td class="tbAcompanhamentoTitulo">% Mediçao Fisica Acumulada</td>
            </tr>
            </thead>
            <?php if ($acaoEditar) {
                ?>
                <tbody>
                <?php
                $arrDadosNotaMedicao = getDadosNotaMedicao($ntfid);
                $arrM = array();

                $execucaoFinanceira = new ExecucaoFinanceira();

                foreach ($arrDadosNotaMedicao as $k=>$dadosNotaMedicao) {

                    //echo "<pre>";var_dump($dadosNotaMedicao);exit;

                    $ntmid = $dadosNotaMedicao["ntmid"];
                    $mecid = $dadosNotaMedicao["mecid"];
                    $dataNotaFiscal = $dadosNotaMedicao["ntfdtnota"];
                    $arquivo_medicao = $dadosNotaMedicao["arquivo_medicao"];
                    $numeroMedicao = $dadosNotaMedicao["mednummedicao"];
                    $dataMedicao = $dadosNotaMedicao["meddtmedicao"];
                    $periodoExec = $dadosNotaMedicao["meddtinicioexec"] . " à " . $dadosNotaMedicao["meddtfimexec"];
                    $valorMedicao = $dadosNotaMedicao["medvlrmedicao"];
                    $valorMedicao = number_format($valorMedicao, 2, ",", ".");
                    $valorPagoNota = $dadosNotaMedicao["ntmvlrpago"];
                    $valorPagoNotaFormatado = number_format($valorPagoNota, 2, ",", ".");

                    $arrIdContratoNotaMedicao = getIdContratoNotaMedicao($mecid);
                    $crtid = $arrIdContratoNotaMedicao["crtid"];
                    $cceid = $arrIdContratoNotaMedicao["cceid"];

                    $tipoEmpresaContratada = $crtid ? "ent" : "cex";

                    if ($tipoEmpresaContratada === "ent") {

                        $valorContratoOriginal = $execucaoFinanceira->getValorContratoAtual($crtid, $cceid);

                        // Calculando o percentual da medição física.
                        $percentualMedicaoFisica = $execucaoFinanceira->calcularPercentualMedicao($valorPagoNota, $valorContratoOriginal);
                        $percentualMedicaoFisica = number_format($percentualMedicaoFisica, 2, ",", ".");
                        $percentualMedicaoFisica = $percentualMedicaoFisica . "%";

                        // Calculando o percentual da medição acumulada.
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
                    } else {
                        $percentualMedicaoFisica = "Não se aplicass.";
                        $percentualMedicaoAcumulada = "Não se aplica.";
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
                  
                    echo "<td><img src=\"../imagens/salvar.png\" title=\"Download Medição\" style=\"cursor:pointer;\" onclick=\"downloadArquivo({$arquivo_medicao})\" width=\"16\"></td>";
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
    var acaoEditar = false;
    var idEmpresaContratada = $("#idEmpresaContratada").val();
    var dadosMedicao = null;

    //window.onunload = refreshParent;

    $('#contratos').change(function(){
        var crt = $('#contratos').val();
        $('#valorMedicao').text("");
        montaComboMedicao(crt);
    });

    $('#empresaContratada').change(function(){
        var opcao = $('#empresaContratada :selected').text();
        var cnpj = opcao.substring(1, 19);
        var cnpjLimpo = cnpj.replace(/\D+/g, "");
        consultaCNPJFornecedor(cnpjLimpo);
    });

    /*$('#medicao').change(function(){
        var medid = $('#medicao').val();
        recuperaDadosMedicao(medid);
    });*/

    $(document).ready(function() {

		var cnpj = $("#cnpj_visualizacao").val();
		consultaCNPJFornecedor(cnpj)

        acaoEditar = $("#ntfid").val() != "" ? true : false;
        if (acaoEditar) {

            listaNotasRemovidas = [];

            // Automatizando a seleção da empresa automaticamente para que seja feita a busca da UF pelo serviço.
            if (idEmpresaContratada) {
                $("#empresaContratada").val(idEmpresaContratada).trigger("change");
            }

            //listaObjetoNota = JSON.parse(JSON.stringify($("#dadosMedicaoNota").val()));
            listaObjetoNota = JSON.parse($("#dadosMedicaoNota").val());

            $("#dadosNotaFiscalMedicao").val(listaObjetoNota);
            $("#dadosMedicaoNota").remove();
            /*$("#empresaContratada").attr("disabled", true);*/
            idControleObjDocumento = listaObjetoNota.length;
        }
    });

    /**
     * @todo DESCOMENTAR.
     * Função responsável por atualizar a página com a lista de itens na tela de notas fiscais. Esta atualização é feita
     * logo após o cadastro de uma nota fiscal.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     */
    function refreshParent() {
        //window.opener.location.reload();
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
                var crt = $('#contratos').val();
                if(crt != ""){
                    montaComboMedicao(crt);
                }
                divCarregado();
            }
        });
    }

    /**
     * Função responsável por fazer a requisição para a montagem da combo 'Boletim de Medição'.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param contrato ID do contrato de origem da entidade ou de construtora extra'.
     * @returns {boolean}
     */
    function montaComboMedicao(contrato) {

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

        if (listaObjetoNota.length <= 0) {
            $("#acompanhamento").hide();
            $("#dadosNotaFiscalMedicao").val("");
        }

        /* Validação: Uma nota fiscal não pode pagar o mesmo contrato/aditivo mais de uma vez . */
        $("#medicao").append($('<option>', {
            value: idMedicaoContrato,
            text: numeroMedicao
        }));

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
        var arqiOld = $("#arqidOld").val();

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
        } else if (arqiOld != "" && acaoEditar) {
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
        } /*else if (valorPagoNota > valorReferencia) {
            return "O valor pago na Nota Fiscal para este contrato não pode ser maior que o valor referenciado na medição.";
        }*/ else {
            return true;
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

        divCarregando();
        setTimeout(function () {
            var pessoaJudirica = new dCNPJ();
            pessoaJudirica.buscarDados(cnpj);
            if (pessoaJudirica.dados.no_empresarial_rf == "") {
                alert("Não há informações sobre o CNPJ informado em nossa base de dados.");
                return false;
            }

            $("#ufFornecedor").text(pessoaJudirica.dados.sg_uf);
        }, 0);
        divCarregado();

        return false;
    }

    /**
     * @todo Comentar depois
     */
    function getDadosMedicao(objSelecionado) {

        mecid = objSelecionado.value;

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

</script>

</body>