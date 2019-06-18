<?php
namespace Simec\Corporativo\Workflow\Modelo;

/**
 * Gerencia a tabela: workflow.estadodocumentoperfil
 *
 * @author felipe.tcc@gmail.com
 */
class EstadoDocumentoPerfil extends \Simec\AbstractModelo
{
    public function pegarPerfilPorAcao($aedid)
    {
        $aedid = (array) $aedid;
        
        if (!$aedid) {
            return array();
        }
        
        $sql = "select
                    aedid,
        			pflcod
        		from workflow.estadodocumentoperfil
        		where
        			aedid in (" . implode(', ', $aedid) . ")";
        $dado = $this->carregar($sql);
        $dado = ($dado ? $dado : array());
        
        $retorno = array();
        foreach ($dado as $d) {
            $retorno[$d['aedid']][] = $d['pflcod'];
        }
        
        return $retorno;
    }
    
}
