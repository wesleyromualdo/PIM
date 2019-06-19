<?php

class Par3_Model_IniciativaTiposAssistencia extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_tipos_assistencia";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    const POR_ESCOLA			= 2;

    protected $arChavePrimaria = array('intaid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'intaid'            => null,
        'intadsc'           => null,
        'intastatus'        => null,
        'intasituacao'      => null,
        'intadtinclusao'    => null,
        'intacpfinclusao'   => null,
        'intadtinativacao'  => null,
        'intacpfinativacao' => null
    );

    public function montarSQLSimples($arrPost)
    {
        $intadsc = sanitizar_string_pesquisa($arrPost['intadsc']);
        $where  = $arrPost['intadsc']?" AND upper(removeacento(intadsc)) LIKE upper(removeacento('%{$intadsc}%'))":'';
        $where .= $arrPost['intasituacao']?" AND intasituacao = '{$arrPost['intasituacao']}'":'';
        $where .= $arrPost['intaid'] && validarInteiro($arrPost['intaid'])?" AND intaid = {$arrPost['intaid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT 
	                intaid AS id,
	                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
	                intaid AS codigo, 
	                intadsc AS descricao, 
	                intasituacao
                FROM 
        			{$this->stNomeTabela}
                WHERE 
                	intastatus = 'A'
                	{$where}
                {$orderBy}";
                
        return $sql;
    }

    public function getIniciativa($arrPost) {
        $where = $arrPost['intaid']?" AND intaid != {$arrPost['intaid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(intadsc) = LOWER('{$arrPost['intadsc']}') AND intastatus = 'A' {$where}");
    }
    
//     public function pegarSQLSelectCombo($intaid = array())
//     {
//     	$intaid = is_array($intaid) ? implode(',',$intaid):$intaid;
//     	$where = "WHERE intastatus = 'A' and intasituacao = 'A'";
//     	$where  .= $indid ? " AND intaid in({$intaid})":'';

//     	return "SELECT  intaid as codigo, intadsc as descricao FROM {$this->stNomeTabela} $where";
//     }
    
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['intadsc'] = array();

        //VALIDA CAMPOS
        if(strlen($campos['intadsc']) > 255){array_push($erros['erros']['intadsc'],'Limite de caracteres excedido.');}
        if($campos['intadsc'] == '' || $campos['intadsc'] == null){array_push($erros['erros']['intadsc'],'O campo não pode ser vazio.');}
        if($this->getIniciativa($campos)){array_push($erros['erros']['intadsc'],'A descrição informada já existe.');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

//     public function verificaLigacaoPlanejamento($intaid){
//         $sql = "SELECT * FROM 
//           par3.iniciativa_planejamento as iniciativa_planejamento
//           INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
//           INNER JOIN par3.iniciativa_tipos_atendimento as iniciativa_tipos_atendimento ON iniciativa_tipos_atendimento.intaid = iniciativa.intaid
// 	      WHERE iniciativa_tipos_atendimento.intaid = {$intaid}";
//         $result = $this->carregar($sql);
//         return $result;
//     }
//     public function getTiposAtendimentoById($intaid){
//         $sql = "SELECT * FROM 
//           par3.iniciativa_planejamento as iniciativa_planejamento
//           INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
//           INNER JOIN par3.iniciativa_tipos_atendimento as iniciativa_tipos_atendimento ON iniciativa_tipos_atendimento.intaid = iniciativa.intaid
// 	      WHERE iniciativa_tipos_atendimento.intaid = {$intaid}";
//         $result = $this->pegaLinha($sql);
//         return $result;
//     }

    public function getAssistenciaPorAnaid($anaid)
    {
        $sql = "SELECT a.edeid, ve.emecod, it.intaid, it.intadsc, au.autcod||' - '||au.autnome AS parlamentar
                FROM par3.analise a
                	LEFT JOIN par3.iniciativa_tipos_assistencia it ON it.intaid = a.intaid
                	LEFT JOIN emenda.v_emendadetalheentidade ve
                        INNER JOIN emenda.autor au ON au.autid = ve.autid
                    ON ve.edeid = a.edeid
                WHERE anaid = {$anaid}";
        return $sql;
    }
}