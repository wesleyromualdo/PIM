<?php
/**
 * Created by PhpStorm.
 * User: fiuza
 * Date: 29/08/18
 * Time: 16:04
 */

namespace visao\Helper\ToolBoxIcon;

require_once APPRAIZ . 'par3/modulos/visao/Helper/ToolBoxIcon/ToolBoxIcon.class.inc';

class CheckBoxFilterToolBox extends ToolBoxIcon
{
    protected $id;
    protected $name                 = '';

    /**
     * @return mixed
     */
    public function getId()
    {
        return $this->id;
    }
    protected $params               = array();
    protected $values               = array();
    protected $event                = 'onclick';
    protected $request              = '';
    protected $method               = 'GET';
    protected $url                  = '';
    protected $idGrafico            = '';
    protected $title                = '';
    protected $agrupar              = false;
    protected $aditionalSubmitNames = array();
    public function iniciarFluxo()
    {

    }

    /**
     * id do checkbox
     * @param null $id
     * @return $this
     */
    public function setId($id = null)
    {
        if($id) {
            $this->id = $id;
        }
        return $this;
    }

    /**
     * Name do checkbox
     * @param null $name
     * @return $this
     */
    public function setName($name = null)
    {
        $this->name = $name;
        return $this;
    }

    /**
     * Parâmetros para construção do checkbox
     * @param $params
     * @return $this
     */
    public function setParams($params)
    {
        $this->params = $params;
        return $this;
    }

    public function setValues($values)
    {
        $this->values = $values;
        return $this;
    }

    public function setEvent($event)
    {
        $this->event = $event;
        return $this;
    }

    /**
     * @param $method
     * @return $this
     */
    public function setMethod($method)
    {
        $this->method = $method;
        return $this;
    }

    /**
     * url da requisição
     * @param $url
     * @return $this
     */
    public function setUrl($url)
    {
        $this->url = $url;
        return $this;
    }

    /**
     * id da div que será renderizada
     * @param $idGrafico
     * @return $this
     */
    public function setIdDivAtualizar($idGrafico)
    {
        $this->idGrafico = $idGrafico;
        return $this;
    }

    public function setTitle($title)
    {
        $this->title = $title;
        return $this;
    }

    public function agrupar($agrupar = false)
    {
        $this->agrupar = $agrupar;
        return $this;
    }

    public function setAditionalSubmitNames($aditionalNames)
    {
        $this->aditionalSubmitNames = $aditionalNames;
        return $this;
    }

    /**
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * @return array
     */
    public function getParams()
    {
        return $this->params;
    }

    /**
     * @return array
     */
    public function getValues()
    {
        return $this->values;
    }

    /**
     * @return string
     */
    public function getEvent()
    {
        return $this->event;
    }

    /**
     * @return string
     */
    public function getRequest()
    {
        return $this->request;
    }

    /**
     * @return string
     */
    public function getMethod()
    {
        return $this->method;
    }

    /**
     * @return string
     */
    public function getUrl()
    {
        return $this->url;
    }

    /**
     * @return string
     */
    public function getIdGrafico()
    {
        return $this->idGrafico;
    }

    /**
     * @return string
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     * @return bool
     */
    public function isAgrupar()
    {
        return $this->agrupar;
    }

    /**
     * @return array
     */
    public function getAditionalSubmitNames()
    {
        return $this->aditionalSubmitNames;
    }
}