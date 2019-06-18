<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 11/01/18
 * Time: 19:56
 */

class Spo_Job_Loa_Quantitativo_Execucaoorcamentaria extends Simec_Job_Abstract
{
    use Spo_Job_Trait_Salvarservico;

    protected $params = [
        'log' => 'spo',
        'servicos' => [
            [
                'nome' => 'Execução Orçamentária',
                'tabela' => 'spo.job_loa_execucaoorcamentariadto',
                'dto' => 'RetornoExecucaoOrcamentariaDTO',
                'servico' => 'consultarExecucaoOrcamentaria',
                'params' => [
                    'filtro' => [],
                    'selecionaRetorno' => [
                        'numeroptres',
                        'acao',
                        'funcao',
                        'subFuncao',
                        'programa',
                        'localizador',
                        'unidadeOrcamentaria',
                        'dotInicialSiafi',
                        'planoOrcamentario',
                        'esfera',
                        'anoExercicio'
                    ],
                    'pagina' => 0,
                    'retornaArray' => false,
                    'metodo' => 'consultarExecucaoOrcamentaria',
                    'retornoPorPagina' => 2000
                ]
            ],
        ]
    ];

    private $paginaAtual = 0;

    public function getName()
    {
        return 'Processando execução orçamentária...';
    }

    protected function init()
    {
        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );

        $this->wsqualitativo = new Spo_Ws_Sof_Quantitativo(
            $this->params['log'],
            Simec_BasicWS::PRODUCTION
        );

        $_SESSION['sisid'] = $this->params['sisid'];

        return true;
    }

    protected function execute()
    {
        $this->carregarDados();
    }

    protected function shutdown()
    {
        // TODO: Implement shutdown() method.
    }

    protected function parametrosWs($servico)
    {
        $servico['params']['filtro']['anoExercicio'] = $this->params['exercicio'];
        return $servico['params'];
    }

    /**
     * Deleta informações da tabela temporaria e inseri novos registros
     *
     * @param $resultados
     * @param $servico
     */
    private function persistirDados($resultados, $servico)
    {

        $this->salvarOutput();


        if ($resultados) {
            $this->setTable($servico['tabela']);
            $count = $this->jobInsert($resultados, true);

            echo "{$count} {$servico['nome']} importados(as)...<br />";
            $this->salvarOutput();

            return;
        }

        echo "0 {$servico['nome']} importados(as)...<br />";
    }

    /**
     * @param $params
     *
     * @throws Exception
     */
    protected function carregarDados()
    {
        if (empty($this->params['servicos'])) {
            throw new Exception('Nenhum serviço selecionado para importação.');
        }

        try {
            foreach ($this->params['servicos'] as $servico) {
                foreach ($this->obterDados($servico) as $dado) {
                    $this->persistirDados(
                        $dado,
                        $servico
                    );
                }
            }
        } catch (Exception $e) {
            $this->rollback();
            throw $e;
        }
    }

    /**
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function obterDados($servico)
    {
        try {

            $retornoSoap = null;
            do {
                $count = 0;
                $servico['params']['pagina'] = $this->paginaAtual;
                $retornoSoap = call_user_func_array([$this->wsqualitativo,$servico['servico']], $this->parametrosWs($servico));

                if ($retornoSoap instanceof SoapFault) {
                    throw new Exception($retornoSoap);
                }

                $retornoSoap = $retornoSoap->return;

                if ($retornoSoap->mensagensErro) {
                    throw new Exception($retornoSoap->mensagensErro);
                }

                $retorno = $retornoSoap->execucoesOrcamentarias->execucaoOrcamentaria;
                $count += count($retorno);

                yield $retorno;

                $terminate = $this->isUltimaPagina($count, $servico);
            } while (!$terminate);
        } catch (Exception $ex) {
            throw $this->gerarXml(
                $servico['metodo'],
                $retornoSoap,
                $this->params['log'],
                $ex
            );
        }
    }

    private function isUltimaPagina($numRegistro, $servico) {
        if ($numRegistro == $servico['params']['retornoPorPagina']) {
            $this->paginaAtual = $this->paginaAtual+1;
            return false;
        } else {
            return true;
        }
    }
}