<?php
/**
 * Job de preparação de base para carga da LOA
 * @version $Id: Acoes.php 136665 2018-01-16 22:37:12Z maykelbraz $
 */

class Spo_Job_Loa_Preparabase_Acoes extends Simec_Job_Abstract
{

    protected $params = [
        'limpar-tabelas' => [
            //tabelas de apoio
            'spo.job_loa_esferasdto',
            'spo.job_loa_funcoesdto',
            'spo.job_loa_subfuncoesdto',
            'spo.job_loa_produtosdto',
            'spo.job_loa_unidadesmedidadto',
            'spo.job_loa_tiposacaodto',
            //programação completa
            'spo.job_loa_acoesdto',
            'spo.job_loa_iniciativasdto',
            'spo.job_loa_localizadoresdto',
            'spo.job_loa_objetivosdto',
            'spo.job_loa_programasdto'
        ]
    ];

    private $prefixoTabela;

    public function getName()
    {
        return 'Preparando a base de dados...';
    }

    protected function init()
    {
    }

    protected function execute()
    {
        echo "Limpando base para armazenar dados do SIOP...<br />";
        $this->salvarOutput();

        foreach ($this->params['limpar-tabelas'] as $tabela) {
            try {
                echo "Limpando tabela {$tabela}...<br />";
                $this->salvarOutput();

                $this->setTable($tabela);
                $this->jobDelete('', true);
            } catch (Exception $ex) {
                throw $ex;
            }
        }
    }

    protected function shutdown()
    {

    }
}
