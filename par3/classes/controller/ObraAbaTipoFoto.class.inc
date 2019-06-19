<?php
/**
 * Classe de controle do  obra_aba_tipo_foto
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Obra_aba_tipo_foto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-07-2017
 * @link     no link
 */
class Par3_Controller_ObraAbaTipoFoto
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraAbaTipoFoto($_GET['atfid']);
        $this->modelHistorico = new Par3_Model_ObraAbaTipoFotoHistorico();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $acao = Par3_Model_ObraAbaTipoFotoHistorico::CREATE;
        if($arrPost['atfid']){
            $arrSalvar['atfid'] = $arrPost['atfid'];
            $acao = Par3_Model_ObraAbaTipoFotoHistorico::UPDATE;
        }

        $arrSalvar['atfstatus']  = 'A';
        $arrSalvar['atfdsc']     = trim(($arrPost['atfdsc']));
        //Valida campos
        $erro = $this->model->validarInput($arrSalvar);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrSalvar);
            $id =  $this->model->salvar();
            $this->model->commit();
            $model = new Par3_Model_ObraAbaTipoFoto($id);
            $this->modelHistorico->gravarHistorico($model,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($id) {
            return $id;
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
        $id = $_GET['atfid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&atfid={$id}";
        $obra_aba_tipo_foto = new Par3_Model_ObraAbaTipoFoto($id);
        try{
             $obra_aba_tipo_foto->Xstatus = 'I';
             $obra_aba_tipo_foto->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_aba_tipo_foto->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_aba_tipo_foto->salvar();
             $obra_aba_tipo_foto->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>