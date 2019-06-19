<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 10/03/2016
 * Time: 17:11
 */

class Sase_Model_Municipiosporacao extends Modelo
{
    protected $stNomeTabela = 'sase.municipiosporacao';
    protected $arChavePrimaria = array('mpaid');
    protected $arAtributos = array(
        'mpaid' => null,
        'ravid' => null,
        'aedid' => null,
        'qtdmun' => null
    );

    public function montaSqlRelatorio($ravid){
        if(!$ravid) return array();
        $tpdid = TPDID_SASE_SITUACAO_PLANOCARREIRA;
        $sql = <<<DML
            select
                aed.aedid,
                esd.esddsc as esdOrigem,
                esd2.esddsc as esdDestino,
                mpa.qtdmun,
                mpa.mpaid
            from workflow.acaoestadodoc aed
            inner join workflow.estadodocumento esd on aed.esdidorigem = esd.esdid
            inner join workflow.estadodocumento esd2 on aed.esdiddestino = esd2.esdid
            left join sase.municipiosporacao mpa on aed. aedid = mpa.aedid and mpa.ravid = {$ravid}
            where esd.tpdid = {$tpdid}
            and aed.aedid in (4070, 4071, 3883, 3889, 4078, 3895, 3901)
            order by esd.esdordem
DML;
        return $sql;
    }

    public function getTabelaRelatorio($ravid, $perCampos = false){
        $sql = $this->montaSqlRelatorio($ravid);
        if(!$sql){ return false; }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
            ->setQuery($sql)
            ->setCabecalho(array('Situaçao Anterior', 'Situação Atual', 'N° de municípios que apresentaram alteração'))
            ->setCampos(array('esdOrigem', 'esdDestino', 'qtdnum'))
            ->setFormOff()
            ->esconderColunas(array('aedid', 'mpaid'));
        if($perCampos){
            $lista->addCallbackDeCampo('qtdmun', 'getQtdMun');
        }
        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
}