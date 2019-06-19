<?php

class DemandasPortfolioTipo_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_portfolio_tipo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('dctid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('dctid', 'dctdsc', 'dctstatus');

    protected $arAtributos = array(
        'dctid' => null,
        'dctdsc' => null,
        'dctstatus' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function getPortfolioTipoCombo( $arr ) {
        
        $sql = "SELECT dctid AS codigo, dctdsc AS descricao FROM demandas.demandas_portfolio_tipo WHERE dctstatus=1;";
        
        if( $arr['returnsql'] ) {
            return $sql;
        }
        
    }
    
    

    public function listaPortfolioTipo( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dctdsc'] ){
            $filtro = " and dctdsc ilike removeAcento('%".$arrPost['dctdsc']."%')";
        }
        
        $sql = "SELECT dctid, dctdsc FROM demandas.demandas_portfolio_tipo WHERE dctstatus=1 {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }
    
    public function getPortfolioTipoPorId( $dctid ) {
                
        if($dctid){
            $sql = "SELECT dctid, dctdsc, dctstatus 
                    FROM demandas.demandas_portfolio_tipo
                    WHERE dctid = $dctid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
}