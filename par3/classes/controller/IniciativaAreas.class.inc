<?php
class Par3_Controller_IniciativaAreas extends Controle
{
    public function __construct()
    {
        $this->model          = new Par3_Model_IniciativaAreas($_REQUEST['iarid']);
        $this->modelHistorico = new Par3_Model_IniciativaAreasHistorico();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function salvar($arrPost)
    {
       $acao = Par3_Model_IniciativaAreasHistorico::CREATE;
       if($arrPost['iarid']){
           $arrayArea['iarid'] = $arrPost['iarid'];
           $acao = Par3_Model_IniciativaAreasHistorico::UPDATE;
       }

        $arrayArea['iuoid']          = $arrPost['iuoid'];
        $arrayArea['co_unidade_org'] = $arrPost['co_unidade_org'];
        $arrayArea['iarsigla']       = $arrPost['iarsigla'];
        $arrayArea['iarid_pai']      = $arrPost['iarid_pai'];
        $arrayArea['iarsituacao']    = $arrPost['iarsituacao'];
        $arrayArea['iarstatus']      = 'A';
        $arrayArea['iardsc']         = trim(($arrPost['iardsc']));
        //Valida campos
        $erro = $this->model->validarInput($arrayArea);
        if($erro) return $erro;
        try {
            $this->model->popularDadosObjeto($arrayArea);
            $id =  $this->model->salvar(true,true,array('iarid_pai'));

            $this->model->commit();
            $model = new Par3_Model_IniciativaAreas($id);
            $this->modelHistorico->gravarHistorico($model,$acao);
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
        $arrayArea['iarid']             = $arrPost['iarid'];
        $arrayArea['iarstatus']         = 'I';
        $arrayArea['iarsituacao']       = 'I';
        $arrayArea['iardtinativacao']   = date('Y-m-d H:m:s');
        $arrayArea['iarcpfinativacao']  = $_SESSION['usucpf'];
        $arrayArea['iarsigla']          = $arrPost['iarsigla'];

        $mIap = new Par3_Model_IniciativaAreasRelacionadasProgramas();
        if(!empty($mIap->recuperarPorArea($arrayArea['iarid']))){
            $erros = array();
            return 'A área está relacionada a um programa';
        }
        try {
            $this->model->popularDadosObjeto($arrayArea);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelAreas = new Par3_Model_IniciativaAreas($arrPost['iarid']);

           $this->modelHistorico->gravarHistorico($modelAreas,Par3_Model_IniciativaAreasHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        $erro = array();
        return "Ocorreu um erro ao remover a Area.";
    }

    public function listaIniciativaAreas($arrPost)
    {
        global $disabled;
        $modelAreas = new Par3_Model_IniciativaAreas();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'DESC'? 'DESC':'ASC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'iuodsc':
                $arrQuery['ordenacao']['campo'] = '4';
                break;
            case 'iarsituacao':
                $arrQuery['ordenacao']['campo'] = '6';
                break;
            default:
                $arrQuery['ordenacao']['campo'] = 'iardsc';
                break;
        }

        $sql = $modelAreas->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código','Área','Sigla','Unidade Orçamentária','Situação');

        $esconderColunas = array('id','qtd','iarid');

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
        $listagem->setId('areas');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('iarsituacao','formata_status');
        $listagem->setTamanhoPagina(50);
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverArea');
            $listagem->addAcao('edit', 'editarArea');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function pegarSQLSelectComboSigla()
    {
        return $this->model->pegarSQLSelectComboSigla();
    }


    public function getIniciativaAreaById($iarid){
        $result = $this->model->getIniciativaAreaById($iarid);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($iarid){
        $result = $this->model->verificaLigacaoPlanejamentoById($iarid);
        return $result;
    }
}