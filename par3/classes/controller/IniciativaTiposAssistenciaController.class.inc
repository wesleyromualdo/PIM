<?php

class Par3_Controller_IniciativaTiposAssistenciaController extends Controle
{
    public function __construct()
    {
        $intaid = validarInteiro($_POST['intaid'])? $_POST['intaid']:'';
        $this->model = new Par3_Model_IniciativaTiposAssistencia($intaid);
    }

    public function form($arrPost)
    {
        global $simec;
        echo '<input type="hidden" name="intaid" value="'.$this->model->intaid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->intaid.'" class="form-control"></div>';
        echo '</div>';
        echo $simec->input('intadsc', 'Tipo de Assistência', $this->model->intadsc, array('maxlength' => '255','required'));
        echo $simec->radio('intasituacao', 'Situação',(!($this->model->intasituacao)?'A':$this->model->intasituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    public function salvar($arrPost)
    {
        $arrIniDesHistorico['intahacao'] = Par3_Model_IniciativaTiposAssistenciaHistorico::CREATE;

        if( !empty($arrPost['intaid']) ) {
            $arrayIniDesc['intaid']          = $arrPost['intaid'];
            $arrIniDesHistorico['intahacao'] = Par3_Model_IniciativaTiposAssistenciaHistorico::UPDATE;
        }else {
            $arrayIniDesc['intadtinclusao']  = date('Y-m-d H:m:s');
        }

        $arrayIniDesc['intasituacao']    = $arrPost['intasituacao'];
        $arrayIniDesc['intastatus']      = 'A';
        $arrayIniDesc['intadsc']         = trim(($arrPost['intadsc']));
        $arrayIniDesc['intacpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        //Valida campos
        $erro = $this->model->validarInput($arrayIniDesc);
        if($erro) return $erro;

        try {
            $this->model->popularDadosObjeto($arrayIniDesc);
            $id  = $this->model->salvar();
            $this->model->commit();

            $this->salvarHistorico($id, $arrIniDesHistorico);
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
        $arrayIniDesc['intaid']            = $arrPost['intaid'];
        $arrayIniDesc['intastatus']        = 'I';
        $arrayIniDesc['intasituacao']       = 'I';
        $arrayIniDesc['intadtinativacao']  = date('Y-m-d H:m:s');
        $arrayIniDesc['intacpfinativacao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        try {
            $this->model->popularDadosObjeto($arrayIniDesc);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $arrIniDesHistorico['intahacao'] = Par3_Model_IniciativaTiposAssistenciaHistorico::DELETE;
            $this->salvarHistorico($arrPost['intaid'], $arrIniDesHistorico);

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
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
                $arrQuery['ordenacao']['campo'] = 'intaid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'intadsc';
                break;
            case 'indsituacao':
                $arrQuery['ordenacao']['campo'] = 'intasituacao';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }

        $sql = $this->model->montarSQLSimples($arrQuery);
        
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Tipo de Assistência','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
        }
        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setId('listTipoAssistencia');
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('intasituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverTipoAssitencia');
            $listagem->addAcao('edit', 'editar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    private function salvarHistorico($id, $status){
        $modelIniDesc = new Par3_Model_IniciativaTiposAssistencia($id);

        $arrIniDesHistorico['intahdt']           = date('Y-m-d H:m:s');
        $arrIniDesHistorico['intahcpf']          = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        $arrIniDesHistorico['intaid']            = $modelIniDesc->intaid;
        $arrIniDesHistorico['intadsc']           = $modelIniDesc->intadsc;
        $arrIniDesHistorico['intastatus']        = $modelIniDesc->intastatus;
        $arrIniDesHistorico['intasituacao']      = $modelIniDesc->intasituacao;
        $arrIniDesHistorico['intadtinclusao']    = $modelIniDesc->intadtinclusao;
        $arrIniDesHistorico['intacpfinclusao']   = $modelIniDesc->intacpfinclusao;
        $arrIniDesHistorico['intadtinativacao']  = $modelIniDesc->intadtinativacao;
        $arrIniDesHistorico['intacpfinativacao'] = $modelIniDesc->intacpfinativacao;
        $arrIniDesHistorico = array_merge($arrIniDesHistorico, $status);

        $modelIniDescHistorico = new Par3_Model_IniciativaTiposAssistenciaHistorico();
        $modelIniDescHistorico->popularDadosObjeto($arrIniDesHistorico);
        $modelIniDescHistorico->salvar();
        $modelIniDescHistorico->commit();
    }

//     public function verificaLigacaoPlanejamento($intaid){
//         $result = $this->model->verificaLigacaoPlanejamento($intaid);
//         return $result;
//     }
//     public function getTiposAtendimentoById($intaid){
//         $result = $this->model->getTiposAtendimentoById($intaid);
//         return $result;
//     }
}