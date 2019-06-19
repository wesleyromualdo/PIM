<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos estados.
 *
 * @version $Id: DadosEstadosPlanoCarreira.php 113323 2016-09-12 21:55:39Z mariluciaqueiroz $
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */

/**
 * Classe de dados de Adesao.
 */
class DadosEstadosPlanoCarreira extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados com informações da Lei PNE dos estados.
     *
     * @return void
     */
    public function carregaDado()
    {
        global $db;

        $sql = <<<DML
                SELECT 
                       case
                            when pce.docid is null then '#FFFFFF'
                            else spc.spccor
                        end as cor,
                       case
                            when pce.docid is null then 'Sem Informação'
                            else spc.spcdsc
                            end AS situacao,
                       estuf
                FROM sase.planocarreiraprofessorestado pce
                left join workflow.documento doc on doc.docid = pce.docid
                left join workflow.documento doc2 on doc2.docid = pce.docid2
                left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
                    with temp_doc as (select
                                pce.pceid,
                                case
                                    when pce.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessorestado pce
                            left join workflow.documento doc on pce.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            {$where}
                            order by pceid)
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
