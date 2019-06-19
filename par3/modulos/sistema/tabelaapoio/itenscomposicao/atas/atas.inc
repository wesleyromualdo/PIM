<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
include_once APPRAIZ . 'par3/classes/model/ItensComposicaoAtaPregao.class.inc';
include_once APPRAIZ . 'par3/classes/controller/ItensComposicaoAtaPregao.class.inc';
include_once APPRAIZ . 'par3/classes/model/ItensComposicaoAtaPregaoEstado.class.inc';

$mAta = new Par3_Controller_ItensComposicaoAtaPregao();
function retornaRelatorio($arrPost)
{
    $controllerAta = new Par3_Controller_ItensComposicaoAtaPregao();
    $controllerAta->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $mAta->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'form':
        ob_clean();
        $ata = $mAta->recuperar();
        require 'form.inc';
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $mAta->listar($_REQUEST);
        die;
        break;
    case 'recuperar':
        ob_clean();
        echo $mAta->recuperar();
        die;
        break;
    case 'remover':
        ob_clean();
        echo $mAta->remover($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        echo $mAta->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Atas_Pregao.xls");
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $mAta->listar($_REQUEST);
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'upload':
        ob_clean();
        if (isset($_FILES)) {
            if ($_FILES[0]['type'] != 'application/pdf') {
                echo simec_json_encode(array('success' => 'false', 'error' => 'Somente são permitidos arquivos do formato PDF.'));
                die;
            }
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec("itenscomposicao_ata_pregao", array('atpid' => $_REQUEST['atpid']), "par3");
            $file->setUpload($_REQUEST['atpdescricaoanexo'], null, false);
            $arqid = $file->getIdArquivo();
            if (is_numeric($arqid) && $arqid > 0) {
                echo simec_json_encode(array('success' => 'true', 'arqid' => $arqid));
            } else {
                echo simec_json_encode(array('success' => 'false', 'error' => 'Falha ao gravar o arquivo'));
            }
        } else {
            echo simec_json_encode(array('error' => 'false', 'error' => 'Arquivo não enviado.'));
        }
        die;
        break;
    case 'downloadata':
        $file = new FilesSimec("itenscomposicao_ata_pregao", array('atpid' => $_REQUEST['atpid']), "par3");
        $file->getDownloadArquivo($_REQUEST['arqid']);
        break;
}
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Atas de Pregão</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Objeto:</label>
                            <div class="col-lg-8 col-md-8">
                                <input type="text" name="atpobjeto" id="atpobjeto" maxlength="255" class="form-control" value="<?php echo ($_GET['atpobjeto']); ?>"/>
                            </div>
                        </div>
                        <!--filtros-->
                        <div id="avancado" style="display: none;">
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">NºAta:</label>
                                <div class="col-lg-8 col-md-8">
                                    <input type="text" name="atpnumeroata" id="atpnumeroata" class="form-control" value="<?php echo $_GET['atpnumeroata']; ?>"/>
                                </div>
                            </div>
                            <?php echo $simec->data('vigencia', 'Vigência', $_GET['vigencia'], array('required'), array('input-size' => '8')); ?>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-md-2 col-lg-2 control-label">Situação:</label>
                                <div class="col-lg-8">
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="atpsituacao" id="atpsituacao1" <?php echo ($_GET['atpsituacao'] == 'A'?'checked':'');?> value="A">
                                        <label for="atpsituacao1"> Ativo </label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="atpsituacao" id="atpsituacao2" <?php echo ($_GET['atpsituacao'] == 'I'?'checked':'');?> value="I">
                                        <label for="atpsituacao2"> Inativo </label>
                                    </div>
                                    <div class="radio radio-default radio-inline pull-left">
                                        <input type="radio" name="atpsituacao" id="atpsituacao3" <?php echo ($_GET['atpsituacao'] == ''?'checked':'');?> value="">
                                        <label for="atpsituacao3"> Todos </label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="ibox-title">
                            <div class="ibox-tools">
                                <a id="btn-avancado">Opções Avançadas <i id="chevron" class="fa fa-chevron-down"></i></a>
                            </div>
                        </div>
                        <br>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-ata"><i class="fa fa-search"></i>Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal"
                        data-target="#modal_ataPregao"><i class="fa fa-plus"></i>Incluir
                </button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                            class="fa fa-file-excel-o"></i> Gerar Excel
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i
                            class="glyphicon glyphicon-print"></i> Imprimir
                </button>
                |
                <button type="reset" class="btn btn-success" id="limpar-formfiltro"><i class="fa fa-eraser"></i>Limpar
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="ataListagem">
            <?php echo $mAta->listar($_REQUEST); ?>
        </div>
    </div>
</div>

<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório Descrição de Iniciativas<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content">

    </div>
</div>
<br>
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_ataPregao" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="formAtaPregao" id="formAtaPregao" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Ata de Pregão - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarAtaPregao"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    var caminho = '<?php echo $urlGuia;?>';
    // Variable to store your files
    var files;

    // Add events
    $(document).on('change', 'input[type=file]', prepareUpload);

    // Grab the files and set them to our variable
    function prepareUpload() {
        files = event.target.files;
    }

    jQuery(function () {
        //mostra/esconde ibox
        $("#btn-avancado").click(function () {
            $('#avancado').slideToggle();
            //chevron up/down
            $('#chevron').toggleClass(function () {
                if ($(this).is(".fa-chevron-down")) {
                    $('#chevron').removeClass('fa-chevron-down');
                    return 'fa-chevron-up';
                } else {
                    $('#chevron').removeClass('fa-chevron-up');
                    return 'fa-chevron-down';
                }
            });
        });
        //Ao alterar período fim altera a quantidade de dias
        $(document).on('change', "#atpperiodofim", function () {
            var anofim = $(this).val();
            var anoinicio = $("#atpperiodoinicio").val();
            var diasvigencia = moment(anofim, 'DD/MM/YYYY').diff(moment(anoinicio, 'DD/MM/YYYY'), 'days');
            $('#atpvigencia').val(diasvigencia);

        });

        $(document).on('change keyup', "#atpvigencia", function () {
            var diavigencia = $("#atpvigencia").val();
            var dataanoinicio = $("#atpperiodoinicio").val();
            var dataanoinicio = moment(dataanoinicio, 'DD/MM/YYYY');
            $('#atpperiodofim').val(dataanoinicio.add(diavigencia, 'day').format('DD/MM/YYYY'));

        });

        $("#pesquisar-ata").click(function (evt) {
            window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//            evt.preventDefault();
//            var caminho = window.location.href;
//            var action = '&req=pesquisar&' + $("#form-filtro").serialize();
//            $.ajax({
//                type: "POST",
//                url: caminho,
//                data: action,
//                async: false,
//                success: function (resp) {
//                    $("#ataListagem").html(resp);
//                    return true;
//                }
//            });
        });

        jQuery('#salvarAtaPregao').click(function (evt) {
            evt.preventDefault();
            var atpid = $('#conteudo-modal').find('[name=atpid]').val();
            var msg = 'Incluir';
            if (atpid) {
                msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>" + msg + "</b> esta Ata?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = '<?php echo $urlGuia;?>';
                        var arqid = '';
                        if ($('input[type=file]').val()) {
                            var descricaoArquivo = $('input[name=atpdescricaoanexo]').val();
                            var dados = uploadFiles();
                            if (dados.success != 'true') {
                                swal("Erro.", dados.error, "error");
                                var divForm = $("#modal_ataPregao").find("[name=arqid]").parent("div");
                                divForm.closest('.form-group').addClass('has-error');
                                divForm.append("<div id='div-arqid'></div>");
                                $("#div-arqid").html("<span class='danger'>" + dados.error + "</span>");
                                return false;
                            }
                            arqid = '&arqid=' + dados.arqid;
                        }
                        var action = '&req=salvar&' + $("#formAtaPregao").serialize() + arqid;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                var res = $.parseJSON(resp);
                                if (!isNaN(parseInt(res)) && isFinite(res)) {
                                    $("#modal_ataPregao").modal("hide");
                                    $("#modal_ataPregao").find("input").val("");
                                    msgSuccess(caminho, "Ata Pregão salvo com sucesso");
                                    return false;
                                }
                                swal.close();
                                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                                    $.each(erro, function (err, errMsg) {
                                        if (errMsg < err.length) {
                                            return;//se a mensagem for vazia não retorna nada
                                        }
                                        var divFormInput = $("#modal_ataPregao").find("#" + err).parent("div");
                                        divFormInput.closest('.form-group').addClass('has-error');
                                        divFormInput.append("<div id='div-" + err + "'></div>");
                                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                                    });
                                });
                                return false;
                            }
                        });
                        return false;
                    }
                    return false;
                }
            );
        });
    });
    $("#btn-incluir").click(function () {
        $("#modal_titulo").html("Ata de Pregão - Incluir");

        carregarAta();
    });

    $(".xls").on("click", function () {
        window.location.assign(window.location.href + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=imprimir&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#impressao-content").html(resp);
                var container = $("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                table.css('width', '100%')
                table.css('border-collapse', 'collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border', '1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height', '50px');
                td.css('text-align', 'center');
                var w = window.open();
                w.document.write($('#div-impressao').html());
                w.document.title = 'Relatório - Atas de Pregão';
                w.print();
                w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });

    function carregarAta(id = null) {
        var caminho = '<?php echo $urlGuia;?>';
        var requisicao = '&req=form';
        var atpid = '';
        if (id != null) {
            var atpid = '&atpid=' + id;
        }
        $('#conteudo-modal').load(caminho + requisicao + atpid);
    }

    $("#imgInp").change(function () {
        readURL(this);
    });

    // Catch the form submit and upload the files
    function uploadFiles(descricao = null) {
        event.stopPropagation(); // Stop stuff happening
        event.preventDefault(); // Totally stop stuff happening

        var descricaoArquivo = '&atpdescricaoanexo=Arquivo';
        if (descricao != null) {
            descricaoArquivo = '&atpdescricaoanexo=' + descricao;
        }
        // START A LOADING SPINNER HERE

        // Create a formdata object and add the files
        var data = new FormData();
        $.each(files, function (key, value) {
            data.append(key, value);
        });

        var retorno;

        $.ajax({
            url: 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=atas&req=upload' + descricaoArquivo,
            type: 'POST',
            data: data,
            cache: false,
            async: false,
            processData: false, // Don't process the files
            contentType: false, // Set content type to false as jQuery will tell the server its a query string request
            success: function (data, textStatus, jqXHR) {
                var dados = $.parseJSON(data);
                retorno = dados;
            },
            error: function (jqXHR, textStatus, errorThrown) {
                // Handle errors here
                return false;
                // STOP LOADING SPINNER
            }
        });
        return retorno;
    }

    function editarAtaPregao(id) {
        $("#modal_ataPregao").modal("show");
        $("#modal_titulo").html("Ata Pregão - Editar");
        carregarAta(id);
    }

    function removerAtaPregao(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&atpid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var atp = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Ata de Pregão",
                    text: "Tem certeza que deseja remover: <b>" + atp.atpid + " - " + atp.atpnumeroata + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=remover&atpid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_ataPregao").modal("hide");
                                $("#modal_ataPregao").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho, "Ata removida com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover a Ata de Pregão.", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarListagem(params ="") {
        if (params) {
            params = '&' + params
        }
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagem' + params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#ataListagem").html(resp);
                return true;
            }
        });
    }

    jQuery("#limpar-formfiltro").click(function () {
        window.location.assign(caminho);
        $('#atpobjeto').val('');
        $('#atpnumeroata').val('');
        $('#vigencia').val('');

        $("#form-filtro").find('#atpsituacao1').removeAttr("checked");
        $("#form-filtro").find('#atpsituacao2').removeAttr("checked");
        $("#form-filtro").find('#atpsituacao3').prop('checked', true);
    });
</script>