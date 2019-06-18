<?php

include_once APPRAIZ. 'includes/siop-soap-curl/xml.php';

/**
 * Classe para conectar com o Webservice do SIOP atrav?s do componente SoapCurl
 *
 */
class SiopSoapCurl_Receita_Xml extends SiopSoapCurl_Xml {

    protected $service = 'captarBaseExterna';

    /**
     * Lista de filtros
     *
     * @var array
     */
    private $listFilter = array('anoExercicio' => '2018');

    /**
     * Lista de campos que ser?o retornados
     *
     * @var array
     */
    private $listField = array(
        'codigoCaptacaoBaseExterna' => 'true',
        'detalhesBaseExterna' => 'true',
        'planoOrcamentario' => 'true',
        'programa' => 'true',
        'projetoLei' => 'true',
        'rapAPagarNaoProcessado' => 'true',
        'rapAPagarProcessado' => 'true',
        'rapCanceladosNaoProcessados' => 'true',
        'rapCanceladosProcessados' => 'true',
        'rapExerciciosAnteriores' => 'true',
        'rapInscritoNaoProcessado' => 'true',
        'rapInscritoProcessado' => 'true',
        'rapNaoProcessadoALiquidar' => 'true',
        'rapNaoProcessadoBloqueado' => 'true',
        'rapNaoProcessadoLiquidadoAPagar' => 'true',
        'rapPagoNaoProcessado' => 'true',
        'rapPagoProcessado' => 'true',
        'resultadoPrimarioAtual' => 'true',
        'resultadoPrimarioLei' => 'true',
        'subElementoDespesa' => 'true',
        'subFuncao' => 'true',
        'tematicaPO' => 'true',
        'tipoApropriacaoPO' => 'true',
        'tipoCredito' => 'true',
        'unidadeGestoraResponsavel' => 'true',
        'unidadeOrcamentaria' => 'true',
    );

    /**
     * Pagina atual da consulta
     *
     * @var int
     */
    private $page = 0;

    /**
     * Quantidade de registros por consulta
     *
     * @var int
     */
    private $limit = 2000;

    public function getListFilter() {
        return $this->listFilter;
    }

    public function getListField() {
        return $this->listField;
    }

    public function getPage() {
        return $this->page;
    }

    public function getLimit() {
        return $this->limit;
    }

    public function setListFilter($listFilter) {
        $this->listFilter = $listFilter;
        return $this;
    }

    public function setListField($listField) {
        $this->listField = $listField;
        return $this;
    }

    public function setPage($page) {
        $this->page = $page;
        return $this;
    }

    public function setLimit($limit) {
        $this->limit = $limit;
        return $this;
    }

    public function describeService() {
        $xml = "\n        <ns1:". $this->service. '>';
        $xml .= $this->describeCredential();
        $xml .= $this->describeListField();
        $xml .= "\n". '        </ns1:'. $this->service. '>';
        return $xml;
    }

    public function describeListField() {
        $xml = <<<XML
            <captacaoBaseExterna>
              <codigoCaptacaoBaseExterna>598</codigoCaptacaoBaseExterna>
              <detalhesBaseExterna>
                 <detalheBaseExterna>
                 <codigoNaturezaReceita>13210011</codigoNaturezaReceita>
                    <codigoUnidadeRecolhedora>26239</codigoUnidadeRecolhedora>
                    <subNatureza>280</subNatureza>
                    <justificativa>A previs?o de R$ 1.360.894 ser? para atender ? receita oriunda remunera??o de dep?sitos banc?rios, cuja proje??o encontra-se em linha com o arrecadado registrado at? setembro de 2018.</justificativa>
                    <metodologia>Foi utilizada como base de c?lculo os valores estimados para aplica??o somado aos rendimentos auferidos no exerc?cio at? 28/09/2018 Rendimentos = (Montante x juros x per?odo estimado da aplica??o) + rendimento j? auferidos.</metodologia>
                    <memoriaDeCalculo>Rendimentos = (Montante x juros x per?odo estimado da aplica??o) + rendimento j? auferidos 1.360.894,04 = (15.926.570,41 x 0,042631286 x 62/100) + 939.932,53</memoriaDeCalculo>
                    <valoresBaseExterna>
                       <valorBaseExterna>
                          <exercicio>2018</exercicio>
                          <valor>1360894.00</valor>
                       </valorBaseExterna>
                    </valoresBaseExterna>
                 </detalheBaseExterna>
              </detalhesBaseExterna>
            </captacaoBaseExterna>
XML;


//        $xml = "\n              <captacaoBaseExterna>";
//        if($this->listField){
//            foreach ($this->listField as $field => $value) {
//                $xml .= "\n                  <". $field. '>'. $value. '</'. $field. '>';
//            }
//        }
//        $xml .= "\n              </captacaoBaseExterna>";
        return $xml;
    }

}