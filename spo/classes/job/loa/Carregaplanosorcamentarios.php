<?php
/**
 * Job para carregar a tabela monitora.acao.
 *
 * CUIDADO: Este job apaga a tabela monitora.acao para o exercicio selecionado.
 *
 * @version $Id: Carregaplanosorcamentarios.php 136885 2018-01-24 21:50:16Z maykelbraz $
 */

class Spo_Job_Loa_Carregaplanosorcamentarios extends Simec_Job_Abstract
{
    protected $params = [];

    public function getName()
    {
        return 'Carregando planos orçamentários...';
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
        echo "Apagando planos orçamentários do exercício {$this->params['exercicio']}...<br />";
        $this->salvarOutput();
        $this->apagarPlanosorcamentarios($this->params['exercicio']);

        echo 'Carregando novos planos orçamentários...<br />';
        $this->salvarOutput();
        $qtdPlanosorcamentariosInseridos = $this->inserirPlanosorcamentarios(
            $this->params['exercicio']
        );

        echo "{$qtdPlanosorcamentariosInseridos} planos orçamentários inseridos...";
        $this->salvarOutput();
    }

    protected function shutdown()
    {
    }

    protected function apagarPlanosorcamentarios($exercicio)
    {
        $this->setTable('monitora.planoorcamentario');
        $this->jobDelete(["exercicio = '{$exercicio}'"]);
    }

    protected function inserirPlanosorcamentarios($exercicio)
    {
        $sql = <<<DML
INSERT INTO monitora.planoorcamentario(prgcod,
                                       acacod,
                                       unicod,
                                       plocodigo,
                                       ploidentificadorunicosiop,
                                       plotitulo,
                                       plodetalhamento,
                                       ploproduto,
                                       plounidademedida,
                                       ploobrigatorio,
                                       plostatus,
                                       acaid,
                                       exercicio)
SELECT aca.prgcod,
       aca.acacod,
       aca.unicod,
       plo.planoorcamentario,
       plo.identificadorunico,
       plo.titulo,
       plo.detalhamento,
       plo.codigoproduto,
       plo.descunidademedida,
       false,
       'A',
       aca.acaid,
       '{$exercicio}'
  FROM spo.job_loa_planosorcamentariosdto plo
    JOIN monitora.acao aca ON (aca.ididentificadorunicosiop::INTEGER = plo.identificadorunicoacao::INTEGER)
  WHERE aca.prgano = '{$exercicio}';
DML;

        $this->executar($sql);
        if (!($this->commit())) {
            throw new Exception('Não foi possível inserir os planos orçamentários.');
        }

        $sql = <<<DML
SELECT COUNT(1)
  FROM monitora.planoorcamentario
  WHERE exercicio = '{$exercicio}'
DML;
        return $this->pegaUm($sql);
    }
}
