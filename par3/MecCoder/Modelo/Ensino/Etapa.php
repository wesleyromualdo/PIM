<?php
namespace Simec\Par3\Modelo\Ensino;

use Simec\Par3\Dado\Ensino\Etapa as dadosEnsinoEtapa;
use \Par3_Model_EnsinoEtapa as modeloLegado;

class Etapa extends \Simec\AbstractModelo
{
    /**
     * @var dadosEnsinoEtapa
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
        $this->dados->carregar($this->modeloLegado->getArAtributos);
        return $this;
    }

    public function antesSalvar()
    {
        return $this->modeloLegado->antesSalvar();
    }

    public function pegarSelectCombo(){
        return $this->modeloLegado->pegarSelectCombo();
    }

    public function pegarSQLSelectCombo($nivid = array(), $etaid)
    {
        return $this->modeloLegado->pegarSQLSelectCombo($nivid, $etaid);
    }

    public function listaEnsinoEtapas($nivid)
    {
        return $this->modeloLegado->listaEnsinoEtapas($nivid);
    }

    public function getFormListaEnsinoEtapa($nivid = null, $etaid = null)
    {
        return $this->modeloLegado->getFormListaEnsinoEtapa($nivid, $etaid);
    }

    public function findById($etaid)
    {
        return $this->modeloLegado->findById($etaid);
    }

}
