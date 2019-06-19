<?php

class DemandasCardapio_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandas_cardapio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('dcoid');

    /**
     * Atributos
     * @var array
     * @access protected
     * 
     *
     */
    protected $arAtributosTabela = array('dcoid', 'dcdid', 'dctid', 'dddid', 'dcostatus');

    protected $arAtributos = array(
        'dcoid' => null,
        'dcdid' => null,
        'dctid' => null,
        'dddid' => null,
        'dcostatus' => null
    );
    
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaCardapio( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['dctid'] ){
            $filtro = " and ct.dctid='".$arrPost['dctid']."'";
        }
        
        if( $arrPost['dcdid'] ){
            $filtro = " and cd.dcdid='".$arrPost['dcdid']."'";
        }
        
        if( $arrPost['dddid'] ){
            $filtro = " and dd.dddid='".$arrPost['dddid']."'";
        }
        
        $sql = "SELECT dc.dcoid, cd.dcddsc, ct.dctdsc, ej.enjdsc FROM demandas.demandas_cardapio dc 
                INNER JOIN demandas.demandas_cardapio_descricao cd ON cd.dcdid = dc.dcdid 
                INNER JOIN demandas.demandas_cardapio_tipo ct ON ct.dctid = dc.dctid 
                INNER JOIN demandas.demandados dd ON dd.dddid = dc.dddid 
                INNER JOIN corporativo.entidades_juridica ej ON ej.enjid = dd.enjid
                WHERE dcostatus='A' {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }
    
    public function getCardapioPorId( $dcoid ) {
                
        if($dcoid){
            $sql = "SELECT dcoid, dcdid, dctid, dddid, dcostatus 
                    FROM demandas.demandas_cardapio
                    WHERE dcoid = $dcoid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
}