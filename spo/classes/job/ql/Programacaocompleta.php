<?php

/**
 * @version $Id: Programacaocompleta.php 134663 2017-11-28 18:21:10Z saulocorreia $
 * Date: 01/11/2017
 * Time: 16:57
 */
class Spo_Job_Ql_Programacaocompleta extends Simec_Job_Abstract
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
        return 'Carga de Programação Completa - Qualitativo';
    }

    protected function init()
    {
        $this->mapa = [
            'Ações'                             => ['ws' => 'retornarAcoes', 'dto' => 'acoesDTO'],
            'Agendas Sam'                       => ['ws' => 'retornarAgendaSam', 'dto' => 'agendasSamDTO'],
            'Indicadores'                       => ['ws' => 'retornarIndicadores', 'dto' => 'indicadoresDTO'],
            'Iniciativas'                       => ['ws' => 'retornarIniciativas', 'dto' => 'iniciativasDTO'],
            'Localizadores'                     => ['ws' => 'retornarLocalizadores', 'dto' => 'localizadoresDTO'],
            'Medidas Institucionais Normativas' => ['ws' => 'retornarMedidasInstitucionaisNormativas', 'dto' => 'medidasInstitucionaisNormativasDTO'],
            'Metas'                             => ['ws' => 'retornarMetas', 'dto' => 'metasDTO'],
            'Objetivos'                         => ['ws' => 'retornarObjetivos', 'dto' => 'objetivosDTO'],
            'Orgãos'                            => ['ws' => 'retornarOrgaos', 'dto' => 'orgaosDTO'],
            'Planos Orçamentarios'              => ['ws' => 'retornarPlanosOrcamentarios', 'dto' => 'planosOrcamentariosDTO'],
            'Programas'                         => ['ws' => 'retornarProgramas', 'dto' => 'programasDTO'],
            'Regionilizações'                   => ['ws' => 'retornarRegionalizacoes', 'dto' => 'regionalizacoesDTO'],

        ];

        $this->params = array_merge(
            [
                'execucao' => 'cadastrar',
                'momento'  => '9000'
            ],
            $this->loadParams());

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
     * Conexao com Siop e retorno dos dados
     *
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function conexaoSiop($informacoes)
    {
        try {
            $retornoSoap = $this->ws->obterProgramacaoCompleta($this->montarProgramacaoCompleta($informacoes));

            if ($retornoSoap instanceof SoapFault) {
                throw new Exception($retornoSoap);
            }

            if ($retornoSoap->mensagensErro) {
                throw new Exception($retornoSoap->mensagensErro);
            }

            $retornoSoap = $retornoSoap->return;
        } catch (Exception $ex) {
            throw $this->gerarXml('obterProgramacaoCompleta', $retornoSoap, $this->params['log'], $ex);
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
        $this->nomeTabela($this->params['schemaDb'], $this->params['prefixoTabela'], $informacoes['dto']);
    }

    /**
     * @param $informacoes
     *
     * @return ObterProgramacaoCompleta
     */
    private function montarProgramacaoCompleta($informacoes)
    {
        $obterProgramacaoCompleta = new ObterProgramacaoCompleta();
        $obterProgramacaoCompleta->exercicio = $this->params['exercicio'];
        $obterProgramacaoCompleta->codigoMomento = $this->params['momento'];
        $obterProgramacaoCompleta->$informacoes['ws'] = true;

        return $obterProgramacaoCompleta;
    }

}