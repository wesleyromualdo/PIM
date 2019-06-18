<?php

class Par3_Model_IniciativaProgramas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.programa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'prgid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pgoid' => array('tabela' => 'par3.programaorigem', 'pk' => 'pgoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prgid'                 => null,
        'prgdsc'                => null,
    	'prgstatus'             => null,
        'pgoid'                 => null,
        'prglink'               => null,
        'prgabreviatura'        => null,
        'prgarquivo'            => null,
        'prgresumo'             => null,
        'prgatd_extraordinario' => null,
        'prgsituacao'           => null,
        'prgdtinclusao'         => null,
        'prgcpfinclusao'        => null,
        'prgdtinativacao'       => null,
        'prgcpfinativacao'      => null,
        'prgperiodoinicio'      => null,
        'prgperiodofim'         => null,
        'prgesfera'             => null,
        'prganoreferencia'      => null,
        'arqid'                 => null
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
//    public function getCamposValidacao($dados = array())
//    {
//        return array(
//            'inprgid' => array('Digits'),
//            'inprgabreviatura' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
//            'inprgdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
//            'inprglink' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
//            'inprgresumo' => array('allowEmpty' => true),
//            'inprgatd_extraordinario' => array('allowEmpty' => false),
//            'inprgstatus' => array('allowEmpty' => true),
//            'inprgsituacao' => array('allowEmpty' => true),
//            'inprgdtinclucao' => array('allowEmpty' => true),
//            'inprgcpfinclusao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
//            'inprgdtinativacao' => array('allowEmpty' => true),
//            'inprgcpfinativacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
//        );
//    }

    public function montarSQLSimples($arrPost)
    {
        $prgdsc = sanitizar_string_pesquisa($arrPost['prgdsc']);
        $prgabreviatura = sanitizar_string_pesquisa($arrPost['prgabreviatura']);

        $whereSituacao = $arrPost['prgsituacao']?" WHERE prgsituacao = '{$arrPost['prgsituacao']}'": "WHERE prgsituacao IN ('A', 'I')";

        $where  = $arrPost['prgdsc']?" AND upper(removeacento(prgdsc)) LIKE upper(removeacento('%{$prgdsc}%'))":'';
        $where .= $arrPost['prgabreviatura']?" AND upper(removeacento(prgabreviatura)) LIKE '%{$prgabreviatura}%'":'';
        $where .= $arrPost['prgatd_extraordinario']?" AND prgatd_extraordinario = '{$arrPost['prgatd_extraordinario']}'":'';
        $where .= $arrPost['prgid'] && validarInteiro($arrPost['prgid']) ?" AND prgid = {$arrPost['prgid']}":'';
        $where .= " AND prgstatus = 'A' ";

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                prgid AS id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                prgid AS codigo,
                case when prgabreviatura is null then '-' else prgabreviatura end as sigla,
                prgdsc AS descricao,
                CASE WHEN
                    prgatd_extraordinario = false
						THEN
							'Não'
						ELSE
							'Sim'
						END as atd_extraordinario,
                prgsituacao
                FROM {$this->stNomeTabela}
                $whereSituacao
                {$where}
                {$orderBy}
                ";
//                 ver($sql);
        return $sql;
    }

    public function getIniciativa($arrPost) {
        $where = $arrPost['prgid']?" AND prgid != {$arrPost['prgid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(prgdsc) = LOWER('{$arrPost['prgdsc']}') AND prgstatus = 'A' {$where}");
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['prgdsc']           = array();
        $erros['erros']['prgperiodo']       = array();
        $erros['erros']['prgesfera']        = array();
        $erros['erros']['prganoreferencia'] = array();

        if($campos['prgatd_extraordinario_old'] == 't' && $campos['prgatd_extraordinario'] == 't') {
            if($campos['prgperiodoinicio'] == ''){array_push($erros['erros']['prgperiodo'],'O período de início não pode ser vazio');}
            if($campos['prgperiodofim'] == ''){array_push($erros['erros']['prgperiodo'],'O fim do período não pode ser vazio');}
            if($campos['prgperiodoinicio'] > $campos['prgperiodofim']){array_push($erros['erros']['prgperiodo'],'O início do período não pode ser maior do que o fim do período');}
            if($campos['prgesfera'] == '' || $campos['prgesfera'] == null){array_push($erros['erros']['prgesfera'],'O campo não pode ser vazio');}
            if($campos['prganoreferencia'] == '' || $campos['prganoreferencia'] == null){array_push($erros['erros']['prganoreferencia'],'O campo não pode ser vazio');}
        }
        //VALIDA CAMPOS
        if(strlen($campos['prgdsc']) > 255){array_push($erros['erros']['prgdsc'],'Limite de caracteres excedido');}
        if($campos['prgdsc'] == '' || $campos['prgdsc'] == null){array_push($erros['erros']['prgdsc'],'O campo não pode ser vazio');}
        if($this->getIniciativa($campos)){array_push($erros['erros']['prgdsc'],'A descrição informada já existe');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
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

    public function pegarSQLSelectComboSigla()
    {
        $sql = "SELECT
                prgid as codigo,
                prgabreviatura||' - '|| prgdsc as descricao
                FROM {$this->stNomeTabela}
                
                WHERE prgstatus = 'A' AND prgsituacao = 'A'
        ";

        return $sql;
    }

    public function pegarSQLSelectComboSiglaUnidade($inuid)
    {
        $sql = "            SELECT DISTINCT
                        prg.prgid as codigo,
                        prgdsc as descricao
                        FROM par3.iniciativa_iniciativas_programas iiprg
                        INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                        inner join par3.iniciativa ini on ini.iniid = iiprg.iniid
                 	    inner join  par3.iniciativa_planejamento inp on inp.iniid = ini.iniid
                 	    where inp.inuid = {$inuid}  and inp.inpstatus = 'A'";

        return $sql;
    }

    public function getIniciativaProgramaById($prgid){
        $sql = "SELECT prog.* FROM par3.programa as prog WHERE prog.prgid = {$prgid}";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($prgid){
        $sql = "SELECT prog.* FROM par3.programa as prog
                    INNER JOIN par3.iniciativa_iniciativas_programas as iniciativa_programas ON iniciativa_programas.prgid = prog.prgid
                    INNER JOIN par3.iniciativa ON iniciativa_programas.iniid = iniciativa_programas.iniid
                    INNER JOIN par3.iniciativa_planejamento as iniciativa_planejamento ON iniciativa_planejamento.iniid = iniciativa.iniid
                    WHERE prog.prgid = {$prgid}";

        $result = $this->carregar($sql);
        return $result;
    }

}
