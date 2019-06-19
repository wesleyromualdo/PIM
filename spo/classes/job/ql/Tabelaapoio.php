<?php

/**
 * @version $Id: Tabelaapoio.php 134663 2017-11-28 18:21:10Z saulocorreia $
 * Class Spo_Job_Ql_TabelaApoio
 */
class Spo_Job_Ql_Tabelaapoio extends Simec_Job_Abstract
{
    /**
     * @var array
     */
    private $mapa;

    /**
     * @var Spo_Ws_Sof_Qualitativo
     */
    private $ws;

    /**
     * @var array
     */
    private $params;

    /**
     * @return string
     */
    public function getName()
    {
        return 'Baixando dados Qualitativos';
    }

    protected function init()
    {
        $this->mapa = [
            'Esferas'            => ['ws' => 'retornarEsferas', 'dto' => 'esferasDTO'],
            'Funções'            => ['ws' => 'retonarFuncoes', 'dto' => 'funcoesDTO'],
            'Momentos'           => ['ws' => 'retornarMomentos', 'dto' => 'momentosDTO'],
            'Perfis'             => ['ws' => 'retornarPerfis', 'dto' => 'perfisDTO'],
            'Produtos'           => ['ws' => 'retornarProdutos', 'dto' => 'produtosDTO'],
            'Subfunções'         => ['ws' => 'retornarSubFuncoes', 'dto' => 'subFuncoesDTO'],
            'Regiões'            => ['ws' => 'retornarRegioes', 'dto' => 'regioesDTO'],
            'Unidades de medida' => ['ws' => 'retornarUnidadesMedida', 'dto' => 'unidadesMedidaDTO'],
            'Tipos de ações'     => ['ws' => 'retornarTiposAcao', 'dto' => 'tiposAcaoDTO'],
            'Tipos de inclusão'  => ['ws' => 'retornarTiposInclusao', 'dto' => 'tiposInclusaoDTO'],
            'Tipos de programa'  => ['ws' => 'retornarTiposPrograma', 'dto' => 'tiposProgramaDTO']
        ];

        $this->params = $this->loadParams();

        $this->ws = new Spo_Ws_Sof_Qualitativo($this->params['log'], Simec_BasicWS::PRODUCTION);
    }

    protected function execute()
    {
        echo "Processamento iniciado.<br>";
        $this->salvarOutput();


        if (empty($this->params['servicos'])) {
            throw new Exception('Nenhum serviço selecionado para importação.');
        } else {
            foreach ($this->params['servicos'] as $servico) {
                try {
                    $info = $this->mapa[$servico];

                    $this->montarNomeTabela($info);

                    $this->persistirDados($this->conexaoSiop($info), $servico);
                } catch (Exception $e) {
                    $this->rollback();

                    throw new Exception("Ocorreu um problema ao importar os registros<br>{$e->getMessage()}");
                }
            }
        }
    }

    protected function shutdown()
    {
    }

    /**
     * Coenxao com Siop e retorno dos dados
     *
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function conexaoSiop($informacoes)
    {
        try {
            $retornoSoap = $this->ws->obterTabelasApoio($this->montarObterTabelaApoio($informacoes));

            if ($retornoSoap instanceof SoapFault) {
                throw new Exception($retornoSoap);
            }

            $retornoSoap = $retornoSoap->return;

            if ($retornoSoap->mensagensErro) {
                throw new Exception($retornoSoap->mensagensErro);
            }
        } catch (Exception $ex) {
            throw $this->gerarXml('obterTabelasApoio', $retornoSoap, $this->params['log'], $ex);
        }

        return $retornoSoap->$informacoes['dto'];
    }

    /**
     * Deleta informações da tabela temporaria e inseri novos registros
     *
     * @param $resultados
     * @param $servico
     */
    private function persistirDados($resultados, $servico)
    {
        if ($resultados) {
            $count = $this->jobInsert($resultados, true);

            echo "{$count} {$servico} importados.<br>";

            $this->salvarOutput();

            $this->commit();
        }
    }

    /**
     * Gera a nome da tabela para conexao e persistencia
     *
     * @param $informacoes
     */
    private function montarNomeTabela($informacoes)
    {
        $this->nomeTabela($this->params['schemaDb'], $this->params['prefixoTabela'], strtolower($informacoes['dto']));
    }

    /**
     * @param $informacoes
     *
     * @return ObterTabelasApoio
     */
    private function montarObterTabelaApoio($informacoes)
    {
        $obterTabelasApoio = new ObterTabelasApoio();
        $obterTabelasApoio->exercicio = $this->params['exercicio'];
        $obterTabelasApoio->$informacoes['ws'] = true;

        return $obterTabelasApoio;
    }
}
