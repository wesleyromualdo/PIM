<?php
namespace Simec\Par3\Modelo;

use \Par3_Model_Restricao as modeloLegado;

/**
 * 
 *
 * @author Felipe Chiavicatti 
 */
class Restricao extends \Simec\AbstractModelo
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
    
    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br> 
     * @description < Descobre se a unidade é do tipo 1(estado) ou 2(municipio), retorna array das obras que possuem restrições tuleap hstória 12936>
     * @param $inuid
     * @return array;
     */
    public function getObrasRestricoesInstrumentoUnidadeById($inuid)
    {
        return $this->modeloLegado->getObrasRestricoesInstrumentoUnidadeById($inuid);       
    }
    
    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description Esta função é utilizada para carregar as restrições da obra
     * @param $obrid
     * @return array|mixed|NULL
     */
    public function getRestricoesDaObra($obrid)
    {
        return $this->modeloLegado->getRestricoesDaObra($obrid);
    }
}