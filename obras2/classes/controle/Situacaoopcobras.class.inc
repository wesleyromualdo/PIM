<?php
/**
 * Classe de controle do  situacaoopcobras
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14-08-2018
 * @link     no link
 */



/**
 * Obras2_Controller_Situacaoopcobras
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14-08-2018
 * @link     no link
 */
class Obras2_Controller_Situacaoopcobras
{
    private $model;

    public function __construct()
    {
        $this->model = new Obras2_Model_Situacaoopcobras($_GET['sopid']);
        
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Obras2_Controller_Situacaoopcobras();
        $this->model->popularDadosObjeto();
        $url .= '&sopid=' . $this->model->sopid;

        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }

    
    public function tramitaInadimplenciaObras($processosObrasPar)
    {
        /*Pega os registros para tramitar*/
        $processosObrasPar = (is_array($processosObrasPar)) ? $processosObrasPar : Array();
        $tramitou = 0;
        /*Caso tenha registro faz tramitação, senão retorna falso*/
        if (count($processosObrasPar) > 0) {
            foreach ($processosObrasPar as $k => $v) {
                
                $estadoAtual = '';
                $estadoAtual = $v['situacao_opc'];
                $acao = AEDID_OBRAS_OPC_DE_ADIMPLENTE_PARA_INADIMPLENTE;
                /*Verifica se a origem está correta, caso esteja, pede para tramitar*/
                if ($estadoAtual == ESDID_SITUACAO_PC_ADIMPLENTE) {
                    $registro = $v;
                    $registro['acao'] = $acao;
                    $registro['commit'] = true;
        
                    $tramite = $this->realizaTramiteInadimplencia($registro);
        
                    if ($tramite) {
                        $tramitou ++;
                        continue;
                    }
                } else {
                    continue;
                }
            }
        
            if ($tramitou > 0) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    public function realizaTramiteInadimplencia($dados)
    {
        
        $acao = $dados['acao'];
        $docid = $dados['docid'];
        $commit = $dados['commit'];
        /*Verifica se a ação é a esperada, caso seja processa tramitação e retorna resultado*/
        if (($acao == AEDID_OBRAS_OPC_DE_ADIMPLENTE_PARA_INADIMPLENTE) && (($acao != '') && ($docid != ''))) {
        
            $tramitacao = wf_alterarEstado($docid, $acao, "Colocando Obrigatoriedade de Prestar Contas como Inadimplente, após 15 dias da ciência do responsável.", array(), array(
                'commit' => $commit
            ));
        
            if ($tramitacao) {
                return true;
            }
        }
    }

    /**
     *
     * */
    public function tramitaProcessosSemSaldo($arrDocumento)
    {
        $docid = $arrDocumento['docid_opc'];
        $esdid = $arrDocumento['esdid_opc'];
        
        // Tramitar para Situação da OPC: Concluída
        // Trata de acordo com a origem da fase.        
        //2276;"Adimplente"
        if ($esdid == ESDID_SITUACAO_PC_ADIMPLENTE) {
            $acao = AEDID_OBR_OPC_ADIMPLENTE_P_CONCLUIDA; 
        }
        //2278;"Inadimplente"
        if ($esdid == ESDID_OBRAS_SITUACAO_OPC_INADIMPLENTE) {
            $acao = AEDID_OBR_OPC_INAADIMPLENTE_P_CONCLUIDA;
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
     * Funcao responsável por verificar se existem processos que ainda não estão com o fluxo, e os inserir
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *
     */
    public function cadastroProcessos($arrParams)
    {
    	//@todo
    	// Chama função do $this->model para verificar quais processos devem ser cadastrados
    	$arrReturn = $this->model->retornaProcessosSemFluxo($arrParams);
    	
    	// chama função do $this->model para inserir os registros e caso o retorno seja verdadeiro cadastroRegistro = true
    	$existe = $arrReturn["existeRegistro"];
    
    	if ($existe) {
    	    $arrRegistrosObrasPAR = $arrReturn['cadastroRegistroPAR'];
    	    $arrRegistrosObrasPAC = $arrReturn['cadastroRegistroPAC'];
    	
    	    $inseriuObrasPar = $this->model->insereRegistrosInexistenteObrasPar($arrRegistrosObrasPAR);
    	    $inseriuObrasPaC = $this->model->insereRegistrosInexistenteObrasPac($arrRegistrosObrasPAC);
    	      
    	    if($inseriuObrasPar && $inseriuObrasPaC)
    	    {
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
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['sopid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&sopid={$id}";
        $situacaoopcobras = new Obras2_Model_Situacaoopcobras($id);
        try{
             $situacaoopcobras->Xstatus = 'I';
             $situacaoopcobras->Xdtinativacao = date('Y-m-d H:i:s');
             $situacaoopcobras->Xusucpfinativacao = $_SESSION['usucpf'];

             $situacaoopcobras->salvar();
             $situacaoopcobras->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>