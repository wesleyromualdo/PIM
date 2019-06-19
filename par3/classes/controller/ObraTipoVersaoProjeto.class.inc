<?php
/**
 * Classe de controle do  ObraTipoVersaoProjeto
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_ObraTipoVersaoProjeto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipoVersaoProjeto
{
    private $model;

    public function __construct()
    {
        $ovpid = validarInteiro($_REQUEST['ovpid'])? $_REQUEST['ovpid']:'';
        $this->model = new Par3_Model_ObraTipoVersaoProjeto($ovpid);
        $this->modelHistorico = new Par3_Model_ObraTipoVersaoProjetoHistorico();
    }

    public function pegarSelectCombo($otpid = null)
    {
        return $this->model->pegarSQLSelectCombo($otpid);
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function remover($arrPost)
    {
        $arrayDesdobramento['ovpid']             = $arrPost['ovpid'];
        $arrayDesdobramento['ovpstatus']         = 'I';
        $arrayDesdobramento['ovpsituacao']       = 'I';

        try {
            $this->model->popularDadosObjeto($arrayDesdobramento);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $this->model = new Par3_Model_ObraTipoVersaoProjeto($arrPost['ovpid']);

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
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'ovpid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'ovpdsc';
                break;
            case 'otpdsc':
                $arrQuery['ordenacao']['campo'] = 'otpdsc';
                break;
            case 'ovpsituacao':
                $arrQuery['ordenacao']['campo'] = 'ovpsituacao';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $this->model->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código','Versão Projeto','Tipo de Obra','Situação');

        $esconderColunas = array('id','qtd','ovpid');

        if($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['requisicao'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('versaoProjeto');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('ovpsituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'remover');
            $listagem->addAcao('plus', 'detalhar');
        }
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
        $acao = Par3_Model_ObraTipoVersaoProjeto::CREATE;
        if($arrPost['ovpid']){
            $arrayDesTipo['ovpid'] = $arrPost['ovpid'];
            $acao = Par3_Model_ObraTipoVersaoProjeto::UPDATE;
        }
        $arrayDesTipo['ovpsituacao']    = 'A';
        $arrayDesTipo['ovpstatus']      = 'A';
        $arrayDesTipo['otpid']          = $arrPost['otpid'];
        $arrayDesTipo['ovpdsc']         = trim(($arrPost['ovpdsc']));
        $arrayDesTipo['ovpmetragem']    = (float)$arrPost['ovpmetragem'] < 1 ? null:$arrPost['ovpmetragem'];
        //Valida campos
        $erro = $this->model->validarInput($arrayDesTipo);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrayDesTipo);
            $id =  $this->model->salvar();
            $this->model->commit();
            $modelVersaoProjeto= new Par3_Model_ObraTipoVersaoProjeto($id);
            $this->modelHistorico->gravarHistorico($modelVersaoProjeto,$acao);
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
        $id = $_GET['ovpid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ovpid={$id}";
        $ObraTipoVersaoProjeto = new Par3_Model_ObraTipoVersaoProjeto($id);
        try{
             $ObraTipoVersaoProjeto->Xstatus = 'I';
             $ObraTipoVersaoProjeto->Xdtinativacao = date('Y-m-d H:i:s');
             $ObraTipoVersaoProjeto->Xusucpfinativacao = $_SESSION['usucpf'];

             $ObraTipoVersaoProjeto->salvar();
             $ObraTipoVersaoProjeto->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>