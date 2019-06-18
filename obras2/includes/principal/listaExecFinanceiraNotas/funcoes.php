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

function getArrayDadosForm()
{
    $arArquivos = uploadArquivosExecucaoOrc();
    $arDado = array();
    // Dados do formulário
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
    // Dados de inclusão
    $arDado['peodtinclusao'] = date('Y-m-d');
    $arDado['peodtupdate'] = date('Y-m-d');
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['peostatus'] = 'A';

    return $arDado;
}




/**
 * todo. comentar
 * @param $obrid
 * @return array|mixed|NULL
 */
function carregaDadosNotas($obrid, $tipoFornecedor, $idFornecedor){
    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    $filtro = null;
    if ($tipoFornecedor === "ent") {
        $filtro = "WHERE a.entid = {$idFornecedor}";
    } elseif ($tipoFornecedor === "cex") {
        $filtro = "WHERE a.cexid = {$idFornecedor}";
    }

    $sql = "
    SELECT
      a.ntfid,
      a.fornecedor,
      a.ntfnumnota                       AS numnota,
      to_char(a.ntfdtnota, 'DD/MM/YYYY') AS datanota,
      a.ntfvalornota                     AS valornota,
      a.arqid,
      a.crtid,
      a.cceid,
      a.entid,
      a.cexid
    FROM
      (SELECT DISTINCT
         (ntf.ntfid),
         CASE WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
           THEN
             (SELECT (SELECT substr(entnumcpfcnpj, 1, 2) || '.' || SUBSTR(entnumcpfcnpj, 3, 3) || '.' ||
                             substr(entnumcpfcnpj, 6, 3) || '/' || SUBSTR(entnumcpfcnpj, 9, 4) || '-' ||
                             substr(entnumcpfcnpj, 13) AS cnpj
                      FROM (SELECT cast(entnumcpfcnpj AS VARCHAR) AS entnumcpfcnpj) a)
              FROM entidade.entidade
              WHERE entstatus = 'A' AND entid = ntf.entid)
         ELSE
           (SELECT (SELECT substr(cexnumcnpj, 1, 2) || '.' || SUBSTR(cexnumcnpj, 3, 3) || '.' ||
                           substr(cexnumcnpj, 6, 3) || '/' || SUBSTR(cexnumcnpj, 9, 4) || '-' ||
                           substr(cexnumcnpj, 13) AS cnpj
                    FROM (SELECT cast(cexnumcnpj AS VARCHAR) AS cexnumcnpj) a)
            FROM obras2.construtoraextra
            WHERE cexstatus = 'A' AND cexid = ntf.cexid)
         END AS fornecedor,
         ntf.ntfnumnota,
         ntf.ntfdtnota,
         ntf.ntfvalornota,
         ntf.arqid,
         ntf.crtid,
         ntf.cceid,
         ent.entid,
         cex.cexid
       FROM
         obras2.notafiscal ntf
         INNER JOIN
         obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
         INNER JOIN
         obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
         INNER JOIN
         obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A'
         LEFT JOIN
         entidade.entidade ent ON ent.entid = ntf.entid AND ent.entstatus = 'A'
         LEFT JOIN
         obras2.construtoraextra cex ON cex.cexid = ntf.cexid AND cex.cexstatus = 'A'
       WHERE
         ntf.ntfstatus = 'A'
         AND
         med.obrid IN ({$strObrids})
      ) AS a
      {$filtro}
    ORDER BY a.ntfdtnota;";

    $resultado = $db->carregar($sql);

    if (!$resultado) {
        $resultado = array();
    }

    return $resultado;
}

/**
 * TODO. comentar
 * @param $ntfid
 * @return string
 */
function buscaMedicoesNota($ntfid){
    global $db;

    $sql = "SELECT DISTINCT
              med.medid,
              med.mednummedicao
            FROM
              obras2.medicoes med
              INNER JOIN
              obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
              INNER JOIN
              obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
            WHERE 
              med.medstatus = 'A'
            AND
              ntm.ntfid = {$ntfid}
            ";

    $resultado = $db->carregar($sql);

    if (!$resultado) {
        $resultado = array();
    }

    /*foreach ($resultado as $k){
        $arrNumMedicao[] = $k["mednummedicao"];
    }*/

    //$strNumMedicao = implode(",", $arrNumMedicao);


    return $resultado;
}

/**
 * TODO comentar
 * @param $ntfid
 * @return bool
 */
function excluirNotaFiscal($ntfid){
    global $db;

    $sql = "UPDATE obras2.notafiscal SET ntfstatus = 'I', ntfdtinativacao = now(), ntfcpfinativacao = '{$_SESSION['usucpf']}' WHERE ntfid = {$ntfid}";

    if($db->executar($sql)){
        $sql1 = "UPDATE obras2.notamedicao SET ntmstatus = 'I', ntmdtinativacao = now(), ntmcpfinativacao = '{$_SESSION['usucpf']}' WHERE ntfid = {$ntfid}";
        if($db->executar($sql1)) {
            $db->commit();
            return true;
        }
    }
    return false;
}

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

/**
 * @Função responsável por buscar lista de fornecedores e notas fiscais de uma obra e suas vinculadas.
 * @param $obrid
 * @return array|mixed|NULL
 */
function getFornecedores($obrid) {

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);
    $arrFornecedores = array();

    if ($obrid) {

        global $db;

        $sql = "
        SELECT DISTINCT
          ent.entid                                     AS idfornecedor,
          'ent'                                         AS tipofornecedor,         
          (SELECT (SELECT substr(entnumcpfcnpj, 1, 2) || '.' || SUBSTR(entnumcpfcnpj, 3, 3) || '.' ||
                          substr(entnumcpfcnpj, 6, 3) || '/' || SUBSTR(entnumcpfcnpj, 9, 4) || '-' ||
                          substr(entnumcpfcnpj, 13) AS cnpj
                   FROM (SELECT cast(entnumcpfcnpj AS VARCHAR) AS entnumcpfcnpj) a)
           FROM entidade.entidade
           WHERE entstatus = 'A' AND entid = ntf.entid) AS fornecedor,
          crt.crtid                                     AS idcontratofornecedor,
          to_char(crt.crtdtassinatura, 'DD/MM/YYYY')    AS dataassinaturacontrato,
          ent.entnome                                   AS nomefornecedor
        FROM
          obras2.notafiscal ntf
          INNER JOIN
          obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
          INNER JOIN
          obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
          INNER JOIN
          obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A'
          INNER JOIN
          entidade.entidade ent ON ent.entid = ntf.entid AND ent.entstatus = 'A'
          INNER JOIN
          obras2.contrato crt ON ntf.crtid = crt.crtid AND crt.crtstatus = 'A'
        WHERE
          ntf.ntfstatus = 'A'
          AND
          med.obrid IN ({$strObrids}) ORDER BY crt.crtid ASC        
        ";

        $retorno = $db->carregar($sql);
        if (is_array($retorno) && !empty($retorno)) {
            $arrFornecedores = $retorno;
        }
    }

    return $arrFornecedores;

}

/**
 * Função responsável por buscar fornecedores e notas fiscais inseridos como adicionais em uma obra
 * @param $obrid
 * @return array|mixed|NULL
 */
function getFornecedoresExtras($obrid) {

    $arrFornecedores = array();

    if ($obrid) {

        global $db;

        $sql = "
        SELECT DISTINCT
          cex.cexid                                     AS idfornecedor,
          'cex'                                         AS tipofornecedor,
          (SELECT (SELECT substr(cexnumcnpj, 1, 2) || '.' || SUBSTR(cexnumcnpj, 3, 3) || '.' ||
                          substr(cexnumcnpj, 6, 3) || '/' || SUBSTR(cexnumcnpj, 9, 4) || '-' ||
                          substr(cexnumcnpj, 13) AS cnpj
                   FROM (SELECT cast(cexnumcnpj AS VARCHAR) AS cexnumcnpj) a)
           FROM obras2.construtoraextra
           WHERE cexstatus = 'A' AND cexid = ntf.cexid) AS fornecedor,
          cce.cceid                                     AS idcontratofornecedor,
          to_char(cce.ccedataassinatura, 'DD/MM/YYYY')  AS dataassinaturacontrato,
          cex.cexrazsocialconstrutora                   AS nomefornecedor
        FROM
          obras2.notafiscal ntf
          INNER JOIN
          obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
          INNER JOIN
          obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
          INNER JOIN
          obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A'
          INNER JOIN
          obras2.construtoraextra cex ON cex.cexid = ntf.cexid AND cex.cexstatus = 'A'
          INNER JOIN
          obras2.contratoconstrutoraextra cce ON ntf.cceid = cce.cceid AND cce.ccestatus = 'A'
        
        WHERE
          ntf.ntfstatus = 'A'
          AND
          med.obrid IN ({$obrid})
        ORDER BY idfornecedor ASC        
        ";

        $retorno = $db->carregar($sql);
        if (is_array($retorno) && !empty($retorno)) {
            $arrFornecedores = $retorno;
        }
    }

    return $arrFornecedores;

}

function verificaSeTemPagamento($ntfid) {

    global $db;
    $sql = "SELECT
          dotid
        FROM
          obras2.documentotransacao WHERE ntfid = {$ntfid} AND dotstatus = 'A'";

    return $db->pegaUm($sql);

}
