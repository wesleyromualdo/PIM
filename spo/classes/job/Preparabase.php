<?php

/**
 * Created by PhpStorm.
 * User: saulocorreia
 * Date: 22/11/2017
 * Time: 11:45
 */
class Spo_Job_Preparabase extends Simec_Job_Abstract
{
    /**
     * @var array
     */
    private $params;

    /**
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public function getName()
    {
        return 'Preparando Base';
    }

    protected function init()
    {
        $this->params = $this->loadParams();
    }

    protected function execute()
    {
        echo "Preparando base para armazenamento dos dados";
        $this->salvarOutput();
        foreach ($this->params['limpar'] as $limpar) {
            try {
                $this->setTable($limpar['tabela']);
                $deleteAll = (count($limpar['where']) > 0) ? false : true;
                $this->jobDelete($limpar['where'], $deleteAll);
            } catch (Exception $ex) {
                throw  $ex;
            }
        }
    }

    protected function shutdown()
    {
        // TODO: Implement shutdown() method.
    }
}