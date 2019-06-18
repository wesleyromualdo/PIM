<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 04/02/2016
 * Time: 17:20
 */

require_once APPRAIZ . 'includes/library/simec/Listagem.php';

class Sase_Model_Municipiossemalteracaoperiodotecredeae extends Modelo
{
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.municipiossemalteracaoperiodotecredeae";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("mstid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'mstid' => null,
        'sciid' => null,
        'marid' => null,
        'esdid' => null,
        'msaobservacoes' => null
    );

    /**
     * Atributos String da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosStr = array();

    /**
     * @name $arCampos
     * @var array
     * @access protected
     * Campos da Tabela
     */
    protected $arCampos = array(
        'mstid',
        'sciid',
        'marid',
        'esdid',
        'msaobservacoes'
    );

    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array();
}