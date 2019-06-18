<?php

class Par3_Model_EscolasPrioritarias extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.proadesao_lista_escolas_prioritarias";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pepid" );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    	'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
    );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'pepid ' => null,
		'pepescola' => null,
		'co_entidade ' => null,
		'muncod ' => null,
    	'estuf ' => null
	);

	protected $stOrdem = null;
}