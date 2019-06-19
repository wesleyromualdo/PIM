<?php
namespace Simec\Par3\Modelo\Iniciativa;

use Simec\Par3\Dado\Iniciativa\TiposObjeto as dadosIniciativaTiposObjeto;
use \Par3_Model_IniciativaTiposObjeto as modeloLegado;

class TiposObjeto extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaTiposObjeto
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

    public function pegarSQLSelectComboUnidade($intaid = array())
    {
        return $this->modeloLegado->pegarSQLSelectComboUnidade($intaid);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

}