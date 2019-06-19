<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
namespace MecCoder\View;

/**
 * Description of ViewTxtTrait
 *
 * @author calixto
 */
trait ViewTxtTrait
{

    /**
     * Valores definidos para a visualização
     * @var array
     */
    private $viewVars = [];

    /**
     * Método de publicação de uma view
     * @param string $view
     */
    public function showTxt($view)
    {
        foreach ($this->viewVars as $var => $val) {
            $$var = $val;
        }
        include_once APPRAIZ . "{$view}";
    }

    public function getTxt($view)
    {
        $res = file_get_contents(APPRAIZ. "{$view}");
        foreach ($this->viewVars as $var => $val) {
            $res = str_replace('$'.$var,$val,$res);
        }
        return $res;
    }

    /**
     * Armazena o valor para a apresentação na visualização
     * @param string $var nome da variável a ser utilizada da visualização
     * @param mixed $values valor da variável
     * @return $this
     */
    public function toView($var, $values)
    {
        $this->viewVars[$var] = $values;
        return $this;
    }
}
