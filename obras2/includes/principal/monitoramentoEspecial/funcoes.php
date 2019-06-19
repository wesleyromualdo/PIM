<?php 
function getSqlMonitoramentoEspecial($obrid) {
		return <<<SQL
		SELECT
                m.mesid,
                m.mestitulo,
                er.esddsc esddscresponsavel,
                es.esddsc esddscsituacao,
                TO_CHAR(m.mesdtcadastro, 'DD/MM/YYYY') mesdtcadastro,
                TO_CHAR(m.mesdtlimite, 'DD/MM/YYYY') mesdtlimite,
                u.usunome
            FROM obras2.monitoramento_especial m
            JOIN workflow.documento dr ON dr.docid = m.docidresponsavel
            JOIN workflow.estadodocumento er ON er.esdid = dr.esdid
            JOIN workflow.documento ds ON ds.docid = m.docidsituacao
            JOIN workflow.estadodocumento es ON es.esdid = ds.esdid
            JOIN seguranca.usuario u ON u.usucpf = m.usucpfinclusao
            WHERE
                m.messtatus = 'A'
                AND m.obrid = {$obrid}
SQL;
}