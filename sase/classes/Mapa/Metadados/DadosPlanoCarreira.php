<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos municípios.
 *
 * @version $Id: DadosPlanoCarreira.php 113323 2016-09-12 21:55:39Z mariluciaqueiroz $
 */

/**
 * Dado - Situação de adesão
 */
class DadosPlanoCarreira extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados da Situação de Adesão
     * @return void
     */
    public function carregaDado()
    {
        global $db;

        if ($this->estuf === 'undefined') {
            $this->estuf = '';
        }

        $where = '';
        if ((is_array($this->estuf) && count($this->estuf) > 0)) {
            $where = "WHERE estuf IN ('". implode("','", $this->estuf) . "') ";
        }

        $sql = <<<DML
SELECT 
       case
	    when pcp.docid is null then '#FFFFFF'
	    else spc.spccor
	end as cor,
       case
	    when pcp.docid is null then 'Sem Informação'
	    else spcdsc
	    end AS situacao,
       muncod,
       estuf
FROM sase.planocarreiraprofessor pcp
left join workflow.documento doc on doc.docid = pcp.docid
left join workflow.documento doc2 on doc2.docid = pcp.docid2
left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
  {$where}
DML;

        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            {$where}
                            order by pcpid)
                    select
                        spc.spcid,
                        spc.spcdsc as descricao,
                        spc.spccor as cor,
                        spc.esdid,
                        (select count(*) from temp_doc where spcid = spc.spcid) as total
                    from sase.sitplancarprofessor spc
                    group by spc.spcid, spc.spcdsc, spc.spccor
                    order by spc.spcid
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
