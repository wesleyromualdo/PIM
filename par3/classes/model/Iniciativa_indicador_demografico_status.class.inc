<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_indicador_demografico_status.
 *
 * @version $Id$
 * @since 2017.07.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Iniciativa_indicador_demografico_status: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_indicador_demografico_status($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_indicador_demografico_status($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $iniidsdesc 
 * @property int $iniidsid  - default: nextval('par3.iniciativa_indicador_demografico_status_iniidsid_seq'::regclass)
 */
class Par3_Model_Iniciativa_indicador_demografico_status extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_indicador_demografico_status';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iniidsid',
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
        'iniidsdesc' => null,
        'iniidsid' => null,
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
            'iniidsdesc' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'iniidsid' => array('Digits'),
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


    public function fetchAll(){
        $sql =  "SELECT * FROM par3.iniciativa_indicador_demografico_status";
        $result = $this->carregar($sql);
        return $result;
    }
}
