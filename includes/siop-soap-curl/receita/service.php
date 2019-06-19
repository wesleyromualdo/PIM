<?php

include_once APPRAIZ. 'includes/siop-soap-curl/service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/receita/xml.php';


/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 *
 */
class SiopSoapCurl_Receita_Service extends SiopSoapCurl_Service {

    /**
     * Url do serviço
     *
     * @var string
     */
    protected $url = 'https://webservice.siop.gov.br/services/WSReceita';

    /**
     * Filtro do ano do exercicio
     *
     * @var int
     */
    protected $ano;

    /**
     * Documento XML
     *
     * @var SiopSoapCurl_Receita_Xml
     */
    protected $xml;

    public function getUrl() {
        return $this->url;
    }

    public function getAno() {
        return $this->ano;
    }

    public function getXml() {
        return $this->xml;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setAno($ano) {
        $this->ano = $ano;
        return $this;
    }

    public function setXml(SiopSoapCurl_Receita_Xml $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function __construct() {
        parent::__construct();
        $this->xml = new SiopSoapCurl_Receita_Xml();
    }

    public function loadXml() {
        $documento = $this->xml->describe();

        return $documento;
    }

    public function request() {
        return parent::request();
    }

}