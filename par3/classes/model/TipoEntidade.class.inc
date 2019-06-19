<?php
/**
 * Classe de mapeamento da entidade par3.tipoentidade
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-11-2015
 * @link     no link
 */



/**
 * Tipoentidade
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-11-2015
 * @link     no link
 */
class Par3_Model_TipoEntidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.tipoentidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'tenid',
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
        'tenid' => null,
        'tendsc' => null,
        'tenunico' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'tenid' => array(  'Digits'  ),
            'tendsc' => array(  new Zend_Validate_StringLength(array('max' => 255))  ),
            'tenunico' => array(  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>