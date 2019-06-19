<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/28/2017
 * Time: 11:09 AM
 */


class Par3_Controller_Usuario extends Controle
{

    private $modelSegurancaUsuario;

    public function __construct($usucpf = null)
    {
        $this->modelSegurancaUsuario = new Par3_Model_Usuario();
    }

    public function getDadosDoUsuarioByCPF($cpf){
        $arrDadosDoUsuario = $this->modelSegurancaUsuario->getUsuarioByCPF($cpf);
        return $arrDadosDoUsuario;
    }

}