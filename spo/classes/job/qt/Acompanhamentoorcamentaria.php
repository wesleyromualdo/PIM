<?php
/**
 * @version $Id$
 * Date: 21/11/2017
 * Time: 10:28
 */

include_once APPRAIZ . "spo/ws/siop/Quantitativo.php";

class Spo_Job_Qt_Acompanhamentoorcamentaria extends Simec_Job_Abstract
{
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
        return 'Baixando Dados de Execução';
    }

    /**
     * @throws Exception
     */
    protected function init()
    {
        // merge parametros recebidos com parametros padrões
        $this->params = $this->loadParams();

        // inicializa a chamada com o SIOP
//        $this->ws = new Spo_Ws_Sof_Quantitativo($this->params['log'], Simec_BasicWS::PRODUCTION);
    }

    /**
     * @throws Exception
     */
    protected function execute()
    {
        try {
            // mostrando mensagem
            echo "Processamento Iniciado.<br>";
            $this->salvarOutput();

            if (key_exists('acao', $this->params) && !empty($this->params['acao']) && is_array($this->params['acao']) && $this->params['acao'][0] != '') {
                $acoes = $this->params['acao'];
            } else {
                $acoes = array_map(function (&$val) {
                    return $val['acacod'];
                }, (new Monitora_Model_Acao())->buscarCodigoPorExercicio($this->params['filtro']['exercicio']));
            }

            $countAcoes = count($acoes);
            $countAcoes = $countAcoes > 1 ? "{$countAcoes} ações" : "{$countAcoes} ação";

            echo "Preparando para carregar dados de {$countAcoes}.<br>";
            $this->salvarOutput();

            $numRegistros = 0;
            $acoesComZero = 0;
            foreach ($acoes as $acao) {
                // busca dados no SIOP
                $retornoSiop = $this->conexaoSiop($acao);

                // contabiliza quantidade de registros
                $count = $this->persistirDados($retornoSiop);
                $numRegistros += $count;
                echo "{$count} registros importados para a ação {$acao}.<br>";
                $this->salvarOutput();
                if (!$count) {
                    $acoesComZero++;
                }
            }

            // commitando toda a informaçao
            $this->commit();

            echo "<br>Inserido no total {$numRegistros} registros.<br>{$acoesComZero} ações não tiveram registros.";
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function shutdown()
    {
    }

    /**
     * @param $acao
     *
     * @return mixed
     * @throws Exception
     */
    private function conexaoSiop($acao)
    {
        try {
//            $retornoSiop = $this->ws->consultarAcompanhamentoOrcamentario($this->montarConsultarAcompanhamentoOrcamentario($acao));

            $retornoSiop = (new Spo_Ws_Siop_Quantitativo())
                ->setSchema('acomporc')
                ->consultarAcompanhamentoOrcamentario(
                    $this->params['filtro']['exercicio'],
                    $this->params['filtro']['periodo'],
                    $this->montarFuncionalProgramatica($acao),
                    $this->params['filtro']['momento']
                );
            sleep(1);

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

            return $retornoSiop->acompanhamentosAcoes;
        } catch (Exception $e) {
            throw $this->gerarXml('consultarAcompanhamentoOrcamentario', $retornoSiop, $this->params['log'], $e);
        }
    }

    /**
     * @param $dadosSiop
     *
     * @return int
     * @throws Exception
     */
    private function persistirDados($dadosSiop)
    {
        $count = 0;
        $dadosSiop = $dadosSiop[0];
        try {
            if (empty($dadosSiop->acompanhamentoAcao))
                return $count;

            if (!is_array($dadosSiop->acompanhamentoAcao))
                $dadosSiop->acompanhamentoAcao = [$dadosSiop->acompanhamentoAcao];

            foreach ($dadosSiop->acompanhamentoAcao as $acao) {
                if (!is_array($acao->acompanhamentosLocalizadores->acompanhamentoLocalizador))
                    $acao->acompanhamentosLocalizadores->acompanhamentoLocalizador = [$acao->acompanhamentosLocalizadores->acompanhamentoLocalizador];

                foreach ($acao->acompanhamentosLocalizadores->acompanhamentoLocalizador as $localizador) {
                    $acaoAux = get_object_vars($acao);
                    unset($acaoAux['acompanhamentosLocalizadores']);

                    $localizadorAux = get_object_vars($localizador);
                    unset($localizadorAux['acompanhamentosPlanoOrcamentario']);
                    unset($localizadorAux['analisesLocalizador']);

                    $acaoAux = array_merge($acaoAux, $localizadorAux);

                    $this->inserirDado($acaoAux, 'acompanhamentoorcamentarioacaodto');

                    if (!is_array($localizador->acompanhamentosPlanoOrcamentario->acompanhamentoPlanoOrcamentario))
                        $localizador->acompanhamentosPlanoOrcamentario->acompanhamentoPlanoOrcamentario = [$localizador->acompanhamentosPlanoOrcamentario->acompanhamentoPlanoOrcamentario];

                    foreach ($localizador->acompanhamentosPlanoOrcamentario->acompanhamentoPlanoOrcamentario as $plano) {
                        $plano = array_merge($acaoAux, get_object_vars($plano));
                        $plano['localizador'] = $localizadorAux['localizador'];
                        unset($plano['periodoOrdem']);
                        unset($plano['justificativa']);
                        unset($plano['comentariosRegionalizacao']);

                        $this->inserirDado($plano, 'acompanhamentoplanoorcamentariodto');

                        $count++;
                    }
                    $count++;
                }
            }

            return $count;
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $dado
     * @param $tabela
     *
     * @throws Exception
     */
    private function inserirDado($dado, $tabela)
    {
        try {
            $sql = $this->montarInsert($dado, $tabela) . "'" . implode("','", $dado) . "')";

            $sql = str_replace("''", "null", $sql);
            $this->executar($sql);
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $acao
     *
     * @return ConsultarAcompanhamentoOrcamentario
     */
    private function montarConsultarAcompanhamentoOrcamentario($acao)
    {
        $consulta = new ConsultarAcompanhamentoOrcamentario();
        $consulta->exercicio = $this->params['filtro']['exercicio'];
        $consulta->codigoMomento = $this->params['filtro']['momento'];
        $consulta->periodoOrdem = $this->params['filtro']['periodo'];

        $consulta->filtro = $this->montarFuncionalProgramatica($acao);

        return $consulta;
    }

    /**
     * @param $acao
     *
     * @return FiltroFuncionalProgramaticaDTO
     */
    private function montarFuncionalProgramatica($acao)
    {
        $filtro = new FiltroFuncionalProgramaticaDTO();
        $filtro->exercicio = $this->params['filtro']['exercicio'];
        $filtro->codigoAcao = $acao;

        return $filtro;
    }

    /**
     * @param $colunas
     * @param $tabela
     */
    private function montarInsert($colunas, $tabela)
    {
        $colunas = implode(', ', array_keys($colunas));

        return <<<SQL
INSERT INTO {$this->nomeTabela($this->params['schemaDb'], $this->params['prefixoTabela'], $tabela)}
({$colunas}) VALUES (
SQL;
    }

}