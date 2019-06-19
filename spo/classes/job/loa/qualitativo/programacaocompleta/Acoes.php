<?php
/**
 * Job de Programação completa qualitativa pra SPO.
 * @version $Id: Acoes.php 136675 2018-01-16 23:20:18Z lucasgomes $
 */

class Spo_Job_Loa_Qualitativo_Programacaocompleta_Acoes extends Simec_Job_Abstract
{
    use Spo_Job_Trait_Salvarservico;

    protected $params = [
        'log' => 'spo',
        'servicos' => [
            [
                'nome' => 'Ações',
                'tabela' => 'spo.job_loa_acoesdto',
                'dto' => 'acoesDTO',
                'servico' => 'obterProgramacaoCompleta',
                'params' => ['retornarAcoes' => true]
            ],
            [
                'nome' => 'Iniciativas',
                'tabela' => 'spo.job_loa_iniciativasdto',
                'dto' => 'iniciativasDTO',
                'servico' => 'obterProgramacaoCompleta',
                'params' => ['retornarIniciativas' => true]
            ],
            [
                'nome' => 'Localizadores',
                'tabela' => 'spo.job_loa_localizadoresdto',
                'dto' => 'localizadoresDTO',
                'servico' => 'obterProgramacaoCompleta',
                'params' => ['retornarLocalizadores' => true]
            ],
            [
                'nome' => 'Objetivos',
                'tabela' => 'spo.job_loa_objetivosdto',
                'dto' => 'objetivosDTO',
                'servico' => 'obterProgramacaoCompleta',
                'params' => ['retornarObjetivos' => true]
            ],
            [
                'nome' => 'Programas',
                'tabela' => 'spo.job_loa_programasdto',
                'dto' => 'programasDTO',
                'servico' => 'obterProgramacaoCompleta',
                'params' => ['retornarProgramas' => true]
            ]
        ]
    ];

    private $prefixoTabela = 'spo';

    protected $wsqualitativo;

    public function getName()
    {
        return 'Processando programação completa...';
    }

    protected function init()
    {
        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );

        $this->wsqualitativo = new Spo_Ws_Sof_Qualitativo(
            $this->params['log'],
            Simec_BasicWS::PRODUCTION
        );

        $_SESSION['sisid'] = $this->params['sisid'];
    }

    protected function execute()
    {
        $this->carregarDados();
    }

    protected function shutdown()
    {

    }

    protected function parametrosWs($servico)
    {
        $oTA = new ObterProgramacaoCompleta();
        $oTA->exercicio = $this->params['exercicio'];
        foreach ($servico['params'] as $param => $valor) {
            $oTA->$param = $valor;
        }
        $oTA->codigoMomento = $this->params['momento'];

        return $oTA;
    }
}
