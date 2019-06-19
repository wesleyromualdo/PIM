<?php
/**
 * Classe de mapeamento da entidade par3.empenhoobracomposicao.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_Empenhoobracomposicao: sem descricao
 *
 * @package Model\Empenhoobracomposicao
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_EmpenhoObraComposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_EmpenhoObraComposicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $eocid  - default: nextval('par3.empenhoobracomposicao_eocid_seq'::regclass)
 * @property int $empid 
 * @property int $pocid 
 * @property numeric $eocpercentualemp 
 * @property numeric $eocvalorempenho 
 * @property string $eocstatus  - default: 'A'::bpchar
 */
class Par3_Model_EmpenhoObraComposicao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empenhoobracomposicao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'eocid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pocid' => array('tabela' => 'par3.processoobracomposicao', 'pk' => 'pocid'),
        'empid' => array('tabela' => 'par3.empenho', 'pk' => 'empid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'eocid' => null,
        'empid' => null,
        'pocid' => null,
        'eocpercentualemp' => null,
        'eocvalorempenho' => null,
        'eocstatus' => null,
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
            'eocid' => array('Digits'),
            'empid' => array('Digits'),
            'pocid' => array('Digits'),
            'eocpercentualemp' => array('allowEmpty' => true),
            'eocvalorempenho' => array('allowEmpty' => true),
            'eocstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
