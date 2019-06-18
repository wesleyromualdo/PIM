<?php

abstract class Par3_Model_Relatorio_Abstract
{
    private $filters;
    private $cabecalho;
    private $cols;
    protected $_where;
    public $optionsColumns;
    public $optionsColumnsAlias;
    public $showGrid;

    public function __construct()
    {
        $this->cabecalho=[];
        $this->_where=[];
        $this->filters=[];
        $this->cols='';
        $this->showGrid = false;
        $this->_buildColumnsHeader();
    }

    abstract protected function _getQuery();

    public function setWhere(array $fromPost)
    {
        foreach ($fromPost as $key => $value) {
            if (!empty($value)) {
                $this->_where[] = "vTable.{$key} = '{$value}'";
            }
        }
    }

    public function buildSelectOptions()
    {
        $html = '<option value=""></option>';
        foreach ($this->optionsColumns as $optKey => $optName) {
            $selected = '';
            if (count($_POST['cols'])) {
                foreach ($_POST['cols'] as $i => $chaveColuna) {
                    if ($chaveColuna == $optKey) {
                        $selected = 'selected=\'selected\'';
                        break;
                    }
                }
            }
            $html .= "<option {$selected} value=\"{$optKey}\">{$optName}</option>";
        }
        return $html;
    }

    public function buildQuery()
    {
        $strSQL = $this->_getQuery();
        if (count($this->_where)) {
            $strSQL = sprintf($strSQL, $this->getCols(), ' WHERE ' . implode(' AND ', $this->_where));
        } else {
            $strSQL = sprintf($strSQL, $this->getCols(), '');
        }
        return str_replace('\\', '', $strSQL);
    }

    public function getHeader(){
        if (!count($this->cabecalho)) {
            throw new Exception('O cabeçaho precisa ser definido, e não pode estar vazio.');
        }
        return $this->cabecalho;
    }

    public function getCols()
    {
        if (!strlen($this->cols)) {
            throw new Exception('A lista de colunas não pode estar vazia.');
        }
//        ver($this->cols, d);
        return $this->cols;
    }

    public function getFilters()
    {
        if (!count($this->filters)) {
            throw new Exception('A lista de filtros não pode estar vazia.');
        }
        return $this->filters;
    }

    public function getWhere()
    {
        if (count($this->_where)) {
            return $this->_where;
        }
    }

    protected function _populateColumns($index)
    {

        if (array_key_exists($index, $this->optionsColumns)) {
            array_push($this->cabecalho, $this->optionsColumns[$index]);
            array_push($this->filters, $index);
        }
    }

    protected function _assingAliasColumn($column)
    {
        if (count($this->optionsColumnsAlias) && array_key_exists($column, $this->optionsColumnsAlias)) {
            return "$column AS {$this->optionsColumnsAlias[$column]},\n";
        } else {
            return "$column,\n";
        }
    }

    protected function _buildColumnsHeader(){
        if (!count($this->optionsColumns)) {
            throw new Exception('Voce precisa definir as colunas do relatorio.');
        }

        if (count($_POST['cols']) > 0) {
            foreach ($_POST['cols'] as $k => $columns) {
                $this->cols .= $this->_assingAliasColumn($columns);
                $this->_populateColumns($columns);
            }
            $this->showGrid = true;
        } else {
            foreach ($this->optionsColumns as $keyColumn => $labelColumn) {
                $this->cols .= $this->_assingAliasColumn($keyColumn);
                $this->_populateColumns($keyColumn);
            }
        }
        $this->cols = substr($this->cols, 0, -2);
    }
}