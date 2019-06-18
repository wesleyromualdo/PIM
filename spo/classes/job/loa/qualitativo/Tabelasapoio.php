<?php
/**
 * Job de tabela de apoio qualitativa pra SPO.
 * @version $Id$
 */

class Spo_Job_Loa_Qualitativo_Tabelasapoio extends Simec_Job_Abstract
{
    use Spo_Job_Trait_Salvarservico;

    protected $params = [
        'log' => 'spo',
        'servicos' => [
            [
                'nome' => 'Esferas',
                'tabela' => 'spo.job_loa_esferasdto',
                'dto' => 'esferasDTO',
                'servico' => 'obterTabelasApoio',
                'params' => ['retornarEsferas' => true]
            ],
            [
                'nome' => 'Funções',
                'tabela' => 'spo.job_loa_funcoesdto',
                'dto' => 'funcoesDTO',
                'servico' => 'obterTabelasApoio',
                'params' => ['retonarFuncoes' => true]
            ],
            [
                'nome' => 'Subfunções',
                'tabela' => 'spo.job_loa_subfuncoesdto',
                'dto' => 'subFuncoesDTO',
                'servico' => 'obterTabelasApoio',
                'params' => ['retornarSubFuncoes' => true]
            ],
            [
                'nome' => 'Produtos',
                'tabela' => 'spo.job_loa_produtosdto',
                'dto' => 'produtosDTO',
                'servico' => 'obterTabelasApoio',
                'params' => ['retornarProdutos' => true]
            ],
            [
                'nome' => 'Unidades de medidas',
                'tabela' => 'spo.job_loa_unidadesmedidadto',
                'dto' => 'unidadesMedidaDTO',
                'servico' => 'obterTabelasApoio',
                'params' => ['retornarUnidadesMedida' => true]
            ],
            [
                'nome' => 'Tipos de ação',
                'tabela' => 'spo.job_loa_tiposacaodto',
                'dto' => 'tiposAcaoDTO',
                'servico' => 'obterTabelasApoio',
                'params' => ['retornarTiposAcao' => true]
            ],
        ]
    ];

    private $prefixoTabela;

    protected $wsqualitativo;

    public function getName()
    {
        return 'Processando tabelas de apoio...';
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
        $oTA = new ObterTabelasApoio();
        $oTA->exercicio = $this->params['exercicio'];
        foreach ($servico['params'] as $param => $valor) {
            $oTA->$param = $valor;
        }

        return $oTA;
    }
}
