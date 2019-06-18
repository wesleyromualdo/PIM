<?php
namespace Simec\Par3\Modelo\Iniciativa;

use Simec\Par3\Dado\Iniciativa\IniciativasProjetos as dadosIniciativaIniciativasProjetos;
use \Par3_Model_IniciativaIniciativasProjetos as modeloLegado;

class IniciativasProjetos extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaIniciativasProjetos
     */
    public $dados;

    protected $modeloLegado;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        if ($id) $this->carregarDados($id, $tempocache);
    }

    public function carregarDados($id, $tempocache = null){
        if ( is_null($this->modeloLegado) ) $this->modeloLegado = new modeloLegado($id, $tempocache);
        $this->dados->carregar($this->modeloLegado->arAtributos);
        return $this;
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