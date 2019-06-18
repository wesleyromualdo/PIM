<?php
/**
 * Classe de controle do  obra_planilha_orcamentaria
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RENNER NASCENTES TANIZAKI <rennertanizaki@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 07-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Obra_planilha_orcamentaria
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 07-07-2017
 * @link     no link
 */
class Par3_Controller_ObraPlanilhaOrcamentaria
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraPlanilhaOrcamentaria($_GET['opoid']);
        $this->modelHistorico = new Par3_Model_ObraPlanilhaOrcamentariaHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($dados)
    {
        try{
            $this->model->popularDadosObjeto($dados);
            $this->model->salvar();
            $this->model->commit();

            $this->model = new Par3_Model_ObraPlanilhaOrcamentaria($dados['opoid']);
            $this->modelHistorico->gravarHistorico($this->model, 'UPDATE');

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['opoid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&opoid={$id}";
        $obra_planilha_orcamentaria = new Par3_Model_Obra_planilha_orcamentaria($id);
        try{
             $obra_planilha_orcamentaria->Xstatus = 'I';
             $obra_planilha_orcamentaria->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_planilha_orcamentaria->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_planilha_orcamentaria->salvar();
             $obra_planilha_orcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function dadosIniciais($dados){
        $this->model->carregaValoresIniciais($dados);
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();

        $mObra = new Par3_Model_Obra($_GET['obrid']);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        if(in_array($esd['esdid'],$mAnalise->estadosCorrecaoAnalise())){
            $mAnalise->validarAnalisePlanilha($_GET['obrid']);
        }

        return $this->model->carregaValoresLista($dados);
    }

    public function valorTotalPlanilha($dados){
        $this->model->carregaValoresIniciais($dados);
        return $this->model->carregaTotalLista($dados);
    }





}