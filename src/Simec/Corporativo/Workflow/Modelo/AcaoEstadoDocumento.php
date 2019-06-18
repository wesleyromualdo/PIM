<?php
namespace Simec\Corporativo\Workflow\Modelo;

/**
 * Gerencia a tabela: workflow.acaoestadodoc
 *
 * @author felipe.tcc@gmail.com
 */
class AcaoEstadoDocumento extends \Simec\AbstractModelo
{
    public function pegarAcaoPorEstado($esdid)
    {
        if (!$esdid){
            return array();
        }
        
        $sql = "select
        			a.aedid,
        			a.aeddscrealizar,
        			a.aedobs,
        			a.aedicone,
        			a.aedcodicaonegativa,
        			a.aedpreacao,
                    a.aedvisivel,
                    a.esdidorigem,
                    ed.esddsc as esddscorigem,
                    a.esdiddestino,
    				a.aeddscrealizada,
    				a.aedcondicao,
    				a.esdsncomentario,
    				a.aedposacao,
    				a.aeddatafim,
                    a.aeddescregracondicao,
    				case when a.aeddatainicio is null then
    					'S'
                    else
                    	case
    						when cast(to_char(now(), 'YYYY-MM-DD') as date) BETWEEN a.aeddatainicio and a.aeddatafim then 'S'
    						else 'N'
    					end
                    end as bloqueio_periodo
        		from 
                    workflow.acaoestadodoc a
                join workflow.estadodocumento ed on ed.esdid = a.esdidorigem
        		where
        			esdidorigem = " . $esdid . " and
        			aedstatus = 'A'
        		order by 
                    a.aedordem asc";
        $dado = $this->carregar($sql);
        
        return ($dado ? $dado : array());
    }
    
}
