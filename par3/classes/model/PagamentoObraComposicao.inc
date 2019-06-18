<?php
/**
 * Classe de mapeamento da entidade par3.pagamentoobracomposicao.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_Pagamentoobracomposicao: sem descricao
 *
 * @package Model\Pagamentoobracomposicao
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_PagamentoObraComposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_PagamentoObraComposicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $pmcid  - default: nextval('par3.pagamentoobracomposicao_pmcid_seq'::regclass)
 * @property int $pagid 
 * @property int $eocid 
 * @property numeric $pmcpercentualpag 
 * @property numeric $pmcvalorpagamento 
 * @property string $pmcstatus  - default: 'A'::bpchar
 */
class Par3_Model_PagamentoObraComposicao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.pagamentoobracomposicao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'pmcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pagid' => array('tabela' => 'par3.pagamento', 'pk' => 'pagid'),
        'eocid' => array('tabela' => 'par3.empenhoobracomposicao', 'pk' => 'eocid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'pmcid' => null,
        'pagid' => null,
        'eocid' => null,
        'pmcpercentualpag' => null,
        'pmcvalorpagamento' => null,
        'pmcstatus' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'pmcid' => array('Digits'),
            'pagid' => array('Digits'),
            'eocid' => array('allowEmpty' => true,'Digits'),
            'pmcpercentualpag' => array('allowEmpty' => true),
            'pmcvalorpagamento' => array('allowEmpty' => true),
            'pmcstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
