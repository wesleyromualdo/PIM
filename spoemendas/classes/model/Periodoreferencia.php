<?php
/**
 *
 * $Id: Periodoreferencia.php 98660 2015-06-16 13:52:40Z maykelbraz $
 */

/**
 *
 */
class SpoEmendas_Model_Periodoreferencia extends SpoEmendas_Model_Periodoreferencia_Abstract
{
    protected $stNomeTabela = 'spoemendas.periodoreferencia';

    protected function init()
    {
    }

    public function queryTodosPeriodosCombo()
    {
        return <<<DML
SELECT t1.prfid AS codigo,
       '{$_SESSION['exercicio']}' || ' - ' || t1.prftitulo AS descricao
  FROM {$this->stNomeTabela} t1
DML;
    }

}
