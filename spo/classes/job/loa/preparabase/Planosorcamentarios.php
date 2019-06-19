<?php
/**
 * Job de preparação de base para carga da LOA
 * @version $Id: Planosorcamentarios.php 136680 2018-01-16 23:43:32Z maykelbraz $
 */

class Spo_Job_Loa_Preparabase_Planosorcamentarios extends Spo_Job_Loa_Preparabase_Acoes
{
    protected $params = [
        'limpar-tabelas' => [
            //programação completa
            'spo.job_loa_planosorcamentariosdto',
        ]
    ];
}
