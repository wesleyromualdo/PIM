<?php
/**
 * Classe de mapeamento da entidade emenda.emendapariniciativa
 *
 * @category Class
 * @package  A1
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 */



/**
 * Emenda__ModelEmendapariniciativa
 *
 * @category Class
 * @package  A1
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 */
class Spoemendas_Model_Emendapariniciativa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emenda.emendapariniciativa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'epiid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'edeid' => array('tabela' => 'emenda.emendadetalheentidade', 'pk' => 'edeid'),
        'iniid' => array('tabela' => 'emenda.iniciativa', 'pk' => 'iniid'),
        'ppsid' => array('tabela' => 'par.propostasubacao', 'pk' => 'ppsid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'epiid' => null,
        'ppsid' => null,
        'edeid' => null,
        'iniid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'epiid' => array(  'Digits'  ),
            'ppsid' => array( 'allowEmpty' => true, 'Digits'  ),
            'edeid' => array( 'allowEmpty' => true, 'Digits'  ),
            'iniid' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)


}//end Class
?>