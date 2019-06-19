<?php

class DemandasCardapioItensDescricao_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_cardapio_itens_descricao";

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

    public function listaCardapioItensDescricao( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dcddsc'] ){
            $filtro = " and dcddsc ilike removeAcento('%".$arrPost['dcddsc']."%')";
        }
        
        $sql = "SELECT cidid, ciddsc FROM demandas.demandas_cardapio_itens_descricao WHERE cidstatus='A' {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }
    
    public function getCardapioItensDescricaoPorId( $cidid ) {
                
        if($cidid){
            $sql = "SELECT cidid, ciddsc, cidstatus 
                    FROM demandas.demandas_cardapio_itens_descricao
                    WHERE cidid = $cidid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
}