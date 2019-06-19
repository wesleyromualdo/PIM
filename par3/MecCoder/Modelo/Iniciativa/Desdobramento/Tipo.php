<?php
namespace Simec\Par3\Modelo\Iniciativa\Desdobramento;

use Simec\Par3\Dado\Iniciativa\Desdobramento\Tipo as dadosIniciativaDesdobramentoTipo;
use \Par3_Model_IniciativaDesdobramentosTipo as modeloLegado;

class Tipo extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaDesdobramentoTipo
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

    public function pegarSQLSelectCombo($tipid = null)
    {
        return $this->modeloLegado->pegarSQLSelectCombo($tipid);
    }

    public function verificarExisteDesdobramentoTipo(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteDesdobramentoTipo($arrPost);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

}