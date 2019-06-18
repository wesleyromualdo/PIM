<?php

/**
 * Classe para conectar com o Webservice do SIOP atrav?s do componente SoapCurl
 * 
 */
abstract class SiopSoapCurl_Service implements SiopSoapCurl_InterfaceService {
    
    /**
     * Url do servi?o
     * 
     * @var string
     */
    protected $url;

    /**
     * Cliente usado pra comunicar com o servi?o
     * 
     * @var SoapCurl_Client
     */
    protected $client;
    
    /**
     * Resposta da requisi??o
     * 
     * @var SimpleXMLElement 
     */
    protected $response;

    private $certificado;

    /**
     * Tag onde existem os resultados da requisi??o
     * 
     * @var string
     */
    protected $tagReturn = 'return';

    public function getUrl() {
        return $this->url;
    }

    public function getClient() {
        return $this->client;
    }

    public function getResponse() {
        return $this->response;
    }

    public function getTagReturn() {
        return $this->tagReturn;
    }

    public function setUrl($url) {
        $this->url = $url;
        return $this;
    }

    public function setClient(SoapCurl_Client $client) {
        $this->client = $client;
        return $this;
    }

    public function setResponse(SimpleXMLElement $response) {
        $this->response = $response;
        return $this;
    }

    public function setTagReturn($tagReturn) {
        $this->tagReturn = $tagReturn;
        return $this;
    }
  
    public function __construct() {
        $this->client = new SoapCurl_Client();
        $this->certificado = APPRAIZ . "planacomorc/modulos/sistema/comunica/simec.pem";
    }
    
    protected function mountListHeader(){
        $listHeader = array(
            "Content-type: text/xml;charset=\"utf-8\"",
            "Accept: text/xml",
            "Cache-Control: no-cache",
            "Pragma: no-cache",
            "SOAPAction: ". $this->url,
            "Content-length: ". strlen($this->loadXml())
        );
        
        return $listHeader;
    }
    
    public function configure(){
        # Configurado Protocolo de seguran?a
        $this->client->getSsl()
            ->setVerifyPeer(FALSE)
            ->setVerifyHost(FALSE)
            ->setVersion('4')
            ->setCertificate($this->certificado)
            ->setPassword('simec')
        ;
        
        # Configurando Requisi??o
        $this->client->getHttp()
            ->setWsdl($this->url. '?wsdl')
            ->setReturn(TRUE)
            ->setAuth(CURLAUTH_ANY)
            ->setTimeout(2000)
            ->setPost(TRUE)
            ->setListHeader($this->mountListHeader())
        ;
        
        # Configurando xml contendo o nome e parametros utilizados pelo servi?o especifico
        $this->client
            ->setXml($this->loadXml())->configureAll();
        
        return $this;
    }

    /**
     * Faz requisi??o ao servi?o
     * 
     * @return string
     */
    public function request(){
        $this->client->request();
        $this->convertResponseToObject();
        return $this->response;
    }
    
    /**
     * Converte o resultado da requisi??o(xml) em objeto
     * 
     * @return SimpleXMLElement
     */
    public function convertResponseToObject() {
        $this->response = new SimpleXMLElement('<'. $this->tagReturn. '></'. $this->tagReturn. '>');
        
        # Verifica se existe resposta na requisi??o
        if($this->client->getResponse()){
            # Retira tags de cabe?alho do documento XML
            $posTagStartReturn = strpos($this->client->getResponse(), '<'. $this->tagReturn. '>');
            $responseNoHeader = substr($this->client->getResponse(), $posTagStartReturn, strlen($this->client->getResponse()));
            $posTagEndReturn = strpos($responseNoHeader, '</'. $this->tagReturn. '>');
            $responseReturn = substr($responseNoHeader, 0, ((int)$posTagEndReturn + (int)strlen('</'. $this->tagReturn. '>')));

            # Verifica se dentro da tag de retorno do documento XML existem resultados
//            var_dump($responseReturn);exit;
            if(trim($responseReturn)){
                $this->response = simplexml_load_string(str_ireplace(['env:', 'ns2:', 'sof:'], '', $responseReturn));
            }
        }

        return $this->response;
    }

    /**
     * Exibe o documento xml enviado no momento da requisi??o completo em um arquivo separado pra Baixar/fazer download.
     * 
     * @return VOID
     */    
    public function showXmlRequest(){
        echo $this->loadXml();
        header('Content-Type: application/xml; charset=utf-8');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="request.xml"');
        die;
    }

    /**
     * Exibe o documento xml de resposta completo em um arquivo separado pra Baixar/fazer download.
     * 
     * @return VOID
     */
    public function showXmlResponse() {
        echo $this->client->getResponse();
        header('Content-Type: application/xml; charset=utf-8');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Disposition: attachment; filename="response.xml"');
        die;
    }
    
}
