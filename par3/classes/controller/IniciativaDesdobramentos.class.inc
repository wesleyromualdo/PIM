<?php

class Par3_Controller_IniciativaDesdobramentos extends Controle
{
    public function __construct()
    {
        $desid = validarInteiro($_REQUEST['desid'])? $_REQUEST['desid']:'';
        $this->model          = new Par3_Model_IniciativaDesdobramentos($desid);
        $this->modelHistorico = new Par3_Model_IniciativaDesdobramentosHistorico();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaDesdobramentosHistorico::CREATE;
        if($arrPost['desid']){
            $arrayDesdobramento['desid'] = $arrPost['desid'];
            $acao = Par3_Model_IniciativaDesdobramentosHistorico::UPDATE;
        }

        $arrayDesdobramento['tipid']          = $arrPost['tipid'];
        $arrayDesdobramento['dessituacao']    = $arrPost['dessituacao'];
        $arrayDesdobramento['desstatus']      = 'A';
        $arrayDesdobramento['desdsc']         = trim(($arrPost['desdsc']));
        $arrayDesdobramento['descpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        $arrayDesdobramento['desdtinclusao']  = date('Y-m-d H:m:s');
        //Valida campos
        $erro = $this->model->validarInput($arrayDesdobramento);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrayDesdobramento);
            $id =  $this->model->salvar();
            $this->model->commit();
            $modelDes = new Par3_Model_IniciativaDesdobramentos($id);
            $this->modelHistorico->gravarHistorico($modelDes,$acao);
            $this->modelHistorico->commit();
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
        if($this->model->desimutavel == 't'){
            return 'Este Desdobramento Não pode ser removido pois está vinculado a outros registros do Sistema';
        }

        $arrayDesdobramento['desid']             = $arrPost['desid'];
        $arrayDesdobramento['desstatus']         = 'I';
        $arrayDesdobramento['dessituacao']       = 'I';
        $arrayDesdobramento['desdtinativacao']   = date('Y-m-d H:m:s');
        $arrayDesdobramento['descpfinativacao']  = $_SESSION['usucpf'];
        try {
            $this->model->popularDadosObjeto($arrayDesdobramento);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelDesdobramento = new Par3_Model_IniciativaDesdobramentos($arrPost['desid']);

            $this->modelHistorico->gravarHistorico($modelDesdobramento,Par3_Model_IniciativaDesdobramentosHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'Ocorreu um erro';
    }

    public function listaIniciativaDesdobramentos($arrPost)
    {
        global $disabled;
        $modelDesdobramentos = new Par3_Model_IniciativaDesdobramentos();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['tipid'] = $arrPost['tipodesc'];
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'desid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'desdsc';
                break;
            case 'tipdsc':
                $arrQuery['ordenacao']['campo'] = 'tipdsc';
                break;
            case 'dessituacao':
                $arrQuery['ordenacao']['campo'] = 'dessituacao';
                break;
            default:
                $arrQuery['ordenacao']['campo'] = 'desid';
                break;
        }

        $sql = $modelDesdobramentos->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código','Tipo','Desdobramento','Situação');

        $esconderColunas = array('id','qtd','desid','imutavel');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id','imutavel');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('desdobramentos');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('dessituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverDesdobramento');
            $listagem->addAcao('edit', 'editarDesdobramento');
            $listagem->setAcaoComoCondicional('remove', [['campo' => 'imutavel', 'valor' => 'f', 'op' => 'igual']]);
            $listagem->setAcaoComoCondicional('edit', [['campo' => 'imutavel', 'valor' => 'f', 'op' => 'igual']]);
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function getIniciativaDesdobramentoById($desid){
        $result = $this->model->getIniciativaDesdobramentoById($desid);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($desid){
        $result = $this->model->verificaLigacaoPlanejamentoById($desid);
        return $result;
    }
}