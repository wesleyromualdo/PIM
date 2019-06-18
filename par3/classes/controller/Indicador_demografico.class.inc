<?php
/**
 * Classe de controle do  indicador_demografico
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   LEO KENZLEY BESERRA DE OLIVEIRA <leokenzley@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Indicador_demografico
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-07-2017
 * @link     no link
 */
class Par3_Controller_Indicador_demografico
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Indicador_demografico($_GET['inddid']);
        $this->modelHistorico = new Par3_Model_Indicador_demograficoHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_Indicador_demografico();
        $this->model->popularDadosObjeto();
        $url .= '&inddid=' . $this->model->inddid;

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
        $id = $_GET['inddid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&inddid={$id}";
        $indicador_demografico = new Par3_Model_Indicador_demografico($id);
        try{
             $indicador_demografico->Xstatus = 'I';
             $indicador_demografico->Xdtinativacao = date('Y-m-d H:i:s');
             $indicador_demografico->Xusucpfinativacao = $_SESSION['usucpf'];

             $indicador_demografico->salvar();
             $indicador_demografico->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>