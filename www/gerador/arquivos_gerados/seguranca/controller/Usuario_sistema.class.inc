<?php
/**
 * Classe de controle do  usuario_sistema
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */



/**
 * Seguranca_Controller_Usuario_sistema
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */
class Seguranca_Controller_Usuario_sistema
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $usuario_sistema = new Seguranca_Controller_Usuario_sistema();
        $usuario_sistema->popularDadosObjeto();
        $url .= '&usucpf=' . $usuario_sistema->usucpf;

        try{
            $sucesso = $usuario_sistema->salvar();
            $usuario_sistema->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }//end salvar()


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['usucpf'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&usucpf={$id}";
        $usuario_sistema = new Seguranca_Controller_Usuario_sistema($id);
        try{
             $usuario_sistema->Xstatus = 'I';
             $usuario_sistema->Xdtinativacao = date('Y-m-d H:i:s');
             $usuario_sistema->Xusucpfinativacao = $_SESSION['usucpf'];

             $usuario_sistema->salvar();
             $usuario_sistema->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>