<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_grupo_itens.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Model_Itenscomposicao_grupo_itens: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_grupo_itens($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_grupo_itens($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $gitid  - default: nextval('par3.itenscomposicao_grupos_itens_gitid_seq'::regclass)
 * @property int $igrid
 * @property int $itcid
 * @property int $gitqtd
 */
class Par3_Model_ItensComposicaoGruposItens extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_grupos_itens';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'gitid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'itcid' => array('tabela' => 'par3.itenscomposicao',        'pk' => 'itcid'),
        'igrid' => array('tabela' => 'par3.itenscomposicao_grupos', 'pk' => 'igrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'gitid'  => null,
        'igrid'  => null,
        'itcid'  => null,
        'gitqtd' => null,
        'gitqtd' => null,
    );

    public function vincularItemGrupo($arrayPost)
    {
        try {
            $query_delete = "DELETE FROM {$this->stNomeTabela} where igrid = {$arrayPost['igrid']}";
            $this->executar($query_delete);
            $values = array();

            foreach($arrayPost['itcid'] as $itcid){
                $values[] = "({$arrayPost['igrid']},{$itcid},{$arrayPost['gitqtd'][$itcid]})";
            }

            $values = implode(',',$values);
            $sql = "INSERT INTO {$this->stNomeTabela} (igrid,itcid,gitqtd) VALUES {$values}";
            $this->executar($sql);
            $this->commit();
            $id = 1;
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($id) {
            return $id;
        }
        return 'erro';
    }

    public function desvincularItemGrupo($gitid)
    {
        try {
            $query_delete = "DELETE FROM {$this->stNomeTabela} where gitid = {$gitid}";
            $this->executar($query_delete);
            $this->commit();
            return 1;
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function recuperarItemVinculado($arrPost)
    {
        $gitid = $arrPost['gitid'];
        $sql = "SELECT
                itc.itcdsc,
                git.gitid
                FROM {$this->stNomeTabela} as git
                INNER JOIN par3.itenscomposicao as itc ON itc.itcid = git.itcid
                WHERE git.gitid = {$gitid}
                ";
        return $this->recuperar($sql);
    }

    public function validarInput($campos)
    {
        $erros['erros']['itciderros']  = array();
        $erros['erros']['gitqtderros'] = array();

        if(empty($campos['itcid'])  || $campos['itcid']  == null){array_push($erros['erros']['itciderros'],'Nenhum Item selecionado.');}
        if(empty($campos['gitqtd']) || $campos['gitqtd'] == null){array_push($erros['erros']['gitqtderros'],'Nenhuma quantidade adicionada.');}

        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function sqlListagemSelecionarGrupoItem($arrPost)
    {
        $arrInto = array();
        if($arrPost['intoid']){
            foreach($arrPost['intoid'] as $into){
                if(empty($into)){
                    continue;
                }
                $arrInto[] = $into;
            }
        }

        $intoid  = implode(',',$arrInto);
        $itcnome = sanitizar_string_pesquisa($arrPost['itcnome']);
        $where   = $arrPost['itcnome'] ? " AND upper(removeacento(itc.itcdsc)) LIKE '%{$itcnome}%'" : '';
        $where  .= $intoid ? " AND igrtinto.intoid in({$intoid})" : '';

        $checkbox = <<<DML
         '<input type=\"checkbox\" class=\"i-checks\" name=\"itcid['||itc.itcid||']\" value=\"'||itc.itcid||'\" '
         ||(CASE WHEN (SELECT count(*) FROM par3.itenscomposicao_grupos_itens as git WHERE git.itcid = itc.itcid AND git.igrid = {$arrPost['igrid']}) > 0 THEN 'checked' ELSE '' END)||
         ' />'
DML;

        $selectInputQtd = <<<DML
         '<input type=\"number\" class=\"\" name=\"gitqtd['||itc.itcid||']\" value=\"'
         ||(CASE WHEN (SELECT count(*) FROM par3.itenscomposicao_grupos_itens as git WHERE git.itcid = itc.itcid AND git.igrid = {$arrPost['igrid']}) > 0 THEN
         (SELECT git.gitqtd FROM par3.itenscomposicao_grupos_itens as git WHERE git.itcid = itc.itcid AND git.igrid = {$arrPost['igrid']}) ELSE 0 END)||
         '\" />'
DML;
        $igrid = $arrPost['igrid'];
        $sql = "
            SELECT DISTINCT
            {$checkbox} as checkbox,
            itc.itcid as id,
            itc.itcid as codigo,
            {$selectInputQtd} as qtd,
            itc.itcdsc as descricao
            FROM par3.itenscomposicao as itc
            INNER JOIN par3.itenscomposicao_tipos_objeto  as tipo_obj ON  tipo_obj.itcid = itc.itcid
            INNER JOIN par3.itenscomposicao_grupos_tipos_objeto as igrtinto ON igrtinto.intoid = tipo_obj.intoid
            WHERE  igrtinto.igrid = {$igrid}
            {$where}
            ORDER BY checkbox
        ";
        return $sql;
    }

    public function getMediaTotal($igrid)
    {
        $sql = "
                SELECT
                     formataMonetario(SUM((SELECT (git.gitqtd * avg(item_detalhamento.itdvalor))::numeric
                     FROM par3.itenscomposicao_detalhamento as item_detalhamento
                     LEFT JOIN par3.itenscomposicao as item ON item.itcid = item_detalhamento.itcid
                     WHERE item_detalhamento.itcid = git.itcid
                     AND item_detalhamento.itdsituacao = 'A' AND item_detalhamento.itdstatus = 'A'
                    )))   as total
                FROM {$this->stNomeTabela} as git
                WHERE git.igrid = {$igrid}
                ";
        return $this->recuperar($sql);
    }

    /**
     *Cabeçalho
     * id,codigo,itemcomposicao,quantidade,
     */
    public function sqlDataGrid($arrPost)
    {
        $igrid = $arrPost['igrid'];
        $sql = "
            SELECT
            git.gitid  as id,
            itc.itcdsc as itemcomposicao,
            itc.itcdsc as nome,
            itc.itcespecificacao as descricao,
            git.gitqtd as quantidade,

            'R$'||(formataMonetario((foo.vlr)::numeric) )   as mediavalorunitario,

            'R$'||(formataMonetario(git.gitqtd * (foo.vlr)::numeric) )   as mediasubtotal

            FROM {$this->stNomeTabela} as git
            INNER JOIN par3.itenscomposicao as itc ON itc.itcid = git.itcid
            LEFT  JOIN
                (SELECT
                    avg(item_detalhamento.itdvalor) as vlr,
                    item_detalhamento.itcid
                 FROM par3.itenscomposicao_detalhamento as item_detalhamento
                 LEFT JOIN par3.itenscomposicao as item ON item.itcid = item_detalhamento.itcid
                 WHERE item_detalhamento.itdsituacao = 'A' AND item_detalhamento.itdstatus = 'A'
                 GROUP BY item_detalhamento.itcid
            ) AS foo ON foo.itcid = itc.itcid
            WHERE git.igrid = {$igrid}
        ";
            return $sql;
    }
}
