<?php

class DemandasPortfolioItensDescricao_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_portfolio_itens_descricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('cidid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('cidid', 'ciddsc', 'cidstatus');

    protected $arAtributos = array(
        'cidid' => null,
        'ciddsc' => null,
        'cidstatus' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaPortfolioItensDescricao( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dcddsc'] ){
            $filtro = " and dcddsc ilike removeAcento('%".$arrPost['dcddsc']."%')";
        }
        
        $sql = "SELECT cidid, ciddsc FROM demandas.demandas_portfolio_itens_descricao WHERE cidstatus=1 {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }
    
    public function getPortfolioItensDescricaoPorId( $cidid ) {
                
        if($cidid){
            $sql = "SELECT cidid, ciddsc, cidstatus 
                    FROM demandas.demandas_portfolio_itens_descricao
                    WHERE cidid = $cidid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
}