<?php

/**
 * Classe de controle do  execucaofinanceira
 * @category Class
 * @package  A1
 * @version $Id$
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-08-2018
 * @link     no link
 */

/**
 * Obras2_Controller_Execucaofinanceira
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 13-08-2018
 * @link no link
 */
class Obras2_Controller_Execucaofinanceira
{

    private $model;

    public function __construct()
    {
        include_once APPRAIZ . "obras2/classes/modelo/ExecucaoFinanceira.class.inc";
        $this->model = new ExecucaoFinanceira($_GET['exeid']);
    }

    /**
     * Função gravar
     * - grava os dados
     */
    public function salvar()
    {
        global $url;
        // $this->model = new Obras2_Controller_Execucaofinanceira();
        $this->model->popularDadosObjeto();
        $url .= '&exeid=' . $this->model->exeid;
        
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
     */
    public function administraCadastroProcessosObras($arrParams)
    {
        
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSituOPC = new Obras2_Controller_Situacaoopcobras();
        
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSitPC = new Obras2_Controller_Situacaoprestacaocontasobras();
        
        /* Verifica existencia de registros para inserir e faz a inserção  */
        $resultadoDocPC = $this->cadastroProcessos($arrParams);
        /* Situa STAÇÃO DE CONTAS */
        $resultadoSitPC = $objSitPC->cadastroProcessos($arrParams);
        
        /* Situação da Obrigatoriedade de Prestar Contas */
        $resultadoSitOPC = $objSituOPC->cadastroProcessos($arrParams);
        
        if (($resultadoSitOPC['existeRegistro']) || ($resultadoSitPC['existeRegistro']) || ($resultadoSitOPC['existeRegistro'])) {
            return true;
        } else {
            return false;
        }
    }
    
     public function retornaProcessosSemSaldo($arrParams)
    {
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $arrReturn = $this->model->retornaProcessosSemSaldo($arrParams);
        return $arrReturn;
    }

    /**
     * Tabela responsável por verificar se existem processos que ainda não estão com o fluxo, e os inserir
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function cadastroProcessos($arrParams)
    {
        
        // Chama função do $this->model para verificar quais processos devem ser cadastrados
        $arrReturn = $this->model->retornaProcessosSemFluxo($arrParams);
        
        // chama função do $this->model para inserir os registros e caso o retorno seja verdadeiro cadastroRegistro = true
        $existe = $arrReturn["existeRegistro"];
        
        if ($existe) {
            $arrRegistrosObrasPAR = $arrReturn['cadastroRegistroPAR'];
            $arrRegistrosObrasPAC = $arrReturn['cadastroRegistroPAC'];
            
            $inseriuObrasPar = $this->model->insereRegistrosInexistenteObrasPar($arrRegistrosObrasPAR);
            $inseriuObrasPaC = $this->model->insereRegistrosInexistenteObrasPac($arrRegistrosObrasPAC);
            
            if ($inseriuObrasPar || $inseriuObrasPaC) {
                $inseriu = TRUE;
            }
        } else {
            $inseriu = FALSE;
            $existe = FALSE;
        }
        
        $arrReturn = Array(
            "existeRegistro" => $existe,
            "cadastroRegistro" => $inseriu
        );
        return $arrReturn;
    }

    /**
     * Função responsável por retornar os processos passíveis de notificação
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function retornaProcessosOmissaoEnte($arrParams)
    {
        // Caso retorne algum resultado o existeRegistro é true
        $arrReturn = $this->model->retornaProcessosNotificacaoOmissaoEnte($arrParams);
        return $arrReturn;
    }

    /**
     * Função responsável por retornar os processos passíveis de notificação por parte do CACS
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *
     */
    public function retornaProcessosOmissaoCACS($arrParams)
    {
        // Caso retorne algum resultado o existeRegistro é true
        $arrReturn = $this->model->retornaProcessosNotificacaoOmissaoCACS($arrParams);
        return $arrReturn;
    }
    
    /**
     * Função responsável por retornar os processos passíveis de notificação por parte do CACS
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *
     */
    public function retornaProcessosCACS($arrParams)
    {
        // Caso retorne algum resultado o existeRegistro é true
        $arrReturn = $this->model->retornaProcessosCACS($arrParams);
        return $arrReturn;
    }
    
    
    /**
     */
    public function administraNotificacoes($arrParams)
    {
        
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Obras2_Controller_Situacaoopcobras();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Obras2_Controller_Situacaoprestacaocontasobras();
        
        /* Verifica se tem algum processo vencido para adicionar notificação relativa a omissão inicial do ente (60 dias) */
        $resultadoProcessoOmissao = $this->retornaProcessosOmissaoEnte($arrParams);
        
        // //@tramitar de adimplente para inadimplente (IMPLEMENTAÇÃO FUTURA)
        
        // Caso existam registros
        
        $existe = $arrReturn["existeRegistro"];
        
        if ($resultadoProcessoOmissao['existeRegistro']) {
            // Trata processos
            
            $arrProcessosPAR = (is_array($resultadoProcessoOmissao['cadastroRegistroPAR'])) ? $resultadoProcessoOmissao['cadastroRegistroPAR'] : Array();
            $arrProcessosPAC = (is_array($resultadoProcessoOmissao['cadastroRegistroPAC'])) ? $resultadoProcessoOmissao['cadastroRegistroPAC'] : Array();
            
            // Caso esteja ok solicita verifica as omissoes do ente
            if (count($arrProcessosPAR) > 0) {
                $retorno1 = $this->administraOmissaoEntePAR($arrProcessosPAR);
            }
            if (count($arrProcessosPAC) > 0) {
                $retorno2 = $this->administraOmissaoEntePAC($arrProcessosPAC);
            }
            return ($retorno1 || $retorno2) ? TRUE : FALSE;
        } else {
            return false;
        }
    }

    /**
     */
    public function administraTramitacoesSemSaldo($arrParams)
    {
        
        $arrRegistrosObrasPar   = $arrParams["cadastroRegistroPAR"];
        $arrRegistrosPac        = $arrParams["cadastroRegistroPAC"];
        
        $arrRegistrosObrasPar   = (is_array($arrRegistrosObrasPar)) ? $arrRegistrosObrasPar : Array();
        $arrRegistrosPac        = (is_array($arrRegistrosPac)) ? $arrRegistrosPac : Array();
    
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Obras2_Controller_Situacaoopcobras();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Obras2_Controller_Situacaoprestacaocontasobras();
    
        $tramite = 0;
        /**
         *  */
        
        if(count($arrRegistrosObrasPar) > 0) {
            
            foreach($arrRegistrosObrasPar as $k => $v ) {
                /*Tramita Fase*/
                $retornoFase = $this->tramitaProcessosSemSaldo($v);
                /*Tramita Situação*/
                $retornoSitu = $objSitPC->tramitaProcessosSemSaldo($v);
                /*Tramita OPC */
                $retornoOPC = $objSituOPC->tramitaProcessosSemSaldo($v);
                // CAso tenha havido alguma tramitação, incrementa
                if( $retornoFase || $retornoSitu || $retornoOPC) {
                    $tramite++;
                }
            }
        }
        if(count($arrRegistrosPac) > 0) {
            
            foreach($arrRegistrosPac as $k => $v ) {
                /*Tramita Fase*/
                $retornoFasePAC = $this->tramitaProcessosSemSaldo($v);
                /*Tramita Situação*/
                $retornoSituPAC = $objSitPC->tramitaProcessosSemSaldo($v);
                /*Tramita OPC */
                $retornoOPCPAC = $objSituOPC->tramitaProcessosSemSaldo($v); 
                // CAso tenha havido alguma tramitação, incrementa
                if( $retornoFasePAC || $retornoSituPAC || $retornoOPCPAC ) {
                    $tramite++;
                }
            }
        }
        
        
        if ($tramite > 0){
            $objSitPC->confirmaRegistros();
            return true;
        } else {
            return false;
        }
    }
    
    public function tramitaProcessosSemSaldo($arrDocumento)
    {
        
        // Tramitar para "Registro de Execução"
        $docid = $arrDocumento['docid_fase_pc'];
        $esdid = $arrDocumento['esdid_fase'];
        
        // Trata de acordo com a origem da fase.
        //"Registro da Execução" 2174
        if ($esdid == ESDID_PC_OBRAS_EM_CADASTRAMENTO) {
            return false;
        }
        //"Análise" 2277
        if ($esdid == ESDID_PC_ANALISE) {
            $acao = AEDID_OBR_ANALISE_P_REGISTRO_SEM_SALDO; 
        }
        //"Controle Social" - 2175;
        if ($esdid == ESDID_PC_CONTROLE_SOCIAL) {
            $acao = AEDID_OBR_CACS_P_REGISTRO_SEM_SALDO; 
        }  
        
        if( $acao != '' ) {
            $tramitacao = wf_alterarEstado($docid, $acao, "Tramitando processo sem saldo.", array(), array(
                'commit' => false
            ));
            return $tramitacao;
        } else {
            return false;
        }
    }
    
    public function getInuid($arrParams)
    {
        return $this->model->getInuid($arrParams);
    }

    public function administraOmissaoCACS($arrProcessos)
    {
       
        $arrProcessos = is_array($arrProcessos) ? $arrProcessos : Array();
        $SemPagamentoInserido = 0;
        $inseridos = 0;
        // Ciclo por registro
        foreach ($arrProcessos as $k => $v) {
            
            $proid = '';
            $estadoSituacaoPC = '';
            $arrParams = Array();
            $proid = $v['proid'];
            $inuid = $v['inuid'];
            $estadoSituacaoPC = $v['esdid_situacao'];
            $faseAtual = $v['esdid_fase'];
    
            // Confirma se está na situação inicial de fato
            if ($proid != '' && ($estadoSituacaoPC == ESDID_PC_ENVIADA_AO_CONTROLE_SOCIAL) && ($faseAtual == ESDID_PC_CONTROLE_SOCIAL) && ($inuid != '')) {
    
                $temPagamento = $this->model->verificaExistenciaPagamentoObrasPar($proid);
                if($temPagamento)
                {
                    $inseridos++;
                    // Tramitação;;; //@todo pendente, vai ser após os 15 dias, mas já deixando adiantado
                    $objSituOPC = new Obras2_Controller_Situacaoprestacaocontasobras();
                    $arrParams['aedid'] = AEDID_OBRAS_SPC_NOTIFICAR_CACS_POR_OMISSAO;
                    $arrParams['destino'] = ESDID_OBRAS_SPC_NOTIFICADO_OMISSAO_CACS;
                    $arrParams['origem'] = ESDID_PC_ENVIADA_AO_CONTROLE_SOCIAL;
                    $arrParams['docid'] = $v['docid_situ_pc'];
                    $arrParams['commit'] = FALSE;
                    
                    // Tramita para Notificada por omissao do controle social
                    $tramite = $objSituOPC->tramitaFluxoWFCACS($arrParams);
    
                    // Registra a notificação na tabela que será insumo para a "Ciencia do ente", colocando parametro commit setado como false, para não commitar, igual a do wf
                    if (($proid != "") && ($inuid != "")) {
                        $arrParams = Array(
                            'proid' => $proid,
                            'inuid' => $inuid
                        );
                        $idRegistraOmissao = $objSituOPC->registraOmissaoCACSPar($arrParams);
                    }
    
                    $RegistraOmissao = ($idRegistraOmissao != '') ? TRUE : FALSE;
                    //
                    if ($RegistraOmissao && $tramite) {
    
                        $commit = $objSituOPC->confirmaRegistros();
                        if ($commit) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } else {
                    $SemPagamentoInserido++;
                    continue;
                }
            } else {
                continue;
            }
        }
        // Caso algum tenha sido inserido retorna true
        if ($inseridos > 0) {
            return true;
        } elseif($SemPagamentoInserido > 0 && $inseridos == 0) { // Caso nenhum tenha sido inserido e tenha exceção retorna falso
            return false;
        } else { // 
            return true;
        }
    }
    
    public function administraOmissaoEntePAR($arrProcessos)
    {
       
        $arrProcessos = is_array($arrProcessos) ? $arrProcessos : Array();
        $SemPagamentoInserido = 0;
        $inseridos = 0;
        // Ciclo por registro
        foreach ($arrProcessos as $k => $v) {
            
            $proid = '';
            $estadoSituacaoPC = '';
            $arrParams = Array();
            $proid = $v['proid'];
            $inuid = $v['inuid'];
            $estadoSituacaoPC = $v['esdid_situacao'];
            $faseAtual = $v['esdid_fase'];
            
            // Confirma se está na situação inicial de fato
            if ($proid != '' && ($estadoSituacaoPC == ESDID_SITUACAO_PC_NAO_ENVIADA) && ($faseAtual == ESDID_PC_OBRAS_EM_CADASTRAMENTO) && ($inuid != '')) {
                
                $temPagamento = $this->model->verificaExistenciaPagamentoObrasPar($proid);
                if($temPagamento) 
                {
                    $inseridos++;
                    // Tramita a situação para "Notificada por Omissão" através do objeto controle da situacao da Prestação de Contas
                    $objSituOPC = new Obras2_Controller_Situacaoprestacaocontasobras();
                    $arrParams['aedid'] = AEDID_OBRAS_SPC_NOTIFICAR_POR_OMISSAO;
                    $arrParams['destino'] = ESDID_OBRAS_NOTIFICADA_POR_OMISSAO;
                    $arrParams['origem'] = ESDID_SITUACAO_PC_NAO_ENVIADA;
                    $arrParams['docid'] = $v['docid_situ_pc'];
                    $arrParams['commit'] = FALSE;
                    
                    $tramite = $objSituOPC->tramitaFluxoWF($arrParams);
                    
                    // Registra a notificação na tabela que será insumo para a "Ciencia do ente", colocando parametro commit setado como false, para não commitar, igual a do wf
                    if ($tramite) {
                        $arrParams = Array(
                            'proid' => $proid,
                            'inuid' => $inuid
                        );
                        $idRegistraOmissao = $objSituOPC->registraOmissaoEntePar($arrParams);
                    }
                    
                    $RegistraOmissao = ($idRegistraOmissao != '') ? TRUE : FALSE;
                    // @todo Notifica ex gestores (postal)
                    $notificacaoPostal = true;
                    if ($tramite && $RegistraOmissao && $notificacaoPostal) {
                        
                        $commit = $objSituOPC->confirmaRegistros();
                        if ($commit) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } else {
                    $SemPagamentoInserido++;
                    continue;
                }
            } else {
                continue;
            }
        }
        
        if ($inseridos > 0) {
            return true;
        } elseif($SemPagamentoInserido > 0 && $inseridos == 0) {
            return false;
        } else {
            return true;
        }
    }

    public function administraOmissaoEntePAC($arrProcessos)
    {
        // PRINT_R($arrProcessos);die('tramiiiiiiiiiiitaaaaaaaaaaaaaaaaaaaaaaaaaaa krai');
        $arrProcessos = is_array($arrProcessos) ? $arrProcessos : Array();
        $SemPagamentoInserido = 0;
        $inseridos = 0;
        
        // Ciclo por registro
        foreach ($arrProcessos as $k => $v) {
            
            $proid = '';
            $estadoSituacaoPC = '';
            $arrParams = Array();
            $proid = $v['proid'];
            $inuid = $this->getInuid($v);
            $estadoSituacaoPC = $v['esdid_situacao'];
            $faseAtual = $v['esdid_fase'];
            
            // Confirma se está na situação inicial de fato
            if ($proid != '' && ($estadoSituacaoPC == ESDID_SITUACAO_PC_NAO_ENVIADA) && ($faseAtual == ESDID_PC_OBRAS_EM_CADASTRAMENTO) && ($inuid != '')) {
                
                $temPagamento = $this->model->verificaExistenciaPagamentoObrasPac($proid);
                
                if($temPagamento)
                {
                    
                    $inseridos++;
                    // Tramita a situação para "Notificada por Omissão" através do objeto controle da situacao da Prestação de Contas
                    $objSituOPC = new Obras2_Controller_Situacaoprestacaocontasobras();
                    $arrParams['aedid'] = AEDID_OBRAS_SPC_NOTIFICAR_POR_OMISSAO;
                    $arrParams['destino'] = ESDID_OBRAS_NOTIFICADA_POR_OMISSAO;
                    $arrParams['origem'] = ESDID_SITUACAO_PC_NAO_ENVIADA;
                    $arrParams['docid'] = $v['docid_situ_pc'];
                    $arrParams['commit'] = FALSE;
                    
                    $tramite = $objSituOPC->tramitaFluxoWF($arrParams);
                    
                    // Registra a notificação na tabela que será insumo para a "Ciencia do ente", colocando parametro commit setado como false, para não commitar, igual a do wf
                    if ($tramite) {
                        $arrParams = Array(
                            'proid' => $proid,
                            'inuid' => $inuid
                        );
                        $idRegistraOmissao = $objSituOPC->registraOmissaoEntePac($arrParams);
                    }
                    
                    $RegistraOmissao = ($idRegistraOmissao != '') ? TRUE : FALSE;
                    // @todo Notifica ex gestores (postal)
                    $notificacaoPostal = true;
                    if ($tramite && $RegistraOmissao && $notificacaoPostal) {
                        
                        $commit = $objSituOPC->confirmaRegistros();
                        if ($commit) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } else {
                    $SemPagamentoInserido++;
                    continue;
                }
            } else {
                continue;
            }
        }
        
        if ($inseridos > 0) {
            return true;
        } elseif($SemPagamentoInserido > 0 && $inseridos == 0) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * Função excluir
     * - grava os dados
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['exeid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&exeid={$id}";
        $execucaofinanceira = new Obras2_Model_Execucaofinanceira($id);
        try {
            $execucaofinanceira->Xstatus = 'I';
            $execucaofinanceira->Xdtinativacao = date('Y-m-d H:i:s');
            $execucaofinanceira->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $execucaofinanceira->salvar();
            $execucaofinanceira->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
    
    
    /**
     * *
     * */
    public function administraTramitacoesCACSObrasCanceladas($arrParams)
    {
        //Traz todos os processos do CACS
        $resultadoProcessoOmissao = $this->retornaProcessosCACS($arrParams);
        
        $existeRegistro = $resultadoProcessoOmissao['existeRegistro'];
        $arrProcessos   = (is_array($resultadoProcessoOmissao['arrProcessos'])) ? $resultadoProcessoOmissao['arrProcessos'] : Array();
        // Verifica se existe algum processo e realiza tramitação caso exista
        if( $existeRegistro && ( count($arrProcessos) > 0 ) ) {
            $retorno = $this->realizaTramitacoesCacsObrasCanceladas($arrProcessos);
            return $retorno;
        } else {
            return FALSE;
        }
        
    }
    
    
    /**
     * *
     * */
    public function realizaTramitacoesCacsObrasCanceladas($arrProcessos)
    {
        // Caso existam processos
        if(is_array($arrProcessos)) {
            foreach($arrProcessos as $k => $v ) {
                
                $arrReturn = Array();
                // Retorna os totais das situações da obra
                $arrReturn = $this->model->retornaTotaisObrasCACS($v);
                
                $existe     = FALSE;
                $arrTotais  = Array();
                
                $existe     = $arrReturn['existe'];
                $arrTotais  = $arrReturn['arrTotais'];
                // Caso exista alguma obra 
                if( $existe ) {
                    // Caso exista alguma obra cancelada
                    if ( $arrTotais['canceladas'] > 0 ) {
                        // Verifica se todas as obras do processo estão canceladas
                        if ($arrTotais['canceladas'] == $arrTotais['total']) {
                            // Caso todas estejam canceladas, ele tramita para análise
                           $this->tramitaWFObrasCanceladasCACS($v, $arrTotais);
                        } else {
                            continue;
                        }
                    }  
                        
                } else {
                    continue;
                }
                
            }
        }
        return true;
    }
    
    
    /**
     * *
     * */
    public function tramitaWFObrasCanceladasCACS($dadosProcesso, $arrTotais)
    { 
        // Dados necessários para tramitar
        $docidPC        = $dadosProcesso['docid_fase_pc'];
        $docidSituPC    = $dadosProcesso['docid_situ_pc'];
        $esdid          = $dadosProcesso['esdid_fase'];
        $esdidSitu      = $dadosProcesso['esdid_situacao'];
        $proid          = $dadosProcesso['proid'];
        // Verifica novamaente a fase se é CACS
        if( ($esdid == ESDID_PC_CONTROLE_SOCIAL) && ($docidSituPC != ""))
        {
            // CAso seja monta texto padrão da tramitação
            $textoParecer = "Tramitado via rotina, o processo possui todas as obras canceladas e não necessita realizar a Análise do CACS";
    
            if( $textoParecer != '')
            {
                // Caso os ids estejam corretos
                if( $docidPC!= '' && $docidSituPC != '') {
    
                    // Tramite principal
                    $tramitacao1 = wf_alterarEstado($docidPC, AEDID_CACS_OBR_P_ANALISE, $textoParecer, array(commit => FALSE));
                    
                    // Tramite da Situação
                    if( $esdidSitu == ESDID_PC_ENVIADA_AO_CONTROLE_SOCIAL ) {
                        // CAso não tenha sido notificado
                        $tramitacao2 = wf_alterarEstado($docidSituPC, AEDID_SITU_CACS_OBR_P_ANALISE, $textoParecer, array(commit => FALSE));
                    } else if ($esdidSitu == ESDID_OBRAS_SPC_NOTIFICADO_OMISSAO_CACS) {
                        // Caso esteja notificado por omissao
                        $tramitacao2 = wf_alterarEstado($docidSituPC, AEDID_SITU_CACS_NOTIF_OBR_P_ANALISE, $textoParecer, array(commit => FALSE));
                    }
    
                } else {
                    return false;
                }
                // Caso as tramitações tenham ocorrido corretamente 
                if( $tramitacao1 && $tramitacao2 )
                {
                    // inativa a notificação, caso exista
                    $objOmissao = new Par_Model_Omissaopcente();
                    $retorno = $objOmissao->inativaRegistroObrasParCACS($proid);
                    return ($retorno) ? TRUE : FALSE;
                    
                } else {
                    return false;
                }
            }
        }
        return true;
    }
    
    
    /**
     */
    public function administraNotificacoesCACS($arrParams)
    {
    
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Obras2_Controller_Situacaoopcobras();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Obras2_Controller_Situacaoprestacaocontasobras();
    
        /* Verifica se tem algum processo vencido para adicionar notificação relativa a omissão inicial do ente (60 dias) */
        $resultadoProcessoOmissao = $this->retornaProcessosOmissaoCACS($arrParams);
        // //@tramitar de adimplente para inadimplente (IMPLEMENTAÇÃO FUTURA)
        
        // Caso existam registros
        if ($resultadoProcessoOmissao['existeRegistro']) {
            // Trata processos
            $arrProcessos = (is_array($resultadoProcessoOmissao['arrProcessos'])) ? $resultadoProcessoOmissao['arrProcessos'] : Array();
            // Caso esteja ok solicita verifica as omissoes do ente
            if (count($arrProcessos) > 0) {
                $retorno = $this->administraOmissaoCACS($arrProcessos); // @todo
                return ($retorno) ? TRUE : FALSE;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}
?>