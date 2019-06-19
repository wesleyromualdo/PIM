<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação da Lei PNE dos estados.
 *
 * @version $Id: DadoEstadosPaisLeipne.php 110582 2016-04-29 14:02:39Z maykelbraz $
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */

/**
 * Classe de dados da Lei PNE.
 */
class DadoEstadosPaisLeipne extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados com informações da Lei PNE dos estados.
     *
     * @return void
     */
    public function carregaDado()
    {
        global $db;

        $sql = <<<DML
SELECT CASE WHEN ase.aseleipne IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
       CASE WHEN ase.aseleipne IS NOT NULL
              THEN 'Com lei PNE'
            ELSE 'Sem lei PNE' END AS situacao,
       ase.estuf
  FROM sase.assessoramentoestado ase
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(aseleipne) AS total,
       'Com lei PNE' AS descricao,
       '#006600' AS cor
  FROM sase.assessoramentoestado
UNION ALL
SELECT COUNT(1),
       'Sem lei PNE',
       '#FFFFFF'
  FROM sase.assessoramentoestado
  WHERE aseleipne IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
