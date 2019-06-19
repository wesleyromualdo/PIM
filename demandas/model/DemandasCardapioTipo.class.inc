<?php

class DemandasCardapioTipo_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_cardapio_tipo";

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

    public function listaCardapioTipo( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dctdsc'] ){
            $filtro = " and dctdsc ilike removeAcento('%".$arrPost['dctdsc']."%')";
        }
        
        $sql = "SELECT dctid, dctdsc FROM demandas.demandas_cardapio_tipo WHERE dctstatus='A' {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }
    
    public function getCardapioTipoPorId( $dctid ) {
                
        if($dctid){
            $sql = "SELECT dctid, dctdsc, dctstatus 
                    FROM demandas.demandas_cardapio_tipo
                    WHERE dctid = $dctid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
}