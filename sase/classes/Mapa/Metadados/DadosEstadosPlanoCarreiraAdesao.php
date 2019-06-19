<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos estados.
 *
 * @version $Id: DadosEstadosPlanoCarreiraAdesao.php 113443 2016-09-19 14:22:02Z mariluciaqueiroz $
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */

/**
 * Classe de dados de Adesao.
 */
class DadosEstadosPlanoCarreiraAdesao extends DadosAbstract implements DadosInterface
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
SELECT CASE WHEN pceadesao IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
       CASE WHEN pceadesao IS NOT NULL
              THEN 'Com Adesão'
            ELSE 'Sem Adesão' END AS situacao,
       ase.estuf
  FROM sase.planocarreiraprofessorestado ase
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(pceadesao) AS total,
       'Com Adesão' AS descricao,
       '#006600' AS cor
  FROM sase.planocarreiraprofessorestado
UNION ALL
SELECT COUNT(1) as total,
       'Sem Adesão' AS descricao,
       '#FFFFFF' as cor
  FROM sase.planocarreiraprofessorestado
  WHERE pceadesao IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
