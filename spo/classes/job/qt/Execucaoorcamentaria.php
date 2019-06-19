<?php
/**
 * @version $Id$
 * Date: 21/11/2017
 * Time: 10:28
 */

class Spo_Job_Qt_Execucaoorcamentaria extends Simec_Job_Abstract
{
    const TAMANHO_PAGINA = Spo_Ws_Sof_Quantitativo::REGISTROS_POR_PAGINA;

    /**
     * @var Spo_Ws_Sof_Quantitativo
     */
    private $ws;

    /**
     * @var array
     */
    private $params;

    /**
     * @var array
     */
    private $infoBloco;

    /**
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public function getName()
    {
        return 'Baixando Dados de Execução Orçamentária';
    }

    protected function init()
    {
        // merge parametros recebidos com parametros padrões
        $this->params = array_merge(
            [
                'pagina' => '0'
            ],
            $this->loadParams()
        );

        // seta esquema e nome da tabela com base em parametros
        $this->nomeTabela($this->params['schemaDb'], $this->params['prefixoTabela'], 'execucaoorcamentariadto');

        // inicializa a chamada com o SIOP
        $this->ws = new Spo_Ws_Sof_Quantitativo($this->params['log'], Simec_BasicWS::PRODUCTION);
    }

    protected function execute()
    {
        // mostrando mensagem
        $tamanhoPagina = self::TAMANHO_PAGINA;
        echo "Processamento de carga por bloco de {$tamanhoPagina} registros.<br>";
        $this->salvarOutput();

        // contador de registros
        $count = 0;

        // bloco de informacoes utilizados para identificar paginas
        $this->infoBloco = [
            'pagina'   => $this->params['pagina'],
            'metodo'   => $this->params['metodo'],
            'mesAtual' => $this->params['mesAtual']
        ];

        do {
            // mostrando mensagem
            $bloco = $this->infoBloco['pagina'] + 1;
            echo "Processando o bloco {$bloco}.<br>";
            $this->salvarOutput();

            // busca dados no SIOP
            $retornoSiop = $this->conexaoSiop();

            // processando o infoBloco da consulta
            $this->persistirDados($retornoSiop);

            // contabiliza quantidade de registros
            $numRegistros = count($retornoSiop);
            $count += $numRegistros;

            // verifica se possui proxima pagina
            $this->infoBloco = $this->isUltimaPagina($numRegistros);
        } while (!$this->infoBloco['terminate']);

        // commitando toda a informaçao
        $this->commit();

        echo "<br>Inserido no total {$count} registros.";
    }

    protected function shutdown()
    {
    }

    private function conexaoSiop()
    {
        try {
            $retornoSiop = $this->ws->consultarExecucaoOrcamentaria(
                $this->params['filtro'],
                $this->params['selecaoretorno'],
                $this->infoBloco['pagina'],
                false,
                $this->infoBloco['metodo']
            );

            // verifica SoupFault
            if ($retornoSiop instanceof SoapFault) {
                throw new Exception($retornoSiop->getMessage());
            }

            // traz o resultado
            $retornoSiop = $retornoSiop->return;

            // Falha ao executar a consulta
            if ($retornoSiop->mensagensErro) {
                if (is_array($retornoSiop->mensagensErro)) {
                    throw new Exception(implode('<br>', $retornoSiop->mensagensErro));
                }

                throw new Exception($retornoSiop->mensagensErro);
            }

            return $retornoSiop->execucoesOrcamentarias->execucaoOrcamentaria;
        } catch (Exception $e) {
            throw $this->gerarXml('consultarExecucaoOrcamentaria', $retornoSiop, $this->params['log'], $e);
        }
    }

    /**
     * @param $numRegistros
     *
     * @return array
     */
    private function isUltimaPagina($numRegistros)
    {
        if ($numRegistros == self::TAMANHO_PAGINA) {
            return [
                'terminate' => false,
                'pagina'    => $this->infoBloco['pagina'] + 1,
                'mesAtual'  => $this->infoBloco['mesAtual'],
                'metodo'    => $this->infoBloco['metodo']
            ];

        } elseif (
            $this->params['filtro']['mes'] == '1-6'
            || $this->params['filtro']['mes'] == '7-12'
        ) {
            if ($this->infoBloco['mesAtual'] == '99') {

                return [
                    'terminate' => true
                ];
            }

            if (
                $this->infoBloco['mesAtual'] == 6
                || $this->infoBloco['mesAtual'] == 12
                || $this->params['filtro']['mes'] == ''
            ) {

                return [
                    'terminate' => false,
                    'pagina'    => 0,
                    'mesAtual'  => 99,
                    'metodo'    => 'consultarExecucaoOrcamentaria'
                ];

            } else {

                return [
                    'terminate' => false,
                    'pagina'    => 0,
                    'mesAtual'  => $this->infoBloco['mesAtual'] + 1,
                    'metodo'    => 'consultarExecucaoOrcamentariaMensal'
                ];
            }

        } else {

            return [
                'terminate' => true
            ];

        }
    }

    /**
     * @param $dadosSiop
     *
     * @throws Exception
     */
    private function persistirDados($dadosSiop)
    {
        try {
            if (count($dadosSiop)) {
                $insert = $this->gerarInsert();

                foreach ($dadosSiop as $execucao) {
                    $valores = [];
                    foreach ($this->params['selecaoretorno'] as $campo) {
                        $valores[] = "'{$execucao->$campo}'";
                    }

                    if (!empty($this->infoBloco['mesAtual']) && $this->infoBloco['mesAtual'] != 99) {
                        array_pop($valores);
                        $valores[] = $this->infoBloco['mesAtual'];
                    }

                    $this->executar($insert . implode(', ', $valores) . ')');
                }
            }
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * Gewrador de insert, baseado no mesAtual
     *
     * @return string
     */
    private function gerarInsert()
    {
        if (!empty($this->infoBloco['mesAtual']) && $this->infoBloco['mesAtual'] != 99) {
            $this->params['selecaoretorno'][] = 'mes';
        }

        return "INSERT INTO {$this->getTable()} (" . implode(', ', $this->params['selecaoretorno']) . ') VALUES (';
    }
}