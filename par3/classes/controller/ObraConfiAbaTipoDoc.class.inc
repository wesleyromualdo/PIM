<?php
/**
 * Classe de controle do  obra_confi_aba_x_tipo_doc
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_ObraConfiAbaTipoDoc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */
class Par3_Controller_ObraConfiAbaTipoDoc
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraConfiAbaTipoDoc($_REQUEST['octid']);
        $this->modelHistorico = new Par3_Model_ObraConfiAbaTipoDocHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($dados)
    {
        global $url;

        $this->model->inativarTodos($dados['ocaid']);

        if(is_array($dados['otdid'])){
            foreach($dados['otdid'] as $otdid){

                $acao = Par3_Model_ObraConfiAbaTipoDocHistorico::CREATE;
                if($arrPost['octid']){
                    $arraSalvar['octid'] = $arrPost['octid'];
                    $acao = Par3_Model_ObraConfiAbaTipoDocHistorico::UPDATE;
                }

                $arraSalvar['otdid']    = $otdid;
                $arraSalvar['ocaid']    = $dados['ocaid'];
                $arraSalvar['ocstatus'] = 'A';
                $arraSalvar['octordem'] = trim($dados['octordem'][$otdid]);

                //Valida campos
                $erro = $this->model->validarInput($arraSalvar);
                if($erro) return $erro;

                try {
                    $this->model = new Par3_Model_ObraConfiAbaTipoDoc();
                    $this->model->popularDadosObjeto($arraSalvar);
                    $id =  $this->model->salvar();
                    $this->model->commit();

                    $model = new Par3_Model_ObraConfiAbaTipoDoc($id);
                    $this->modelHistorico->gravarHistorico($model,$acao);
                    $this->modelHistorico->commit();

                } catch (Simec_Db_Exception $e) {
                    return 'erro';
                }
            }
        }else{
        	return array('erros'=>array('otdid'=>'O campo não pode ser vazio'));
        }

        if ($id) {
            return 'sucesso';
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
        $id = $_GET['octid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&octid={$id}";
        $obra_confi_aba_x_tipo_doc = new Par3_Model_ObraConfiAbaTipoDoc($id);
        try{
             $obra_confi_aba_x_tipo_doc->Xstatus = 'I';
             $obra_confi_aba_x_tipo_doc->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_confi_aba_x_tipo_doc->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_confi_aba_x_tipo_doc->salvar();
             $obra_confi_aba_x_tipo_doc->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>