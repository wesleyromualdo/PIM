<?php

/**
 * Class ExecucaoFinanceira
 */
class ExecucaoFinanceira extends Modelo
{

    /**
     * Método responsável pela criação das sub-abas da aba "Execução Financeira".
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $url
     * @return string
     */
    public function criaSubAba($url)
    {
        $acao = substr($url, -1);

        $arrAbas = array(
            array(
                "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraLicitacao&acao={$acao}",
                "mnutransacao" => "Licitação",
                "mnudsc" => "Licitação"),
            array(
                "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraContratacao&acao={$acao}",
                "mnutransacao" => "Contratação",
                "mnudsc" => "Contratação"),
            array(
                "mnulink" => "obras2.php?modulo=principal/listaExecFinanceira&acao={$acao}",
                "mnutransacao" => "Medição Execução Financeira",
                "mnudsc" => "Medição"),

            array(
                "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraNotas&acao={$acao}",
                "mnutransacao" => "Nota Fiscal",
                "mnudsc" => "Nota Fiscal"),
            array(
                "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraPagamentos&acao={$acao}",
                "mnutransacao" => "Pagamento",
                "mnudsc" => "Pagamento"),
            array(
                "mnulink" => "obras2.php?modulo=principal/listaDevolucaoGRU&acao={$acao}",
                "mnutransacao" => "Devolução (GRU)",
                "mnudsc" => "Devolução (GRU)"),
            array(
                "mnulink" => "obras2.php?modulo=principal/finalizar&acao={$acao}",
                "mnutransacao" => "Finalizar",
                "mnudsc" => "Finalizar")


        );

        $html = "";

        $html .= "
            <div class='row'>
                <br />
                <div class='col-md-12'>
                    <ul class='nav nav-tabs'>
        ";

        foreach ($arrAbas as $aba) {

            $classActive = $aba["mnulink"] == $url ? "class='active'" : null;
            $idLink = str_replace(" ", "", $arrAbas['mnutransacao']);
            $link = $aba["mnulink"];
            $linkDsc = $aba["mnudsc"];

            $html .= "<li {$classActive}>
                        <a id='{$idLink}' href='{$link}'>{$linkDsc}</a></li>";
        }

        $html .= "</ul></div></div>";

        return $html;
    }

    /**
     * Método responsável por obter os dados de determinada obra.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|bool|mixed|NULL
     */
    public function capturaDadosObra($obrid)
    {
        $obraOriginal = $this->pegaObraOriginal($obrid);

        if (!$obraOriginal) {
            return false;
        }

        $obraOriginal['obraVinculada'] = $this->pegaObrasVinculadas($obraOriginal['obrid'], array(), $obraOriginal['obrid']);
        return $obraOriginal;
    }

    /**
     * Método responsável por obter os dados da obra original.
     * author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|bool|mixed|NULL
     */
    private function pegaObraOriginal($obrid)
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid,
                co.coid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A')
            WHERE obr.obrid = {$obrid}
                AND obrstatus IN ('A')
            ";
        $obra = $this->pegaLinha($strSQL);

        if ($obra) {
            return $obra;
        }

        $strSQL = "SELECT obrid, obridvinculado FROM obras2.obras WHERE obrid = {$obrid} AND obrstatus IN ('P')";
        $obra = $this->pegaLinha($strSQL);
        return $this->pegaObraOriginal($obra['obridvinculado']);
    }

    /**
     * Método responsável por obter os dados da(s) obra(s) vinculada(s) a uma obra original.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @param array $vinculados
     * @param $principal
     * @return array
     */
    private function pegaObrasVinculadas($obrid, $vinculados = array(), $principal)
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON ($principal = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A' AND obr.obrid = cod.obridvinculado)
            WHERE obr.obridvinculado = {$obrid}
                AND obr.obrstatus IN ('P');
        ";
        $vinculada = $this->pegaLinha($strSQL);

        if ($vinculada) {
            $vinculados[] = $vinculada;
            return $this->pegaObrasVinculadas($vinculada['obrid'], $vinculados, $principal);
        }

        return $vinculados;
    }

    /**
     * Método responsável por obter os 'obrids' de determinada obra (Original + vinculadas).
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array
     */
    public function retornaObrids($obrid)
    {

        $dadosObra = $this->capturaDadosObra($obrid);
        $arrObrids = array();

        if (is_array($dadosObra) && !empty($dadosObra)) {

            $arrObrids[] = $dadosObra["obrid"];

            if (is_array($dadosObra["obraVinculada"]) && !empty($dadosObra["obraVinculada"])) {
                foreach ($dadosObra["obraVinculada"] as $obraVinculada) {
                    $arrObrids[] = $obraVinculada["obrid"];
                }
            }

        }

        return $arrObrids;
    }

    /**
     * Método responsável por obter informações de um arquivo de medição.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $arqid
     * @return array
     */
    public function recuperaDadosArquivo($arqid)
    {

        if (ctype_digit($arqid)) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $files = new FilesSimec();
            return $files->getDadosArquivo($arqid);
        }
    }

    /**
     * @todo comentar.
     * @param $crtid
     * @param $cceid
     * @return bool
     */
    function verificaContratoOriginal($crtid, $cceid)
    {
        global $db;

        if (!$crtid && !$cceid) {
            return false;
        }

        if (ctype_digit($crtid) && !$cceid) {
            $sql = "SELECT crtid FROM obras2.contrato WHERE crtstatus = 'A' AND ttaid ISNULL AND crtid = {$crtid}";
        } elseif (ctype_digit($cceid) && !$crtid) {
            $sql = "SELECT ccevalor FROM obras2.contratoconstrutoraextra WHERE ccestatus = 'A' AND cceid_pai ISNULL AND ttaid ISNULL AND cceid = {$cceid}";
        }

        if ($db->pegaUm($sql)) {
            return true;
        } else {
            return false;
        }
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
    function calculaPerc($ntfid, $obrid, $dataNota)
    {

        global $db;

        $arrObrids = $this->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);
        $dataNota = formata_data_sql($dataNota);

        $sqlMedFisica = "WITH vtotal AS (SELECT DISTINCT ON (ntm.ntfid)
                                  ntm.ntfid,
                                  CASE WHEN mec.cceid ISNULL AND mec.crtid NOTNULL
                                    THEN
                                      (SELECT crtvalorexecucao
                                       FROM obras2.contrato
                                       WHERE crtstatus = 'A' AND ttaid ISNULL AND crtid = mec.crtid)
                                  ELSE
                                    (SELECT ccevalor
                                     FROM obras2.contratoconstrutoraextra
                                     WHERE ccestatus = 'A' AND cceid_pai ISNULL AND ttaid ISNULL AND cceid = mec.cceid)
                                  END AS valorcontrato
                                FROM
                                  obras2.notamedicao ntm
                                  INNER JOIN
                                  obras2.medicoes med ON ntm.medid = med.medid AND med.medstatus = 'A'
                                  INNER JOIN
                                  obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
                                WHERE
                                  ntm.ntmstatus = 'A'
                                  AND ntm.ntfid = {$ntfid}
                                GROUP BY valorcontrato, ntfid),
                    valor AS (SELECT
                                sum(ntm.ntmvlrpago) AS valor,
                                vt.valorcontrato
                              FROM
                                vtotal vt
                                INNER JOIN
                                obras2.notamedicao ntm ON vt.ntfid = ntm.ntfid
                                INNER JOIN
                                obras2.contratoconstrutoraextra cce
                                  ON ntm.cceid = cce.cceid AND cce.cceid_pai ISNULL AND cce.ttaid ISNULL
                              WHERE
                                cce.ccestatus = 'A'
                              GROUP BY vt.valorcontrato
                
                              UNION ALL
                
                              SELECT
                                sum(ntm.ntmvlrpago) AS valor,
                                vt.valorcontrato
                              FROM
                                vtotal vt
                                INNER JOIN
                                obras2.notamedicao ntm ON vt.ntfid = ntm.ntfid
                                INNER JOIN
                                obras2.contrato crt ON ntm.crtid = crt.crtid AND ttaid ISNULL
                              WHERE
                                crt.crtstatus = 'A'
                              GROUP BY vt.valorcontrato
                  )
                    SELECT (v.valor * 100) / v.valorcontrato AS percentfisica
                    FROM valor v;
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
                          (x.vlrpago*100)/y.vlrcontrato AS percentAcumulada
                        FROM
                          x
                        INNER JOIN y ON x.codigo = y.codigo;
    ";

        $medicaoAcumulada = $db->pegaUm($sqlMedAcumulada);


        if (!empty($medicaoFisica) && !empty($medicaoAcumulada)) {
            $resposta = array("medicaoFisica" => number_format($medicaoFisica, 2, ",", ".") . "%",
                "medicaoAcumulada" => number_format($medicaoAcumulada, 2, ",", ".") . "%");
        } else {
            $resposta = array("medicaoFisica" => "Não foi possível calcular a % da Medição Física",
                "medicaoAcumulada" => "Não foi possível calcular a % da Medição Acumulada");

        }

        return $resposta;
    }

    /**
     * @todo Comentar depois.
     * @param $ntfid
     * @param null $medid
     * @return array|mixed|NULL
     */
    function getValoresPagosNotaMedicao($ntfid, $mecid = null, $ignoraAditivo = false)
    {

        global $db;

        $filtroMedicao = ctype_digit($mecid) ? "AND ntm.mecid = {$mecid}" : null;
        $filtroAditivo = "
            INNER JOIN obras2.medicaocontrato mec ON mec.mecid = ntm.mecid AND mec.mecstatus = 'A'
            INNER JOIN obras2.
        ";

        $sql = "
            SELECT
              ntm.ntmvlrpago
            FROM obras2.notamedicao ntm
            WHERE
              ntm.ntmstatus = 'A'
            AND
              ntm.ntfid = {$ntfid}
            {$filtroMedicao};
        ";

        $retorno = array();
        $arrResult = $db->carregar($sql);
        if (is_array($arrResult) && !empty($arrResult)) {
            foreach ($arrResult as $r) {
                $retorno[] = (float)$r["ntmvlrpago"];
            }
        }

        return $retorno;

    }

    /**
     * @todo Comentar depois.
     * @param $mecid
     * @return bool|mixed|NULL|string
     */
    function getDadosMedicao($mecid)
    {

        global $db;

        $arrDadosMedicao = array();

        if ($mecid > 0) {
            $sql = "
                SELECT
                  med.medid,
                  med.mednummedicao,
                  to_char(med.meddtmedicao, 'DD/MM/YYYY') AS meddtmedicao,
                  to_char(med.meddtinicioexec, 'DD/MM/YYYY') AS meddtinicioexec,
                  to_char(med.meddtfimexec, 'DD/MM/YYYY') AS meddtfimexec,
                  med.medvlrmedicao
                FROM
                  obras2.medicoes med
                  INNER JOIN
                  obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
                WHERE
                  med.medstatus = 'A'
                  AND
                  mec.mecid = {$mecid};
            ";

            $result = $db->carregar($sql);
            if (is_array($result) && !empty($result)) {
                $arrDadosMedicao = current($result);
            }
        }

        return $arrDadosMedicao;

    }

    /**
     * @todo Comentar depois.
     * @param $crtid
     * @param $cceid
     * @return bool|int|mixed|NULL|string
     */
    function getValorContratoPrincipal($obrid)
    {

        global $db;

        if ($obrid) {
            $arrObras = $this->retornaObrids($obrid);
            if (is_array($arrObras) && !empty($arrObras)) {
                $strObrids = implode(",", $arrObras);
            }
        }

        $sql = "
            SELECT
              --crt.crtid,
              coalesce(ocr.ocrvalorexecucao, 0)
            FROM obras2.obras obr
              INNER JOIN obras2.obrascontrato ocr ON obr.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
              INNER JOIN obras2.contrato crt ON ((ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)) AND crt.crtstatus = 'A'
            WHERE obr.obrstatus = 'A'
                  AND obr.obrid IN ({$strObrids})
                  AND crt.ttaid ISNULL;
            ";

        $valorContratoPrincipal = $db->pegaUm($sql);
        if (!$valorContratoPrincipal) {
            $valorContratoPrincipal = 0;
        }

        return $valorContratoPrincipal;
    }

    /**
     * @todo Comentar depois.
     * @param $origemContrato
     * @param $idContrato
     * @return bool|mixed|NULL|string
     */
    function getTipoContrato($origemContrato, $idContrato)
    {

        global $db;

        if ($origemContrato !== "cex" && $origemContrato !== "crt") {
            return false;
        } else {
            if ($origemContrato === "crt") {
                $sql = "
                    SELECT CASE WHEN contratoinfo.ttaid ISNULL
                      THEN 'original'
                           ELSE 'aditivo' END tipocontrato
                    FROM (
                           SELECT c.ttaid
                           FROM obras2.contrato c
                           WHERE c.crtid = {$idContrato}
                         ) contratoinfo;
                ";
            } else {
                $sql = "
                    SELECT CASE WHEN (contratoinfo.cceid_pai ISNULL AND contratoinfo.ttaid ISNULL)
                      THEN 'original'
                           ELSE 'aditivo' END tipocontrato
                    FROM (
                           SELECT
                             cce.cceid_pai,
                             cce.ttaid
                           FROM obras2.contratoconstrutoraextra cce
                           WHERE cce.cceid = {$idContrato}
                         ) contratoinfo;
                ";
            }

            return $db->pegaUm($sql);
        }

    }

    /**
     * @todo Comentar depois.
     * @param $obrid
     * @param $valorPagoNota
     * @param $valorContratoOriginal
     * @return float|int
     */
    function calcularPercentualMedicao($valorPagoMedicao, $valorContrato)
    {

        $valorPagoMedicao = (float)$valorPagoMedicao;
        $valorContrato = (float)$valorContrato;

        $percentualMedicao = 0;

        if ($valorContrato > 0) {
            $percentualMedicao = ($valorPagoMedicao * 100) / $valorContrato;
        }

        return $percentualMedicao;

    }

    /**
     * @todo Comentar depois.
     * @param $ntfid
     * @return int
     */
    function verificaNotaFiscalComPendencia($ntfid)
    {

        if (ctype_digit($ntfid)) {
            global $db;

            $sql = "
                WITH dadosnotafiscal AS
                (
                    SELECT
                      CASE
                      WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
                        THEN ntf.entid
                      WHEN ntf.entid ISNULL AND ntf.cexid NOTNULL
                        THEN ntf.cexid
                      END idfornecedor,
                      CASE
                      WHEN ntf.crtid NOTNULL AND ntf.cceid ISNULL
                        THEN ntf.crtid
                      WHEN ntf.crtid ISNULL AND ntf.cceid NOTNULL
                        THEN ntf.cceid
                      END idcontrato,
                      ntf.arqid,
                      ntf.ntfnumnota,
                      ntf.ntfdtnota,
                      ntf.ntfvalornota,
                      ntm.mecid,
                      ntm.ntmvlrpago,
                      med.mednummedicao
                    FROM
                      obras2.notafiscal ntf
                      INNER JOIN obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                      INNER JOIN obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                      INNER JOIN obras2.medicoes med ON mec.medid = med.medid AND med.medstatus = 'A'
                    WHERE ntf.ntfstatus = 'A'
                          AND ntf.ntfid = {$ntfid}
                )
                SELECT count(*)
                FROM dadosnotafiscal
                WHERE NOT (dadosnotafiscal NOTNULL);
            ";

            return (int)$db->pegaUm($sql);

        }

    }

    function getValoresPagosNotas($obrid, $dataNotaFiscal = null)
    {

        $valoresPagos = 0;
        $filtroData = null;

        $arrData = explode("-", $dataNotaFiscal);
        if (checkdate($arrData[1], $arrData[2], $arrData[0])) {
            $filtroData = "AND ntf.ntfdtnota <= '{$dataNotaFiscal}'";
        }

        if ($obrid) {

            $strObrid = $this->retornaObrids($obrid);
            $strObrid = implode(",", $strObrid);

            global $db;

            $sql = "
                SELECT COALESCE(sum(ntm.ntmvlrpago), 0) totalpago
                      FROM obras2.medicaocontrato mec
                        INNER JOIN obras2.medicoes med ON mec.medid = med.medid
                        INNER JOIN obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
                        INNER JOIN obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
                        INNER JOIN obras2.obrascontrato ocr ON med.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
                        INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                      WHERE mec.mecstatus = 'A'
                        AND crt.crtstatus = 'A'
                        AND med.obrid IN ({$strObrid})
                        {$filtroData}
                        AND crt.ttaid ISNULL
            ";

            $valoresPagos = $db->pegaUm($sql);

        }

        return $valoresPagos;

    }

    function getValorReferenciaMedicaoAcumulada($obrid, $dataNotaFiscal)
    {

        $valorReferencia = 0;

        if ($obrid) {

            $strObrid = $this->retornaObrids($obrid);
            $strObrid = implode(",", $strObrid);

            global $db;

            $sql = "
                WITH
                    qtdobrasenvolvidas AS (k
                      SELECT count(obr.obrid) AS qtdobrasenvolvidas
                      FROM obras2.obras obr
                      WHERE obr.obrid IN ({$strObrid})
                  ),
                    obraprincipal AS (
                      SELECT CASE WHEN qoe.qtdobrasenvolvidas > 1
                        THEN (
                          SELECT obr.obrid
                          FROM
                            obras2.obras obr
                          WHERE
                            obr.obridvinculado NOTNULL
                            AND obr.obrstatus = 'A'
                            AND obr.obrid IN ({$strObrid})
                            AND obr.obridpai ISNULL
                          ORDER BY obr.obrid ASC
                          LIMIT 1
                        )
                             ELSE ({$obrid})
                             END AS obrid
                      FROM qtdobrasenvolvidas qoe
                
                  ),
                
                    contratoobraprincipal AS (
                      SELECT
                        crt.crtid,
                        COALESCE(crt.crtvalorexecucao, 0) crtvalorexecucao
                      FROM obras2.contrato crt
                        INNER JOIN obras2.obrascontrato ocr ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                        INNER JOIN obraprincipal op ON op.obrid = ocr.obrid
                        INNER JOIN entidade.entidade ent ON ent.entid = crt.entidempresa
                      WHERE crt.crtstatus = 'A'
                            AND ocr.ocrstatus = 'A'
                            AND ent.entstatus = 'A'
                            AND crt.ttaid ISNULL
                            AND ocr.obrid = op.obrid
                  ),
                
                    notaspagasobrasvinculadas AS (
                      SELECT COALESCE(sum(ntm.ntmvlrpago), 0) totalpago
                      FROM obras2.medicaocontrato mec
                        INNER JOIN obras2.medicoes med ON mec.medid = med.medid
                        INNER JOIN obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
                        INNER JOIN obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
                        LEFT JOIN contratoobraprincipal cop ON cop.crtid = mec.crtid
                        INNER JOIN obras2.obrascontrato ocr ON med.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
                        INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                      WHERE mec.mecstatus = 'A'
                        AND crt.crtstatus = 'A'
                        AND med.obrid IN ({$strObrid})
                        AND med.obrid NOT IN (cop.crtid)
                        AND ntf.ntfdtnota <= '{$dataNotaFiscal}'
                        AND crt.ttaid ISNULL
                  )
                SELECT CASE WHEN qoe.qtdobrasenvolvidas > 1
                  THEN (
                    SELECT npo.totalpago FROM notaspagasobrasvinculadas npo
                  )
                       ELSE (SELECT cop.crtvalorexecucao
                             FROM contratoobraprincipal cop)
                       END
                FROM qtdobrasenvolvidas qoe;
            ";

            $valorReferencia = $db->pegaUm($sql);

        }

        return $valorReferencia;

    }

    function getSomatorioTodosContratos($obrid, $tipoFornecedor)
    {

        if ($obrid) {

            $arrObras = $this->retornaObrids($obrid);

            if (is_array($arrObras) && !empty($arrObras)) {

                $strObrids = implode(",", $arrObras);

                global $db;

                if ($tipoFornecedor === "ent") {
                    $sql = "
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

                }

                return $db->pegaUm($sql);

            }
        }
    }

    /***************************************/

    function getValorContratoAtual($crtid, $cceid)
    {

        global $db;

        if (!$crtid && !$cceid) {
            return false;
        }

        if (ctype_digit($crtid) && !$cceid) {
            $sql = "
            SELECT 
              c.crtvalorexecucao 
            FROM obras2.contrato c
            WHERE
              c.crtstatus = 'A'
            AND crtid = {$crtid};
            ";

        } elseif (ctype_digit($cceid) && !$crtid) {
            $sql = "
            SELECT
              cce.ccevalor
            FROM obras2.contratoconstrutoraextra cce
            WHERE cce.ccestatus = 'A'
            AND cce.cceid_pai ISNULL AND cce.ttaid ISNULL
            AND cce.cceid = {$cceid};
            ";
        }

        $valorContrato = $db->pegaUm($sql);
        if (!$valorContrato) {
            $valorContrato = 0;
        }

        return $valorContrato;
    }

    function getSomatorioValoresPagosNota($obrid, $excluirContratoPrincipal = false)
    {

        if ($obrid) {

            $arrObras = $this->retornaObrids($obrid);

            if (is_array($arrObras) && !empty($arrObras)) {

                $strObrids = implode(",", $arrObras);

                global $db;

                $sql = "
                SELECT
                  coalesce(sum(ntm.ntmvlrpago), 0)
                FROM obras2.notafiscal ntf
                INNER JOIN (
                    SELECT crt.crtid
                    FROM obras2.obrascontrato ocr
                      INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                      INNER JOIN entidade.entidade ent ON ent.entid = crt.entidempresa
                    WHERE ocr.ocrstatus = 'A'
                          AND ocr.obrid IN ({$strObrids})
                          AND crt.crtstatus = 'A'
                          AND crt.ttaid ISNULL
                          AND ent.entstatus = 'A'
                    ) x ON ntf.crtid = x.crtid
                INNER JOIN obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                INNER JOIN obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                INNER JOIN obras2.medicoes med ON mec.medid = med.medid AND med.medstatus = 'A'
                INNER JOIN obras2.obras obr ON med.obrid = obr.obrid AND obr.obrstatus = 'A'
                WHERE ntf.ntfstatus = 'A'
                AND mec.crtid = x.crtid
                AND med.obrid IN ({$strObrids})
                ";

                if ($excluirContratoPrincipal) {
                    $sql .= "AND x.crtid NOT IN (
                    SELECT crt.crtid
                    FROM obras2.obras obr
                        INNER JOIN obras2.obrascontrato ocr ON obr.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
                        INNER JOIN obras2.contrato crt ON ((ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)) AND crt.crtstatus = 'A'
                    WHERE obr.obrstatus = 'A'
                        AND obr.obrid IN ({$strObrids})
                        AND crt.ttaid ISNULL
                    )";
                }

                return $db->pegaUm($sql);

            }
        }
    }

    public function gerarDocid($obrid)
    {
        global $db;

        $docid = wf_cadastrarDocumento(TPDID_EXECUCAO_FINANCEIRA, "Execução Financeira Obras II - Obra: {$obrid}", ESDID_EM_CADASTRAMENTO);

        $sql = "UPDATE obras2.execucaofinanceira SET docid = {$docid} WHERE obrid = {$obrid}";

        $db->executar($sql);

        $db->commit();

        return true;
    }

    public function salvarExecucaoFinanceira($arrDados)
    {

        global $db;

        $sql = "INSERT INTO obras2.execucaofinanceira 
                (obrid,exefinalizada)VALUES({$arrDados['obrid']},FALSE) RETURNING  exeid";

        $arrDados['exeid'] = $this->pegaUm($sql);

        $this->gerarDocid($arrDados['obrid']);
        $this->salvarDevolucaoGruExecucaoFinanceira($arrDados);

        return true;

    }

    public function salvarDevolucaoGruExecucaoFinanceira($arrDados)
    {

        $devvalor = !empty($arrDados['req']['devvalor']) ? $arrDados['req']['devvalor'] : 0;
        $campos = array(
            'exeid' => $arrDados['exeid'],
            'devvalor' => $devvalor = str_replace(' ', '', MoedaToBd($devvalor))

        );

        $file = new FilesSimec("devolucao_gru_execucao_financeira", $campos, 'obras2');
        $file->setUpload('Devolução GRU', '', true, 'devid');
        $file->getCampoRetorno();
        $this->commit();

        return true;

    }

    public function retornarDevolucaoGruExecucaoFinanceira($obrid)
    {

        global $db;

        $sql = "SELECT
                dev.devid,
                dev.arqid,
                dev.devvalor,
                arq.arqnome,
                arq.arqextensao
                FROM obras2.execucaofinanceira ex
                INNER JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.exeid = ex.exeid AND dev.devstatus = 'A'
                INNER JOIN public.arquivo arq ON arq.arqid = dev.arqid
                WHERE ex.obrid = {$obrid} ORDER BY dev.devid DESC";
        $retorno = $db->carregar($sql);

        return $retorno;

    }

    public function retornaExecucaoFinanceira($obrid)
    {
        global $db;
        $sql = "SELECT
                ex.exeid,
                ex.obrid,
                ex.docid,
                ex.exefinalizada,
                dev.devid,
                dev.arqid,
                dev.devvalor
                
                FROM obras2.execucaofinanceira ex
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.exeid = ex.exeid AND dev.devstatus = 'A'
                WHERE ex.obrid = {$obrid} ORDER BY dev.devid DESC";

        $retorno = $db->pegaLinha($sql);

        return $retorno;
    }

    public function downloadDevolucaoGruExecucaoFinanceira($arqid)
    {
        if (!$arqid) {
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        $file = new FilesSimec();
        $file->getDownloadArquivo($arqid);
    }

    public function excluirArquivoDevolucaoGruExecucaoFinanceira($arqid)
    {
        if (!$arqid) {
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }

        $file = new FilesSimec('devolucao_gru_execucao_financeira', NULL, 'obras2');
        $file->excluiArquivoFisico($arqid);
        $sql = "DELETE FROM obras2.devolucao_gru_execucao_financeira  WHERE arqid = {$arqid} ";

        $this->executar($sql);
        $this->commit();

    }

    public function retornarValorDevolucaoGruExecucaoFinanceira($obrid)
    {
        global $db;
        $sql = "SELECT 
                SUM(devvalor) 
                FROM obras2.execucaofinanceira ex 
                INNER JOIN obras2.devolucao_gru_execucao_financeira  dev ON dev.exeid = ex.exeid AND dev.devstatus = 'A'
                WHERE obrid = {$obrid}";

        $retorno = $db->pegaUm($sql);

        return $retorno;

    }

    /**
     * Função responsável por pesquisar as construtoras extras cadastradas para determinada obra.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|mixed|NULL
     */
    function buscaConstrutorasExtra($obrid)
    {
        global $db;

        $arrObrids = $this->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);

        $sql = "SELECT
              cex.cexid,
              cex.cexrazsocialconstrutora,
              cex.cexnumcnpj,
              cex.lieid,
              lie.lienumero,
              mod.moldsc,
              cce.ccedataassinatura,
              cce.cceid
            FROM 
              obras2.construtoraextra cex
            LEFT JOIN
              obras2.licitacaoextra lie ON cex.lieid = lie.lieid 
            INNER JOIN
              obras2.modalidadelicitacao mod ON lie.molid = mod.molid
            LEFT JOIN 
              obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
            WHERE
              cexstatus = 'A'
            AND
              cex.obrid IN ({$strObrids})
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

    function buscaContratoOriginalExtras($cexid)
    {
        global $db;

        $sql = "SELECT
                  cceid,
                  ccenumero,
                  ccedataassinatura,
                  ccevalor,
                  arqid,
                  cceid_pai,
                  ttaid
                FROM obras2.contratoconstrutoraextra WHERE cexid = {$cexid} 
                AND ttaid ISNULL AND cceid_pai ISNULL AND ccestatus = 'A'";

        $arrContratoExtra = $db->pegaLinha($sql);

        return $arrContratoExtra;
    }

    function buscaAditivoExtra($cexid, $cceid)
    {
        global $db;

        $sql = "SELECT
                  cceid,
                  ccenumero,
                  ccedataassinatura,
                  ccevalor,
                  arqid,
                  cceid_pai,
                  cex.ttaid,
                  tta.ttadsc
                FROM obras2.contratoconstrutoraextra cex
                INNER JOIN obras2.tipotermoaditivo tta ON cex.ttaid = tta.ttaid
                WHERE cex.cexid = {$cexid}
                AND cex.ttaid NOTNULL AND cex.cceid_pai = {$cceid} AND cex.ccestatus = 'A'";

        $arrAditivoExtra = $db->carregar($sql);
        $arrAditivoExtra = $arrAditivoExtra ? $arrAditivoExtra : array();

        return $arrAditivoExtra;
    }

    function verificarExecucaoFinanceira($obrid)
    {
        global $db;
        $sql = "SELECT 
                COUNT(*)
                FROM obras2.execucaofinanceira ex
                INNER JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.exeid = ex.exeid AND dev.devstatus = 'A'
                WHERE ex.obrid = {$obrid}";

        $retorno = $db->pegaUm($sql);

        return $retorno;

    }

    function finalizarDevolucaoGru($obrd)
    {
        global $db;
        $sql = "UPDATE obras2.execucaofinanceira SET exefinalizada = TRUE WHERE obrid = {$obrd} ";
        $db->executar($sql);
        $db->commit();
    }

    function listarObrasFinalizar($obrid)
    {
        global $db;

        $sql = "
    SELECT preid,
          docid,
          obrid,
          obrnome,
          empid,
          esdid,
          esddsc,
          exefinalizada
           FROM((
                SELECT 
                    o.preid,
                    o.docid,
                    o.obrid,
                    o.obrnome,
                    o.empid,
                    wfe.esdid,
                    wfe.esddsc,
                    ex.exefinalizada
                                    
                FROM obras2.obras o 
                INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceira ex ON ex.obrid = o.obrid
                WHERE 
                    pto.terid IN (
                                    SELECT 
                                        terid 
                                    FROM  par.termoobra pto 
                                    WHERE 
                                        preid = (
                                                    SELECT 
                                                        preid 
                                                    FROM  obras2.obras 
                                                    WHERE obrid = {$obrid})
                                                    ) 
                    AND o.obridpai IS NULL
                GROUP BY 
                    o.preid,
                    o.docid,
                    o.obrid,
                    o.obrnome,
                    o.empid,
                    wfe.esdid,
                    wfe.esddsc,
                    ex.exefinalizada
                )UNION ALL(
                SELECT 
                    o.obrid_par3,
                    o.docid,
                    o.obrid,
                    o.obrnome,
                    o.empid,
                    wfe.esdid,
                    wfe.esddsc,
                    ex.exefinalizada
                    
                FROM obras2.obras o
                INNER JOIN par3.obra po ON po.obrid = o.obrid_par3 AND o.obridpai IS NULL
                INNER JOIN par3.processoobracomposicao poc ON poc.obrid = po.obrid
                INNER JOIN par3.processo pp ON pp.proid = poc.proid
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceira ex ON ex.obrid = o.obrid
                WHERE 
                    o.obrid = {$obrid}
                )UNION ALL(
                SELECT
                    o.preid,
                    o.docid,
                    o.obrid,
                    o.obrnome,
                    o.empid,
                    wfe.esdid,
                    wfe.esddsc,
                    ex.exefinalizada
                FROM obras2.obras o
                INNER JOIN painel.dadosconvenios pd ON pd.dcoprocesso = o.obrnumprocessoconv AND o.obridpai IS NULL
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceira ex ON ex.obrid = o.obrid
                WHERE 
                    o.obrid = {$obrid} 
                )UNION ALL(
                SELECT 
                    o.preid,
                    o.docid,
                    o.obrid,
                    o.obrnome,
                    o.empid,
                    wfe.esdid,
                    wfe.esddsc,
                    ex.exefinalizada
                FROM obras2.obras o
                INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
                INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceira ex ON ex.obrid = o.obrid
                WHERE 
                    o.obrid = {$obrid} 
                ))  AS foo ORDER BY exefinalizada";

        $arrDados = $db->carregar($sql);

        return $arrDados;

    }
}