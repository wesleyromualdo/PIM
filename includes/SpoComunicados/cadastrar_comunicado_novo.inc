<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/SpoComunicados/classes/Comunicado.php';
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
include_once APPRAIZ . "includes/SpoComunicados/Usuarioresponsabilidade.class.inc";
$comunicado = new Comunicado(MODULO);
$message = new Simec_Helper_FlashMessage(MODULO . '/comunicados');
include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . "seguranca/classes/model/Perfil.class.inc";

if (isset($_REQUEST['pflid'])) {
    ob_clean();
    $resultado = (new Public_Model_Unidade())->obtemUnidadesOrcamentariasPorPerfil($_REQUEST['pflid']);
    echo simec_json_encode($resultado);
    exit();
}

switch ($_REQUEST['requisicao']) {
    case 'cadastrar':
        $comunicado->cadastrar();
            header('location: '.MODULO.'.php?modulo=principal/comunicado/cadastrar&acao=A');
            exit;
        break;
    case 'editar':
            $comunicado->atualizar();
        break;
    case 'comunicadoEditar':
        ob_clean();
        echo simec_json_encode($comunicado->listarComunicadoEditar($_POST['angid']));
        exit;
        break;
    case 'excluir':
        return $comunicado->deletar($_POST['angid']);
        break;
    default:
}
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Comunicado</li>
        <li class="active">Cadastro de Arquivos</li>
    </ol>
    <?php
    echo $message->getMensagens();
    bootstrapPopup('Editar comunicado', 'editarComunicadoModal', APPRAIZ . "includes/SpoComunicados/modal_editar_comunicado.inc", ['fechar', 'confirmar'], array('tamanho' => 'lg'));
    ?>
    <?php $simec = new Simec_View_Helper(); ?>
    <div class="well">
        <div class="col-md-12"><h2 style="padding-bottom: 40px;text-align:center;">Comunicado</h2></div>
        <div class="col-md-12"></div>
        <fieldset>
            <form action="" id="formPrincipal" name="formPrincipal" method="post" class="form-horizontal"
                  enctype="multipart/form-data">
                <input type='hidden' name="modulo_requisicao" id="modulo" value='<?php echo MODULO; ?>'/>
                <input type="hidden" name="requisicao" value="cadastrar"/>

                <section class="form-group">
                    <label class="control-label col-md-2">Perfil:</label>
                    <section class="col-md-10">
                        <?php
                        echo inputCombo("perfil", (new Seguranca_Model_Perfil())->montaSQLCombo($_SESSION['sisid']), null, "perfil", ['multiple' => true, 'obrig' => 'S']); ?>
                    </section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Unidade Or�ament�ria:</label>
                    <section class="col-md-10">
                        <?php
                        $resultado = (new Public_Model_Unidade())->obtemUnidadesOrcamentariasMec();
                        echo inputCombo("unidadeOrcamentaria", $resultado, null, "unidadeOrcamentaria", ['multiple' => true]); ?>
                    </section>
                </section>
                <?php echo $simec->periodo(array('dataini', 'datafim'), 'Per�odo', array('', ''),array('type' => 'date'), array('label-size' => '2', 'input-size' => '10')); ?>
                <div class="col-lg-12" style="padding-left: 219px; padding-bottom: 15px;">
                    <input type="checkbox" name="flagPeriodoIndeterminado" id="flagPeriodoIndeterminado"
                           style="font-size: 6px;"/>
                    Mostrar mensagem por per�odo indeterminado.
                </div>
                <div class="form-group">
                    <label for="mensagem" class="col-lg-2 control-label">Mensagem:</label>
                    <div class="col-lg-10">
                        <?php inputTextArea('mensagem', '', 'mensagem', 1000, array('obrig' => 'N', 'habil' => 'S', 'cols' => 131, 'rows' => 10)); ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="file" class="col-lg-2 control-label">Anexar Arquivos:</label>
                    <div class="col-lg-10">
                        <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                        <input type="hidden" name="arqid" id="arqid" value="<?php echo $arqidR; ?>">

                        <?php if (!$arqidR): ?>
                            <input type="file" class="btn btn-primary start" name="file" id="file"
                                   title="Selecionar arquivo"/>
                        <?php else: ?>
                            <p class="form-control-static">
                                <?php echo $arquivo['arquivo']; ?>
                            </p>
                        <?php endif; ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="nomeArquivo" class="col-lg-2 control-label">Nome Arquivo:</label>
                    <div class="col-lg-10">
                        <input type="text" maxlength="255" class="normal form-control" name="nomeArquivo"
                               id="nomeArquivo" value="<?php echo $arquivo['angdsc']; ?>"/>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">
                            Limpar
                        </button>
                        <button class="btn btn-warning" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="inserir" type="submit">
                            <i class="glyphicon glyphicon-upload"></i>
                            Salvar
                        </button>
                    </div>
                </div>
            </form>
            <?php
            $listagem = new Simec_Listagem();
            $listagem->setCabecalho(['Perfil', 'Unidade Or�ament�ria', 'Per�odo', 'Arquivo']);
            $listagem->esconderColunas(['angid', 'arqid', 'Mensagem', 'nomeArquivo']);
            $listagem->addAcao('delete', 'excluirComunicado');
            $listagem->addAcao('edit', 'modalEditar');
            $listagem->setDados($comunicado->listarComunicados());
            $listagem->render();
            ?>
        </fieldset>
    </div>

    <script type="text/javascript">
        var urlBase;

        $(function(){
            urlBase = window.location.href.split("/");
            adicionaOpcaoTodosComboUnidadesOrcamentarias();
        });

        function adicionaOpcaoTodosComboUnidadesOrcamentarias(){
            $("select[name='unidadeorcamentaria[]']").each(function(i,e)
            {
                $(e).append('<option value="-1">Todos</option>').trigger('chosen:updated');
            });

            $("select[name='unidadeorcamentaria[]']").change(function(){
                var selectedValues = $(this).val();

                if(selectedValues){
                    var todos = selectedValues.find(function(e){
                        return e === "-1";
                    });

                    if(todos){
                        $(this ).find('option').prop('selected', true).trigger("chosen:updated");
                    }
                }
            });
        }

        function excluirComunicado(angid) {
            if (confirm('Deseja excluir o comunicado selecionado?')) {
                var url = urlBase[4]+'/comunicado/cadastrar&acao=A&requisicao=excluir';
                $.post(url, {angid: angid}, function () {
                    var url = urlBase[4]+'/comunicado/cadastrar&acao=A';
                    window.location = url;
                });
            }
        }

        function checkFlagTempoIndeterminado(){
            if($('#flagPeriodoIndeterminado, #flagPeriodoIndeterminadoModal').is(':checked')){
                $('#dataini, #datainiModal, #datafim,#datafimModal').val("");
                $('#dataini, #datainiModal, #datafim,#datafimModal').prop('disabled', true);
            }else{
                $('#dataini, #datainiModal, #datafim,#datafimModal').prop('disabled', false);
            }

        }

        $('#flagPeriodoIndeterminado, #flagPeriodoIndeterminadoModal').click(function(){
            checkFlagTempoIndeterminado();
        });

        function modalEditar(angid) {
            urlBase = window.location.href.split("/");
            var url = urlBase[4]+'/comunicado/cadastrar&acao=A&requisicao=comunicadoEditar';

            $.post(url, {angid: angid}, function (dados) {
                var comunicado = JSON.parse(dados);

                $('#perfilModal').val(comunicado['Perfil']).trigger('chosen:updated');
                $('#unidadeOrcamentariaModal').val(comunicado['Unidade']).trigger('chosen:updated');

                if(comunicado['Periodo'] !== ""){
                    var dataIni = (comunicado['Periodo'].split('/')[0]).trim();
                    var dataFim = (comunicado['Periodo'].split('/')[1]).trim();

                    $('#datainiModal').val(dataIni);
                    $('#datafimModal').val(dataFim);

                }else {
                    $('#flagPeriodoIndeterminadoModal').prop('checked', true);
                    checkFlagTempoIndeterminado();
                }
                $('#mensagemModal').text(comunicado['Mensagem']);
                $('#nomeArquivoModal').val(comunicado['nomeArquivo']);
                $('#angidModal').val(comunicado['angid']);
                $('#arqid').val(comunicado['arqid']);
            });
            $('#editarComunicadoModal').modal('show');
        }
        function voltar() {
            window.location = $('#modulo').val() + ".php?modulo=inicio&acao=C";
        }
    </script>