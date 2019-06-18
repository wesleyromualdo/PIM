<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/26/2017
 * Time: 2:47 PM
 */

class Par3_Controller_PerfilUsuario{

    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_PerfilUsuario();
    }

    public function getSqlPerfilTecnicoAnaliseEfetuada($statusTecnico = null){
       return $this->model->getSqlPerfilTecnicoObrasMontaCombo($statusTecnico);
    }

}