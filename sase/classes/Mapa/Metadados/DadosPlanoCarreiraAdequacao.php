<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação da lei PNE dos municípios.
 *
 * @version $Id: DadosPlanoCarreiraAdequacao.php 113323 2016-09-12 21:55:39Z mariluciaqueiroz $
 */

/**
 * Dado - Situação Lei PNE
 */
class DadosPlanoCarreiraAdequacao extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados da Situação da Lei PNE.
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
                                else spm.spccor
                            end as cor,
                           case
                                when pcp.docid is null then 'Sem Informação'
                                else spm.spmdsc
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
                                            when pcp.docid2 is null then (select spmid from sase.sitplanomunicipio where spmid = 1)
                                            else spm.spmid
                                        end as spmid
                                      from sase.planocarreiraprofessor pcp
                                      left join workflow.documento doc on pcp.docid2 = doc.docid
                                      left join sase.sitplanomunicipio spm on spm.esdid = doc.esdid
                                      {$where}
                                      order by pcpid)
                    select
                        spm.spmid as spcid,
                        spm.spmdsc as descricao,
                        spm.spccor as cor,
                        spm.esdid,
                        (select count(*) from temp_doc where spmid = spm.spmid) as total
                    from sase.sitplanomunicipio spm
                    group by spm.spmid, spm.spmdsc, spm.spccor
                    order by spm.spmid
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
