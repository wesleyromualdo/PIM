<?php

function getSqlSolicitacoes($obrid, $pronumeroprocesso )
{
	$solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();
    $dadosProcessoConv     = $solicitacaoDesembolso->pegaProcessoConvPorObra($obrid);
    $where[]               = "processo = '".$pronumeroprocesso."'";

    $sql = "

    SELECT
    sv.sdcid,
    sv.sdcjustificativa,
    u.usunome usunome1,
    TO_CHAR(sv.sdcdatainclusao, 'DD/MM/YYYY') sdcdatainclusao,
    e.esddsc,
    e.esdid,
     (
			SELECT ud.usunome FROM workflow.historicodocumento h
			LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
			WHERE h.docid = d.docid ORDER BY h.htddata DESC LIMIT 1
                ) as usunome,
    (
    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
    ) as htddata,
    (
    SELECT c.cmddsc FROM workflow.historicodocumento h
    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
    ) as cmddsc,
    sdcpercsolicitado,
    sdcpercpagamento,
    sdcparcela,
    sdcobstec,
    sv.docid
    FROM obras2.solicitacao_desembolso_convenio sv
    JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
    JOIN workflow.documento d ON d.docid = sv.docid
    JOIN workflow.estadodocumento e ON e.esdid = d.esdid
    WHERE sv.sdcstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . " ORDER BY sv.sdcid DESC";


   return $sql;


}





function getSqlTooIdNotIs2($obrid){

$where[] = "o.obrid = ".$obrid;

    $sql = "
            SELECT
                sv.sldid,
                o.obrid,
                o.obrnome,
                m.estuf,
                m.mundescricao,
                sv.sldjustificativa,
                u.usunome usunome1,
                TO_CHAR(sv.slddatainclusao, 'DD/MM/YYYY') slddatainclusao,
                e.esddsc,
                e.esdid,
                  (
					SELECT ud.usunome FROM workflow.historicodocumento h
					LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
					WHERE h.docid = d.docid ORDER BY h.htddata DESC LIMIT 1
                ) as usunome,
                (SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1) as htddata,
                (SELECT c.cmddsc FROM workflow.historicodocumento h
                LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1 ) as cmddsc,
                sldpercsolicitado,
                supid,
                sldpercpagamento,
                sldpercobra,
                sldobstec,
                sv.docid,
        		sldjustificativacancelamento,
        		sldpedidocancelado::int,
        		sldcpfcancelamento,
        		to_char( slddatacancelamento, 'DD/MM/YYYY às HH24:MI:SS') as data_cancelamento,
        		slddatacancelamento,
                ps.situacao as situacao_pagamento
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio m ON m.muncod = ed.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            LEFT JOIN (

                SELECT f.sldid, array_to_string(array_agg(f.situacao), '<br />')  as situacao FROM (
                    SELECT f.sldid, f.percentualpag || ' - ' || f.situacao_pagamento as situacao FROM (
                        SELECT sldid, situacao_pagamento, sum(percentualpag) as percentualpag FROM obras2.v_solicitacao_pagamento GROUP BY 1,2
                    ) as f
                ) as f
                GROUP BY f.sldid

            ) ps ON ps.sldid = sv.sldid
            WHERE sv.sldstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
            ORDER BY sv.sldid DESC";


	return $sql;
}