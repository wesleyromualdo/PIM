<?php
/**
 * Classe de mapeamento da entidade public.cargo.
 *
 * @version $Id$
 *
 * @since 2016.11.04
 */

/**
 * Public_Model_Cargo: sem descricao.
 *
 * @uses Simec\Db\Modelo
 *
 * @author Victor Eduardo Barreto <victor@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Public_Model_Cargo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Public_Model_Cargo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $carid  - default: nextval('cargo_carid_seq'::regclass)
 * @property string $cardsc
 */
class Public_Model_Cargo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada
     */
    protected $stNomeTabela = 'public.cargo';

    /**
     * @var string[] Chave primaria
     */
    protected $arChavePrimaria = array(
        'carid',
    );

    /**
     * @var mixed[] Chaves estrangeiras
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela
     */
    protected $arAtributos = array(
        'carid' => null,
        'cardsc' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     *
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'carid' => array('Digits'),
            'cardsc' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))),
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
