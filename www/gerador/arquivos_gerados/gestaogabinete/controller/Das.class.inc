<?php
/**
 * Classe de controle do  das
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio@teste.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-11-2016
 * @link     no link
 */



/**
 * Gestaogabinete_Controller_Das
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-11-2016
 * @link     no link
 */
class Gestaogabinete_Controller_Das
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $das = new Gestaogabinete_Controller_Das();
        $das->popularDadosObjeto();
        $url .= '&dasid=' . $das->dasid;

        try{
            $sucesso = $das->salvar();
            $das->commit();
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
        $id = $_GET['dasid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&dasid={$id}";
        $das = new Gestaogabinete_Controller_Das($id);
        try{
             $das->Xstatus = 'I';
             $das->Xdtinativacao = date('Y-m-d H:i:s');
             $das->Xusucpfinativacao = $_SESSION['usucpf'];

             $das->salvar();
             $das->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end inativar()



}//end Class
?>