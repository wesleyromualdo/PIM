<?php
/**
 * Job cara de PTRES com execução orçamentária pra SPO.
 * @version $Id: Carregaptres.php 136847 2018-01-23 23:57:59Z maykelbraz $
 */

class Spo_Job_Loa_Carregaptres extends Simec_Job_Abstract
{
    protected $params = [];

    const TABLE_NAME = 'monitora.ptres';

    public function getName()
    {
        return 'Carregando PTRES...';
    }

    protected function init()
    {
        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );
    }

    protected function execute()
    {
        echo "Apagando PTRES do exercício {$this->params['exercicio']}...<br />";
        $this->salvarOutput();
        $this->apagarPtres();

        echo 'Carregando novos PTRES...<br />';
        $this->salvarOutput();
        $this->inserirPtres();

        echo $this->getTotalPtres() . " PTRES inseridos...";
        $this->salvarOutput();
    }

    protected function shutdown()
    {
    }

    protected function apagarPtres()
    {
        $this->setTable(self::TABLE_NAME);
        $this->jobDelete(["ptrano = '{$this->params['exercicio']}' AND ptres <> '111523'"]);
    }

    private function getTotalPtres()
    {
        return $this->pegaUm("SELECT COUNT(*) FROM monitora.ptres WHERE ptrano = '{$this->params['exercicio']}'");
    }

    protected function inserirPtres()
    {
        // -- Ajustando sequence para tratar exceção do TED - 111523
        $sql = <<<DML
SELECT setval('monitora.ptres_ptrid_seq', COALESCE((SELECT MAX(ptrid)+1 FROM monitora.ptres), 1), false);
DML;
        $this->executar($sql);

        $strSQL = <<<DML
INSERT INTO monitora.ptres (ptres,
                            acaid,
                            ptrano,
                            funcod,
                            sfucod,
                            prgcod,
                            acacod,
                            loccod,
                            unicod,
                            ptrdotacao,
                            ptrstatus,
                            plocod,
                            esfcod)
SELECT *
  FROM (SELECT exe.numeroptres AS ptres,
               (SELECT acaid
                  FROM monitora.acao aca
                  WHERE aca.unicod = exe.unidadeorcamentaria
                    AND aca.acacod = exe.acao
                    AND aca.loccod = exe.localizador
                    AND aca.prgano = exe.anoexercicio
                    AND aca.acastatus = 'A') AS acaid,
               exe.anoexercicio AS ptrano,
               exe.funcao AS funcod,
               exe.subfuncao AS sfucod,
               exe.programa AS prgcod,
               exe.acao AS acacod,
               exe.localizador AS loccod,
               exe.unidadeorcamentaria AS unicod,
               SUM(exe.dotinicialsiafi::NUMERIC(15,2)) AS ptrdotacao,
               'A' AS ptrstatus,
               exe.planoorcamentario AS plocod,
               exe.esfera AS esfcod
          FROM spo.job_loa_execucaoorcamentariadto exe
          WHERE exe.numeroptres <> ''
            AND exe.numeroptres <> '111523'
          GROUP BY exe.numeroptres,
                   exe.anoexercicio,
                   exe.funcao,
                   exe.subfuncao,
                   exe.programa,
                   exe.acao,
                   exe.localizador,
                   exe.unidadeorcamentaria,
                   exe.planoorcamentario,
                   exe.esfera) tab
  WHERE tab.acaid IS NOT NULL
DML;

        $this->executar($strSQL);
        if (!($this->commit())) {
            throw new Exception('Não foi possível inserir o PTRES.');
        }
    }
}