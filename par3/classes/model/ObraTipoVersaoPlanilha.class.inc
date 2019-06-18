<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_versao_planilha.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_tipo_versao_planilha: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_tipo_versao_planilha($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_tipo_versao_planilha($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $tplid  - default: nextval('par3.obra_tipo_versao_planilha_tplid_seq'::regclass)
 * @property int $ovpid
 * @property string $tpldsc
 * @property string $tplsituacao
 * @property string $tplstatus
 */
class Par3_Model_ObraTipoVersaoPlanilha extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_versao_planilha';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'tplid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ovpid' => array('tabela' => 'par3.obra_tipo_versao_projeto', 'pk' => 'ovpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'tplid'             => null,
        'ovpid'             => null,
        'tpldsc'            => null,
        'tplsituacao'       => null,
        'tplstatus'         => null,
        'tpltipo_regiao'    => null
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['ovpid']            = array();
        $erros['erros']['tpldsc']           = array();
        $erros['erros']['tpltipo_regiao']   = array();

        //Validações
        if($campos['ovpid'] == ''           || $campos['ovpid'] == null){    array_push($erros['erros']['ovpid'],'O campo não pode ser vazio');}
        if($campos['tpldsc'] == ''          || $campos['tpldsc'] == null){          array_push($erros['erros']['tpldsc'],'O campo não pode ser vazio');}
        if($campos['tpltipo_regiao'] == ''  || $campos['tpltipo_regiao'] == null){  array_push($erros['erros']['tpltipo_regiao'],'O campo não pode ser vazio');}

        if(in_array($campos['tpltipo_regiao'], array('E', 'M'))){
            if(is_array($campos['estuf'])){
                foreach($campos['estuf'] as $k => $estuf){
                    if($estuf[1] == '' || $estuf[1] == null){
                        $erros['erros']["estuf-$k"] = array('O campo não pode ser vazio');
                    }
                }
            }
        }

        if(in_array($campos['tpltipo_regiao'], array('M'))){
            if(is_array($campos['muncod'])){
                foreach($campos['muncod'] as $k => $muncod){
                    if($muncod[1] == '' || $muncod[1] == null){
                        $erros['erros']["muncod-$k"] = array('O campo não pode ser vazio');
                    }
                }
            }
        }

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function pegarSQLSelectCombo($ovpid = null)
    {
        $where = $ovpid ? "ovpid = $ovpid" : '1=2';
        $sql = "SELECT tplid as codigo, tplid||' - '||tpldsc as descricao FROM {$this->stNomeTabela} WHERE $where";
        return $sql;
    }

    public function montarSQLSimples($arrPost)
    {
        $where = array("tplstatus = 'A'");
        if($arrPost['ovpid']) $where[] = "tpl.ovpid = {$arrPost['ovpid']}";

        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                    tplid||'_plan' as id,
                    ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                    tplid as codigo,
                    tplid,
                    tpldsc,
                    tplstatus,
                    case
                    	when tpltipo_regiao = 'N' then 'Nacional'
                    	when tpltipo_regiao = 'E' then 'Estadual'
                    	when tpltipo_regiao = 'M' then 'Municipal'
                    end as tpltipo_regiao
                FROM
                    par3.obra_tipo_versao_planilha tpl
                WHERE
                    ".implode(' AND ', $where)."
                ORDER BY
                    tpldsc";

        return $sql;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'tplid' => array('Digits'),
            'ovpid' => array('allowEmpty' => true,'Digits'),
            'tpldsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'tplsituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'tplstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
        );
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

}
