<?php
/**
 * Classe de mapeamento da entidade par3.obra_categoria.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_categoria: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_categoria($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_categoria($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $octid  - default: nextval('par3.obra_categoria_octid_seq'::regclass)
 * @property string $octdsc 
 * @property string $octstatus 
 */
class Par3_Model_ObraCategoria extends Modelo
{
    const CATEGORIA_AMPLIACAO_QUADRA = 1;
    const CATEGORIA_AMPLIACAO_COBERTURA = 2;
    const CATEGORIA_AMPLIACAO_ESCOLA = 3;

    const CATEGORIA_CONSTRUCAO = 1;
    const CATEGORIA_AMPLIACAO  = 2;
    const CATEGORIA_REFORMA    = 3;

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_categoria';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'octid',
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
        'octid' => null,
        'octdsc' => null,
        'octstatus' => null,
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
            'octid' => array('Digits'),
            'octdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'octstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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

    public function getComboCategoriaAmpliacao(){
        return array(Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_QUADRA => 'Quadra',
            Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_COBERTURA => 'Cobertura',
            Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_ESCOLA => 'Escola' );
    }

}
