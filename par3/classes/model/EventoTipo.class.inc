<?php
/**
 * Classe de mapeamento da entidade par3.eventotipo
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-12-2015
 * @link     no link
 */



/**
 * Eventotipo
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-12-2015
 * @link     no link
 */
class Par3_Model_EventoTipo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.eventotipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'evtid',
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
        'evtid' => null,
        'evtdsc' => null,
        'evtcor' => null,
        'evtordem' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'evtid' => array(  'Digits'  ),
            'evtdsc' => array(  new Zend_Validate_StringLength(array('max' => 200))  ),
            'evtcor' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))  ),
            'evtordem' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>