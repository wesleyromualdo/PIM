<?php
/**
 * Classe de mapeamento da entidade public.joblog.
 *
 * @version $Id: Joblog.php 135237 2017-12-08 17:37:25Z saulocorreia $
 * @since   2017.11.06
 */

/**
 * Simec_Job_Model_Joblog: Tabela de log de atividades dos job
 *
 * @package Simec\Job
 * @uses    Simec\Db\Modelo
 * @author  Saulo Araújo Correia <saulo.correia@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Simec_Job_Model_Joblog($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Simec_Job_Model_Joblog($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int                    $jlgid                - default: nextval('joblog_jlgid_seq'::regclass)
 * @property int                    $jlgzendid            job id do zend server
 * @property int                    $jlgzendidpredecessor job id do zend server do predecessor
 * @property int                    $jobid
 * @property string                 $jlgclassname         classe chamada pelo job
 * @property string                 $jlgbaseurl           url base chamada pelo job
 * @property string                 $jlgparams            parametros passados para o job
 * @property string                 $jlgoptions           options passados para o job
 * @property string                 $jlgstatus            status do job
 * @property string                 $jlgoutput            output do job
 * @property string                 $jlgerror             erro do job
 * @property string                 $jlghost              host do job
 * @property \Datetime(Y-m-d H:i:s) $jlgdatainsercao      data de insercao - default: now()
 */
class Simec_Job_Model_Joblog extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'job.joblog';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'jlgid',
    ];

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = [
        'jobid' => ['tabela' => 'job', 'pk' => 'jobid'],
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'jlgid'                => null,
        'jlgzendid'            => null,
        'jlgzendidpredecessor' => null,
        'jobid'                => null,
        'jlgclassname'         => null,
        'jlgbaseurl'           => null,
        'jlgparams'            => null,
        'jlgoptions'           => null,
        'jlgdatainsercao'      => null,
        'jlgoutput'            => null,
        'jlgstatus'            => null,
        'jlghost'              => null,
        'jlgerror'             => null
    ];

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
            'jlgid'                => ['Digits'],
            'jlgzendid'            => ['Digits'],
            'jlgzendidpredecessor' => ['allowEmpty' => true, 'Digits'],
            'jobid'                => ['allowEmpty' => true, 'Digits'],
            'jlgclassname'         => [new Zend_Validate_StringLength(['max' => 255])],
            'jlgbaseurl'           => [new Zend_Validate_StringLength(['max' => 255])],
            'jlgparams'            => ['allowEmpty' => true],
            'jlgoptions'           => ['allowEmpty' => true],
            'jlgoutput'            => ['allowEmpty' => true],
            'jlgerror'             => ['allowEmpty' => true],
            'jlgstatus'            => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 255])],
            'jlgdatainsercao'      => [],
            'jlghost'              => [new Zend_Validate_StringLength(['max' => 200])],
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
        return parent::antesSalvar();
    }

    /**
     * @param $zendJobId
     * @param $host
     *
     * @throws Exception
     */
    public function carregarPorZendID($zendJobId, $host)
    {
        if (empty($host)) {
            $host = " jlghost IS NULL ";
        } else {
            $host = " jlghost = '{$host}'";
        }

        $sql = <<<SQL
SELECT jlgid
  FROM {$this->stNomeTabela}
 WHERE jlgzendid = '{$zendJobId}'
   AND {$host}
SQL;
        try {
            $this->carregarPorId($this->pegaUm($sql));
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * Atualizar as informações no fim do job
     *
     * @param $zendJobId
     * @param $output
     * @param $status
     * @param $erro
     *
     * @throws Exception
     */
    public function atualizarFimJob($zendJobId, $output, $status, $erro, $host)
    {
        try {
            $this->carregarPorZendID($zendJobId, $host);

            $this->jlgoutput = $output;
            $this->jlgerror = $erro;
            $this->jlgstatus = $status;

            $this->alterar();
            $this->commit();
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * Atualizar as informações no fim do job
     *
     * @param $zendJobId
     * @param $output
     * @param $host
     *
     * @throws Exception
     */
    public function atualizarOutput($zendJobId, $output, $host)
    {
        try {
            $this->carregarPorZendID($zendJobId, $host);

            $this->jlgoutput = $output;

            $this->alterar();
            $this->commit();
        } catch (Exception $e) {
            throw $e;
        }

    }
}
