<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 14/06/2016
 * Time: 16:44
 */

class DadoEspacoParticipacaoMunicipio extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    public function carregaDado(){
        global $db;
        $tpmapa = $this->tpmapa;
        $estuf = is_array($this->estuf) ? " and mun.estuf in ('".implode("','", $this->estuf)."') " : " ";
        $muncod = is_array($this->muncod) ? " and mun.muncod in ('".implode("','", $this->muncod)."') " : " ";
        $sepjoin = '';
        switch($tpmapa){
            case 'f': $sepjoin = "sepforum"; break;
            case 'c': $sepjoin = "sepcons"; break;
            case 's': $sepjoin = "sepsist"; break;
        }

        $sql = <<<DML
            select
                    esp.espid,
                    esp.estuf,
                    esp.muncod,
                    mun.mundescricao,
                    case
                        when {$sepjoin}.sepcor is null then '#FFFFFF'
                        else {$sepjoin}.sepcor
                    end as cor,
                    case
                        when {$sepjoin}.sepdsc is null then 'Sem Informação'
                        else {$sepjoin}.sepdsc
                    end as situacao
                from sase.espacoparticipacao esp
                left join sase.sitespparticipacao sepforum on esp.sepidforum = sepforum.sepid
                left join sase.sitespparticipacao sepcons on esp.sepidcons = sepcons.sepid
                left join sase.sitespparticipacao sepsist on esp.sepidsist = sepsist.sepid
                inner join territorios.municipio mun on esp.muncod = mun.muncod
                where 1=1
                {$estuf}
                {$muncod}
                order by mun.estuf, mun.mundescricao
DML;

        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda($tpmapa = null, $estuf = null){
        global $db;
        $tipoMapa = $tpmapa == null ? trim($this->tpmapa) : trim($tpmapa);
        $tpdid = '';
        $docCampo = "";
        $estuf = $estuf == null ? $this->estuf : $estuf;
        $estados = is_array($estuf) ? " and esp.estuf in ('".implode("','", $estuf)."') " : " ";
        switch($tipoMapa){
            case 'f': $tpdid = TPDID_SASE_ESPACOAVALIACAO_FORUM; $docCampo = "docid1"; break;
            case 'c': $tpdid = TPDID_SASE_ESPACOAVALIACAO_CONSELHO; $docCampo = "docid2"; break;
            case 's': $tpdid = TPDID_SASE_ESPACOAVALIACAO_SISTEMA; $docCampo = "docid3"; break;
        }

        $sql = <<<DML
            with
                temp_esd as (select esdid from workflow.estadodocumento where tpdid = {$tpdid}),
                temp_sep as (select esp.*, sep.esdid from sase.espacoparticipacao esp left join workflow.documento doc on esp.{$docCampo} = doc.docid left join sase.sitespparticipacao sep on doc.esdid = sep.esdid where 1=1 {$estados})
            select
                sep.*,
                count(esp.espid) as total
            From sase.sitespparticipacao sep
            inner join temp_esd esd on esd.esdid = sep.esdid
            left join temp_sep esp on esp.esdid = sep.esdid
            GROUP BY 1,2,3
            ORDER BY sep.sepid ASC
DML;
        $res = $db->carregar($sql);
        $sql = <<<DML
            select count(*) as total from sase.espacoparticipacao esp where docid1 is null {$estados}
DML;
        $totalSI = $db->pegaUm($sql);

        $res[0]['total'] = $res[0]['total'] + $totalSI;
        return $res;
    }
}