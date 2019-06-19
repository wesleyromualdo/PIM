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
        $filtro = '';
        
        if( $arrFiltro['nu_codigo_grupo_servico'] ){
            $filtro = " and grupo.nu_codigo_grupo_servico ilike removeAcento('%".$arrFiltro['nu_codigo_grupo_servico']."%')";
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



        
        $sql = " 
        SELECT
            grupo.co_grupo_servico ,
            grupo.nu_codigo_grupo_servico, 
            grupo.ds_grupo_servico,
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

            WHERE grupo.co_status =  1

                {$filtro} 

            ORDER BY 6 
         ";
        

        return $sql;
        
    }





    public function listarGrupoServico ($arrFiltro){

        $listagemSimec = new Simec_Listagem();


        $arrayCabecalho = array('Código', 'Grupo Serviço', 'Código Serviço Pai', 'Grupo Serviço Pai');
        $esconderColunas = array('co_grupo_servico', 'ordem');

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->listaFiltroSql($arrFiltro));
        $listagemSimec->addAcao('plus', array('func' => 'mostrarFilhos', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->addAcao('edit', array('func' => 'abrirGrupoServico', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarGrupoServico', 'extra-params' => array('co_grupo_servico')));
        $listagemSimec->setFormFiltros('formulario-cardapio-tipo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);


        return $listagemSimec;

    }

    public function listaGrupoServicoFilhos()
    {
        $sql = "
                SELECT 
                    gp.co_grupo_servico as codigo, 
                    COALESCE(gppai.nu_codigo_grupo_servico || ' - ' || gp.nu_codigo_grupo_servico || ' - ' || gp.ds_grupo_servico) as descricao 
                    
                FROM    demandas.grupo_servico gp 
                INNER JOIN demandas.grupo_servico gppai ON gp.co_grupo_servico_pai = gppai.co_grupo_servico
                WHERE   gp.co_status = 1 
                AND     gp.co_grupo_servico_pai is not null

                ";

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
