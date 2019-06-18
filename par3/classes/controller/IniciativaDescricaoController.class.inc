<?php

/**
 * User: danielfiuza
 * Date: 27/03/17
 * Time: 15:13
 */
class Par3_Controller_IniciativaDescricaoController extends Controle
{
    public function __construct()
    {
        $indid = validarInteiro($_POST['indid'])? $_POST['indid']:'';
        $this->model = new Par3_Model_IniciativaDescricao($indid);
        $this->modelIniciativaPlanejamento = new Par3_Model_IniciativaPlanejamento();
    }

    public function formIniciativaDescricao($arrPost)
    {
        global $simec;
        echo '<input type="hidden" name="indid" value="'.$this->model->indid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->indid.'" class="form-control"></div>';
        echo '</div>';
        echo $simec->input('inddsc', 'Descrição', $this->model->inddsc, array('maxlength' => '255','required'));
//        if ($arrPost['indid']){
            echo $simec->radio('indsituacao', 'Situação',(!($this->model->indsituacao)?'A':$this->model->indsituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
//        }
    }

    public function salvar($arrPost)
    {
        $arrIniDesHistorico['inhacao']   = Par3_Model_IniciativaDescricaoHistorico::CREATE;
        if($arrPost['indid']){
            $arrayIniDesc['indid']         = trim($arrPost['indid']);
            $arrIniDesHistorico['inhacao'] = Par3_Model_IniciativaDescricaoHistorico::UPDATE;
        }

        $arrayIniDesc['indsituacao']      = $arrPost['indsituacao'];
        $arrayIniDesc['indstatus']    = 'A';
        $arrayIniDesc['inddsc']         = trim(($arrPost['inddsc']));
        $arrayIniDesc['indcpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);

        if(!$arrPost['indid']){$arrayIniDesc['inddtinclusao']  = date('Y-m-d H:m:s');}

        //Valida campos
        $erro = $this->model->validarInput($arrayIniDesc);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayIniDesc);
            $novoind =  $this->model->salvar();
            $this->model->commit();

            $modelIniDesc = new Par3_Model_IniciativaDescricao($novoind);

            //grava informações no histórico
            //grava no histórico
            $arrIniDesHistorico['indid']            = $modelIniDesc->indid;
            $arrIniDesHistorico['inhdt']            = date('Y-m-d H:m:s');
            $arrIniDesHistorico['inhcpf']           = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
            $arrIniDesHistorico['inddsc']           = $modelIniDesc->inddsc;
            $arrIniDesHistorico['indsituacao']      = $modelIniDesc->indsituacao;
            $arrIniDesHistorico['indstatus']        = $modelIniDesc->indstatus;
            $arrIniDesHistorico['inddtinativacao']  = $modelIniDesc->inddtinativacao;
            $arrIniDesHistorico['indcpfinativacao'] = $modelIniDesc->indcpfinativacao;
            $arrIniDesHistorico['inddtinclusao']    = $modelIniDesc->inddtinclusao;
            $arrIniDesHistorico['indcpfinclusao']   = $modelIniDesc->indcpfinclusao;


            $modelIniDescHistorico = new Par3_Model_IniciativaDescricaoHistorico();
            $modelIniDescHistorico->popularDadosObjeto($arrIniDesHistorico);
            $modelIniDescHistorico->salvar();
            $modelIniDescHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($novoind) {
            return $novoind;
        }
        return 'erro';
    }

    public function desvincular($arrPost)
    {
        $arrayIniDesc['indid']           = $arrPost['indid'];
        $arrayIniDesc['indsituacao']     = 'I';
        $arrayIniDesc['indstatus']       = 'I';
        $arrayIniDesc['inddtinativacao'] = date('Y-m-d H:m:s');

        try {

            $this->model->popularDadosObjeto($arrayIniDesc);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelIniDesc = new Par3_Model_IniciativaDescricao($arrPost['indid']);
            //grava no histórico
            $arrIniDesHistorico['indid']            = $modelIniDesc->indid;
            $arrIniDesHistorico['inhdt']            = date('Y-m-d H:m:s');
            $arrIniDesHistorico['inhcpf']           = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
            $arrIniDesHistorico['inhacao']          = Par3_Model_IniciativaDescricaoHistorico::UPDATE;
            $arrIniDesHistorico['inddsc']           = $modelIniDesc->inddsc;
            $arrIniDesHistorico['indstatus']        = $modelIniDesc->indstatus;
            $arrIniDesHistorico['indsituacao']      = $modelIniDesc->indsituacao;
            $arrIniDesHistorico['inddtinativacao']  = $modelIniDesc->inddtinativacao;
            $arrIniDesHistorico['indcpfinativacao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
            $arrIniDesHistorico['inddtinclusao']    = $modelIniDesc->inddtinclusao;
            $arrIniDesHistorico['indcpfinclusao']   = $modelIniDesc->indcpfinclusao;
            $listagem->setTamanhoPagina(50);
            $modelIniDescHistorico = new Par3_Model_IniciativaDescricaoHistorico();
            $modelIniDescHistorico->popularDadosObjeto($arrIniDesHistorico);
            $modelIniDescHistorico->salvar();
            $modelIniDescHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayIniDesc['indid']             = $arrPost['indid'];
        $arrayIniDesc['indstatus']         = 'I';
        $arrayIniDesc['inddtinativacao'] = date('Y-m-d H:m:s');

        try {

            $this->model->popularDadosObjeto($arrayIniDesc);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelIniDesc = new Par3_Model_IniciativaDescricao($arrPost['indid']);
            //grava no histórico
            $arrIniDesHistorico['indid']            = $modelIniDesc->indid;
            $arrIniDesHistorico['inhdt']            = date('Y-m-d H:m:s');
            $arrIniDesHistorico['inhcpf']           = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
            $arrIniDesHistorico['inhacao']          = Par3_Model_IniciativaDescricaoHistorico::DELETE;
            $arrIniDesHistorico['inddsc']           = $modelIniDesc->inddsc;
            $arrIniDesHistorico['indstatus']        = $modelIniDesc->indstatus;
            $arrIniDesHistorico['indsituacao']      = $modelIniDesc->indsituacao;
            $arrIniDesHistorico['inddtinativacao']  = $modelIniDesc->inddtinativacao;
            $arrIniDesHistorico['indcpfinativacao'] = $modelIniDesc->indcpfinativacao;
            $arrIniDesHistorico['inddtinclusao']    = $modelIniDesc->inddtinclusao;
            $arrIniDesHistorico['indcpfinclusao']   = $modelIniDesc->indcpfinclusao;

            $modelIniDescHistorico = new Par3_Model_IniciativaDescricaoHistorico();
            $modelIniDescHistorico->popularDadosObjeto($arrIniDesHistorico);
            $modelIniDescHistorico->salvar();
            $modelIniDescHistorico->commit();
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

    public function listaIniciativaDescricao($arrPost)
    {
        global $disabled;
        $modelIniciativa = new Par3_Model_IniciativaDescricao();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'indid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'inddsc';
                break;
            case 'indsituacao':
                $arrQuery['ordenacao']['campo'] = 'indsituacao';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }

        $sql = $modelIniciativa->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Descrição','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }
        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('iniciativaDesc');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('indsituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaIniciativaPlanejamentoExclusao');
            $listagem->addAcao('edit', 'editarIniciativaDescricao');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function getIniciativaDescricaoById($indid){
        return $this->modelIniciativaPlanejamento->getIniciativaPlanejamentoByIdIniciativaDescricao($indid);
    }
}