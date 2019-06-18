<?php
namespace Simec\Corporativo\Workflow\Modelo;

/**
 * Gerencia a tabela: workflow.historicodocumento
 *
 * @author felipe.tcc@gmail.com
 */
class HistoricoDocumento extends \Simec\AbstractModelo
{
    public function listarSql($docid)
    {
        if (!$docid) {
            return '';
        }
        
        $sql = <<<DML
            SELECT
                hd.hstid,
                ed.esddsc,
                ac.aeddscrealizada,
                us.usunome,
                TO_CHAR(hd.htddata, 'DD/MM/YYYY HH24:MI:SS') as datahora,
                hd.docid,
                us.unicod,
                TO_CHAR(hd.htddata - COALESCE(LEAD(hd.htddata) over (), docdatainclusao), 'DD" dia(s) " HH24:MI:SS') as tempo_decorrido,
                un.unidsc,
                COALESCE(cd.cmddsc, '-') AS cmddsc
            FROM
                workflow.historicodocumento hd
            JOIN workflow.documento d on d.docid = hd.docid
            JOIN workflow.acaoestadodoc ac ON (ac.aedid = hd.aedid)
            JOIN workflow.estadodocumento ed ON (ed.esdid = ac.esdidorigem)
            JOIN seguranca.usuario us ON (us.usucpf = hd.usucpf)
            LEFT JOIN workflow.comentariodocumento cd ON (cd.hstid = hd.hstid)
            LEFT JOIN public.unidade un on un.unicod = us.unicod
            WHERE
                hd.docid = {$docid}
            ORDER BY 
                hd.htddata ASC
DML;
        return $sql;
    }
}
