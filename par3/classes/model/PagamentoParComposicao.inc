<?php
/**
 * Classe de mapeamento da entidade par3.pagamentoparcomposicao.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_Pagamentoparcomposicao: sem descricao
 *
 * @package Model\Pagamentoparcomposicao
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_PagamentoParComposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_PagamentoParComposicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ppcid  - default: nextval('par3.pagamentoparcomposicao_ppcid_seq'::regclass)
 * @property int $pagid 
 * @property int $epcid 
 * @property numeric $ppcpercentualpag 
 * @property numeric $ppcvalorpagamento 
 * @property string $ppcstatus  - default: 'A'::bpchar
 */
class Par3_Model_PagamentoParComposicao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.pagamentoparcomposicao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ppcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pagid' => array('tabela' => 'par3.pagamento', 'pk' => 'pagid'),
        'epcid' => array('tabela' => 'par3.empenhoparcomposicao', 'pk' => 'epcid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ppcid' => null,
        'pagid' => null,
        'epcid' => null,
        'ppcpercentualpag' => null,
        'ppcvalorpagamento' => null,
        'ppcstatus' => null,
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
            'ppcid' => array('Digits'),
            'pagid' => array('Digits'),
            'epcid' => array('allowEmpty' => true,'Digits'),
            'ppcpercentualpag' => array('allowEmpty' => true),
            'ppcvalorpagamento' => array('allowEmpty' => true),
            'ppcstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
