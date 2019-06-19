<?php

class Par3_Model_ObjetoIniciativa extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.objetoiniciativa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "obiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'obiid' => null,
									  	'obidesc' => null,
									  	'obistatus' => null
									  );

	protected $stOrdem = null;


}