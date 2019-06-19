<?php

class EntidadesFuncao_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.entidades_funcao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('enfid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
//     protected $arAtributosTabela = array('enfid', 'enfdsc', 'enfstatus');

    protected $arAtributos = array(
        'enfid' => null,
        'enfdsc' => null,
        'enfstatus' => null,
        'enftipo' => null
    );

    public function antesSalvar()
    {
        $this->arAtributos['enfdsc'] = ucwords($this->arAtributos['enfdsc']);
        
        return true;
    }
    
    public function listarEntidadesFuncao(Array $arFiltro = array())
    {
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
                    enfid,
                    enfdsc,
                    CASE WHEN enftipo = 'J' THEN 'Pessoa Jurídica' ELSE 'Pessoa Física' END enftipo
                from
                    corporativo.entidades_funcao
                where
                    enfstatus = 'A'" . 
                    ($where ? " and " . implode(" and ", $where) : "");
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Função entidade', 'Tipo');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('edit', array('func' => 'editarFuncaoEntidade', 'extra-params' => array('enfid')));
        $listagemSimec->addAcao('user', array('func' => 'editarRepresentanteEntidade', 'extra-params' => array('enfid')));
        $listagemSimec->addAcao('delete', array('func' => 'excluirFuncaoEntidade', 'extra-params' => array('enfid')));
        $listagemSimec->addCallbackDeCampo('enfdsc','alignLeft');
        $listagemSimec->addCallbackDeCampo('enftipo','alignLeft');
        $listagemSimec->setFormFiltros('filtro-funcao-entidade');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setListaColunas($arrayCabecalho);
                    
        return $listagemSimec;
    }
    
    
    public function getComboSql(Array $arFiltro = array())
    {
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
                    enfid as codigo, enfdsc as descricao 
                from 
                    corporativo.entidades_funcao 
                where
                    enfstatus = 'A'
                    ". ($where ? " and " . implode(" and ", $where) : "") ."
                order by
                    enfdsc";
        
        return $sql;
    }
    
    public function getRepresentanteParaAssociacaoComboSql($enfidpai, Array $arFiltro = array(), $retornoSql=true)
    {
        $where = array();
        $whereRepresentante = array();
        foreach ($arFiltro as $k => $value) {
            $k = strtolower($k);
            switch(true) {
                case empty($value):
                    continue 2 ;
                case $k == 'inclui-representante':
                    $value = (array) $value;
                    $whereRepresentante[] = "enfidfilho not in (". implode("', '", $value) .")";
                    break;
                case (strpos($k, 'not') !== false):
                    $value = (array) $value;
                    $where[] = "{$k} ('". implode("', '", $value) ."')"; 
                    break;
                default:
                    $value = (array) $value;
                    $where[] = "{$k} IN ('". implode("', '", $value) ."')";
                    break;
            }
        }
        
        $sql = "select
                    enfid as codigo, enfdsc as descricao
                from
                    corporativo.entidades_funcao
                where
                    enfstatus = 'A' and 
                    enfid not in (
                        select 
                            enfidfilho  
                        from 
                            corporativo.entidades_representantes 
                        where 
                            enfidpai = {$enfidpai}
                            ". ($whereRepresentante ? " and " . implode(" and ", $whereRepresentante) : "") ."
                    ) 
                    ". ($where ? " and " . implode(" and ", $where) : "") ."
                order by
                    enfdsc";
        if ($retornoSql == false) {
            $arDado = $this->carregar($sql);
            return ($arDado ? $arDado : array());
        } else {
            return $sql;
        }
    }
    
    
}