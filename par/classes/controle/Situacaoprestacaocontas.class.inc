<?php

/**
 * Classe de controle do  situacaoprestacaocontas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-07-2018
 * @link     no link
 */

/**
 * Par_Controller_Situacaoprestacaocontas
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 04-07-2018
 * @link no link
 */
class Par_Controller_Situacaoprestacaocontas
{

    private $model;

    public function __construct()
    {
        $this->model = new Par_Model_Situacaoprestacaocontas($_GET['spcid']);
    }
    
    public function tramitaProcessosSemSaldo($arrDocumento)
    {
        // Para Sem Valor a Comprovar
        $docid = $arrDocumento['docid_situ_pc'];
        $esdid = $arrDocumento['esdid_situacao'];
    
        // Trata de acordo com a origem da fase.
        
        // "Registro da Execução"
        if ($esdid == ESDID_SPC_NAO_ENVIADA) {
            $acao = AEDID_N_ENVIADA_SEM_VALOR; 
        }
        // "Controle Social"
        if ($esdid == ESDID_SPC_ENVIADA_AO_CONTROLE_SOCIAL) {
            $acao = AEDID_CACS_P_SEM_VALOR;
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
        // Chama função do $this->model para verificar quais processos devem ser cadastrados
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
     * Função gravar
     * - grava os dados
     */
    public function salvar()
    {
        global $url;
        // $this->model = new Par_Controller_Situacaoprestacaocontas();
        $this->model->popularDadosObjeto();
        $url .= '&spcid=' . $this->model->spcid;
        
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
        $id = $_GET['spcid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&spcid={$id}";
        $situacaoprestacaocontas = new Par_Model_Situacaoprestacaocontas($id);
        try {
            $situacaoprestacaocontas->Xstatus = 'I';
            $situacaoprestacaocontas->Xdtinativacao = date('Y-m-d H:i:s');
            $situacaoprestacaocontas->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $situacaoprestacaocontas->salvar();
            $situacaoprestacaocontas->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
}
?>