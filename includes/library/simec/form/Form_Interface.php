<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 16/11/2016
 * Time: 17:53
 */
interface Simec_Form_Interface {

    /**
     * Adiciona um input ao formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addInput($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de CEP no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addCep($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de CPF no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addCpf($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de CNPJ no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addCnpj($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de email no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addEmail($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de telefone no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addTelefone($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de data no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addData($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo de período no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addPeriodo($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona uma combobox no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     * @param array $attribs
     */
    public function addSelect($name, $label = null, $value = null, $opcoes = [], $attribs = []);

    /**
     * Adiciona um campo textarea no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     */
    public function addTextarea($name, $label = null, $value = null, $opcoes = []);

    /**
     * Adiciona um campo radio no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     * @param array $attribs
     */
    public function addRadio($name, $label = null, $value = null, $opcoes = [], $attribs = []);

    /**
     * Adiciona um campo checkbox no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     * @param array $attribs
     */
    public function addCheckbox($name, $label = null, $value = null, $opcoes = [], $attribs = []);

    /**
     * Adiciona um campo file no formulário
     *
     * @param $name
     * @param $titulo
     * @param $accept
     * @param array $attribs
     */
    public function addFile($name, $titulo, $accept = null, $attribs = []);

    /**
     * Adiciona um campo hidden no formulário
     *
     * @param $name
     * @param null $value
     * @param array $attribs
     */
    public function addHidden($name, $value = null, $attribs = []);

    /**
     * Renderiza o formulário
     */
    public function render();
}