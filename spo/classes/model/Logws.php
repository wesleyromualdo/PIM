<?php
/**
 * Classe de mapeamento da entidade recorc.logws.
 *
 * @version $Id$
 * @since 2018.12.06
 */

/**
 * Spo_Ws_Siop_Model_Logws: Armazena os dados transitados entre chamadas do WSAlteracaoFinanceira
 *
 * @package Spo\Ws
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spo_Ws_Siop_Model_Logws($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spo_Ws_Siop_Model_Logws($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property bool $lwserro Indica se a requisicao resultou ou nao em um erro
 * @property string $lwsmetodo Metodo chamado no ws
 * @property string $lwsurl Url da requisicao
 * @property \Datetime(Y-m-d H:i:s) $lwsresponsetimestamp Timestamp da resposta do ws
 * @property string $lwsresponseheader Conteudo do header da resposta
 * @property string $lwsresponsecontent Conteudo da resposta do ws
 * @property \Datetime(Y-m-d H:i:s) $lwsrequesttimestamp Timestamp da requisicao
 * @property string $lwsrequestheader Conteudo do header da requisicao
 * @property string $lwsrequestcontent Conteudo da requisicao
 * @property int $lwsid  - default: nextval('recorc.logws_lwsid_seq'::regclass)
 */
class Spo_Ws_Siop_Model_Logws extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela;

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'lwsid',
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
        'lwserro' => null,
        'lwsmetodo' => null,
        'lwsurl' => null,
        'lwsresponsetimestamp' => null,
        'lwsresponseheader' => null,
        'lwsresponsecontent' => null,
        'lwsrequesttimestamp' => null,
        'lwsrequestheader' => null,
        'lwsrequestcontent' => null,
        'lwsid' => null,
    );

    public function __construct($schema = null, $id = null, $tempocache = null)
    {
        $this->stNomeTabela = empty($schema) ? $_SESSION['sisdiretorio'].'.logws' : $schema.'.logws';
        parent::__construct($id, $tempocache);
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'lwserro' => array('allowEmpty' => true),
            'lwsmetodo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'lwsurl' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'lwsresponsetimestamp' => array('allowEmpty' => true),
            'lwsresponseheader' => array('allowEmpty' => true),
            'lwsresponsecontent' => array('allowEmpty' => true),
            'lwsrequesttimestamp' => array('allowEmpty' => true),
            'lwsrequestheader' => array('allowEmpty' => true),
            'lwsrequestcontent' => array('allowEmpty' => true),
            'lwsid' => array('Digits'),
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
