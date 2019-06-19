<?php
/**
 * Classe de controle do  agrupador
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   LEO KENZLEY BESERRA DE OLIVEIRA <leokenzley@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-10-2017
 * @link     no link
 */



/**
 * Par3_Controller_Agrupador
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-10-2017
 * @link     no link
 */
class Par3_Controller_Agrupador extends Controle
{
    private $model;
    private $modelAgruparObra;

    public function __construct()
    {
        $this->model = new Par3_Model_Agrupador($_GET['agrid']);
        $this->modelHistorico = new Par3_Model_AgrupadorHistorico();
        $this->modelAgruparObra = new Par3_Model_AgrupadorObra();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {

        global $url;
//        $this->model = new Par3_Controller_Agrupador();
        $this->model->popularDadosObjeto();
        $url .= '&menu=agrupador&agrid=' . $this->model->agrid;
        //historico
        //$acao = (isset($this->model->agrid) && $this->model->agrid != null && $this->model->agrid != '')? Par3_Model_AgrupadorHistorico::UPDATE : Par3_Model_AgrupadorHistorico::CREATE;

        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
            if($sucesso){
                $this->modelAgruparObra->popularDadosObjeto();
                $this->modelAgruparObra->agrid = $sucesso;
                $this->modelAgruparObra->salvar();
                $this->modelAgruparObra->commit();
            }
            return $sucesso;
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
        $id = $_GET['agrid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&agrid={$id}";
        $agrupador = new Par3_Model_Agrupador($id);
        try{
             $agrupador->Xstatus = 'I';
             $agrupador->Xdtinativacao = date('Y-m-d H:i:s');
             $agrupador->Xusucpfinativacao = $_SESSION['usucpf'];

             $agrupador->salvar();
             $agrupador->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function isValid($action = 'insert'){
        if($action ==  'insert'){
            $isValidPreId = ($_POST['preid'] != null && $_POST['preid'] != '')? true:false;
            if($isValidPreId == false){
                return false;
            }
            $isValidAgrdesc = ($_POST['agrdesc'] != null && $_POST['preid'] != '')? true:false;
            if($isValidAgrdesc == false){
                return false;
            }
        }

        return true;
    }

    public function fetchAllList(){
        return $this->model->fetchAllList();
    }

    public function create($id){
        if(isset($id) && $id != ''){
            return $this->model->create($id);
        }
    }

    public function inativaAgrupador($id){
        if(isset($id) && $id != ''){
            $this->model->inativaAgrupador($id);
        }
    }

    public function validaDescricaoAgrupador($agrdesc){
            return $this->model->validaDescricaoAgrupador($agrdesc);
    }

    public function getListSelectFormAgrupador(){
        return $this->model->getListSelectForm();
    }
}
?>