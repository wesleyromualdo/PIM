<?php

use mvc\Controle;

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
    protected $arAtributos = array(
        'co_insumo' => null,
        'nu_codigo_insumo' => null,
        'ds_insumo' => null,
        'co_unidade_medida' => null,
        'co_piso_insumo' => null,
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

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'co_insumo' => array('Digits'),
            'nu_codigo_insumo' => array(new Zend_Validate_StringLength(array('max' => 3))),
            'ds_insumo' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'co_unidade_medida' => array('Digits'),
            'co_tipo_insumo' => array('Digits'),
            'co_grupo_insumo' => array('Digits'),
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
        $resposta = array();
//        if ( empty($this->co_grupo_insumo) ) {
//            $resposta = array('erro' => true, 'msg' => "O campo Subgrupo responsável é obrigatório!");
//        } else if ( empty($this->nu_codigo_insumo) ) {
//            $resposta = array('erro' => true, 'msg' => "O campo Código do insumo é obrigatório!");
//        } else if ( empty($this->ds_insumo) ) {
//            $resposta = array('erro' => true, 'msg' => "O campo Descrição do insumo é obrigatório!");
//        } else if ( empty($this->co_unidade_medida) )  {
//            $resposta = array('erro' => true, 'msg' => "O campo Unidade de medida é obrigatório!");
//        } else if ( empty($this->co_tipo_insumo) )  {
//            $resposta = array('erro' => true, 'msg' => "O campo Tipo é obrigatório!");
//        } else if ( ($this->co_tipo_insumo == 1) && empty($this->co_piso_insumo) )  {
//            $resposta = array('erro' => true, 'msg' => "O campo Piso é obrigatório!");
//        } else if  ( !empty($this->nu_codigo_insumo) && !empty($this->co_grupo_insumo))  {
//            $codigoInsumoExiste = $this->getInsumoCodigoSubgrupoId($_POST);
//            if ($codigoInsumoExiste){
//                $resposta = array('erro' => true, 'msg' => "O código do insumo inserido já existe!");
//            }
//        } else {
//            $resposta = array('erro' => false, 'msg' => "");
//        }
//        return false;
//        return $resposta;

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function listaInsumo( $arrPost )
    {
        $filtro = ' im.co_status = 1 ';

        if( isset($arrPost['co_status']) ){
            if($arrPost['co_status'] != '-1'){
                $filtro = " im.co_status = '".$arrPost['co_status']."' ";
            } else {
                $filtro = " (im.co_status = 1 OR im.co_status = 0) ";
            }
        }

        if( isset($arrPost['co_grupo_insumo']) ){
            $filtro .= " AND im.co_grupo_insumo = {$arrPost['co_grupo_insumo']} ";
        }

        if( isset($arrPost['ds_insumo']) ){
            $filtro .= " AND ds_insumo ILIKE removeAcento('%".$arrPost['ds_insumo']."%') ";
        }

        if( isset($arrPost['nu_codigo_insumo']) ){
            $filtro .= " AND nu_codigo_insumo ILIKE '%".$arrPost['nu_codigo_insumo']."%' ";
        }

        $sql = "SELECT 
                    im.co_insumo, 
                    im.nu_codigo_insumo, 
                    im.ds_insumo,
                    COALESCE(um.ds_unidade_medida || ' (' || um.ds_sigla || ')' ) as ds_unidade_medida,
                    ti.ds_tipo_insumo,
                    COALESCE(gi.nu_codigo_grupo || ' - ' || gi.ds_grupo_insumo) as ds_grupo_insumo,
                    im.co_status
                FROM 
                    demandas.insumo im
                inner join 
                    demandas.unidade_medida um on um.co_unidade_medida = im.co_unidade_medida
                left join 
                    demandas.piso_insumo pi on pi.co_piso_insumo = im.co_piso_insumo
                inner join 
                    demandas.tipo_insumo ti on ti.co_tipo_insumo = im.co_tipo_insumo
                inner join 
                    demandas.grupo_insumo gi on gi.co_grupo_insumo = im.co_grupo_insumo
                WHERE {$filtro} ";

        if(isset($arrPost['returnsql'])) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
    }

    public function listaInsumoPorSubgrupo($arrPost)
    {
        $filtro = ' im.co_status = 1 ';

        if( isset($arrPost['co_grupo_insumo']) ){
            $filtro .= " AND im.co_grupo_insumo = {$arrPost['co_grupo_insumo']} ";
        }

        $sql = "select
                    im.co_insumo,
                    (
                    select
                        coalesce(grupo_pai.nu_codigo_grupo || '.' || gi.nu_codigo_grupo || '.' || im.nu_codigo_insumo)
                    from
                        demandas.grupo_insumo gi
                    inner join (
                        select
                            gip.co_grupo_insumo,
                            gip.nu_codigo_grupo,
                            gip.co_grupo_insumo_pai
                        from
                            demandas.grupo_insumo gip
                        where
                            gip.co_status = 1
                            and gip.co_grupo_insumo_pai is null ) as grupo_pai on
                        grupo_pai.co_grupo_insumo = gi.co_grupo_insumo_pai
                        and gi.co_status = 1
                        and gi.co_grupo_insumo = {$arrPost['co_grupo_insumo']}
                    order by
                        grupo_pai.nu_codigo_grupo
                    limit 1 ) as nu_codigo_insumo,
                    im.ds_insumo,
                    coalesce(um.ds_unidade_medida || ' (' || um.ds_sigla || ')' ) as ds_unidade_medida,
                    ti.ds_tipo_insumo
                from
                    demandas.insumo im
                inner join demandas.unidade_medida um on
                    um.co_unidade_medida = im.co_unidade_medida
                left join demandas.piso_insumo pi on
                    pi.co_piso_insumo = im.co_piso_insumo
                inner join demandas.tipo_insumo ti on
                    ti.co_tipo_insumo = im.co_tipo_insumo
                inner join demandas.grupo_insumo gi on
                    gi.co_grupo_insumo = im.co_grupo_insumo
                where
                     {$filtro}
                order by
                    im.nu_codigo_insumo asc
                 ";

        $listagemSimec = new Simec_Listagem();

        $arrayCabecalho = array('Código', 'Insumo', 'Unidade de medida', 'Tipo');
        $esconderColunas = array('co_insumo', 'ds_grupo_insumo', 'co_status');
        $arrFiltro['returnsql'] = true;
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('edit', array('func' => 'abrirInsumo'));
        //$listagemSimec->addAcao('delete', array('func' => 'deletarInsumo'));
        $listagemSimec->setFormFiltros('formulario-insumo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);

        return $listagemSimec;
    }

    public function getInsumoPorId($co_insumo) {

        if($co_insumo){
            $sql = "SELECT 
                        im.co_insumo, 
                        im.nu_codigo_insumo, 
                        im.ds_insumo, 
                        im.co_status,
                        um.co_unidade_medida, 
                        um.ds_sigla, 
                        um.ds_unidade_medida,
                        pi.co_piso_insumo, 
                        pi.ds_piso_insumo,
                        ti.co_tipo_insumo, 
                        ti.ds_tipo_insumo,
                        gi.co_grupo_insumo, 
                        gi.ds_grupo_insumo
                    FROM 
                        demandas.insumo im
                    inner join 
                        demandas.unidade_medida um on um.co_unidade_medida = im.co_unidade_medida
                    left join 
                        demandas.piso_insumo pi on pi.co_piso_insumo = im.co_piso_insumo
                    inner join 
                        demandas.tipo_insumo ti on ti.co_tipo_insumo = im.co_tipo_insumo
                    inner join 
                        demandas.grupo_insumo gi on gi.co_grupo_insumo = im.co_grupo_insumo
                    WHERE 
                        im.co_insumo = $co_insumo";

            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }

    public function getInsumoCodigoSubgrupoId($dados) {

        $filtro = '';
        $co_insumo = $dados['co_insumo'];
        $co_grupo_insumo = $dados['co_grupo_insumo'];
        $nu_codigo_insumo = $dados['nu_codigo_insumo'];

        // se estiver alterando um insumo
        if ( !empty($co_grupo_insumo) && !empty($co_insumo) ){
            $filtro .= " AND im.co_insumo <> {$co_insumo} AND im.co_grupo_insumo = {$co_grupo_insumo} ";
        }

        // se estiver incluindo um novo insumo
        if ( !empty($co_grupo_insumo) && !isset($co_insumo) && empty($co_insumo) ) {
            $filtro .= " AND im.co_grupo_insumo = {$co_grupo_insumo} ";
        }

        $sql = "SELECT 
                    im.co_insumo, 
                    im.nu_codigo_insumo, 
                    im.ds_insumo, 
                    im.co_status, 
                    im.co_grupo_insumo
                FROM 
                    demandas.insumo im
                WHERE 
                    im.nu_codigo_insumo = '{$nu_codigo_insumo}' {$filtro} ";

        $arrDados = $this->pegaLinha($sql);

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }

    public function validaCadastroInsumo($dados)
    {
        if ( empty($dados['co_grupo_insumo']) ) {
            $resposta = array('erro' => true, 'msg' => "O campo Subgrupo responsável é obrigatório!");
        } else if ( empty($dados['nu_codigo_insumo']) ) {
            $resposta = array('erro' => true, 'msg' => "O campo Código do insumo é obrigatório!");
        } else if ( empty($dados['ds_insumo']) ) {
            $resposta = array('erro' => true, 'msg' => "O campo Descrição do insumo é obrigatório!");
        } else if ( empty($dados['co_unidade_medida']) )  {
            $resposta = array('erro' => true, 'msg' => "O campo Unidade de medida é obrigatório!");
        } else if ( empty($dados['co_tipo_insumo']) )  {
            $resposta = array('erro' => true, 'msg' => "O campo Tipo é obrigatório!");
        } else if ( ($dados['co_tipo_insumo'] == 1) && empty($dados['co_piso_insumo']) )  {
            $resposta = array('erro' => true, 'msg' => "O campo Piso é obrigatório!");
        } else if ( !empty($dados['nu_codigo_insumo']) && !empty($dados['co_grupo_insumo']))  {
            $codigoInsumoExiste = $this->getInsumoCodigoSubgrupoId($dados);
            if (!empty($codigoInsumoExiste)){
                $resposta = array('erro' => true, 'msg' => "O código do insumo inserido já existe!");
            } else {
                $resposta = array('erro' => false, 'msg' => "");
            }
        } else {
            $resposta = array('erro' => false, 'msg' => "");
        }

        return $resposta;

    }

//    public function salvarInsumo($dados){
//
//        $arrInsumo = array();
//
//        // @TODO método para validar
//        if ( $dados['co_tipo_insumo'] == 1 && empty($dados['co_piso_insumo']) ){
//            $this->registrarMensagemSistema("O campo Piso é obrigatório!", mvc\Controle::msgNegativa);
//            $this->redirecionarTela();
//        }
//        // @TODO validar regra ao alterar tipo de insumo para material
//        if ( $dados['co_tipo_insumo'] == 2 && $dados['co_insumo'] ){
//            $dados['co_piso_insumo'] = NULL;
//        }
//
//        $arrInsumo['nu_cpf_inclusao'] = $dados['usucpf'];
//        $arrInsumo['nu_codigo_insumo'] = $dados['nu_codigo_insumo'];
//        $arrInsumo['ds_insumo'] = $dados['ds_insumo'];
//        $arrInsumo['co_unidade_medida'] = $dados['co_unidade_medida'];
//        $arrInsumo['co_tipo_insumo'] = $dados['co_tipo_insumo'];
//        $arrInsumo['co_piso_insumo'] = $dados['co_piso_insumo'];
//        $arrInsumo['co_grupo_insumo'] = $dados['co_grupo_insumo'];
//        $arrInsumo['nu_cpf_inclusao'] = $_SESSION['usucpf'];
//        $arrInsumo['dt_inclusao'] = $dados['co_status'];
//
//        $this->model->popularDadosObjeto($arrInsumo)->salvar();
//        $this->model->commit();
//    }

}