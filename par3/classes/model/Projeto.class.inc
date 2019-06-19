<?php
/**
 * Classe de mapeamento da entidade par3.projeto.
 *
 * @version $Id$
 * @since 2017.04.17
 */

/**
 * Par3_Model_Projeto: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Projeto($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Projeto($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $proid Chave Primária - default: nextval('par3.projeto_proid_seq'::regclass)
 * @property string $prosigla sigla do projeto
 * @property string $prodsc Descrição do projeto
 * @property string $prolink Link do projeto
 * @property string $proresumo resumo do projeto
 * @property "char" $prostatus status do projeto: indidica se o projeto foi removido
 * @property "char" $prosituacao situação do projeto: indidica se o projeto está ativo/inativo
 * @property \Datetime(Y-m-d H:i:s) $prodtinclusao data de inclusão do projeto
 * @property string $procpfinclusao cpf do responsável pela inclusão do projeto
 * @property \Datetime(Y-m-d H:i:s) $prodtinativacao data de inativação do projeto
 * @property string $procpfinativacao cpf do responsável pela inativação do projeto
 */
class Par3_Model_Projeto extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.projeto';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'proid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'proid' => null,
        'prosigla' => null,
        'prodsc' => null,
        'prolink' => null,
        'proresumo' => null,
        'prostatus' => null,
        'prosituacao' => null,
        'prodtinclusao' => null,
        'procpfinclusao' => null,
        'prodtinativacao' => null,
        'procpfinativacao' => null,
    );

    //Cabeçalho: código,sigla,descrição,situção
    public function montarSQLSimples($arrPost)
    {
        $prodsc   = sanitizar_string_pesquisa($arrPost['prodsc']);
        $prosigla = trim(str_to_upper($arrPost['prosigla']));
        $where    = $arrPost['prodsc']?" AND upper(removeacento(prodsc)) LIKE upper(removeacento('%{$prodsc}%'))":'';
        $where   .= $arrPost['prosituacao']?" AND prosituacao = '{$arrPost['prosituacao']}'":'';
        $where   .= $arrPost['proid'] && validarInteiro($arrPost['proid'])?" AND proid = {$arrPost['proid']}":'';
        $where   .= $arrPost['prosigla']?" AND upper(prosigla) LIKE '%{$prosigla}%'":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                proid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                proid    as codigo,
                prosigla as sigla,
                prodsc   as descricao,
                prosituacao
                FROM {$this->stNomeTabela}
                WHERE prostatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

    public function verificarExisteProjetoDescricao(array $arrPost)
    {
        $where = $arrPost['proid']?" AND proid != {$arrPost['proid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(prodsc) = '".str_to_upper($arrPost['prodsc'])."' AND prostatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    public function verificarExisteProjetoSigla(array $arrPost)
    {
        $where = $arrPost['proid']?" AND proid != {$arrPost['proid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(prosigla) = '".str_to_upper($arrPost['prosigla'])."' AND prostatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }
    
    public function pegarSQLSelectCombo($proid = array())
    {
    	$proid   = is_array($proid) ? implode(',',$proid):$proid;
    	$where   = "WHERE prostatus = 'A' and prosituacao = 'A'";
    	$where  .= $proid ? " AND proid in({$proid})":'';

    	return "SELECT  proid as codigo, prodsc as descricao FROM {$this->stNomeTabela} $where";
    }
    
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['prodsc']        = array();
        $erros['erros']['prosigla']      = array();
        $erros['erros']['proresumo']     = array();
        $erros['erros']['prosituacao']   = array();
        $erros['erros']['prolink']       = array();

        //Validações
        if($campos['prodsc'] == ''   || $campos['prodsc'] == null){array_push($erros['erros']['prodsc'],'O campo não pode ser vazio');}
        if($campos['prosigla'] == '' || $campos['prosigla'] == null){array_push($erros['erros']['prosigla'],'O campo não pode ser vazio');}
        if(strlen($campos['prosigla']) > 255){array_push($erros['erros']['prosigla'],'O campo não pode conter mais que 255 caracteres');}
        if(strlen($campos['proresumo']) > 255){array_push($erros['erros']['proresumo'],'O campo não pode conter mais que 255 caracteres');}
        if(strlen($campos['prolink']) > 255){array_push($erros['erros']['prolink'],'O campo não pode conter mais que 255 caracteres');}
        if($this->verificarExisteProjetoDescricao($campos)){array_push($erros['erros']['prodsc'], 'Já existe Projeto cadastrado com essa descrição');}
        if($this->verificarExisteProjetoSigla($campos)){array_push($erros['erros']['prosigla'], 'Já existe Projeto cadastrado com essa sigla');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function getIniciativaProjetoById($proid){
        $sql = " SELECT * FROM  par3.projeto as projeto WHERE projeto.proid = {$proid}";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($proid){
        $sql = "SELECT * FROM  par3.projeto as projeto
                    INNER JOIN par3.iniciativa_iniciativas_projetos as  iniciativa_inciativas_projeto ON projeto.proid = iniciativa_inciativas_projeto.proid
                    INNER JOIN par3.iniciativa_planejamento iniciativa_planejamento ON iniciativa_planejamento.iniid = iniciativa_inciativas_projeto.iniid
                    WHERE projeto.proid = {$proid}";
        $result = $this->carregar($sql);
        return $result;
    }
}
