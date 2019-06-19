<?php
namespace Simec\Par3\Modelo\Iniciativa\Areas;

use Simec\Par3\Dado\Iniciativa\Areas\Areas as dadosIniciativaAreas;
use \Par3_Model_IniciativaAreas as modeloLegado;

class Areas extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaAreas
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

    public function verificarExisteArea(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteArea($arrPost);
    }

    public function verificarExisteCodigoArea(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteCodigoArea($arrPost);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function pegarSQLSelectComboSigla($arrPost = null)
    {
        return $this->modeloLegado->pegarSQLSelectComboSigla($arrPost);
    }

    public function pegarSQLSelectComboDescricao($arrPost = null)
    {
        return $this->modeloLegado->pegarSQLSelectComboDescricao($arrPost);
    }

    public function pegarSQLSelectComboDescricaoUnidade($inuid)
    {
        return $this->modeloLegado->pegarSQLSelectComboDescricaoUnidade($inuid);
    }

    public function listaAreas($arrPost = null)
    {
        return $this->modeloLegado->listaAreas($arrPost);
    }

    public function getIniciativaAreaById($iarid)
    {
        return $this->modeloLegado->getIniciativaAreaById($iarid);
    }

    public function verificaLigacaoPlanejamentoById($iarid)
    {
        return $this->modeloLegado->verificaLigacaoPlanejamentoById($iarid);
    }

    public function getAreaPorIniciativa()
    {
        return $this->modeloLegado->getAreaPorIniciativa();
    }
}