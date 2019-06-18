<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 17/03/2016
 * Time: 10:23
 */
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
class Sase_Model_Munporestadodoc extends Modelo
{
    protected $stNomeTabela = 'sase.munporestadodoc';
    protected $arChavePrimaria = array('mpeid');
    protected $arAtributos = array(
        'mpeid' => null,
        'ravid' => null,
        'esdid' => null,
        'mpeqtdmun' => null,
        'mpeacao' => null
    );

    public function montaDados($ravid){
        if(!$ravid) return array();
        global $db;
        $tpdid = TPDID_SASE_SITUACAO_PLANOCARREIRA;
        $sql = <<<DML
            select
                esd.esdid,
                esd.esddsc,
                mpe.mpeid,
                mpe.mpeqtdmun,
                mpe.mpeacao
            from workflow.estadodocumento esd
            left join sase.munporestadodoc mpe on esd.esdid = mpe.esdid and mpe.ravid = {$ravid}
            where tpdid = {$tpdid}
            order by esdordem
DML;
        $res = $db->carregar($sql);
        return $res;
    }

    public function getTabelaRelatorio($ravid, $perCampos = false){
        $res = $this->montaDados($ravid);
        $tab = new HTML_Table('table_munporsituacao', 'table table-striped table-bordered table-hover table-condensed tabela-listagem');
        $tab->addRow();
        $tab->addCell('Etapa', '', 'header', array('width' => '30%'));
        $tab->addCell('Numero de município', '', 'header', array('width' => '20%'));
        $tab->addCell('Ações propostas', '', 'header');
        $total = 0;
        $mpeid = 0;
        foreach ($res as $r) {
            $mpeid = !empty($r['mpeid']) ? $r['mpeid'] : 0;
            $tab->addRow();
            $tab->addCell($r['esddsc'], '', 'data');
            $tab->addCell($perCampos ? retornaCampoHidden($r['esdid'], 'esdid').retornaCampoHidden($mpeid, 'mpeid').retornaCampo($r['mpeqtdmun'], 'mpeqtdmun', false) : $r['mpeqtdmun'], '', 'data');
            $tab->addCell($perCampos ? retornaCampoTextArea($r['mpeacao'], 'mpeacao', false, "", 300, 2, 5) : $r['mpeacao'], '', 'data');
            $total = $total + $r['mpeqtdmun'];
        }
        $tab->addRow();
        $tab->addCell('Total', '', 'data');
        $tab->addCell($total, '', 'data');
        $tab->addCell('', '', 'data');
        return $tab->display();
    }
}