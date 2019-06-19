<?php
/**
 * Classe de controle do  modelo_veiculo
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */



/**
 * Par3_Controller_Modelo_veiculo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */
class Par3_Controller_ModeloVeiculo extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ModeloVeiculo($_GET['mveid']);
    }

    public function formModelo($arrPost)
    {
        $fabricante = new Par3_Model_FabricanteVeiculo();
        $rsFabricantes = $fabricante->pegarSQLSelectListagemCombo($arrPost);

        global $simec;
        echo $simec->select('fabid', 'Fabricante',"",$rsFabricantes);
        echo $simec->input('mvedsc', 'Nome Modelo', '', array('maxlength' => '255', $disabled));
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $this->model->popularDadosObjeto($arrPost);
//        $url .= '&mveid=' . $this->model->mveid;

        $arrPost['usucpf']      = $_SESSION['usucpf'];
        $arrPost['mvestatus']   = 'A';
        $arrPost['mvedsc']      = ($arrPost['mvedsc']);
        $arrPost['mvedtcricao'] = date('Y-m-d H:m:s');
        $this->model->popularDadosObjeto($arrPost);
        try{
            $id = $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            return false;
        }
        if($sucesso){
            return $id;
        }
        return false;
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['mveid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&mveid={$id}";
        $modelo_veiculo = new Par3_Model_ModeloVeiculo($id);
        try{
             $modelo_veiculo->Xstatus = 'I';
             $modelo_veiculo->Xdtinativacao = date('Y-m-d H:i:s');
             $modelo_veiculo->Xusucpfinativacao = $_SESSION['usucpf'];

             $modelo_veiculo->salvar();
             $modelo_veiculo->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>