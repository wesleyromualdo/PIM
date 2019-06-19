<?php
/**
 * Classe de controle do  ObraTipoItemPlanilhaValores
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_ObraTipoItemPlanilhaValores
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipoItemPlanilhaValores
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraTipoItemPlanilhaValores($_GET['oivid']);
        $this->modelHistorico = new Par3_Model_ObraTipoItemPlanilhaValoresHistorico();
    }

    public function lista($arrPost)
    {
        global $disabled;

        $arrQuery = $arrPost;

        $sql = $this->model->montarSQLSimples($arrQuery);

        $cabecalho = array('Código','Unidade','Quantidade','É editável?', 'Valor Unitário','É editável?');

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
        $listagem->addCallbackDeCampo('oivqtd','formata_numero_monetario');
        $listagem->addCallbackDeCampo('oivvalor','formata_numero_monetario');
        $listagem->turnOnOrdenacao();
        $listagem->setTamanhoPagina(1000000);
        $listagem->render($tratamentoListaVazia);
    }

    public function salvarValoresItem($oipid, $oivqtd, $oivqtd_editavel, $oivvalor, $oivvalor_editavel, $estufs = array(), $muncods = array())
    {
        global $url;

        $sql = '';
        if($estufs[1] != '' || $muncods[1] != ''){
            $arr    = $muncods[1] != '' ? $muncods  : $estufs;
            $itrid  = $muncods[1] != '' ? '='       : '!=';
            $campo  = $muncods[1] != '' ? 'muncod'  : 'estuf';

            foreach($arr as $valor){
                if($valor != ''){
                    $mdUnidade = new Par3_Model_InstrumentoUnidade();
                    $arrUnidade = $mdUnidade->recuperarTodos('inuid', array("$campo = '$valor'", "itrid $itrid 2"));
                    $inuid = $arrUnidade[0]['inuid'];

                    if($inuid != ''){
                        $sql .= $this->retornaSQLValorItem($oipid, $inuid, $oivqtd, $oivqtd_editavel, $oivvalor, $oivvalor_editavel);
                    }
                }
            }
        }else{
            $sql .= $this->retornaSQLValorItem($oipid, 'null', $oivqtd, $oivqtd_editavel, $oivvalor, $oivvalor_editavel);
        }

        if($sql != ''){
            $this->model->executar($sql);
            $this->model->commit();
        }
    }

    public function retornaSQLValorItem($oipid, $inuid, $oivqtd, $oivqtd_editavel, $oivvalor, $oivvalor_editavel)
    {
        global $url;

        $sql = "INSERT INTO par3.obra_tipo_item_planilha_valores(oipid, inuid, oivqtd, oivqtd_editavel, oivvalor, oivvalor_editavel, oivstatus)
                VALUES ($oipid, $inuid, $oivqtd, '$oivqtd_editavel', $oivvalor, '$oivvalor_editavel', 'A');";

        return $sql;
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['oivid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&oivid={$id}";
        $ObraTipoItemPlanilhaValores = new Par3_Model_ObraTipoItemPlanilhaValores($id);
        try{
             $ObraTipoItemPlanilhaValores->Xstatus = 'I';
             $ObraTipoItemPlanilhaValores->Xdtinativacao = date('Y-m-d H:i:s');
             $ObraTipoItemPlanilhaValores->Xusucpfinativacao = $_SESSION['usucpf'];

             $ObraTipoItemPlanilhaValores->salvar();
             $ObraTipoItemPlanilhaValores->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>