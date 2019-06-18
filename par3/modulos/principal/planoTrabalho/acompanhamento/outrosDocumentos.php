
<?php
checkParamInuid();

require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

$inuid = $_REQUEST['inuid'];

$controleUnidade = new Par3_Controller_InstrumentoUnidade();

if($_REQUEST['menu'] == null){

    $_REQUEST['menu'] = 'receitaeducacao';

}
if($_REQUEST['req'] || $_REQUEST['requisicao']) ob_clean();

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

$url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=outros';
$db->cria_aba( 58014, $url , '&inuid=' . $inuid, array());
?>

<script language="javascript" src="js/documentoLegado.js"></script>

<style>
    .iconUnidade{
        margin-top: 0px !important;
        margin-left: 0px !important;
        font-size: 15px !important;
        background-color:black;
        border-radius: 20px 20px 20px 20px;
        -moz-border-radius: 20px 20px 20px 20px;
        -webkit-border-radius: 20px 20px 20px 20px;
        color: yellow !important;
    }

    .menuUnidade{
        border-bottom: 1px solid #e3e3e3;
        padding: 5px;
        cursor:pointer;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
    }

    .menuUnidade:hover{
        font-weight: bold;
    }

    .menuSelecionado{
        font-weight: bold;
        cursor: default !important;
    }

    @media (max-width: 1450px) {
        .ibox-content-round-gray{
            margin-top: 0.5%
        }
    }

    @media (max-width: 1200px) {
        .i1450 {
            display: inline;
        }
        .custom-col-10{
            padding: 5px 0px 0px 3.8%;
        }
        .marcador{
            position:absolute;
            border-radius: 100% 0% 0% 100%;
            -moz-border-radius: 100% 0% 0% 100%;
            -webkit-border-radius: 100% 0% 0% 100%;
            border: 0px;
            width:50px;
            height:10px;
            margin-top:21px;
            margin-left:-1%;
        }
    }


</style>

<div class="wrapper wrapper-content animated fadeIn">

    <div class="row">
        <div class="ibox float-e-margins">
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row">
                        <div class="col-md-2 colunaMenu noprint">
                            <div class="space-25"> </div>
                            <div id="menuDirigentes">
                                <h5>Documentos</h5>

                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'documentoobrapar' ? 'menuSelecionado' : '') ?>"tipo="documentoobrapar">
                                    Documento de Obras do Par
                                </div>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'documentoobrapac' ? 'menuSelecionado' : '') ?>" tipo="documentoobrapac">
                                    Documento de Obras PAC
                                </div>

                            </div>
                            <div class="space-25"></div>

                            <div id="menuEquipeTecnica">
                                <h5>SIOPE</h5>
                                <!--<div class="menuUnidade <?php /*echo ( $_REQUEST['menu'] === 'situacaoentrega' ? 'menuSelecionado' : '') */?>" tipo="situacaoentrega">
                                    Situação de Entrega
                                </div>-->
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'receitaeducacao' ? 'menuSelecionado' : '') ?>" tipo="receitaeducacao">
                                    Receitas vinculadas a Educação
                                </div>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'despesaseducacao' ? 'menuSelecionado' : '') ?>" tipo="despesaseducacao">
                                    Despesas em Educação
                                </div>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'vinculacaorecursos' ? 'menuSelecionado' : '') ?>" tipo="vinculacaorecursos">
                                    Cumprimento da Vinculação dos Recursos Destinados a Educação
                                </div>
                            </div>

                            <div class="space-25"></div>

                            <div id="menuConselhos">

                                <h5>Acompanhamento PNATE</h5>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'dadosempresa' ? 'menuSelecionado' : '') ?>" tipo="dadosempresa">
                                    Dados das Empresas Cadastradas
                                </div>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'dadosveiculos' ? 'menuSelecionado' : '') ?>" tipo="dadosveiculos">
                                    Dados dos Veículos
                                </div>

                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'responsavel' ? 'menuSelecionado' : '') ?>" tipo="responsavel">
                                    Responsável pelo Contrato
                                </div>
                            </div>
                        </div>
                        <div class="col-md-10 colunaForm" id="print_area">
                            <?php $controleUnidade->desenharTela($_REQUEST['menu']); ?>
                        </div>
                    </div>
                </div>
        </div>
    </div>
</div>

<!-- Modal de visualização dos termos -->
<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog" style="width: 70%;">
        <div class="ibox-title">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form" class="ibox-content">
        </div>
    </div>
</div>
<!-- -->

<script>

    $('.menuUnidade').click(function(){
        var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
        var menu      = $(this).attr('tipo');
        var url       = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A'+'&inuid=<?php echo $_REQUEST['inuid']?>&aba=outros&menu='+menu;

        if (menu != menuAtual) {
            $(location).attr('href',url);
        }
    });


    $(document).ready(function()
    {
        $('.next').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $('.previous').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        setTimeout(jQueryShowBtnProxReturn, 1000); //desabilita o btn próximo do menu quando entram diretamente no planejamento
    });
</script>