<?php

class Servico_Model extends Modelo
{
    protected $stNomeTabela = 'demandas.servico';

    protected $arChavePrimaria = array(
        'co_servico',
    );

    protected $arChaveEstrangeira = array(
        'co_grupo_servico' => array('tabela' => 'demandas.grupo_servico', 'pk' => 'co_grupo_servico'),
        'co_unidade_medida' => array('tabela' => 'demandas.unidade_medida', 'pk' => 'co_unidade_medida'),
    );

    protected $arAtributos = array(
        'co_servico' => null,
        'nu_codigo_servico' => null,
        'ds_servico' => null,
        'co_unidade_medida' => null,
        'co_grupo_servico' => null,
        'nu_cpf_inclusao' => null,
        'dt_inclusao' => null,
        'co_status' => null,
    );

    public function listaServico( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['nu_codigo_servico'] ){
            $filtro .= " AND sv.nu_codigo_servico ILIKE '%".$arrPost['nu_codigo_servico']."%' ";
        }

        if( $arrPost['ds_servico'] ){
            $filtro .= " AND sv.ds_servico ILIKE '%".$arrPost['ds_servico']."%' ";
        }

        if( $arrPost['co_unidade_medida'] ){
            $filtro .= " AND um.co_unidade_medida = '".$arrPost['co_unidade_medida']."' ";
        }

        if( $arrPost['co_grupo_servico'] ){
            $filtro .= " AND es.co_grupo_servico = '".$arrPost['co_grupo_servico']."' ";
        }

        if( $arrPost['co_servico'] ){
            $filtro .= " AND es.co_servico = '".$arrPost['co_servico']."' ";
        }
        
        $sql = "SELECT sv.co_servico, sv.nu_codigo_servico, sv.ds_servico, um.ds_unidade_medida, es.ds_grupo_servico 
                FROM demandas.servico sv
                LEFT JOIN demandas.unidade_medida um ON um.co_unidade_medida = sv.co_unidade_medida
                LEFT JOIN demandas.grupo_servico es ON es.co_grupo_servico = sv.co_grupo_servico
                WHERE sv.co_status = 1 {$filtro}";
        

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }

    public function getComboUnidadeMedida()
    {
        $sql = "SELECT co_unidade_medida as codigo, ds_unidade_medida as descricao FROM demandas.unidade_medida WHERE co_status = 1";

        return $sql;
    }

    public function getComboGrupoServico()
    {
        $sql = "SELECT co_grupo_servico, COALESCE(ds_grupo_servico || ' (' || nu_codigo_grupo_servico || ')' ) as descricao  FROM demandas.grupo_servico WHERE co_status = 1";

        return $sql;
    }

    public function getServicoPorId($co_servico)
    {
        if($co_servico)
        {
            $sql = "SELECT sv.co_servico, sv.nu_codigo_servico, sv.ds_servico, um.ds_unidade_medida, es.ds_grupo_servico, 
                um.co_unidade_medida, es.co_grupo_servico
                FROM demandas.servico sv
                LEFT JOIN demandas.unidade_medida um ON um.co_unidade_medida = sv.co_unidade_medida
                LEFT JOIN demandas.grupo_servico es ON es.co_grupo_servico = sv.co_grupo_servico
                WHERE sv.co_status = 1 {$filtro}";

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
            'co_servico' => array('Digits'),
            'nu_codigo_servico' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'ds_servico' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'co_unidade_medida' => array('allowEmpty' => true,'Digits'),
            'co_grupo_servico' => array('allowEmpty' => true,'Digits'),
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
