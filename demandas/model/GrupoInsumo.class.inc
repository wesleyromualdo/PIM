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
    protected $arAtributos = array(
        'co_grupo_insumo' => null,
        'nu_codigo_grupo' => null,
        'ds_grupo_insumo' => null,
        //'co_grupo_insumo_pai' => null,
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

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'co_grupo_insumo' => array('Digits'),
            'nu_codigo_grupo' => array(new Zend_Validate_StringLength(array('max' => 3))),
            'ds_grupo_insumo' => array(new Zend_Validate_StringLength(array('max' => 200))),
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

    public function listaGrupoInsumo( $arrPost )
    {
        $filtro = ' gi.co_status = 1 ';

        if( isset($arrPost['co_status']) ){
            if($arrPost['co_status'] != '-1'){
                $filtro = " gi.co_status = '".$arrPost['co_status']."' ";
            } else {
                $filtro = " (gi.co_status = 1 OR gi.co_status = 0) ";
            }
        }

        if( isset($arrPost['ds_grupo_insumo']) ){
            $filtro .= " AND gi.ds_grupo_insumo ILIKE removeAcento('%".$arrPost['ds_grupo_insumo']."%') ";
        }

        if( isset($arrPost['nu_codigo_grupo']) ){
            $filtro .= " AND gi.nu_codigo_grupo ILIKE '%".$arrPost['nu_codigo_grupo']."%' ";
        }

//        $sql = "select
//                    gi.co_grupo_insumo,
//                    gi.nu_codigo_grupo,
//                    gi.ds_grupo_insumo,
//                    CASE WHEN (gip.nu_codigo_grupo IS NOT NULL)
//                    THEN gip.nu_codigo_grupo ELSE '-'
//                    END as nu_codigo_grupo_pai,
//                    CASE WHEN (gip.ds_grupo_insumo IS NOT NULL)
//                    THEN gip.ds_grupo_insumo  ELSE '-'
//                    END as ds_grupo_insumo_pai
//                from
//                    demandas.grupo_insumo gi
//                    left join demandas.grupo_insumo gip on gi.co_grupo_insumo = gip.co_grupo_insumo_pai and gip.co_status = 1
//                where
//                    gi.co_status = 1 {$filtro} order by gi.nu_codigo_grupo asc";

        $sql = "select 
                    gi.co_grupo_insumo,
                    gi.nu_codigo_grupo,
                    gi.ds_grupo_insumo,
                    gi.co_status
                from
                    demandas.grupo_insumo gi
                where
                    {$filtro} 
                    AND gi.co_grupo_insumo_pai is null 
                    order by gi.nu_codigo_grupo asc";

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }

    public function listaSubgrupoInsumo( $arrPost )
    {
        $filtro = '';

        if( isset($arrPost['co_grupo_insumo']) ){
            $filtro .= " AND grupo_pai.co_grupo_insumo_pai = ".$arrPost['co_grupo_insumo'] ."";
        }

//        $sql = "select
//                    gi.co_grupo_insumo,
//                    gi.nu_codigo_grupo,
//                    gi.ds_grupo_insumo
//                from
//                    demandas.grupo_insumo gi
//                where
//                    gi.co_status = 1 {$filtro}
//                    AND gi.co_grupo_insumo_pai is not null
//                order by
//                    gi.nu_codigo_grupo asc";

        $sql = "select
                    grupo_pai.co_grupo_insumo as co_grupo_insumo,
                    coalesce(gi.nu_codigo_grupo || '.' || grupo_pai.nu_codigo_grupo ) as nu_codigo_grupo,
                    grupo_pai.ds_grupo_insumo as ds_grupo_insumo
                from
                    demandas.grupo_insumo gi
                cross join lateral (
                        select 
                            gip.co_grupo_insumo,
                            gip.nu_codigo_grupo,
                            gip.ds_grupo_insumo,
                            gip.co_grupo_insumo_pai
                        from
                            demandas.grupo_insumo gip
                        where
                            gip.co_status = 1
                            and gi.co_grupo_insumo = gip.co_grupo_insumo_pai
                        ) as grupo_pai
                where
                    gi.co_status = 1 {$filtro}
                    and grupo_pai.co_grupo_insumo_pai is not null 
                order by 
                    grupo_pai.nu_codigo_grupo";

        return $sql;

    }

    public function listaSubgrupoInsumoPorGrupo($arrFiltro)
    {

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Descrição do Subgrupo');
        $esconderColunas = array('co_grupo_insumo', 'co_status');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($this->listaSubgrupoInsumo($arrFiltro));
        $listagemSimec->addAcao('plus', array('func' => 'listarInsumoSubgrupo'));
        $listagemSimec->addAcao('edit', array('func' => 'abrirGrupoInsumo'));
        $listagemSimec->addAcao('delete', array('func' => 'deletarSubgrupoInsumo'));
        $listagemSimec->setFormFiltros('formulario-subgrupo-insumo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);

        return $listagemSimec;
    }

    public function getComboGrupoInsumo()
    {
        $sql = "SELECT 
                    co_grupo_insumo as codigo, 
                    COALESCE(nu_codigo_grupo || ' - ' || ds_grupo_insumo) as descricao 
                FROM 
                    demandas.grupo_insumo 
                WHERE 
                    co_status = 1 ";

        return $sql;
    }

    public function getComboGrupoInsumoFilho()
    {
        $sql = "select
                    grupo_pai.co_grupo_insumo as codigo,
                    coalesce(grupo_pai.nu_codigo_grupo || '.' || gi.nu_codigo_grupo || ' - ' || gi.ds_grupo_insumo) as descricao
                from
                    demandas.grupo_insumo gi
                cross join lateral (
                        select 
                            gip.co_grupo_insumo,
                            gip.nu_codigo_grupo,
                            gip.ds_grupo_insumo,
                            gip.co_grupo_insumo_pai
                        from
                            demandas.grupo_insumo gip
                        where
                            gip.co_status = 1
                            and gi.co_grupo_insumo = gip.co_grupo_insumo_pai
                        ) as grupo_pai
                where
                    gi.co_status = 1
                    and grupo_pai.co_grupo_insumo_pai is not null order by grupo_pai.nu_codigo_grupo; ";

        return $sql;
    }

    public function listaGrupoInsumoPai($arrPost)
    {
        $filtro = '';

        if( isset($arrPost['co_grupo_insumo']) ){
            $filtro .= " AND co_grupo_insumo <> {$arrPost['co_grupo_insumo']} ";
        }

        $sql = "SELECT 
                    co_grupo_insumo as codigo, 
                    COALESCE(nu_codigo_grupo || ' - ' || ds_grupo_insumo) as descricao 
                FROM 
                    demandas.grupo_insumo 
                WHERE 
                    co_status = 1 
                    AND co_grupo_insumo_pai is null 
                    {$filtro} 
                order by 
                    nu_codigo_grupo asc";

        return $sql;
    }

    public function listaSubgrupoInsumoGrupoID($co_grupo_insumo)
    {
        if ($co_grupo_insumo){

            $sql = "SELECT 
                        co_grupo_insumo as codigo, 
                        COALESCE(nu_codigo_grupo || ' - ' || ds_grupo_insumo) as descricao 
                    FROM 
                        demandas.grupo_insumo 
                    WHERE 
                        co_status = 1 
                        AND co_grupo_insumo_pai = $co_grupo_insumo ";

            $arrDados = $this->carregar($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }
    
    public function getGrupoInsumoPorId( $co_grupo_insumo ) {

        if ($co_grupo_insumo){

            $sql = "SELECT 
                        co_grupo_insumo, 
                        nu_codigo_grupo, 
                        ds_grupo_insumo, 
                        co_grupo_insumo_pai, 
                        co_status 
                    FROM 
                        demandas.grupo_insumo 
                    WHERE 
                        co_grupo_insumo = $co_grupo_insumo";

            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }

    public function getGrupoInsumoPorCodigoId( $dados ) {

        $filtro = '';
        $co_grupo_insumo = $dados['co_grupo_insumo'];
        $nu_codigo_grupo = $dados['nu_codigo_grupo'];
        $co_grupo_insumo_pai = $dados['co_grupo_insumo_pai'];

        // valida no cadastro
        if ( empty($co_grupo_insumo) ){
            if ( !empty($co_grupo_insumo_pai) ){
                // valida um subgrupo existente
                $resSubgrupo = $this->getSubgrupoInsumoPorCodigoId($dados);
                //ver($resSubgrupo);
                if(!empty($resSubgrupo)){
                    return $resSubgrupo;
                }
            } else {
                $filtro .= " AND co_grupo_insumo_pai is null";
            }
        }

        // valida na edição
        if ( !empty($co_grupo_insumo) ){
            if ( !empty($co_grupo_insumo_pai) ){
                // valida primeiro um subgrupo existente
                $resSubgrupo = $this->getSubgrupoInsumoPorCodigoId($dados);
                //ver($resSubgrupo);
                if (!empty($resSubgrupo)){
                    return $resSubgrupo;
                }
            } else {
                $filtro .= " AND co_grupo_insumo <> {$co_grupo_insumo} AND co_grupo_insumo_pai is null";
            }
        }


        if (!empty($nu_codigo_grupo) && empty($co_grupo_insumo_pai) ){

            $sql = "SELECT 
                        co_grupo_insumo, 
                        nu_codigo_grupo, 
                        ds_grupo_insumo, 
                        co_grupo_insumo_pai, 
                        co_status
                    FROM 
                        demandas.grupo_insumo 
                    WHERE 
                        nu_codigo_grupo = '{$nu_codigo_grupo}'
                        {$filtro} ";

            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }

    public function getSubgrupoInsumoPorCodigoId( $dados ) {

        $filtro = '';
        $co_grupo_insumo = $dados['co_grupo_insumo'];
        $nu_codigo_grupo = $dados['nu_codigo_grupo'];
        $co_grupo_insumo_pai = $dados['co_grupo_insumo_pai'];

        if($co_grupo_insumo){
            $filtro .= " AND co_grupo_insumo <> {$co_grupo_insumo} ";
        }

//        if($co_grupo_insumo_pai){
//            $filtro .= " AND co_grupo_insumo <> {$co_grupo_insumo_pai} ";
//        }

        if ($nu_codigo_grupo){

            $sql = "SELECT 
                        co_grupo_insumo, 
                        nu_codigo_grupo, 
                        ds_grupo_insumo, 
                        co_grupo_insumo_pai, 
                        co_status
                    FROM 
                        demandas.grupo_insumo 
                    WHERE 
                        nu_codigo_grupo = '{$nu_codigo_grupo}'
                        AND co_grupo_insumo_pai = {$co_grupo_insumo_pai}
                        {$filtro} ";

            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }

    public function validaCadastroGrupoInsumo($dados)
    {
        $resposta = array();
        $cadSubgrupo = false;
        $editGrupo = false;

        // se existir o campo co_grupo_insumo_pai então
        if ( isset($dados['co_grupo_insumo_pai'] ) && !empty($dados['co_grupo_insumo_pai'] ) ) {
            $cadSubgrupo = true;
        }
        if ( !isset($dados['co_grupo_insumo_pai'] ) && !empty($dados['co_grupo_insumo'] ) ) {
            $editGrupo = true;
        }

        if ( empty($dados['nu_codigo_grupo']) ) {
            $resposta = array('erro' => true, 'msg' => "O campo código do grupo é obrigatório!");
        }  else if ( empty($dados['ds_grupo_insumo']) ) {
            $resposta = array('erro' => true, 'msg' => "O campo descrição do grupo é obrigatório!");
        } else if  ( !empty($dados['nu_codigo_grupo']) )  {
            $codigoExiste = $this->getGrupoInsumoPorCodigoId($dados);
            if ($codigoExiste){
                $resposta = array('erro' => true, 'msg' => "O código do grupo inserido já existe!");
            } else {
                $resposta = array('erro' => false, 'msg' => "");
            }
        } else {
            $resposta = array('erro' => false, 'msg' => "");
        }

        return $resposta;
    }
    
}