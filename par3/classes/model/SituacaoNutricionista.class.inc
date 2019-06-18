<?php
/**
 * Classe de mapeamento da entidade par3.situacaonutricionista
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-12-2015
 * @link     no link
 */



/**
 * Situacaonutricionista
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-12-2015
 * @link     no link
 */
class Par3_Model_SituacaoNutricionista extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.situacaonutricionista';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'snid',
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
        'snid' => null,
        'sndescricao' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */


    public function getCamposValidacao($dados = array())
    {
        return array(
            'snid' => array(  'Digits'  ),
            'sndescricao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>