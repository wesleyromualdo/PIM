<?php
namespace Simec;

use MecCoder\Request;

abstract class AbstractAjudante extends \MecCoder\AbstractController
{

    public function finish()
    {
    }

    public function init()
    {
    }

    public function index()
    {
    }

    private function helperPath($view = null)
    {
        $name = (new \ReflectionClass($this))->getName();
        return ($view) ? ($view) : sprintf('%s/%s', \Simec\Simec::pegarCaminhoAjudante($name), debug_backtrace()[2]['function']);
    }


    /**
     * Método para construção de um helper
     * @param string $view caminho e nome para a view
     * @return string html gerado pelo helper
     */
    protected function renderHelper($view = null)
    {
        $path = $this->helperPath($view);
        ob_start();
        $this->showPartial($path);
        $partial = ob_get_clean();
        if (Request::isAjax()) echo $partial;
        else return $partial;
    }

}
