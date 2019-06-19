<?php
/**
 * Classe de controle do  omissaopccacs
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-01-2019
 * @link     no link
 */



/**
 * Par_Controller_Omissaopccacs
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-01-2019
 * @link     no link
 */
class Par_Controller_Omissaopccacs
{
    private $model;

    public function __construct()
    {
        $this->model = new Par_Model_Omissaopccacs($_GET['occid']);
        
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par_Controller_Omissaopccacs();
        $this->model->popularDadosObjeto();
        $url .= '&occid=' . $this->model->occid;

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

    /**
     */
    public function insereCienciaOmissao($arrParams = Array())
    {
        $insercao = $this->model->insereCienciaOmissao($arrParams);
        return $insercao;
    }

   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['occid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&occid={$id}";
        $omissaopccacs = new Par_Model_Omissaopccacs($id);
        try{
             $omissaopccacs->Xstatus = 'I';
             $omissaopccacs->Xdtinativacao = date('Y-m-d H:i:s');
             $omissaopccacs->Xusucpfinativacao = $_SESSION['usucpf'];

             $omissaopccacs->salvar();
             $omissaopccacs->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
    
    /**
     */
  /**
     */
    public function verificaExistenciaOmissaoNaoAssinada($inuid, $ropid)
    {
        // @todo verifica o efeito suspensivo, liberaEfeitoSuspensivo
        $efeitoSuspensivoOk = true;
        $omissao = $this->model->verificaExistenciaOmissaoNaoAssinada($inuid, $ropid);
        
        if ($omissao && $efeitoSuspensivoOk) {
            return true;
        } else {
            return false;
        }
    }
    
    /**
     */
    public function retornaListaProcessosComOmissao($inuid)
    {
        return $this->model->retornaListaProcessosComOmissao($inuid);
    }

}
?>