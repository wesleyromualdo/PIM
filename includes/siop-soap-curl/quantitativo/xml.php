<?php

include_once APPRAIZ. 'includes/siop-soap-curl/xml.php';

/**
 * Classe para conectar com o Webservice do SIOP através do componente SoapCurl
 *
 */
class SiopSoapCurl_Quantitativo_Xml extends SiopSoapCurl_Xml {

    protected $service;

    /**
     * Lista de filtros
     *
     * @var array
     */
    private $listFilter = array('anoExercicio' => '2018');

    /**
     * Lista de campos que serão retornados
     *
     * @var array
     */
    private $listField = array(
        'acao' => 'true',
        'acompanhamentoPO' => 'true',
        'anoExercicio' => 'true',
        'anoReferencia' => 'true',
        'autorizado' => 'true',
        'bloqueadoRemanejamento' => 'true',
        'bloqueadoSOF' => 'true',
        'categoriaEconomica' => 'true',
        'creditoContidoSOF' => 'true',
        'detalheAcompanhamentoPO' => 'true',
        'disponivel' => 'true',
        'dotAtual' => 'true',
        'dotInicialSiafi' => 'true',
        'dotacaoAntecipada' => 'true',
        'dotacaoInicial' => 'true',
        'elementoDespesa' => 'true',
        'empLiquidado' => 'true',
        'empenhadoALiquidar' => 'true',
        'esfera' => 'true',
        'estatalDependente' => 'true',
        'executadoPorInscricaoDeRAP' => 'true',
        'fonte' => 'true',
        'funcao' => 'true',
        'grupoNaturezaDespesa' => 'true',
        'identificadorAcompanhamentoPO' => 'true',
        'idoc' => 'true',
        'iduso' => 'true',
        'indisponivel' => 'true',
        'localizador' => 'true',
        'modalidadeAplicacao' => 'true',
        'natureza' => 'true',
        'numeroptres' => 'true',
        'origem' => 'true',
        'pago' => 'true',
        'planoInterno' => 'true',
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

    protected $exercicio;

    protected $codigoMomento;

    protected $periodo;

    protected $snAcompanhamentoOpcionalAcao;

    protected $snAcompanhamentoOpcionalLocalizador;

    protected $periodoOrdem;

    protected $dataHoraReferencia;

    protected $mostraPaginacao = false;

    /**
     * Pagina atual da consulta
     *
     * @var int
     */
    private $page = 1;

    /**
     * Quantidade de registros por consulta
     *
     * @var int
     */
    private $limit = 2000;

    public function getExercicio(){
        return $this->exercicio;
    }

    public function getCodigoMomento(){
        return $this->codigoMomento;
    }

    public function getPeriodo(){
        return $this->periodo;
    }

    public function getSnAcompanhamentoOpcionalAcao(){
        return $this->snAcompanhamentoOpcionalAcao;
    }

    public function getSnAcompanhamentoOpcionalLocalizador(){
        return $this->snAcompanhamentoOpcionalLocalizador;
    }

    public function getPeriodoOrdem(){
        return $this->periodoOrdem;
    }

    public function getDataHoraReferencia(){
        return $this->dataHoraReferencia;
    }

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

    public function setExercicio($exercicio) {
        $this->exercicio = $exercicio;
        return $this;
    }

    public function setCodigoMomento($codigoMomento){
        $this->codigoMomento = $codigoMomento;
        return $this;
    }

    public function setPeriodo($periodo){
        $this->periodo = $periodo;
        return $this;
    }

    public function setSnAcompanhamentoOpcionalAcao($valor){
        $this->snAcompanhamentoOpcionalAcao = $valor;
        return $this;
    }

    public function setSnAcompanhamentoOpcionalLocalizador($valor){
        $this->snAcompanhamentoOpcionalLocalizador = $valor;
        return $this;
    }

    public function setPeriodoOrdem($valor){
        $this->periodoOrdem = $valor;
        return $this;
    }

    public function setDataHoraReferencia($valor) {
        $this->dataHoraReferencia = $valor;
        return $this;
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

    public function setPaginacao($limit = 2000){
        $this->mostraPaginacao = true;
        $this->limit = $limit;
        return $this;
    }

    public function describeService() {
        $xml = "\n        <ns1:". $this->service. '>';
        $xml .= $this->describeCredential();
        $xml .= $this->exercicio? "\n              <exercicio>". (int) $this->exercicio. "</exercicio>": NULL;
        $xml .= $this->codigoMomento? "\n              <codigoMomento>". (int) $this->codigoMomento. "</codigoMomento>": NULL;
        $xml .= $this->periodo? "\n              <periodo>". (int) $this->periodo. "</periodo>": NULL;
        $xml .= $this->periodoOrdem? "\n              <periodoOrdem>". (int) $this->periodoOrdem. "</periodoOrdem>": NULL;
        $xml .= $this->snAcompanhamentoOpcionalAcao? "\n              <snAcompanhamentoOpcionalAcao>". (int) $this->snAcompanhamentoOpcionalAcao. "</snAcompanhamentoOpcionalAcao>": NULL;
        $xml .= $this->snAcompanhamentoOpcionalLocalizador? "\n              <snAcompanhamentoOpcionalLocalizador>". (int) $this->snAcompanhamentoOpcionalLocalizador. "</snAcompanhamentoOpcionalLocalizador>": NULL;
        $xml .= $this->describeListFilter();
        $xml .= $this->describeListField();
        $xml .= $this->describePagination();
        $xml .= "\n". '        </ns1:'. $this->service. '>';
        return $xml;
    }

    public function describeListFilter() {
        $xml = "\n              <filtro>";
        if($this->listFilter){
            foreach ($this->listFilter as $filter => $value) {
                switch ($filter){
                    default:
                        $xml .= "\n                  <". $filter. '>'. $value. '</'. $filter. '>';
                        break;

                    case 'acoes':
                        $xml .= $this->describeListAcoes($value);
                        break;
                }
            }
        }
        $xml .= "\n              </filtro>";
        return $xml;
    }

    public function describeListField() {
        if($this->listField){
            $xml = "\n              <selecaoRetorno>";
            foreach ($this->listField as $field => $value) {
                $xml .= "\n                  <". $field. '>'. $value. '</'. $field. '>';
            }
            $xml .= "\n              </selecaoRetorno>";
        }
        return $xml;
    }

    public function describeListAcoes($acoes) {
        $xml = "\n                  <acoes>";
        foreach ($acoes as $acao) {
            $xml .= "\n                      <acao>".$acao."</acao>";
        }
        $xml .= "\n                  </acoes>";
        return $xml;
    }

    public function describePagination() {
        $xml = "";
        $xml = "\n              <paginacao>";
        if ($this->mostraPaginacao) {
            $xml .= "\n                  <pagina>" . (int)$this->page . '</pagina>';
            $xml .= "\n                  <registrosPorPagina>" . (int)$this->limit . '</registrosPorPagina>';
        }
        $xml .= "\n              </paginacao>";

        return $xml;
    }

}