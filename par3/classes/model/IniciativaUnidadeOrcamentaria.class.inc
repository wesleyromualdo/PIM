<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_unidade_orcamentaria.
 *
 * @version $Id$
 * @since 2017.06.13
 */

/**
 * Par3_Model_Iniciativa_unidade_orcamentaria: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_unidade_orcamentaria($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_unidade_orcamentaria($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iuoid  - default: nextval('par3.iniciativa_unidade_orcamentaria_iuoid_seq'::regclass)
 * @property string $iuodsc
 * @property string $iuostatus  - default: 'A'::bpchar
 */
class Par3_Model_IniciativaUnidadeOrcamentaria extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_unidade_orcamentaria';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iuoid',
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
        'iuoid' => null,
        'iuodsc' => null,
        'iuostatus' => null,
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
            'iuoid' => array('Digits'),
            'iuodsc' => array(new Zend_Validate_StringLength(array('max' => 500))),
            'iuostatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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

    public function pegarSQLSelectCombo()
    {
        $sql = "SELECT
                	iuoid as codigo,
                	iuodsc as descricao
                FROM
                	par3.iniciativa_unidade_orcamentaria
                ORDER BY
                	2";

        return $sql;
    }

}
