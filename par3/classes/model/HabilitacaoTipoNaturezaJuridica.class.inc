<?php
/**
 * Classe de mapeamento da entidade par3.tiponaturezajuridica.
 *
 * @version $Id$
 * @since 2018.06.26
 */

/**
 * Par3_Model_HabilitacaoTipoNaturezaJuridica: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Victor de Mello Benzi <victorbenzi@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_HabilitacaoTipoNaturezaJuridica($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_TipoNaturezaJuridica($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_HabilitacaoTipoNaturezaJuridica extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.tiponaturezajuridica';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'tnjid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
//    protected $arChaveEstrangeira = array(
//        'usucpfinclusao'  => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
//        'usucpfalteracao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
//    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'tnjid' => null,
        'tnjcodigo' => null,
        'tnjnome' => null,
        'tnjstatus' => null
    );

    /**
     * Validators.Par3_Model_HabilitacaoTipoNaturezaJuridica
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
        $tpenome = sanitizar_string_pesquisa($arrPost['tnjnome']);

        $where = "WHERE tnjstatus = 'A'";

        $where .= $arrPost['tnjnome']?" AND upper(removeacento(tnjnome)) LIKE upper(removeacento('%{$tnjnome}%'))":'';
//        $where .= $arrPost['tpehabilitacao']?" AND tpehabilitacao = '{$arrPost['tpehabilitacao']}'": "AND tpehabilitacao IN ('S', 'N')";
        $where .= $arrPost['tnjid'] && validarInteiro($arrPost['tnjid']) ?" AND tnjid = {$arrPost['tnjid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                tnjid AS id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                tnjid AS codigo,
                tnjcodigo AS codigonatureza,
                tnjnome AS descricao
                FROM {$this->stNomeTabela}
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

    public function recuperarPorTipoNatureza($tnjid)
    {
        return $this->recuperarTodos('*',array("tnjid = {$tnjid}"));
    }
    
    
    public function pegarSelectCombo()
    {
        $sql = "SELECT
                  tnjid as codigo,
                  tnjcodigo || '. ' || tnjnome as descricao
                FROM {$this->stNomeTabela}
                WHERE
                  tnjstatus = 'A'
                ORDER BY
                   tnjcodigo";
                
        return $sql;
        
    }
}
