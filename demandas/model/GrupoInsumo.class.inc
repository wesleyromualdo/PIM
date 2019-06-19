<?php

class GrupoInsumo_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandas.grupo_insumo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('co_grupo_insumo');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('co_grupo_insumo', 'nu_codigo_grupo', 'ds_grupo_insumo', 'co_grupo_insumo_pai', 'nu_cpf_inclusao', 'dt_inclusao', 'co_status');

    protected $arAtributos = array(
        'co_grupo_insumo' => null,
        'nu_codigo_grupo' => null,
        'ds_grupo_insumo' => null,
        'co_grupo_insumo_pai' => null,
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

    public function listaGrupoInsumo( $arrPost )
    {
        $filtro = '';
        
        if( $arrPost['ds_grupo_insumo'] ){
            $filtro .= " AND ds_grupo_insumo ILIKE removeAcento('%".$arrPost['ds_grupo_insumo']."%') ";
        }

        if( $arrPost['nu_codigo_grupo'] ){
            $filtro .= " AND nu_codigo_grupo ILIKE '%".$arrPost['nu_codigo_grupo']."%' ";
        }

        $sql = "select 
                    gi.co_grupo_insumo,
                    gi.nu_codigo_grupo,
                    gi.ds_grupo_insumo,
                    CASE WHEN (gip.nu_codigo_grupo IS NOT NULL)
                    THEN gip.nu_codigo_grupo ELSE '-'
                    END as nu_codigo_grupo_pai,
                    
                    CASE WHEN (gip.ds_grupo_insumo IS NOT NULL)
                    THEN gip.ds_grupo_insumo  ELSE '-'
                    END as ds_grupo_insumo_pai
                from
                    demandas.grupo_insumo gi
                    left join demandas.grupo_insumo gip on gi.co_grupo_insumo = gip.co_grupo_insumo_pai and gip.co_status = 1
                where
                    gi.co_status = 1 {$filtro}";

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }

    public function getComboGrupoInsumo()
    {
        $sql = "SELECT co_grupo_insumo as codigo, COALESCE(nu_codigo_grupo || ' - ' || ds_grupo_insumo) as descricao FROM demandas.grupo_insumo WHERE co_status = 1 ";

        return $sql;
    }

    public function listaGrupoInsumoPai()
    {
        $sql = "SELECT co_grupo_insumo as codigo, COALESCE(nu_codigo_grupo || ' - ' || ds_grupo_insumo) as descricao FROM demandas.grupo_insumo WHERE co_status = 1 AND co_grupo_insumo_pai is null";

        return $sql;
    }

    public function listaGrupoInsumoFilhoPorPaiID($co_grupo_insumo)
    {
        $sql = "SELECT co_grupo_insumo as codigo, COALESCE(nu_codigo_grupo || ' - ' || ds_grupo_insumo) as descricao FROM demandas.grupo_insumo WHERE co_status = 1 AND co_grupo_insumo_pai = $co_grupo_insumo ";

        $arrDados = $this->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;

        return $sql;
    }
    
    public function getGrupoInsumoPorId( $co_grupo_insumo ) {

        if($co_grupo_insumo){
            $sql = "SELECT co_grupo_insumo, nu_codigo_grupo, ds_grupo_insumo, co_grupo_insumo_pai, co_status
                    FROM demandas.grupo_insumo
                    WHERE co_grupo_insumo = $co_grupo_insumo";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }
    
}