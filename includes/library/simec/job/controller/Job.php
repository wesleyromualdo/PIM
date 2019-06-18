<?php

/**
 * Classe de controle do  job
 *
 * @category Class
 * @package  A1
 * @version  $Id: Job.php 135237 2017-12-08 17:37:25Z saulocorreia $
 * @author   SAULO ARAÚJO CORREIA <saulo.correia@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-11-2017
 * @link     no link
 */
class Simec_Job_Controller_Job implements Simec_Job_Model_Interface
{
    private $model;

    public function __construct()
    {
        $this->model = new Simec_Job_Model_Job($_GET['jobid']);
    }

    /**
     * @param string $jobClassName
     *
     * @return int
     */
    public function findJobTokenByClassName($jobClassName)
    {
        return $this->model->findJobTokenByJobClassName($jobClassName);
    }

    /**
     * @param string $jobClassName
     *
     * @return int|bool
     */
    public function saveNewJobToken($jobClassName)
    {
        $model = new Simec_Job_Model_Job();
        $model->jobclassname = $jobClassName;

        $jobId = $model->salvar();
        $model->commit();

        $model->carregarPorId($jobId);

        return $model->jobtoken;
    }

    /**
     * @param int $jobToken
     *
     * @return string
     */
    public function findClassNameByJobToken($jobToken)
    {
        return $this->model->findClassNameByJobToken($jobToken);
    }

    /**
     * @param $jobToken
     *
     * @return array|bool|mixed|NULL|string
     */
    public function findIdByJobToken($jobToken)
    {
        return $this->model->findIdByJobToken($jobToken);
    }

    /**
     * @param $zendId
     * @param $host
     *
     * @return array|null
     */
    public function getJobsInfoByIdPai($zendId, $host)
    {
        $jobs = $this->model->getJobsInfoByIdPai($zendId, $host);

        if (is_array($jobs)) {
            foreach ($jobs as $index => &$job) {
                $job = $this->getInfo($job);

            }
        }

        return $jobs;
    }

    /**
     * @param $job
     *
     * @return mixed
     */
    public function getInfo($job)
    {
        // busca o nome do Label
        $job['label'] = (new $job['jlgclassname']())->getName();

        // inicializa o ZendJobQueue para buscar Info e Status
        try {
            $zend = new ZendJobQueue();
            $job['info'] = simec_utf8_decode_recursive($zend->getJobInfo($job['jlgzendid']));
        } catch (Exception $e){

        }

        // converte status de CONST para TEXT
        $job['status'] = $job['info']['status'] = Simec_Job_Manager::getStatusZendJobQueue($job['info']['status']);

        return $job;
    }
}
