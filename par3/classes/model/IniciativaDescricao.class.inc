<?php
class Par3_Model_IniciativaDescricao extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_descricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('indid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'indid'            => null,
        'inddsc'           => null,
        'indstatus'        => null,
        'indsituacao'      => null,
        'inddtinclusao'    => null,
        'indcpfinclusao'   => null,
        'inddtinativacao'  => null,
        'indcpfinativacao' => null
    );

    public function montarSQLSimples($arrPost)
    {
        $inddsc = ($arrPost['inddsc'] == '/') ? sanitizar_string_pesquisa($arrPost['inddsc'],true) : sanitizar_string_pesquisa($arrPost['inddsc']);
        $where  = $arrPost['inddsc']?" AND upper(removeacento(inddsc)) LIKE '%{$inddsc}%'":'';
        $where .= $arrPost['indsituacao']?" AND indsituacao = '{$arrPost['indsituacao']}'":'';
        $where .= $arrPost['indid'] && validarInteiro($arrPost['indid'])?" AND indid = {$arrPost['indid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";
        $sql = "SELECT 
                indid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                indid as codigo, inddsc as descricao, indsituacao
                FROM par3.iniciativa_descricao 
                WHERE indstatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

    public function getIniciativa(array $arrPost) {
        $where = $arrPost['indid']?" AND indid != {$arrPost['indid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE upper(inddsc) = '".str_to_upper($arrPost['inddsc'])."' AND indstatus = 'A' {$where}");
    }
    
    public function pegarSQLSelectCombo($indid = array())
    {
    	$indid   = is_array($indid) ? implode(',',$indid):$indid;
    	$where   = "WHERE indstatus = 'A' AND  indsituacao = 'A'";
    	$where  .= $indid ? " AND indid in({$indid})":'';

    	return "SELECT  indid as codigo, inddsc as descricao FROM {$this->stNomeTabela} $where";
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['inddsc'] = array();

        //VALIDA CAMPOS
        if(strlen($campos['inddsc']) > 255){array_push($erros['erros']['inddsc'],'Limite de caracteres excedido');}
        if($campos['inddsc'] == '' || $campos['inddsc'] == null){array_push($erros['erros']['inddsc'],'O campo não pode ser vazio');}
        if($this->getIniciativa($campos)){array_push($erros['erros']['inddsc'],'A descrição informada já existe');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

}

//if($novoind && $arrayIniDesc['indsituacao'] == 'I' ) {
//    return "status inativo";
//}