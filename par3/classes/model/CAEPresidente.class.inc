<?php
/**
 * Classe de mapeamento da entidade par3.cae_conselheiro.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.cae_conselheiro.
 *
 * @see Modelo
 */
class Par3_Model_CAEPresidente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.cae_presidente';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'capid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    	'caeid' => array('tabela' => 'par3.cae', 'pk' => 'caeid'),
    	'entid' => array('tabela' => 'par3.instrumentounidade_entidade', 'pk' => 'entid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'capid' => null,
		'caeid' => null,
    	'entid' => null,
    	'arqid' => null
    );
    
}//end Class
?>