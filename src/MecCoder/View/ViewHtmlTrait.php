<?php
namespace MecCoder\View;

use MecCoder\Request;

trait ViewHtmlTrait
{

    /**
     * Valores definidos para a visualização
     * @var array
     */
    private $viewVars = [];

    /**
     * Valores configurados para o controle ajustar a visualização
     * @var array
     */
    private $viewConfigs = [
        '  js' => [],
        '  filesCss' => [],
        '  filesJs' => [],
    ];

    /**
     * Método de publicação de uma view
     * @global cls_banco $db
     * @param string $view
     */
    public function showHtml($view)
    {
        global $db;
        if (!Request::isAjax()) require APPRAIZ . 'includes/cabecalho.inc';
        foreach ($this->viewVars as $var => $val) {
            $$var = $val;
        }
        
        include_once APPRAIZ . "{$view}.view.phtml";
        
        foreach ($this->viewConfigs as $var => $val) {
            $$var = $val;
        }
        include_once 'public/register.phtml';
        if (!Request::isAjax()) {
            include APPRAIZ . "includes/rodape.inc";
        }else{
            die;
        }
    }
    
    /**
     * Método de publicação de uma partial
     * @param string $view
     */
    public function showPartial($view)
    {
        global $db;
        
        foreach ($this->viewVars as $var => $val) {
            $$var = $val;
        }
        
        include APPRAIZ . "{$view}.view.phtml";
        
        foreach ($this->viewConfigs as $var => $val) {
            $$var = $val;
        }
        
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

    /**
     * Armazena o valor para registrar no script da vizualização
     * @param string $var nome da variável a ser utilizada no namespace "_JS" do script
     * (Exemplo: a variável "bla" será acessível no script como "_JS.bla")
     * @param mixed $values valor da variável
     * @return $this
     */
    public function toJs($var, $values)
    {
        $this->viewConfigs['  js'][$var] = $values;
        return $this;
    }

    /**
     * Armazena a url para registrar na visualização
     * @param string $urlPath
     * @return $this
     */
    public function css($urlPath)
    {
        if (!in_array($urlPath, $this->viewConfigs['  filesCss'])) {
            $this->viewConfigs['  filesCss'][] = $urlPath;
        }
        
        return $this;
    }

    /**
     * Armazena a url para registrar na visualização
     * @param type $urlPath
     * @return $this
     */
    public function js($urlPath)
    {
        if (!in_array($urlPath, $this->viewConfigs['  filesJs'])) {
            $this->viewConfigs['  filesJs'][] = $urlPath;
        }
        
        return $this;
    }
}
