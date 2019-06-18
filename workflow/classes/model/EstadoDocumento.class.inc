<?php
/**
 * Classe de mapeamento da entidade workflow.estadodocumento.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade workflow.estadodocumento.
 *
 * @see Modelo
 */
class Workflow_Model_EstadoDocumento extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'workflow.estadodocumento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'esdid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'tpdid' => array(
            'tabela' => 'workflow.tipodocumento', 'pk' => 'tpdid'
        ),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'esdid' => null,
        'tpdid' => null,
        'esdstatus' => null,
        'esdordem' => null,
        'esddsc' => null,
        'constantevirtual' => null,
        'esdprazo' => null,
        'esdatraso' => null,
        'esdatrazo' => null,
        'esdfim' => null
    );


    /**
     * Função pegarSQLSelect
     *
     * @return query para montar componentes.
     */
    public function pegarSQLSelect($arrPost)
    {
        $where = '';

        if ($arrPost['esdid'] != '') $where = " and e.esdid = ".$arrPost['esdid'];
        if ($arrPost['tpdid'] != '') $where = " and e.tpdid = ".$arrPost['tpdid'];

        $sql = "

        (SELECT
                	0 as codigo,
                	'0 - Não Iniciado' as descricao)
                	UNION ALL
        (SELECT
                    esdid as codigo,
                	CASE
                	     WHEN esdid=1638 THEN '1 - Etapa Preparatória do Diagnóstico'
                	     WHEN esdid=1683 THEN '3 - Elaboração'
                         WHEN esdid=1637 THEN '2 - Diagnóstico'

                     END as descricao
                FROM workflow.estadodocumento e
                WHERE
                    esdstatus = 'A'
                	$where
                ORDER BY descricao ASC)

                ";

        return $sql;

    }

    public function getArrayEstadoDocumentoPAR3()
    {
        $sql = <<<DML
SELECT esdid as codigo,
       esddsc as descricao
  FROM {$this->stNomeTabela}
  WHERE tpdid = 243
    AND esdstatus = 'A'
  ORDER BY esdordem ASC
DML;
        $estados = $this->carregar($sql);
        if(empty($estados)){
            return [];
        }
        array_unshift($estados,array('codigo'=>'0','descricao'=>'Diagnóstico Não Iniciado'));
        $arrayLista['Etapa Diagnóstico'] = array();
        $arrayLista['Etapa Diagnóstico'][$estados[0]['codigo']] = '0 - '.$estados[0]['descricao'];
        $arrayLista['Etapa Diagnóstico'][$estados[1]['codigo']] = '1 - '.$estados[1]['descricao'];
        $arrayLista['Etapa Diagnóstico'][$estados[2]['codigo']] = '2 - '.$estados[2]['descricao'];
        $arrayLista['Etapa Diagnóstico'][$estados[3]['codigo']] = '3 - '.$estados[3]['descricao'];

        $arrayLista['Etapa Planejamento'] = array();
        $arrayLista['Etapa Planejamento'][$estados[4]['codigo']] = '4 - '.$estados[4]['descricao'];
        $arrayLista['Etapa Planejamento'][$estados[5]['codigo']] = '5 - '.$estados[5]['descricao'];
        $arrayLista['Etapa Planejamento'][$estados[6]['codigo']] = '6 - '.$estados[6]['descricao'];

        return $arrayLista;
    }

    /**
     * Carrega todos os estados com base em um tipo de documento.
     *
     * @param int $tpdid Tipo do documento para busca dos estados.
     * @return mixed[]
     */
    public function carregarEstados($tpdid)
    {
        $sql = <<<DML
SELECT esdid,
       esddsc
  FROM {$this->stNomeTabela}
  WHERE tpdid = {$tpdid}
    AND esdstatus = 'A'
DML;
        $estados = $this->carregar($sql);
        return empty($estados)?[]:$estados;
    }
}
