<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adesão dos estados.
 *
 * @version $Id: DadosEstadosPlanoCarreiraAto.php 120490 2017-03-20 17:27:39Z gabrielalmeida $
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */

/**
 * Classe de dados de Adesao.
 */
class DadosEstadosPlanoCarreiraAto extends DadosAbstract implements DadosInterface
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
SELECT CASE WHEN pceato IS NOT NULL
              THEN '#006600'
            ELSE '#FFFFFF' END AS cor,
       CASE WHEN pceato IS NOT NULL
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
SELECT count(pceato) AS total,
       'Com Adesão' AS descricao,
       '#006600' AS cor
  FROM sase.planocarreiraprofessorestado
UNION ALL
SELECT COUNT(1) as total,
       'Sem Adesão' AS descricao,
       '#FFFFFF' as cor
  FROM sase.planocarreiraprofessorestado
  WHERE pceato IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
