<?php
namespace Simec\Corporativo\Workflow\Modelo;

/**
 * Gerencia a tabela: workflow.documento
 *
 * @author felipe.tcc@gmail.com
 */
class Documento extends \Simec\AbstractModelo
{
    public function pegarDados($docid)
    {
        if (!$docid){
            return array();
        }
        
        $sql = "select
    				docid,
    				docdsc,
    				ed.esdid,
                    ed.esddsc
    			from 
                    workflow.documento d
                join workflow.estadodocumento ed on ed.esdid = d.esdid
    			where
    				d.docid = " . $docid;
        $dado = $this->pegaLinha($sql);
        
        return ($dado ? $dado : array());
    }
    
}
