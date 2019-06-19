<?php
namespace Simec\Par3\Modelo\Iniciativa\Planejamento;

use Simec\ModeloLegadoTrait;
use Simec\Par3\Dado\Iniciativa\Planejamento\Ciclo as dadosIniciativaPlanejamentoCiclo;

class Ciclo extends \Simec\AbstractModelo
{
    use ModeloLegadoTrait;
    /**
     * @var dadosIniciativaPlanejamentoCiclo
     */
    public $dados;

    private $nomeModeloLegado = \Par3_Model_CicloPar::class;

    protected $stNomeTabela = "par3.ciclo_par";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('cicid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cicid'            => null,
        'cidid'            => null,
        'cicdsc'           => null,
        'cicdtinicio'      => null,
        'cicdtfim'         => null,
        'cicstatus'        => null,
        'cicsituacao'      => null,
        'cicvigencia'      => null,
        'cicdtinclusao'    => null,
        'ciccpfinclusao'   => null,
        'cicdtinativacao'  => null,
        'ciccpfinativacao' => null,
        'cicanos'          => null,
        'cicduracao'       => null,

    );

    /**
     * Tipos de Vigência cicvigencia
     */
    const VIGENTE      = 'VIGENTE';
    const FECHADO      = 'FECHADO';
    const NAO_INICIADO = 'NAO_INICIADO';

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->carregarModeloLegado($this->nomeModeloLegado, $id, $tempocache);
    }

    public function montarSQLSimples($arrPost)
    {
        return $this->modeloLegado->montarSQLSimples($arrPost);
    }

    public function verificarExisteCiclo(array $arrPost)
    {
        return $this->modeloLegado->verificarExisteCiclo($arrPost);
    }

    public function verificarExisteDataFim($arrPost)
    {
        return $this->modeloLegado->verificarExisteDataFim($arrPost);
    }

    public function verificarExisteDataInicio($arrPost)
    {
        return $this->modeloLegado->verificarExisteDataInicio($arrPost);
    }

    public function pegarSQLSelectCombo($cicid = array())
    {
        $cicid = is_array($cicid) ? implode(',', $cicid) : $cicid;
        $where  = "WHERE cicstatus = 'A' AND cicsituacao = 'A'";
        $where .= $cicid ? " AND cicid in({$cicid})" : '';
        return "SELECT  cicid as codigo, cicdsc as descricao FROM {$this->stNomeTabela} $where";
    }

    public function selectCiclos()
    {
        $ciclosSQL = $this->pegarSQLSelectCombo();
        return $this->carregar($ciclosSQL);
    }

    public function validarInput(array $campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function verificaLigacaoPlanejamento($cicid)
    {
        return $this->modeloLegado->verificaLigacaoPlanejamento($cicid);
    }

    public function getCicloPorIniciativa()
    {
        return $this->modeloLegado->getCicloPorIniciativa();
    }
}