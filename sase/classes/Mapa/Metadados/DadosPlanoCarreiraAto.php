<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos municípios.
 *
 * @version $Id: DadosPlanoCarreiraAto.php 120490 2017-03-20 17:27:39Z gabrielalmeida $
 */

/**
 * Dado - Situação de adesão
 */
class DadosPlanoCarreiraAto extends DadosAbstract implements DadosInterface, DadosLegendaInterface
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
	CASE WHEN pcpato IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
	CASE WHEN pcpato IS NOT NULL
              THEN 'Com  Ato Legal da Comissão'
            ELSE 'Sem  Ato Legal da Comissão' END AS situacao,
       muncod,
       estuf
FROM sase.planocarreiraprofessor 
  {$where}
DML;


        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(pcpato) AS total,
       'Com  Ato Legal da Comissão' AS descricao,
       '#006600' AS cor
  FROM sase.planocarreiraprofessor
UNION ALL
SELECT COUNT(1),
       'Sem  Ato Legal da Comissão',
       '#FFFFFF'
  FROM sase.planocarreiraprofessor
  WHERE pcpato IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
