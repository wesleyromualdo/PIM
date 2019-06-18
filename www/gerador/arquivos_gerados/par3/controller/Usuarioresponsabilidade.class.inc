<?php
/**
 * Classe de controle do  usuarioresponsabilidade
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-09-2016
 * @link     no link
 */



/**
 * Par3_Controller_Usuarioresponsabilidade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-09-2016
 * @link     no link
 */
class Par3_Controller_Usuarioresponsabilidade
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $usuarioresponsabilidade = new Par3_Controller_Usuarioresponsabilidade();
        $usuarioresponsabilidade->popularDadosObjeto();
        $url .= '&rpuid=' . $usuarioresponsabilidade->rpuid;

        try{
            $sucesso = $usuarioresponsabilidade->salvar();
            $usuarioresponsabilidade->commit();
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
        $id = $_GET['rpuid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&rpuid={$id}";
        $usuarioresponsabilidade = new Par3_Controller_Usuarioresponsabilidade($id);
        try{
             $usuarioresponsabilidade->Xstatus = 'I';
             $usuarioresponsabilidade->Xdtinativacao = date('Y-m-d H:i:s');
             $usuarioresponsabilidade->Xusucpfinativacao = $_SESSION['usucpf'];

             $usuarioresponsabilidade->salvar();
             $usuarioresponsabilidade->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>