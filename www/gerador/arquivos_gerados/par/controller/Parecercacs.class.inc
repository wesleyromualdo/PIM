<?php
/**
 * Classe de controle do  parecercacs
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-10-2016
 * @link     no link
 */



/**
 * Par_Controller_Parecercacs
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-10-2016
 * @link     no link
 */
class Par_Controller_Parecercacs
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $parecercacs = new Par_Controller_Parecercacs();
        $parecercacs->popularDadosObjeto();
        $url .= '&pcacsid=' . $parecercacs->pcacsid;

        try{
            $sucesso = $parecercacs->salvar();
            $parecercacs->commit();
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
        $id = $_GET['pcacsid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&pcacsid={$id}";
        $parecercacs = new Par_Controller_Parecercacs($id);
        try{
             $parecercacs->Xstatus = 'I';
             $parecercacs->Xdtinativacao = date('Y-m-d H:i:s');
             $parecercacs->Xusucpfinativacao = $_SESSION['usucpf'];

             $parecercacs->salvar();
             $parecercacs->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>