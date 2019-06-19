<?php
/**
 * Classe de controle do  iniciativa_indicador_demografico_indicador_status
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   LEO KENZLEY BESERRA DE OLIVEIRA <leokenzley@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Iniciativa_indicador_demografico_indicador_status
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-07-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIndicadorDemograficoIndicadorStatus
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIndicadorDemograficoIndicadorStatus($_GET['iniindsid']);
        $this->modelHistorico = new Par3_Model_Iniciativa_indicador_demografico_indicador_statusHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_Iniciativa_indicador_demografico_indicador_status();
        $this->model->popularDadosObjeto();
        $url .= '&iniindsid=' . $this->model->iniindsid;

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
        $id = $_GET['iniindsid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iniindsid={$id}";
        $iniciativa_indicador_demografico_indicador_status = new Par3_Model_Iniciativa_indicador_demografico_indicador_status($id);
        try{
             $iniciativa_indicador_demografico_indicador_status->Xstatus = 'I';
             $iniciativa_indicador_demografico_indicador_status->Xdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_indicador_demografico_indicador_status->Xusucpfinativacao = $_SESSION['usucpf'];

             $iniciativa_indicador_demografico_indicador_status->salvar();
             $iniciativa_indicador_demografico_indicador_status->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>