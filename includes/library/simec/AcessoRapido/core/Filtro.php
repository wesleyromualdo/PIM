<?php
namespace AcessoRapido\core;

abstract class Filtro
{
    protected $pathVisao;
    
    final public function __construct()
    {
        $rc = new \ReflectionClass(get_class($this));
        $this->pathVisao = str_replace("controle", "visao", dirname($rc->getFileName()));        
    }
    
    abstract public function montarTela();
    
    abstract public function aplicarFiltro();
    
    final protected function incluirVisao($arquivo, Array $dados)
    {
        global $db;
        
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            $simec = new \Simec_View_Helper();
        }
        
        require_once $this->pathVisao . "/{$arquivo}";
    }
}