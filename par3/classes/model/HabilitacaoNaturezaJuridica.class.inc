<?php
/**
 * Classe de mapeamento da entidade par3.naturezajuridica.
 *
 * @version $Id$
 * @since 2018.09.22
 */

/**
 * Par3_Model_HabilitacaoNaturezaJuridica: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Victor de Mello Benzi <victorbenzi@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_HabilitacaoNaturezaJuridica($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_HabilitacaoNaturezaJuridica($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_HabilitacaoNaturezaJuridica extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.naturezajuridica';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ntjid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    //protected $arChaveEstrangeira = array(
    //    'tnjid'  => array('tabela' => 'habilita.tiponaturezajuridica', 'pk' => 'tnjid')
    //);

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ntjid' => null,
        'ntjnome' => null,
  //      'tnjid' => null,
        'ntjstatus' => null
    );

    /**
     * Validators.Par3_Model_HabilitacaoNaturezaJuridica
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
//    public function getCamposValidacao($dados = array())
//    {
//        return array(
//            'tpeid' => array('Digits'),
//            'tpenome' => array('Digits'),
//            'tpehabilitacao' => array('Digits'), //array('allowEmpty' => true),
//            'tpestatus' => array('Digits'),
//        );
//    }
    
    public function montarSQLSimples($arrPost)
    {
        $tpenome = sanitizar_string_pesquisa($arrPost['ntjnome']);

        $where = "WHERE ntjstatus = 'A'";

        $where .= $arrPost['ntjnome']?" AND upper(removeacento(ntjnome)) LIKE upper(removeacento('%{$ntjnome}%'))":'';
//        $where .= $arrPost['tpehabilitacao']?" AND tpehabilitacao = '{$arrPost['tpehabilitacao']}'": "AND tpehabilitacao IN ('S', 'N')";
//        $where .= $arrPost['ntjid'] && validarInteiro($arrPost['ntjid']) ?" AND ntjid = {$arrPost['ntjid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                ntj.ntjid AS id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                ntj.ntjid AS codigo,
                ntj.ntjnome AS descricao,
                (SELECT array_to_string(array(
                    SELECT  DISTINCT
                    tnj.tnjcodigo || '. ' || tnj.tnjnome as tipo
                    FROM
                    habilita.tiponaturezajuridica tnj
                    INNER JOIN habilita.naturezajuridicarelacionada rel ON rel.tnjid = tnj.tnjid
                    WHERE rel.ntjid = ntj.ntjid AND tnj.tnjstatus = 'A' AND rel.njrstatus = 'A'), ', ')
                )  AS tipo
                FROM {$this->stNomeTabela} ntj
                $whereSituacao
                {$where}
                {$orderBy}
                ";
//                 ver($sql);
        return $sql;
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

    public function recuperarPorNatureza($ntjid)
    {
        return $this->recuperarTodos('*',array("ntjid = {$ntjid}"));
    }
    
    public function pegarSelectCombo()
    {
        $sql = "SELECT
                  ntjid as codigo,
                  ntjnome as descricao
                FROM {$this->stNomeTabela}
                WHERE
                  ntjstatus = 'A'";
                
        return $sql;
        
    }
    
}
