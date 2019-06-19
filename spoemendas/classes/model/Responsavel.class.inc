<?php
/**
 * Classe de mapeamento da entidade emenda.responsavel;
 *
 * @version $Id: Responsavel.class.inc 108670 2016-02-29 20:01:18Z maykelbraz $
 */

/**
 * Spoemendas_Model_Responsavel
 *
 * @package Spoemendas\Model\Responsavel
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Spoemendas_Model_Responsavel extends Modelo
{
    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela = 'emenda.responsavel';

    /**
     * @var mixed[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'resid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'autid' => array('tabela' => 'emenda.autor', 'pk' => 'autid'),
        'unicod, unitpocod' => array('tabela' => 'unidade', 'pk' => 'unicod, unitpocod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'resid' => null,
        'unicod' => null,
        'unitpocod' => null,
        'resdsc' => null,
        'resassunto' => null,
        'resstatus' => null,
        'rescontrapartidapadrao' => null,
        'autid' => null,
    );

    /**
     * Validators
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'resid' => array(  'Digits'  ),
            'unicod' => array(  new Zend_Validate_StringLength(array('max' => 5))  ),
            'unitpocod' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'resdsc' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
            'resassunto' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
            'resstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'rescontrapartidapadrao' => array( 'allowEmpty' => true ),
            'autid' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }

}
