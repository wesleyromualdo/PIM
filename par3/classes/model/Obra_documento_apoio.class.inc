<?php
/**
 * Classe de mapeamento da entidade par3.obra_documento_apoio.
 *
 * @version $Id$
 * @since 2017.07.10
 */

/**
 * Par3_Model_Obra_documento_apoio: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_documento_apoio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_documento_apoio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $odaid  - default: nextval('par3.obra_documento_apoio_odaid_seq'::regclass)
 * @property int $arqid 
 * @property string $odadsc 
 * @property string $odasituacao 
 * @property string $odastatus 
 */
class Par3_Model_Obra_documento_apoio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_documento_apoio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'odaid',
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
        'odaid' => null,
        'arqid' => null,
        'odadsc' => null,
        'odasituacao' => null,
        'odastatus' => null,
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
            'odaid' => array('Digits'),
            'arqid' => array('allowEmpty' => true,'Digits'),
            'odadsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'odasituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'odastatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
