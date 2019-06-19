<?php
/**
 * Classe de controle do  obra_aba
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Obra_aba
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-07-2017
 * @link     no link
 */
class Par3_Controller_ObraAba
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraAba($_GET['oabid']);
    }

    public function listar($arrPost)
    {
        global $disabled;

        $arrAbas    = $this->model->recuperaAbas($arrPost);
        $arrAbas     = is_array($arrAbas) ? $arrAbas : array();
        $arrAbaConf = array(4, 7);

        include APPRAIZ . 'par3/modulos/sistema/tabelaapoio/obras/configurar_aba/listaConfAba.inc';
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_ObraAba();
        $this->model->popularDadosObjeto();
        $url .= '&oabid=' . $this->model->oabid;

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
        $id = $_GET['oabid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&oabid={$id}";
        $obra_aba = new Par3_Model_ObraAba($id);
        try{
             $obra_aba->Xstatus = 'I';
             $obra_aba->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_aba->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_aba->salvar();
             $obra_aba->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>