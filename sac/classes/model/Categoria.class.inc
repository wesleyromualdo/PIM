<?php
/**
 * Classe de mapeamento da entidade sac.categoria
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */



/**
 * Sac__ModelCategoria
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-02-2016
 * @link     no link
 */
class Sac_Model_Categoria extends Modelo
{
    const K_PAR3_APOIO_TECNICO = 6;
    const K_SAC_OUTRO = 100;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.categoria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'catid',
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
        'catid' => null,
        'catdsc' => null,
        'catordem' => null,
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
            'catid' => array(  'Digits'  ),
            'catdsc' => array(  new Zend_Validate_StringLength(array('max' => 200))  ),
            'catordem' => array( 'allowEmpty' => true, 'Digits'  ),
            'sisid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)



}//end Class
?>