<?php
/**
 * Classe de controle do  ObraTipoVersaoPlanilha
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_ObraTipoVersaoPlanilha
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipoVersaoPlanilha
{
    private $model;

    public function __construct()
    {
        $tplid = validarInteiro($_REQUEST['tplid'])? $_REQUEST['tplid']:'';
        $this->model            = new Par3_Model_ObraTipoVersaoPlanilha($tplid);
        $this->modelHistorico   = new Par3_Model_ObraTipoVersaoPlanilhaHistorico();
    }

    public function pegarSelectCombo($ovpid = null)
    {
//         ver($ovpid, d);
        return $this->model->pegarSQLSelectCombo($ovpid);
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        return $this->model->validarInput($campos);
    }

    public function remover($arrPost)
    {
        $arrayDesdobramento['tplid']             = $arrPost['tplid'];
        $arrayDesdobramento['tplstatus']         = 'I';
        $arrayDesdobramento['tplsituacao']       = 'I';

        try {
            $this->model->popularDadosObjeto($arrayDesdobramento);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $this->model = new Par3_Model_ObraTipoVersaoProjeto($arrPost['tplid']);

            $this->modelHistorico->gravarHistorico($this->model,Par3_Model_ObraTipoVersaoProjetoHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function lista($arrPost)
    {
        global $disabled;

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;

        $sql = $this->model->montarSQLSimples($arrQuery);

        $cabecalho = array('Código','Versão Planilha','Situação','Tipo Região');

        $esconderColunas = array('id','qtd','tplid');

        if($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio          = ($arrPost['requisicao'] == 'xls')                                    ? Simec_Listagem::RELATORIO_XLS          : Simec_Listagem::RELATORIO_PAGINADO;
        $tratamentoListaVazia   = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir')   ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        if($arrPost['requisicao'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('versaoPlanilha');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('tplstatus','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerPlanilha');
            $listagem->addAcao('plus', 'listarItens');
        }
        $listagem->setTamanhoPagina(1000000);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $acao = Par3_Model_ObraTipoVersaoPlanilha::CREATE;
        if($arrPost['tplid']){
            $arrayDesTipo['tplid'] = $arrPost['tplid'];
            $acao = Par3_Model_ObraTipoVersaoPlanilha::UPDATE;
        }
        $arrayDesTipo['tplsituacao']    = 'A';
        $arrayDesTipo['tplstatus']      = 'A';
        $arrayDesTipo['ovpid']          = $arrPost['ovpid_salvar'];
        $arrayDesTipo['tpltipo_regiao'] = $arrPost['tpltipo_regiao'];
//         $arrayDesTipo['tpldsc']         = trim(($arrPost['tpldsc']));
        $arrayDesTipo['tpldsc']         = trim($arrPost['tpldsc']);
        //Valida campos
        $erro = $this->model->validarInput($arrayDesTipo);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrayDesTipo);
            $id =  $this->model->salvar();
            $this->model->commit();
            $modelVersaoPlanilha= new Par3_Model_ObraTipoVersaoPlanilha($id);
            $this->modelHistorico->gravarHistorico($modelVersaoPlanilha,$acao);
            $this->modelHistorico->commit();

            $arrPost['tplid'] = $id;
            $ctlItem = new Par3_Controller_ObraTipoItemPlanilha();
            $ctlItem->salvar($arrPost);

        } catch (Simec_Db_Exception $e) {
            return 'erro';
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
        $id = $_GET['tplid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&tplid={$id}";
        $ObraTipoVersaoPlanilha = new Par3_Model_ObraTipoVersaoPlanilha($id);
        try{
             $ObraTipoVersaoPlanilha->Xstatus = 'I';
             $ObraTipoVersaoPlanilha->Xdtinativacao = date('Y-m-d H:i:s');
             $ObraTipoVersaoPlanilha->Xusucpfinativacao = $_SESSION['usucpf'];

             $ObraTipoVersaoPlanilha->salvar();
             $ObraTipoVersaoPlanilha->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>