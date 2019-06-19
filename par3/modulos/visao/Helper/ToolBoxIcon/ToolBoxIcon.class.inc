<?php
/**
 * Created by PhpStorm.
 * User: fiuza
 * Date: 29/08/18
 * Time: 15:37
 */

namespace visao\Helper\ToolBoxIcon;


abstract class ToolBoxIcon extends \MVC\HelperVisao
{
    protected $parentId = '';
    abstract public function setId($id);
    public function setParentId($id)
    {
        $this->parentId = $id;
        return $this;
    }

    public function getParentId()
    {
        return $this->parentId;
    }
}