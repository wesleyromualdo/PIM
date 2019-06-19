<?php

require_once APPRAIZ .  "includes/ActiveRecord/classes/base/corporativo/entidades_juridicaBase.php";

class entidades_juridica extends entidades_juridicaBase {
    /**
     * 
     */
    static public function getByCnpj($cnpj)
    {
        $sql = "SELECT
                    
                    enjid,
                    enjdsc,
                    enfid,
                    enjstatus,
                    enjcnpj,
                    enjrazaosocial,
                    enjinscricaoestadual,
                    enjemail,
                    enjtelefone,
                    enjendcep,
                    enjendlogradouro,
                    enjendcomplemento,
                    enjendnumero,
                    enjendbairro,
                    muncod

                FROM
                    corporativo.entidades_juridica
                WHERE
                    enjcnpj = ? ";

        $rs        = ActiveRecord::ExecSQL($sql, array($cnpj));

        $retorno = $rs->fields;

        return $retorno;
    }
}





