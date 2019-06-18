<?php

class Par3_Model_ObraTipoFoto extends Modelo
{
    protected $stNomeTabela = 'par3.obra_tipo_foto';

    protected $arChavePrimaria = array(
        'otfid',
    );

    protected $arChaveEstrangeira = array();

    protected $arAtributos = array(
        'otfid' => null,
        'otfdsc' => null,
        'otfstatus' => null,
    );

    public function getCamposValidacao($dados = array())
    {
        return array(
            'otfid' => array('Digits'),
            'otfdsc' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'otfstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function getComboTipoFoto()
    {
        $dados = $this->recuperarTodosFormatoInput('otfdsc', array("otfstatus = 'A' "));
        $arr = array();
        foreach ($dados as $valor) {
            $arr[$valor['codigo']] = $valor['descricao'];
        }
        return $arr;

    }
}
