<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 17/11/2016
 * Time: 09:26
 */

require_once(dirname(__FILE__) . '/Form_Interface.php');

abstract class Simec_Form_Abstract implements Simec_Form_Interface
{
    const POST = 'POST';
    const GET = 'GET';


    protected $dados;
    protected $id;
    protected $titulo;
    protected $action;
    protected $opcoes;
    protected $method;
    protected $campos = [];
    protected $formOff = false;
    protected $acao;
    protected $script = [];

    public function __construct($id = null, $titulo = '', $method = self::POST, $acao = 'salvar', $action = '', $opcoes = [])
    {
        $this->id = !empty($id) ? $id : 'formulario';
        $this->titulo = $titulo;
        $this->method = $method;
        $this->setAcao($acao);
        if (!empty($action)) {
            $this->action = $action;
        }
        if (!empty($opcoes)) {
            $this->opcoes = $opcoes;
        }
    }

    /**
     * Retira os campos de dentro de um formulário.
     *
     * @param bool $formOff
     * @return $this
     */
    public function setFormOff($formOff = true)
    {
        $this->formOff = $formOff;
        return $this;
    }

    /**
     * Seta o valor do campo hidden "acao", existente no formulário.
     *
     * @param string $acao
     */
    public function setAcao($acao = '')
    {
        $this->acao = $acao;
        return $this;
    }

    /**
     * Gera um ID para um campo, com base no id do formulário e no nome do campo
     *
     * @param $nome
     * @return string
     */
    protected function getCampoId($nome)
    {
        return "{$this->id}_{$nome}";
    }

    /**
     * Adiciona um campo personalizado no formulário. A função espera o HTML comopleto do campo, com form-group e tudo.
     *
     * @param $name
     * @param $html
     * @return $this
     */
    public function addCampoPersonalizado($name, $html)
    {
        $this->campos[] = [
        'name' => $name,
        'html' => $html
        ];
        return $this;
    }

    /**
     * Carrega a os dados da requisição, para usa-los nos campos do formulário
     *
     * @param array $dados
     */
    public function setDados($dados = [])
    {
        $this->dados = $dados;
        return $this;
    }

    public function setId($ident)
    {
        $this->id = $ident;
        return $this;
    }

    public function setTitulo($titulo)
    {
        $this->titulo = $titulo;
        return $this;
    }

    public function setAction($action)
    {
        $this->action = $action;
        return $this;
    }

    public function setOpcoes(array $opcoes = [])
    {
        $this->opcoes = $opcoes;
        return $this;
    }

    public function setMethod(array $method = [])
    {
        $this->method = $method;
        return $this;
    }

    public function addScript($param)
    {
        $this->script[] = $param;
        return $this;
    }

    public function renderScript()
    {
        $cod = "\n<script>";

        foreach ($this->script as $valor) {
            $cod .= <<<HTML
			\n {$valor} \n
HTML;
        }

        return $cod .= '</script>';
    }

    /**
     * Retorna os atributos, passados pelo array "attribs", em uma string no formato de atributos html.
     * Exemplo:
     *      Para: $attribs = ['value' => '30']           retorna: value="30"
     *      Para: $attribs = ['style' => 'width: 100%']  retorna: style="width: 100%"
     *
     * @param $attribs
     * @return string
     * @throws Exception
     */
    public function trataAtributos($attribs){
        if(is_array($attribs)){
            $atributos = "";
            foreach ($attribs as $key => $val) {
                $atributos .= " {$key}=\"{$val}\" ";
            }
            return $atributos;
        } else {
            throw new Exception("Parâmetro 'attribs' deve ser um array");
        }
    }
}
