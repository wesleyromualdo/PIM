<?php

include_once APPRAIZ . "includes/siop-soap-curl/siop-soap-curl.php";
include_once APPRAIZ . "spo/classes/model/Logws.php";

class Spo_Ws_Siop_Qualitativo {
    private $service;

    const PRODUCAO = 1;
    const DESENVOLVIMENTO = 2;

    public function __construct(){
        $ambiente = IS_PRODUCAO ? self::PRODUCAO : self::DESENVOLVIMENTO;
        $this->service = new SiopSoapCurl_Qualitativo_Service_ProgramacaoCompleta($ambiente);
    }

    /**
     * Seta as listas que serão retornadas pelo serviço
     *
     * @param $setListasRetornadas
     * @return $this
     */
    public function setListasRetornadas($setListasRetornadas) {
        $setListasRetornadas = is_array($setListasRetornadas) ? $setListasRetornadas : [$setListasRetornadas];
        $this->service->getXml()->setListasRetornadas($setListasRetornadas);
        return $this;
    }

    protected function trataRetorno($retorno) {
        $service = strtolower($this->getService().'response');
//        $resultado = simplexml_load_string(str_ireplace(['env:'-, 'ns2:', 'sof:'], '', $retorno->response));
        $this->salvaLog($retorno);
        return $retorno->body->$service;
    }


    protected function salvaLog() {
        $model = new Spo_Ws_Siop_Model_Logws('acomporc');
        $model->lwsrequestcontent = $this->service->getClient()->getXml();
        $model->lwsrequesttimestamp = date('Y-m-d H:i:s');
        $model->lwsmetodo = $this->getService();
        $model->lwsurl = $this->service->getUrl();
        $model->lwsresponsecontent = $this->service->getClient()->getResponse();
//        $model->lwserro = $this->service->getClient()->getError() === false ? 'true' : 'false';
        $model->salvar();
        $model->commit();
    }

    /**
     * Retorna o resultado do serviço obterAcaoPorIdentificadorUnico
     *
     * @param $exercicio
     * @param $codigoMomento
     * @return SimpleXMLElement
     */
    public function obterAcaoPorIdentificadorUnico($exercicio, $codigoMomento) {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('obterAcaoPorIdentificadorUnico')
            ->setExercicio($exercicio)
            ->setCodigoMomento($codigoMomento);

        return $this->service->configure()->request();
    }

    /**
     * Retorna o resultado do serviço obterAcoesPorIniciativa
     *
     * @param $exercicio
     * @param $codigoPrograma
     * @param $codigoObjetivo
     * @param $codigoIniciativa
     * @param $codigoMomento
     * @param null $dataHoraReferencia
     * @return SimpleXMLElement
     */
    public function obterAcoesPorIniciativa($exercicio, $codigoPrograma, $codigoObjetivo, $codigoIniciativa, $codigoMomento, $dataHoraReferencia = null) {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('obterAcoesPorIniciativa')
            ->setExercicio($exercicio)
            ->setCodigoMomento($codigoMomento)
            ->setCodigoPrograma($codigoPrograma)
            ->setCodigoObjetivo($codigoObjetivo)
            ->setCodigoIniciativa($codigoIniciativa)
        ;

        if (!empty($dataHoraReferencia)) {
            $this->service->getXml()->setDataHoraReferencia($dataHoraReferencia);
        }

        return $this->service->configure()->request();
    }

    /**
     * Retorna o resultado do servico obterProgramacaoCompleta
     *
     * @param $exercicio
     * @param $codigoMomento
     * @param null $dataHoraReferencia
     * @return array
     */
    public function obterProgramacaoCompleta($exercicio, $codigoMomento, $dataHoraReferencia = null) {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('obterProgramacaoCompleta')
            ->setExercicio($exercicio)
            ->setCodigoMomento($codigoMomento);

        if (!empty($dataHoraReferencia)) {
            $this->service->getXml()->setDataHoraReferencia($dataHoraReferencia);
        }

        $retorno = $this->service->configure()->request();
        $this->salvaLog();
        return $retorno;
    }

    /**
     * Retorna o resultado do servico obterTabelasApoio
     *
     * @param $exercicio
     * @param null $dataHoraReferencia
     * @return SimpleXMLElement
     */
    public function obterTabelasApoio($exercicio, $dataHoraReferencia = null) {
        header("Content-type: text/html; charset=utf-8");
        $this->service->getXml()
            ->setService('obterTabelasApoio')
            ->setExercicio($exercicio);

        if (!empty($dataHoraReferencia)) {
            $this->service->getXml()->setDataHoraReferencia($dataHoraReferencia);
        }

        $retorno = $this->service->configure()->request();
        $this->salvaLog();
        return $retorno;
    }

    public function getService() {
        return $this->service->getXml()->getService();
    }

    public function getExercicio() {
        return $this->service->getXml()->getExercicio();
    }

    public function getCodigoMomento() {
        return $this->service->getXml()->getCodigoMomento();
    }

    public function getDataHoraReferencia() {
        return $this->service->getXml()->getDataHoraReferencia();
    }

    public function getListasRetornadas() {
        return $this->service->getXml()->getListasRetornadas();
    }

    public function getCodigoPrograma() {
        return $this->service->getXml()->getCodigoPrograma();
    }

    public function getCodigoObjetivo() {
        return $this->service->getXml()->getCodigoObjetivo();
    }

    public function getCodigoIniciativa() {
        return $this->service->getXml()->getCodigoIniciativa();
    }

    public function getCodigoOrgao() {
        return $this->service->getXml()->getCodigoOrgao();
    }

    public function getCodigoUo() {
        return $this->service->getXml()->getCodigoUo();
    }

    public function setService($service) {
        $this->service->getXml()->setService($service);
        return $this;
    }

    public function setExercicio($exercicio) {
        $this->service->getXml()->setExercicio($exercicio);
        return $this;
    }

    public function setCodigoMomento($codigoMomento) {
        $this->service->getXml()->setCodigoMomento($codigoMomento);
        return $this;
    }

    public function setDataHoraReferencia($dataHoraReferencia) {
        $this->service->getXml()->setDataHoraReferencia($dataHoraReferencia);
        return $this;
    }

    public function setCodigoPrograma($codigoPrograma) {
        $this->service->getXml()->setCodigoPrograma($codigoPrograma);
        return $this;
    }

    public function setCodigoObjetivo($codigoObjetivo) {
        $this->service->getXml()->setCodigoObjetivo($codigoObjetivo);
        return $this;
    }

    public function setCodigoIniciativa($codigoIniciativa) {
        $this->service->getXml()->setCodigoIniciativa($codigoIniciativa);
        return $this;
    }

    public function setCodigoOrgao($codigoOrgao) {
        $this->service->getXml()->setCodigoOrgao($codigoOrgao);
        return $this;
    }

    public function setCodigoUo($codigoUo) {
        $this->service->getXml()->setCodigoUo($codigoUo);
        return $this;
    }

}