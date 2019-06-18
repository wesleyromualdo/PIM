<?php
namespace Simec\Par3\Modelo\Iniciativa;

use Simec\Par3\Dado\Iniciativa\IniciativasAnos as dadosIniciativaIniciativasAnos;
use \Par3_Model_IniciativaIniciativasAnos as modeloLegado;

class IniciativasAnos extends \Simec\AbstractModelo {
    /**
     * @var dadosIniciativaIniciativasAnos
     */
    public  $dados;

    public  $modeloLegado;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        if ($id) $this->carregarDados($id, $tempocache);
    }

    public function carregarDados($id, $tempocache = null)
    {
        if ( is_null($this->modeloLegado) ) $this->modeloLegado = new modeloLegado($id, $tempocache);
        $this->dados->carregar($this->modeloLegado->getArAtributos());
        return $this;
    }

    public function pegarSQLSelectCombo($iniid = array(), $cicid = array())
    {
        return $this->modeloLegado->pegarSQLSelectCombo($iniid, $cicid);
    }

    public function getCamposValidacao($dados = array())
    {
        return $this->modeloLegado->getCamposValidacao($dados);
    }

    public function antesSalvar()
    {
        return $this->modeloLegado->antesSalvar();
    }

    public function recuperarPorIniciativa($iniid)
    {
        return $this->modeloLegado->recuperarPorIniciativa($iniid);
    }
}
