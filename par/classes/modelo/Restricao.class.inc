<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 8/23/2017
 * Time: 10:16 AM
 */



class Restricao extends Modelo{

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description Esta função é utilizada para carregar os dados da restrição pelo rstid
     */
    public function getRestricoes($rstid){
        if(isset($rstid) and $rstid != '' and $rstid != null){
            $sql = "SELECT * FROM obras2.restricao as restricao WHERE restricao.rstid = {$rstid}";
            return $this->pegaLinha($sql);
        }else{
            return false;
        }
    }
}