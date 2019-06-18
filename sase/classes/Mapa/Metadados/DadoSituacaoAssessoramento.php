<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adequação dos municípios.
 *
 * @version $Id: DadoSituacaoAssessoramento.php 110582 2016-04-29 14:02:39Z maykelbraz $
 */

/**
 * Dado - Situação Assessoramento
 */
class DadoSituacaoAssessoramento extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento
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
SELECT s.stacor AS cor,
       CASE WHEN s.stacod IS NULL
              THEN 'Não Cadastrado'
            ELSE s.stadsc END AS situacao,
       a.muncod,
       a.estuf
  FROM sase.assessoramento a
    INNER JOIN workflow.documento d ON d.docid = a.docid
    INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid
  {$where}
DML;

        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT s.stacod,
       s.stadsc AS descricao,
       s.stacor AS cor,
       count(a.assid) AS total
  FROM sase.situacaoassessoramento s
    LEFT JOIN sase.assessoramento a on a.stacod = s.stacod
  WHERE stastatus = 'A'
  GROUP BY 1,2,3
  ORDER BY stacod ASC
DML;
        $legenda = $db->carregar($sql);

        foreach (is_array($legenda)?$legenda:[] as $key => &$value) {
            $sql = <<<DML
SELECT count(d.docid) AS total
  FROM workflow.documento d
    INNER JOIN sase.situacaoassessoramento s ON d.esdid = s.esdid AND s.stacod = {$value['stacod']}
    INNER JOIN sase.assessoramento a ON a.docid = d.docid
DML;
            $total = $db->pegaUm($sql);
            $value['total'] = ('' == $total)?0:$total;
        }

        return $legenda?$legenda:[];
    }
}
