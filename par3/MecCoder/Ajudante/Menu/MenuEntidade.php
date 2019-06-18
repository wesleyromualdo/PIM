<?php
namespace Simec\Par3\Ajudante\Menu;

class MenuEntidade extends \Simec\AbstractAjudante
{
    public function menu()
    {
        ob_start();
        include APPRAIZ . 'par3/modulos/principal/planoTrabalho/cabecalhoUnidade.inc';
        $partial = ob_get_clean();
        
        return $partial;
    }
}