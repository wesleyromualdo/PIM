<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_grupos.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Model_Itenscomposicao_grupos: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_grupos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_grupos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $igrid Chave Primária - default: nextval('par3.itenscomposicao_grupos_igrid_seq'::regclass)
 * @property string $igrnome Nome do grupo de item de composição
 * @property string $igrdsc Descrição do grupo de item de composição
 * @property "char" $igrstatus Status do grupo de item de composição (Soft Delete)
 * @property "char" $igrsituacao Situação do grupo de item de composição
 * @property \Datetime(Y-m-d H:i:s) $igrdtinclusao Data de Inclusão do grupo de item de composição
 * @property string $igrcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $igrdtinativacao Data de Inativação do grupo de item de composição
 * @property string $igrcpfinativacao CPF do responsável do grupo de item de composição
 */
class Par3_Model_ItenscomposicaoGrupos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_grupos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'igrid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'igrid'            => null,
        'igrnome'          => null,
        'igrdsc'           => null,
        'igrstatus'        => null,
        'igrsituacao'      => null,
        'igrdtinclusao'    => null,
        'igrcpfinclusao'   => null,
        'igrdtinativacao'  => null,
        'igrcpfinativacao' => null,
        'igridsigarp' 	   => null,
    );

    public function getItemGrupo($arrPost)
    {
        $where = $arrPost['igrid']?" AND igrid != {$arrPost['igrid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE LOWER(igrnome) = LOWER('{$arrPost['igrnome']}') AND igrstatus = 'A' {$where}");
    }

    public function validarInput($campos)
    {
        //campos
        $erros['erros']['igrnome'] = array();
        $erros['erros']['igrdsc']  = array();
        $erros['erros']['intoid']  = array();

        $igrid = new $this($campos['igrid']);
        if($campos['igrnome'] == ''){array_push($erros['erros']['igrnome'],'O campo Nome não pode ser vazio');}
        if($campos['igrdsc'] == '<p><br></p>'){array_push($erros['erros']['igrdsc'],'O campo Descrição não pode ser vazio');}
        if(empty(prepararArraySelectMultiSelecao($campos['intoid'])) || $campos['intoid'] == null){array_push($erros['erros']['intoid'],'Nenhum Tipo de Objeto foi selecionado.');}

        //VALIDA CAMPOS
        if(strlen($campos['igrnome']) > 255){array_push($erros['erros']['igrnome'],'Limite de caracteres excedido');}
        if(strlen($campos['igrdsc']) > 1000){array_push($erros['erros']['igrdsc'],'Limite de caracteres excedido');}
        if($this->getItemGrupo($campos)){array_push($erros['erros']['igrnome'],'Já Existe Item de Composição cadastrado com esse nome.');}
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
                $arrInto[] = $into;
            }
        }

        $igrdsc   = sanitizar_string_pesquisa($arrPost['igrdsc']);
        $igrnome  = sanitizar_string_pesquisa($arrPost['igrnome']);
        $igrid    = $arrPost['codigo'];
        $intoid   = implode(',',$arrInto);

        $where    = $arrPost['igrnome']?" AND upper(removeacento(igr.igrnome)) LIKE '%{$igrnome}%' ":'';
        $where   .= $arrPost['igrdsc']?" AND upper(removeacento(igr.igrdsc)) LIKE '%{$igrdsc}%' ":'';
        $where   .= $igrid && validarInteiro($igrid)?"  AND igr.igrid = {$igrid} ":'';
        $where   .= $arrPost['igrsituacao']?" AND igr.igrsituacao = '{$arrPost['igrsituacao']}' ":'';
        $where   .= $arrPost['intoid']['0'] != '' ?" AND igo.intoid in({$intoid})":'';
        $orderBy  = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";
        $contador = $arrPost['req'] == 'imprimir' ||  $arrPost['req'] == 'xls' ? "ROW_NUMBER() OVER ({$orderBy}) AS qtd," : '';
        $inicio   = $arrPost['req'] == 'imprimir' || $arrPost['req']  == 'xls' ? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (":'';
        $fim      = $arrPost['req'] == 'imprimir' || $arrPost['req']  == 'xls' ? ") as foo":'';

        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,nome = 4,descricao = 5,tiposobjetos= 6,datainclusao = 7,situacao= 8
         */
        $sql = "
        {$inicio}
                SELECT DISTINCT
                igr.igrid   as id,
                igr.igrid   as codigo,
                igr.igrnome as nome,
                igr.igrdsc  as descricao,
                (SELECT 
                    array_to_string(array(
                        SELECT  DISTINCT
                        tipos_obj.intodsc
                        FROM par3.itenscomposicao_grupos_tipos_objeto as igo
                        INNER JOIN par3.iniciativa_tipos_objeto as tipos_obj ON tipos_obj.intoid = igo.intoid
                        WHERE igo.igrid = igr.igrid ORDER BY tipos_obj.intodsc ASC
                        ), ', ')
                ) as tiposobjetos,
                TO_CHAR(igrdtinclusao, 'dd/mm/YYYY') as datainclusao,
                igr.igrsituacao as situacao
                FROM {$this->stNomeTabela} as igr
                LEFT JOIN par3.itenscomposicao_grupos_tipos_objeto as igo ON igo.igrid = igr.igrid
                WHERE igrstatus = 'A'
                {$where}
                {$orderBy}
        {$fim}
                ";
        return $sql;
    }

    public function listaGrupo($iniciativa)
    {

        return $this->carregar("SELECT DISTINCT
                igr.igrid   as codigo,
                igr.igrdsc  as descricao,
                tipos_obj.intoid
                FROM  {$this->stNomeTabela} as igr
                LEFT JOIN par3.itenscomposicao_grupos_tipos_objeto as igo ON igo.igrid = igr.igrid
                INNER JOIN par3.iniciativa_tipos_objeto as tipos_obj ON tipos_obj.intoid = igo.intoid
                WHERE igrstatus = 'A' and  tipos_obj.intoid = (SELECT DISTINCT inito.intoid FROM par3.iniciativa as ini LEFT JOIN par3.iniciativa_tipos_objeto as inito ON inito.intoid = ini.intoid  WHERE ini.inistatus = 'A' AND ini.iniid = $iniciativa)
                AND igr.igrsituacao = 'A'                
                ");
    }
}
