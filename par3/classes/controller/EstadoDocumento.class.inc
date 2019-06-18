<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/18/2017
 * Time: 11:08 AM
 */

class Par3_Controller_EstadoDocumento extends Controle {
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_EstadoDocumento();
    }

    public function getSqlSituacaoEstadoDocumentoPar(){
        return $this->model->getSqlListaSituacaoPar();
    }

    public function getSqlSituacaoEstadoDocumentoParObras(){
        return $this->model->getSqlListaSituacaoParObras();
    }

}