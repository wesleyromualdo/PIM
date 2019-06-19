<?php
/**
 * Classe de controle do  analise_engenharia_obra_aba_respostas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 07-11-2017
 * @link     no link
 */



/**
 * Par3_Controller_Analise_engenharia_obra_aba_respostas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 07-11-2017
 * @link     no link
 */
class Par3_Controller_AnaliseEngenhariaObraAbaRespostas extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($_POST['aerid']);
        $this->modelHistorico = new Par3_Model_AnaliseEngenhariaObraAbaRespostasHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $acao = Par3_Model_AnaliseEngenhariaObraAbaRespostasHistorico::CREATE;
        if($arrPost['aerid']){
            $acao = Par3_Model_AnaliseEngenhariaObraAbaRespostasHistorico::UPDATE;
        }

        $dados['aerid']          = $arrPost['aerid'];
        $dados['aegid']          = $arrPost['aegid'];
        $dados['aepid']          = $arrPost['aepid'];
        $dados['aer_pendencia']  = $arrPost['aer_pendencia'];
        $dados['aer_observacao'] = $arrPost['aer_pendencia'] == 'N' ? ' ' : ($arrPost['aer_observacao']);
        $dados['oabid'] = $arrPost['oabid'];


        if($dados['oabid'] == Par3_Model_ObraAba::ESTUDO_DEMANDA) {
            $dados['aer_observacao'] = ' ';
            $dadosObservacao['aed_observacao'] = ($arrPost['aer_observacao']);
        }

        try{
            if($dados['oabid'] == Par3_Model_ObraAba::ESTUDO_DEMANDA){
                $mEstudoDemandaObs = new Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacao();
                $observacao = $mEstudoDemandaObs->recuperarObservacao($dados['aegid']);

                $historicoObs = new Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacaoHistorico;
                $acaoObservacao = $historicoObs::CREATE;
                if($observacao['aedid']){
                    $acaoObservacao = $historicoObs::UPDATE;
                }

                $dadosObservacao['aegid']          = $dados['aegid'];
                $dadosObservacao['aedid']          = $observacao['aedid'];

                $mEstudoDemandaObs->popularDadosObjeto($dadosObservacao);
                $id = $mEstudoDemandaObs->salvar();
                $mEstudoDemandaObs->commit();

                $analise = new Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacao($id);
                $historicoObs->gravarHistorico($analise,$acao);
                $historicoObs->commit();
            }

            $this->model->popularDadosObjeto($dados);
            $id = $this->model->salvar();
            $this->model->commit();
            $analise = new Par3_Model_AnaliseEngenhariaObraAbaRespostas($id);
            $this->modelHistorico->gravarHistorico($analise,$acao);
            $this->modelHistorico->commit();


        } catch (Simec_Db_Exception $e) {
            return 0;
        }
        return $id;
    }
}
?>