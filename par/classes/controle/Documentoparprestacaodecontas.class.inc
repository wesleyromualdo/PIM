<?php

/**
 * Classe de controle do  documentoparprestacaodecontas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-07-2018
 * @link     no link
 */

/**
 * Par_Controller_Documentoparprestacaodecontas
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 04-07-2018
 * @link no link
 */
class Par_Controller_Documentoparprestacaodecontas
{

    private $model;

    public function __construct()
    {
        $this->model = new Par_Model_Documentoparprestacaodecontas($_GET['dppid']);
    }

    /**
     * Função gravar
     * - grava os dados
     */
    public function salvar()
    {
        global $url;
        // $this->model = new Par_Controller_Documentoparprestacaodecontas();
        $this->model->popularDadosObjeto();
        $url .= '&dppid=' . $this->model->dppid;
        
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
    public function administraCadastroProcessos($arrParams)
    {
        
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Par_Controller_Situacaoopc();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Par_Controller_Situacaoprestacaocontas();
        
        /* Verifica existencia de registros para inserir e faz a inserção */
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
    // Caso retorne algum resultado o existeRegistro é true
        $arrReturn = $this->model->retornaProcessosSemSaldo($arrParams);
        return $arrReturn;
    }

    /**
     */
    public function administraNotificacoes($arrParams)
    {
        
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Par_Controller_Situacaoopc();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Par_Controller_Situacaoprestacaocontas();
        
        /* Verifica se tem algum processo vencido para adicionar notificação relativa a omissão inicial do ente (60 dias) */
        $resultadoProcessoOmissao = $this->retornaProcessosOmissaoEnte($arrParams);
        
        // //@tramitar de adimplente para inadimplente (IMPLEMENTAÇÃO FUTURA)
        
        // Caso existam registros
        if ($resultadoProcessoOmissao['existeRegistro']) {
            // Trata processos
            $arrProcessos = (is_array($resultadoProcessoOmissao['arrProcessos'])) ? $resultadoProcessoOmissao['arrProcessos'] : Array();
            // Caso esteja ok solicita verifica as omissoes do ente
            if (count($arrProcessos) > 0) {
                $retorno = $this->administraOmissaoEnte($arrProcessos); // @todo
                return ($retorno) ? TRUE : FALSE;
            } else {
                return false;
            }
        } else { 
            return false;
        }  
    }
    
    /**
     */
    public function administraTramitacoesSemSaldo($arrParams)
    {
        $arrRegistros = $arrParams['cadastroRegistro'];
        $arrRegistros = (is_array($arrRegistros)) ? $arrRegistros : Array();
        
        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Par_Controller_Situacaoopc();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Par_Controller_Situacaoprestacaocontas();
        
        $tramite = 0;
        if(count($arrRegistros) > 0) {
            foreach($arrRegistros as $k => $v ) {
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
        if ($tramite > 0){
            $objSituOPC->confirmaRegistros();
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
        // "Registro da Execução" 
        if ($esdid == ESDID_AGUARDANDO_ENVIO_PROPONENTE) {
            return false;
        }
        // "Controle Social"
        if ($esdid == ESDID_AGUARDANDO_ANALISE_CONSELHO) {
            $acao = AEDID_CACS_REGISTRO_SEM_SALDO;  
        }
        // "Análise de Retorno para Monitoramento"
        if ($esdid == ESDID_PC_ANALISE_DE_RETORNO_PARA_MONITORAMENTO) {
            $acao = AEDID_ANALISE_P_REGISTRO_SEM_SALDO;
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
    public function administraOmissaoEnte($arrProcessos)
    {
        $arrProcessos = is_array($arrProcessos) ? $arrProcessos : Array();
        $inseridos = 0;
        $excecaoInserida = 0;
        // Ciclo por registro
        if (count($arrProcessos) > 0) {
            foreach ($arrProcessos as $k => $v) {
                $prpid = '';
                $estadoSituacaoPC = '';
                $arrParams = Array();
                $prpid = $v['prpid'];
                $inuid = $v['inuid'];
                $estadoSituacaoPC = $v['esdid_situacao'];
                $faseAtual = $v['esdid_fase'];
                
                // Confirma se está na situação inicial de fato
                if ($prpid != '' && ($estadoSituacaoPC == ESDID_SPC_NAO_ENVIADA) && ($faseAtual == ESDID_AGUARDANDO_ENVIO_PROPONENTE)) {
                    // Verifico se não é termo de formação, se for, continue. Se não for, envia para a alteração de status
                    /*
                     * Validações de EXCEÇÃO, situações onde o processo não será notificado
                     * */
                    $formacao = $this->model->verificaFormacao($prpid);
                    $temPagamento = $this->model->verificaExistenciaPagamento($prpid);
                    $retornoATresDias = $this->model->verificaRetornoUltimosTresDias($prpid);
                    
                    
                    if ((! $formacao) && ($temPagamento) && (!$retornoATresDias)) {
                        $inseridos++;
                        // Tramita a situação para "Notificada por Omissão" através do objeto controle da situacao da Prestação de Contas
                        $objSituOPC = new Par_Controller_Situacaoopc();
                        $arrParams['aedid'] = AEDID_SPC_NOTIFICAR_POR_OMISSAO;
                        $arrParams['destino'] = ESDID_NOTIFICADA_POR_OMISSAO;
                        $arrParams['origem'] = ESDID_SPC_NAO_ENVIADA;
                        $arrParams['docid'] = $v['docid_situ_pc'];
                        $arrParams['commit'] = FALSE;
                        
                        $tramite = $objSituOPC->tramitaFluxoWF($arrParams);
                        
                        // Registra a notificação na tabela que será insumo para a "Ciencia do ente", colocando parametro commit setado como false, para não commitar, igual a do wf
                        if ($tramite) {
                            $arrParams = Array(
                                'prpid' => $prpid,
                                'inuid' => $inuid
                            );
                            $idRegistraOmissao = $objSituOPC->registraOmissaoEnte($arrParams);
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
                        $excecaoInserida++;
                        continue;
                    }
                } else {
                    continue;
                }
            }
            if ($inseridos > 0) {
                return true;
            } elseif($excecaoInserida > 0 && $inseridos == 0) {
                return false;
            } else {
               return true; 
            }
            
            
        } else {
            return false;
        }
    }
    
    public function administraOmissaoCACS($arrProcessos)
    {
        $arrProcessos = is_array($arrProcessos) ? $arrProcessos : Array();
        $inseridos = 0;
        $excecaoInserida = 0;
        // Ciclo por registro
        if (count($arrProcessos) > 0) {
            foreach ($arrProcessos as $k => $v) {
                $prpid = '';
                $estadoSituacaoPC = '';
                $arrParams = Array();
                $prpid = $v['prpid'];
                $inuid = $v['inuid'];
                $estadoSituacaoPC = $v['esdid_situacao'];
                $faseAtual = $v['esdid_fase'];
               
                // Confirma se está na situação inicial de fato
                if ($prpid != '' && ($estadoSituacaoPC == ESDID_SPC_ENVIADA_AO_CONTROLE_SOCIAL) && ($faseAtual == ESDID_AGUARDANDO_ANALISE_CONSELHO)) {
                    
                    // Verifico se não é termo de formação, se for, continue. Se não for, envia para a alteração de status
                    /*
                     * Validações de EXCEÇÃO, situações onde o processo não será notificado
                     * */
                    $formacao = $this->model->verificaFormacao($prpid);
                    $temPagamento = $this->model->verificaExistenciaPagamento($prpid);
    
    
                    if ((! $formacao) && ($temPagamento)) {
                        $inseridos++;
                        // Tramita a situação para "Notificada por Omissão do Conselho Social" através do objeto controle da situacao da Prestação de Contas
                        $objSituOPC = new Par_Controller_Situacaoopc();
                        $arrParams['aedid'] = AEDID_SPC_NOTIFICAR_CACS_POR_OMISSAO;
                        $arrParams['destino'] = ESDID_SPC_NOTIFICADO_OMISSAO_CACS; 
                        $arrParams['origem'] = ESDID_SPC_ENVIADA_AO_CONTROLE_SOCIAL; 
                        $arrParams['docid'] = $v['docid_situ_pc']; 
                        $arrParams['commit'] = FALSE;
                        
                        // Faz o tramite
                        $tramite = $objSituOPC->tramitaFluxoWFCACS($arrParams);
                    
                        // Registra a notificação na tabela que será insumo para a "Ciencia do ente", colocando parametro commit setado como false, para não commitar, igual a do wf
                        if (($prpid != "")&&($inuid != "")) {
                            $arrParams = Array(
                                'prpid' => $prpid,
                                'inuid' => $inuid
                            );
                            //Registra as omissões para assinatura do presidente e do gestor da unidade
                            $registraOmissao = $objSituOPC->registraOmissaoCACS($arrParams);
                        }
    
                        $RegistraOmissao = ($registraOmissao != '') ? TRUE : FALSE;
                        // Caso tenha inserido e feito o tramite com sucesso então ele commita a alteração
                        if ($RegistraOmissao && $tramite) {
    
                            $commit = $objSituOPC->confirmaRegistros();
                            if ($commit) {
                                continue;
                            }
                        } else {
                            continue;
                        }
                    } else {
                        $excecaoInserida++;
                        continue;
                    }
                } else {
                    continue;
                }
            }
            // Caso algum tenha sido alterado
            if ($inseridos > 0) {
                return true;
            } elseif($excecaoInserida > 0 && $inseridos == 0) { /*Caso não tenha mais nenhum caso novo*/
                return false;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }

    /**
     * Função responsável por verificar se existem processos que ainda não estão com o fluxo, e os inserir
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function cadastroProcessos($arrParams)
    {
        // Caso retorne algum resultado o existeRegistro é true
        $arrReturn = $this->model->retornaProcessosSemFluxo($arrParams);
        
        // chama função do $this->model para inserir os registros e caso o retorno seja verdadeiro cadastroRegistro = true
        $existe = $arrReturn["existeRegistro"];
        if ($existe) {
            $arrRegistros = $arrReturn['cadastroRegistro'];
            $inseriu = $this->model->insereRegistrosInexistente($arrRegistros);
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
     * * 
     * */
    public function retornaProcessosOmissaoCACS($arrParams)
    {
        // Caso retorne algum resultado o existeRegistro é true
        $arrReturn = $this->model->retornaProcessosNotificacaoOmissaoCACS($arrParams);
        return $arrReturn;
    }

    /**
     * Função excluir
     * - grava os dados
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['dppid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&dppid={$id}";
        $documentoparprestacaodecontas = new Par_Model_Documentoparprestacaodecontas($id);
        try {
            $documentoparprestacaodecontas->Xstatus = 'I';
            $documentoparprestacaodecontas->Xdtinativacao = date('Y-m-d H:i:s');
            $documentoparprestacaodecontas->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $documentoparprestacaodecontas->salvar();
            $documentoparprestacaodecontas->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
    
    public function contadorTermosOmissao(){
        return $this->model->contadorTermosOmissao();
    }


    /**
     * Método que controla as notificações dos Ex-Gestores.
     *
     * @param $offset
     *
     * @return bool $houveErro
     */
    public function administraNotificacaoExGestores($offset){
        
        //Busca por omissão ativa
        $arrOmissao = $this->model->recuperaOmissoes($offset);
        
        $houveErro = false;
        
        if($arrOmissao['existeRegistro']){
            $notificouTodos = true;
            foreach($arrOmissao['omissoesRegistros'] as $omissao){
                
                $situacao = SITUACAO_NOTIFICACAO_EXGESTORES_INICIADO;
                
                //verifica se a omissão já foi notificada  
                $notificaexgestorespc = $this->model->verificaNotificaExGestores($omissao, $situacao);
    
                if($notificaexgestorespc['sngid'] != SITUACAO_NOTIFICACAO_EXGESTORES_FINALIZADO){

                    //se a notificação não foi finalizada, verificamos os ex-gestores do municipio em questão para enviar a notificação.
                    $exGestores = $this->model->verificaExGestores($notificaexgestorespc, $omissao);
                    $notificouTodos = $this->model->notificarExGestores($exGestores);

                    
                    if($notificouTodos){
                        $situacao = SITUACAO_NOTIFICACAO_EXGESTORES_FINALIZADO;
                    }else{
                        $situacao = SITUACAO_NOTIFICACAO_EXGESTORES_EXECUTADO_PARCIALMENTE;
                        $houveErro = true;
                    }
                    
                    $this->model->atualizarNotificaExGestores($situacao, $omissao['opcid']);
                }
                
            }
        }

        return $houveErro;
    }
    
    
    public function buscaDadosPorToken($token){
              
        return $this->model->buscaDadosPorToken($token);
                
    }
    
    public function confirmarCiencia($egnid){
        return $this->model->confirmarCiencia($egnid);
    }
    
    public function salvarUploadPC($arqid, $egnid, $descricao){
        $this->model->salvarUploadPC($arqid, $egnid, $descricao);
    }
    
    public function salvarDadosCpfReceita($pessoa){
        $this->model->salvarDadosCpfReceita($pessoa);
    }
    
    public function tramitaPcExGestor($docid)
    {

        $tramitacao = wf_alterarEstado($docid, AEDID_ENVIAR_PARA_ANALISE, "Enviar PC.", array(), array());
        
        return $tramitacao;
    }
    
    /**
     * * 
     * */
    public function administraNotificacoesCACS($arrParams){


        /* OBJ situação da PRESTAÇÃO DE CONTAS */
        $objSituOPC = new Par_Controller_Situacaoopc();
        /* OBJ Situação da Obrigatoriedade de Prestar Contas */
        $objSitPC = new Par_Controller_Situacaoprestacaocontas();
        
        /* Verifica se tem algum processo vencido para adicionar notificação relativa a omissão inicial do CACS (60 dias) */
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