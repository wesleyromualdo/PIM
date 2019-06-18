<?php
/**
 * Classe de mapeamento da entidade par.parecercacs.
 *
 * @version $Id$
 * @since 2016.10.17
 */

/**
 * Par_Model_Parecercacs: sem descricao
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par_Model_Parecercacs($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par_Model_Parecercacs($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $pcacsid  - default: nextval('par.parecercacs_pcacsid_seq'::regclass)
 * @property int $dopid 
 * @property string $pcacsobservacao 
 * @property int $pcacsparecer  - default: 1
 * @property string $pcacscompleto  - default: 'I'::bpchar
 * @property string $usucpf 
 * @property int $docid 
 */
class Par_Model_Parecercacs extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.parecercacs';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'pcacsid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'dopid' => array('tabela' => 'par.documentopar', 'pk' => 'dopid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'pcacsid' => null,
        'dopid' => null,
        'pcacsobservacao' => null,
        'pcacsparecer' => null,
        'pcacscompleto' => null,
        'usucpf' => null,
        'docid' => null,
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
            'pcacsid' => array('Digits'),
            'dopid' => array('allowEmpty' => true,'Digits'),
            'pcacsobservacao' => array('allowEmpty' => true),
            'pcacsparecer' => array('Digits'),
            'pcacscompleto' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'docid' => array('allowEmpty' => true,'Digits'),
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
