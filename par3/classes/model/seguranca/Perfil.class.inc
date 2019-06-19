<?php

/**
 * Seguranca_Model_Perfil: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 */
class Seguranca_Model_Seguranca_Perfil extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.perfil";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pflcod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pflcod' => null,
        'pfldesc' => null,
    );

    public function listaPerfis()
    {
        $sisId = $_SESSION['sisid'];
        $arrWhere[] = "pflstatus = 'A'";
        $arrWhere[] = "sisid = $sisId";
        $arrWhere[] = "pfldsc <> 'Super Usuário'";
        $arrWhere[] = "pfldsc <> 'Administrador'";

        $perfis = $this->recuperarTodos("pflcod AS codigo, pfldsc AS descricao", $arrWhere, 'pfldsc');

        return $perfis;
    }
}
