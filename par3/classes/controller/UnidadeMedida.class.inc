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
class Par3_Controller_UnidadeMedida extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_UnidadeMedida($_GET['uniid']);
        $this->modelHistorico = new Par3_Model_UnidadeMedidaHistorico();
    }

    public function formUnidadeMedida($arrPost)
    {
        global $simec;
        if($this->model->uniid){
            echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
            echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->uniid.'" class="form-control"></div>';
            echo '</div>';
        }
        echo '<input type="hidden" name="uniid" value="'.$this->model->uniid.'"/>';
        echo $simec->input('unidsc', 'Descrição', $this->model->unidsc,array('required'=>'required'));
    }

    public function salvar($arrPost)
    {
        //define acão para guardar em histórico
        $acao = Par3_Model_UnidadeMedidaHistorico::CREATE;
        if($arrPost['uniid']){
            $arrayUniMedida['uniid'] = trim($arrPost['uniid']);
            $acao = Par3_Model_UnidadeMedidaHistorico::UPDATE;
        }

        $arrayUniMedida['unisituacao']    = $arrPost['unisituacao'];
        $arrayUniMedida['unistatus']      = 'A';
        $arrayUniMedida['unisituacao']    = 'A';
        $arrayUniMedida['unidsc']         = trim(($arrPost['unidsc']));
        $arrayUniMedida['unicpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        if(empty($arrPost['uniid'])){$arrayUniMedida['unidtinclusao']  = date('Y-m-d H:m:s');}//só grava data de inclusão ao criar

        //Valida campos
        $erro = $this->model->validarInput($arrayUniMedida);
        if($erro){return $erro;}

        //validar antes de realizar operação

        try {
            $this->model->popularDadosObjeto($arrayUniMedida);
            $novoUnidadeMedida =  $this->model->salvar();//grava Unidade de Medida
            $this->model->commit();

            $modelUnMedida = new Par3_Model_UnidadeMedida($novoUnidadeMedida);//recupera Unidade de Medida

            $modelHistorico = new Par3_Model_UnidadeMedidaHistorico();
            $modelHistorico->gravarHistorico($modelUnMedida,$acao);//grava em hitórico
            $modelHistorico->salvar();
            $modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($novoUnidadeMedida) {
            return $novoUnidadeMedida;
        }
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayUniMedida['uniid']            = $arrPost['uniid'];
        $arrayUniMedida['unistatus']        = 'I';
        $arrayUniMedida['unisituacao']      = 'I';
        $arrayUniMedida['unidtinativacao']  = date('Y-m-d H:m:s');
        $arrayUniMedida['unicpfinativacao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        try {
            $this->model->popularDadosObjeto($arrayUniMedida);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $arrUnidadeMedidaHistorico['hunacao'] = Par3_Model_UnidadeMedidaHistorico::DELETE;
            $this->modelHistorico->gravarHistorico($arrPost['uniid'], $arrUnidadeMedidaHistorico);

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        global $disabled;
        $modelCiclo = new Par3_Model_UnidadeMedida();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){//filtros de ordenação da listagem
            case 'codigo':      $arrQuery['ordenacao']['campo'] = 'uniid';break;
            case 'descricao':   $arrQuery['ordenacao']['campo'] = 'unidsc';break;
            case 'unisituacao': $arrQuery['ordenacao']['campo'] = 'unisituacao';break;
            default:            $arrQuery['ordenacao'] = '';break;
        }

        $sql = $modelCiclo->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $mClasseUnidadeMedida = new Par3_Model_ClasseUnidadeMedida();
        $listaClasses = $mClasseUnidadeMedida->listarClassesDescricao();


        $cabecalho = array('Código', 'Descrição','Situação');
        if(count($listaClasses) > 0 && $arrPost['requisicao'] != 'xls' && $arrPost['requisicao'] != 'imprimir'){$cabecalho[] = 'Todos';}
        foreach($listaClasses as $class){
            $cabecalho[] = $class;
        }//Adiciona a descrição das classes  ao cabeçalho da listagem


        $esconderColunas = array('id','qtd');

        if($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('unidadeMedida');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);

        $listagem->addCallbackDeCampo('unisituacao','formata_status');

        if($disabled == ''){
            $listagem->addAcao('remove', 'removerUnidadeMedida');
            $listagem->addAcao('edit', 'editarUnidadeMedida');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}
?>