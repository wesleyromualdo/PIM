<?php
/**
 * Implementação da consulta de dados de adequação dentro do estado.
 *
 * @version $Id: DadoMacroCategoria.php 110596 2016-04-29 17:43:50Z maykelbraz $
 */

/**
 * Dado - Macro-Categoria
 */
class DadoMacroCategoria extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    protected $uf;

    public function setUf($uf)
    {
        $this->uf = $uf;
    }

    /**
     * Carrega metadados da Situação de Assessoramento Baseado na MacroCategoria
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
SELECT s.stacor AS cor,
       CASE WHEN m.macdsc IS NULL
              THEN 'Não Cadastrado'
            ELSE m.macdsc END AS situacao,
       a.muncod,
       a.estuf
  FROM sase.assessoramento a
    INNER JOIN workflow.documento d ON d.docid = a.docid
    INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid
    INNER JOIN sase.macrocategoria m ON s.maccod = m.maccod
  {$where}
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        if (is_null($this->estuf)) {
            throw new Exception('Defina uma lista de UFs antes de solicitar as legendas.');
        }

        $sql = <<<DML
SELECT s.stacod as maccod,
       s.stadsc as macdsc,
       s.stacor as maccor,
       s.stacod
  from sase.macrocategoria m
    left join sase.situacaoassessoramento s on s.maccod = m.maccod
    left join sase.assessoramento a on a.stacod = s.stacod
  group by 1,2,3
  order by stacod ASC
DML;

        $lista = $db->carregar($sql);
        $legenda = array();
        $rotacao = array();
        foreach ($lista as $value) {
            $sql = "
                select s.stacod, count(a.assid) as total
                from workflow.documento d
                inner join sase.assessoramento a on a.docid = d.docid
                inner join sase.situacaoassessoramento s on s.esdid = d.esdid and s.stacod = " . ( ($value['stacod'])?$value['stacod']:0 ) . "
                inner join sase.macrocategoria m on m.maccod = s.maccod
                where
                                        " . (($this->estuf!=''&&count($this->estuf)>0)?" a.estuf in ( '". (implode( "','", $this->estuf )) ."' ) ":"") . "
                group by 1 ";

            $total = $db->carregar($sql);

            if (array_search($value['maccod'], $rotacao) === false) {
                array_push($legenda, array('maccor' => $value['maccor'],'macdsc'=>$value['macdsc'],'total'=>$total[0]['total']));
                array_push($rotacao, $value['maccod']);
            }else{
                $legenda[count($legenda)-1]['total'] = $legenda[count($legenda)-1]['total'] + $total[0]['total'];
            }
        }

        return $legenda?$legenda:[];
    }
}
