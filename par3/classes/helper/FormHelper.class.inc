<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/18/2017
 * Time: 10:32 AM
 */

/**
 * Class Par3_Helper_FormHelper
 */
class Par3_Helper_FormHelper{

    private $nameForm; // nome do arquivo que será carregado do par3/classes/forms
    private $isModal; //caso o formulário esteja em um modal, o nome dele será NOME+_modal
    private $method;
    private $class;
    private $metaData;
    private $style;
    private $complementar;
    private $arquivo;

    /**
     * @return mixed
     */
    public function getNameForm()
    {
        return $this->nameForm;
    }

    /**
     * @param mixed $nameForm
     * @return Par3_Helper_FormHelper
     * @description nome do arquivo sem extensão que será renderizado da pasta par3/classes/form
     */
    public function setNameForm($nameForm=null)
    {
        $this->nameForm = $nameForm;
        if($nameForm == null || $nameForm == ''){
            if($this->getIsModal() == null || $this->getIsModal() == ''){
                if($this->getNameForm() == null || $this->getNameForm() == ''){
                    $this->nameForm = 'form';
                }else{
                    $this->nameForm = $nameForm;
                }
            }else{
                if($this->getNameForm() == null || $this->getNameForm() == ''){
                    $this->nameForm = 'form_modal';
                }else{
                    $this->nameForm = $nameForm."_modal";
                }
            }
        }
        return $this;
    }

    /**
     * @return mixed
     */
    public function getIsModal()
    {
        return $this->isModal;
    }

    /**
     * @param mixed $isModal
     * @return Par3_Helper_FormHelper
     */
    public function setIsModal($isModal = null)
    {
        $this->isModal = $isModal;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getMethod()
    {
        return $this->method;
    }

    /**
     * @param mixed $method
     * @return Par3_Helper_FormHelper
     */
    public function setMethod($method)
    {
        $this->method = $method;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getClass()
    {
        return $this->class;
    }

    /**
     * @param mixed $class
     * @return Par3_Helper_FormHelper
     */
    public function setClass($class)
    {
        $this->class = $class;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getMetaData()
    {
        return $this->metaData;
    }

    /**
     * @param mixed $metaData
     * @return Par3_Helper_FormHelper
     */
    public function setMetaData($metaData)
    {
        $this->metaData = $metaData;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getStyle()
    {
        return $this->style;
    }

    /**
     * @param mixed $style
     * @return Par3_Helper_FormHelper
     */
    public function setStyle($style)
    {
        $this->style = $style;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getComplementar()
    {
        return $this->complementar;
    }

    /**
     * @param mixed $complementar
     * @return Par3_Helper_FormHelper
     */
    public function setComplementar($complementar)
    {
        $this->complementar = $complementar;
        return $this;
    }

    /**
     * @return mixed
     * @description nome Arquivo que será carregado de par3/classes/forms/ sem a extensão .inc
     *
     */
    public function getArquivo()
    {
        return $this->arquivo;
    }

    /**
     * @param mixed $arquivo
     * @return Par3_Helper_FormHelper
     */
    public function setArquivo($arquivo)
    {
        $this->arquivo = $arquivo;
        return $this;
    }




    /**
     * @param $nameForm
     * @throws Exception
     * @description Rederiza o formulário
     */
    public function renderForm(){
        $simec = new Simec_View_Helper();
        if($this->getArquivo() != ''){
            $arquivo = "..".DIRECTORY_SEPARATOR."..".DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR."classes".DIRECTORY_SEPARATOR."forms".DIRECTORY_SEPARATOR."{$this->getArquivo()}".".inc";
            if(file_exists($arquivo)){
                $this->openForm();
                include($arquivo);
                $this->closeForm();
            }else{
               throw new Exception("Arquivo {$this->getArquivo()} Não Encontrado ");
            }
        }
    }

    /**
     * @throws Exception
     */
    public function renderFormModal(){
        $simec = new Simec_View_Helper();
        if($this->getArquivo() != ''){
            $arquivo = "..".DIRECTORY_SEPARATOR."..".DIRECTORY_SEPARATOR.'par3'.DIRECTORY_SEPARATOR."classes".DIRECTORY_SEPARATOR."forms".DIRECTORY_SEPARATOR."{$this->getArquivo()}".".inc";
            if(file_exists($arquivo)){
                $isModal = true;
                include($arquivo);
            }else{
                throw new Exception("Arquivo {$this->getArquivo()} Não Encontrado ");
            }
        }
    }


    public function openForm(){
        echo "<form id='{$this->getNameForm()}_id' name='{$this->getNameForm()}' method='{$this->getMethod()}' >";
    }

    public function closeForm(){
        echo "</form>";
    }

}