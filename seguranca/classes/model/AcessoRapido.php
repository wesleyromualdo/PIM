<?php

class Seguranca_Model_AcessoRapido extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.acessorapido';
    
    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'acrid'
    );
    
    /**
     * @var mixed[] Chaves estrangeiras.
     */
//     protected $arChaveEstrangeira = array(
//         'modid' => array('tabela' => 'modulo', 'pk' => 'modid'),
//     );
    
    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'acrid' => null,
//         'sisid' => null,
        'acrdsc' => null,
        'acrstatus' => null,
        'acrfiltro' => null,
        'acrordem' => null
    );
    
    public function apagarPorAcrid($acrid)
    {
        $sql = "UPDATE seguranca.acessorapido SET acrstatus='I' WHERE acrid = {$acrid}";
        $this->executar($sql);
    }
    
    public function listar(Array $arWhere=array())
    {
        $where = array();
        foreach ($arWhere as $k => $value){
            switch($k) {
                default:
                    $value = (array) $value;
                    $where[] = "{$k} IN ('". implode("', '", $value) ."')";
                    break;
            }
        }
        
        $img = "<center>
                    <img align=\"absmiddle\"
                     src=\"/imagens/alterar.gif\"
                     style=\"cursor: pointer\"
                     onclick=\"javascript: alterarAcessoRapido(' || ar.acrid || ');\"
                     title=\"Alterar Acesso Rápido\" />
                    &nbsp;
                   <img align=\"absmiddle\"
                     src=\"/imagens/excluir.gif\"
                     style=\"cursor: pointer\"
                     onclick=\"javascript: excluirAcessoRapido(' || ar.acrid || ');\"
                     title=\"Excluir Acesso Rápido\" /></center>";
        
        $sql = "SELECT
                    '$img' as acao,
                    acrdsc,
                    ARRAY_TO_STRING(ARRAY_AGG( DISTINCT s.sisdsc), '<br>') as sisdsc,
                    acrfiltro,
                    ARRAY_TO_STRING(ARRAY_AGG( DISTINCT p.pfldsc || ' - ' || s1.sisdiretorio), '<br>') as perfil,
                    acrordem
                FROM
                    seguranca.acessorapido ar
                JOIN seguranca.acessorapido_sistema acs ON acs.acrid = ar.acrid
                JOIN seguranca.sistema s on s.sisid = acs.sisid
                LEFT JOIN seguranca.acessorapido_perfil ap on ap.acrid = ar.acrid
                LEFT JOIN seguranca.perfil p on p.pflcod = ap.pflcod
                LEFT JOIN seguranca.sistema s1 on s1.sisid = p.sisid
                WHERE
                    acrstatus = 'A'"
                    . ($where ? " AND " . implode(" AND ", $where) : "") . "
                GROUP BY
                    ar.acrid, ar.acrdsc, ar.acrfiltro, acrordem";
        $dados = $this->carregar($sql);
        
        return ($dados ? $dados : array());
    }
}
