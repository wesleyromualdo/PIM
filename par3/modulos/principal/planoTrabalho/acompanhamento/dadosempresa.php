<?php
switch ($_REQUEST['req']) {
    default:
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        if (!isset($modelUnidade)) $modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);

        $controllerContrato = new Par3_Controller_Contratos();
        $controllerEmpresa = new Par3_Controller_EmpresasContrato();
        $controllerResponsavel = new Par3_Controller_ResponsavelContrato();
        $controllerVeiculo = new Par3_Controller_VeiculoContrato();
        $controllerFabricante = new Par3_Controller_FabricanteVeiculo();
        $controllerModelo = new Par3_Controller_ModeloVeiculo();

        if ($modelUnidade->itrid == '2') {
            $esferaTitulo = 'Municipal';
            $esferaTituloPlural = 'Municipais';
            $esferaEntidade = 'Municipio';
        } elseif ($modelUnidade->itrid == '3') {
            $esferaTitulo = 'Distrital';
            $esferaTituloPlural = 'Distritais';
            $esferaEntidade = 'Distrito Federal';
        } else {
            $esferaTitulo = 'Estadual';
            $esferaTituloPlural = 'Estaduais';
            $esferaEntidade = 'Estado';
        }
        break;
}


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


?>
    <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; ">
        <div class="ibox">
            <?php if ($disabled == '') { ?>
                <div class="ibox-title">
                    <div class="row">
                        <div class="col-md-6">
                            <h3 class="pull-left" style="margin-bottom: 10px" id="dadosEmpresasContratadas">Dados das Empresas
                                Contratadas</h3>
                        </div>
                        <div class="col-md-6">
                            <button class="btn btn-success pull-right" data-toggle="modal" data-target="#modal_contrato">
                                <i class="fa fa-plus-square-o"></i> Inserir Contrato
                            </button>

                        </div>
                        <div class="col-md-2"></div>
                    </div>
                </div>
            <?php } ?>
            <?php if($disabled == '') { ?>
                <div class="ibox-content" style="display: block">
                    <div class="row">
                        <form method="POST" id="formEmpresa" action="">
                            <div id="conteudoFormEmpresa">
                                <?php $controllerEmpresa->crudEmpresas($_REQUEST); ?>
                            </div>
                            <input type="hidden" name="req" value="salvarEmpresa"/>
                            <div class="pull-right">
                                <button type="submit" class="btn btn-success" id="novo_empresa"
                                        title="Cadastrar Empresa Contratada"><i class="fa fa-save"></i></button>
                                <button type="button" class="btn btn-warning" id="atualizarFormEmpresaContrato" title="Recarregar Formulário">
                                    <i class="fa fa-refresh"></i>
                                </button>
                            </div>

                        </form>
                    </div>
                </div>
            <?php } ?>
            <div class="ibox-content" style="display: block" id="listagemEmpresas">
                <?php $controllerEmpresa->listaEmpresas($_REQUEST); ?>
            </div>
        </div>
    </div>

    <!-- Modals -->
<?php if($disabled == '') { ?>
    <div class="ibox float-e-margins animated modal" id="modal_contrato" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-dialog">
            <form method="post" name="formContrato" id="formContrato" class="form form-horizontal">
                <div class="modal-content animated flipInY">
                    <div class="ibox-title" tipo="integrantes">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                    aria-hidden="true">&times;</span>
                        </button>
                        <h3>Formulário de Contrato</h3>
                    </div>
                    <div class="ibox-content">
                        <!--                        <input type="hidden" name="req" value="salvar"/>-->
                        <?php echo $controllerContrato->formNovoContrato($_REQUEST); ?>
                    </div>
                    <div class="ibox-footer">
                        <div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
                            <button type="submit" class="btn btn-success" id="novo_contrato" <?php echo $disabled; ?>>
                                <i class="fa fa-plus-square-o"></i> Salvar
                            </button>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>

    <div class="ibox float-e-margins animated modal" id="modal_fabricante" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-dialog">
            <form method="post" name="formFabricante" id="formFabricante" class="form form-horizontal">
                <div class="modal-content animated flipInY">
                    <div class="ibox-title">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                    aria-hidden="true">&times;</span>
                        </button>
                        <h3>Formulário Fabricante</h3>
                    </div>
                    <div class="ibox-content">
                        <?php echo $controllerFabricante->formFabricante($_REQUEST); ?>
                    </div>
                    <div class="ibox-footer">
                        <div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
                            <button type="submit" class="btn btn-success" id="novo_fabricante" <?php echo $disabled; ?>>
                                <i class="fa fa-plus-square-o"></i> Salvar
                            </button>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
    <div class="ibox float-e-margins animated modal" id="modal_modelo" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-dialog">
            <form method="post" name="formModelo" id="formModelo" class="form form-horizontal">
                <div class="modal-content animated flipInY">
                    <div class="ibox-title">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                    aria-hidden="true">&times;</span>
                        </button>
                        <h3>Formulário Modelo</h3>
                    </div>
                    <div class="ibox-content" id="conteudoFormModelo">
                        <?php echo $controllerModelo->formModelo($_REQUEST); ?>
                    </div>
                    <div class="ibox-footer">
                        <div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
                            <button type="submit" class="btn btn-success" id="novo_modelo" <?php echo $disabled; ?>>
                                <i class="fa fa-plus-square-o"></i> Salvar
                            </button>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
<?php } ?>
    <!-- FIM - Modals -->
<?php if($disabled == '') { ?>
    <script>
        $(document).ready(function () {
            // Variable to store your files
            var files;

            // Add events
            $(document).on('change', 'input[type=file]', prepareUpload);

            // Grab the files and set them to our variable
            function prepareUpload()
            {
                files = event.target.files;
            }
            // Catch the form submit and upload the files
            function uploadFiles()
            {
                event.stopPropagation(); // Stop stuff happening
                event.preventDefault(); // Totally stop stuff happening

                // START A LOADING SPINNER HERE

                // Create a formdata object and add the files
                var data = new FormData();
                $.each(files, function(key, value)
                {
                    data.append(key, value);
                });

                var retonro;

                $.ajax({
                    url: window.location.href+'&req=upload',
                    type: 'POST',
                    data: data,
                    cache: false,
                    async: false,
                    processData: false, // Don't process the files
                    contentType: false, // Set content type to false as jQuery will tell the server its a query string request
                    success: function(data, textStatus, jqXHR)
                    {
                        var dados = $.parseJSON(data);
                        console.log(dados);
                        retonro = dados;
                    },
                    error: function(jqXHR, textStatus, errorThrown)
                    {
                        // Handle errors here
                        console.log('ERRORS: ' + textStatus);
                        return false;
                        // STOP LOADING SPINNER
                    }
                });

                return retonro;
            }

            var possuirenavam = "";

            $(document).on('change click',"input[name=vcopossuirenavam]", function () {
                possuirenavam = $("input:checked[name=vcopossuirenavam ]").val();
                console.log(possuirenavam);
                $("#div-vcorenavam").toggle();
                $("#div-vcoplaca").toggle();
                $("#div-fabricante").toggle();
                $("#div-mveid").toggle();
                $("#div-vcochassi").toggle();
                $("#div-vcoinscricao").toggle();
                $("#div-arqid").toggle();

                if (possuirenavam === 't') {
                    console.log(true);
                    $("#fabricante").attr('required','required');
                    $("#mveid").attr('required','required');
                    $("#vcoplaca").attr('required','required');
                    $("#vcochassi").attr('required','required');
                    $("#vcorenavam").attr('required','required');
                }

                if (possuirenavam === 'f') {
                    console.log(false);
                    $("#fabricante").removeAttr('required');
                    $("#mveid").removeAttr('required');
                    $("#vcoplaca").removeAttr('required');
                    $("#vcochassi").removeAttr('required');
                    $("#vcorenavam").removeAttr('required');
                }
            }).change();

            $(document).on('click','#atualizarFormVeiculo',function(evt){
                evt.preventDefault();
                atualizarFormVeiculo();
            });
            $(document).on('click','#atualizarFormEmpresaContrato',function(evt){
                evt.preventDefault();
                atualizarFormEmpresa();
            });

            $(document).on('click','#novo_contrato',function (evt) {
                evt.preventDefault();
                var caminho = window.location.href;
                jQuery('#formContrato').isValid(function (isValid) {
                    if (isValid) {
                        var action = '&req=salvarAjax&' + $("#formContrato").serialize();
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_contrato").modal("hide");
                                $("#modal_contrato").find("input").val("");
                                if (resp == 'sucesso') {
                                    swal("Sucesso.", "Contrato salvo com sucesso.", "success");
                                    atualizarFormEmpresa();
                                    atualizarFormVeiculo();
                                    atualizarFormResponsavel();
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao salvar.", "error");
                            }
                        });
                    }
                });
            });

            $(document).on('click', '#novo_veiculo', function (evt) {
                evt.preventDefault();

                var possuirenavam = $("input:checked[name=vcopossuirenavam ]").val();
                var vcorenavam = $('input[name=vcorenavam]').val();
                var vcochassi = $('input[name=vcochassi]').val();
                var vcoqtdassentos = $('input[name=vcoqtdassentos]').val();

                var erro = false;
                if (possuirenavam == 't') {
//                 if (!verificaRenavam(vcorenavam)) {
//                     erro['renavam'] = "O número do renavam está incorreto.";
//                     swal("Erro.", "O número do renavam está incorreto.", "error");
//                 }
                    if (vcochassi.length > 16) {
                        erro['chassi'] = "O número do chassi está incorreto.";
                        swal("Erro.", "O número do chassi está incorreto.", "error");
                    }
                }
                if($.isNumeric(vcoqtdassentos) && vcoqtdassentos < 0){
                    erro['assentos'] = "Quantidade de assentos é inválida";
                    swal("Erro.", "Quantidade de assentos é inválida", "error");
                }
                if (erro) {
                    console.log(erro);
                    return false;
                }
                var form = $("#formVeiculo").serialize();
                jQuery('#formVeiculo').isValid(function (isValid) {
                    var dados = uploadFiles();
                    console.log(dados);
                    if (isValid && dados.success == 'true') {
                        var caminho = window.location.href;
                        var action = '&req=salvarVeiculo&arqid='+dados.arqid+'&' + form;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (!isNaN(parseFloat(resp)) && isFinite(resp)) {
                                    swal("Sucesso.", "Veículo salvo com sucesso.", "success");
                                    atualizarListagemVeiculos(possuirenavam);
                                    atualizarFormVeiculo();
                                    return true;
                                }
                                switch(resp){
//                                 case 'chassi':
//                                     swal("Erro.", "Chassi incorreto.", "error");
//                                     break;
//                                 case 'renavam':
//                                     swal("Erro.", "Renavam incorreto.", "error");
//                                     break;
                                    case 'ano':
                                        swal("Erro.", "O Ano do Veículo não é válido.", "error");
                                        break;
                                    default:
                                        swal("Erro.", resp,"error");
                                        break;
                                }
                            }
                        });
                    }else{
                        swal("Erro.", dados.error, "error");
                    }
                });
            });

            $(document).on('change click',"#fabricante", function () {
                var fabricante_id = $("#fabricante").val();
                var caminho = window.location.href;
                var options = $("#mveid");
                var action = '&req=listarModelos&fabid=' + fabricante_id;
                $.getJSON(caminho + action, function (dados) {
                    options.empty();
                    if (!dados) {
                        options.append(new Option("",""));
                    }else{
                        $.each(dados, function (index, modelo) {
                            options.append(new Option(modelo.mvedsc, modelo.mveid));
                        });
                    }
                    options.focus();
                    options.trigger('chosen:updated');
                });
            }).change();

        });
        $(document).on("click","#novo_fabricante",function (evt) {
            evt.preventDefault();
            var form = $("#formFabricante").serialize();
            if(verificarFabricanteExiste(form)){
                swal("Atenção.", "Já existe um fabricante cadastrado com essa descrição.", "info");
                return false;
            }

            var caminho = window.location.href;
            var action = '&req=salvarFabricante&' +form;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    console.log(resp);
                    $("#modal_fabricante").modal("hide");
                    $("#modal_fabricante").find("input").val("");
                    if (resp != 'erro') {
                        swal("Sucesso.", "Fabricante salvo com sucesso.", "success");
                        var fabid = "&fabricante=" + resp;
                        var formSerializado = $("input[name!=req]","#formVeiculo").serialize();
                        atualizarFormVeiculo(formSerializado + fabid);
                        atualizarFormModelo();
                        return true;
                    }
                    swal("Erro.", "Ocorreu um erro ao salvar.", "error");
                }
            });
        });

        $(document).on("click","#novo_modelo",function (evt) {
            evt.preventDefault();
            var form = $("#formModelo").serialize() ;
            var teste = verificarModeloExiste(form);
            if(teste){
                swal("Atenção.", "Já existe um modelo com o mesmo nome para este fabricante.", "info");
                return false;
            }
            var caminho = window.location.href;

            var action = '&req=salvarModelo&' + form;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#modal_modelo").modal("hide");
                    $("#modal_modelo").find("input").val("");
                    if (resp != 'erro') {
                        swal("Sucesso.", "Modelo salvo com sucesso.", "success");
                        var mveid = "&mveid=" + resp;
                        var formSerializado = $("input[name!=req]","#formVeiculo").serialize();
                        atualizarFormVeiculo(formSerializado + mveid);
                        return false;
                    }
                    swal("Erro.", "Ocorreu um erro ao salvar.", "error");
                }
            });
        });

        $(document).on('click', '.salvar_responsavel', function () {
            $("#loading").fadeIn();
            var caminho = window.location.href;
            var action = '&' + $('#formResponsavel').serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    if (resp != 'erro') {
                        swal("Sucesso!", "Rsponsavel salvo com sucesso!", "success")
                        $("#div_lista_responsaveis").html(resp);
                        action = '&req=editaResponsavel';
                        $.ajax({
                            type: "POST",
                            url: window.location.href,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#div_form_responsaveis").html(resp);
                            }
                        });
                    }
                }
            });
        });

        function inativarResponsavel(id) {
            var action = '&req=inativarResponsavel&rcoid=' + id;
            var url = window.location.href;
            if (!id) {
                swal("Erro!", "Falta de parâmetros!", "error")
                return false;
            }
            $("#loading").fadeIn();
            swal({
                    title: "Você tem certeza?",
                    text: "O registro uma vez excluído não poderá ser recuperado!",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    confirmButtonText: "Sim",
                    cancelButtonText: "Não",
                    closeOnConfirm: false
                },
                function () {
                    $.ajax({
                        type: "POST",
                        url: url,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if (resp != 'erro') {
                                swal("Sucesso!", "Responsavel excluído com sucesso!", "success")
                                $("#div_lista_responsaveis").html(resp);
                            }
                        }
                    });
                }
            );
        }
        function editarResponsavel(id) {
            $("#loading").fadeIn();

            var caminho = window.location.href;
            var action = '&req=editaResponsavel&rcoid=' + id;

            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#div_form_responsaveis").html(resp);
                }
            });
        }

        function atualizarFormVeiculo(params = "") {
            if(params){
                params = '&'+params
            }
            var caminho = window.location.href;
            var action = '&req=atualizarFormVeiculo'+params;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#formVeiculo").html(resp);
                }
            });
        }

        function atualizarFormResponsavel(params = "") {
            if(params){
                params = '&'+params
            }
            var caminho = window.location.href;
            var action = '&req=atualizarFormResponsavel'+params;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#div_form_responsaveis").html(resp);
                }
            });
        }

        function atualizarFormModelo(params = "") {
            if(params){
                params = '&'+params
            }
            var caminho = window.location.href;
            var action = '&req=atualizarFormModelo'+params;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#conteudoFormModelo").html(resp);
                }
            });
        }

        function atualizarFormEmpresa() {
            var caminho = window.location.href;
            var action = '&req=editar';
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#conteudoFormEmpresa").html(resp);
                }
            });
        }

        $(document).on("click","#novo_empresa",function (evt) {
            evt.preventDefault();
            var form  = $("#formEmpresa").serialize();
            var verificaForm = $("input[name!=req]","#formEmpresa").serialize();
            var cnpj = $("#ecocnpjempresa").val();
            if(!validaCNPJ(cnpj)){
                swal("Erro.", "CNPJ inválido.", "error");
                return false;
            }
            if(verificarExisteEmpresaContratoCNPJ(verificaForm)){
                swal("Atenção.", "Já existe um empresa cadastrada com esse CNPJ.", "info");
                return false;
            }

            jQuery('#formEmpresa').isValid(function (isValid) {
                if (isValid) {
                    var caminho = window.location.href;
                    var action = '&req=salvarEmpresa&'+form;
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if (resp != 'erro') {
                                swal("Sucesso.", "Empresa salva com sucesso.", "success");
                                atualizarFormEmpresa();
                                $("#listagemEmpresas").html(resp);
                                return true;
                            }
                            swal("Erro.", "Ocorreu um erro ao salvar.", "error");
                            return false;
                        }
                    });
                }
            });
        });

        function inativarEmpresa(id) {
            var action = '&req=inativarEmpresa&ecoid=' + id;
            var url = window.location.href;

            if (!id) {
                swal("Erro!", "Falta de parâmetros!", "error")
                return false;
            }
            swal({
                    title: "Você tem certeza?",
                    text: "O registro uma vez excluído não poderá ser recuperado!",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    confirmButtonText: "Sim",
                    cancelButtonText: "Não",
                    closeOnConfirm: false
                },
                function () {
                    $.ajax({
                        type: "POST",
                        url: url,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if (resp != 'erro') {
                                swal("Sucesso!", "Empresa excluída com sucesso!", "success")
                                $("#listagemEmpresas").html(resp);
                                return true;
                            }
                            swal("Erro!", "Ocorreu um erro ao tentar Excluir a empresa.", "error")
                            return false;
                        }
                    })
                    ;
                }
            );
        }

        function editarEmpresa(id, params = "") {
            var caminho = window.location.href;
            if (params != "") {
                params = "&" + params;
            }
            var action = '&req=editar&ecoid=' + id + params;

            //window.location.href = url + action + '&entid=' + id;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#conteudoFormEmpresa").html(resp);
                }
            });
        }

        function editarVeiculo(id, params = "") {
            var caminho = window.location.href;
            if (params != "") {
                params = "&" + params;
            }
            var action = '&req=editarVeiculo&vcoid=' + id + params;
            //window.location.href = url + action + '&entid=' + id;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $("#formVeiculo").html(resp);
                    var renavam = $("#formVeiculo").find("input[name=vcopossuirenavam]").val();
                }
            });
        }

        function verificaRenavam(renavam) {

            var d = renavam.split("");
            soma = 0,
                valor = 0,
                digito = 0,
                x = 0;

            for (var i = 5; i >= 2; i--) {
                soma += d[x] * i;
                x++;
            }

            valor = soma % 11;

            if (valor == 11 || valor == 0 || valor >= 10) {
                digito = 0;
            } else {
                digito = valor;
            }

            if (digito == d[4]) {
                return true;
            } else {
                return false;
            }
        }



        function inativarVeiculo(id) {
            var action = '&req=inativarVeiculo&vcoid=' + id;
            var url = window.location.href;

            if (!id) {
                swal("Erro!", "Falta de parâmetros!", "error")
                return false;
            }
            swal({
                    title: "Você tem certeza?",
                    text: "O registro uma vez excluído não poderá ser recuperado!",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    confirmButtonText: "Sim",
                    cancelButtonText: "Não",
                    closeOnConfirm: false
                },
                function () {
                    $.ajax({
                        type: "POST",
                        url: url,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if (resp != 'erro') {
                                swal("Sucesso!", "Empresa excluída com sucesso!", "success")
                                atualizarFormEmpresa();
                                atualizarFormVeiculo()
                                atualizarListagemVeiculos('t');
                                atualizarListagemVeiculos('f');
                            }
                        }
                    });
                }
            );
        }

        function verificarModeloExiste(req)
        {
            var caminho = window.location.href;
            var action = '&req=verificarModeloExiste&'+req;
            var resposta;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    resposta = resp;
                }
            });
            return resposta;
        }

        function verificarExisteEmpresaContratoCNPJ(req)
        {
            var caminho = window.location.href;
            var action = '&req=verificarExisteEmpresaContratoCNPJ&'+req;
            var resposta;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    console.log(caminho+action);
                    console.log("Esse CNPJ já existe? "+resp);
                    resposta = resp;
                }
            });
            return resposta;
        }

        function verificarFabricanteExiste(req)
        {
            var caminho = window.location.href;
            var action = '&req=verificarFabricanteExiste&'+req;
            var resposta;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    resposta = resp;
                }
            });
            return resposta;
        }

        function validaCNPJ(cnpj)
        {
            var caminho = window.location.href;
            var action = '&req=validaCNPJ&cnpj=' + cnpj;
            var retorno;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    retorno = resp;
                }
            });
            return retorno;
        }

        function atualizarListagemVeiculos(vcopossuirenavam) {
            var caminho = window.location.href;
            var action = '&req=atualizarListagemVeiculos&vcopossuirenavam=' + vcopossuirenavam;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    if (vcopossuirenavam == 't') {
                        $("#listagemAutomoveis").html(resp);
                        return true;
                    }
                    if (vcopossuirenavam == 'f') {
                        $("#listagemEmbarcacoes").html(resp);
                        return true;
                    }
                }
            });
        }

        function baixarAutorizacao(vcoid) {
            var action = '&req=baixarAutorizacao&vcoid=' + vcoid;
            window.open(window.location.href+action);
        }
    </script>
<?php } ?>