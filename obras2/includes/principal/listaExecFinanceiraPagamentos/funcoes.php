<?php 

function uploadArquivosExecucaoOrc()
{
    $arrRetornoIdsArquivos = array();

    if ($_REQUEST['requisicao'] == 'salvar') {
        $arquivo_ordbanc = $_FILES['arquivo_ordbanc'];
        $arquivo_notafiscal = $_FILES['arquivo_notafiscal'];
        $arquivo_boletim = (isset($_FILES['arquivo_boletim']) ? $_FILES['arquivo_boletim'] : array());
    } elseif ($_REQUEST['requisicao'] == 'update') {
        $arquivo_ordbanc = (isset($_FILES['arquivo_ordbanc'])) ? $_FILES['arquivo_ordbanc'] : array();
        $arquivo_notafiscal = (isset($_FILES['arquivo_notafiscal'])) ? $_FILES['arquivo_notafiscal'] : array();
        $arquivo_boletim = (isset($_FILES['arquivo_boletim']) ? $_FILES['arquivo_boletim'] : array());
    }

    // Arquivo da ordem bancária
    if ($arquivo_ordbanc["name"] && $arquivo_ordbanc["type"] && $arquivo_ordbanc["size"] && $arquivo_ordbanc["error"] == 0) {
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $nome_arq = "anexoordembancaria_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file->setUpload($nome_arq, 'arquivo_ordbanc', false, false);
        $arqid = $file->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_ordembancaria'] = $arqid;
    } else {
        $arrRetornoIdsArquivos['peoarqid_ordembancaria'] = 'NULL';
        if ($_REQUEST['requisicao'] == 'salvar') {
            die("<script>
                        alert('O arquivo Comprovante da Transferência é obrigatório!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
    }

    // Arquivo da nota fiscal
    if ($arquivo_notafiscal["name"] && $arquivo_notafiscal["type"] && $arquivo_notafiscal["size"] && $arquivo_notafiscal["error"] == 0) {
        $file2 = new FilesSimec();
        $file2->setPasta('obras2');
        $nome_arq2 = "anexonotafiscal_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file2->setUpload($nome_arq2, 'arquivo_notafiscal', false, false);
        $arqid2 = $file2->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_notafiscal'] = $arqid2;
    } else {
        $arrRetornoIdsArquivos['peoarqid_notafiscal'] = 'NULL';
        if ($_REQUEST['requisicao'] == 'salvar') {
            die("<script>
                        alert('O arquivo da Nota Fiscal é obrigatório!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
    }

    // Arquivo do Boletim de Medição
    if ($arquivo_boletim["name"] != '' && $arquivo_boletim["type"] != '' && $arquivo_boletim["size"] != '' && $arquivo_boletim["error"] == 0) {
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $nome_arq = "anexoboletimmedicao_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file->setUpload($nome_arq, 'arquivo_boletim', false, false);
        $arqid = $file->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_boletimmedicao'] = $arqid;
    } else {
        $arrRetornoIdsArquivos['peoarqid_boletimmedicao'] = '';
    }

    return $arrRetornoIdsArquivos;
}


function buscaConstrutoras($obrid, $tipo = 'padrao')
{
    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    if ($tipo == 'padrao') {
        $sql = "
		SELECT
			ent.entid,
			ent.entnumcpfcnpj,
			ent.entnome,
			c.crtid,
			to_char(c.crtdtassinatura, 'DD/MM/YYYY') as crtdtassinatura
		FROM
			obras2.obrascontrato ocr
		INNER JOIN
			obras2.contrato c ON ocr.crtid = c.crtid OR ocr.crtid = c.crtidpai
		INNER JOIN
			entidade.entidade ent ON ent.entid = c.entidempresa
		WHERE
			ocr.ocrstatus = 'A'
		AND
			c.crtstatus = 'A'
		AND
			ent.entstatus = 'A'
		AND
			c.ttaid ISNULL
		AND
			ocr.obrid IN ({$strObrids})
			ORDER BY crtid ASC";

        $arrConstrutora = $db->carregar($sql);

        if (!is_array($arrConstrutora)) {
            $arrConstrutora = array();
        }

        return $arrConstrutora;
    } else {
        if ($tipo == 'extra') {
            $sql = "
		SELECT
			cex.cexid,
			cex.cexrazsocialconstrutora,
			cex.cexnumcnpj,
			cce.ccedataassinatura,
			cce.cceid
		FROM
			obras2.construtoraextra cex
		LEFT JOIN
			obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
		WHERE
			cexstatus = 'A'
		AND
			obrid IN ({$strObrids})
		AND
			cce.ttaid IS NULL
		AND
			cce.cceid_pai IS NULL
			ORDER BY cce.ccedataassinatura
		";


            $arrConstrutoraExtra = $db->carregar($sql);

            if (!is_array($arrConstrutoraExtra)) {
                $arrConstrutoraExtra = array();
            }

            return $arrConstrutoraExtra;
        }
    }
}


/*
 *  Retorna array para listagem principal da tela
 */
function getListaPrincipal($obrid, $dadosConstrutora, $tipo = 'padrao')
{
    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    if ($tipo == 'padrao') {

        $crtId = $dadosConstrutora['crtid'];

        $sql = "
			SELECT array_to_string( array(
				SELECT
					crt.crtid
				FROM
					obras2.contrato crt
				WHERE
				(
					crt.crtid  = {$crtId}
					or
					(crt.crtidpai = (select crtidpai from obras2.contrato where crtid = {$crtId} order by crtid limit 1) AND ttaid in (2,3))
				)
			), ', ')
		";
        $resultadoIds = $db->pegaUm($sql);

        if ($resultadoIds != '') {
            $strWhereContratos = "AND pgt.crtid in ({$resultadoIds})";
        } else {
            $strWhereContratos = '';
        }


    } else {
        if ($tipo == 'extra') {
            $cceId = $dadosConstrutora['cceid'];

            if (!empty($cceId)) {
                $sql = "
            SELECT array_to_string( array(
                SELECT
                    cce.cceid
                FROM
                    obras2.contratoconstrutoraextra cce
                WHERE
                    ccestatus = 'A'
                AND
                (
                    cce.cceid  = {$cceId}
                    or
                    (cce.cceid_pai = {$cceId} AND ttaid in (2,3))
                )
            ), ', ')
            ";
                $resultadoIds = $db->pegaUm($sql);

                if ($resultadoIds != '') {
                    $strWhereContratos = "AND pgt.cceid in ({$resultadoIds})";
                } else {
                    $strWhereContratos = '';
                }
            } else {
                $strWhereContratos = '';
            }
        }
    }

    $sql = "
		SELECT
			--ID do pagamento
			pgtid as id_pagamento,
			-- id do arquivo
			arqid as id_arquivo,
			-- número da transação (pgt)
			pgtnumtransacao as numero_transacao,
			-- data da transação (pgt)
			to_char(pgtdtpagamento, 'DD/MM/YYYY') as data_pagamento,
			-- tipo de pagamento (pgt)
			tpt.tptdesc as tipo_de_pagamento,
			-- valor da transação ( pgt)
			pgtvalortransacao as valor_pagamento,
			-- favorecido ( pgt) (cnpj)
			CASE WHEN entid IS NOT NULL THEN
		
			(
				SELECT substr(e1.entnumcpfcnpj, 1, 2) || '.' || substr(e1.entnumcpfcnpj, 3, 3) || '.' || substr(e1.entnumcpfcnpj, 6, 3) || '/' || substr(e1.entnumcpfcnpj, 9, 4) ||
				'-' || substr(e1.entnumcpfcnpj, 13) AS cnpj
				FROM
				entidade.entidade e1 where e1.entid = pgt.entid
			)
		
			WHEN cexid IS NOT NULL THEN
			(
				SELECT substr(c1.cexnumcnpj, 1, 2) || '.' || substr(c1.cexnumcnpj, 3, 3) || '.' || substr(c1.cexnumcnpj, 6, 3) || '/' || substr(c1.cexnumcnpj, 9, 4) ||
				'-' || substr(c1.cexnumcnpj, 13) AS cnpj
				FROM
				obras2.construtoraextra c1 where c1.cexid = pgt.cexid
			)
			END as favorecido_pagamento,
			-- notas fiscais ( arr to string)
			array_to_string(
				array(
					select distinct '<img border=\"0\" src=\"../imagens/icone_lupa.png\" title=\"Visualizar Nota Fiscal\" onclick=\"visualizanota( ' ||n1.ntfid|| ' )\">' || n1.ntfnumnota
					 from obras2.documentotransacao d1
					INNER JOIN obras2.notafiscal n1 ON n1.ntfid = d1.ntfid
					where d1.pgtid = pgt.pgtid
					AND n1.ntfstatus = 'A'
					AND d1.dotstatus = 'A'
				)
			, ', '
			) as notas_fiscais
		FROM
		obras2.pagamentotransacao pgt
		LEFT JOIN obras2.tipopagamentotransacao  tpt ON tpt.tptid = pgt.tptid
		WHERE
		pgtstatus = 'A'
		AND
		obrid IN ({$strObrids})
		{$strWhereContratos}
		ORDER BY pgtdtpagamento ASC  
	";

    $resultado = $db->carregar($sql);

    if(empty($dadosConstrutora['crtid']) && empty($dadosConstrutora['cceid'])){
        $resultado =array();
    }

    return (is_array($resultado)) ? $resultado : Array();
}

/*
 *  Retorna a porcentagem física ou acumulada
 *  @param 	$pgtid = o id do registro de pagamento
 *  @param 	$tipoPorcentagem = campo define qual o tipo da porcentagem vai calcular. Valores aceitos: FISICA, FISICA_ACUMULADA
 *
 */
function getPorcentagemListagem($pgtid, $tipoPorcentagem = "FISICA")
{


}

function getArrayDadosForm()
{
    $arArquivos = uploadArquivosExecucaoOrc();
    $arDado = array();

    $arDado['obrid'] = $_SESSION['obras2']['obrid'];
    $arDado['peodtpagamento'] = ajusta_data($_POST['peodtpagamento']);
    $arDado['peonrordembancaria'] = trim($_POST['peonrordembancaria']);
    $arDado['peovlordembancaria'] = str_replace(' ', '', MoedaToBd($_POST['peovlordembancaria']));
    $arDado['peodtnotafiscal'] = ajusta_data($_POST['peodtnotafiscal']);
    $arDado['peonrnotafiscal'] = trim($_POST['peonrnotafiscal']);
    $arDado['peovlnotafiscal'] = str_replace(' ', '', MoedaToBd($_POST['peovlnotafiscal']));
    $arDado['peovlordembancaria'] = (float)str_replace(' ', '', $arDado['peovlordembancaria']);
    $arDado['peovlnotafiscal'] = (float)str_replace(' ', '', $arDado['peovlnotafiscal']);

    $arDado['peopercentfispagamento'] = MoedaToBd($_POST['peopercentfispagamento']);
    $arDado['peopercentfisacumulado'] = MoedaToBd($_POST['peopercentfisacumulado']);

    $arDado['peoarqid_ordembancaria'] = $arArquivos['peoarqid_ordembancaria'];
    $arDado['peoarqid_notafiscal'] = $arArquivos['peoarqid_notafiscal'];
    $arDado['peoarqid_boletimmedicao'] = (($arArquivos['peoarqid_boletimmedicao'] != '') ? $arArquivos['peoarqid_boletimmedicao'] : 'NULL');
    $arDado['peodtinclusao'] = date('Y-m-d');
    $arDado['peodtupdate'] = date('Y-m-d');
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['peostatus'] = 'A';

    return $arDado;
}
