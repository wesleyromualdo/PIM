<?php
namespace Simec\Par3\Modelo;

use phpDocumentor\Reflection\Types\Array_;

/**
 * Gerencia a tabela: par3.regra_bloqueio
 *
 * @author felipe.tcc@gmail.com
 */
class RegraBloqueio extends \Simec\AbstractModelo
{
    /**
     * Retorna a lista com as regras de bloqueio
     * @param array $arParam
     * @return array
     */
    public function listarBloqueio(Array $arParam=array())
    {
        $where = array();
        foreach ($arParam as $k => $param) {
            switch ($k) {
                case 'somenteBloqueio':
                    $where[] = "(
                		            b.rebplanejamento = true
                		            or b.rebempenho = true
                		            or b.rebtermo = true
                		            or b.rebpagamento = true
                                )";
                    break;
                case 'somentePendencia':
                    $where[] = "(
                		            b.rebplanejamento = false 
                		            and b.rebempenho = false 
                		            and b.rebtermo = false 
                		            and b.rebpagamento = false 
                                )";
                    break;
                default:
                    $param = (array) $param;
                    $where[] = "{$k} IN (".(implode(",", $param)).")";
                    break;
            }
        }
        
        $where = ($where ? " AND " . implode(" and ", $where) : "");
        
        $sql = <<<sql
                SELECT
    	            b.rebid as "id",
    	            p.tipdsc as "descricao",
    	            b.rebplanejamento as "planejamento",
    	            b.rebempenho as "empenho",
    	            b.rebtermo as "termo",
    	            b.rebpagamento as "pagamento"
                FROM
                	par3.regra_bloqueio b
                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                WHERE
    	            p.tipstatus = 'A'
                    $where
                ORDER BY b.rebid
sql;
        $dado = $this->carregar($sql);
        return ($dado ? $dado : array()); 
    }
    
    /**
     * Retorna a tupla com a regra de bloqueio pelo id
     * @param integer $rebid
     * @return array
     */
    public function pegarBloqueioPorId($rebid)
    {
        $sql = <<<sql
                SELECT
    	            b.rebid as "id",
    	            p.tipdsc as "descricao",
    	            b.rebplanejamento as "planejamento",
    	            b.rebempenho as "empenho",
    	            b.rebtermo as "termo",
    	            b.rebpagamento as "pagamento"
                FROM
                	par3.regra_bloqueio b
                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                WHERE
    	            p.tipstatus = 'A' and
                    b.rebid = $rebid
sql;
        $dado = $this->pegaLinha($sql);
        return ($dado ? $dado : array()); 
    }
}
