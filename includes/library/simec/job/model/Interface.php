<?php

/**
 * $Id: Interface.php 135210 2017-12-08 13:58:38Z saulocorreia $
 * Interface Simec_Job_Model_Interface
 */
interface Simec_Job_Model_Interface
{
    /**
     * @param string $jobClassName
     *
     * @return string
     */
    public function findJobTokenByClassName($jobClassName);

    /**
     * @param string $jobClassName
     *
     * @return string
     */
    public function saveNewJobToken($jobClassName);

    /**
     * @param int $jobToken
     *
     * @return string
     */
    public function findClassNameByJobToken($jobToken);

    /**
     * @param $zendId
     *
     * @return array
     */
    public function getJobsInfoByIdPai($zendId, $host);
}
