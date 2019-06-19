<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 16/03/2016
 * Time: 10:57
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Munporhorascontratadas extends Modelo
{
    protected $stNomeTabela = 'sase.munporhorascontratadas';
    protected $arChavePrimaria = array('mphid');
    protected $arAtributos = array(
        'mphid' => null,
        'ravid' => null,
        'mphqtdcompativel' => null,
        'mphpcrcompativel' => null,
        'mphqtdsup10' => null,
        'mphpcrsup10' => null,
        'mphqtdsup1130' => null,
        'mphpcrsup1130' => null,
        'mphqtdsup31' => null,
        'mphpcrsup31' => null
    );

    public function montaDados($ravid){
        if(!$ravid) return array();
        global $db;
        $sql = <<<DML
            select
                mphqtdcompativel,
                mphqtdsup10,
                mphqtdsup1130,
                mphqtdsup31,
                mphpcrcompativel,
                mphpcrsup10,
                mphpcrsup1130,
                mphpcrsup31
            from sase.munporhorascontratadas
            where ravid = {$ravid}
DML;
        $res = $db->carregar($sql)[0];
        $dados = array();
        $dados[] = array(
            'N° de municípios',
            $res['mphqtdcompativel'],
            $res['mphqtdsup10'],
            $res['mphqtdsup1130'],
            $res['mphqtdsup31'],
            array(
                'mphqtdcompativel',
                'mphqtdsup10',
                'mphqtdsup1130',
                'mphqtdsup31'
            )
        );
        $dados[] = array(
            'PCR Estadual',
            $res['mphpcrcompativel'],
            $res['mphpcrsup10'],
            $res['mphpcrsup1130'],
            $res['mphpcrsup31'],
            array(
                'mphpcrcompativel',
                'mphpcrsup10',
                'mphpcrsup1130',
                'mphpcrsup31'
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
        $tab->addCell('Relação de Horas Docentes Contratadas Com a Necessidade de Horas docentes', 'labelEstrutura', 'header', array('width' => '60'));
        $tab->addCell('Compatível', 'colunaEstrutura', 'header');
        $tab->addCell('Superior (de 1% a 10%)', 'colunaEstrutura', 'header');
        $tab->addCell('Superior (de 11% a 30%)', 'colunaEstrutura', 'header');
        $tab->addCell('Superior (maior que 30%)', 'colunaEstrutura', 'header');
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