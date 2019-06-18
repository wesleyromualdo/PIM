<?php

/**
 * Created by PhpStorm.
 * User: danielfiuza
 * Date: 28/03/17
 * Time: 16:14
 */
class Par3_Model_IniciativaDescricaoHistorico extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_descricao_historico";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('inhid');

    protected $arChaveEstrangeira = array(
        'indid' => array(
            'tabela' => 'iniciativa_descricao',
            'pk' => 'indid',
        ),
    );
    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'inhid'            => null,
        'indid'            => null,
        'inhcpf'           => null,//CPF do usuário da sessão
        'inhdt'            => null,//Data de gravação do registro
        'inhacao'          => null,//acão realizada C,A,D
        'inddsc'           => null,
        'indstatus'        => null,
        'inddtinclusao'    => null,
        'indcpfinclusao'   => null,
        'inddtinativacao'  => null,
        'indcpfinativacao' => null,
        'indsituacao'      => null
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';
}