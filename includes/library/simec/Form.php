<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 16/11/2016
 * Time: 17:34
 */

/**
 * Renderizador para o layout Espinia
 * @see Simec_Renderer_Form_Espinia
 */
require_once dirname(__FILE__) . '/form/renderer/Espinia.php';

class simec_Form
{

    /**
     * Methods
     */
    const POST = 'POST';
    const GET = 'GET';

    /**
     *  Renderers
     */
    const ESPINIA = 'Simec_Renderer_Form_Espinia';

    /**
     * @var $renderer - Render utilizado pela classe e instanciado no construct
     */
    protected $renderer;

    public function __construct($id = null, $titulo = '', $renderer = self::ESPINIA, $method = self::POST, $acao = 'salvar', $action = '', $opcoes = [])
    {
        $this->renderer = new $renderer($id, $titulo, $method, $acao, $action, $opcoes);
    }

    /**
     * Faz com que o formulário retorne somente os campos, sem o "FORM"
     *
     * @param bool $formOff
     * @return $this
     */
    public function setFormOff($formOff = true)
    {
        $this->renderer->setFormOff($formOff);
        return $this;
    }

    /**
     * Executa uma função existente, apenas, no renderer.
     *
     * @param $name - Nome da função
     * @param array $args - Parametros da função
     * @return $this
     */
    public function __call($name, $args = [])
    {
        if (method_exists($this->renderer, $name)) {
            if (!empty($args)) {
                call_user_func_array([$this->renderer, $name], $args);
                return $this;
            }
            $this->renderer->$name();
            return $this;
        } else {
            throw new Exception("Método '{$name}' não encontrado");
        }
    }

    public function addInput($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addInput($name, $label, $value, $attribs);
        return $this;
    }

    public function addCep($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addCep($name, $label, $value, $attribs);
        return $this;
    }

    public function addCpf($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addCpf($name, $label, $value, $attribs);
        return $this;
    }

    public function addCnpj($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addCnpj($name, $label, $value, $attribs);
        return $this;
    }

    public function addEmail($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addEmail($name, $label, $value, $attribs);
        return $this;
    }

    public function addTelefone($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addTelefone($name, $label, $value, $attribs);
        return $this;
    }

    public function addData($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addData($name, $label, $value, $attribs);
        return $this;
    }

    public function addPeriodo($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addPeriodo($name, $label, $value, $attribs);
        return $this;
    }

    public function addSelect($name, $label = null, $value = null, $attribs = [], $opcoes = [])
    {
        $this->renderer->addSelect($name, $label, $value, $attribs, $opcoes);
        return $this;
    }

    public function addSelectOutros($name, $label = null, $value = null, $opcoes = [], $attribs = [], $attribsOutros = [])
    {
        $this->renderer->addSelectOutros($name, $label, $value, $opcoes, $attribs, $attribsOutros);
        return $this;
    }

    public function addTextarea($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addTextarea($name, $label, $value, $attribs);
        return $this;
    }

    public function addRadio($name, $label = null, $value = null, $attribs = [], $opcoes = [])
    {
        $this->renderer->addRadio($name, $label, $value, $attribs, $opcoes);
        return $this;
    }

    public function addRadioOutros($name, $label = null, $value = null, $opcoes = [], $attribs = [], $attribsOutros = [])
    {
        $this->renderer->addRadioOutros($name, $label, $value, $opcoes, $attribs, $attribsOutros);
        return $this;
    }

    public function addCheckbox($name, $label = null, $value = null, $attribs = [])
    {
        $this->renderer->addCheckbox($name, $label, $value, $attribs);
        return $this;
    }

    public function addCampoPersonalizado($name, $html)
    {
        $this->renderer->addCampoPersonalizado($name, $html);
        return $this;
    }

    public function addFile($name, $titulo, $accept = null, $attribs = [])
    {
        $this->renderer->addFile($name, $titulo, $accept, $attribs);
        return $this;
    }

    public function addHidden($name, $value = null, $attribs = [])
    {
        $this->renderer->addHidden($name, $value, $attribs);
        return $this;
    }

    public function setDados($dados = [])
    {
        $this->renderer->setDados($dados);
        return $this;
    }

    public function setId($ident)
    {
        $this->id = $ident;
        return $this;
    }

    public function setTitulo($titulo)
    {
        $this->renderer->setTitulo($titulo);
        return $this;
    }

    public function setAction($action)
    {
        $this->renderer->setAction($action);
        return $this;
    }

    public function setOpcoes(array $opcoes = [])
    {
        $this->renderer->setOpcoes($opcoes);
        return $this;
    }

    public function setMethod(array $method = [])
    {
        $this->renderer->setMethod($method);
        return $this;
    }

    public function render()
    {
        $this->renderer->render();
        $this->renderer->renderScript();
        return $this;
    }

    public function addScript($param)
    {
        $this->renderer->addScript($param);
        return $this;
    }
}
