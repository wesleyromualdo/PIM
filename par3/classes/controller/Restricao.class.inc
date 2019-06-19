<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 8/23/2017
 * Time: 5:04 PM
 */

class Par3_Controller_Restricao extends Controle{

    private $model;


    public function __construct()
    {

        require_once APPRAIZ."par3/classes/model/Restricao.class.inc";
        $this->model = new Par3_Model_Restricao();
    }


    public function existeRestricaoIntrumentoUnidadeById($inuid){
        if(isset($inuid) && $inuid != '' && $inuid != null){
            return $this->model->existeRestricaoInstrumentoUnidadeById($inuid);
        }
    }

}

?>