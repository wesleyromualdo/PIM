<?php
namespace Simec\Par3\Modelo;

use \Par3_Model_Siope as modeloLegado;

/**
 * 
 *
 * @author Felipe Chiavicatti 
 */
class Siope extends \Simec\AbstractModelo
{
    public $dados;
    
    protected $modeloLegado;
    
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
    
    public function transmissaoSiope($dados)
    {
        return $this->modeloLegado->transmissaoSiope($dados);
    }
}
