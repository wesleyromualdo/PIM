<?php

class Pim_Model_FuncaoEntidade extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.entidades_funcao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('enfid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('enfid', 'enfdsc', 'enfstatus');

    protected $arAtributos = array(
        'enfdsc' => null,
        'enfstatus' => null
    );

    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

}