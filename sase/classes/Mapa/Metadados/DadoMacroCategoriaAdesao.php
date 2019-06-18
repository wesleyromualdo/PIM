<?php
/**
 * Implementação da consulta de dados de Adesão dentro do estado.
 *
 * @version $Id: DadoMacroCategoriaAdesao.php 110599 2016-04-29 18:37:40Z maykelbraz $
 */

/**
 * Dado - Macro-Categoria
 */
class DadoMacroCategoriaAdesao extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Adesão na MacroCategoria
     *
     * @return void
     */
    public function carregaDado()
    {
        global $db;

        $where = '';
        if (is_array($this->EstadosSolicitados)) {
            $where = "WHERE a.estuf IN ('" . implode("','", $this->EstadosSolicitados) . "')";
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

        $where1 = $where2 = '';
        if (is_array($this->estuf)) {
            $where1 = "WHERE estuf IN('" . implode($this->estuf) . "')";
            $where2 = " AND estuf IN('" . implode($this->estuf) . "')";
        }

        $sql = <<<DML
SELECT count(assadesao) AS total,
       'Com Adesão' AS macdsc,
       '#006600' AS maccor
  FROM sase.assessoramento
  {$where1}
UNION ALL
SELECT COUNT(1),
       'Sem Adesão',
       '#FFFFFF'
  FROM sase.assessoramento
  WHERE assadesao IS NULL
  {$where2}
  ORDER BY macdsc
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
