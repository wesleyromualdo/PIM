<?php
/**
 * Classe de mapeamento da entidade par3.empenhoparcomposicao.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_Empenhoparcomposicao: Tabela com a composição do empenho PAR
 *
 * @package Model\Empenhoparcomposicao
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_EmpenhoParComposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_EmpenhoParComposicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $epcid Chave primaria da tabela par3.empenhoparcomposicao - default: nextval('par3.empenhoparcomposicao_epcid_seq'::regclass)
 * @property int $empid Chave estrangeira da tabela par3.empenho
 * @property int $ppcid Chave estrangeira da tabela par3.processoparcomposicao
 * @property numeric $epcpercentualemp Percentual empenhado
 * @property numeric $epcvalorempenho Valor do empenho
 * @property string $epcstatus Status A - Ativo I - Inativo - default: 'A'::bpchar
 */
class Par3_Model_EmpenhoParComposicao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empenhoparcomposicao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'epcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ppcid' => array('tabela' => 'par3.processoparcomposicao', 'pk' => 'ppcid'),
        'empid' => array('tabela' => 'par3.empenho', 'pk' => 'empid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'epcid' => null,
        'empid' => null,
        'ppcid' => null,
        'epcpercentualemp' => null,
        'epcvalorempenho' => null,
        'epcstatus' => null,
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
            'epcid' => array('Digits'),
            'empid' => array('Digits'),
            'ppcid' => array('Digits'),
            'epcpercentualemp' => array('allowEmpty' => true),
            'epcvalorempenho' => array('allowEmpty' => true),
            'epcstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
