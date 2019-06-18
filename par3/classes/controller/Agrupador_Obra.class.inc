<?php
/**
 * Classe de controle do  agrupador_obra
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   LEO KENZLEY BESERRA DE OLIVEIRA <leokenzley@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-10-2017
 * @link     no link
 */



/**
 * Par3_Controller_Agrupador_obra
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-10-2017
 * @link     no link
 */
class Par3_Controller_Agrupador_obra
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Agrupador_obra($_GET['agobid']);
        $this->modelHistorico = new Par3_Model_Agrupador_obraHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_Agrupador_obra();
        $this->model->popularDadosObjeto();
        $url .= '&agobid=' . $this->model->agobid;

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
        $id = $_GET['agobid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&agobid={$id}";
        $agrupador_obra = new Par3_Model_Agrupador_obra($id);
        try{
             $agrupador_obra->Xstatus = 'I';
             $agrupador_obra->Xdtinativacao = date('Y-m-d H:i:s');
             $agrupador_obra->Xusucpfinativacao = $_SESSION['usucpf'];

             $agrupador_obra->salvar();
             $agrupador_obra->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>