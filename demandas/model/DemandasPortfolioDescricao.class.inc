<?php

class DemandasPortfolioDescricao_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_portfolio_descricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('dcdid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('dcdid', 'dcddsc', 'dcdstatus');

    protected $arAtributos = array(
        'dcdid' => null,
        'dcddsc' => null,
        'dcdstatus' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function getPortfolioDescricaoCombo( $arr ) {
        
        $sql = "SELECT dcdid AS codigo, dcddsc AS descricao FROM demandas.demandas_portfolio_descricao WHERE dcdstatus=1;";
        
        if( $arr['returnsql'] ) {
            return $sql;
        }
        
    }

    public function listaPortfolioDescricao( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dcddsc'] ){
            $filtro = " and dcddsc ilike removeAcento('%".$arrPost['dcddsc']."%')";
        }
        
        $sql = "SELECT dcdid, dcddsc FROM demandas.demandas_portfolio_descricao WHERE dcdstatus=1 {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }
    
    public function getPortfolioDescricaoPorId( $dcdid ) {
                
        if($dcdid){
            $sql = "SELECT dcdid, dcddsc, dcdstatus 
                    FROM demandas.demandas_portfolio_descricao
                    WHERE dcdid = $dcdid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
}