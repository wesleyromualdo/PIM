<?php
/**
 * Classe de mapeamento da entidade par3.eventolocalizacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-12-2015
 * @link     no link
 */



/**
 * Eventolocalizacao
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-12-2015
 * @link     no link
 */
class Par3_Model_EventoLocalizacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.eventolocalizacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'evlid',
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
        'evlid' => null,
        'eveid' => null,
        'regcod' => null,
        'estuf' => null,
        'muncod' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'evlid' => array(  'Digits'  ),
            'eveid' => array(  'Digits'  ),
            'regcod' => array( 'allowEmpty' => true, 'Digits'  ),
            'estuf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))  ),
            'muncod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>