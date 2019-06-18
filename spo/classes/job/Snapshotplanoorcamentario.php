<?php
/**
 * @version $Id$
 * Date: 23/11/2017
 * Time: 13:51
 */

class Spo_Job_Snapshotplanoorcamentario extends Simec_Job_Abstract
{
    /**
     * @var array
     */
    private $param;

    /**
     * @var string
     */
    private $prefixoTabela;

    /**
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public function getName()
    {
        return 'Processando dados da carga';
    }

    protected function init()
    {
        // pega parametros
        $this->param = $this->loadParams();

        $this->prefixoTabela = "{$this->param['schemaDb']}.{$this->param['prefixoTabela']}";
    }

    /**
     * @throws Exception
     */
    protected function execute()
    {
        try {
            echo "Consolidando Informações.";
            $this->salvarOutput();

            $this->carregarTabelaSiopExecucaoTmp();

            echo "Preparando Informações.";
            $this->salvarOutput();

            if (!$this->param['localizadorOpcional']) {
                $this->limparTabelaSnapshotPlanoOrcamentario();
            }

            echo "Carregando Informações.";
            $this->salvarOutput();

            $this->carregarSnapshotPlanoOrcamentario();

            $this->commit();

            (new Acomporc_Model_Cargatipo())->historicoCarga($this->param['usuario'], $this->param['tipoCarga']);
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function shutdown()
    {
    }

    protected function setTabelaSnapshotPlanoOrcamentario()
    {
        // seta tabela snapshotPlanoOrcamentario
        return $this->nomeTabela($this->param['snapshot']['schemaDb'], $this->param['snapshot']['prefixoTabela'], 'snapshotPlanoOrcamentario');
    }

    protected function setTabelaSiopExecucaoTmp()
    {
        // seta tabela siopexecucao_tmp
        return $this->nomeTabela($this->param['siopexecucao']['schemaDb'], $this->param['siopexecucao']['prefixoTabela'], 'siopexecucao_tmp');
    }

    /**
     * @throws Exception
     */
    protected function limparTabelaSnapshotPlanoOrcamentario()
    {
        try {
            $this->setTabelaSnapshotPlanoOrcamentario();

            $this->jobDelete(["prfid = {$this->param['prfid']}"]);
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @throws Exception
     */
    protected function carregarSnapshotPlanoOrcamentario()
    {
        try {
            $this->carregarSnapshot();
            echo 'Plano Orçamentário: Migrados com sucesso.<br/>';
            $this->salvarOutput();

            // Carga dos Responsáveis do Período Anterior para cada localizador
            $this->carregarUltimosresponsaveis();
            echo 'Responsáveis do último período: Migrados com sucesso. <br/>';
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @throws Exception
     */
    protected function carregarUltimosresponsaveis()
    {
        try {
            $this->executar(
                <<<SQL
INSERT INTO {$this->param['schemaDb']}.usuarioresponsabilidade ( pflcod, usucpf, rpudata_inc, prfid, unicod, acacod )
SELECT
    pflcod,
    usucpf,
    rpudata_inc,
    {$this->param['prfid']} AS prfid,
    unicod,
    acacod
FROM {$this->param['schemaDb']}.usuarioresponsabilidade
WHERE rpustatus = 'A'
AND prfid =
    (
        SELECT prfid
          FROM {$this->param['schemaDb']}.periodoreferencia
         WHERE prftipo = 'A'
           AND prfid <> {$this->param['prfid']}
        ORDER BY prfid DESC limit 1)
AND usucpf
 || '.'
 || {$this->param['prfid']}
 || '.'
 || unicod
 || '.'
 || acacod NOT IN ( SELECT DISTINCT usucpf || '.' || prfid || '.' || unicod || '.' || acacod
                      FROM {$this->param['schemaDb']}.usuarioresponsabilidade
                     WHERE prfid = {$this->param['prfid']} )
SQL
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @throws Exception
     */
    protected function carregarSnapshot()
    {
        try {
            // Carregar dados de PO
            $this->executar(
                <<<DML
INSERT INTO {$this->setTabelaSnapshotPlanoOrcamentario()}
(ploid, acaid, prfid, plocod, dotacaoatual, liquidado, empenhado, pago,
 prddescricao, unmdescricao, metafisica, plodsc, unicod, acacod, loccod)
    SELECT DISTINCT
          plo.ploid                                                                 AS ploid
        , aca.acaid                                                                 AS acaid
        , {$this->param['prfid']}                                                   AS prfid
        , plo.plocodigo                                                             AS plocod
        , COALESCE (sex.dotacaoatual, 0)                                            AS dotacaoatual
        , COALESCE (sex.liquidado, 0)                                               AS liquidado
        , COALESCE (sex.empenhado, 0)                                               AS empenhado
        , COALESCE (sex.pago, 0)                                                    AS pago
        , (SELECT descricao
             FROM {$this->prefixoTabela}produtosdto
            WHERE codigoproduto :: VARCHAR = plo.ploproduto :: VARCHAR)             AS prddescricao
        , (SELECT descricao
             FROM {$this->prefixoTabela}unidadesmedidadto
            WHERE codigounidademedida :: VARCHAR = plo.plounidademedida :: VARCHAR) AS unmdescricao
        , COALESCE (plo.metafisica, 0)                                              AS metafisica
        , plo.plotitulo                                                             AS plodsc
        , sna.unicod
        , sna.acacod
        , sna.loccod
    FROM {$this->prefixoTabela}planosorcamentariosdto wpo
        JOIN monitora.acao aca ON aca.ididentificadorunicosiop :: INTEGER = wpo.identificadorunicoacao
        JOIN monitora.acao sna USING (acaid)
        JOIN monitora.planoorcamentario plo ON (plo.acaid = aca.acaid AND plo.plocodigo = wpo.planoorcamentario)
        JOIN (
                 SELECT
                     unicod
                     , acacod
                     , prgcod
                     , loccod
                     , plocod
                     , SUM (vlrdotacaoatual) AS dotacaoatual
                     , SUM (vlrempenhado)    AS empenhado
                     , SUM (vlrliquidado)    AS liquidado
                     , SUM (vlrpago)         AS pago
                 FROM {$this->setTabelaSiopExecucaoTmp()}
                 WHERE exercicio = '{$this->param['exercicio']}'
                 GROUP BY 1, 2, 3, 4, 5) sex ON (sex.unicod = aca.unicod
                                AND sex.acacod = aca.acacod
                                AND sex.prgcod = aca.prgcod
                                AND sex.loccod = aca.loccod
                                AND sex.plocod = plo.plocodigo)
    WHERE plo.plocodigo <> '0000'
      AND aca.prgano = '{$this->param['exercicio']}'
DML
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @throws Exception
     */
    private function carregarTabelaSiopExecucaoTmp()
    {
        try {
            $sql = $this->montarInsertSiopExecucaoTmp();

            $this->executar($sql);
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @return string
     */
    private function montarInsertSiopExecucaoTmp()
    {

        list($acao, $unidade) = $this->montarFiltroAcaoUnidade('acao', 'unidadeorcamentaria');

        $acao = $acao ? " AND {$acao}" : '';
        $unidade = $unidade ? " AND {$unidade}" : '';

        return <<<SQL
            INSERT INTO  {$this->setTabelaSiopExecucaoTmp()} (
                unicod, acacod, prgcod, loccod, plocod, vlrdotacaoinicial, vlrdotacaoatual,
                vlrempenhado, vlrliquidado, vlrpago, vlrrapnaoprocessadoinscritoliquido,
                vlrrapnaoprocessadoliquidadoapagar, vlrrapnaoprocessadopago,
                vlrrapnaoprocessadoliquidadoefetivo, exercicio
            )
            SELECT
                unidadeorcamentaria
                , acao
                , programa
                , localizador
                , planoorcamentario
                , SUM (apo.dotacaoinicial)                         AS dotacaoinicial
                , SUM (COALESCE (apo.dotacaoatual, 0))             AS dotatual
                , SUM (COALESCE (apo.empenhado, 0))                AS vlrempenhado
                , SUM (COALESCE (apo.liquidado, 0))                AS empliquidado
                , SUM (COALESCE (apo.pago, 0))                     AS pago
                , SUM (COALESCE (apo.rapinscritoliquido, 0))       AS rapInscritoLiquido
                , SUM (COALESCE (apo.rapliquidadoapagar, 0))       AS rapnaoprocessadoliquidadoapagar
                , SUM (COALESCE (apo.rappago, 0))                  AS rappagonaoprocessado
                , abs (SUM (COALESCE (apo.rapliquidadoapagar, 0))) AS rapnaoprocessadoaliquidar
                , exercicio
            FROM {$this->prefixoTabela}acompanhamentoplanoorcamentariodto apo
            JOIN {$this->prefixoTabela}acompanhamentoorcamentarioacaodto aor USING (exercicio, unidadeorcamentaria, programa, acao, localizador)
            WHERE exercicio = '{$this->param['exercicio']}'
                {$acao}
                {$unidade}
            GROUP BY unidadeorcamentaria, acao, programa, localizador, exercicio, planoorcamentario
SQL;

    }


    /**
     * @return array
     */
    private function montarFiltroAcaoUnidade($colunaAcao, $colunaUnidade)
    {
        $acao = ($acao = implode("', '", $this->param['acao'])) ? " {$colunaAcao} IN ('{$acao}')" : '';
        $unidade = ($unidade = implode("', '", $this->param['unidade'])) ? " {$colunaUnidade} IN ('{$unidade}')" : '';

        return [$acao, $unidade];
    }
}