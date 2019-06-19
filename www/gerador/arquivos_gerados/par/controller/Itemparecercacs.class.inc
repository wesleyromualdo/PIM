<?php
/**
 * Classe de controle do  itemparecercacs
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-10-2016
 * @link     no link
 */



/**
 * Par_Controller_Itemparecercacs
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-10-2016
 * @link     no link
 */
class Par_Controller_Itemparecercacs
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $itemparecercacs = new Par_Controller_Itemparecercacs();
        $itemparecercacs->popularDadosObjeto();
        $url .= '&ipcacsid=' . $itemparecercacs->ipcacsid;

        try{
            $sucesso = $itemparecercacs->salvar();
            $itemparecercacs->commit();
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
        $id = $_GET['ipcacsid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipcacsid={$id}";
        $itemparecercacs = new Par_Controller_Itemparecercacs($id);
        try{
             $itemparecercacs->Xstatus = 'I';
             $itemparecercacs->Xdtinativacao = date('Y-m-d H:i:s');
             $itemparecercacs->Xusucpfinativacao = $_SESSION['usucpf'];

             $itemparecercacs->salvar();
             $itemparecercacs->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>