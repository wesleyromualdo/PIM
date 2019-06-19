<?php
/**
 * Classe de mapeamento da entidade acomporc.logws.
 *
 * @version $Id$
 * @since   2017.11.27
 */

/**
 * Simec_Job_Model_Logws: Armazena os dados transitados entre chamadas do WSAlteracaoFinanceira
 *
 * @package Simec\Job
 * @uses    Simec\Db\Modelo
 * @author  Saulo Araújo Correia <saulo.correia@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Simec_Job_Model_Logws($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Simec_Job_Model_Logws($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int                    $lwsid                - default: nextval('acomporc.logws_lwsid_seq'::regclass)
 * @property string                 $lwsrequestcontent    Conteudo da requisicao
 * @property string                 $lwsrequestheader     Conteudo do header da requisicao
 * @property \Datetime(Y-m-d H:i:s) $lwsrequesttimestamp  Timestamp da requisicao
 * @property string                 $lwsresponsecontent   Conteudo da resposta do ws
 * @property string                 $lwsresponseheader    Conteudo do header da resposta
 * @property \Datetime(Y-m-d H:i:s) $lwsresponsetimestamp Timestamp da resposta do ws
 * @property string                 $lwsurl               Url da requisicao
 * @property string                 $lwsmetodo            Metodo chamado no ws
 * @property bool                   $lwserro              Indica se a requisicao resultou ou nao em um erro
 * @property string                 $lwsobservacao        observacoes gerais sobre a requisicao
 */
class Simec_Job_Model_Logws extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'logws';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'lwsid',
    ];

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = [
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'lwsid'                => null,
        'lwsrequestcontent'    => null,
        'lwsrequestheader'     => null,
        'lwsrequesttimestamp'  => null,
        'lwsresponsecontent'   => null,
        'lwsresponseheader'    => null,
        'lwsresponsetimestamp' => null,
        'lwsurl'               => null,
        'lwsmetodo'            => null,
        'lwserro'              => null,
        'lwsobservacao'        => null,
    ];

    public function __construct($esquema, $id = null)
    {
        $this->stNomeTabela = "{$esquema}.{$this->stNomeTabela}";

        parent::__construct($id);
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     *
     * @return mixed[]
     */
    public function getCamposValidacao($dados = [])
    {
        return [
            'lwsid'                => ['Digits'],
            'lwsrequestcontent'    => ['allowEmpty' => true],
            'lwsrequestheader'     => ['allowEmpty' => true],
            'lwsrequesttimestamp'  => ['allowEmpty' => true],
            'lwsresponsecontent'   => ['allowEmpty' => true],
            'lwsresponseheader'    => ['allowEmpty' => true],
            'lwsresponsetimestamp' => ['allowEmpty' => true],
            'lwsurl'               => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 1000])],
            'lwsmetodo'            => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 255])],
            'lwserro'              => ['allowEmpty' => true],
            'lwsobservacao'        => ['allowEmpty' => true],
        ];
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

    /**
     * @param $metodo
     *
     * @return Modelo
     * @throws Exception
     */
    public function getLastItem($metodo)
    {
        try {
            return $this->popularDadosObjeto(
                $this->pegaLinha(
                    <<<SQL
SELECT *
FROM {$this->stNomeTabela}
WHERE
    lwsmetodo = '{$metodo}'
ORDER BY
    lwsid DESC
LIMIT 1
SQL
                )
            );
        } catch (Exception $e) {
            throw $e;
        }
    }
}
