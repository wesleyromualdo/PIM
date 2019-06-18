<?php
namespace Simec\Par3\Modelo;

use \Par3_Model_Pendencias as modeloLegado;

/**
 * 
 *
 * @author Felipe Chiavicatti 
 */
class Pendencias extends \Simec\AbstractModelo
{
    protected $modeloLegado;
    
    /**
     * Construtor do objeto de model, que carrega o registro caso seja passo o parâmetro $id
     * @param int $id pk da tabela para carregar os dados
     * @param int $tempocache Time to live, in seconds
     */
    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->modeloLegado = new modeloLegado($id);
    }
    
    public function buscaDetalheItemSemContrato($dados)
    {
        $dados['filtro'] = 'NOT(SUM(contratos_item_ano) = SUM(item_ano))';
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }
    
    public function buscaDetalheItemContratoSqmQtdRecebido($dados)
    {
        $dados['filtro'] = 'NOT(SUM(item_contrato_recebido_ano) = SUM(contratos_item_ano))';
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }
    
    public function buscaDetalheItemSemDetalhar($dados)
    {
        $dados['filtro'] = "SUM(item_contrato_recebido_detalhado_ano) = 0 AND UPPER(ico.icodescricao) ILIKE '%ÔNIBUS%'";
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }
    
    public function buscaDetalheItemDetalhadoMenosRecebido($dados)
    {
        $dados['filtro'] = '(SUM(item_contrato_recebido_ano) > SUM(item_contrato_recebido_detalhado_ano))';
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }
    
    public function buscaDetalheItemSemNota($dados)
    {
        $dados['filtro'] = 'NOT(SUM(item_com_nota_ano) = SUM(item_ano))';
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }
    
    public function buscaDetalheContratoDiligencia($dados)
    {
        $dados['filtro'] = 'SUM(contrato_diligencia_ano) > 0';
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }
    
    public function buscaDetalheNotaDiligencia($dados)
    {
        $dados['filtro'] = 'SUM(nota_diligencia_ano) > 0';
        $sql = $this->modeloLegado->montaSQLPendenciarMonitoramento($dados);
        
        return $this->carregar($sql);
    }

    public function buscaDetalheTermosNAssinados($dados)
    {
        return $this->modeloLegado->buscaDetalheTermosNAssinados($dados);
    }
    
    public function recuperarPendenciasEntidade($inuid, $estuf = null, $muncod = null, $itrid = null)
    {
        if ($itrid === '2'){
            //  $where[] = "instrumentounidade.muncod = '$muncod'";
            $esfera = 'M';
        }else{
            // $where[] = "instrumentounidade.estuf = '$estuf'";
            $esfera = 'E';
        }
        
        $where[] = "empesfera = '$esfera'";
        $where[] = "instrumentounidade.inuid = {$inuid}";
        
        $sql = "select 
                    * 
                from 
                    obras2.v_pendencia_obras_base as mfv
                join territorios.estado as estuf on estuf.estuf = mfv.estuf
                left join territorios.municipio as municipio on municipio.muncod = mfv.muncod
                join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = estuf.estuf and 
                                                                      instrumentounidade.muncod = municipio.muncod
                WHERE 
                    ".implode(' AND ', $where)." 
                ORDER BY 
                    mfv.obrid";
        //ver($sql);
        $dados = $this->carregar($sql,0,600000);
        return $dados;        
    }
    
    public function recuperaProcessosRegraPagamento($inuid, $estuf = null, $muncod = null, $itrid = null,$retornaQuery = false)
    {
        return $this->modeloLegado->recuperaProcessosRegraPagamento($inuid, $estuf, $muncod, $itrid, $retornaQuery);
    }
    
    public function recuperaProcessosRegraSaldoConta($inuid, $estuf = null, $muncod = null, $itrid = null,$retornaQuery = false)
    {
        return $this->modeloLegado->recuperaProcessosRegraSaldoConta($inuid, $estuf, $muncod, $itrid, $retornaQuery);
    }
     
    public function recuperaProcessosRegraMonitoramento2010($inuid)
    {
        return $this->modeloLegado->recuperaProcessosRegraMonitoramento2010($inuid);
    }
    
    public function recuperaProcessosRegraMonitoramento2010Termos($inuid)
    {
        return $this->modeloLegado->recuperaProcessosRegraMonitoramento2010Termos($inuid);
    }
}
