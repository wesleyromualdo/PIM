<?php

class TipoInsumo_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.tipo_insumo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('co_tipo_insumo');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('co_tipo_insumo', 'ds_tipo_insumo', 'co_status');

    protected $arAtributos = array(
        'co_tipo_insumo' => null,
        'ds_tipo_insumo' => null,
        'co_status' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function getComboTipoInsumo()
    {
        $sql = "SELECT co_tipo_insumo as codigo, ds_tipo_insumo as descricao FROM demandas.tipo_insumo WHERE co_status = 1 ";

        return $sql;
        
    }
    
}