<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/28/2017
 * Time: 11:12 AM
 */



class Par3_Model_Usuario extends Modelo
{

    public function __construct($usucpf = null)
    {

    }

    public function getUsuarioByCPF($cpf){
        $sql = "select * from seguranca.usuario as usu where usu.usucpf = '{$cpf}';";
        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

}