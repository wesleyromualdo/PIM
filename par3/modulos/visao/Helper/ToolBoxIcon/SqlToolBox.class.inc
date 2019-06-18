<?php
/**
 * Created by PhpStorm.
 * User: fiuza
 * Date: 29/08/18
 * Time: 16:04
 */

namespace visao\Helper\ToolBoxIcon;

require_once APPRAIZ . 'par3/modulos/visao/Helper/ToolBoxIcon/ToolBoxIcon.class.inc';

class SqlToolBox extends ToolBoxIcon
{
    protected $sql;
    protected $id;

    public function iniciarFluxo()
    {

    }

    public function setSql($sql = null) {
        if($sql) {
            $this->sql = $sql;
        }
        return $this;
    }

    public function setId($id = null)
    {
        if($id) {
            $this->id = $id;
        }
        return $this;
    }
}