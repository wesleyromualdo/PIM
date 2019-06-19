<?php

/**
 * Classe de controle do  situacaoopc
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-07-2018
 * @link     no link
 */

/**
 * Par_Controller_Situacaoopc
 *
 * @category Class
 * @package A1
 * @author <>
 * @license GNU simec.mec.gov.br
 * @version Release: 04-07-2018
 * @link no link
 */
class Par_Controller_Situacaoopc
{

    private $model;

    public function __construct()
    {
        $this->model = new Par_Model_Situacaoopc($_GET['sopid']);
    }
    
    public function tramitaProcessosSemSaldo($arrDocumento)
    {
        // Tramitar para "Registro de Execução"
        $docid = $arrDocumento['docid_opc'];
        $esdid = $arrDocumento['esdid_opc'];
        
        // Enviar para Concluída
        // "Adimplente"
        if ($esdid == ESDID_OPC_ADIMPLENTE) {
            $acao = AEDID_OPC_ADIMPLENTE_P_CONCLUIDA;
        }
        //@todo add situação inadimplente
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
     * Função gravar
     * - grava os dados
     */
    
    /**
     * Funcao responsável por verificar se existem processos que ainda não estão com o fluxo, e os inserir
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
      
    public function tramitaInadimplenciaPar($processosPar)
    {
        /*Pega os registros para tramitar*/
        $processosPar = (is_array($processosPar)) ? $processosPar : Array();
        $tramitou = 0;
        /*Caso tenha registro faz tramitação, senão retorna falso*/
        if (count($processosPar) > 0) {
            foreach ($processosPar as $k => $v) {
                
                $estadoAtual = '';
                $estadoAtual = $v['situacao_opc'];
                $acao = AEDID_OPC_DE_ADIMPLENTE_PARA_INADIMPLENTE;
                if ($estadoAtual == ESDID_OPC_ADIMPLENTE) {
                /*Verifica se a origem está correta, caso esteja, pede para tramitar*/
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
    
    
    public function realizaTramiteInadimplencia($dados){
        
        $acao = $dados['acao'];
        $docid = $dados['docid'];
        $commit = $dados['commit'];
        /*Verifica se a ação é a esperada, caso seja processa tramitação e retorna resultado*/
        if (($acao == AEDID_OPC_DE_ADIMPLENTE_PARA_INADIMPLENTE) && (($acao != '') && ($docid != ''))) {
        
            $tramitacao = wf_alterarEstado($docid, $acao, "Colocando Obrigatoriedade de Prestar Contas como Inadimplente, após 15 dias da ciência do responsável.", array(), array(
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
    public function tramitaFluxoWF($arrParams)
    {
        /* Recupera parâmetros do array */
        $acao = $arrParams['aedid'];
        $destino = $arrParams['destino'];
        $origem = $arrParams['origem'];
        $docid = $arrParams['docid'];
        $commit = $arrParams['commit'];
        
        if (($acao == AEDID_SPC_NOTIFICAR_POR_OMISSAO) && (($acao != '') && ($docid != ''))) {
            
            $tramitacao = wf_alterarEstado($docid, $acao, "Notificando a entidade por omissão após 60 dias.", array(), array(
                'commit' => $commit
            ));
            
            if ($tramitacao) {
                return true;
            }
        }
    }
    /**
     * Funcao responsável por realizar a ação de enviar a situação a ""
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
    
        if (($acao == AEDID_SPC_NOTIFICAR_CACS_POR_OMISSAO) && (($acao != '') && ($docid != ''))) {
    
            $tramitacao = wf_alterarEstado($docid, $acao, "Notificando o CACS por omissão após 60 dias.", array(), array(
                'commit' => $commit
            ));
    
            if ($tramitacao) {
                return true;
            }
        }
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
    public function registraOmissaoEnte($arrParams = Array())
    {
        if (count($arrParams) > 0) {
            // Cria objeto de Omissao
            $objOmissao = new Par_Model_Omissaopcente();
            $id = $objOmissao->insereRegistro($arrParams);
            return $id;
        }else{
         return false;  
       }
    }
    
    /**
     * Funcao responsável por registrar a Omissao do CACS
     * m
     *
     * @param
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'cadastroRegistro' => retorna bool caso a ocorra corretamente)
     *
     */
    public function registraOmissaoCACS($arrParams = Array())
    {
        if (count($arrParams) > 0) {
            // Cria objeto de Omissao
            $objOmissao = new Par_Model_Omissaopcente();
            $id = $objOmissao->insereRegistroOmissaoCacs($arrParams);
            return $id;
        }else{
            return false;
        }
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

    public function salvar()
    {
        global $url;
        // $this->model = new Par_Controller_Situacaoopc();
        $this->model->popularDadosObjeto();
        $url .= '&sopid=' . $this->model->sopid;
        
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
        $id = $_GET['sopid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&sopid={$id}";
        $situacaoopc = new Par_Model_Situacaoopc($id);
        try {
            $situacaoopc->Xstatus = 'I';
            $situacaoopc->Xdtinativacao = date('Y-m-d H:i:s');
            $situacaoopc->Xusucpfinativacao = $_SESSION['usucpf'];
            
            $situacaoopc->salvar();
            $situacaoopc->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
}
?>