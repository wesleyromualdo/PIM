<?php
/**
 * Created by PhpStorm.
 * User: saulocorreia
 * Date: 24/11/2017
 * Time: 15:46
 */

class Spo_Job_Snapshotsubacao extends Simec_Job_Abstract
{
    /**
     * @var array
     */
    private $params;

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
        $this->params = $this->loadParams();

        $this->prefixoTabela = "{$this->params['schemaDb']}.{$this->params['prefixoTabela']}";
    }

    /**
     * @throws Exception
     */
    protected function execute()
    {
        try {
            echo "Processamento Iniciado.<br>";
            $this->salvarOutput();

            $this->inserirTabelaSiopExecucao();

            $this->limparTabelaSnapshotSubAcao();

            $this->commit();

            $this->carregarSnapshotSubacao();
            echo "Carregado dados de subação.<br>";

            $this->carregarUltimosresponsaveis();
            echo "Responsáveis do último período: Migrados com sucesso. <br/>";

            $this->commit();

            (new Acomporc_Model_Cargatipo())->historicoCarga($this->params['usuario'],$this->params['tipoCarga']);

            echo "<br>Processamento Finalizado.";
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function shutdown()
    {
    }

    /**
     * @throws Exception
     */
    private function inserirTabelaSiopExecucao()
    {
        try {
            $this->executar(<<<DML
INSERT INTO {$this->setTabelaSiopExecucaoTmp()}
( unicod, acacod, prgcod, loccod, ptres, plicod, vlrdotacaoinicial, vlrdotacaoatual, vlrempenhado,
  vlrliquidado, vlrpago, vlrrapnaoprocessadoinscritoliquido, vlrrapnaoprocessadoliquidadoapagar,
  vlrrapnaoprocessadopago, vlrrapnaoprocessadoliquidadoefetivo, exercicio )
SELECT unidadeorcamentaria,
       acao,
       programa,
       localizador,
       numeroptres,
       planointerno,
       dotacaoinicial::NUMERIC(15,2),
       (CASE WHEN dotatual = ''
               THEN '0'
             ELSE dotatual
          END)::NUMERIC(15,2),
       (empenhadoaliquidar::NUMERIC(15,2) + COALESCE(empliquidado::NUMERIC(15,2))) AS vlrempenhado,
       empliquidado::numeric(15,2) AS vlrliquidado,
       pago::NUMERIC(15,2),
       rapnaoprocessadoaliquidar::NUMERIC(15,2),
       rapnaoprocessadoliquidadoapagar::NUMERIC(15,2),
       rappagonaoprocessado::NUMERIC(15,2),
       0,
       '{$this->params['exercicio']}'
  FROM {$this->prefixoTabela}execucaoorcamentariadto wex_o
 WHERE anoexercicio = '{$this->params['exercicio']}'
DML
            );
        } catch (Exception $e) {
            throw $e;
        }
    }


    /**
     * @throws Exception
     */
    private function limparTabelaSnapshotSubAcao()
    {
        try {
            $this->setTabelaSnapshotSubAcao();

            $this->jobDelete(["prfid = {$this->params['prfid']}"]);
        } catch (Exception $e) {
            throw $e;
        }
    }

    private function setTabelaSnapshotSubAcao()
    {
        return $this->nomeTabela($this->params['snapshot']['schemaDb'], $this->params['snapshot']['prefixoTabela'], 'snapshotsubacao');
    }


    private function setTabelaSiopExecucaoTmp()
    {
        // seta tabela siopexecucao_tmp
        return $this->nomeTabela($this->params['siopexecucao']['schemaDb'], $this->params['siopexecucao']['prefixoTabela'], 'siopexecucao_tmp');
    }

    /**
     * @throws Exception
     */
    private function carregarSnapshotSubacao()
    {
        try {
            $this->executar(
                <<<DML
INSERT INTO {$this->setTabelaSnapshotSubAcao()}
(prfid, sbacod, ptres, metafisica, prddescricao, unmdescricao,
 plocod, plodsc, dataultimaatualizacao, vlrdotacao, vlrempenhado,
 vlrliquidado, vlrpago)
  SELECT DISTINCT exe.ptrid,
                  exe.sbacod,
                  exe.ptres,
                  COALESCE(po.metafisica, 0)  AS metafisica,
                  po.prddsc,
                  po.unmdsc,
                  po.plocodigo,
                  po.plotitulo,
                  NOW(),
                  SUM(COALESCE(psd.sadvalor, 0)) AS sadvalor,
                  COALESCE(empenhado, 0) AS empenhado,
                  COALESCE(liquidado, 0)       AS liquidado,
                  COALESCE(pago, 0)            AS pago
    FROM (SELECT DISTINCT {$this->params['prfid']}            AS ptrid,
                          SUBSTR(plicod,2,4)       AS sbacod,
                          sex.ptres,
                          SUM(sex.vlrempenhado)    AS empenhado,
                          SUM(sex.vlrliquidado)    AS liquidado,
                          SUM(sex.vlrpago)         AS pago
            FROM {$this->setTabelaSiopExecucaoTmp()} sex
            WHERE sex.exercicio = '{$this->params['exercicio']}'
              AND sex.unicod IN ('26101', '26298', '26290', '74902')
              AND SUBSTR(sex.plicod,2,4) <> ''
              AND sex.ptres <> ''
            GROUP BY 1, 2, 3
            ORDER BY 1, 2) exe
        LEFT JOIN monitora.ptres p ON (p.ptres = exe.ptres)
        LEFT JOIN monitora.pi_subacao sba ON (sba.sbacod = exe.sbacod
                                              and sba.sbaano = '{$this->params['exercicio']}')
        LEFT JOIN monitora.pi_subacaodotacao psd ON (sba.sbaid = psd.sbaid
                                                     and p.ptrid = psd.ptrid)
        LEFT JOIN monitora.acao a ON (p.acaid = a.acaid)
        LEFT JOIN monitora.produtosof prds ON (a.procod = prds.procodsof)
        INNER JOIN monitora.planoorcamentario po ON (p.plocod = po.plocodigo
                                                     AND p.acaid = po.acaid
                                                     AND po.exercicio = '{$this->params['exercicio']}')
    GROUP BY exe.ptrid, exe.sbacod, exe.ptres, po.metafisica, po.prddsc, po.unmdsc,
             po.plocodigo, po.plotitulo, empenhado, liquidado, pago
DML
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function carregarUltimosresponsaveis()
    {
        try {
            $this->executar(<<<SQL
INSERT INTO {$this->params['schemaDb']}.usuarioresponsabilidade (pflcod, usucpf, rpudata_inc, prfid, unicod, sbacod)
    SELECT DISTINCT
        pflcod
        , usucpf
        , rpudata_inc
        , {$this->params['prfid']} AS prfid
        , unicod
        , sbacod
    FROM {$this->params['schemaDb']}.usuarioresponsabilidade
    WHERE
        rpustatus = 'A'
        AND sbacod IS NOT NULL
        AND prfid =
            (
                SELECT prfid
                FROM {$this->params['schemaDb']}.periodoreferencia
                WHERE
                    prftipo = 'S'
                    AND prfid <> {$this->params['prfid']}
                ORDER BY
                    prfid DESC
                LIMIT 1)
        AND (usucpf
                , prfid
                , unicod
                , sbacod) NOT IN (
                SELECT
                    DISTINCT
                    usucpf
                    , prfid
                    , unicod
                    , sbacod
                FROM {$this->params['schemaDb']}.usuarioresponsabilidade
                WHERE
                    prfid = {$this->params['prfid']}
                    AND sbacod IS NOT NULL
                    AND rpustatus = 'A'
            )
SQL
            );
        } catch (Exception $e) {
            throw $e;
        }
    }
}