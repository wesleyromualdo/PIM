<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao.
 *
 * @version $Id$
 * @since 2017.06.09
 */

/**
 * Model_Itenscomposicao: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $itcid Chave Primária - default: nextval('par3.itenscomposicao_itcid_seq'::regclass)
 * @property int $ctdid Chave estrangeira par3.categoriadespesa - default: nextval('par3.itenscomposicao_ctdid_seq'::regclass)
 * @property int $uniid Chave estrangeira par3.unidade_medida - default: nextval('par3.itenscomposicao_uniid_seq'::regclass)
 * @property string $itcdsc
 * @property string $itcespecificacao Especificação Técnica
 * @property "char" $itcstatus Status Item de Composição (Soft Delete)
 * @property "char" $itcsituacao Situação Item de Composição
 * @property \Datetime(Y-m-d H:i:s) $itcdtinclusao Data de inclusão do Item de Composição
 * @property string $itccpfinclusao CPF do responsável pela inclusão de um Item de Composição
 * @property \Datetime(Y-m-d H:i:s) $itcdtinativacao Data de Inativação de um Item de Composição
 * @property string $itccpfinativacao
 */
class Par3_Model_Itenscomposicao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'itcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'uniid' => array('tabela' => 'par3.unidade_medida',   'pk' => 'uniid'),
        'ctdid' => array('tabela' => 'par3.categoriadespesa', 'pk' => 'ctdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'itcid'            => null,
        'ctdid'            => null,
        'uniid'            => null,
        'itcdsc'           => null,
        'itcespecificacao' => null,
        'itcstatus'        => null,
        'itcsituacao'      => null,
        'itcdtinclusao'    => null,
        'itccpfinclusao'   => null,
        'itcdtinativacao'  => null,
        'itccpfinativacao' => null,
        'itcidsigarp' 	   => null,
    );

    public function getItem($arrPost)
    {
        $where = $arrPost['itcid']?" AND itcid != {$arrPost['itcid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(itcdsc) = LOWER('{$arrPost['itcdsc']}') AND itcstatus = 'A' {$where}");
    }

    public function atualizaValorPlanejadoPlanejamentoIniciativa()
    {
    	if($this->inpid){
    		$sql = "SELECT par3.atualiza_valor_planejado_planejamento_iniciativa({$this->inpid});";
    		$this->executar($sql);
    		$this->commit();
    	}
    }
    
    public function removerDetalhes($itcid, $status, $situacao)
    {
        $sql = "UPDATE par3.itenscomposicao_detalhamento SET
                	itdstatus = '$status',
                    itdsituacao = '$situacao'
                WHERE
                	itcid = $itcid";
        
        if($itcid > 0) $this->executar($sql);
        $this->commit();
    }

    public function validarInput($campos)
    {
        //campos
        $erros['erros']['itcdsc']             = array();
        $erros['erros']['itcespecificacao']   = array();
        $erros['erros']['ctdid']              = array();
        $erros['erros']['uniid']              = array();
        $erros['erros']['itcsituacao']        = array();
        $erros['erros']['intoid']             = array();

        $itc = new $this($campos['itcid']);
        if($campos['itcdsc'] == ''){array_push($erros['erros']['itcdsc'],'O campo Nome não pode ser vazio');}
        if($campos['itcespecificacao'] == ''){array_push($erros['erros']['itcespecificacao'],'O campo especificação não pode ser vazio');}
        if(empty($campos['ctdid'])  || $campos['ctdid'] == null){array_push($erros['erros']['ctdid'],'Nenhuma Categoria de Despesa selecionada.');}
        if(empty($campos['uniid'])  || $campos['uniid'] == null){array_push($erros['erros']['uniid'],'Nenhuma Unidade de Medida selecionada.');}
        if(empty(prepararArraySelectMultiSelecao($campos['intoid']))){array_push($erros['erros']['intoid'],'Nenhum Tipo de Objeto foi selecionado.');}

        //VALIDA CAMPOS
        if(strlen($campos['itcespecificacao']) > 1000){array_push($erros['erros']['itcespecificacao'],'Limite de caracteres excedido');}
        if(strlen($campos['itcdsc']) > 255){array_push($erros['erros']['itcdsc'],'Limite de caracteres excedido');}
        if($this->getItem($campos)){array_push($erros['erros']['itcdsc'],'Já Existe Item de Composição cadastrado com esse nome.');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function montarSQLDataGrid($arrPost)
    {
        $arrInto = array();
        if($arrPost['intoid']){
            foreach($arrPost['intoid'] as $into){
                if($into == ''){//Verificar se o array tem valor
                    continue;
                }
                $arrInto[] = $into;
            }
        }

        $itcdsc  = sanitizar_string_pesquisa($arrPost['itcdsc']);
        $itcid   = $arrPost['codigo'];
        $intoid  = implode(',',$arrInto);
        $where   = $arrPost['itcdsc']?" AND upper(removeacento(itcdsc)) LIKE upper(removeacento('%{$itcdsc}%')) ":'';
        $where  .= $itcid && validarInteiro($itcid)?"  AND itc.itcid = {$itcid} ":'';
        $where  .= $arrPost['ctdid']?"AND ctd.ctdid = {$arrPost['ctdid']}":"";
        $where  .= $arrPost['uniid']?"AND uni.uniid = {$arrPost['uniid']}":"";
        $where  .= $arrPost['itcsituacao']?" AND itc.itcsituacao = '{$arrPost['itcsituacao']}' ":'';
        $where  .= !empty($arrInto)?" AND ito.intoid in({$intoid})":'';
        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";
        $contador = $arrPost['req'] == 'imprimir' ||  $arrPost['req'] == 'xls'? "ROW_NUMBER() OVER ({$orderBy}) AS qtd," : '';
        $inicio = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls' ? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (":'';
        $fim = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls'? ") as foo":'';
        $colunaEspecificacao = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls'? ',itcespecificacao':'';
        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
         */
        $sql = "
        {$inicio}
                SELECT DISTINCT
                itc.itcid as id,
                itc.itcid as codigo,
                itc.itcdsc as descricao,
                (SELECT array_to_string(array(
                    SELECT  DISTINCT
                    tipos_obj.intodsc
                    FROM par3.itenscomposicao_tipos_objeto as ito
                    INNER JOIN par3.iniciativa_tipos_objeto as tipos_obj ON tipos_obj.intoid = ito.intoid
                    WHERE ito.itcid = itc.itcid ORDER BY tipos_obj.intodsc ASC), ', ')
                ) as tiposobjetos,
                uni.unidsc as unidademedida,
                ctd.ctddsc as categoriadespesa,
                itc.itcsituacao as situacao,
                itc.itcidsigarp
                {$colunaEspecificacao}
                FROM {$this->stNomeTabela} as itc
                INNER JOIN par3.categoriadespesa as ctd ON ctd.ctdid = itc.ctdid
                INNER JOIN par3.unidade_medida   as uni ON uni.uniid = itc.uniid
                INNER JOIN par3.itenscomposicao_tipos_objeto as ito ON ito.itcid = itc.itcid
                WHERE itcstatus = 'A'
                {$where}
                {$orderBy}
        {$fim}
                ";
        return $sql;
    }
    public function listaItemComposicao($iniciativa)
    {
         return $this->carregar("SELECT DISTINCT
                itc.itcid as codigo,
                itc.itcdsc as descricao,
		        tipos_obj.intoid
                FROM  {$this->stNomeTabela}  as itc
                INNER JOIN par3.categoriadespesa as ctd ON ctd.ctdid = itc.ctdid
                INNER JOIN par3.unidade_medida   as uni ON uni.uniid = itc.uniid
                INNER JOIN par3.itenscomposicao_tipos_objeto as ito ON ito.itcid = itc.itcid
                INNER JOIN par3.iniciativa_tipos_objeto as tipos_obj ON tipos_obj.intoid = ito.intoid
		WHERE tipos_obj.intoid = (SELECT DISTINCT inito.intoid FROM par3.iniciativa as ini LEFT JOIN par3.iniciativa_tipos_objeto as inito ON inito.intoid = ini.intoid  WHERE ini.inistatus = 'A' AND ini.iniid =$iniciativa)
		AND itc.itcstatus   = 'A'
		AND itc.itcsituacao = 'A'
		");
    }
}
