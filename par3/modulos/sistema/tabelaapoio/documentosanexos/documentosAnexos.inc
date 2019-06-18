<?php

include_once APPRAIZ .'par3/classes/controller/TipoDocumentoAnexo.class.inc';

$objControllerTipoDocumentoAnexo = new Par3_Controller_TipoDocumentoAnexo();

if (isset($_REQUEST['metodo']) && $_REQUEST['metodo'] == 'download') {
    $file = new FilesSimec();
    $file->getDownloadArquivo($_REQUEST['arqid']);
}

if (isset($_REQUEST['opcao']) && isset($_REQUEST['metodo']) && $_REQUEST['metodo'] == 'json') {
    exit($objControllerTipoDocumentoAnexo->controlarRequisicaoJson($_REQUEST['opcao']));
}

switch ($_REQUEST['requisicao']) {
    case 'salvarTipoDocumentoAnexo':
        $objControllerTipoDocumentoAnexo->inserir($_REQUEST);
        break;
    case 'desativarTipoDocumentoAnexo':
        $objControllerTipoDocumentoAnexo->desativar($_REQUEST['doaid']);
        break;
    case 'editarTipoDocumentoAnexo':
        $objControllerTipoDocumentoAnexo->editar($_REQUEST);
        break;
}
?>

<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Documentos Anexos</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" method="post" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Código:</label>
                            <div class="col-sm-2">
                                <input type="text" name="codigo" maxlength="8"
                                       value="<?php echo isset($_POST['codigo']) ? $_POST['codigo'] : ''; ?>"
                                       class="form-control"/>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Descrição:</label>
                            <div class="col-sm-7">
                                <input type="text" name="descricao" maxlength="255"
                                       value="<?php echo isset($_POST['descricao']) ? $_POST['descricao'] : ''; ?>"
                                       class="form-control"/>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="btn-incluir" onclick="buscarDocumentoAnexo()">
                    <i class="fa fa-search"></i> Buscar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" onclick="inserirDocumentoAnexo()">
                    <i class="fa fa-plus"></i> Incluir
                </button>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-sm-12 text-center">
                    <?php $objControllerTipoDocumentoAnexo->listar($_REQUEST); ?>
                </div>
            </div>
        </div>
        <div id="div-impressao" style="display: none">
            <div id="div_cabecalho" class="row col-lg-offset-2">
                <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align: center;">
                    <tr bgcolor="#ffffff">
                        <td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
                        <td nowrap align="left" valign="middle" height="1" style="padding: 5px 0 0 0;">MEC/FNDE <br> Relatório Documentos Anexos<br></td>
                        <td align="right" valign="middle" height="1" style="padding: 5px 0 0 0;">Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/></td>
                    </tr>
                </table>
                <hr style="color: black; width: 95%; margin-left: -18px;">
            </div>
            <div class="clearfix" style="margin-bottom: 10px;"></div>
            <div id="impressao-content"></div>
        </div>
    </div>
</div>

<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_documento_anexo" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formDocumentoAnexo" id="formDocumentoAnexo" class="form form-horizontal"
              enctype="multipart/form-data">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Documentos Anexos - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">

                    <input type="hidden" name="requisicao" id="requisicao" value="">
                    <input type="hidden" name="doaid" id="doaid" value="">
                    <input type="hidden" name="possuianexo" id="possuianexo" value="nao">

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Código:
                        </label>
                        <div class="col-md-2  col-md-2 col-lg-2">
                            <input name="doaidDisabled" id="doaidDisabled" type="text" class="form-control" disabled/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Descrição: *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="text" name="doadescricao" id="doadescricao" maxlength="250" class="form-control"/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Texto de Ajuda: *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                        <textarea name="doatxtajuda" id="doatxtajuda" maxlength="5000" class="form-control" rows="4"
                                  cols="50"></textarea>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Tipo:
                        </label>
                        <div class="col-sm-8">
                            <select name="tpaid[]" id="tpaid" multiple class="chosen-select"
                                    data-placeholder="Selecione">
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <div class="i-checks">
                                <label for="doadetalharitem">
                                    <input type="checkbox"  name="doadetalharitem" id="doadetalharitem">
                                    <i></i> Detalhar item </label>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <div class="i-checks">
                                <label for="doadesembolso">
                                    <input type="checkbox"  name="doadesembolso" id="doadesembolso">
                                    <i></i> Solicitação Desembolso </label>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <div class="i-checks">
                                <label for="doagerarpendencia">
                                    <input type="checkbox"  name="doagerarpendencia" id="doagerarpendencia">
                                    <i></i> Gerar pendência </label>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <div class="i-checks">
                                <label for="doadocumentofnde">
                                    <input type="checkbox"  name="doadocumentofnde" id="doadocumentofnde">
                                    <i></i> Documento do FNDE </label>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <div class="i-checks">
                                <label for="doaanalise">
                                    <input type="checkbox"  name="doaanalise" id="doaanalise">
                                    <i></i> Análise </label>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Quantidade Máxima:*
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="text" name="doaqtdmaxima" id="doaqtdmaxima" maxlength="14"
                                   onkeyup='this.value=mascaraglobal("#######",this.value);'
                                   class="form-control"/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Extensão:
                        </label>
                        <div class="col-sm-8">
                            <select name="extid[]" id="extid" multiple class="chosen-select"
                                    data-placeholder="Selecione">
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Texto de instrução:
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <textarea name="doatxtinstrucao" id="doatxtinstrucao" maxlength="5000" class="form-control" rows="4" cols="50" readonly="readonly"></textarea>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Arquivo Modelo: *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="file" name="arquivo" id="arquivo" class="form-control" style="height: auto !important;"/>
                            <div id="listaAnexo"></div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-sm-3 control-label">
                            Situação: *
                        </label>
                        <div class="col-sm-4">
                            <div class="radio radio-inline ">
                                <input name="doasituacao" id="doasituacao-true" type="radio" value="A" checked="checked" />
                                <label for="doasituacao-true">Ativo</label>
                            </div>
                            <div class="radio radio-inline ">
                                <input name="doasituacao" id="doasituacao-false" type="radio" value="I" />
                                <label for="doasituacao-false">Inativo</label>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDocumentoAnexo"
                                data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="confirmarSalvar"
                                class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script src="/includes/multifile/jquery.MultiFile.js"></script>

<script>
    $(document).ready(function () {
        $('#doagerarpendencia').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        $('#doadetalharitem').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        $('#doadesembolso').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        $('#doadesembolso').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        $('#doaanalise').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        $('#doadocumentofnde').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        $("#doaanalise").on('ifChecked', function(){
            $("#doatxtinstrucao").removeAttr('readonly');
        });

        $("#doaanalise").on('ifUnchecked', function(){
            $("#doatxtinstrucao").attr('readonly', 'readonly');
        });

        $("#doadescricao").change(function() {
            var doadescricao = $(this).val();
            var doaid = $('#doaidDisabled').val();
            $.post(
                'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosanexos&metodo=json&opcao=buscardescricaoexistente',
                {
                    doaid: doaid,
                    doadescricao: doadescricao
                },
                function (data) {
                    var tipoDocumentoAnexo = $.parseJSON(data);
                    if (tipoDocumentoAnexo.doaid) {
                        swal("Alerta", "Tipo de documento anexo já existente.", "warning");
                        $("#doadescricao").val("");
                    }
                }
            );
        });

        $('#confirmarSalvar').on('click', function () {
            if ($("#doadescricao").val() == "") {
                swal("Alerta", "O campo Descrição é obrigatório e não foi preenchido.", "warning");
                return false;
            }

            if ($("#doatxtajuda").val() == "") {
                swal("Alerta", "O campo Texto de Ajuda é obrigatório e não foi preenchido.", "warning");
                return false;
            }

            if ($("#doaqtdmaxima").val() == "") {
                swal("Alerta", "O campo Quantidade Máxima é obrigatório e não foi preenchido.", "warning");
                return false;
            }

            if($('#arquivo').val() == '' && $('#possuianexo').val() == 'nao'){
                swal("Alerta", "Por favor, anexe o arquivo modelo!", "warning");
                return false;
            }

            $("#formDocumentoAnexo").submit();
        });
    });

    function buscarDocumentoAnexo() {
        $('#form-filtro').submit();
    }

    function carregaChosenSelect(url, campoId) {
        $.getJSON(url, function (data, textStatus) {
            if (textStatus == 'success') {
                var option = '<option value="t">Todos</option>';
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                });
                $('#' + campoId).empty();
                $('#' + campoId).append($(option));
                $('#' + campoId).trigger("chosen:updated");
            }
        });
    }

    function inserirDocumentoAnexo() {
        limparDadosForm();
        $('#requisicao').val('salvarTipoDocumentoAnexo');
        $('#modal_titulo').html('Documentos Anexos - Inserir');
        carregaChosenSelect('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosanexos&metodo=json&opcao=extensoes', 'extid');
        carregaChosenSelect('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosanexos&metodo=json&opcao=tipoatendimentos', 'tpaid');
        $("#modal_documento_anexo").modal();
    }

    function limparDadosForm() {
        $('#doadetalharitem').iCheck('uncheck');
        $('#doagerarpendencia').iCheck('uncheck');
        $('#doadocumentofnde').iCheck('uncheck');
        $('#doaanalise').iCheck('uncheck');

        $('#listaAnexo').html('');

        $('.MultiFile-remove').each(function (obj) {
            $(this).click();
        });

        $('#formDocumentoAnexo').reset();

        $('#id').val('');
        $('#doaid').val('');
        $('#doadescricao').val('');
        $('#doatxtajuda').val('');
        $('#doaqtdmaxima').val('');
        $('#doatxtinstrucao').val('');
        $('#doaidDisabled').val('');
        $('#possuianexo').val('nao');

        $('#doasituacao-true').attr('checked', true);
        $('#doasituacao-true').prop('checked', true);
        $('#doasituacao-false').prop('checked', false);
        $('#doasituacao-false').attr('checked', false);
    }

    function removerTipoDocumentoAnexo(doaid) {
        swal({
            title: 'Excluir',
            text: 'Tem certeza que deseja excluir o item selecionado?',
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: 'Sim, tenho certeza!',
            closeOnConfirm: 'on',
            cancelButtonText: 'Cancelar',
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $('#doaid').val(doaid);
                $('#requisicao').val('desativarTipoDocumentoAnexo');
                $('#formDocumentoAnexo').submit();
            }
        });
    }

    function editarTipoDocumentoAnexo(doaid) {
        limparDadosForm();
        $.getJSON('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosanexos&metodo=json&opcao=buscaTipoDocumentoAnexo&doaid=' + doaid, function (data, textStatus) {
            if (textStatus == 'success') {

                $('#modal_titulo').html('Documentos Anexos - Editar');

                $('#requisicao').val('editarTipoDocumentoAnexo');
                $('#possuianexo').val('sim');

                $('#id').val(data.doaid);
                $('#doaid').val(data.doaid);
                $('#doadescricao').val(data.doadescricao);
                $('#doatxtajuda').val(data.doatxtajuda);
                $('#doaqtdmaxima').val(data.doaqtdmaxima);
                $('#doatxtinstrucao').val(data.doatxtinstrucao);
                $('#doaidDisabled').val(data.doaid);

                if (data.doadetalharitem == 't') {
                    $('#doadetalharitem').iCheck('check');
                }

                if (data.doadesembolso == 't') {
                    $('#doadesembolso').iCheck('check');
                }

                if (data.doagerarpendencia == 't') {
                    $('#doagerarpendencia').iCheck('check');
                }

                if (data.doadocumentofnde == 't') {
                    $('#doadocumentofnde').iCheck('check');
                }

                if (data.doaanalise == 't') {
                    $('#doaanalise').iCheck('check');
                }

                if (data.doasituacao === 'A') {
                    $('#doasituacao-true').attr('checked', 'checked');
                    $('#doasituacao-true').prop('checked', true);
                }

                if (data.doasituacao === 'I') {
                    $('#doasituacao-false').attr('checked', 'checked');
                    $('#doasituacao-false').prop('checked', true);
                }

                var optionExtensoes = '';

                if (data.anexos) {
                    listarAnexo(data.anexos);
                }

                if (data.extensoes.length > 0) {
                    optionExtensoes = '<option value="t">Todos</option>';
                    var IsInArr = function (needle, haystack) {
                        for (var i = 0; i < haystack.length; i++) {
                            if (haystack[i].extid == needle) {
                                return true;
                            }
                        }
                        return false;
                    };

                    $.each(data.listarExtensoes, function (i, obj) {
                        if (IsInArr(obj.codigo, data.extensoes)) {
                            optionExtensoes += '<option value="' + obj.codigo + '" selected=selected >' + obj.descricao + '</option>';
                        } else {
                            optionExtensoes += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                        }
                    });
                } else {
                    optionExtensoes = '<option value="t" selected=selected>Todos</option>';
                    $.each(data.listarExtensoes, function (i, obj) {
                        optionExtensoes += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                    });
                }

                $('#extid').empty();
                $('#extid').append($(optionExtensoes));
                $('#extid').trigger('chosen:updated');

                optionTipoAtendimentos = '';

                if (data.tipoAtendimentos.length > 0) {

                    optionTipoAtendimentos = '<option value="t">Todos</option>';

                    var IsInArr = function (needle, haystack) {
                        for (var i = 0; i < haystack.length; i++) {
                            if (haystack[i].tpaid == needle) {
                                return true;
                            }
                        }
                        return false;
                    };

                    $.each(data.listarTipoAtendimentos, function (i, obj) {
                        if (IsInArr(obj.codigo, data.tipoAtendimentos)) {
                            optionTipoAtendimentos += '<option value="' + obj.codigo + '" selected=selected >' + obj.descricao + '</option>';
                        } else {
                            optionTipoAtendimentos += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                        }
                    });
                } else {
                    optionTipoAtendimentos = '<option value="t" selected=selected>Todos</option>';

                    $.each(data.listarTipoAtendimentos, function (i, obj) {
                        optionTipoAtendimentos += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                    });
                }

                $('#tpaid').empty();
                $('#tpaid').append($(optionTipoAtendimentos));
                $('#tpaid').trigger('chosen:updated');
            }
        });

        $("#modal_documento_anexo").modal();
    }

    function listarAnexo(anexo) {
        var lista = "<div class='ibox-title' id='arquivo_" + anexo.arqid + "'>" +
            "<div class='MultiFile-label'></div>" +
            "<a class='MultiFile-remove' href='#anexo'>" +
            "<a onclick='excluirAnexo(" + anexo.arqid + ")' class='btn btn-danger btn-xs' title='Excluir'" +
            "aria-label='Close'><span aria-hidden='true'>×</span> </a> </a>   " +
            "<a title='Baixar' href='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosanexos&metodo=download&arqid=" + anexo.arqid + "'>" +
            "<i class='glyphicon glyphicon-cloud-download btn btn-warning btn-xs'></i></a>" +
            "<span disabled>" +
            "<span class='MultiFile-label' title='Selecionado: " + anexo.arqnome + "." + anexo.arqextensao + "'>" +
            "<span class='MultiFile-title'>" +
            "<input type='hidden' name='arquivo-selecionado' id='arquivo-selecionado' value='anterior'>  " +
            anexo.arqnome + "." + anexo.arqextensao +
            "</span>" +
            "</span>" +
            "</span>" +
            "</div>";

        $('#listaAnexo').html(lista);
    }

    function excluirAnexo(idAnexo) {
        swal({
            title: " Remover Documento Anexo!",
            text: "Tem certeza que deseja remover o Documento Anexo?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $("#possuianexo").val('nao');
                $("#arquivo_" + idAnexo).html("<input type='hidden' name='idsExcluirAnexo[]' value='" + idAnexo + "'>");
            }
        });
    }
</script>




