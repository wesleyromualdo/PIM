<?php
/**
 * Created by PhpStorm.
 * User: saulocorreia
 * Date: 21/11/2017
 * Time: 16:11
 */

include_once APPRAIZ . "spo/ws/siop/Qualitativo.php";
include_once APPRAIZ . 'acomporc/classes/model/Job_acoesdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_esferasdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_funcoesdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_iniciativasdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_localizadoresdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_metasdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_objetivosdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_planosorcamentariosdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_produtosdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_programasdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_subfuncoesdto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_tiposacaodto.php';
include_once APPRAIZ . 'acomporc/classes/model/Job_unidadesmedidadto.php';

class Spo_Job_Ql_Tabelaapoioprogramacaocompleta extends Simec_Job_Abstract
{
    const TABELA_APOIO = 'obterTabelasApoio';
    const PROGRAMACAO_COMPLETA = 'obterProgramacaoCompleta';

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
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public function getName()
    {
        return 'Baixando dados Qualitativos';
    }

    protected function init()
    {
        $this->mapa = [
            'Ações'                             => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarAcoes', 'dto' => 'acoesDTO', 'model' => new Acomporc_Model_Job_acoesdto()],
            'Agendas Sam'                       => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarAgendaSam', 'dto' => 'agendasSamDTO', 'model' => null],
            'Esferas'                           => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarEsferas', 'dto' => 'esferasDTO', 'model' => new Acomporc_Model_Job_esferasdto()],
            'Funções'                           => ['metodo' => self::TABELA_APOIO, 'ws' => 'retonarFuncoes', 'dto' => 'funcoesDTO', 'model' => new Acomporc_Model_Job_funcoesdto()],
            'Indicadores'                       => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarIndicadores', 'dto' => 'indicadoresDTO', 'model' => null],
            'Iniciativas'                       => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarIniciativas', 'dto' => 'iniciativasDTO', 'model' => new Acomporc_Model_Job_iniciativasdto()],
            'Localizadores'                     => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarLocalizadores', 'dto' => 'localizadoresDTO', 'model' => new Acomporc_Model_Job_localizadoresdto()],
            'Medidas Institucionais Normativas' => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarMedidasInstitucionaisNormativas', 'dto' => 'medidasInstitucionaisNormativasDTO', 'model' => null],
            'Metas'                             => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarMetas', 'dto' => 'metasDTO', 'model' => new Acomporc_Model_Job_metasdto()],
            'Momentos'                          => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarMomentos', 'dto' => 'momentosDTO', 'model' => null],
            'Objetivos'                         => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarObjetivos', 'dto' => 'objetivosDTO', 'model' => new Acomporc_Model_Job_objetivosdto()],
            'Orgãos'                            => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarOrgaos', 'dto' => 'orgaosDTO', 'model' => null],
            'Perfis'                            => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarPerfis', 'dto' => 'perfisDTO', 'model' => null],
            'Planos Orçamentarios'              => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarPlanosOrcamentarios', 'dto' => 'planosOrcamentariosDTO', 'model' => new Acomporc_Model_Job_planosorcamentariosdto()],
            'Produtos'                          => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarProdutos', 'dto' => 'produtosDTO', 'model' => new Acomporc_Model_Job_produtosdto()],
            'Programas'                         => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarProgramas', 'dto' => 'programasDTO', 'model' => new Acomporc_Model_Job_programasdto()],
            'Subfunções'                        => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarSubFuncoes', 'dto' => 'subFuncoesDTO', 'model' => new Acomporc_Model_Job_subfuncoesdto()],
            'Regiões'                           => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarRegioes', 'dto' => 'regioesDTO', 'model' => null],
            'Regionilizações'                   => ['metodo' => self::PROGRAMACAO_COMPLETA, 'ws' => 'retornarRegionalizacoes', 'dto' => 'regionalizacoesDTO', 'model' => null],
            'Unidades de medida'                => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarUnidadesMedida', 'dto' => 'unidadesMedidaDTO', 'model' => new Acomporc_Model_Job_unidadesmedidadto()],
            'Tipos de ações'                    => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarTiposAcao', 'dto' => 'tiposAcaoDTO', 'model' => new Acomporc_Model_Job_tiposacaodto()],
            'Tipos de inclusão'                 => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarTiposInclusao', 'dto' => 'tiposInclusaoDTO', 'model' => null],
            'Tipos de programa'                 => ['metodo' => self::TABELA_APOIO, 'ws' => 'retornarTiposPrograma', 'dto' => 'tiposProgramaDTO', 'model' => null],
        ];


        // Programacao Completa
        $this->params = $this->loadParams();
//        $this->params = array_merge(
//            [
//                'momento' => '9000'
//            ],
//            $this->loadParams()
//        );

//        $this->ws = new Spo_Ws_Sof_Qualitativo($this->params['log'], Simec_BasicWS::PRODUCTION);
    }

    protected function execute()
    {
        try {
            echo "Processamento iniciado.<br><br>";
            $this->salvarOutput();

            $this->carregarDados($this->params);

            $this->commit();
        } catch (Exception $e) {
            throw  $e;
        }
    }

    protected function shutdown()
    {
    }

    /**
     * @param $params
     *
     * @throws Exception
     */
    protected function carregarDados($params)
    {
        if (empty($params['servicos'])) {
            throw new Exception('Nenhum serviço selecionado para importação.');
        }

        try {
            foreach ($params['servicos'] as $servico) {
                $info = $this->mapa[$servico];

                $this->montarNomeTabela($params, $info);

                $this->persistirDados($this->conexaoSiop($info), $servico, $info);
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
    private function conexaoSiop($informacoes)
    {
        try {
            $retornoSoap = $this->requisicaoSiop($informacoes);
        } catch (Exception $ex) {
            throw $ex;
        }

        return $retornoSoap->$informacoes['dto'];
    }


    /**
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function requisicaoSiop($informacoes)
    {
        switch ($informacoes['metodo']) {
            case self::TABELA_APOIO :
                $retornoSoap = $this->conexaoSiopTabelaApoio($informacoes);
                break;
            case self::PROGRAMACAO_COMPLETA :
                $retornoSoap = $this->conexaoSiopProgramacaoCompleta($informacoes);
                break;
            default :
                throw new Exception("Chamada não encontrada.");
        }

        return $retornoSoap;
    }

    /**
     * @param $informacoes
     * @param $retornoSoap
     *
     * @return mixed
     * @throws Exception
     */
    private function tratarRetornoSiop($informacoes, $retornoSoap)
    {
        try {
            if ($retornoSoap instanceof SoapFault) {
                throw new Exception($retornoSoap);
            }

            $retornoSoap = $retornoSoap->return;

            if ($retornoSoap->mensagensErro) {
                throw new Exception($retornoSoap->mensagensErro);
            }

            return $retornoSoap;
        } catch (Exception $e) {
            throw $this->gerarXml($informacoes['metodo'], $retornoSoap, $this->params['log'], $e);
        }
    }

    /**
     * Conexao com Siop e retorno dos dados
     *
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function conexaoSiopTabelaApoio($informacoes)
    {
        try {
            return $this->tratarRetornoSiop(
                $informacoes,
                (new Spo_Ws_Siop_Qualitativo())
                    ->setListasRetornadas([$informacoes['ws']])
                    ->obterTabelasApoio($this->params['exercicio'])
                );
        } catch (Exception $ex) {
            throw  $ex;
        }
    }

    /**
     * Conexao com Siop e retorno dos dados
     *
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function conexaoSiopProgramacaoCompleta($informacoes)
    {
        try {
            return $this->tratarRetornoSiop(
                $informacoes,
                (new Spo_Ws_Siop_Qualitativo())
                    ->setListasRetornadas([$informacoes['ws']])
                    ->obterProgramacaoCompleta($this->params['exercicio'], $this->params['momento'])
            );
        } catch (Exception $ex) {
            throw  $ex;
        }
    }

    /**
     * Deleta informações da tabela temporaria e inseri novos registros
     *
     * @param $resultados
     * @param $servico
     */
    private function persistirDados($resultados, $servico, $info){
        if (!$info['model']){
            $this->persistirDadosJobModel($resultados, $servico);
        } else {
            $this->jobDelete([], true);

            $model = $info['model'];
            $count = 0;
            foreach ($resultados as $dado) {
                $dado = simec_utf8_decode_recursive(array_change_key_case(get_object_vars($dado), CASE_LOWER));
                $model->popularDadosObjeto($dado);
                $model->salvar();
                $count++;
                $model = $info['model'];
            }

            $model->commit();

            echo "{$count} {$servico} importados.<br>";

            $this->salvarOutput();
        }
    }

    private function persistirDadosJobModel($resultados, $servico){
        if ($resultados) {
            $this->jobDelete([], true);

            $count = $this->jobInsert($resultados, true);

            echo "{$count} {$servico} importados.<br>";

            $this->salvarOutput();
        }
    }

    /**
     * Gera a nome da tabela para conexao e persistencia
     *
     * @param $informacoes
     * @param $params
     */
    private function montarNomeTabela($params, $informacoes)
    {
        $this->nomeTabela($params['schemaDb'], $params['prefixoTabela'], strtolower($informacoes['dto']));
    }

    /**
     * @param $informacoes
     *
     * @return SimpleXMLElement
     */
    private function montarObterTabelaApoio($informacoes)
    {
//        $obterTabelasApoio = new ObterTabelasApoio();
//        $obterTabelasApoio->exercicio = $this->params['exercicio'];
//        $obterTabelasApoio->$informacoes['ws'] = true;

        $obterTabelasApoio = (new Spo_Ws_Siop_Qualitativo())
            ->setListasRetornadas([$informacoes['ws']])
            ->obterTabelasApoio($this->params['exercicio'])
        ;

        return $obterTabelasApoio;
    }


    /**
     * @param $informacoes
     *
     * @return SimpleXMLElement
     */
    private function montarProgramacaoCompleta($informacoes)
    {
//        $obterProgramacaoCompleta = new ObterProgramacaoCompleta();
//        $obterProgramacaoCompleta->exercicio = $this->params['exercicio'];
//        $obterProgramacaoCompleta->codigoMomento = $this->params['momento'];
//        $obterProgramacaoCompleta->$informacoes['ws'] = true;

        $obterProgramacaoCompleta = (new Spo_Ws_Siop_Qualitativo())
            ->setListasRetornadas([$informacoes['ws']])
            ->obterProgramacaoCompleta($this->params['exercicio'], $this->params['momento'])
        ;

        return $obterProgramacaoCompleta;
    }
}