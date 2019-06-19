<?php

/**
 * Classe de controle do  situacaoprestacaocontasobras
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14-08-2018
 * @link     no link
 */

/**
 * Obras2_Controller_Situacaoprestacaocontasobras
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 14-08-2018
 * @link no link
 */
class Obras2_Controller_Situacaoprestacaocontasobras
{

    private $model;

    public function __construct()
    {
        $this->model = new Obras2_Model_Situacaoprestacaocontasobras($_GET['spoid']);
    }

    /**
     * Função gravar
     * - grava os dados
     */
    public function salvar()
    {
        global $url;
        // $this->model = new Obras2_Controller_Situacaoprestacaocontasobras();
        $this->model->popularDadosObjeto();
        $url .= '&spoid=' . $this->model->spoid;
        
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
     * Funcao responsável por registrar a Omissao do Ente
     * m
     *
     * @param            
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function confirmaRegistros($arrParams = Array())
    {
        $objOmissao = new Par_Model_Omissaopcente();
        return $objOmissao->confirmaRegistros();
    }
    
    
    /**
     * 
     * */
    public function tramitaProcessosSemSaldo($arrDocumento)
    {
    

        $docid = $arrDocumento['docid_situ_pc'];
        $esdid = $arrDocumento['esdid_situacao'];
    
        // Tramitar para Situação da PC: Sem Valor a Comprovar 
        
        // Trata de acordo com a origem da fase.
        
       //2273;"Não Enviada"
        if ($esdid == ESDID_SITUACAO_PC_NAO_ENVIADA) {
            $acao = AEDID_OB_SIT_NAO_ENVI_P_SEM_VALOR; 
        }
        // 2286;"Enviada ao FNDE" agora "
        if ($esdid == ESDID_SIT_PC_OBRAS_EM_ANALISE) {
            $acao = AEDID_OB_SIT_ANALISE_FNDE_P_SEM_VALOR;
        }
        //2274;"Enviada ao Controle Social"
        if ($esdid == ESDID_PC_ENVIADA_AO_CONTROLE_SOCIAL) {
            $acao = AEDID_OBR_SIT_CACS_P_SEM_VALOR; 
        }
        //2275;"Notificada por Omissão"
        if ($esdid == ESDID_OBRAS_NOTIFICADA_POR_OMISSAO) {
            $acao = AEDID_OB_SIT_NOTIFICADA_P_SEM_VALOR;
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
    
    /**
     * Tabela responsável por verificar se existem processos que ainda não estão com o fluxo, e os inserir
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function cadastroProcessos($arrParams)
    {
        
        // @todo
        // Chama função do $this->model para verificar quais processos devem ser cadastrados
        $arrReturn = $this->model->retornaProcessosSemFluxo($arrParams);
        // chama função do $this->model para inserir os registros e caso o retorno seja verdadeiro cadastroRegistro = true
        $existe = $arrReturn["existeRegistro"];
        
        if ($existe) {
            $arrRegistrosObrasPAR = $arrReturn['cadastroRegistroPAR'];
            $arrRegistrosObrasPAC = $arrReturn['cadastroRegistroPAC'];
            
            $inseriuObrasPar = $this->model->insereRegistrosInexistenteObrasPar($arrRegistrosObrasPAR);
            $inseriuObrasPaC = $this->model->insereRegistrosInexistenteObrasPac($arrRegistrosObrasPAC);
            
            if ($inseriuObrasPar && $inseriuObrasPaC) {
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
     * Funcao responsável por realizar a ação de enviar a situação a "Notificada por Omissao"
     * m
     *
     * @param            
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function tramitaFluxoWF($arrParams)
    {
        /* Recupera parâmetros do array */
        $acao = $arrParams['aedid'];
        $destino = $arrParams['destino'];
        $origem = $arrParams['origem'];
        $docid = $arrParams['docid'];
        $commit = $arrParams['commit'];
        
        if (($acao == AEDID_OBRAS_SPC_NOTIFICAR_POR_OMISSAO) && (($acao != '') && ($docid != ''))) {
            
            $tramitacao = wf_alterarEstado($docid, $acao, "Notificando a entidade por omissão após 60 dias.", array(), array(
                'commit' => $commit
            ));
            
            if ($tramitacao) {
                return true;
            }
        }
    }

    /**
     * Funcao responsável por realizar a ação de enviar a situação a "Notificada por Omissao"
     * m
     *
     * @param            
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *        
     */
    public function tramitaFluxoWFCACS($arrParams)
    {
        /* Recupera parâmetros do array */
        $acao = $arrParams['aedid'];
        $destino = $arrParams['destino'];
        $origem = $arrParams['origem'];
        $docid = $arrParams['docid'];
        $commit = $arrParams['commit'];
        
        if (($acao == AEDID_OBRAS_SPC_NOTIFICAR_CACS_POR_OMISSAO) && (($acao != '') && ($docid != ''))) {
            
            $tramitacao = wf_alterarEstado($docid, $acao, "Notificando a entidade por omissão após 60 dias.", array(), array(
                'commit' => $commit
            ));
            
            if ($tramitacao) {
                return true;
            }
        }
    }

    public function registraOmissaoEntePar($arrParams = Array())
    {
        if (count($arrParams) > 0) {
            // Cria objeto de Omissao
            $objOmissao = new Par_Model_Omissaopcente();
            $id = $objOmissao->insereRegistroObrasPar($arrParams);
            return $id;
        } else {
            return false;
        }
    }
    
    public function registraOmissaoCACSPar($arrParams = Array())
    {
        if (count($arrParams) > 0) {
            // Cria objeto de Omissao
            $objOmissao = new Par_Model_Omissaopcente();
            $id = $objOmissao->insereRegistroObrasParCACS($arrParams);
            return $id;
        } else {
            return false;
        }
    }

    public function registraOmissaoEntePac($arrParams = Array())
    {
        if (count($arrParams) > 0) {
            // Cria objeto de Omissao
            $objOmissao = new Par_Model_Omissaopcente();
            $id = $objOmissao->insereRegistroObrasPac($arrParams);
            return $id;
        } else {
            return false;
        }
    }

    /**
     * Função excluir
     * - grava os dados
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['spoid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&spoid={$id}";
        $situacaoprestacaocontasobras = new Obras2_Model_Situacaoprestacaocontasobras($id);
        try {
            $situacaoprestacaocontasobras->Xstatus = 'I';
            $situacaoprestacaocontasobras->Xdtinativacao = date('Y-m-d H:i:s');
            $situacaoprestacaocontasobras->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $situacaoprestacaocontasobras->salvar();
            $situacaoprestacaocontasobras->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
}
?>