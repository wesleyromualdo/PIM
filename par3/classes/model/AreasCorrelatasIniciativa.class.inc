<?php

class Par3_Model_AreasCorrelatasIniciativa extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.areascorrelatasiniciativa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aciid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'aciid' => null,
									  	'acidesc' => null,
									  	'acistatus' => null
									  );

	protected $stOrdem = null;


}