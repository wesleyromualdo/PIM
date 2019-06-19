<?php

class Par3_Controller_ObraFotosTerreno
{
    public $model;
    public $modelHistorico;
    public $modelObraAbaTipoFoto;

    public function __construct($oftid)
    {
        $this->model = new Par3_Model_ObraFotosTerreno($oftid);
        $this->modelHistorico = new Par3_Model_ObraFotosTerrenoHistorico();
        $this->modelObraAbaTipoFoto = new Par3_Model_ObraAbaTipoFoto();
    }

    public function salvar($arrPost)
    {
        $url = '';
        if(isset($_REQUEST['toReturn'])){
            $url = "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=cadastro_foto_terreno&inuid={$arrPost['inuid']}&inpid={$arrPost['inpid']}&obrid={$arrPost['obrid']}&toReturn={$_REQUEST['toReturn']}";
        }else{
            $url = "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=cadastro_foto_terreno&inuid={$arrPost['inuid']}&inpid={$arrPost['inpid']}&obrid={$arrPost['obrid']}";
        }

        $mdObra             = new Par3_Model_Obra($arrPost['obrid']);
        $modelAnaliseEngenharia = new Par3_Model_AnaliseEngenhariaObra();
        $modelAnaliseEngenharia->retirarPendencia($_REQUEST['obrid'], Par3_Model_ObraAba::FOTOS_TERRENO, 1);

        $mdConfiguracaoAba  = new Par3_Model_ObraConfiguracaoAba();
        $arrConf            = $mdConfiguracaoAba->recuperarTodos('*', array("otpid = ".$mdObra->otpid, "ocastatus = 'A'", "oabid = 4"), 'oabid');
        $conf               = $arrConf[0];

        if ($_FILES['arqid']['name']) {
            if($_FILES['arqid']['size'] > ($conf['ocatamanho_maximo']*1000000)){
                simec_redirecionar($url, 'error', "A foto excede o tamanho máximo de {$conf['ocatamanho_maximo']}MB.");
                die;
            }else{
                $file = new FilesSimec("documentoarquivo", [], 'par3');

                $file->setUpload(null, 'arqid', false);
                $arqid = $file->getIdArquivo();
            }
        }

        $arrayDadosHistorico = array();
        $arrayDadosHistorico['hofacao'] = Par3_Model_ObraFotosTerrenoHistorico::CREATE;
        if ($arrPost['oftid']) {
            $arrayDados['oftid'] = (int)$arrPost['oftid'];
            $arrayDadosHistorico['hofacao'] = Par3_Model_ObraFotosTerrenoHistorico::UPDATE;
        }
        $arrayDados['obrid'] = $arrPost['obrid'];
        $arrayDados['arqid'] = $arqid;
        $arrayDados['oftstatus'] = 'A';
        $arrayDados['oftdsc'] = trim(($arrPost['oftdsc']));
        $arrayDados['otfid'] = (int)$arrPost['otfid'];
        $arrayDados['otfcpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        if (!$arrPost['oftid']) {
            $arrayDados['otfdtinclusao'] = date('Y-m-d H:m:s');
        }

        try {

            $this->model->popularDadosObjeto($arrayDados);
            $novooda = $this->model->salvar();
            $this->model->commit();

            $modelo = new Par3_Model_ObraFotosTerreno($novooda);

            //grava informações no histórico
            //grava no histórico
            $arrayDadosHistorico['oftid'] = $modelo->oftid;
            $arrayDadosHistorico['hofdtcriacao'] = date('Y-m-d H:m:s');
            $arrayDadosHistorico['hofcpf'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
            $arrayDadosHistorico['oftdsc'] = $modelo->oftdsc;
            $arrayDadosHistorico['otfid'] = (int)$modelo->otfid;
            $arrayDadosHistorico['oftstatus'] = 'A';
            $arrayDadosHistorico['obrid'] = $arrPost['obrid'];

            $this->modelHistorico->popularDadosObjeto($arrayDadosHistorico);
            $this->modelHistorico->salvar();
            $this->modelHistorico->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($novooda) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'success');
    }

    public function inativar()
    {
        global $url;
        $id = $_GET['oftid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&oftid={$id}";
        $obra_fotos_terreno = new Par3_Model_Obra_fotos_terreno($id);
        try {
            $obra_fotos_terreno->Xstatus = 'I';
            $obra_fotos_terreno->Xdtinativacao = date('Y-m-d H:i:s');
            $obra_fotos_terreno->Xusucpfinativacao = $_SESSION['usucpf'];

            $obra_fotos_terreno->salvar();
            $obra_fotos_terreno->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function getComboTipoFoto($obrid)
    {
        $mdObra = new Par3_Model_Obra($obrid);
        return $this->modelObraAbaTipoFoto->getComboTipoFoto($mdObra->otpid);
    }

    public function retornaQtdFotos($arrPost)
    {
        $sql = $this->getSqlLista($arrPost);

        $affFotos = $this->model->carregar($sql);

        return count($affFotos);
    }

    public function listar($arrPost)
    {
        global $disabled;

//        $disabled = 'disabled';
//        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $sql = $this->getSqlLista($arrPost);

        $cabecalho = array('Código', 'Descrição', 'Arquivo', 'Inserido por:', 'Data da Criação', 'Situação');
        $esconderColunas = array('id', 'qtd', 'usunome', 'hoddtcriacaoinativacao', 'arqid');

        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('obraDocumento');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('odasituacao', 'formata_status_inativo');
        $listagem->addCallbackDeCampo('hoddtcriacao', 'formata_data_hora');
        $listagem->addCallbackDeCampo('hodcpf', 'concatenaCpfNome');
        $listagem->addCallbackDeCampo('arquivo', 'linkArquivo');

        if ($disabled == '') {
            $listagem->addAcao('remove', 'remover');
            $listagem->addAcao('edit', 'editar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function getSqlLista($arrPost)
    {
        global $disabled;
        $model = new Par3_Model_ObraFotosTerreno();

        $campo = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC' ? 'ASC' : 'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch ($campo) {
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'of.oftid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'of.odadsc';
                break;
            case 'oftstatus':
                $arrQuery['ordenacao']['campo'] = 'of.oftstatus';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        return $model->montarSQLSimples($arrQuery);
    }

    public function listaImagens($arrPost)
    {
        global $disabled;

//        $disabled = 'disabled';
//        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $sql = $this->getSqlLista($arrPost);
        $this->model->gerarListaFoto($sql);

    }

    public function remover($arrPost)
    {
        $arrayDados['oftid'] = $arrPost['oftid'];
        $arrayDados['oftstatus'] = 'I';
        $url = "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=cadastro_foto_terreno&inuid={$arrPost['inuid']}&inpid={$arrPost['inpid']}&obrid={$arrPost['obrid']}";
        try {

            $modelAnaliseEngenharia = new Par3_Model_AnaliseEngenhariaObra();
            $modelAnaliseEngenharia->retirarPendencia($_REQUEST['obrid'], Par3_Model_ObraAba::FOTOS_TERRENO, 1);

            $this->model->popularDadosObjeto($arrayDados);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelo = new Par3_Model_ObraFotosTerreno($arrPost['oftid']);

            //grava no histórico
            $arrayDadosHistorico['oftid'] = $modelo->oftid;
            $arrayDadosHistorico['hofdtcriacao'] = date('Y-m-d H:m:s');
            $arrayDadosHistorico['hofcpf'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
            $arrayDadosHistorico['hofacao'] = Par3_Model_ObraFotosTerrenoHistorico::DELETE;
            $arrayDadosHistorico['arqid'] = $modelo->arqid;
            $arrayDadosHistorico['ofadsc'] = $modelo->oftdsc;
            $arrayDadosHistorico['oftstatus'] = $modelo->oftstatus;
            $arrayDadosHistorico['obrid']  = $arrPost['obrid'];

            $this->modelHistorico->popularDadosObjeto($arrayDadosHistorico);
            $this->modelHistorico->salvar();
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            $this->model->rollback();
            $this->modelHistorico->rollback();
            simec_redirecionar($url, 'success');
        }
    }
}

?>