<?php
/**
 * Job de preparação de base para carga da LOA
 * @version $Id: Subacoes.php 136894 2018-01-24 22:46:10Z maykelbraz $
 */

class Spo_Job_Loa_Preparabase_Subacoes extends Simec_Job_Abstract
{

    protected $params = [
        'tabela' => 'monitora.pi_subacao',
    ];

    public function getName()
    {
        return 'Preparando a base de dados...';
    }

    protected function init()
    {
    }

    protected function execute()
    {
        echo "Apagando subações do exercício {$this->params['exercicio']}...<br />";
        $this->salvarOutput();

        global $db;
        $sql = <<<SQL
DELETE
  FROM monitora.pi_subacao
  WHERE sbaano = '{$this->params['exercicio']}';
SQL;
        $db->executar($sql);
    }

    protected function shutdown()
    {
    }
}
