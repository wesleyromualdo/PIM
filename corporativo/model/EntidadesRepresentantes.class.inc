<?php

class EntidadesRepresentantes_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.entidades_representantes";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('enrid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
//     protected $arAtributosTabela = array('enfid', 'enfdsc', 'enfstatus');

    protected $arAtributos = array(
        'enrid' => null,
        'enfidpai' => null,
        'enfidfilho' => null,
        'enrqtdcadastrados' => null,
        'enrtipo' => null,
        'enrgestor' => null,
        'enrstatus' => null
    );
    
    public function carregarPorRelacionamento($enfidpai, $enfidfilho)
    {
        $sql = "select * from corporativo.entidades_representantes where enfidpai={$enfidpai} and enfidfilho={$enfidfilho} and enrstatus=1";
        $arDados = $this->pegaLinha($sql);
        $arDados = ($arDados ? $arDados : array());
        $this->popularDadosObjeto($arDados);
    }
    
    public function listarEntidadesRepresentantes(Array $arFiltro = array())
    {
        $arFiltro['enfidpai'] = ($arFiltro['enfidpai'] ? $arFiltro['enfidpai']  : 0); 
        $where = array();
        foreach ($arFiltro as $k => $value) {            
            switch(true) {
                case empty($value):
                    continue 2 ;
                default:
                    $value = (array) $value;
                    $where[] = "{$k} IN ('". implode("', '", $value) ."')";
                    break;
            }
        }
        
        $sql = "select 
                    er.enrid,
                	ef.enfdsc,
                    er.enrqtdcadastrados,
                    case when enrtipo = 'E' then 'Por entidade' else 'Global' end as enrtipo,
                    case when enrgestor = true then 'Sim' else 'Não' end as enrgestor,
                    case when enftipo = 'F' then 'Física' else 'Jurídica' end as enftipo
                from
                	corporativo.entidades_representantes er
                join corporativo.entidades_funcao ef on ef.enfid = er.enfidfilho and 
                										ef.enfstatus = 'A'
                where
                    er.enrstatus = 1" . 
                    ($where ? " and " . implode(" and ", $where) : "") . "
                order by
                    ef.enfdsc";
        
        $arrayCabecalho = array('Representante', 'Quantidade', 'Tipo', 'Gestor?', 'Tipo de personalidade');
        
        $listagemSimec = new Simec_Listagem();
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('edit', array('func' => 'editarRepresentanteEntidade', 'extra-params' => array('enrid')));
        $listagemSimec->addAcao('delete', array('func' => 'excluirRepresentanteEntidade', 'extra-params' => array('enrid')));
        $listagemSimec->addCallbackDeCampo('enfdsc','alignLeft');
        $listagemSimec->addCallbackDeCampo('enrtipo','alignLeft');
        $listagemSimec->setFormFiltros('lista-representante-entidade');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setListaColunas($arrayCabecalho);
                    
        return $listagemSimec;
    }
    
    
}