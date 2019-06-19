<?php
/**
 * Implementação da consulta de dados da Lei PNE dentro do estado.
 *
 * @version $Id: DadoMacroCategoriaLeipne.php 110599 2016-04-29 18:37:40Z maykelbraz $
 */

/**
 * Dado - Macro-Categoria
 */
class DadoMacroCategoriaLeipne extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados da Situação da Lei PNE na MacroCategoria
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

        $where1 = $where2 = '';
        if (is_array($this->estuf)) {
            $where1 = "WHERE estuf IN('" . implode($this->estuf) . "')";
            $where2 = " AND estuf IN('" . implode($this->estuf) . "')";
        }

        $sql = <<<DML
SELECT count(assleipne) AS total,
       'Com lei PNE' AS macdsc,
       '#006600' AS maccor
  FROM sase.assessoramento
  {$where1}
UNION ALL
SELECT COUNT(1),
       'Sem lei PNE',
       '#FFFFFF'
  FROM sase.assessoramento
  WHERE assleipne IS NULL
  {$where2}
  ORDER BY macdsc
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
