<?php

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 * 
 */
class SiopSoapCurl_Qualitativo_Service_ProgramacaoCompleta extends SiopSoapCurl_Service {

    const PRODUCAO = 1;
    const DESENVOLVIMENTO = 2;

    /**
     * Url do serviço
     * 
     * @var string
     */
    protected $url;
    
    /**
     * Documento XML
     * 
     * @var SiopSoapCurl_Qualitativo_Xml_ProgramacaoCompleta
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

    public function setXml(SiopSoapCurl_Qualitativo_Xml_ProgramacaoCompleta $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function __construct($ambiente = self::PRODUCAO) {
        $this->xml = new SiopSoapCurl_Qualitativo_Xml_ProgramacaoCompleta();
        if ($ambiente == self::PRODUCAO) {
            $this->setUrl('https://webservice.siop.gov.br/services/WSQualitativo');
        } else {
            $this->setUrl('https://testews.siop.gov.br/services/WSQualitativo');
        }
        parent::__construct();
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
    
    /**
     * Faz requisição ao serviço e retorna lista de ações, localizadores e Planos Orçamentários(POs)
     * 
     * @return array
     */
    public function request() {
        $result = parent::request();
        $listas = new stdClass();
        $listas->return = new stdClass();

        foreach($result as $nome => $registro){
            if(strpos($nome, 'DTO')){
                if(!$listas->return->$nome){
                    $listas->return->$nome = array();
                }
                array_push($listas->return->$nome, $registro);
            } else {
                switch ($nome) {
                    case 'mensagensErro':
                    case 'sucesso':
                        $listas->return->$nome = json_decode(json_encode($registro), true)[0];
                        break;
                }
            }
        }

        return $listas;
    }

}

