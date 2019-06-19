<?php
/**
 * Classe de mapeamento da entidade workflow.documento.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade workflow.documento.
 *
 * @see Modelo
 */
class Workflow_Model_Documento extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'workflow.documento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'docid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'tpdid' => array(
            'tabela' => 'workflow.tipodocumento', 'pk' => 'tpdid'
        ),
        'esdid' => array(
            'tabela' => 'workflow.estadodocumento', 'pk' => 'esdid'
        ),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'docid' => null,
        'tdpid' => null,
        'esdid' => null,
        'docdsc' => null,
        'unicod' => null,
        'docdatainclusao' => null,
        'hstid' => null
    );

}//end class

?>