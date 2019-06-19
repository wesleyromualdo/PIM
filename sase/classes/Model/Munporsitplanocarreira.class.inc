<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 11/03/2016
 * Time: 15:28
 */
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Munporsitplanocarreira extends Modelo
{
    protected $stNomeTabela = 'sase.munporsitplanocarreira';
    protected $arChavePrimaria = array('mpcid');
    protected $arAtributos = array(
        'mpcid' => null,
        'ravid' => null,
        'qtdmuncomplano' => null,
        'sitprccomplano' => null,
        'qtdmunpagapiso' => null,
        'sitprcpagapiso' => null,
        'qtdmuncumprejornada' => null,
        'sitprccumprejornada' => null,
        'qtdmunobsmeta18' => null,
        'sitprcobsmeta18' => null
    );

    public function montaDados($ravid){
        if(!$ravid) return array();
        global $db;
        $sql = <<<DML
            select
                qtdmuncomplano,
                qtdmunpagapiso,
                qtdmuncumprejornada,
                qtdmunobsmeta18,
                sitprccomplano,
                sitprcpagapiso,
                sitprccumprejornada,
                sitprcobsmeta18
            from sase.munporsitplanocarreira
            where ravid = {$ravid}
DML;
        $res = $db->carregar($sql)[0];
        $dados = array();
        $dados[] = array(
            'Com Plano de Carreira',
            $res['qtdmuncomplano'],
            $res['sitprccomplano'],
            array(
                'qtdmuncomplano',
                'sitprccomplano'
            )
        );
        $dados[] = array(
            'Paga o Piso (Lei 11.738/2008)',
            $res['qtdmunpagapiso'],
            $res['sitprcpagapiso'],
            array(
                'qtdmunpagapiso',
                'sitprcpagapiso'
            )
        );
        $dados[] = array(
            'Cumpre Jornada (Lei 11.738/2008)',
            $res['qtdmuncumprejornada'],
            $res['sitprccumprejornada'],
            array(
                'qtdmuncumprejornada',
                'sitprccumprejornada'
            )
        );
        $dados[] = array(
            'Observações quanto à meta 18',
            $res['qtdmunobsmeta18'],
            $res['sitprcobsmeta18'],
            array(
                'qtdmunobsmeta18',
                'sitprcobsmeta18'
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