<?php
/**
 * Classe de controle do  ObraTipoItemPlanilha
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_ObraTipoItemPlanilha
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipoItemPlanilha
{
    private $model;

    public function __construct()
    {
        $this->model            = new Par3_Model_ObraTipoItemPlanilha($_GET['oipid']);
        $this->modelHistorico   = new Par3_Model_ObraTipoItemPlanilhaHistorico();
    }

    public function lista($arrPost)
    {
        global $disabled;

        $arrQuery = $arrPost;
        $mVersaoPlanilha = new Par3_Model_ObraTipoVersaoPlanilha($arrPost['tplid']);

        $sql = $this->model->montarSQLSimples($arrQuery);

        $cabecalho = array('Código','Item','Unidade de Medida','Situação', 'Quantidade', 'Quantidade Editável?', 'Valor Unitário', 'Valor Editável?');
        if($mVersaoPlanilha->tpltipo_regiao != 'N') $cabecalho = array('Código','Item','Unidade de Medida','Situação');

        $esconderColunas = array('id','qtd','tplid');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio          = ($arrPost['req'] == 'xls')                                    ? Simec_Listagem::RELATORIO_XLS          : Simec_Listagem::RELATORIO_PAGINADO;
        $tratamentoListaVazia   = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir')   ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('versaoPlanilha');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('oipstatus','formata_status');
        if($disabled == '' && $mVersaoPlanilha->tpltipo_regiao != 'N'){
            $listagem->addAcao('plus', 'listarValorItens');
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
        global $url;

        if($arrPost['tpltipo_regiao'] == 'N'){
            unset($dados);
            $chave = 1;
            if(!(strpos($_FILES['arqid_1']['name'], '.csv') > 0)) simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cad_planilha_orcamentaria&otpid='.$_REQUEST['otpid'], 'error', 'Arquivo inválido.');
            $dados = file($_FILES['arqid_1']['tmp_name']);

            if(is_array($dados)) $this->model->salvarItens($arrPost, $dados);
        }else{
            foreach($arrPost['estuf'] as $k => $estuf){
                unset($dados);
                $chave = $k;
                if(!(strpos($_FILES['arqid_'.$k]['name'], '.csv') > 0)) simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cad_planilha_orcamentaria&otpid='.$_REQUEST['otpid'], 'error', 'Arquivo inválido.');
                $dados = file($_FILES['arqid_'.$k]['tmp_name']);

                if(is_array($dados)) $this->model->salvarItens($arrPost, $dados, $arrPost['estuf'][$k], $arrPost['muncod'][$k]);
            }
        }
    }
}
?>