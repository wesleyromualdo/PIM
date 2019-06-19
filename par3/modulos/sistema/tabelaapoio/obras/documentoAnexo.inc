<?php
/**
 * Sistema Par3
 *
 * @author Philipe Allan Almeida <philipeph3@gmail.com>.
 * Date: 05/07/2017
 *
 */

//Controller responsável pela Obra Tipo Documento.
$objControllerObraTipoDocumento = new Par3_Controller_ObraTipoDocumento();


//Realiza o download do arquivo
if (isset($_GET['metodo']) && $_GET['metodo'] == 'download') {
    $file = new FilesSimec("obra_tipo_documento", null, "par3");
    $file->getDownloadArquivo($_GET['arqid']);
}

//Verifica se há requisições para retorno de dados Json
if (isset($_GET['opcao']) && isset($_GET['metodo']) && $_GET['metodo'] == 'json') {
    exit($objControllerObraTipoDocumento->controlarRequisicaoJson($_GET['opcao']));
}

switch ($_POST['acao']) {
    case 'salvarTipoDocumento':
        $arrPost = $_POST;
        ob_clean();
        $objControllerObraTipoDocumento = new Par3_Controller_ObraTipoDocumento();
        $objControllerObraTipoDocumento->salvar($arrPost);
        break;
    case 'inativar':
        $id = $_POST['id'];
        $objControllerObraTipoDocumento->inativar($id);
        break;
    case 'validar':
        $objControllerObraTipoDocumento->validar($_POST);
        break;
    case 'editar':
        $objControllerObraTipoDocumento->editar($_POST);
        break;
}

switch ($_GET['requisicao']) {
	case 'xls' :
		ob_clean();
		$objControllerObraTipoDocumento->listar($_REQUEST);
		header("Content-Disposition: attachment; filename=Relatorio_Obras_Documentos.xls");
		die ();
		break;
	case 'imprimir' :
		ob_clean();
        $objControllerObraTipoDocumento->listar($_REQUEST);
        die();
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
                <input type="hidden" name="acao" id="acao" value="pesquisar">
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
                |
                <button type="submit" class="btn btn-primary xls" data-title="Ajuda - Exportar XLS"
                        data-content="Exporta o conteúdo do relatório para o formato XLS (Excel)." data-id-lista="otdid"
                        data-original-title="" title="" style="width:110px;">
                <i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-sm-12 text-center">
                    <?php
                    $objControllerObraTipoDocumento->listar($_REQUEST);
                    ?>
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

                    <div id="inputs-hidden">
                        <input type="hidden" name="acao" id="acao" value="salvarTipoDocumento">
                        <input type="hidden" name="otdid" id="otdid" value="">
                        <input type="hidden" name="id" id="id" value="">
                    </div>

                    <input type="hidden" name="possui-anexo" id="possui-anexo" value="nao">
                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Código:
                        </label>
                        <div class="col-md-2  col-md-2 col-lg-2">
                            <input name="otdidDisabled" id="otdidDisabled" type="text" class="form-control" disabled/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Descrição:*
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="text" name="otddsc" id="otddsc" maxlength="250" class="form-control"/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Texto de Ajuda: *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                        <textarea name="otdajuda" id="otdajuda" maxlength="5000" class="form-control" rows="4"
                                  cols="50"><?php echo isset($dadosObra['otdajuda']) ? $dadosObra['otdajuda'] : ''; ?></textarea>
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
                            Tamanho Máximo (Kb): *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="text" name="otdtamanho_maximo" id="otdtamanho_maximo" maxlength="14"
                                   onkeyup='this.value=mascaraglobal("#######",this.value);'
                                   class="form-control"/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Quantidade Mínima: *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="text" name="otdqtd_minima" id="otdqtd_minima" maxlength="14"
                                   onkeyup='this.value=mascaraglobal("#######",this.value);'
                                   class="form-control"/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Quantidade Máxima:*
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="text" name="otdqtd_maximo" id="otdqtd_maximo" maxlength="14"
                                   onkeyup='this.value=mascaraglobal("#######",this.value);'
                                   class="form-control"/>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Análise de Engenharia:*
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <textarea name="otdanalise_engenharia" id="otdanalise_engenharia" maxlength="5000" class="form-control" rows="4"
                                      cols="50"><?php echo isset($dadosObra['otdanalise_engenharia']) ? $dadosObra['otdanalise_engenharia'] : ''; ?></textarea>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <div class="i-checks">
                                <label for="otdanalise_textorico">
                                    <input type="checkbox"  name="otdanalise_textorico" id="otdanalise_textorico">
                                    <i></i> Habilitar texto rico na Análise </label>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
                            Arquivo Modelo: *
                        </label>
                        <div class="col-md-8  col-md-8 col-lg-8">
                            <input type="file" name="anexo" id="anexo" class="form-control" style="height: auto !important;"/>
                            <div id="listaAnexo"></div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-sm-3 control-label">
                            Situação: *
                        </label>
                        <div class="col-sm-4">
                            <div class="radio radio-inline ">
                                <input name="otdsituacao" id="otdsituacao-true" type="radio" value="A" checked="checked"
                                       onclick="obrasEtapasSim()"
                                    <?php echo isset($dadosObra['otdsituacao']) && $dadosObra['otdsituacao'] == true ? 'checked' : ''; ?> />
                                <label for="otdsituacao-true">Ativo</label>
                            </div>
                            <div class="radio radio-inline ">
                                <input name="otdsituacao" id="otdsituacao-false" type="radio" value="I"
                                       onclick="obrasEtapasNao()"
                                    <?php echo isset($dadosObra['otdsituacao']) && $dadosObra['otdsituacao'] == false ? 'checked' : ''; ?> />
                                <label for="otdsituacao-false">Inativo</label>
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
//    $("#otdanalise_textorico").click(function () {
//        console.log($('#otdanalise_textorico').is(':checked'));
//        if ($(this).is(':checked')) {
//            $('#otdanalise_textorico').attr('checked',true);
//            $(this).val('S');
//        } else {
//            $('#otdanalise_textorico').attr('checked',false);
//            $(this).val('N');
//        }
//    });

    $(document).ready(function () {
        $('#otdanalise_textorico').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });
    });

    $(document).ready(function () {
        $('#confirmarSalvar').click(function () {

            //Descrição
            if ($('#otddsc').val().length <= 0) {
                alert("O campo Descrição é  obrigatório e não foi preenchido.");
                return false;
            }

            //Texto de Ajuda
            if ($('#otdajuda').val().length <= 0) {
                alert("O campo Texto de Ajuda é  obrigatório e não foi preenchido.");
                return false;
            }

            //Tamanho Máximo (Kb)
            if ($('#otdtamanho_maximo').val().length <= 0) {
                alert("O campo Tamanho Máximo (Kb) é  obrigatório e não foi preenchido.");
                return false;
            }

            // Quantidade Mínima
            if ($('#otdqtd_minima').val().length <= 0) {
                alert("O campo  Quantidade Mínima é  obrigatório e não foi preenchido.");
                return false;
            }

            // Quantidade Máxima
            if ($('#otdqtd_maximo').val().length <= 0) {
                alert("O campo  Quantidade Máxima é  obrigatório e não foi preenchido.");
                return false;
            }

            var isChecked = jQuery("input[name=otdsituacao]:checked").val();
            if (!isChecked) {
                alert("O campo Situação é  obrigatório e não foi preenchido.");
                return false;
            }

            // Análise de Engenharia
            if ($('#otdanalise_engenharia').val().length <= 0) {
                alert("O campo  Análise de Egenharia é  obrigatório e não foi preenchido.");
                return false;
            }

            var numeroAnexo = 0;

            $('#arquivo-selecionado').each(function (i) {
                numeroAnexo = numeroAnexo + 1;
            });

            if (numeroAnexo < 1) {
                alert("O campo Arquivo Modelo é  obrigatório e não foi preenchido.");
                return false;
            }

            /**
             * Validação da quantidade de arquivos anexados.
             */
            if ($("#formDocumentoAnexo #acao").val() === 'editar' && $("#possui-anexo").val() === 'sim' && $('#arquivo-novo').length) {
                alert("Permitido upload de somente 1 arquivo.");
                return false;
            }

            var otdid = $('#conteudo-modal').find('[name=id]').val();
            var msg = 'Incluir';
            if (otdid) {
                msg = 'Alterar';
            }

            var dados = $('#formDocumentoAnexo').serialize();

            $.ajax({
                url: "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentoAnexo",
                data: {
                    dados: dados,
                    acao: 'validar',
                },
                dataType: "json",
                type: "POST",
                success: function (data) {
                    var status = data.status;
                    var mensagem = data.mensagem;
                    if (status === "false") {
                        alert(mensagem);
                        return false;
                    } else {
                        swal({
                            title: "Confirmar",
                            text: "Deseja <b>" + msg + "</b> este Documento Anexo?",
                            type: "success",
                            html: true,
                            showCancelButton: true,
                            confirmButtonColor: "#DD6B55",
                            cancelButtonText: "Não",
                            confirmButtonText: "Sim",
                            closeOnConfirm: false
                        }, function (isConfirm) {
                            if (isConfirm) {
                                $('#formDocumentoAnexo').submit();
                            }
                            return false;
                        });
                    }
                }
            });
        });

        /**
         *Configuração do plugin de upload
         * Utilizado para realizar a seleção de vários arquivos para upload
         */
        $('#anexo').MultiFile({
            max: 1,
            maxsize: -1,
            accept: 'dwg|dxf|rar|zip|jpg|jpeg|docx|doc|xls|xlsx|pdf',
            STRING: {
                remove: '<button type="button" class="btn btn-danger btn-xs" title="Excluir" aria-label="Close"><span aria-hidden="true">&times;</span></button>',
                file: '$file <input type="hidden" name="arquivo-selecionado" id="arquivo-selecionado" value="novo"> <input type="hidden" name="arquivo-novo" id="arquivo-novo" value="sim"> ',
                selected: "Selecionado: $file",
                denied: 'Arquivo não permitido. </br> Anexar arquivo somente nos seguintes formatos: .dwg, .dxf, .rar, .zip, .jpg, .jpeg, .doc, .docx, .pdf,  .xls, .xlsx',
                duplicate: 'Arquivo já selecionado.',
                toomuch: 'O arquivo selecionado excede o tamanho máximo permitido ($size)',
                toomany: 'Permitido upload de somente 1 arquivo.',
                toobig: 'Limite máximo de 50 Mb por arquivo'
            },
            error: function (s) {
                alert(s);
            }
        });

    });

    $('#otdsituacao-false').click(function () {
        alert("Este Documentos Anexo não aparecerá para novas iniciativas.");
    });

    /**
     * Realiza a busca
     */
    function buscarDocumentoAnexo() {
        
        $('#form-filtro').submit();
    }

    function inserirDocumentoAnexo() {

        limparDadosForm();

        /**
         * Devido alguns problemas no template, foi necessário mudar os valores dessa forma.
         */
        $("#inputs-hidden").html(
            "<input type='hidden' name='acao' id='acao' value='salvarTipoDocumento'>" +
            "<input type='hidden' name='otdid' id='otdid' value=''>" +
            "<input type='hidden' name='id' id='id' value=''>"
        );

        $("#modal_titulo").html("Documentos Anexos - Inserir");

        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentoAnexo&metodo=json&opcao=extensoes', function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!

                var option = '<option value="t">Todos</option>';

                //Monta a combo
                $.each(data, function (i, obj) {
                    option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                })

                //Envia as opções para o select
                $('#extid').empty();
                var newOption = $(option);
                $('#extid').append(newOption);
                $('#extid').trigger("chosen:updated");
            }
        });

        $("#modal_documento_anexo").modal();

    }


    /**
     *
     * @param otdid
     */
    function editarDocumentoAnexo(otdid) {
        $("#otdanalise_textorico").iCheck('uncheck');
        limparDadosForm();
        //Envia o código via Ajax pelo método Get
        $.getJSON('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentoAnexo&metodo=json&opcao=documentoanexo&otdid=' + otdid, function (data, textStatus) {
            if (textStatus == 'success') { //verifica se o status está tudo ok!

                $("#modal_titulo").html("Documentos Anexos - Editar");

                /**
                 * Devido alguns problemas no template, foi necessário mudar os valores dessa forma.
                 */
                $("#inputs-hidden").html(
                    "<input type='hidden' name='acao' id='acao' value='editar'>" +
                    "<input type='hidden' name='otdid' id='otdid' value='" + otdid + "'>" +
                    "<input type='hidden' name='id' id='id' value='" + otdid + "'>"
                );

                $('#otdid').val(data.otdid);
                $('#id').val(data.otdid);
                $('#otdidDisabled').val(data.otdid);
                $('#otddsc').val(data.otddsc);
                $('#otdajuda').val(data.otdajuda);
                $('#otdqtd_maximo').val(data.otdqtd_maximo);
                $('#otdqtd_minima').val(data.otdqtd_minima);
                $('#otdtamanho_maximo').val(data.otdtamanho_maximo);
                $('#otdanalise_engenharia').val(data.otdanalise_engenharia);


                $("#otdanalise_textorico").iCheck('uncheck');
                if(data.otdanalise_textorico == 'S'){
                    console.log(data.otdanalise_textorico);
                    $("#otdanalise_textorico").iCheck('check');
                }

                if (data.otdsituacao === 'A') {
                    $("#otdsituacao-true").attr('checked', 'checked');
                    $("#otdsituacao-true").prop("checked", true);
                }

                if (data.otdsituacao === 'I') {
                    $("#otdsituacao-false").attr('checked', 'checked');
                    $("#otdsituacao-false").prop("checked", true);
                }

                var option = '';

                if (data.anexos) {
                    listarAnexo(data.anexos);
                }

                /**
                 * Verifica se possui extensões selecionadas.
                 */
                if (data.extesoes.length > 0) {

                    option = '<option value="t">Todos</option>';

                    /**
                     * Função anônima para verificar se o registro está no array.
                     */
                    var IsInArr = function (needle, haystack) {
                        for (var i = 0; i < haystack.length; i++) {
                            if (haystack[i].extid == needle) {
                                return true;
                            }
                        }
                        return false;
                    };

                    //Monta a combo com os selecionados
                    $.each(data.listarExtensoes, function (i, obj) {
                        if (IsInArr(obj.codigo, data.extesoes)) {
                            option += '<option value="' + obj.codigo + '" selected=selected >' + obj.descricao + '</option>';
                        } else {
                            option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                        }
                    });

                } else {

                    option = '<option value="t" selected=selected>Todos</option>';

                    //Monta a combo sem os selecionados
                    $.each(data.listarExtensoes, function (i, obj) {
                        option += '<option value="' + obj.codigo + '">' + obj.descricao + '</option>';
                    });

                }

                //Envia as opções para o select
                $('#extid').empty();
                var newOption = $(option);
                $('#extid').append(newOption);
                $('#extid').trigger("chosen:updated");

            }
        });

        $("#modal_documento_anexo").modal();

    }

    /**
     * Função para excluir o documento anexo.
     */
    function removerDocumentoAnexo(id) {

        swal({
                title: " Excluir",
                text: "Tem certeza que deseja excluir o item selecionado?",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {

                    /**
                     * Devido alguns problemas no template, foi necessário mudar os valores dessa forma.
                     */
                    $("#inputs-hidden").html(
                        "<input type='hidden' name='acao' id='acao' value='inativar'>" +
                        "<input type='hidden' name='otdid' id='otdid' value='" + id + "'>" +
                        "<input type='hidden' name='id' id='id' value='" + id + "'>"
                    );

                    $('#formDocumentoAnexo').submit();
                }
            });

    }

    function limparDadosForm() {

        $("#otdanalise_textorico").iCheck('uncheck');
        $("#listaAnexo").html('');

        /**
         * Remove todos os anexos
         */
        $('.MultiFile-remove').each(function (obj) {
            $(this).click();
        });

        $("#formDocumentoAnexo").reset();

        $('#id').val('');
        $('#otdid').val('');
        $('#otddsc').val('');
        $('#otdajuda').val('');
        $('#otdqtd_maximo').val('');
        $('#otdqtd_minima').val('');
        $('#otdtamanho_maximo').val('');
        $('#otdidDisabled').val('');
        $('#otdanalise_engenharia').val('');

        $("#otdsituacao-true").attr('checked', true);
        $("#otdsituacao-true").prop("checked", true);
        $("#otdsituacao-false").prop("checked", false);
        $("#otdsituacao-false").attr('checked', false);

    }

    /**
     *
     * @param anexo
     */
    function listarAnexo(anexo) {

        $("#possui-anexo").val('sim');

        var lista = "<div class='ibox-title' id='arquivo_" + anexo.arqid + "'>" +
            "<div class='MultiFile-label'></div>" +
            "<a class='MultiFile-remove' href='#anexo'>" +
            "<a onclick='excluirAnexo(" + anexo.arqid + ")' class='btn btn-danger btn-xs' title='Excluir'" +
            "aria-label='Close'><span aria-hidden='true'>×</span> </a> </a>   " +
            "<a title='Baixar' href='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentoAnexo&metodo=download&arqid=" + anexo.arqid + "'>" +
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

        $("#listaAnexo").html(lista);

    }

    /**
     * Excluir anexo
     */
    function excluirAnexo(idAnexo) {

        swal({
                title: " Remover Documento Anexo!",
                text: "Tem certeza que deseja remover o Documento Anexo?",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {
                    $("#possui-anexo").val('nao');
                    $("#arquivo_" + idAnexo).html("<input type='hidden' name='idsExcluirAnexo[]' value='" + idAnexo + "'>");
                }
            });
    }

    $(".xls").on("click", function () {
        var codigo = $('[name=codigo]').val();
        var descricao = $('[name=descricao]').val();
        window.location.assign(window.location.href + "&requisicao=xls&codigo=" + codigo + "&descricao=" +descricao);
    });

    $("#imprimir").on("click", function () {
    	var codigo = $('[name=codigo]').val();
        var descricao = $('[name=descricao]').val();
        var caminho = window.location.href;
        var action = "&requisicao=imprimir&codigo=" + codigo + "&descricao=" + descricao;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
            	//console.log('resp-impressao:'); console.log(resp);
            	console.log($("#impressao-content").html());
                $("#impressao-content").html(resp);
                var container = $("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                var tr = container.find('tr');
                //table.css('max-width', '50px');
                table.css('width', '100%');
                table.css('border-collapse', 'collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border', '1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height', '50px');
                td.css('text-align', 'center');
                var w = window.open();
                w.document.write($('#div-impressao').html());
                w.document.title = 'Relatório - Documentos Anexos';
                w.print();
                w.close();
                $("#impressao-content").html("");
                //atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });


</script>




