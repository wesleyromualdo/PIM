<?php
namespace Simec\Par3\Modelo\Iniciativa\Planejamento;

use Simec\Par3\Dado\Iniciativa\Planejamento\Desdobramentos as dadosIniciativaPlanejamentoDesdobramentos;
use \Par3_Model_IniciativaPlanejamentoDesdobramentos as modeloLegado;

class Desdobramentos extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaPlanejamentoDesdobramentos
     */
    public $dados;

    protected $modeloLegado;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        if ($id) $this->carregarDados($id, $tempocache);
    }

    public function carregarDados($id, $tempocache = null)
    {
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

    public function salvarDesdobramentos($inpid, $arrDesdobramentos)
    {
        return $this->modeloLegado->salvarDesdobramentos($inpid, $arrDesdobramentos);
    }

    public function recuperarDesdobramentos($inpid = null)
    {
        return $this->modeloLegado->recuperarDesdobramentos($inpid);
    }

    public function recuperarDesdobramentosEscolhida($inpid = null)
    {
        return $this->modeloLegado->recuperarDesdobramentosEscolhida($inpid);
    }

    public function verificaLigacaoIniciativaPlanejamentoDesdobramento($desid)
    {
        return $this->modeloLegado->verificaLigacaoIniciativaPlanejamentoDesdobramento($desid);
    }

    public function getDesdobramentosIniciativaPlanejamento(array $array)
    {
        return $this->modeloLegado->getDesdobramentosIniciativaPlanejamento($array);
    }

    public function verificaExistenciaDoDesdobramentoByIdDesdobramento(array $array, $desid){
        return $this->modeloLegado->verificaExistenciaDoDesdobramentoByIdDesdobramento($array, $desid);
    }

    public function recuperarDesdobramentos($inpid = null)
    {
        $arrIpd = array();
        if($inpid){
            $sql = "
            SELECT 
                des.desid as codigo
            FROM par3.iniciativa_planejamento_desdobramentos as ipd
            INNER JOIN par3.iniciativa_desdobramento as des ON des.desid = ipd.desid
            WHERE ipd.inpid = $inpid
          ";
            $rsIpd = $this->carregar($sql);
            if(is_array($rsIpd)){
                foreach ($rsIpd as $ipd){
                    $arrIpd[] = $ipd['codigo'];
                }
            }
        }
        return $arrIpd;
    }

}