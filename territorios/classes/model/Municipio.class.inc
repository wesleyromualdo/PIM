<?php
/**
 * Classe de mapeamento da entidade territorios.municipio
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-12-2015
 * @link     no link
 */



/**
 * Municipio
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-12-2015
 * @link     no link
 */
class Territorios_Model_Municipio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'territorios.municipio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'muncod',
        'estuf',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'miccod' => array('tabela' => 'territorios.microregiao', 'pk' => 'miccod'),
        'mescod' => array('tabela' => 'territorios.mesoregiao', 'pk' => 'mescod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'muncod' => null,
        'muncod' => null,
        'estuf' => null,
        'estuf' => null,
        'miccod' => null,
        'mescod' => null,
        'mundescricao' => null,
        'munprocesso' => null,
        'muncodcompleto' => null,
        'munmedlat' => null,
        'munmedlog' => null,
        'munhemis' => null,
        'munaltitude' => null,
        'munmedarea' => null,
        'muncepmenor' => null,
        'muncepmaior' => null,
        'munmedraio' => null,
        'munmerid' => null,
        'muncodsiafi' => null,
        'munpopulacao' => null,
        'munpopulacaohomem' => null,
        'munpopulacaomulher' => null,
        'munpopulacaourbana' => null,
        'munpopulacaorural' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'muncod' => array(  new Zend_Validate_StringLength(array('max' => 7))  ),
            'muncod' => array(  new Zend_Validate_StringLength(array('max' => 7))  ),
            'estuf' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'estuf' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'miccod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 5))  ),
            'mescod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 4))  ),
            'mundescricao' => array(  new Zend_Validate_StringLength(array('max' => 255))  ),
            'munprocesso' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 21))  ),
            'muncodcompleto' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 12))  ),
            'munmedlat' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 9))  ),
            'munmedlog' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 9))  ),
            'munhemis' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'munaltitude' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'munmedarea' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 12))  ),
            'muncepmenor' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 8))  ),
            'muncepmaior' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 8))  ),
            'munmedraio' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'munmerid' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'muncodsiafi' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 4))  ),
            'munpopulacao' => array( 'allowEmpty' => true, 'Digits'  ),
            'munpopulacaohomem' => array( 'allowEmpty' => true, 'Digits'  ),
            'munpopulacaomulher' => array( 'allowEmpty' => true, 'Digits'  ),
            'munpopulacaourbana' => array( 'allowEmpty' => true, 'Digits'  ),
            'munpopulacaorural' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)

    /**
     * Função pegarSQLSelect
     *
     * @return query para montar componentes.
     */
    public function pegarSQLSelect($estuf)
    {
    	$where = '';
    
    	if ($estuf != '') $where = "WHERE estuf = '$estuf'";
    
    	$sql = "SELECT muncod as codigo, mundescricao as descricao
				FROM territorios.municipio
				$where
				ORDER BY 2 ASC";
    
    	return $sql;
    
    }//end pegarSQLSelect()

}//end Class
?>