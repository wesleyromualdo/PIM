<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 8/30/2017
 * Time: 1:49 PM
 */

class Par3_Controller_IniciativaPlanejamentoDesdobramento extends Controle{

    private $model;

    public function __construct(){
        $this->model = new Par3_Model_IniciativaPlanejamentoDesdobramentos();
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $array
     * @param $desid
     * @return boolean true = existe || false = não existe
     * @description <verifica se existe o desdobramento tendo como filtro dimensão + iniciativa + etapa + modalidade + desdobramento>
     */
    public function verificaExistenciaDoDesdobramentoByIdDesdobramento(array $array, $desid){
        if(is_array($array) && $desid != ''){
            return $this->model->verificaExistenciaDoDesdobramentoByIdDesdobramento($array, $desid);
        }
    }
}