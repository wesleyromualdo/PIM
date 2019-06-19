<?php
namespace Simec\Exemplo\Modelo\PessoaTeste;

class PessoaTeste extends \Simec\AbstractModelo
{    
    public function listaSql($arParam=array())
    {
        $arParam = ($arParam ? $arParam : array());
        
        $where = array();
        foreach ($arParam as $attr => $valor) {
            if (empty($valor) || empty($valor[0]))
                continue;
                   
            switch ($attr) {
                case 'nome':
                    $where[] = sprintf("petnome ilike '%%%s%%'", $valor);
                    break;
                case 'idade':
                    $where[] = sprintf("petidade = %s", $valor);
                    break;
                case 'estuf':
                    $valor = (array) $valor;
                    $where[] = sprintf("estuf in ('%s')", implode("', '", $valor));
                    break;
            }
        }
        
        $sql = "SELECT 
                    pt.petid, petnome, petidade, 
                    case when petsexo = 'M' then 'Masculino' else 'Feminino' end petsexo, 
                    ARRAY_TO_STRING(ARRAY_AGG(pte.estuf), ', ') as estuf
                FROM 
                    exemplo.pessoa_teste pt
                join exemplo.pessoa_teste_estado pte on pte.petid = pt.petid
                WHERE
                    petstatus= 'A'
                ". (count($where) ? " and " . implode(" and ", $where) : "") ."
                group by
                	pt.petid, petnome, petidade, petsexo
                order by
                    pt.petnome";
        
        return $sql;
    }
    
    public function pegarEstufAssociadoPorPetid($petid)
    {
        if (!is_numeric($petid)) {
            return array();
        }
        
        $sql = "select
                    estuf
                from
                    exemplo.pessoa_teste_estado
                where
                    petid = {$petid}";
        $dado = $this->carregarColuna($sql);
        return ($dado ? $dado : array());
    }
    
    public function deletarVinculoEstadoPorPetid($petid)
    {
        if (!is_numeric($petid)) {
            return false;
        }
        
        $sql = "delete from exemplo.pessoa_teste_estado where petid = {$petid}";
        $this->executar($sql);
        
        return true;
    }
}