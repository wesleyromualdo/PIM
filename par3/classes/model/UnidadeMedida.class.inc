<?php
/**
 * Classe de mapeamento da entidade par3.unidade_medida.
 *
 * @version $Id$
 * @since 2017.05.29
 */

/**
 * Par3_Model_Unidade_medida: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Unidade_medida($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Unidade_medida($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $uniid Chave Primária - default: nextval('par3.unidade_medida_uniid_seq'::regclass)
 * @property string $unidsc Descrição da unidade de medida
 * @property "char" $unistatus status da unidade de medida: indidica se a unidade de medida foi removida
 * @property "char" $unisituacao situação da unidade de medida: indidica se a unidade de medida está ativa/inativa
 * @property \Datetime(Y-m-d H:i:s) $unidtinclusao data de inclusão da unidade de medida
 * @property string $unicpfinclusao cpf do responsável pela inclusao da unidade de medida
 * @property \Datetime(Y-m-d H:i:s) $unidtinativacao data de inativação da unidade de medida
 * @property string $unicpfinativacao cpf do responsável pela inativação da unidade de ,medida
 */
class Par3_Model_UnidadeMedida extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.unidade_medida';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'uniid',
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
        'uniid' => null,
        'unidsc' => null,
        'unistatus' => null,
        'unisituacao' => null,
        'unidtinclusao' => null,
        'unicpfinclusao' => null,
        'unidtinativacao' => null,
        'unicpfinativacao' => null,
    );

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

    public function verificarExisteUnidadeMedida(array $arrPost)
    {
        $where = $arrPost['uniid']?" AND uniid != {$arrPost['uniid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(unidsc) = '".str_to_upper($arrPost['unidsc'])."' AND unistatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }


    //Cabeçaçho: 'Código','Ciclo','Início','Fim','Duração Ciclo','Anos','Vigência','Situação'
    public function montarSQLSimples($arrPost)
    {
        $unidsc = sanitizar_string_pesquisa($arrPost['unidsc']);
        $where  = $arrPost['unidsc']?" AND upper(removeacento(unidsc)) LIKE '%{$unidsc}%'":'';
        $where .= $arrPost['unisituacao']?" AND unisituacao = '{$arrPost['unisituacao']}'":'';
        $where .= $arrPost['uniid']?" AND uniid = {$arrPost['uniid']}":'';
        $where .= $arrPost['univigencia']?" AND univigencia = '{$arrPost['univigencia']}'":'';
        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"ORDER BY uni.uniid";

        $mClasseUnidadeMedida = new Par3_Model_ClasseUnidadeMedida();
        $listaClasses = $mClasseUnidadeMedida->listarClassesArray();

        $classes = array();
        $classesTodosLista = array();
        $cont = 1;
        if ($listaClasses){
            foreach($listaClasses as $classe){
                $classesTodosLista[] = $classe['cniid'];
                $class = str_replace(array(' ', '(', ')', '-', '{', '}'),'_',$classe['cnidsc']);
                if($arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'){
                    $classes[] = <<<DML
                    (CASE WHEN (SELECT count(*) FROM par3.unidade_medida_classe WHERE cniid = {$classe['cniid']} AND uniid = uni.uniid AND uncstatus = 'A') > 0 THEN 'X' ELSE '' END) as classe{$class}
DML;
                }
                else{
                    $classes[] = <<<DML
             (
                '<div class=\"icheckbox-square-green\"><input type=\"checkbox\" class=\"classeunidademedida\" onclick=\"salvarUnidadeClasse({$classe['cniid']},'||uni.uniid||')\" id=\"classecheck{$classe['cniid']}'||uni.uniid||'\"'
                ||(CASE WHEN (SELECT count(*) FROM par3.unidade_medida_classe WHERE cniid = {$classe['cniid']} AND uniid = uni.uniid AND uncstatus = 'A') > 0 THEN 'checked' ELSE '' END)||
                ' /></div>'
             ) as classe{$class}
DML;
                }
            }

            $classesTodosLista = implode(',',$classesTodosLista);
            $classesTodos = '';
            $countClasse = count($listaClasses);
            if($arrPost['requisicao'] != 'imprimir' && $arrPost['requisicao'] != 'xls'){
                $classesTodos = <<<DML
             (
                '<div class=\"icheckbox-square-green\"><input type=\"checkbox\" id=\"classecheck0'||uni.uniid||'todos\"'
                ||(CASE WHEN (SELECT count(*) FROM par3.unidade_medida_classe WHERE cniid IN ({$classesTodosLista}) AND uniid = uni.uniid AND uncstatus = 'A') = {$countClasse} THEN 
                    'checked onclick=\"marcarTodos('''t''',' 
                    ELSE 
                    'onclick=\"marcarTodos('''f''',' 
                    END)||
                ''||uni.uniid||')\" /></div>'
             ) as todos
DML;
            }
        }
        $classesTodos = $listaClasses && $classesTodos != '' ? ','.$classesTodos : '';
        $classeString = $listaClasses ? ','.implode(',',$classes):'';
        //query
        $sql = "SELECT DISTINCT
                uni.uniid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                uni.uniid as codigo,
                uni.unidsc as descricao,
                uni.unisituacao
                {$classesTodos}
                {$classeString}
                FROM {$this->stNomeTabela} as uni
                WHERE unistatus = 'A'
                {$where}
                {$orderBy}
                ";

        return $sql;
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['unidsc'] = array();
        //Validações
        if($campos['unidsc'] == '' || $campos['unidsc'] == null){array_push($erros['erros']['unidsc'],'O campo não pode ser vazio');}
        if($campos['unidsc']){
            if ($this->verificarExisteUnidadeMedida($campos)) {array_push($erros['erros']['unidsc'], 'Já existe Unidade de Medida com a descrição informada');}
        }
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function selectCombo()
    {
        return "SELECT uniid as codigo, unidsc as descricao FROM {$this->stNomeTabela} WHERE unistatus = 'A'";
    }
}
