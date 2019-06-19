<?php

class Par3_Model_CursosAbasFuncaoAtual extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.curso_aba_pff';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'curid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'curpffid' => null,
        'curid' => null,
        'pffid' => null,
        'curdtinclusao' => null,
        'curcpfinclusao' => null,
        'curdtinativacao' => null,
        'curcpfinativacao' => null,
    );

    public function montarSQLSimples($arrPost)
    {
        $sql = "SELECT curpffid, curid, pffid FROM par3.curso_aba_pff";
        return $sql;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'curpffid' => array('Digits'),
            'curid' => array('Digits'),
            'pffid' => array('Digits'),
            'curdtinclusao' => array('allowEmpty' => true),
            'curcpfinclusao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'curdtinativacao' => array('allowEmpty' => true),
            'curcpfinativacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function getCursosAbasFuncaoAtual(array $arrPost)
    {
        $where = $arrPost['curpffid']?" AND curpffid != {$arrPost['curpffid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela}";
        return $this->pegaLinha($sql,0,0);
    }

//    public function validarInput(array $campos)
//    {
//        //campos
//        $erros['erros']['curdsc'] = array();
//        $erros['erros']['curqtd'] = array();
//
//        //VALIDA CAMPOS
//        if(strlen($campos['curdsc']) > 255){array_push($erros['erros']['curdsc'],'Limite de caracteres excedido');}
//        if($campos['curdsc'] == '' || $campos['curdsc'] == null){array_push($erros['erros']['curdsc'],'O campo Descrição não pode ser vazio.');}
//        if($this->getCursosAbasFuncaoAtual($campos)){array_push($erros['erros']['curdsc'],'A descrição informada já existe');}
//        if(!is_numeric($campos['curqtd'])){array_push($erros['erros']['curqtd'],'O Campo Quantidade de Registro(s) só aceita número(s).');}
//
//
//        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
//        foreach ($erros['erros'] as $key => $value){
//            if (!empty($erros['erros'][$key])){
//                return $erros;
//            }
//        }
//        return false;
//    }
}
