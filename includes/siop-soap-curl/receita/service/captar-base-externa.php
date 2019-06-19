<?php

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Receita_Service_CaptarBaseExterna extends SiopSoapCurl_Service {

    /**
     * Url do serviço
     * 
     * @var string
     */
//    protected $url = 'https://testews.siop.gov.br/services/WSReceita';
    protected $url = 'https://webservice.siop.gov.br/services/WSReceita';

    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Receita_Xml_CaptarBaseExterna
     */
    protected $xml;
    
    public function getUrl() {
        return $this->url;
    }

    public function getXml() {
        return $this->xml;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setXml(SiopSoapCurl_Receita_Xml_CaptarBaseExterna $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function __construct() {
        parent::__construct();
        $this->xml = new SiopSoapCurl_Receita_Xml_CaptarBaseExterna();
    }
    
    /**
     * Retorna o XML pra ser enviado no momento da requisição
     * 
     * @return string xml
     */
    public function loadXml() {
        $documento = $this->xml->describe();
        
        return $documento;
    }
}

