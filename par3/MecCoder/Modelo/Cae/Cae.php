<?php
namespace Simec\Par3\Modelo\Cae;

use \Par3_Model_CAE as modeloLegado;

/**
 * 
 *
 * @author Felipe Chiavicatti 
 */
class Cae extends \Simec\AbstractModelo
{
    public $dados;
    
    protected $modeloLegado;

    /**
     * Construtor do objeto de model, que carrega o registro caso seja passo o parâmetro $id
     * @param int $id pk da tabela para carregar os dados
     * @param int $tempocache Time to live, in seconds
     */
    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->modeloLegado = new modeloLegado();
        //         $this->dados        = new dadosInstrumentoUnidade();
        
        $this->carregarPorId($id, $tempocache);
    }
    
    public function carregarPorId($id, $tempocache = null)
    {
        $this->modeloLegado->carregarPorId($id, $tempocache);
        //         $this->dados->carregar($this->modeloLegado->getDados());
        
        return $this;
    }
    
    public function carregarDadosCAE($inuid)
    {
        $this->modeloLegado->carregarDadosCAE($inuid);
    }
    
    public function carregarDadosPresidenteCAE($inuid)
    {
        $this->modeloLegado->carregarDadosPresidenteCAE($inuid);
    }
    
    public function carregarConselheirosCAE($inuid)
    {
        $this->modeloLegado->carregarConselheirosCAE($inuid);
    }
        
    
}
