<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos estados.
 *
 * @version $Id: DadoEstadosPaisAdesao.php 110582 2016-04-29 14:02:39Z maykelbraz $
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */

/**
 * Classe de dados de Adesao.
 */
class DadoEstadosPaisAdesao extends DadosAbstract implements DadosInterface
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
SELECT CASE WHEN ase.aseadesao IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
       CASE WHEN ase.aseadesao IS NOT NULL
              THEN 'Com Adesão'
            ELSE 'Sem Adesão' END AS situacao,
       ase.estuf
  FROM sase.assessoramentoestado ase
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(aseadesao) AS total,
       'Com Adesão' AS descricao,
       '#006600' AS cor
  FROM sase.assessoramentoestado
UNION ALL
SELECT COUNT(1),
       'Sem Adesão',
       '#FFFFFF'
  FROM sase.assessoramentoestado
  WHERE aseadesao IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
