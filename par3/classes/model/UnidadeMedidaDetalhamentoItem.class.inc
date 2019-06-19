<?php

class Par3_Model_UnidadeMedidaDetalhamentoItem extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.unidademedidadetalhamentoitem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "umiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'umiid' => null,
									  	'umidescricao' => null,
									  	'umistatus' => null
									  );

	protected $stOrdem = null;


}