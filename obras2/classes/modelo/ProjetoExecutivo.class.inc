<?php

class ProjetoExecutivo extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.projeto_executivo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("proid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
            'proid' => null,
            'obrid' => null,
            'proversao1' => null,
            'proversao2' => null,
            'proversao3' => null,
            'proimplantacao_alterado' => null,
            'profundacao_alterado' => null,
            'proestrutural_alterado' => null,
            'proeletrica_alterado' => null,
            'prohidraulica_alterado' => null,
            'proarquitetura_alterado' => null,
            'proartrrt_alterado' => null,
            'proimplantacao_arquivo' => null,
            'profundacao_arquivo' => null,
            'proestrutural_arquivo' => null,
            'proeletrica_arquivo' => null,
            'prohidraulica_arquivo' => null,
            'proarquitetura_arquivo' => null,
            'proartrrt_arquivo' => null,
            'usucpfinclusao' => null,
            'prostatus' => null,
            'prodtinclusao' => null
    );

    public function populaPorObrid($obrid){
        $sql = "SELECT * FROM obras2.projeto_executivo WHERE obrid = {$obrid} AND prostatus = 'A'";
        $projeto = $this->pegaLinha($sql);
        $this->popularDadosObjeto($projeto);
    }
}