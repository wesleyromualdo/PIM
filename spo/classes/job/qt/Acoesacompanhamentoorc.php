<?php

include_once APPRAIZ . "spo/ws/siop/Quantitativo.php";

/**
 * @version $Id: Programacaocompleta.php 134119 2017-11-20 18:03:39Z jefersonaraujo $
 * Date: 01/11/2017
 * Time: 16:57
 */
class Spo_Job_Qt_Acoesacompanhamentoorc extends Simec_Job_Abstract
{
    /**
     * @var array
     */
    private $mapa;

    /**
     * @var Spo_Ws_Sof_Quantitativo
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
        return 'Baixando Localizadores com Acompanhamento ' . ($_SESSION['job_acomporc']['tipoCarga'] == 1 || $_SESSION['job_acomporc']['tipoCarga'] == 3 ? 'Obrigatório' : 'Opcional');
    }

    /**
     * @throws Exception
     */
    protected function init()
    {
        $this->params = $this->loadParams();

        $this->nomeTabela($this->params['schemaDb'], $this->params['prefixoTabela'], 'acoesacompanhamentoorcamentariodto');

//        $this->ws = new Spo_Ws_Sof_Quantitativo($this->params['log'], Simec_BasicWS::PRODUCTION);
    }

    /**
     * @throws Exception
     */
    protected function execute()
    {
        echo "Processamento iniciado.<br>";
        $this->salvarOutput();
        try {
            $count = $this->persistirDados($this->conexaoSiop());

            echo "<br>{$count} Ações importadas.<br>";
        } catch (Exception $e) {
            $this->rollback();

            throw new Exception("Ocorreu um problema ao importar os registros<br>{$e->getMessage()}");
        }
    }


    protected function shutdown()
    {
    }

    /**
     * Coenxao com Siop e retorno dos dados
     *
     * @return mixed
     * @throws Exception
     */
    private function conexaoSiop()
    {
        try {
//            $retornoSoap = $this->ws->obterAcoesDisponiveisAcompanhamentoOrcamentario($this->montarAcoesAcompOrcamentario());
            $retornoSoap = (new Spo_Ws_Siop_Quantitativo())
                ->setSchema('acomporc')
                ->obterAcoesDisponiveisAcompanhamentoOrcamentario(
                    $this->params['exercicio'],
                    $this->params['periodo']
                );


            if ($retornoSoap instanceof SoapFault) {
                throw new Exception($retornoSoap);
            }

            if ($retornoSoap->mensagensErro) {
                throw new Exception($retornoSoap->mensagensErro);
            }

            $retornoSoap = $retornoSoap->return;
        } catch (Exception $ex) {
            throw $this->gerarXml('obterAcoesDisponiveisAcompanhamentoOrcamentario', $retornoSoap, $this->params['log'], $ex);
        }

        return $retornoSoap;
    }

    /**
     * Deleta informações da tabela temporaria e inseri novos registros
     *
     * @param $resultados
     * @param $servico
     */
    private function persistirDados($result)
    {
        $count = 0;

        foreach ($result->acoes[0]->acao as $acao) {
            if (is_array($acao->localizadores->localizador)) {
                foreach ($acao->localizadores->localizador as $localizador) {
                    $this->insertAcoesAcompanhamento($acao, $localizador);
                }
            } else {
                $this->insertAcoesAcompanhamento($acao, $acao->localizadores->localizador);
            }

            $count++;
        }

        $this->commit();

        return $count;
    }

    /**
     * @return ObterAcoesDisponiveisAcompanhamentoOrcamentario
     */
    private function montarAcoesAcompOrcamentario()
    {
        $this->getPeriodoSiop();

        $obterAcoesAcompOrcamentario = new ObterAcoesDisponiveisAcompanhamentoOrcamentario();
        $obterAcoesAcompOrcamentario->exercicio = $this->params['exercicio'];
        $obterAcoesAcompOrcamentario->periodo = $this->params['periodo'];

        return $obterAcoesAcompOrcamentario;
    }

    private function getPeriodoSiop()
    {
        $periodo = new Acomporc_Model_PeriodoReferencia($this->params['periodo']);
        $this->params['periodo'] = $periodo->prfperiodo;
    }

    /**
     * @param $acao
     * @param $localizador
     *
     * @throws Exception
     */
    private function insertAcoesAcompanhamento($acao, $localizador)
    {
        try {
            $localizador->snAcompanhamentoOpcional = is_null($localizador->snAcompanhamentoOpcional) ? '' : ($localizador->snAcompanhamentoOpcional ? 't' : 'f');
            $data = date('Y-m-d H:i:s');

            $sql = <<<SQL
INSERT INTO {$this->getTable()}(codigoacao,codigoprograma,codigofuncao,codigosubfuncao,codigoorgao,
                                codigoesfera,localizadores,dataultimaatualizacao,snacompanhamentoopcional)
  VALUES('{$acao->codigoAcao}',
         '{$acao->codigoPrograma}',
         '{$acao->codigoFuncao}',
         '{$acao->codigoSubFuncao}',
         '{$acao->codigoOrgao}',
         '{$acao->codigoEsfera}',
         '{$localizador->codigoLocalizador}',
         '{$data}',
         '{$localizador->snAcompanhamentoOpcional}')
SQL;

            $this->executar($sql);
            $this->commit();
        } catch (Exception $e) {
            throw $e;
        }
    }
}