<?php

class Seguranca_Model_Menu extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.menu';
    
    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'mnuid'
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
        'mnuid' => null,
        'mnucod' => null,
        'mnucodpai' => null,
        'mnudsc' => null,
        'mnustatus' => null,
        'mnulink' => null,
        'mnutipo' => null,
        'mnustile' => null,
        'mnuhtml' => null,
        'mnusnsubmenu' => null,
        'mnutransacao' => null,
        'mnushow' => null,
        'abacod' => null,
        'mnuhelp' => null,
        'sisid' => null,
        'mnuidpai' => null,
        'mnuordem' => null,
        'mnupaginainicial' => null,
        'mnuimagem' => null,
        'mnuacessorapido' => null,
        'constantevirtual' => null
    );
    
    public function listarMenuAcessoDireto(Array $arWhere=array())
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
        
        $sql = "SELECT DISTINCT
                    m.mnudsc,
                    m.mnulink,
                    m.mnucod,
                    coalesce(m3.mnucod, m3.mnucod, m2.mnucod, m1.mnucod) as mnucodpai,
                    coalesce(m3.mnudsc, m3.mnudsc, m2.mnudsc, m1.mnudsc) as mnudscpai
                FROM
                    seguranca.menu m
                join seguranca.perfilmenu pm on pm.mnuid = m.mnuid and
                                          pm.pmnstatus = 'A'
                join seguranca.perfilusuario pu on pu.pflcod = pm.pflcod
                left join seguranca.menu m1 on m1.mnuid = m.mnuidpai
                left join seguranca.menu m2 on m2.mnuid = m1.mnuidpai
                left join seguranca.menu m3 on m3.mnuid = m2.mnuidpai
                left join seguranca.menu m4 on m4.mnuid = m3.mnuidpai
                WHERE
                    m.mnustatus = 'A' and
                    m.mnuacessorapido = true and
                    m.mnusnsubmenu = false "
                    . ($where ? " AND " . implode(" AND ", $where) : "") . "
                ORDER BY
                    mnucodpai, m.mnucod";
        $dados = $this->carregar($sql);

        return ($dados ? $dados : array());
    }
}
