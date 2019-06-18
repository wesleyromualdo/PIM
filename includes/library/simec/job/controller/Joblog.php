<?php
/**
 * Classe de controle do  joblog
 *
 * @category Class
 * @package  A1
 * @version  $Id: Joblog.php 135237 2017-12-08 17:37:25Z saulocorreia $
 * @author   SAULO ARAÚJO CORREIA <saulo.correia@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-11-2017
 * @link     no link
 */


/**
 * Simec_Job_Controller_Joblog
 *
 * @category  Class
 * @package   A1
 * @author    <>
 * @license   GNU simec.mec.gov.br
 * @link      no link
 */
class Simec_Job_Controller_Joblog
{
    private $model;

    public function __construct()
    {
        $this->model = new Simec_Job_Model_Joblog($_GET['jlgid']);

    }

    /**
     * Função gravar - grava os dados no joblog
     *
     * @param $zendJobId
     * @param $zendJobIdPredecessor
     * @param $className
     * @param $baseUrl
     * @param $params
     * @param $options
     *
     * @return bool|int
     */
    public function salvar($jobDbId, $zendJobId, $zendJobIdPredecessor, $className, $baseUrl, $params, $options)
    {
        try {
            $this->model->popularDadosObjeto([
                'jobid'                => (new Simec_Job_Controller_Job())->findIdByJobToken($jobDbId),
                'jlgzendid'            => $zendJobId,
                'jlgzendidpredecessor' => $zendJobIdPredecessor,
                'jlgclassname'         => $className,
                'jlgbaseurl'           => $baseUrl,
                'jlgparams'            => simec_json_encode($params),
                'jlgoptions'           => simec_json_encode($options),
                'jlghost'              => $_ENV['HOSTNAME']
            ]);

            $sucesso = $this->model->salvar();

            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            return false;
        }

        return $sucesso;
    }

    /**
     * @param $zendJobId
     * @param $status
     * @param $erro
     *
     * @throws Exception
     */
    public function atualizarFimJob($zendJobId, $status, $erro, $host = null)
    {
        try {
            (new Simec_Job_Model_Joblog())
                ->atualizarFimJob(
                    $zendJobId
                    , preg_replace("/[\r]|[\n]/", "<br>", ob_get_contents())
                    , Simec_Job_Manager::getStatusZendJobQueue($status)
                    , $erro
                    , $host ? $host : $_ENV['HOSTNAME']
                );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param      $zendJobId
     * @param null $host
     *
     * @throws Exception
     */
    public function atualizarOutput($zendJobId, $host = null)
    {
        try {
            (new Simec_Job_Model_Joblog())
                ->atualizarOutput(
                    $zendJobId
                    , preg_replace("/[\r]|[\n]/", "<br>", ob_get_contents())
                    , $host ? $host : $_ENV['HOSTNAME']
                );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $zendJobID
     * @param $host
     *
     * @return array
     * @throws Exception
     */
    public function getInfo($zendJobID, $host)
    {
        try {
            $this->model->carregarPorZendID($zendJobID, $host);

            return (new Simec_Job_Controller_Job())->getInfo($this->model->getDados());
        } catch (Exception $e) {
            throw $e;
        }
    }
}
