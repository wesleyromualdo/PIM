<?php
/**
 * Classe de controle do  usuario
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */



/**
 * Seguranca_Controller_Usuario
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */
class Seguranca_Controller_Usuario
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $usuario = new Seguranca_Controller_Usuario();
        $usuario->popularDadosObjeto();
        $url .= '&usucpf=' . $usuario->usucpf;

        try{
            $sucesso = $usuario->salvar();
            $usuario->commit();
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
        $usuario = new Seguranca_Controller_Usuario($id);
        try{
             $usuario->Xstatus = 'I';
             $usuario->Xdtinativacao = date('Y-m-d H:i:s');
             $usuario->Xusucpfinativacao = $_SESSION['usucpf'];

             $usuario->salvar();
             $usuario->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>