<?php
/**
 * Classe de mapeamento da entidade sac.area
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */



/**
 * Sac__ModelArea
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */
class Sac_Model_Area extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.area';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'areid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'sisid' => array('tabela' => 'sistema', 'pk' => 'sisid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'areid' => null,
        'aredsc' => null,
        'areordem' => null,
        'sisid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'areid' => array(  'Digits'  ),
            'aredsc' => array(  new Zend_Validate_StringLength(array('max' => 200))  ),
            'areordem' => array( 'allowEmpty' => true, 'Digits'  ),
            'sisid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>