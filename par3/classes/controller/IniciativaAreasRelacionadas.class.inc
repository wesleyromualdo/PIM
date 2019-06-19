<?php
class Par3_Controller_IniciativaAreasRelacionadas extends Controle
{
    private $model;
    private $modelHistorico;

    public function __construct()
    {
        $this->model          = new Par3_Model_IniciativaAreasRelacionadas($_REQUEST['irrid']);
        $this->modelHistorico = new Par3_Model_IniciativaAreasRelacionadasHistorico();
    }

    public function recuperar()
    {
        return $this->model;
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaAreasRelacionadasHistorico::CREATE;
        if($arrPost['irrid']){
            $arrAreaRelacionada['irrid'] = $arrPost['irrid'];
            $acao = Par3_Model_IniciativaAreasRelacionadasHistorico::UPDATE;
        }

        $arrAreaRelacionada['irrsituacao']    = $arrPost['irrsituacao'];
        $arrAreaRelacionada['irrstatus']      = 'A';
        $arrAreaRelacionada['iuoid']          = trim(($arrPost['iuoid']));
        $arrAreaRelacionada['iarid']          = trim(($arrPost['iarid']));
        $arrAreaRelacionada['irrcpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);

        if(!$arrPost['irrid']){$arrAreaRelacionada['irrdtinclusao'] = date('Y-m-d H:m:s');}

        //Valida campos
        $erro = $this->model->validarInput($arrAreaRelacionada);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrAreaRelacionada);
            $id =  $this->model->salvar();
            $this->model->commit();

            $mItem = new $this->model($id);

            $this->modelHistorico->gravarHistorico($mItem,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function listar($arrPost)
    {
        global $disabled;
        $modelAreas = new Par3_Model_IniciativaAreasRelacionadas();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'DESC'? 'DESC':'ASC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = '1';
                break;
            case 'unidadegestora':
                $arrQuery['ordenacao']['campo'] = '2';
                break;
            case 'arearelacionada':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'programas':
                $arrQuery['ordenacao']['campo'] = '4';
                break;
            case 'situacao':
                $arrQuery['ordenacao']['campo'] = '5';
                break;
            default:
                break;
        }

        $sql = $this->model->dataGridSQL($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código','Unidade Gestora','Área Gestora','Programa','Situação');

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
        $listagem->setId('areasrelaciondadastable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('situacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerAreaRelaciondada');
            $listagem->addAcao('edit', 'editarAreaRelacionada');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}