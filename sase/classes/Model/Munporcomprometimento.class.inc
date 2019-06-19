<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 14/03/2016
 * Time: 10:39
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Munporcomprometimento extends Modelo
{
    protected $stNomeTabela = 'sase.munporcomprometimento';
    protected $arChavePrimaria = array('mppid');
    protected $arAtributos = array(
        'mppid' => null,
        'ravid' => null,
        'mppqtdmun60' => null,
        'mppsitprc60' => null,
        'mppqtdmun6180' => null,
        'mppsitprc6180' => null,
        'mppqtdmun8190' => null,
        'mppsitprc8190' => null,
        'mppqtdmun91' => null,
        'mppsitprc91' => null
    );

    public function montaDados($ravid){
        if(!$ravid) return array();
        global $db;
        $sql = <<<DML
            select
                mppqtdmun60,
                mppsitprc60,
                mppqtdmun6180,
                mppsitprc6180,
                mppqtdmun8190,
                mppsitprc8190,
                mppqtdmun91,
                mppsitprc91
            from sase.munporcomprometimento
            where ravid = {$ravid}
DML;
        $res = $db->carregar($sql)[0];
        $dados = array();
        $dados[] = array(
            'Compromete até 60%',
            $res['mppqtdmun60'],
            $res['mppsitprc60'],
            array(
                'mppqtdmun60',
                'mppsitprc60'
            )
        );
        $dados[] = array(
            'Compromete de 61% a 80%',
            $res['mppqtdmun6180'],
            $res['mppsitprc6180'],
            array(
                'mppqtdmun6180',
                'mppsitprc6180'
            )
        );
        $dados[] = array(
            'Compromete de 81% a 90%',
            $res['mppqtdmun8190'],
            $res['mppsitprc8190'],
            array(
                'mppqtdmun8190',
                'mppsitprc8190'
            )
        );
        $dados[] = array(
            'Compromete acima de 90%',
            $res['mppqtdmun91'],
            $res['mppsitprc91'],
            array(
                'mppqtdmun91',
                'mppsitprc91'
            )
        );
        return $dados;
    }

    public function geraTabelaRelatorio($ravid, $perCampos = false){
        
        global $db;
        $res = $this->montaDados($ravid);
        $tab = new HTML_Table('table_munporsituacao', 'table table-striped table-bordered table-hover table-condensed tabela-listagem');
        $attr = array('class' => 'text-center');
        $tab->addRow();
        $tab->addCell('Situação', '', 'header');
        $tab->addCell('N° de municípios', '', 'header');
        $tab->addCell('Situação PCR Estadual', '', 'header');
        $total = 0;
        foreach ($res as $r) {
            $tab->addRow();
            $tab->addCell($r[0], '', 'data');
            if(!$perCampos) {
                $tab->addCell($r[1], '', 'data');
                $tab->addCell($r[2], '', 'data');
            } else {
                $tab->addCell(retornaCampo($r[1], $r[3][0], false, "", false), '', 'data');
                $tab->addCell(retornaCampoTextArea($r[2], $r[3][1], false, '', 200, 1, 5, false), 'data');
            }
            $total = $total + $r[1];
        }
        $tab->addRow();
        $tab->addCell('Total', '', 'header');
        $tab->addCell($total, '', 'header');
        $tab->addCell('', '', 'header');
        return $tab->display();

    }

    /**
     * Carrega as informações do objeto a partir do id do relatório (ravid)
     *
     * @param $ravid - id do relatório
     */
    public function carregaPorRavid($ravid){
        global $db;
        if(!empty($ravid)) {
            $sql = <<<DML
               select {$this->arChavePrimaria[0]} from {$this->stNomeTabela} where ravid = {$ravid}
DML;
            $res = $db->carregar($sql)[0];
            if (!empty($res)) {
                $this->carregarPorId($res[$this->arChavePrimaria[0]]);
            } else {
                $this->ravid = $ravid;
            }
        }
    }

}