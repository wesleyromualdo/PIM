<?php

class SupervisaoSituacaoEmpresa extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao2_situacao_obra";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sobid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'sobid' => null,
        'sobdsc' => null,
        'sobstatus' => null,
        'sobordem' => null,
    );
}