<?php
namespace Simec\Par3\Modelo\Modalidade;

use Simec\Par3\Dado\Modalidade\Modalidade as dadosModalidade;
use \Par3_Model_Modalidade as modeloLegado;

class Modalidade extends \Simec\AbstractModelo
{
    /**
     * @var dadosModalidade
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

    public function getModalidade(array $arrPost)
    {
        return $this->modeloLegado->getModalidade($arrPost);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function listaModalidade($etaid)
    {
        return $this->modeloLegado->listaModalidade($etaid);
    }

    public function verificaLigacaoPlanejamento($modid)
    {
        return $this->modeloLegado->verificaLigacaoPlanejamento($modid);
    }

    public function getModalidadeById($modid){
        return $this->modeloLegado->getModalidadeById($modid);
    }

    public function getFormListModalidade($modid = null, $etaid = null, $nivid = null){
        return $this->modeloLegado->getFormListModalidade($modid, $etaid, $nivid);
    }

}
