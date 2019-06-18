<?php

class Par3_Model_IniciativaTiposAssistenciaHistorico extends Modelo
{
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
    */
    protected $stNomeTabela = "par3.iniciativa_tipos_assistencia_historico";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('intahid');

    protected $arChaveEstrangeira = array(
        'intaid' => array(
            'tabela' => 'iniciativa_tipos_assistencia',
            'pk' => 'intaid',
        ),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'intahid'           => null,
        'intahcpf'          => null,//CPF do usuário da sessão
        'intahdt'           => null,//Data de gravação do registro
        'intahacao'         => null,//acão realizada C,A,D
        'intaid'            => null,
        'intadsc'           => null,
        'intastatus'        => null,
        'intadtinclusao'    => null,
        'intacpfinclusao'   => null,
        'intadtinativacao'  => null,
        'intacpfinativacao' => null,
        'intasituacao'      => null
    );

}