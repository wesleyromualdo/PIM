<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/18/2017
 * Time: 11:11 AM
 */

class Par3_Model_EstadoDocumento extends Modelo{


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return string
     * @description Retorna (sql) lista de situações das obras que estão no fluxo do par
     */
    public function getSqlListaSituacaoPar(){
        $wf = PAR3_WORKFLOW_FLUXO_OBRAS;
        $sql = "
               select esd.esdid as codigo, esd.esdordem||' - '||esd.esddsc as descricao from workflow.estadodocumento as esd where tpdid = {$wf} order by esd.esdordem asc";
        return $sql;
    }

    public function getSqlListaSituacaoParObras(){
        $wf = PAR3_WORKFLOW_FLUXO_OBRAS;
        $sql = "
                SELECT  
                esdid as codigo,
                esddsc as descricao
                FROM workflow.estadodocumento
                WHERE tpdid =  {$wf}
                ORDER BY esddsc";
        return $sql;
    }



}