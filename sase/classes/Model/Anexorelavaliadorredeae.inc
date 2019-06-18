<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 01/02/2016
 * Time: 09:38
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

class Sase_Model_Anexorelavaliadorredeae extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.anexorelavaliadorredeae';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'araid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'ravid' => array('tabela' => 'sase.relatorioavaliadorredeae', 'pk' => 'ravid'),
        'arqid' => array('tabela' => 'public.arquivo', 'pk' => 'arqid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'araid' => null,
        'arqid' => null,
        'ravid' => null,
        'usucpf' => null,
        'aradsc' => null,
        'aradtinclusao' => null,
        'arastatus' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'araid' => array( 'allowEmpty' => false, 'Digits'),
            'arqid' => array( 'allowEmpty' => false, 'Digits'),
            'ravid' => array( 'allowEmpty' => false, 'Digits'),
            'usucpf' => array( 'allowEmpty' => false ),
            'aradsc' => array( 'allowEmpty' => false ),
            'aradtinclusao' => array( 'allowEmpty' => true ),
            'arastatus' => array( 'allowEmpty' => false )
        );
    }

    protected function montaSqlRelatorio($ravid)
    {
        $sql = <<<DML
            select
                ara.araid,
                ara.ravid,
                ara.arqid,
                ara.aradsc,
                arq.arqnome
            from sase.anexorelavaliadorredeae ara
            inner join public.arquivo arq on ara.arqid = arq.arqid
            where arastatus = 'A'
            and ravid = {$ravid}
            order by aradtinclusao
DML;
        return $sql;
    }

    /**
     * @param $dados - Array com o restulado do SQL.
     * @param bool $allowCampo - Informa se a edição da descrição do arquivo estará disponível.
     * @return string
     */
    protected function montaTabelaRelatorio($dados, $allowCampo = false){
        $tabelaHtml = new HTML_Table('table_prazo_pne', 'table table-striped table-bordered table-hover table-condensed tabela-listagem');
        $camposDaTabela = array(
            'arqnome' => 'Arquivo',
            'aradsc' => 'Descrição'
        );
        $attr = array('class' => 'text-center');
        $txtArea = <<<HTML
            <textarea name="aradsc[]" id="aradsc%s" class="form-control" cols="50" rows="2" maxlength="50">%s</textarea>
HTML;
        $btnEdit = <<<HTML
            <button class="btn btn-primary" type="button" title="Editar" onclick="editaArquivo(%s)" id="btnPesquisar">
                <i class="fa fa-pencil-square-o"></i>
            </button>
HTML;

        $btnDel = <<<HTML
            <button class="btn btn-danger" type="button" title="Excluir" onclick="apagaArquivo(%s, %s)" id="btnPesquisar">
                <i class="fa fa-times"></i>
            </button>
HTML;

        $btnDow = <<<HTML
            <button class="btn btn-siccess" type="button" title="Download" onclick="downloadArquivo(%s)" id="btnPesquisar">
                <i class="fa fa-download"></i>
            </button>
HTML;

        $acoes = $btnEdit . $btnDel;

        $tabelaHtml->addTSection('thead');
        $tabelaHtml->addRow();
        if($allowCampo) {
            $tabelaHtml->addCell('', '', 'header', $attr);
        }
        foreach ($camposDaTabela as $campo => $titulo) {
            $tabelaHtml->addCell($titulo, '', 'header');
        }

        if(is_array($dados)) {
            foreach ($dados as $dado) {

                $tabelaHtml->addTSection('tbody');
                $tabelaHtml->addRow();
                if($allowCampo) {
                    $tabelaHtml->addCell(sprintf($acoes . $btnDow, $dado['araid'], $dado['araid'], $dado['arqid'], $dado['arqid']), '', 'data', $attr);
                } else {
                    $temp = sprintf($btnDow, $dado['arqid']);                    
                    $tabelaHtml->addCell($temp, '', 'data', $attr);
                }
                foreach ($camposDaTabela as $campo => $titulo) {
//                    if($allowCampo && $campo == 'aradsc'){
//                        $tabelaHtml->addCell(sprintf($txtArea, $dado['araid'], $dado[$campo]), '', 'data', $attr);
//                    } else {
                        $tabelaHtml->addCell($dado[$campo], '', 'data', $attr);
//                    }
                }
            }
        } else {
            $tabelaHtml->addTSection('tbody');
            $tabelaHtml->addRow();
            $tabelaHtml->addCell('Sem registros', '', 'data', array('class' => 'text-center', 'colspan' => count($camposDaTabela)+1));
        }

        return $tabelaHtml->display();
    }

    public function getTabelaRelatorio($ravid, $perCampos = true){
        if (is_numeric($ravid)){
            global $db;
            $sql = $this->montaSqlRelatorio($ravid);
            $dados = $db->carregar($sql);
//            ver($sql, $dados, d);
            return $this->montaTabelaRelatorio($dados, $perCampos);
        }else{
            return false;
        }
    }
}