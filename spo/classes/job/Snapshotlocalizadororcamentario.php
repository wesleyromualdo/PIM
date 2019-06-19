<?php
/**
 * @version $Id$
 * Date: 23/11/2017
 * Time: 13:51
 */

class Spo_Job_Snapshotlocalizadororcamentario extends Simec_Job_Abstract
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
        return 'Atualizando Orçamentário';
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
            $this->carregarTabelaSiopExecucaoTmp();

            // se for obrigatorio (false) deve limpar o snapshot
            if (!$this->param['localizadorOpcional']) {
                $this->limparTabelaSnapshotLocalizador();
            }

            $this->carregarSnapshotLocalizador();

            $this->commit();

            (new Acomporc_Model_Cargatipo())->historicoCarga($this->param['usuario'], $this->param['tipoCarga']);
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function shutdown()
    {
    }

    protected function setTabelaSnapshotLocalizador()
    {
        // seta tabela snapshotlocalizador
        return $this->nomeTabela($this->param['snapshot']['schemaDb'], $this->param['snapshot']['prefixoTabela'], 'snapshotlocalizador');
    }

    protected function setTabelaSiopExecucaoTmp()
    {
        // seta tabela siopexecucao_tmp
        return $this->nomeTabela($this->param['siopexecucao']['schemaDb'], $this->param['siopexecucao']['prefixoTabela'], 'siopexecucao_tmp');
    }

    /**
     * @throws Exception
     */
    protected function limparTabelaSnapshotLocalizador()
    {
        try {
            $this->setTabelaSnapshotLocalizador();

            $this->jobDelete(["prfid = {$this->param['prfid']}"]);
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @throws Exception
     */
    protected function carregarSnapshotLocalizador()
    {
        try {
            // Carregar dados de Localizadores
            list($qtdAtualizacoes, $qtdInsercoes) = $this->mergearDadosLocalizadores($this->buscarDadosLocalizadores());
            echo "LOCALIZADORES: Foram atualizados {$qtdAtualizacoes} registros e inseridos {$qtdInsercoes} registros.<br/>";
            $this->salvarOutput();

            // Carga dos Responsáveis do Período Anterior para cada localizador
            $this->carregarUltimosresponsaveis();
            echo "Responsáveis do último período: Migrados com sucesso. <br/>";
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function carregarUltimosresponsaveis()
    {
        try {
            $this->executar("
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
AND usucpf || '.' || {$this->param['prfid']} || '.' || unicod || '.' || acacod NOT IN
                                                                  (
                                                                  SELECT DISTINCT usucpf || '.' || prfid || '.' ||unicod|| '.'||acacod
                                                                  FROM {$this->param['schemaDb']}.usuarioresponsabilidade
                                                                  WHERE prfid = {$this->param['prfid']} )");
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
            $this->executar($this->montarInsertSiopExecucaoTmp());
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
        INSERT INTO {$this->setTabelaSiopExecucaoTmp()}
            (
                unicod, acacod, prgcod, loccod, vlrdotacaoinicial, vlrdotacaoatual,
                vlrempenhado, vlrliquidado, vlrpago, vlrrapnaoprocessadoinscritoliquido,
                vlrrapnaoprocessadoliquidadoapagar, vlrrapnaoprocessadopago,
                vlrrapnaoprocessadoliquidadoefetivo, exercicio
            )
            SELECT
                unidadeorcamentaria
                , acao
                , programa
                , localizador
                , SUM (dotacaoinicial)                         AS dotacaoinicial
                , SUM (COALESCE (aor.dotacaoatual, 0))         AS dotatual
                , SUM (COALESCE (empenhado, 0))                AS vlrempenhado
                , SUM (COALESCE (aor.liquidado, 0))            AS empliquidado
                , SUM (COALESCE (aor.pago, 0))                 AS pago
                , SUM (COALESCE (rapinscritoliquido, 0))       AS rapInscritoLiquido
                , SUM (COALESCE (rapliquidadoapagar, 0))       AS rapnaoprocessadoliquidadoapagar
                , SUM (COALESCE (rappago, 0))                  AS rappagonaoprocessado
                , abs (SUM (COALESCE (rapliquidadoapagar, 0))) AS rapnaoprocessadoaliquidar
                , exercicio
            FROM
                {$this->prefixoTabela}acompanhamentoorcamentarioacaodto aor
            WHERE
                exercicio = '{$this->param['exercicio']}'
                {$acao}
                {$unidade}
            GROUP BY unidadeorcamentaria, acao, programa, localizador, exercicio
SQL;

    }

    /**
     * @return array|mixed|NULL
     * @throws Exception
     */
    private function buscarDadosLocalizadores()
    {
        try {
            list($acao, $unidade) = $this->montarFiltroAcaoUnidade('sex.acacod', 'sex.unidadeorcamentaria');

            $acao = $acao ? " AND {$acao}" : '';
            $opcional = $this->param['localizadorOpcional'] ? '' : ' AND NOT aac.snacompanhamentoopcional';

            $sql = <<<DML
SELECT DISTINCT
    maca.acaid
    , {$this->param['prfid']}                                          AS prfid
    , aca.codigoacao                                                   AS acacod
    , aca.codigoorgao                                                  AS unicod
    , loc.codigolocalizador                                            AS loccod
    , aca.codigofuncao                                                 AS funcod
    , aca.codigosubfuncao                                              AS sfncod
    , aca.codigoprograma                                               AS prgcod
    , aca.codigotipoinclusaoacao                                       AS tipoinclusaoacao
    , loc.codigotipoinclusao                                           AS tipoinclusaolocalizador
    , pro.descricao                                                    AS prddescricao
    , unm.descricao                                                    AS unmdescricao
    , COALESCE (prop.quantidadefisico :: INTEGER, 0)                   AS metafisica
    , COALESCE (prop.valorfisico :: FLOAT, 0)                          AS financeiro
    , COALESCE (sex.dotacaoinicial, 0)                                 AS dotacaoinicial
    , COALESCE (sex.dotacaoatual, 0)                                   AS dotacaoatual
    , COALESCE (sex.empenhado, 0)                                      AS empenhado
    , COALESCE (sex.liquidado, 0)                                      AS liquidado
    , COALESCE (sex.pago, 0)                                           AS pago
    , COALESCE (sex.rapinscritoliquido, 0)                             AS rapinscritoliquido
    , COALESCE (sex.rapliquidadoapagar, 0)                             AS rapliquidadoapagar
    , COALESCE (sex.rappago, 0)                                        AS rappago
    , COALESCE (sex.rapliquidadoapagar, 0) + COALESCE (sex.rappago, 0) AS rapliquidadoefetivo
    , aac.snacompanhamentoopcional                                     AS acompanhamentoopcional
FROM {$this->prefixoTabela}acoesdto aca
    JOIN (
             SELECT
                 codigolocalizador
                 , identificadorunicoacao
                 , MIN (codigotipoinclusao) AS codigotipoinclusao
             FROM {$this->prefixoTabela}localizadoresdto
             GROUP BY
                 1,
                 2) loc ON aca.identificadorunico = loc.identificadorunicoacao
    JOIN {$this->prefixoTabela}produtosdto pro ON aca.codigoproduto = pro.codigoproduto :: TEXT
    JOIN {$this->prefixoTabela}unidadesmedidadto unm ON aca.codigounidademedida = unm.codigounidademedida
    JOIN (
             SELECT
                 codigoorgao
                 , codigoacao
                 , codigolocalizador
                 , COALESCE (quantidadefisico :: INTEGER, 0) AS quantidadefisico
                 , COALESCE (CASE WHEN valorfisico = '' THEN 0
                             ELSE valorfisico :: FLOAT END, 0) AS valorfisico
                 , MIN (codigotipoinclusaolocalizador)       AS codigotipoinclusaolocalizador
             FROM {$this->prefixoTabela}propostadto
             GROUP BY 1, 2, 3, 4, 5) prop ON (prop.codigoorgao = aca.codigoorgao
                             AND prop.codigoacao = aca.codigoacao
                             AND prop.codigolocalizador = loc.codigolocalizador)
    JOIN (
             SELECT
                 unicod
                 , acacod
                 , loccod
                 , SUM (vlrdotacaoinicial)                   AS dotacaoinicial
                 , SUM (vlrdotacaoatual)                     AS dotacaoatual
                 , SUM (vlrempenhado)                        AS empenhado
                 , SUM (vlrliquidado)                        AS liquidado
                 , SUM (vlrpago)                             AS pago
                 , SUM (vlrrapnaoprocessadoinscritoliquido)  AS rapinscritoliquido
                 , SUM (vlrrapnaoprocessadoliquidadoapagar)  AS rapliquidadoapagar
                 , SUM (vlrrapnaoprocessadopago)             AS rappago
                 , SUM (vlrrapnaoprocessadoliquidadoefetivo) AS rapliquidadoefetivo
             FROM {$this->setTabelaSiopExecucaoTmp()}
             WHERE
                 exercicio = '{$this->param['exercicio']}'
                 AND vlrdotacaoinicial > 0
                 AND vlrdotacaoatual > 0
                 GROUP BY 1, 2, 3) sex ON (sex.unicod = aca.codigoorgao
                                           AND sex.acacod = aca.codigoacao
                                           AND sex.loccod = loc.codigolocalizador)
    LEFT JOIN {$this->prefixoTabela}acoesacompanhamentoorcamentariodto aac ON (aac.codigoorgao = aca.codigoorgao
                                                                  AND aac.codigoacao = aca.codigoacao
                                                                  AND aac.codigoprograma = aca.codigoprograma
                                                                  AND aac.localizadores = loc.codigolocalizador)
    JOIN monitora.acao maca ON (maca.unicod = aca.codigoorgao
                                AND maca.acacod = aca.codigoacao
                                AND maca.loccod = loc.codigolocalizador
                                AND maca.prgano = '{$this->param['exercicio']}')
WHERE 1=1
    {$opcional} {$acao}
DML;

            return is_array($registros = $this->carregar($sql)) ? $registros : [];
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $registros
     *
     * @return array
     * @throws Exception
     */
    private function mergearDadosLocalizadores($registros)
    {
        try {
            $qtdAtualizacoes = 0;
            $qtdInsercoes = 0;

            foreach ($registros as $linha) {
                $sql = <<<SQL
    UPDATE {$this->setTabelaSnapshotLocalizador()}
    SET
        financeiro = {$linha['financeiro']},
        dotacaoinicial = {$linha['dotacaoinicial']},
        dotacaoatual = {$linha['dotacaoatual']},
        empenhado = {$linha['empenhado']},
        liquidado = {$linha['liquidado']},
        pago = {$linha['pago']},
        rapinscritoliquido = {$linha['rapinscritoliquido']},
        rapliquidadoapagar = {$linha['rapliquidadoapagar']},
        rappago = {$linha['rappago']},
        rapliquidadoefetivo = {$linha['rapliquidadoefetivo']}
    WHERE prfid = {$linha['prfid']}
      AND acacod = '{$linha['acacod']}'
      AND unicod = '{$linha['unicod']}'
      AND loccod = '{$linha['loccod']}'
      AND prgcod = '{$linha['prgcod']}'
    RETURNING sslid
SQL;
                $sslid = $this->pegaUm($sql);

                if ($sslid) {
                    $qtdAtualizacoes++;
                }
            }

            return [$qtdAtualizacoes, $qtdInsercoes];
        } catch (Exception $e) {
            throw $e;
        }
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