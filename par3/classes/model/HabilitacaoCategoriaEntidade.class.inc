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
 *
 */
class Par3_Model_HabilitacaoCategoriaEntidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.categoriaentidades';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'cteid',
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
        'cteid' => null,
        'ctecodigo' => null,
        'ctenome' => null,
        'ctestatus' => null
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
        $tpenome = sanitizar_string_pesquisa($arrPost['ctenome']);

        $where = "WHERE ctestatus = 'A'";

        $where .= $arrPost['ctenome']?" AND upper(removeacento(ctenome)) LIKE upper(removeacento('%{$tnjnome}%'))":'';
//        $where .= $arrPost['tpehabilitacao']?" AND tpehabilitacao = '{$arrPost['tpehabilitacao']}'": "AND tpehabilitacao IN ('S', 'N')";
        $where .= $arrPost['cteid'] && validarInteiro($arrPost['cteid']) ?" AND cteid = {$arrPost['cteid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                cteid AS id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                cteid AS codigo,
                ctecodigo AS codigonatureza,
                ctenome AS descricao
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
                  cteid as codigo,
                  ctenome as descricao
                FROM {$this->stNomeTabela}
                WHERE
                  ctestatus = 'A'";
                
        return $sql;
        
    }
    
    public function pegarSelectComboTipo($tipo){
        
        $sql = "SELECT DISTINCT
                    cte.cteid,
                    cte.ctenome
                 FROM
                    {$this->stNomeTabela} cte
                 INNER JOIN habilita.categorianaturezajuridica cnj ON cte.cteid = cnj.cteid AND cnjstatus = 'A'
                 INNER JOIN habilita.naturezajuridica ntj ON ntj.ntjid = cnj.ntjid AND ntj.ntjstatus = 'A'
                 INNER JOIN habilita.naturezajuridicarelacionada njr ON njr.ntjid = ntj.ntjid AND njrstatus = 'A'
                 INNER JOIN habilita.tiponaturezajuridica tnj ON tnj.tnjid = njr.tnjid AND tnjstatus = 'A'
                 WHERE
                    cte.ctestatus = 'A' AND
                    tnj.tnjcodigo = '{$tipo}'
                 ORDER BY
                    ctenome";
                    
         return $this->carregar($sql);
        
    }

    public function recuperarCategoriaEntidade($entid){
        $sql = "SELECT
                    c.cteid,
                    c.ctenome
                FROM {$this->stNomeTabela} c
                INNER JOIN habilita.entidade e on e.cteid = c.cteid
                WHERE 
                    ctestatus = 'A' AND e.entid = {$entid} ";
            
        return $this->pegaLinha($sql);
    }
}
