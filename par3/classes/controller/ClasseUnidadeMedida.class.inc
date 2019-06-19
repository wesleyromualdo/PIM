<?php
/**
 * Classe de controle do  unidade_medida
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-05-2017
 * @link     no link
 */

/**
 * Par3_Controller_ClasseUnidadeMedida
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-05-2017
 * @link     no link
 */
class Par3_Controller_ClasseUnidadeMedida extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ClasseUnidadeMedida($_GET['cniid']);
        $this->modelHistorico = new Par3_Model_ClasseUnidadeMedidaHistorico();
    }

    public function formClasseUnidadeMedida($arrPost)
    {
        global $simec;
        echo '<input type="hidden" name="cniid" value="'.$this->model->cniid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->cniid.'" class="form-control"></div>';
        echo '</div>';
        echo $simec->input('cnidsc', 'Descrição', $this->model->cnidsc, array('maxlength' => '255','required'));
    }

    public function salvar($arrPost)
    {
        //define acão para guardar em histórico
        $acao = Par3_Model_ClasseUnidadeMedidaHistorico::CREATE;
        if($arrPost['cniid']){
            $arrayUniMedida['cniid'] = trim($arrPost['cniid']);
            $acao = Par3_Model_ClasseUnidadeMedidaHistorico::UPDATE;
        }

        $arrayClassUniMedida['cnisituacao']    = $arrPost['cnisituacao'];
        $arrayClassUniMedida['cnistatus']      = 'A';
        $arrayClassUniMedida['cnisituacao']    = 'A';
        $arrayClassUniMedida['cnidsc']         = trim(($arrPost['cnidsc']));
        $arrayClassUniMedida['cnicpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        if(empty($arrPost['cniid'])){$arrayClassUniMedida['cnidtinclusao']  = date('Y-m-d H:m:s');}//só grava data de inclusão ao criar

        //Valida campos
        $erro = $this->model->validarInput($arrayClassUniMedida);
        if($erro){return $erro;}

        //validar antes de realizar operação

        try {
            $this->model->popularDadosObjeto($arrayClassUniMedida);
            $novoClassUnidadeMedida =  $this->model->salvar();//grava Unidade de Medida
            $this->model->commit();

            $modelClassUnMedida = new Par3_Model_ClasseUnidadeMedida($novoClassUnidadeMedida);//recupera Unidade de Medida

            $modelHistorico = new Par3_Model_ClasseUnidadeMedidaHistorico();
            $modelHistorico->gravarHistorico($modelClassUnMedida,$acao);//grava em hitórico
            $modelHistorico->salvar();
            $modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($novoClassUnidadeMedida) {
            return $novoClassUnidadeMedida;
        }
        return 'erro';
    }

    public function recuperar()
    {
        return $this->model;
    }

    public function remover($arrPost)
    {
        $mUnidadeMedidaClasse = new  Par3_Model_UnidadeMedidaClasse();
        $rsUnc = $mUnidadeMedidaClasse->recuperarTodos('*',["cniid = {$arrPost['cniid']}","t2.unistatus = 'A'","uncstatus = 'A'"],null,array('join'=>'uniid'));
        if(!empty($rsUnc)){
            $erros['erro'] = 'Esta Classe Está Vinculada a Unidades de Medida';
            return $erros;
            ;
        }

        $modelClassUnMedida = new Par3_Model_ClasseUnidadeMedida($arrPost['cniid']);
        try {
            $modelClassUnMedida->cnistatus = 'I';
            $modelClassUnMedida->salvar();
            $modelClassUnMedida->commit();
            $this->modelHistorico->gravarHistorico($modelClassUnMedida, 'DELETE');
            return $arrPost['cniid'];
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function listar($arrPost)
    {
        global $disabled;
        $modelCiclo = new Par3_Model_ClasseUnidadeMedida();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){//filtros de ordenação da listagem
            case 'codigo':      $arrQuery['ordenacao']['campo'] = 'cniid';break;
            case 'descricao':   $arrQuery['ordenacao']['campo'] = 'cnidsc';break;
            case 'cnisituacao': $arrQuery['ordenacao']['campo'] = 'cnisituacao';break;
            default:            $arrQuery['ordenacao'] = '';break;
        }

        $sql = $modelCiclo->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Classe Unidade de Medida','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('unidadeMedida');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('cnisituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerClasse');
            $listagem->addAcao('edit', 'editarClasse');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}
?>