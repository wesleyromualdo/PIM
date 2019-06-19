<?php
/**
 * Job de Programação completa qualitativa pra SPO.
 * @version $Id: Planosorcamentarios.php 136883 2018-01-24 21:36:42Z maykelbraz $
 */

class Spo_Job_Loa_Qualitativo_Programacaocompleta_Planosorcamentarios extends Spo_Job_Loa_Qualitativo_Programacaocompleta_Acoes
{
    use Spo_Job_Trait_Salvarservico;

    protected $params = [
        'log' => 'spo',
        'servicos' => [
            [
                'nome' => 'Planos orçamentários',
                'tabela' => 'spo.job_loa_planosorcamentariosdto',
                'dto' => 'planosOrcamentariosDTO',
                'servico' => 'obterProgramacaoCompleta',
                'params' => ['retornarPlanosOrcamentarios' => true]
            ]
        ]
    ];

    public function getName()
    {
        return 'Processando planos orçamentários...';
    }
}
