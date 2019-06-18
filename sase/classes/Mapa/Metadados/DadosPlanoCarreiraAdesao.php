<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos municípios.
 *
 * @version $Id: DadosPlanoCarreiraAdesao.php 113323 2016-09-12 21:55:39Z mariluciaqueiroz $
 */

/**
 * Dado - Situação de adesão
 */
class DadosPlanoCarreiraAdesao extends DadosAbstract implements DadosInterface, DadosLegendaInterface
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
	CASE WHEN pcpadesao IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
	CASE WHEN pcpadesao IS NOT NULL
              THEN 'Com Adesão'
            ELSE 'Sem Adesão' END AS situacao,
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
SELECT count(pcpadesao) AS total,
       'Com Adesão' AS descricao,
       '#006600' AS cor
  FROM sase.planocarreiraprofessor
UNION ALL
SELECT COUNT(1),
       'Sem Adesão',
       '#FFFFFF'
  FROM sase.planocarreiraprofessor
  WHERE pcpadesao IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
