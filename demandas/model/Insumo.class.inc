<?php

class Insumo_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.insumo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('co_insumo');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('co_insumo', 'nu_codigo_insumo', 'ds_insumo', 'co_unidade_medida', 'co_piso_insumo', 'co_tipo_insumo', 'co_grupo_insumo', 'nu_cpf_inclusao', 'dt_inclusao', 'co_status');

    protected $arAtributos = array(
        'co_insumo' => null,
        'nu_codigo_insumo' => null,
        'ds_insumo' => null,
        'co_unidade_medida' => null,
        //'co_piso_insumo' => null,
        'co_tipo_insumo' => null,
        'co_grupo_insumo' => null,
        'nu_cpf_inclusao' => null,
        'dt_inclusao' => null,
        'co_status' => null
    );
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaInsumo( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['ds_insumo'] ){
            $filtro .= " AND ds_insumo ILIKE removeAcento('%".$arrPost['ds_insumo']."%') ";
        }

        if( $arrPost['nu_codigo_insumo'] ){
            $filtro .= " AND nu_codigo_insumo ILIKE '%".$arrPost['nu_codigo_insumo']."%' ";
        }

        $sql = "SELECT 
                    im.co_insumo, im.nu_codigo_insumo, im.ds_insumo,
                    COALESCE(um.ds_unidade_medida || ' (' || um.ds_sigla || ')' ) as ds_unidade_medida,
                    ti.ds_tipo_insumo,
                    COALESCE(gi.nu_codigo_grupo || ' - ' || gi.ds_grupo_insumo) as ds_grupo_insumo 
                FROM demandas.insumo im
                inner join demandas.unidade_medida um on um.co_unidade_medida = im.co_unidade_medida
                left join demandas.piso_insumo pi on pi.co_piso_insumo = im.co_piso_insumo
                inner join demandas.tipo_insumo ti on ti.co_tipo_insumo = im.co_tipo_insumo
                inner join demandas.grupo_insumo gi on gi.co_grupo_insumo = im.co_grupo_insumo
                WHERE im.co_status = 1
                {$filtro} ";

        //$sql = "SELECT co_insumo, nu_codigo_insumo, ds_insumo FROM demandas.insumo WHERE co_status = 1 {$filtro}";

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }

    public function getInsumoPorId($co_insumo) {

        if($co_insumo){
            $sql = "SELECT 
                        im.co_insumo, im.nu_codigo_insumo, im.ds_insumo, im.co_status,
                        um.co_unidade_medida, um.ds_sigla, um.ds_unidade_medida,
                        pi.co_piso_insumo, pi.ds_piso_insumo,
                        ti.co_tipo_insumo, ti.ds_tipo_insumo,
                        gi.co_grupo_insumo, gi.ds_grupo_insumo
                    FROM demandas.insumo im
                    inner join demandas.unidade_medida um on um.co_unidade_medida = im.co_unidade_medida
                    left join demandas.piso_insumo pi on pi.co_piso_insumo = im.co_piso_insumo
                    inner join demandas.tipo_insumo ti on ti.co_tipo_insumo = im.co_tipo_insumo
                    inner join demandas.grupo_insumo gi on gi.co_grupo_insumo = im.co_grupo_insumo
                    WHERE im.co_insumo = $co_insumo";

            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }
    
}