<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/14/2017
 * Time: 5:13 PM
 */

class Par3_Controller_Municipio extends Controle{

    private $modelMunicipio;


    public function __construct()
    {
        $this->modelMunicipio = new Par3_Model_Municipio();
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $uf
     * @return string
     * @description Esta função retorna sql para montar a lista de municípios com/sem filtro por UF
     * @example com filtro: $objMunicipio->getSqlListaMunicipioByUF('AC') //retorna todos os municípios do Acre
     *          sem filtro: $objMunicipio->getSqlListaMunicipioByUF() //retorna todos os municípios
     *
     */
    public function getFormListaMunicipioByUF($uf = array()){
        if($uf != "null"){
            return ($arrListaMunicipio = $this->modelMunicipio->getSqlListaMunicipioByUF($uf));
        }else{
            return ($arrListaMunicipio = $this->modelMunicipio->getSqlListaMunicipioByUF());
        }
        return array();
    }


}