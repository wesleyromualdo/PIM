<?php
/**
 * Classe de mapeamento da entidade emenda.emendatipoorigem
 *
 * @category Class
 * @package  A1
 * @author   MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-02-2016
 * @link     no link
 */



/**
 * Emenda_ModelEmendatipoorigem
 *
 * @category Class
 * @package  A1
 * @author   MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-02-2016
 * @link     no link
 */
class Emenda_Model_Emendatipoorigem extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emenda.emendatipoorigem';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'etoid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'etoid' => null,
        'etodescricao' => null,
        'etostatus' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'etoid' => array(  'Digits'  ),
            'etodescricao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))  ),
            'etostatus' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>