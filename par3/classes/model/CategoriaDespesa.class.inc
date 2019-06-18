<?php
/**
 * Classe de mapeamento da entidade par3.categoriadespesa.
 *
 * @version $Id$
 * @since 2017.06.09
 */

/**
 * Model_Categoriadespesa: sem descricao
 *
 * @package Model\Categoriadespesa
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Categoriadespesa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Categoriadespesa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ctdid Chave Primária - default: nextval('par3.categoriadespesa_ctdid_seq'::regclass)
 * @property string $ctddsc Descrição da Categoria de Despesa
 */
class Par3_Model_CategoriaDespesa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.categoriadespesa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ctdid',
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
        'ctdid' => null,
        'ctddsc' => null,
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
            'ctdid' => array('Digits'),
            'ctddsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
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

    public function selectCombo()
    {
        return "SELECT ctdid as codigo,ctddsc as descricao FROM {$this->stNomeTabela}";
    }
}
