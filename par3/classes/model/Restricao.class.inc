<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 8/23/2017
 * Time: 1:28 PM
 */

class Par3_Model_Restricao extends Modelo{

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

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description Esta função é utilizada para carregar as restrições da obra
     * @param $obrid
     * @return array|mixed|NULL
     */
    public function existeRestricoesDaObra($obrid){
        $sql = "select restricao.*
                       from obras2.restricao as restricao 
                       JOIN workflow.documento d ON d.docid = restricao.docid AND d.esdid NOT IN (1142, 1497)
                       where restricao.obrid = {$obrid}
                       and restricao.rststatus = 'A'
                       and restricao.rstitem = 'R'";
        $arrResult = $this->carregar($sql);
        if(count($arrResult) > 0){
            return true;
        }else{
            return false;
        }
     }
    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description Esta função é utilizada para carregar as restrições da obra
     * @param $obrid
     * @return array|mixed|NULL
     */
    public function getRestricoesDaObra($obrid){
        $sql = "select restricao.*
                       from obras2.restricao as restricao 
                       JOIN workflow.documento d ON d.docid = restricao.docid AND d.esdid NOT IN (1142, 1497)
                       where restricao.obrid = {$obrid}
                       and restricao.rststatus = 'A'
                       and restricao.rstitem = 'R'";
        return $this->carregar($sql);
     }
     
    /**
    * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
    * @description Esta função é utilizada para carregar as restrições da obra
    * @param $obrid
    * @return array|mixed|NULL
    */
    public function existeRestricaoInstrumentoUnidadeById($inuid){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
        $unidade = '';
        $sql = '';
        if($instrumentoUnidade['itrid'] == 1 || $instrumentoUnidade['itrid'] == 3){ $unidade = 'E';}elseif($instrumentoUnidade['itrid'] == 2){$unidade = 'M';}

        if($instrumentoUnidade['itrid'] == 1 || $instrumentoUnidade['itrid'] == 3){
            //if for estado
            $sql = "select DISTINCT obra.obrid from obras2.obras as obra 
            				inner join workflow.documento d on d.docid = obra.docid 
                            inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                            inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497) 
                            inner join entidade.endereco as endereco on endereco.endid = obra.endid
                            /*inner join territorios.estado as estado on estado.estuf = endereco.estuf*/
                            left join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = endereco.estuf 
                            inner join obras2.empreendimento as empreendimento on empreendimento.empid = obra.empid
                            where 
                            d.esdid != 769 
                            and 
                            obra.tpoid NOT IN(104,105)
                            and 
                            restricao.rststatus = 'A'
                            and
                            restricao.rstitem = 'R'
                            and
                            instrumentounidade.inuid = {$inuid} -- id da unidade
                            and
                            empreendimento.empesfera IN('{$unidade}') 
                            AND
                            obra.obridpai IS NULL
                            AND obra.obrstatus = 'A';";
        }elseif($instrumentoUnidade['itrid'] == 2){

            //if for municipio
            $sql = "select DISTINCT obra.obrid from obras2.obras as obra 
            				inner join workflow.documento d on d.docid = obra.docid
                            inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                            inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497)
                            inner join entidade.endereco as endereco on endereco.endid = obra.endid
                            /*inner join territorios.municipio as municipio on municipio.estuf = endereco.estuf  and endereco.muncod = municipio.muncod*/
                            left join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = endereco.estuf and endereco.muncod = instrumentounidade.muncod
                            inner join obras2.empreendimento as empreendimento on empreendimento.empid = obra.empid
                            where 
                            obra.obridpai IS NULL
                            AND
                            d.esdid != 769 
                            and 
                            obra.tpoid NOT IN(104,105)
                            and 
                            restricao.rststatus = 'A'
                            and
                            restricao.rstitem = 'R'
                            and
                            instrumentounidade.inuid = {$inuid} -- id da unidade
                            and
                            empreendimento.empesfera IN('{$unidade}') 
                            AND obra.obrstatus = 'A';";
        }

        $resultado = $this->pegaUm($sql,0,600000);
        if($resultado){
            return true;
        }else{
            return false;
        }
    }



    /**
     * @author <leo.oliveira@cast.com.br>
     * @description < Descobre se a unidade é do tipo 1(estado) ou 2(municipio), retorna array das obras que possuem restrições tuleap hstória 12936>
     * @param $inuid
     * @return array;
     */
    public function getObrasRestricoesInstrumentoUnidadeById($inuid){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
        $sql = '';
        $unidade = '';
        if($instrumentoUnidade['itrid'] == 1 || $instrumentoUnidade['itrid'] == 3){ $unidade = 'E';}elseif($instrumentoUnidade['itrid'] == 2){$unidade = 'M';}
        if (is_array($instrumentoUnidade)) {
            //se for uma unidade do tipo  Estado
            if ($instrumentoUnidade['itrid'] == 1 || $instrumentoUnidade['itrid'] == 3) {
                $sql = "select DISTINCT obra.obrid,obra.obrnome,obra.obrdsc from obras2.obras as obra 
                					inner join workflow.documento d on d.docid = obra.docid
                                    inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                                    inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497)
                                    inner join entidade.endereco as endereco on endereco.endid = obra.endid
                                    /* inner join territorios.estado as estado on estado.estuf = endereco.estuf */
                                    left join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = endereco.estuf 
                                    inner join obras2.empreendimento as empreendimento on empreendimento.empid = obra.empid
                                    where 
                                    obridpai IS NULL
                                    AND
                                    d.esdid != 769 
                                    and 
                                    obra.tpoid NOT IN(104,105)
                                    and 
                                    instrumentounidade.inuid = {$inuid}
                                    and 
                                    empreendimento.empesfera IN('{$unidade}')
                                    and obra.obrstatus = 'A'
                                                                        order by obra.obrid ASC";
            } //se for uma unidade do tipo  Municipio
            elseif ($instrumentoUnidade['itrid'] == 2) {
                $sql = "select DISTINCT obra.* from obras2.obras as obra 
                						inner join workflow.documento d on d.docid = obra.docid 
                                        inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                                        inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497)
                                        inner join entidade.endereco as endereco on endereco.endid = obra.endid
                                        /*inner join territorios.municipio as municipio on municipio.estuf = endereco.estuf  and endereco.muncod = municipio.muncod*/
                                        left join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = endereco.estuf and endereco.muncod = instrumentounidade.muncod
                                        inner join obras2.empreendimento as empreendimento on empreendimento.empid = obra.empid
                                        where
                                        obridpai IS NULL
                                        AND
                                        d.esdid != 769 
                                        and 
                                    	obra.tpoid NOT IN(104,105)
                                    	and 
                                        restricao.rststatus = 'A'
                                        and
                                        restricao.rstitem = 'R'
                                        and
                                        instrumentounidade.inuid = {$inuid} -- id da unidade
                                        and
                                        empreendimento.empesfera IN('{$unidade}') 
                                        AND obra.obrstatus = 'A';";
            }
            $result = $this->carregar($sql);
            return $result;
        } else {
            return array();
        }
    }
}