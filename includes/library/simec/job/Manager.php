<?php

/**
 * $Id: Manager.php 147040 2018-12-18 23:48:07Z victormachado $
 *
 * Class Simec_Job_Manager
 *
 * TODO : mensagem de sucesso e de erro
 * TODO : retorno de JSON para cada job
 * TODO : retirar os statics para trabalhar com instancia, uma vez que não posso chamar a mesma classe duas vezes no mesmo job (stack overflow)
 * TODO : verificar carga do servidor antes de executar os jobs, se estiver alta agendar os jobs para depois
 */
class Simec_Job_Manager
{
    /**
     * @var string
     */
    protected static $urlProd = 'http://vm-simec-lap-05.mec.gov.br';

    /**
     * @var string
     */
    protected static $baseUrl = '/seguranca/scripts_exec/jobs.php?job=';

    /**
     * @var Simec_Job_Model_Interface
     */
    protected static $jobModel;

    /**
     * @param Simec_Job_Model_Interface $jobModel
     */
    public static function setJobModel(Simec_Job_Model_Interface $jobModel)
    {
        self::$jobModel = $jobModel;
    }

    /**
     * @return Simec_Job_Model_Interface
     */
    public static function getJobModel()
    {
        if (empty(self::$jobModel)) {
            self::$jobModel = new Simec_Job_Controller_Job();
        }

        return self::$jobModel;
    }

    /**
     * @param string $jobClassName
     * @param array  $params
     * @param array  $options
     *
     * @return int ID do Job Queue
     */
    public static function start($jobClassName, $params = [], $options = [])
    {
        list($predecessor, $predecessorParams) = self::getJobDependency($jobClassName);

        if ($predecessor) {
            $options['predecessor'] = self::start($predecessor, $predecessorParams);
        }

        $options = array_merge($options, ['name' => $jobClassName, 'job_timeout' => 999999]);

        // buscar pelo token
        $jobToken = self::loadJobToken($jobClassName);

        // criando url
        if (IS_PRODUCAO)
            $url = self::$urlProd . self::$baseUrl . "{$jobToken}&db=dbsimec";
        else
            $url = self::$baseUrl . "{$jobToken}&db={$_SESSION['baselogin']}";

        // acrescenta o host do simecs
        $params = array_merge(['Simec_Job_Host' => $_ENV['HOSTNAME']], $params);

        // criar job queue no zend
        $zendJobId = (new ZendJobQueue())->createHttpJob(
            $url,
            simec_utf8_encode_recursive_and_key(array_merge(['Simec_Job_Host' => $_ENV['HOSTNAME']], $params)),
            $options
        );

        // salvar no log
        (new Simec_Job_Controller_Joblog())->salvar(
            $jobToken,
            $zendJobId,
            $options['predecessor'],
            $jobClassName,
            $url,
            $params,
            $options
        );

        return $zendJobId;
    }

    /**
     * @param int $jobDbId
     *
     * @throws Simec_Job_Exception
     */
    public static function run($jobDbId)
    {
        // buscar nome da classe pelo token
        $jobClassName = self::getJobModel()->findClassNameByJobToken($jobDbId);

        // verifica se a classe eh uma subclasse de Simec_Job_Abstract
        if (!(new ReflectionClass($jobClassName))->isSubclassOf(Simec_Job_Abstract::class)) {
            throw new Simec_Job_Exception("{$jobClassName} não é uma subclasse de " . Simec_Job_Abstract::class);
        }

        // executar o RUN da classe
        $jobClassName::run();
    }

    /**
     * @param $zendId
     * @param $host
     *
     * @return array
     */
    public static function getJobsInfoByIdPai($zendId, $host)
    {
        return self::getJobModel()->getJobsInfoByIdPai($zendId, $host);
    }

    /**
     * Converte da constante do zend para texto
     *
     * @param int $status
     *
     * @return string
     */
    public static function getStatusZendJobQueue($status)
    {
        switch ($status) {
            case ZendJobQueue::STATUS_PENDING: // 0
                return 'Pendente';

            case ZendJobQueue::STATUS_WAITING_PREDECESSOR: // 1
                return 'Aguardando Predecessor';

            case ZendJobQueue::STATUS_RUNNING: // 2
                return 'Em execução';

            case ZendJobQueue::STATUS_COMPLETED: // 3
                return 'Finalizado';

            case ZendJobQueue::STATUS_OK: // 4
                return 'OK';

            case ZendJobQueue::STATUS_FAILED: // 5
                return 'Falhou';

            case ZendJobQueue::STATUS_LOGICALLY_FAILED: // 6
                return 'Falhou Logicamente';

            case ZendJobQueue::STATUS_TIMEOUT: // 7
                return 'Tempo Esgotado';

            case ZendJobQueue::STATUS_REMOVED: // 8
                return 'Removido';

            case ZendJobQueue::STATUS_SCHEDULED: // 9
                return 'Agendado';

            case ZendJobQueue::STATUS_SUSPENDED: // 10
                return 'Suspenso';

            default:
                return '(Desconhecido)';
        }
    }

    public static function render(callable $callback = null, $id = null, Simec_Job_Render $render = null)
    {
        if (is_null($render)) {
            $render = new Simec_Job_Render($callback, $id, 'msgSucesso');
        }

        $render->render();
    }

    /**
     * @param $jobClassName
     *
     * @return string
     * @throws Simec_Job_Exception
     */
    protected static function loadJobToken($jobClassName)
    {
        // verifica se a model eh valida e subclasse do Simec_Job_Model_Interface
        if (is_null(self::getJobModel())
            || (!self::getJobModel() instanceof Simec_Job_Model_Interface)) {
            throw new Simec_Job_Exception('Model de decodicação de ID inválida.');
        }

        // busca o token pela nome da classe
        $jobToken = self::getJobModel()->findJobTokenByClassName($jobClassName);

        // se não encontrado ele insere a classe e retorna o token
        if (empty($jobToken)) {
            $jobToken = self::getJobModel()->saveNewJobToken($jobClassName);
        }


        return $jobToken;
    }

    /**
     * @param string $jobClassName
     *
     * @return array
     */
    protected static function getJobDependency($jobClassName)
    {
        return [
            $jobClassName::getPredecessor(),
            $jobClassName::getPredecessorParams()
        ];
    }
}
