<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos municípios.
 *
 * @version $Id: DadoSituacaoAssessoramentoAdesao.php 110677 2016-05-03 20:01:21Z mariluciaqueiroz $
 */

/**
 * Dado - Situação de adesão
 */
class DadoSituacaoAssessoramentoAdesao extends DadosAbstract implements DadosInterface, DadosLegendaInterface
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
            $where = "WHERE a.estuf IN ('". implode("','", $this->estuf) . "') ";
        }

        $sql = <<<DML
SELECT CASE WHEN a.assadesao IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
       CASE WHEN a.assadesao IS NOT NULL
              THEN 'Com Adesão'
            ELSE 'Sem Adesão' END AS situacao,
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
SELECT count(assadesao) AS total,
       'Com Adesão' AS descricao,
       '#006600' AS cor
  FROM sase.assessoramento
UNION ALL
SELECT COUNT(1),
       'Sem Adesão',
       '#FFFFFF'
  FROM sase.assessoramento
  WHERE assadesao IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
