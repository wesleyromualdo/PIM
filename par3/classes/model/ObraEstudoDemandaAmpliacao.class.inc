<?php
/**
 * Classe de mapeamento da entidade par3.obra_estudo_demanda_ampliacao.
 *
 * @version $Id$
 * @since 2017.08.17
 */

/**
 * Model_Obra_estudo_demanda_ampliacao: sem descricao
 *
 * @package Model\Obra
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Obra_estudo_demanda_ampliacao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Obra_estudo_demanda_ampliacao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $edastatus  - default: 'A'::bpchar
 * @property string $edadsc
 * @property int $edaid  - default: nextval('par3.obra_estudo_demanda_ampliacao_edaid_seq'::regclass)
 */
class Par3_Model_ObraEstudoDemandaAmpliacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_estudo_demanda_ampliacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'edaid',
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
        'edastatus' => null,
        'edadsc' => null,
        'edaid' => null,
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
            'edastatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'edadsc' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'edaid' => array('Digits'),
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
