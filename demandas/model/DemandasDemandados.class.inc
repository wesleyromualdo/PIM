<?php

class DemandasDemandados_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.demandados";
    
    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('dddid');
    
    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('dddid', 'ejfid');
    
    protected $arAtributos = array(
        'dddid' => null,
        'ejfid' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function getDemandadosCombo( $arr ) {
        
        $sql = "SELECT dddid AS codigo, enjdsc AS descricao FROM demandas.demandados d
                                    INNER JOIN corporativo.entidades_juridicaxfuncao ej ON ej.ejfid = d.ejfid
                                    INNER JOIN corporativo.entidades_juridica j on j.enjid = ej.enjid;";
        
        
        if( $arr['returnsql'] ) {
            return $sql;
        }
        
    }
    
    
}