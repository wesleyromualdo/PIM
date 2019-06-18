<?php

class Par3_Model_CursosAbas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.curso_aba';

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
        'inprgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'curid' => null,
        'inprgid' => null,
        'curdsc' => null,
        'curtitulo' => null,
        'curqtd' => null,
        'funid' => null,
        'curorientacao' => null,
        'curmsg' => null,
        'pffid' => null,
        'tfoid' => null,
        'curscript' => null,
        'curstatus' => null,
        'cursituacao' => null,
        'curdtinclusao' => null,
        'curcpfinclusao' => null,
        'curdtinativacao' => null,
        'curcpfinativacao' => null,
    );

    public function montarSQLSimples($arrPost)
    {
        $sql = "SELECT curid as id,
                       funcao.fundsc,
                       curdsc as descricao, 
                       CASE WHEN (curorientacao = ('<p><br></p>')) THEN 'Não' ELSE 'Sim' END AS curorientacao,
                       cursituacao
                FROM par3.curso_aba as ca
                LEFT JOIN entidade.funcao on (ca.funid = funcao.funid)
                INNER JOIN par3.programa as pr ON ca.inprgid = pr.prgid
                WHERE curstatus = 'A'
                AND   pr.prgstatus = 'A' 
                AND   ca.inprgid = {$arrPost['prgid']}
                ";

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
            'curid' => array('Digits'),
            'curdsc' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'curtitulo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'curqtd' => array('allowEmpty' => true,'Digits'),
            'funid' => array('allowEmpty' => true,'Digits'),
            'curorientacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4000))),
            'curmsg' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4000))),
            'pffid' => array('allowEmpty' => true,'Digits'),
            'tfoid' => array('allowEmpty' => true,'Digits'),
            'curscript' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4000))),
            'curstatus' => array('allowEmpty' => true),
            'cursituacao' => array('allowEmpty' => true),
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

    public function getCursosAbas(array $arrPost)
    {
        $where = $arrPost['curid']?" AND curid != {$arrPost['curid']} ":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(curdsc) = '".str_to_upper($arrPost['curdsc'])."' AND curstatus = 'A' {$where} AND inprgid = {$arrPost['inprgid']}";
        return $this->pegaLinha($sql,0,0);
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['curdsc'] = array();
        $erros['erros']['curqtd'] = array();

        //VALIDA CAMPOS
        if(strlen($campos['curdsc']) > 255){array_push($erros['erros']['curdsc'],'Limite de caracteres excedido');}
        if($campos['curdsc'] == '' || $campos['curdsc'] == null){array_push($erros['erros']['curdsc'],'O campo Descrição não pode ser vazio.');}
        if($this->getCursosAbas($campos)){array_push($erros['erros']['curdsc'],'A descrição informada já existe');}
        if(!empty ($campos['curqtd']) || isset ($campos['curqtd'])){
            if(!is_numeric($campos['curqtd'])){array_push($erros['erros']['curqtd'],'O Campo Quantidade de Registro(s) só aceita número(s).');}
        }


        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }
}
