<?php

class Par3_Controller_IniciativaTiposObjetoController extends Controle
{
    private $model;
    private $modelHistorico;
    public function __construct()
    {
        $intoid = validarInteiro($_POST['intoid'])? $_POST['intoid']:'';
        $this->model = new Par3_Model_IniciativaTiposObjeto($intoid);
        $this->modelHistorico = new Par3_Model_IniciativaTiposObjetoHistorico();
    }

    public function form($arrPost)
    {
        global $simec;
        echo '<input type="hidden" name="intoid" value="'.$this->model->intoid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->intoid.'" class="form-control"></div>';
        echo '</div>';
        echo $simec->input('intodsc', 'Nome do Objeto', $this->model->intodsc, array('maxlength' => '255','required'),array('class' => 'col-lg-6'));
        echo $simec->radio('intosituacao', 'Situação',(!($this->model->intosituacao)?'A':$this->model->intosituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    public function salvar($arrPost)
    {
        $arrHistorico = Par3_Model_IniciativaTiposObjetoHistorico::CREATE;

        if( !empty($arrPost['intoid']) ) {
            $arrayIniDesc['intoid']          = $arrPost['intoid'];
            $arrHistorico = Par3_Model_IniciativaTiposObjetoHistorico::UPDATE;
        }else {
            $arrayIniDesc['intodtinclusao']  = date('Y-m-d H:m:s');
        }

        $arrayIniDesc['intosituacao']    = $arrPost['intosituacao'];
        $arrayIniDesc['intostatus']      = 'A';
        $arrayIniDesc['intodsc']         = trim(($arrPost['intodsc']));
        $arrayIniDesc['intocpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        //Valida campos
        $erro = $this->model->validarInput($arrayIniDesc);
        if($erro) return $erro;

        try {
            $this->model->popularDadosObjeto($arrayIniDesc);
            $id  = $this->model->salvar();
            $this->model->commit();

            $mInto = new Par3_Model_IniciativaTiposObjeto($id);
            $this->modelHistorico->salvarHistorico($mInto, $arrHistorico);

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($id) {
            return $id;
        }

        return 'erro';
    }

    public function remover($arrPost)
    {
        $mInto = new Par3_Model_IniciativaTiposObjeto($arrPost['intoid']);
        $arrPost['intostatus']        = 'I';
        $arrPost['intosituacao']      = 'I';
        $arrPost['intodtinativacao']  = date('Y-m-d H:m:s');
        $arrPost['intocpfinativacao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        try {
            $this->model->popularDadosObjeto($arrPost);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $this->modelHistorico->salvarHistorico($mInto,Par3_Model_IniciativaDescricaoHistorico::DELETE);
            return $mInto->intoid;
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function recuperar()
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        global $disabled;

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'intoid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'intodsc';
                break;
            case 'indsituacao':
                $arrQuery['ordenacao']['campo'] = 'intosituacao';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }

        $sql = $this->model->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Nome do Objeto','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
        }
        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setId('listTipoObjeto');
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('intosituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'remover');
            $listagem->addAcao('edit', 'editar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}