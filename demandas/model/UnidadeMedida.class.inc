<?php

class UnidadeMedida_Model extends Modelo
{
    protected $stNomeTabela = 'demandas.unidade_medida';

    protected $arChavePrimaria = array(
        'co_unidade_medida',
    );

    protected $arChaveEstrangeira = array(
    );

    protected $arAtributos = array(
        'co_unidade_medida' => null,
        'ds_sigla' => null,
        'ds_unidade_medida' => null,
        'nu_cpf_inclusao' => null,
        'dt_inclusao' => null,
        'co_status' => null,
    );

    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaUnidadeMedida( $arrPost )
    {
        $filtro = '';
        $filtroStatus = ' co_status = 1 ';

        if( $arrPost['co_unidade_medida'] ){
            $filtro .= " AND co_unidade_medida = '".$arrPost['co_unidade_medida']."' ";
        }

        if( $arrPost['ds_unidade_medida'] ){
            $filtro .= " AND ds_unidade_medida ILIKE '%".$arrPost['ds_unidade_medida']."%' ";
        }

        if( isset($arrPost['co_status']) ){
            if($arrPost['co_status'] != '-1')
                $filtroStatus = " co_status = '".$arrPost['co_status']."' ";
            else
                $filtroStatus = " (co_status = 1 OR co_status = 0) ";
        }
        
        $sql = "SELECT co_unidade_medida, ds_sigla, ds_unidade_medida FROM demandas.unidade_medida WHERE {$filtroStatus} {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }

    public function getSiglas()
    {
        $sql = "SELECT co_unidade_medida as codigo, ds_sigla as descricao FROM demandas.unidade_medida WHERE co_status = 1 ";

        return $sql;
    }

    public function getComboUnidadeMedida()
    {
        $sql = "SELECT co_unidade_medida as codigo, COALESCE(ds_unidade_medida || ' (' || ds_sigla || ')' ) as descricao FROM demandas.unidade_medida WHERE co_status = 1 ";

        return $sql;

    }

    public function getUnidadeMedidaPorId($co_unidade_medida)
    {
        if($co_unidade_medida)
        {
            $sql = "SELECT co_unidade_medida, ds_sigla, ds_unidade_medida, co_status FROM demandas.unidade_medida
                    WHERE co_unidade_medida = $co_unidade_medida";
            
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
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
            'co_unidade_medida' => array('Digits'),
            'ds_sigla' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'ds_unidade_medida' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'nu_cpf_inclusao' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'dt_inclusao' => array(),
            'co_status' => array('Digits'),
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

}
