<?php

class DemandasCardapioItens_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_cardapio_itens";

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
    
    protected $arAtributosTabela = array('dciid', 'dcoid', 'cidid','dcistatus','dcivalor','dcivaloreditavel');

    protected $arAtributos = array(
        'dciid' => null,
        'dcoid' => null,
        'cidid' => null,
        'dcistatus' => null,
        'dcivalor' => null,
        'dcivaloreditavel' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaCardapioItens( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dcoid'] ){
            $filtro = " and dcoid='".$arrPost['dcoid']."'";
        }
        
        $sql = "SELECT ci.dciid, cd.ciddsc, ci.dcivalor, dcivaloreditavel FROM demandas.demandas_cardapio_itens ci 
                INNER JOIN demandas.demandas_cardapio_itens_descricao cd ON cd.cidid = ci.cidid 
                WHERE dcistatus='A' {$filtro}";
        

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