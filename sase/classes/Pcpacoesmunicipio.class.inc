<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 04/02/2016
 * Time: 20:12
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Pcpacoesmunicipio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.pcpacoesmunicipio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pamid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'marid' => array('tabela' => 'sase.municipiosassistidosredeae', 'pk' => 'marid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pamid' => null,
        'marid' => null,
        'esdid' => null,
        'pamdsc' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'pamid' => array('Digits'),
            'marid' => array('Digits'),
            'esdid' => array('Digits'),
            'pamdsc' => array('Digits')
        );
    }//end getCamposValidacao($dados)
}