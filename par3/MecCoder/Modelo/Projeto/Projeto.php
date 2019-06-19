<?php
namespace Simec\Par3\Modelo\Projeto;

use Simec\ModeloLegadoTrait;
use Simec\Par3\Dado\Projeto\Projeto as dadosProjeto;

class Projeto extends \Simec\AbstractModelo
{
    use ModeloLegadoTrait;

    /**
     * @var dadosProjeto
     */
    public $dados;

    private $nomeModeloLegado = \Par3_Model_Projeto::class;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->carregarModeloLegado($this->nomeModeloLegado, $id, $tempocache);
    }

    public function montarSQLSimples($arrPost)
    {
        return $this->modeloLegado->montarSQLSimples($arrPost);
    }

    public function verificarExisteProjetoDescricao(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteProjetoDescricao($arrPost);
    }

    public function verificarExisteProjetoSigla(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteProjetoSigla($arrPost);
    }

    public function pegarSQLSelectCombo($proid = array())
    {
        return $this->modeloLegado->pegarSQLSelectCombo($proid);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function getIniciativaProjetoById($proid){
        return $this->modeloLegado->getIniciativaProjetoById($proid);
    }

    public function verificaLigacaoPlanejamentoById($proid){
        return $this->modeloLegado->verificaLigacaoPlanejamentoById($proid);
    }
}
