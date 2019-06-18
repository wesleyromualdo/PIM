<?php
/**
 * Classe de controle do  obra_tipo_x_documento_apoi
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RENNER NASCENTES TANIZAKI <rennertanizaki@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Obra_tipo_x_documento_apoi
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipoXDocumentoApoi
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Obra_tipo_x_documento_apoi($_GET['']);
        $this->modelHistorico = new Par3_Model_Obra_tipo_x_documento_apoiHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_Obra_tipo_x_documento_apoi();
        $this->model->popularDadosObjeto();
        $url .= '&=' . $this->model->;

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
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['id'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&id={$id}";
        $obra_tipo_x_documento_apoi = new Par3_Model_Obra_tipo_x_documento_apoi($id);
        try{
             $obra_tipo_x_documento_apoi->Xstatus = 'I';
             $obra_tipo_x_documento_apoi->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_tipo_x_documento_apoi->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_tipo_x_documento_apoi->salvar();
             $obra_tipo_x_documento_apoi->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>