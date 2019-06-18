<?php
/**
 * Created by PhpStorm.
 * User: saulocorreia
 * Date: 24/11/2017
 * Time: 15:46
 */

class Spo_Job_Snapshotsubacaoorcamentario extends Simec_Job_Abstract
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
        return 'Atualizando Orçamento';
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

            $this->atualizarSnapshotSubacao();

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
    private function atualizarSnapshotSubacao()
    {
        try {

            foreach ($this->buscarDados() as $linha) {
                $this->executar(
                    <<<DML
UPDATE {$this->setTabelaSnapshotSubAcao()}
SET vlrdotacao     = {$linha['vlrdotacao']}
    ,vlrempenhado  = {$linha['vlrempenhado']}
    ,vlrliquidado  = {$linha['vlrliquidado']}
    ,vlrpago       = {$linha['vlrpago']}
WHERE prfid      = '{$linha['prfid']}'
  AND sbacod     = '{$linha['sbacod']}'
  AND ptres      = '{$linha['ptres']}'
  AND metafisica = '{$linha['metafisica']}'
  AND plocod     = '{$linha['plocod']}'
DML
                );
            }
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @return array
     * @throws Exception
     */
    private function buscarDados()
    {
        try {
            $registros = $this->carregar(
                <<<DML
  SELECT DISTINCT 
       exe.ptrid                      AS prfid,
       exe.sbacod,
       exe.ptres,
       COALESCE(po.metafisica, 0)     AS metafisica,
       po.prddsc                      AS prddescricao,
       po.unmdsc                      AS unmdescricao,
       po.plocodigo                   AS plocod,
       po.plotitulo                   AS plodsc,
       NOW()                          AS dataultimaatualizacao,
       SUM(COALESCE(psd.sadvalor, 0)) AS vlrdotacao,
       COALESCE(empenhado, 0)         AS vlrempenhado,
       COALESCE(liquidado, 0)         AS vlrliquidado,
       COALESCE(pago, 0)              AS vlrpago
    FROM (SELECT DISTINCT 
                {$this->params['prfid']} AS ptrid,
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
        LEFT JOIN monitora.pi_subacao sba ON (sba.sbacod = exe.sbacod and sba.sbaano = '{$this->params['exercicio']}')
        LEFT JOIN monitora.pi_subacaodotacao psd ON (sba.sbaid = psd.sbaid and p.ptrid = psd.ptrid)
        LEFT JOIN monitora.acao a ON (p.acaid = a.acaid)
        LEFT JOIN monitora.produtosof prds ON (a.procod = prds.procodsof)
        INNER JOIN monitora.planoorcamentario po ON (p.plocod = po.plocodigo AND p.acaid = po.acaid AND po.exercicio = '{$this->params['exercicio']}')
    GROUP BY exe.ptrid, exe.sbacod, exe.ptres, po.metafisica, po.prddsc, po.unmdsc,
             po.plocodigo, po.plotitulo, empenhado, liquidado, pago
DML
            );

            return $registros ? $registros : [];
        } catch (Exception $e) {
            throw $e;
        }
    }
}