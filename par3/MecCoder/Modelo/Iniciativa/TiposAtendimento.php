<?php
namespace Simec\Par3\Modelo\Iniciativa;

use Simec\Par3\Dado\Iniciativa\TiposAtendimento as dadosIniciativaTiposAtendimento;
use \Par3_Model_IniciativaTiposAtendimento as modeloLegado;

class TiposAtendimento extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaTiposAtendimento
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

    public function montarSQLSimples($arrPost)
    {
        return $this->modeloLegado->montarSQLSimples($arrPost);
    }

    public function getIniciativa($arrPost) {
        return $this->modeloLegado->getIniciativa($arrPost);
    }

    public function pegarSQLSelectCombo($intaid = array())
    {
        return $this->modeloLegado->pegarSQLSelectCombo($intaid);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function verificaLigacaoPlanejamento($intaid)
    {
        return $this->modeloLegado->verificaLigacaoPlanejamento($intaid);
    }

    public function getTiposAtendimentoById($intaid){
        return $this->modeloLegado->getTiposAtendimentoById($intaid);
    }
}