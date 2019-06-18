<?php

include_once APPRAIZ . "spo/ws/sof/QuantitativoMap.php";
include_once APPRAIZ . "includes/siop-soap-curl/siop-soap-curl.php";
include_once APPRAIZ . "spo/classes/model/Logws.php";

class Spo_Ws_Siop_Quantitativo
{
    private $service;

    const PRODUCAO = 1;
    const DESENVOLVIMENTO = 2;

    protected $schema;

    public function __construct()
    {
        $ambiente = IS_PRODUCAO ? self::PRODUCAO : self::DESENVOLVIMENTO;
        $this->setSchema();
        $this->service = new SiopSoapCurl_Quantitativo_Service($ambiente);
    }

    /**
     * Retorna o resultado do serviço obterProgramacaoCompletaQuantitativo do SIOP
     *
     * @param $exercicio
     * @param $codigoMomento
     * @param null $dataHoraReferencia
     * @return string
     */
    public function obterProgramacaoCompletaQuantitativo($exercicio, $codigoMomento, $dataHoraReferencia = null)
    {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('obterProgramacaoCompletaQuantitativo')
            ->setExercicio($exercicio)
            ->setCodigoMomento($codigoMomento);

        if (!empty($dataHoraReferencia)) {
            $this->service->getXml()
                ->setDataHoraReferencia($dataHoraReferencia);
        }

        $retorno = $this->service->configure()->request();
        $this->salvaLog();
        return $retorno;
    }

    /**
     * Retorna os dados do serviço obterAcoesDisponiveisAcompanhamentoOrcamentario do SIOP
     *
     * @param $exercicio
     * @param $periodo
     * @param null $snAcompanhamentoOpcionalAcao
     * @param null $snAcompanhamentoOpcionalLocalizador
     * @return array
     */
    public function obterAcoesDisponiveisAcompanhamentoOrcamentario($exercicio, $periodo, $snAcompanhamentoOpcionalAcao = null, $snAcompanhamentoOpcionalLocalizador = null)
    {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('obterAcoesDisponiveisAcompanhamentoOrcamentario')
            ->setExercicio($exercicio)
            ->setPeriodo($periodo);

        if (!empty($snAcompanhamentoOpcionalAcao)) {
            $this->service->getXml()
                ->setSnAcompanhamentoOpcionalAcao($snAcompanhamentoOpcionalAcao);
        }

        if (!empty($snAcompanhamentoOpcionalLocalizador)) {
            $this->service->getXml()
                ->setSnAcompanhamentoOpcionalLocalizador($snAcompanhamentoOpcionalLocalizador);
        }

        $retorno = $this->service->configure()->request();
        $this->salvaLog();
        return $retorno;
    }

    public function consultarAcompanhamentoOrcamentario($exercicio, $periodoOrdem, FiltroFuncionalProgramaticaDTO $filtro, $codigoMomento = null, $dataHoraReferencia = null)
    {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('consultarAcompanhamentoOrcamentario')
            ->setExercicio($exercicio)
            ->setPeriodoOrdem($periodoOrdem)
            ->setCodigoMomento($codigoMomento)
            ->setListField([])
            ->setListFilter($this->objetoToArray($filtro));

        if (!empty($dataHoraReferencia)) {
            $this->service->getXml()
                ->setDataHoraReferencia($dataHoraReferencia);
        }

        $retorno = $this->service->configure()->request();
        $this->salvaLog();
        return $retorno;
    }

    public function consultarExecucaoOrcamentaria(FiltroExecucaoOrcamentariaDTO $filtro, SelecaoRetornoExecucaoOrcamentariaDTO $selecaoRetorno = null, PaginacaoDTO $paginacao = null, $log = true)
    {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('consultarExecucaoOrcamentaria')
            ->setListFilter($this->objetoToArray($filtro));

        if ($selecaoRetorno) {
            $this->service->getXml()
                ->setListField($this->objetoToArray($selecaoRetorno));
        }

        if ($paginacao) {
            $this->service->getXml()
                ->setPaginacao($paginacao->registrosPorPagina)
                ->setPage($paginacao->pagina);
        }

        $retorno = $this->service->configure()->request();
        if ($log)
            $this->salvaLog();

        return $retorno;
    }

    protected function objetoToArray($filtro)
    {
        $campos = json_decode(json_encode($filtro), true);
        $arr = [];

        foreach ($campos as $campo => $valor) {
            if (!empty($valor)) {
                if ($valor === true) {
                    $valor = 'true';
                }
                $arr[$campo] = $valor;
            }
        }

        return $arr;
    }

    /**
     * Salva o LOG do serviço
     */
    protected function salvaLog()
    {
        $model = new Spo_Ws_Siop_Model_Logws($this->schema);
        $model->lwsrequestcontent = $this->service->getClient()->getXml();
        $model->lwsrequesttimestamp = date('Y-m-d H:i:s');
        $model->lwsmetodo = $this->getService();
        $model->lwsurl = $this->service->getUrl();
        $model->lwsresponsecontent = $this->service->getClient()->getResponse();
//        $model->lwserro = $this->service->getClient()->getError() === false ? 'true' : 'false';
        $model->salvar();
        $model->commit();
    }

    public function setSchema($schema = null)
    {
        $this->schema = empty($schema) ? $_SESSION['sisdiretorio'] : $schema;
        return $this;
    }

    public function getService()
    {
        return $this->service->getXml()->getService();
    }

    public function getExercicio()
    {
        return $this->service->getXml()->getExercicio();
    }

    public function getCodigoMomento()
    {
        return $this->service->getXml()->getCodigoMomento();
    }

    public function getDataHoraReferencia()
    {
        return $this->service->getXml()->getDataHoraReferencia();
    }

}