<?php

class PisoInsumo_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.piso_insumo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('co_piso_insumo');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('co_piso_insumo', 'ds_piso_insumo', 'co_status');

    protected $arAtributos = array(
        'co_piso_insumo' => null,
        'ds_piso_insumo' => null,
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

    public function getComboPisoInsumo()
    {
        $sql = "SELECT co_piso_insumo as codigo, ds_piso_insumo as descricao FROM demandas.piso_insumo WHERE co_status = 1 ";

        return $sql;

    }

}