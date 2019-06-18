<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 8/21/2017
 * Time: 2:21 PM
 */

class Pendencia extends Modelo
{


    /**
     * @author <leo.oliveira@cast.com.br>
     * @description < Descobre se a unidade é do tipo 1(estado) ou 2(municipio), retorna array das obras que possuem restrições tuleap hstória 12936>
     * @param $inuid
     * @return array;
     */
    private function getObrasRestricoesInstrumentoUnidadeById($inuid){
        $modelInstrumentoUnidade = new InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
        //ver($instrumentoUnidade);
        $sql = '';
        if(is_array($instrumentoUnidade)){
            //se for uma unidade do tipo  Estado
            if($instrumentoUnidade['itrid'] == 2){
                $sql = "select DISTINCT obra.obrid,obra.obrnome,obra.obrdsc,
                            (select array_agg(restricao.rstid) as rstdsc 
                                    from obras2.restricao as restricao 
                                    JOIN workflow.documento d ON d.docid = restricao.docid AND d.esdid NOT IN (1142, 1497)
                                    where restricao.obrid = obra.obrid
                                    and restricao.rststatus = 'A'
                                    and restricao.rstitem = 'R') as restricoes
                        from obras2.obras as obra 
                              inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                            inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497)
                            inner join entidade.endereco as endereco on endereco.endid = obra.endid
                            /*inner join territorios.municipio as municipio on municipio.estuf = endereco.estuf  and endereco.muncod = municipio.muncod*/
                            left join par.instrumentounidade as instrumentounidade on endereco.muncod = instrumentounidade.muncod
                            where 
                            restricao.rststatus = 'A'
                            and
                            restricao.rstitem = 'R'
                            and
                            instrumentounidade.inuid = {$inuid} -- id da unidade;";
            }
            //se for uma unidade do tipo  Municipio
            elseif ($instrumentoUnidade['itrid'] == 1){
                $sql = "select DISTINCT obra.obrid,obra.obrnome,obra.obrdsc,
                            (select array_agg(restricao.rstid) as rstdsc 
                                    from obras2.restricao as restricao 
                                    JOIN workflow.documento d ON d.docid = restricao.docid AND d.esdid NOT IN (1142, 1497)
                                    where restricao.obrid = obra.obrid
                                    and restricao.rststatus = 'A'
                                    and restricao.rstitem = 'R') as restricoes
                        from obras2.obras as obra 
                            inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                            inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497) 
                            inner join entidade.endereco as endereco on endereco.endid = obra.endid
                            /*inner join territorios.municipio as municipio on municipio.estuf = endereco.estuf  and endereco.muncod = municipio.muncod*/
                            left join par3.instrumentounidade as instrumentounidade on endereco.muncod = instrumentounidade.muncod
                            where 
                            restricao.rststatus = 'A'
                            and
                            restricao.rstitem = 'R'
                            and
                            instrumentounidade.inuid = $inuid";
            }
            //ver($sql);
            $result = $this->carregar($sql);
            return $result;
        }else{
            return array();
        }

    }//end getObrasRestricoesInstrumentoUnidadeById


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inuid
     */
    public function consultaExisteRestricao($inuid){
        if(isset($inuid) && $inuid != '' && $inuid != null){
                $arrRestricao = $this->getObrasRestricoesInstrumentoUnidadeById($inuid);
                //ver($arrRestricao);
                return $arrRestricao;
        }
    }

}