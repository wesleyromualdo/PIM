<?php
/**
 * Classe de mapeamento da entidade gestaogabinete.das.
 *
 * @version $Id$
 * @since 2016.11.08
 */

/**
 * Gestaogabinete_Model_Das: Lista de DAS disponiveis para selecao no sistema de solicitacao de indicacao
 *
 * @package Gestaogabinete\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio@teste.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Gestaogabinete_Model_Das($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Gestaogabinete_Model_Das($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $dasid  - default: nextval('gestaogabinete.das_dasid_seq'::regclass)
 * @property string $dascodigo Codigo do DAS. Ex: 101.3
 * @property string $dasstatus (A)tivo ou (I)nativo - default: 'A'::bpchar
 */
class Gestaogabinete_Model_Das extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'gestaogabinete.das';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'dasid',
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
        'dasid' => null,
        'dascodigo' => null,
        'dasstatus' => null,
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
            'dasid' => array('Digits'),
            'dascodigo' => array(new Zend_Validate_StringLength(array('max' => 5))),
            'dasstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
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
