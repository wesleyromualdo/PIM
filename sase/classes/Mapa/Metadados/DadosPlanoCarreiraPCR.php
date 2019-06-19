<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação da lei PNE dos municípios.
 *
 * @version $Id: DadosPlanoCarreiraPCR.php 113323 2016-09-12 21:55:39Z mariluciaqueiroz $
 */

/**
 * Dado - Situação Lei PNE
 */
class DadosPlanoCarreiraPCR extends DadosAbstract implements DadosInterface, DadosLegendaInterface
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
            $where = "WHERE a.estuf IN ('". implode("','", $this->estuf) . "') ";
        }

        $sql = <<<DML
SELECT 
	CASE WHEN pcppcr IS NOT NULL
		THEN '#006600'
		ELSE '#FFFFFF' END AS cor,
	CASE WHEN pcppcr IS NOT NULL
		THEN 'Com PCR'
		ELSE 'Sem PCR' END AS situacao,
       muncod,
       estuf
FROM sase.planocarreiraprofessor pcp
  {$where}
DML;

        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(pcppcr) AS total,
       'Com PCR' AS descricao,
       '#006600' AS cor
  FROM sase.planocarreiraprofessor
UNION ALL
SELECT COUNT(1),
       'Sem PCR',
       '#FFFFFF'
  FROM sase.planocarreiraprofessor
  WHERE pcppcr IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
