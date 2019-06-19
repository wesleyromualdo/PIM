<?php

include_once APPRAIZ. 'includes/siop-soap-curl/service.php';

class SiopSoapCurl_Quantitativo_Service extends SiopSoapCurl_Service {

    const PRODUCAO = 1;
    const DESENVOLVIMENTO = 2;

    protected $url;

    protected $ano;

    protected $xml;

    public function __construct($ambiente = self::PRODUCAO) {
        $this->xml = new SiopSoapCurl_Quantitativo_Xml();
        if ($ambiente == self::PRODUCAO) {
            $this->setUrl('https://webservice.siop.gov.br/services/WSQuantitativo');
        } else {
            $this->setUrl('https://testews.siop.gov.br/services/WSQuantitativo');
        }
        parent::__construct();
    }

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

    public function setXml(SiopSoapCurl_Quantitativo_Xml $xml) {
        $this->xml = $xml;
        return $this;
    }

    public function loadXml() {
//        $this->xml = new SiopSoapCurl_Quantitativo_Xml();
//        $this->xml->setListFilter(array('anoExercicio' => $this->ano));
        $documento = $this->xml->describe();

        return $documento;
    }

    /**
     * Faz requisição ao serviço e retorna lista de ações, localizadores e Planos Orçamentários(POs)
     *
     * @return array
     */
    public function request() {
        var_dump($this);
        exit();
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
                        $listas->return->$nome = json_decode(json_encode($registro), true);
                        break;
                    case 'sucesso':
                    case 'numeroRegistros':
                    case 'valorTotal':
                        $listas->return->$nome = json_decode(json_encode($registro), true)[0];
                        break;
                    default:
                        if(!$listas->return->$nome){
                            $listas->return->$nome = array();
                        }
                        array_push($listas->return->$nome, json_decode(json_encode($registro)));
                        break;
                }
            }
        }

        return $listas;
    }
}