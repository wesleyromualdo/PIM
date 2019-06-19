<?php
/**
 * Classe de controle do  perfilusuario
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */



/**
 * Seguranca_Controller_Perfilusuario
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */
class Seguranca_Controller_Perfilusuario
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $perfilusuario = new Seguranca_Controller_Perfilusuario();
        $perfilusuario->popularDadosObjeto();
        $url .= '&pflcod=' . $perfilusuario->pflcod;

        try{
            $sucesso = $perfilusuario->salvar();
            $perfilusuario->commit();
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
        $id = $_GET['pflcod'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&pflcod={$id}";
        $perfilusuario = new Seguranca_Controller_Perfilusuario($id);
        try{
             $perfilusuario->Xstatus = 'I';
             $perfilusuario->Xdtinativacao = date('Y-m-d H:i:s');
             $perfilusuario->Xusucpfinativacao = $_SESSION['usucpf'];

             $perfilusuario->salvar();
             $perfilusuario->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>