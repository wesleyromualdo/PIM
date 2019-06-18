<?php
/**
 * Classe de controle do  fabricante_veiculo
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */



/**
 * Par3_Controller_Fabricante_veiculo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */
class Par3_Controller_FabricanteVeiculo extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_FabricanteVeiculo($_GET['fabid']);
    }

    public function formFabricante()
    {
        global $simec;
        echo $simec->input('fabdsc', 'Nome do fabricante', '', array('maxlength' => '255', $disabled));
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {

        $arrDados = array();
        $arrDados['fabdsc']       = str_replace(array("'"), "", $_REQUEST['fabdsc']);
        $arrDados['fabdsc']       = ($arrDados['fabdsc']);
        $arrDados['fabstatus']    = 'A';
        $arrDados['usucpf']       = $_SESSION['usucpf'];

        $this->model->popularDadosObjeto($arrDados);
        $url = "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=".$arrPost['inuid'];
        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if($sucesso){
            return $sucesso;
        }
        return 'erro';
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['fabid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&fabid={$id}";
        $fabricante_veiculo = new Par3_Model_FabricanteVeiculo($id);
        try{
             $fabricante_veiculo->Xstatus = 'I';
             $fabricante_veiculo->Xdtinativacao = date('Y-m-d H:i:s');
             $fabricante_veiculo->Xusucpfinativacao = $_SESSION['usucpf'];

             $fabricante_veiculo->salvar();
             $fabricante_veiculo->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>