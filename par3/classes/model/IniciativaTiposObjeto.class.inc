<?php

class Par3_Model_IniciativaTiposObjeto extends Modelo
{

    const FORMACAO = 2;
    const OBRA = 1;
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_tipos_objeto";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('intoid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'intoid'            => null,
        'intodsc'           => null,
        'intostatus'        => null,
        'intosituacao'      => null,
        'intodtinclusao'    => null,
        'intocpfinclusao'   => null,
        'intodtinativacao'  => null,
        'intocpfinativacao' => null
    );

    public function montarSQLSimples($arrPost)
    {
        $intodsc = sanitizar_string_pesquisa($arrPost['intodsc']);
        $where  = $arrPost['intodsc']?" AND upper(removeacento(intodsc)) LIKE '%{$intodsc}%'":'';
        $where .= $arrPost['intosituacao']?" AND intosituacao = '{$arrPost['intosituacao']}'":'';
        $where .= $arrPost['intoid'] && validarInteiro($arrPost['intoid'])?" AND intoid = {$arrPost['intoid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                intoid AS id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                intoid AS codigo,
                intodsc AS descricao,
                intosituacao
                FROM {$this->stNomeTabela}
                WHERE intostatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

    public function getIniciativa($arrPost) {
        $where = $arrPost['intoid']?" AND intoid != {$arrPost['intoid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(intodsc) = LOWER('{$arrPost['intodsc']}') AND intostatus = 'A' {$where}");
    }

    public function pegarSQLSelectCombo($intoid = array())
    {
        $intoid = is_array($intoid) ? implode(',',$intoid):$intoid;
        $where  = $intoid ? " AND intoid in({$intoid}) AND intosituacao = 'A'":'';


        return "SELECT  intoid as codigo, intodsc as descricao FROM {$this->stNomeTabela} WHERE intosituacao = 'A' $where ORDER BY intodsc ASC";
    }


    public function pegarSQLSelectComboUnidade($inuid)
    {
        return "select DISTINCT obj.intoid as codigo, obj.intodsc as descricao  FROM par3.iniciativa_tipos_objeto obj
                INNER JOIN par3.iniciativa as ini ON ini.intoid = obj.intoid
                inner join par3.iniciativa_planejamento inp on inp.iniid = ini.iniid
                where inp.inuid =$inuid and intosituacao = 'A'and inp.inpstatus = 'A'";

    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['intodsc'] = array();

        //VALIDA CAMPOS
        if(strlen($campos['intodsc']) > 255){array_push($erros['erros']['intodsc'],'Limite de caracteres excedido');}
        if($campos['intodsc'] == '' || $campos['intodsc'] == null){array_push($erros['erros']['intodsc'],'O campo não pode ser vazio');}
        if($this->getIniciativa($campos)){array_push($erros['erros']['intodsc'],'A descrição informada já existe');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }
}