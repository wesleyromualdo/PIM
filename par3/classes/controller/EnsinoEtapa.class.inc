<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/2/2017
 * Time: 2:56 PM
 */

class Par3_Controller_EnsinoEtapa{
    private $model;


    public function __construct()
    {
        $this->model = new Par3_Model_EnsinoEtapa();
    }

    public function createEtapaEnsino($etaid){
        if(!is_null($etaid)){
            try{
                return (object) $this->model->findById($etaid);
            }catch(Exception $e){
                echo $e->getMessage();
            }
        }
    }
    public function getListEnsinoEtapa($nivid = null, $etaid = null){
        return $this->model->getFormListaEnsinoEtapa($nivid,$etaid);
    }
}