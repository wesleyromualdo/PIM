<?php

class Par3_Controller_ContratoExecucao extends Controle
{

    protected $model;

    public function __construct()
    {
        $this->model = new Par3_Model_InstrumentoUnidadeEntidade();
    }

    /**
     * @param $dotid - par3.documentotermo
     * @return string
     */
    public function recuperarProcessoPorDotId($dotid){
        $sql = "SELECT pronumeroprocesso, dotnumero FROM par3.processo pro 
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                WHERE 
                prostatus = 'A' AND
                dot.dotid = {$dotid}";
        return $this->model->pegaLinha($sql);
    }

}