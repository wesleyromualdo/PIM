<?php
namespace Simec\Par3\Modelo\Programa;

use Simec\ModeloLegadoTrait;
use Simec\Par3\Dado\Programa\Programa as dadosPrograma;

class Programa extends \Simec\AbstractModelo {

    use ModeloLegadoTrait;

    /**
     * @var dadosPrograma
     */
    public $dados;

    private $nomeModeloLegado = \Par3_Model_IniciativaIniciativasProgramas::class;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->carregarModeloLegado($this->nomeModeloLegado, $id, $tempocache);
    }

    public function montarSQLSimples($arrPost)
    {
        return $this->modeloLegado->montarSQLSimples($arrPost);
    }

    public function getIniciativa($arrPost) {
        return $this->modeloLegado->getIniciativa($arrPost);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function antesSalvar()
    {
        return $this->modeloLegado->antesSalvar();
    }

    public function pegarSQLSelectComboSigla()
    {
        return $this->modeloLegado->pegarSQLSelectComboSigla();
    }

    public function pegarSQLSelectComboSiglaUnidade($inuid)
    {
        return $this->modeloLegado->pegarSQLSelectComboSiglaUnidade($inuid);
    }

    public function getIniciativaProgramaById($prgid)
    {
        return $this->modeloLegado->getIniciativaProgramaById($prgid);
    }

    public function verificaLigacaoPlanejamentoById($prgid){
        return $this->modeloLegado->verificaLigacaoPlanejamentoById($prgid);
    }

}
