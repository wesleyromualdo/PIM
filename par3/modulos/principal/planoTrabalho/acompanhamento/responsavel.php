<?php
checkParamInuid();

$inuid = $_REQUEST['inuid'];

$controleUnidade = new Par3_Controller_InstrumentoUnidade();

switch ($_REQUEST['req']) {
    case 'baixarArquivoDopid':
        $controllerDocumento = new Par3_Controller_DocumentoLegado();
        $controllerDocumento->baixarArquivoDopid($_REQUEST);
    case 'vizualizaDocumento':
        $documento = new Par3_Model_DocumentoParLegado();
        $documento->formVizualizaDocumento($_REQUEST);
        die();
        break;
    case 'vizualizaDocumentoPAC':
        ob_clean();
        $documento = new Par3_Model_DocumentoParLegado();
        $documento->formVizualizaDocumentoPAC($_REQUEST);
        die();
        break;
    case 'salvar':
        $controllerContrato = new Par3_Controller_Contratos();
        $controllerContrato->salvar($_REQUEST);
        break;
    case 'salvarAjax':
        ob_clean();
        $controllerContrato = new Par3_Controller_Contratos();
        $retorno = $controllerContrato->salvar($_REQUEST);
        echo $retorno;
        die;
        break;
    case 'verificarExisteEmpresaContratoCNPJ':
        ob_clean();
        $controllerEmpresa = new Par3_Model_EmpresasContrato();
        echo $controllerEmpresa->verificarExisteEmpresaContratoCNPJ($_REQUEST);
        die;
        break;
    case 'validaCNPJ':
        ob_clean();
        echo validarCnpj($_REQUEST['cnpj']);die;
        break;
    case 'salvarEmpresa':
        ob_clean();
        $controllerEmpresa = new Par3_Controller_EmpresasContrato();
        $retorno = $controllerEmpresa->salvar($_REQUEST);

        if ($retorno) {
            echo $controllerEmpresa->listaEmpresas($_REQUEST);
        } else {
            echo 'erro';
        }
        die;
        break;
    case 'inativarEmpresa':
        $controllerEmpresa = new Par3_Controller_EmpresasContrato();
        $retorno = $controllerEmpresa->inativar($_REQUEST);

        if ($retorno) {
            ob_clean();
            echo $controllerEmpresa->listaEmpresas($_REQUEST);
        } else {
            echo 'erro';
        }
        die;
        break;
    case 'upload':
        if (isset($_FILES)) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec("veiculo_contrato", array('vcoid' => $_REQUEST['vcoid']), "par3");
            $file->setUpload('Ata de eleição', null, false);
            $arqid = $file->getIdArquivo();
            if(is_numeric($arqid) && $arqid > 0 ){
                echo json_encode(array('success' => 'true', 'arqid' => $arqid));
            }else{
                echo json_encode(array('success' => 'false', 'error' => 'Falha ao gravar o arquivo'));
            }
        }else{
            echo json_encode(array('success' => 'false', 'error' => 'Arquivo não enviado.'));
        }
        die;
        break;
    case 'salvarVeiculo':
        $controllerVeiculo = new Par3_Controller_VeiculoContrato();
        $retorno = $controllerVeiculo->salvar($_REQUEST);
        echo $retorno;die;
        break;
    case 'editarVeiculo':
        ob_clean();
        $controllerVeiculo = new Par3_Controller_VeiculoContrato();
        echo $controllerVeiculo->formVeiculo($_REQUEST);
        die;
        break;
    case 'baixarAutorizacao':
        $model = new Par3_Model_VeiculoContrato($_REQUEST['vcoid']);
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec("veiculo_contrato", null, "par3");
        $file->getDownloadArquivo($model->arqid);
        exit;
    case 'excluirAutorizacao':
        $model = new Par3_Model_VeiculoContrato($_REQUEST['vcoid']);
        $model->arqid = '';
        $model->salvar();
        $model->commit();
        exit;
    case 'atualizarListagemVeiculos':
        ob_clean();
        $controllerVeiculo = new Par3_Controller_VeiculoContrato();
        echo $controllerVeiculo->listarVeiculos($_REQUEST);
        die;
        break;
    case 'inativarVeiculo':
        ob_clean();
        $controllerVeiculo = new Par3_Controller_VeiculoContrato();
        echo $controllerVeiculo->inativar($_REQUEST);die;
        break;
    case 'salvarFabricante':
        ob_clean();
        $controllerFabricante = new Par3_Controller_FabricanteVeiculo();
        echo $controllerFabricante->salvar($_REQUEST);
        die;
        break;
    case 'verificarFabricanteExiste':
        $mFabricante = new Par3_Model_FabricanteVeiculo();
        echo $mFabricante->verificarFabricanteExiste($_REQUEST);die;
        break;
    case 'salvarModelo':
        $controllerModelo = new Par3_Controller_ModeloVeiculo();
        ob_clean();
        $retorno = $controllerModelo->salvar($_REQUEST);
        if (!$retorno) {
            echo 'erro';
            die;
        }
        echo $retorno;
        die;
        break;
    case 'atualizarFormResponsavel':
        ob_clean();
        $controllerResponsavel = new Par3_Controller_ResponsavelContrato();
        echo $controllerResponsavel->crudResponsavel();
        die;
        break;
    case 'atualizarFormModelo':
        ob_clean();
        $controllerModelo = new Par3_Controller_ModeloVeiculo();
        echo $controllerModelo->formModelo($_REQUEST);die;
    case 'verificarModeloExiste':
        $mModelo = new Par3_Model_ModeloVeiculo();
        ob_clean();
        $result = $mModelo->verificarModeloExiste($_REQUEST);
        echo $result; die;
        break;
    case 'listarModelos':
        $mModelo = new Par3_Model_ModeloVeiculo();
        ob_clean();
        if($_REQUEST['fabid']){
            echo simec_json_encode($mModelo->lista(array('mveid', 'mvedsc'), array("fabid = '{$_REQUEST['fabid']}'")));
        }
        die;
        break;
    case 'atualizarFormVeiculo':
        ob_clean();
        $controllerVeiculo = new Par3_Controller_VeiculoContrato();
        echo $controllerVeiculo->formVeiculo($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        $controllerEmpresa = new Par3_Controller_EmpresasContrato();
        echo $controllerEmpresa->crudEmpresas($_REQUEST);
        die;
    case 'salvarResponsavel':
        ob_clean();
        $controllerResponsavel = new Par3_Controller_ResponsavelContrato();
        $retorno = $controllerResponsavel->salvar($_REQUEST);
        if ($retorno) {
            echo $controllerResponsavel->listaResponsaveis($_REQUEST);
        } else {
            echo 'erro';
        }
        die;
    case 'editaResponsavel':
        ob_clean();
        $controllerResponsavel = new Par3_Controller_ResponsavelContrato();
        $controllerResponsavel->crudResponsavel($_REQUEST);
        die;
    case 'inativarResponsavel':
        ob_clean();
        $controllerResponsavel = new Par3_Controller_ResponsavelContrato();
        $retorno = $controllerResponsavel->inativar($_REQUEST);
        if ($retorno) {
            echo $controllerResponsavel->listaResponsaveis($_REQUEST);
        } else {
            echo 'erro';
        }
        die;
        break;
}

switch ($_REQUEST['requisicao']) {
    case 'listaHistorico':
        $_REQUEST['dopid'] = $_POST['dados'][0];
        Par3_Controller_DocumentoLegado::listaHistoricoPAR($_REQUEST);
        die();
        break;
    case 'listaHistoricoObrasPac':
        $_REQUEST['dopid'] = $_POST['dados'][0];
        Par3_Controller_DocumentoLegado::listaHistoricoObrasPAR($_REQUEST);
        die();
        break;
}

require APPRAIZ.'includes/cabecalho.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc'; ?>

<script language="javascript" src="js/documentoLegado.js"></script>

<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-content">
                    <div class="ibox">
                        <div class="ibox-content" id="pnate">
                            <?php require_once("listaAcompanhamentoPNATE.inc"); ?>
                        </div>
                        <div class="ibox-footer">

                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog" style="width: 70%;">
        <div class="ibox-title">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form" class="ibox-content">
        </div>
    </div>
</div>
</div>

<script>
    $(document).ready(function()
    {
        $('.next').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $('.previous').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });
    });
</script>