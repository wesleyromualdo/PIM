<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace Simec;

/**
 * Description of Controle
 *
 * @author calixto
 */
abstract class AbstractControle extends \MecCoder\AbstractController
{

    /**
     * Método de publicação de uma view
     * @global cls_banco $db
     * @param string $view
     */
    protected function showHtml($view = null)
    {
        $path = $this->viewPath($view);
        
        if ($this->isAjax() == false) {
            $this->js('../mec-coders.php?file=src/Simec/Visao/publico/main.js');
            $this->css('../mec-coders.php?file=src/Simec/Visao/publico/main.css');
        }
        $this->js(sprintf('../mec-coders.php?file=%s.view.js', $path));
        $this->css(sprintf('../mec-coders.php?file=%s.view.css', $path));
        
        return parent::showHtml($path);
    }
    
    /**
     * Método de contrução de uma partial
     * @global cls_banco $db
     * @param string $view
     */
    protected function showPartial($view = null)
    {
        $path = $this->viewPath($view);
        $this->js(sprintf('../mec-coders.php?file=%s.view.js', $path));
        $this->css(sprintf('../mec-coders.php?file=%s.view.css', $path));
        
        return parent::showPartial($view);            
    }
    /**
     * Método para obtenção de uma partial
     * @global cls_banco $db
     * @param string $view
     */
    protected function getPartial($view = null)
    {
        $path = $this->viewPath($view);
        $this->js(sprintf('../mec-coders.php?file=%s.view.js', $path));
        $this->css(sprintf('../mec-coders.php?file=%s.view.css', $path));
        
        ob_start();
        parent::showPartial($view);
        $partial = ob_get_contents();
        ob_get_clean();
        
        return $partial;
    }
    
    /**
     * Método para nomeação padrão de views
     * @param string $view caminho e nome para a view
     * @return string caminho e nome para view
     */
    private function viewPath($view = null)
    {
        $name = (new \ReflectionClass($this))->getName();
        return ($view) ? ($view) : sprintf('%s/%s', \Simec\Simec::pegarCaminhoVisao($name), debug_backtrace()[2]['function']);
    }

    /**
     * Método para importação do SIMEC
     */
    protected function importSimec()
    {
        return new \Simec_View_Helper();
    }
    
    /**
     * Retorna um boleano informando se a requisição é ajax
     * @return boolean
     */
    protected function isAjax()
    {
        return parent::isAjax();
    }
    
    
    /**
     * Método para construção de uma partial
     * @param string $view
     */
/*
        protected function renderPartial($view = null)
        {
            $path = $this->viewPath($view);
            ob_start();
            $this->showPartial($path);
            $partial = ob_get_clean();
            if (Request::isAjax()) echo $partial;
            else return $partial;
        }
// */
        /**
         * Método para nomeação padrão de views
         */
/*

        private function viewPath($view = null)
        {
            $name = (new \ReflectionClass($this))->getName();
            return ($view) ? ($view) : sprintf('%s/%s', \Simec\Simec::pegarCaminhoVisao($name), debug_backtrace()[2]['function']);
        }
// */
}
