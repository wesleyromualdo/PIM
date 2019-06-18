<?php
/**
 * Classe de mapeamento da entidade spoemendas.periodosolicitacao.
 *
 * @version $Id$
 * @since   2017.10.09
 */

/**
 * Spoemendas_Model_Periodosolicitacao: sem descricao
 *
 * @package Spoemendas\Model
 * @uses    Simec\Db\Modelo
 * @author  Saulo Araujo Correia <saulo.correia@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Periodosolicitacao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Periodosolicitacao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int                    $prsid         - default: nextval('spoemendas.periodosolicitacao_prsid_seq'::regclass)
 * @property string                 $prsdescricao  Descrição para o mês do período
 * @property \Datetime(Y-m-d H:i:s) $prsdatainicio Data inicial do período
 * @property \Datetime(Y-m-d H:i:s) $prsdatafim    Data final do período
 * @property bool                   $prsstatus     status do período - default: true
 * @property int                    $prsano        ano exercicio do período
 * @property int                    $prsmes        mes de referencia do periodo
 */
class Spoemendas_Model_Periodosolicitacao extends Modelo
{
    protected $stNomeTabela = 'spoemendas.periodosolicitacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = ['prsid'];

    protected $arAtributos = [
        'prsid'         => null,
        'prsdescricao'  => null,
        'prsdatainicio' => null,
        'prsdatafim'    => null,
        'prsstatus'     => null,
        'prsano'        => null,
        'prsmes'        => null
    ];

    public function dataPreenchida ()
    {
        return (!empty($this->arAtributos['prsdatainicio']) || !empty($this->arAtributos['prsdatafim']));
    }

    public function encontraLinha ($id)
    {
        $strSQL = <<<DML
            SELECT
                prsid,
                prsdescricao,
                TO_CHAR(prsdatainicio, 'DD/MM/YYYY') AS prsdatainicio,
                TO_CHAR(prsdatafim, 'DD/MM/YYYY') AS prsdatafim,
                prsmes
            FROM {$this->stNomeTabela}
            WHERE {$this->arChavePrimaria[0]} = {$id}
DML;

        return $this->pegaLinha($strSQL);
    }

    public function retornaPeriodoAtual ()
    {
        $strSQL = <<<DML
            SELECT
                prsid,
                prsdescricao,
                prsmes,
                prsdatainicio,
                prsdatafim,
                TO_CHAR(prsdatainicio, 'DD/MM/YYYY') AS prsdatainicio_,
                TO_CHAR(prsdatafim, 'DD/MM/YYYY') AS prsdatafim_
            FROM {$this->stNomeTabela}
            WHERE now()::date between prsdatainicio and prsdatafim
DML;

        return $this->pegaLinha($strSQL);
    }

    /**
     * Método que retorna os períodos registrados no ano informado
     *
     * @param $prsano
     * @return array|mixed|NULL
     */
    public function retornaPeriodosDoAno($prsano) {
        $sql = <<<SQL
            SELECT
              prsid,
              prsdescricao,
              prsmes,
              prsdatainicio,
              prsdatafim
            FROM spoemendas.periodosolicitacao
            WHERE prsano = {$prsano}
            ORDER BY prsano, prsmes
SQL;
        return $this->carregar($sql);
    }

    /**
     * Verifica se existe periodo em vigencia
     *
     * @return bool
     */
    public function temPeriodoAberto ()
    {
        $sql = <<<DML
SELECT COUNT(1) AS qtdperiodosabertos
  FROM spoemendas.periodosolicitacao
  WHERE prsstatus
    AND now()::DATE BETWEEN prsdatainicio AND prsdatafim
DML;

        return (bool) $this->pegaUm($sql);
    }

    /**
     * Retorna identificador do periodo vigente
     *
     * @return bool|mixed|NULL|string
     */
    public function retornaIdPeriodoAtual ()
    {
        $sql = <<<DML
SELECT
      prsid
  FROM spoemendas.periodosolicitacao
  WHERE prsstatus
    AND now()::DATE BETWEEN prsdatainicio AND prsdatafim
DML;

        return $this->pegaUm($sql);
    }

    /**
     * Retorna identificador do periodo por mês e ano
     *
     * @return bool|mixed|NULL|string
     */
    public function retornaPeriodo ($mes, $ano)
    {
        $sql = <<<DML
SELECT
      prsid
  FROM spoemendas.periodosolicitacao
  WHERE prsstatus
    AND prsmes = {$mes}
    AND prsano = {$ano}
DML;

        return $this->pegaUm($sql);
    }
}
