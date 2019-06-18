<?php
/**
 * Classe de mapeamento da entidade public.job.
 *
 * @version $Id: Job.php 135291 2017-12-11 13:26:51Z saulocorreia $
 * @since   2017.11.06
 */

/**
 * Simec_Job_Model_Job: tabela de gerenciamento de jobs da classe Simec_Job_Manager
 *
 * @package Simec\Job
 * @uses    Simec\Db\Modelo
 * @author  Saulo Araújo Correia <saulo.correia@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Simec_Job_Model_Job($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Simec_Job_Model_Job($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int                    $jobid           - default: nextval('job_jobid_seq'::regclass)
 * @property string                 $jobtoken        token para identificação do job - default: ((md5(((random())::text || (clock_timestamp())::text)))::uuid)::character varying
 * @property string                 $jobstatus       A - Ativo / I - Inativo - default: 'A'::bpchar
 * @property string                 $jobclassname    nome da classe que sera executada, sendo ela estendida a classe Simec_Job_Abstract
 * @property \Datetime(Y-m-d H:i:s) $jobdatainsercao data de inserção da linha - default: now()
 */
class Simec_Job_Model_Job extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'job.job';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'jobid',
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
        'jobid'           => null,
        'jobstatus'       => null,
        'jobclassname'    => null,
        'jobtoken'        => null,
        'jobdatainsercao' => null,
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
            'jobid'           => ['Digits'],
            'jobtoken'        => [new Zend_Validate_StringLength(['max' => 36])],
            'jobstatus'       => [new Zend_Validate_StringLength(['max' => 1])],
            'jobclassname'    => [new Zend_Validate_StringLength(['max' => 255])],
            'jobdatainsercao' => [],
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
     * @param string $jobClassName
     *
     * @return int
     */
    public function findJobTokenByJobClassName($jobClassName)
    {
        if (empty($jobClassName)) {
            return [];
        }

        $sql = <<<SQL
SELECT jobtoken
  FROM {$this->stNomeTabela}
 WHERE jobclassname = '{$jobClassName}'
   AND jobstatus = 'A'
SQL;

        return $this->pegaUm($sql);
    }

    /**
     * @param int $jobToken
     *
     * @return string
     */
    public function findClassNameByJobToken($jobToken)
    {
        if (empty($jobToken)) {
            return [];
        }

        $sql = <<<SQL
SELECT jobclassname
  FROM {$this->stNomeTabela}
 WHERE jobtoken = '{$jobToken}'
   AND jobstatus = 'A'
SQL;

        return $this->pegaUm($sql);
    }

    /**
     * @param $jobToken
     *
     * @return array|bool|mixed|NULL|string
     */
    public function findIdByJobToken($jobToken)
    {
        if (empty($jobToken)) {
            return [];
        }

        $sql = <<<SQL
SELECT jobid
  FROM {$this->stNomeTabela}
 WHERE jobtoken = '{$jobToken}'
   AND jobstatus = 'A'
SQL;

        return $this->pegaUm($sql);
    }

    /**
     * @param $zendId
     * @param $host
     *
     * @return array|mixed|NULL
     */
    public function getJobsInfoByIdPai($zendId, $host)
    {
        if (empty($host)) {
            $host = "jlghost IS NULL ";
        } else {
            $host = "jlghost = '{$host}'";
        }

        $sql = <<<SQL
WITH RECURSIVE included_parts(
        pai
    , jlgid
    , jobid
    , jlgzendid
    , jlgzendidpredecessor
    , jlgclassname
    , jlgbaseurl
    , jlgparams
    , jlgoptions
    , jlgoutput
    , jlgstatus
    , jlgdatainsercao
    , jlgerror
    , jlghost
) AS (
    SELECT
        jlgzendid AS pai
        , jlgid
        , jobid
        , jlgzendid
        , jlgzendidpredecessor
        , jlgclassname
        , jlgbaseurl
        , jlgparams
        , jlgoptions
        , jlgoutput
        , jlgstatus
        , jlgdatainsercao
        , jlgerror
        , jlghost
     FROM job.joblog j
    WHERE j.{$host}
    UNION ALL
    SELECT
        i.pai
        , j.jlgid
        , j.jobid
        , j.jlgzendid
        , j.jlgzendidpredecessor
        , j.jlgclassname
        , j.jlgbaseurl
        , j.jlgparams
        , j.jlgoptions
        , j.jlgoutput
        , j.jlgstatus
        , j.jlgdatainsercao
        , j.jlgerror
        , j.jlghost
    FROM job.joblog j, included_parts i
    WHERE
        i.jlgzendidpredecessor = j.jlgzendid
        AND j.{$host}
)
SELECT DISTINCT *
FROM included_parts
WHERE
    pai = {$zendId}
    AND {$host}
ORDER BY jlgid
LIMIT 20
SQL;

        return $this->carregar($sql);
    }
}
