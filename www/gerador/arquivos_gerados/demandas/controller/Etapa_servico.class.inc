<?php
/**
 * Classe de controle do  etapa_servico
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   Adminstrador de sistema <simec@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-06-2019
 * @link     no link
 */



/**
 * EtapaServicoController_Etapa_servico
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-06-2019
 * @link     no link
 */
class EtapaServicoController_Etapa_servico
{
    private $model;

    public function __construct()
    {
        $this->model = new EtapaServicoModel_Etapa_servico($_GET['co_etapa_servico']);
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new EtapaServicoController_Etapa_servico();
        $this->model->popularDadosObjeto();
        $url .= '&co_etapa_servico=' . $this->model->co_etapa_servico;

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
        $id = $_GET['co_etapa_servico'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&co_etapa_servico={$id}";
        $etapa_servico = new EtapaServicoModel_Etapa_servico($id);
        try{
             $etapa_servico->Xstatus = 'I';
             $etapa_servico->Xdtinativacao = date('Y-m-d H:i:s');
             $etapa_servico->Xusucpfinativacao = $_SESSION['usucpf'];

             $etapa_servico->salvar();
             $etapa_servico->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>