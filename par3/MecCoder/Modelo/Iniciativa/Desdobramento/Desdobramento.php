<?php
namespace Simec\Par3\Modelo\Iniciativa\Desdobramento;

use Simec\Par3\Dado\Iniciativa\Desdobramento\Desdobramento as dadosIniciativaDesdobramento;
use \Par3_Model_IniciativaDesdobramentos as modeloLegado;

class Desdobramento extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaDesdobramento
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

    public static function campo()
    {
        return 10;
    }

    public static function urbano()
    {
        return \Simec\Simec::producao() ? 1 : 11;
    }

    public static function integral()
    {
        return \Simec\Simec::producao() ? 4 : 12;
    }

    public static function parcial()
    {
        return \Simec\Simec::producao() ? 3 : 13;
    }

    public static function rural()
    {
        return \Simec\Simec::producao() ? 2 : 14;
    }

    public function montarSQLSimples($arrPost)
    {
        return $this->modeloLegado->montarSQLSimples($arrPost);
    }

    public function verificarExisteDesdobramento(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteDesdobramento($arrPost);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function pegarSelectCombo(){
        return $this->modeloLegado->pegarSelectCombo();
    }

    public function listaDesdobramentos()
    {
        return $this->modeloLegado->listaDesdobramentos();
    }

    public function getIniciativaDesdobramentoById($desid)
    {
        return $this->modeloLegado->getIniciativaDesdobramentoById($desid);
    }

    public function verificaLigacaoPlanejamentoById($desid){
        return $this->modeloLegado->verificaLigacaoPlanejamentoById($desid);
    }
}