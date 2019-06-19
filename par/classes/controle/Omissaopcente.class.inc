<?php

/**
 * Classe de controle do  omissaopcente
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-07-2018
 * @link     no link
 */

/**
 * Par_Controller_Omissaopcente
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 24-07-2018
 * @link no link
 */
class Par_Controller_Omissaopcente
{

    private $model;

    public function __construct()
    {
        $this->model = new Par_Model_Omissaopcente($_GET['opcid']);
    }

    /**
     * Função gravar
     * - grava os dados
     */
    public function salvar()
    {
        global $url;
        // $this->model = new Par_Controller_Omissaopcente();
        $this->model->popularDadosObjeto();
        $url .= '&opcid=' . $this->model->opcid;
        
        try {
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        
        if ($sucesso) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }

    /**
     * Função excluir
     * - grava os dados
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['opcid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&opcid={$id}";
        $omissaopcente = new Par_Model_Omissaopcente($id);
        try {
            $omissaopcente->Xstatus = 'I';
            $omissaopcente->Xdtinativacao = date('Y-m-d H:i:s');
            $omissaopcente->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $omissaopcente->salvar();
            $omissaopcente->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function filtraTipoProcesso($arrProcessos = Array())
    {
        $arrProcessos = (is_array($arrProcessos)) ? $arrProcessos : Array();
        
        $arrProcessosPar = Array();
        $arrProcessosObrasPar = Array();
        $arrProcessosPac = Array();
        
        foreach ($arrProcessos as $k => $v) {
        
            if ($v['tipo'] == 'PAR') {
                $arrProcessosPar[] = $v; 
            } elseif ($v['tipo'] == 'OBRAS_PAR') {
                $arrProcessosObrasPar[] = $v;
            } elseif ($v['tipo'] == 'PAC') {
                $arrProcessosPac[] = $v;
            }
        }
        
        $existePar      = (count($arrProcessosPar) > 0) ? TRUE : FALSE; 
        $existeObrasPar = (count($arrProcessosObrasPar) > 0) ? TRUE : FALSE;
        $existePac      = (count($arrProcessosPac) > 0) ? TRUE : FALSE;
        
        $arrReturn = Array(
            "existePar" =>  $existePar, 
            "existeObrasPar" =>  $existeObrasPar,
            "existePac" =>  $existePac,
            "arrProcessosPar" => $arrProcessosPar,
            "arrProcessosObrasPar" => $arrProcessosObrasPar,
            "arrProcessosPac" => $arrProcessosPac
            );
        
        return $arrReturn;
    }
    /**
     * 
     * */
    public function administraInadimplencia($arrParams)
    {
        /*Lista todos processos passíveis de inadimplencia*/
        $listaProcessos = $this->model->getListaPrincipalATramitar($arrParams);
        /*Caso exista processos*/
        if ($listaProcessos["existeRegistro"]) {
            /* Trata processos por tipo, separa nas 3 caterorias, PAR, PAR_OBRAS, PAC */
            $arrProcessos = $listaProcessos['arrProcessos'];
            $arrProcessosPorTipo = $this->filtraTipoProcesso($arrProcessos);
            
            $objSituOPC = new Par_Controller_Situacaoopc();
            $objSituOPCObras = new Obras2_Controller_Situacaoopcobras();
            
            /*Trata existencia de PAR, OBRAS PAR, PAC*/
            if( $arrProcessosPorTipo['existePar'] ) {
                /*TRAMITA PARA INADIMPLENCIA*/
                $processosPar = (is_array($arrProcessosPorTipo['arrProcessosPar'])) ? $arrProcessosPorTipo['arrProcessosPar'] : Array(); 
                $resultadoSitOPCPar = $objSituOPC->tramitaInadimplenciaPar($processosPar);
            }
            
            if( $arrProcessosPorTipo['existeObrasPar'] ) {
                /*TRAMITA PARA INADIMPLENCIA*/
                $processosObrasPar = (is_array($arrProcessosPorTipo['arrProcessosObrasPar'])) ? $arrProcessosPorTipo['arrProcessosObrasPar'] : Array();
                $resultadoSitOPCObrasPar = $objSituOPCObras->tramitaInadimplenciaObras($processosObrasPar);
            }
            
            if( $arrProcessosPorTipo['existePac'] ) {
                /*TRAMITA PARA INADIMPLENCIA*/
                $processosObrasPac = (is_array($arrProcessosPorTipo['arrProcessosPac'])) ? $arrProcessosPorTipo['arrProcessosPac'] : Array();
                $resultadoSitOPCObrasPac = $objSituOPCObras->tramitaInadimplenciaObras($processosObrasPac);
            }
            /*CASO ALGUM DOS TIPOS RETORNE ELE RETORNA TRUE PARA INCREMENTAR A PRÓXIMA BUSCA, ATÉ QUE NÃO SOBRE NENHUM*/
            return ($resultadoSitOPCPar|| $resultadoSitOPCObrasPar || $resultadoSitOPCObrasPac ) ? TRUE : FALSE;
        } else {
            return FALSE;
        }
    }
    
    public function getInuidPeloObras($cpf = null)
    {
        
        if (($cpf != null) && ($cpf != '')) {
            
            $inuid = $this->model->retornaInuidPar($cpf, 'obras2');
            return $inuid;
        } else {
            return '';
        }
    }

    public function getInuidPeloPar3($cpf = null)
    {
        
        if (($cpf != null) && ($cpf != '')) {
            
            $inuid = $this->model->retornaInuidPar($cpf, 'par3');
            return $inuid;
        } else {
            return '';
        }
    }
    
    public function pegaArrayPerfil($usucpf){
        return $this->model->pegaArrayPerfil($usucpf);
    }

    /**
     */
    public function verificaExistenciaOmissaoNaoAssinada($inuid)
    {
        // @todo verifica o efeito suspensivo, liberaEfeitoSuspensivo
        $efeitoSuspensivoOk = true;
        $omissao = $this->model->verificaExistenciaOmissaoNaoAssinada($inuid);
        
        if ($omissao && $efeitoSuspensivoOk) {
            return true;
        } else {
            return false;
        }
    }
    
    
    /**
     */
    public function insereCienciaOmissao($arrParams = Array())
    {
        $insercao = $this->model->insereCienciaOmissao($arrParams);
        return $insercao;
    }
    
    public function isPrefeitoPar($usucpf) {
        return $this->model->isPrefeitoPar($usucpf);
    }
    /**
     */
    public function getDadosUnidade($inuid = '')
    {
        
        $insercao = $this->model->getDadosUnidade($inuid);
        return $insercao;
    }

    /**
     */
    public function retornaListaProcessosComOmissao($inuid, $naoAssinada = true)
    {
        return $this->model->retornaListaProcessosComOmissao($inuid, $naoAssinada);
    }
}
?>