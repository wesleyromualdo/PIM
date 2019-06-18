<?php
/**
 * Classe de controle do  obra_documento_apoio
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RENNER NASCENTES TANIZAKI <rennertanizaki@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */


/**
 * Par3_Controller_Obra_documento_apoio
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */

class Par3_Controller_ObraDocumentoApoio
{
    private $model;

    public function __construct()
    {
        $odaid = validarInteiro($_POST['odaid']) ? $_POST['odaid'] : '';
        $this->model = new Par3_Model_ObraDocumentoApoio($odaid);
    }

    public function form($arrPost)
    {
        $requidoArquivo = 'required';
        if($arrPost['odaid']){
            $this->model = new Par3_Model_ObraDocumentoApoio($arrPost['odaid']);
            $requidoArquivo = '';
//            $arquivoAnterior = '<div class="form-group"><label class="col-lg-3 control-label">Arquivo anterior:</label>';
//            $arquivoAnterior .= '<div class="col-lg-2"><a href=\'?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosApoio&req=download&arqid='.$this->model->arqid.'>adsasasd</a></div>';
//            $arquivoAnterior .= '</div>';
        }

        global $simec;
        global $disabled;
        echo '<input type="hidden" name="odaid" value="' . $this->model->odaid . '"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="' . $this->model->odaid . '" class="form-control"></div>';
        echo '</div>';

        echo $simec->input('odadsc', 'Descrição', $this->model->odadsc, array('maxlength' => '255', 'required'));
        echo $simec->input('arqid', 'Anexo', null, array('type' => 'file', $requidoArquivo, 'style' => 'height: auto !important;'));
        echo $simec->radio('odasituacao', 'Situação', (!($this->model->odasituacao) ? 'A' : $this->model->odasituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    public function salvar($arrPost)
    {
        $url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosApoio';

        if ($_FILES['arqid']['name']) {
            $file = new FilesSimec("documentoarquivo", [], 'par3');

            $file->setUpload(null, 'arqid', false);
            $arqid = $file->getIdArquivo();
        }

        $arrayDadosHistorico['hodacao'] = Par3_Model_ObraDocumentoApoioHistorico::CREATE;
        if ($arrPost['odaid']) {
            $arrayDados['odaid'] = trim($arrPost['odaid']);
            $arrayDadosHistorico['hodacao'] = Par3_Model_ObraDocumentoApoioHistorico::UPDATE;
        }

        $arrayDados['odasituacao'] = $arrPost['odasituacao'];
        $arrayDados['arqid'] = $arqid;
        $arrayDados['odastatus'] = 'A';
        $arrayDados['odadsc'] = trim(($arrPost['odadsc']));
        $arrayDados['odacpfinclusao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);

        if (!$arrPost['odaid']) {
            $arrayDados['odadtinclusao'] = date('Y-m-d H:m:s');
        }

        //Valida campos
        $erro = $this->model->validarInput($arrayDados);
        if ($erro) {
            return $erro;
        }

        try {
            $this->model->popularDadosObjeto($arrayDados);
            $novooda = $this->model->salvar();
            $this->model->commit();

            $modelo = new Par3_Model_ObraDocumentoApoio($novooda);

            //grava informações no histórico
            //grava no histórico
            $arrayDadosHistorico['odaid'] = $modelo->odaid;
            $arrayDadosHistorico['hoddtcriacao'] = date('Y-m-d H:m:s');
            $arrayDadosHistorico['hodcpf'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
            $arrayDadosHistorico['odadsc'] = $modelo->odadsc;
            $arrayDadosHistorico['odasituacao'] = $modelo->odasituacao;
            $arrayDadosHistorico['odastatus'] = 'A';

            $modeloHistorico = new Par3_Model_ObraDocumentoApoioHistorico();
            $modeloHistorico->popularDadosObjeto($arrayDadosHistorico);
            $modeloHistorico->salvar();
            $modeloHistorico->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($novooda) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'success');
    }


    public function remover($arrPost)
    {
        $arrayDados['odaid'] = $arrPost['odaid'];
        $arrayDados['odastatus'] = 'I';

        $modeloObraTipoXDocumentoApoio = new Par3_Model_ObraTipoXDocumentoApoio();
        $dados = $modeloObraTipoXDocumentoApoio->recuperarTodos('odaid',array('odaid = '.$arrPost['odaid']));
        if(!$dados) {
            try {
                $this->model->popularDadosObjeto($arrayDados);
                $sucesso = $this->model->salvar();
                $this->model->commit();

                $modelo = new Par3_Model_ObraDocumentoApoio($arrPost['odaid']);

                //grava no histórico
                $arrayDadosHistorico['odaid'] = $modelo->odaid;
                $arrayDadosHistorico['hoddtcriacao'] = date('Y-m-d H:m:s');
                $arrayDadosHistorico['hodcpf'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
                $arrayDadosHistorico['hodacao'] = Par3_Model_ObraDocumentoApoioHistorico::DELETE;
                $arrayDadosHistorico['arqid'] = $modelo->arqid;
                $arrayDadosHistorico['odadsc'] = $modelo->odadsc;
                $arrayDadosHistorico['odastatus'] = $modelo->odastatus;
                $arrayDadosHistorico['odasituacao'] = $modelo->odasituacao;

                $modeloHistorico = new Par3_Model_ObraDocumentoApoioHistorico();
                $modeloHistorico->popularDadosObjeto($arrayDadosHistorico);
                $modeloHistorico->salvar();
                $modeloHistorico->commit();
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }
            if ($sucesso) {
                return $sucesso;
            }
        }
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaDocumentosApoio($arrPost)
    {
        global $disabled;
        $model = new Par3_Model_ObraDocumentoApoio();

        $campo = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC' ? 'ASC' : 'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch ($campo) {
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'oda.odaid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'oda.odadsc';
                break;
            case 'odastatus':
                $arrQuery['ordenacao']['campo'] = 'oda.odastatus';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $model->montarSQLSimples($arrQuery);

//        $disabled = 'disabled';
//        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Descrição', 'Arquivo', 'Inserido por:', 'Data da Criação', 'Situação');
        $esconderColunas = array('id', 'qtd', 'usunome' , 'hoddtcriacaoinativacao', 'arqid');

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



}