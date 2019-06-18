<?php
/**
 * Classe de controle do  analise_engenharia_obra_documentos_respostas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-11-2017
 * @link     no link
 */



/**
 * Par3_Controller_Analise_engenharia_obra_documentos_respostas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-11-2017
 * @link     no link
 */
class Par3_Controller_AnaliseEngenhariaObraDocumentosRespostas extends Controle
{
    private $model;
    private $modelHistorico;

    public function __construct($obrid = null,$otdid = null)
    {
        $this->model = new Par3_Model_AnaliseEngenhariaObraDocumentosRespostas($aegid = null,$otdid = null);
        $this->modelHistorico = new Par3_Model_AnaliseEngenhariaObraDocumentosRespostasHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $acao = Par3_Model_AnaliseEngenhariaObraDocumentosRespostasHistorico::CREATE;
        if($arrPost['aedid']){
            $acao = Par3_Model_AnaliseEngenhariaObraDocumentosRespostasHistorico::UPDATE;
        }
        //        $this->model = new Par3_Controller_Analise_engenharia_obra_documentos_respostas();
        $dados['aedid']          = $arrPost['aedid'];
        $dados['aegid']          = $arrPost['aegid'];
        $dados['otdid']          = $arrPost['otdid'];
        $dados['aed_pendencia']  = $arrPost['aed_pendencia'] == null ?'N':$arrPost['aed_pendencia'];
        $dados['aed_observacao'] = $arrPost['aed_pendencia'] == null?' ' : $arrPost['aed_observacao'];
        try{
            $this->model->popularDadosObjeto($dados);
            $id = $this->model->salvar();
            $this->model->commit();

            $analise = new Par3_Model_AnaliseEngenhariaObraDocumentosRespostas($dados['aegid'], $dados['otdid']);

            $this->modelHistorico->gravarHistorico($analise,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return $e->getMessage();
        }
        $url =  "//{$_SERVER['HTTP_HOST']}{$_SERVER['REQUEST_URI']}";
        simec_redirecionar($url, 'success');
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['aedid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&aedid={$id}";
        $analise_engenharia_obra_documentos_respostas = new Par3_Model_Analise_engenharia_obra_documentos_respostas($id);
        try{
            $analise_engenharia_obra_documentos_respostas->Xstatus = 'I';
            $analise_engenharia_obra_documentos_respostas->Xdtinativacao = date('Y-m-d H:i:s');
            $analise_engenharia_obra_documentos_respostas->Xusucpfinativacao = $_SESSION['usucpf'];

            $analise_engenharia_obra_documentos_respostas->salvar();
            $analise_engenharia_obra_documentos_respostas->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function listarPendencias($obrid)
    {
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        $aegid = $mAnalise->iniciarAnalise($obrid);
        return $this->model->listarPendencias($aegid);
    }

}
?>