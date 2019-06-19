<?php
 
class GrupoServico_Model extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'demandas.grupo_servico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'co_grupo_servico',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'co_grupo_servico_pai' => array('tabela' => 'demandas.grupo_servico', 'pk' => 'co_grupo_servico'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'co_grupo_servico' => null,
        'nu_codigo_grupo_servico' => null,
        'ds_grupo_servico' => null,
        'co_grupo_servico_pai' => null,
        'nu_cpf_inclusao' => null,
        'dt_inclusao' => null,
        'co_status' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'co_grupo_servico' => array('Digits'),
            'nu_codigo_grupo_servico' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'ds_grupo_servico' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'co_grupo_servico_pai' => array('allowEmpty' => true,'Digits'),
            'nu_cpf_inclusao' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'dt_inclusao' => array(),
            'co_status' => array('Digits'),
        );
    }

    public function antesSalvar()
    {
        return parent::antesSalvar();
    }

    public function Get( $arrFiltro )
    {
        $arrDados = $this->carregar(listaFiltroSql($arrFiltro));
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados; 
    }

    public function listaFiltroSql( $arrFiltro )
    {
        $filtro = ' grupo.co_status = 1 ';

        if( isset($arrFiltro['co_status']) ){
            if($arrFiltro['co_status'] != '-1'){
                $filtro = " grupo.co_status = '".$arrFiltro['co_status']."' ";
            } else {
                $filtro = " (grupo.co_status = 1 OR grupo.co_status = 0) ";
            }
        }

        if( $arrFiltro['nu_codigo_grupo_servico'] ){
            $filtro .= " and grupo.nu_codigo_grupo_servico ilike removeAcento('%".$arrFiltro['nu_codigo_grupo_servico']."%')";
        }

        if( $arrFiltro['ds_grupo_servico'] ){
            $filtro .= " and grupo.ds_grupo_servico ilike removeAcento('%".$arrFiltro['ds_grupo_servico']."%')";
        }

        if( $arrFiltro['co_grupo_servico'] ){
            $filtro .= " and grupo.co_grupo_servico = ".$arrFiltro['co_grupo_servico'] ."";
        }

        if( $arrFiltro['co_grupo_servico_pai'] ){
            $filtro .= " and grupo.co_grupo_servico_pai = ".$arrFiltro['co_grupo_servico_pai'] ."";
        }

        if( $arrFiltro['exibe_somente_pai'] ){
            $filtro .= " and grupo.co_grupo_servico_pai IS NULL ";
        }
        
        $sql = " SELECT
                    grupo.co_grupo_servico ,
                    grupo.nu_codigo_grupo_servico, 
                    grupo.ds_grupo_servico,
                CASE WHEN grupo.co_status = 1 
                    THEN  'Ativo' 
                    ELSE 'Inativo'
                END AS co_status ,
                CASE WHEN pai.co_grupo_servico IS NULL 
                    THEN  '-' 
                    ELSE pai.co_grupo_servico || ''
                END AS co_grupo_servico_pai ,
                CASE WHEN pai.ds_grupo_servico IS NULL 
                    THEN  '-' 
                    ELSE pai.ds_grupo_servico || ''
                END  as cs_grupo_servico_pai , 
                CASE WHEN grupo.co_grupo_servico_pai IS NULL 
                    THEN grupo.co_grupo_servico || '' 
                    ELSE grupo.co_grupo_servico_pai  || '-' || grupo.co_grupo_servico
                END AS ordem
                FROM demandas.grupo_servico grupo 
                LEFT JOIN demandas.grupo_servico pai  ON grupo.co_grupo_servico_pai  =  pai.co_grupo_servico
                WHERE  {$filtro}
                ORDER BY co_grupo_servico";

        return $sql;
    }

    public function listarGrupoServico ($arrFiltro){

        $listagemSimec = new Simec_Listagem();

        $arrayCabecalho = array('Código', 'Grupo Serviço', 'Status');
        $esconderColunas = array('co_grupo_servico', 'cs_grupo_servico_pai', 'co_grupo_servico_pai','ordem');
        $arrFiltro['exibe_somente_pai'] = true;

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->listaFiltroSql($arrFiltro));
        $listagemSimec->addAcao('plus', array('func' => 'listarGrupoFilhoPorPai', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->addAcao('edit', array('func' => 'abrirGrupoServico', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarGrupoServico', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->setFormFiltros('formulario-cardapio-tipo');
        $listagemSimec->setAcaoComoCondicional('delete', [['campo' => 'co_status', 'valor' => 'Ativo', 'op' => 'igual']]);
        $listagemSimec->setAcaoComoCondicional('edit', [['campo' => 'co_status', 'valor' => 'Ativo', 'op' => 'igual']]);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);

        return $listagemSimec;
    }

    public function listarGrupoFilhoPorPai ($co_grupo_servico_pai){

        $listagemSimec = new Simec_Listagem();

        $arrFiltro = array('co_grupo_servico_pai' =>$co_grupo_servico_pai);
        $arrayCabecalho = array('Código', 'Subgrupo Serviço', 'Status');
        $esconderColunas = array('co_grupo_servico', 'cs_grupo_servico_pai', 'co_grupo_servico_pai','ordem');

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($this->listaFiltroSql($arrFiltro));
        $listagemSimec->addAcao('plus', array('func' => 'listarServicosPorGrupo', 'extra-params' => array('co_grupo_servico') ));
        $listagemSimec->addAcao('edit', array('func' => 'abrirGrupoServico', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarGrupoServico', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->setAcaoComoCondicional('delete', [['campo' => 'co_status', 'valor' => 'Ativo', 'op' => 'igual']]);
        $listagemSimec->setAcaoComoCondicional('edit', [['campo' => 'co_status', 'valor' => 'Ativo', 'op' => 'igual']]);
        $listagemSimec->setFormFiltros('formulario-cardapio-tipo');
        $listagemSimec->setCampos($arrayCabecalho);

        return $listagemSimec;

    }


    public function listarServicosPorGrupo ($co_grupo_servico){

        $listagemSimec = new Simec_Listagem();
        $modelServico = new Servico_Model();

        $arrayCabecalho = array('Código', 'Descrição Serviço', 'Unidade de Medida', 'Grupo Serviço');
        $esconderColunas = array('co_servico');

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($modelServico->getSqlListagemFiltro(array('co_grupo_servico' =>$co_grupo_servico)));
        $listagemSimec->addAcao('edit', array('func' => 'abrirServico', 'extra-params' => array('co_servico')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarServico', 'extra-params' => array('co_servico')));
        $listagemSimec->setFormFiltros('formulario-cardapio-tipo');
        $listagemSimec->setCampos($arrayCabecalho);

        return $listagemSimec;

    }

    public function listaGrupoServicoFilhos()
    {
        $sql = "select
                    grupo_pai.co_grupo_servico as codigo,
                    coalesce(grupo_pai.nu_codigo_grupo_servico || '.' || gs.nu_codigo_grupo_servico || ' - ' || gs.ds_grupo_servico) as descricao
                from
                    demandas.grupo_servico gs
                cross join lateral (
                    select 
                        gsp.co_grupo_servico,
                        gsp.nu_codigo_grupo_servico,
                        gsp.ds_grupo_servico,
                        gsp.co_grupo_servico_pai
                    from
                        demandas.grupo_servico gsp
                    where
                        gsp.co_status = 1
                        and gs.co_grupo_servico = gsp.co_grupo_servico_pai
                    ) as grupo_pai
                where
                    gs.co_status = 1
                    and grupo_pai.co_grupo_servico_pai is not null order by grupo_pai.nu_codigo_grupo_servico;";

        return $sql;
    }

    public function listaGrupoServicoPai()
    {
        $sql = "SELECT co_grupo_servico as codigo, COALESCE(nu_codigo_grupo_servico || ' - ' || ds_grupo_servico) as descricao FROM demandas.grupo_servico WHERE co_status = 1 AND co_grupo_servico_pai is null";

        return $sql;
    }

    public function listaGrupoServicoFilhoPorPaiID($co_grupo_servico)
    {
        $sql = "SELECT co_grupo_servico as codigo, COALESCE(nu_codigo_grupo_servico || ' - ' || ds_grupo_servico) as descricao FROM demandas.grupo_servico WHERE co_status = 1 AND co_grupo_servico_pai = $co_grupo_servico ";

        $arrDados = $this->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }
    
    public function getGrupoServicoPorId( $co_grupo_servico ) {

        if($co_grupo_servico){
            $sql = "SELECT co_grupo_servico, nu_codigo_grupo_servico, ds_grupo_servico, co_grupo_servico_pai, co_status
                    FROM demandas.grupo_servico
                    WHERE co_grupo_servico = $co_grupo_servico";
            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();

        return $arrDados;
    }



}
