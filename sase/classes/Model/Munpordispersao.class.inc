<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 15/03/2016
 * Time: 15:26
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Munpordispersao extends Modelo
{
    protected $stNomeTabela = 'sase.munpordispersao';
    protected $arChavePrimaria = array('mpdid');
    protected $arAtributos = array(
        'mpdid' => null,
        'ravid' => null,
        'mpdqtd50' => null,
        'mpdprc50' => null,
        'mpdqtd5180' => null,
        'mpdprc5180' => null,
        'mpdqtd81120' => null,
        'mpdprc81120' => null,
        'mpdqtd121' => null,
        'mpdprc121' => null
    );

    public function montaDados($ravid){
        if(!$ravid) return array();
        global $db;
        $sql = <<<DML
            select
                mpdqtd50,
                mpdprc50,
                mpdqtd5180,
                mpdprc5180,
                mpdqtd81120,
                mpdprc81120,
                mpdqtd121,
                mpdprc121
            from sase.munpordispersao
            where ravid = {$ravid}
DML;
        $res = $db->carregar($sql)[0];
        $dados = array();
        $dados[] = array(
            'N° de municípios',
            $res['mpdqtd50'],
            $res['mpdqtd5180'],
            $res['mpdqtd81120'],
            $res['mpdqtd121'],
            array(
                'mpdqtd50',
                'mpdqtd5180',
                'mpdqtd81120',
                'mpdqtd121'
            )
        );

        $dados[] = array(
            'PRC Estadual',
            $res['mpdprc50'],
            $res['mpdprc5180'],
            $res['mpdprc81120'],
            $res['mpdprc121'],
            array(
                'mpdprc50',
                'mpdprc5180',
                'mpdprc81120',
                'mpdprc121'
            )
        );
        return $dados;
    }

    public function getTabelaRelatorio($ravid, $perCampos = false){
        global $db;
        $res = $this->montaDados($ravid);
        $tab = new HTML_Table('table_munporsituacao', 'table table-striped table-bordered table-hover table-condensed tabela-listagem');
        $attr = array('class' => 'text-center');
        $tab->addRow();
        $tab->addCell('Estrutura', 'tituloEstrutura', 'header', array('colspan' => '5'));
        $tab->addRow();
        $tab->addCell('Dispersão Total', 'labelEstrutura', 'header', array('width' => '60'));
        $tab->addCell('Até 50%', 'colunaEstrutura', 'header');
        $tab->addCell('51% a 80%', 'colunaEstrutura', 'header');
        $tab->addCell('81% a 120%', 'colunaEstrutura', 'header');
        $tab->addCell('Acima de 121%', 'colunaEstrutura', 'header');
        $v = 1;
        foreach ($res as $r) {
            $fun = $v == 1 ? "retornaCampo" : "retornaCampoPrcEstadual";
            $tab->addRow();
            $tab->addCell($r[0], '', 'data');
            if(!$perCampos) {
                $tab->addCell($r[1], '', 'data');
                $tab->addCell($r[2], '', 'data');
                $tab->addCell($r[3], '', 'data');
                $tab->addCell($r[4], '', 'data');
            } else {
                $tab->addCell($fun($r[1], $r[5][0], false, "", false), '', 'data');
                $tab->addCell($fun($r[2], $r[5][1], false, "", false), '', 'data');
                $tab->addCell($fun($r[3], $r[5][2], false, "", false), '', 'data');
                $tab->addCell($fun($r[4], $r[5][3], false, "", false), '', 'data');
            }
            $v++;
        }
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