<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação da lei PNE dos municípios.
 *
 * @version $Id: DadoSituacaoAssessoramentoLeipne.php 110582 2016-04-29 14:02:39Z maykelbraz $
 */

/**
 * Dado - Situação Lei PNE
 */
class DadoSituacaoAssessoramentoLeipne extends DadosAbstract implements DadosInterface, DadosLegendaInterface
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
SELECT CASE WHEN a.assleipne IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
       CASE WHEN a.assleipne IS NOT NULL
              THEN 'Com lei PNE'
            ELSE 'Sem lei PNE' END AS situacao,
       a.muncod,
       a.estuf
  FROM sase.assessoramento a
  {$where}
DML;

        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(assleipne) AS total,
       'Com lei PNE' AS descricao,
       '#006600' AS cor
  FROM sase.assessoramento
UNION ALL
SELECT COUNT(1),
       'Sem lei PNE',
       '#FFFFFF'
  FROM sase.assessoramento
  WHERE assleipne IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
