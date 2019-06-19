<?php
$dados['obrid'] = $_REQUEST['obrid'];
$dados['inuid'] = $_REQUEST['inuid'];
$controllerObradocumentos = new Par3_Controller_ObraDocumentos();

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerObradocumentos->salvar($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $controllerObradocumentos->remover($_REQUEST);
        die;
        break;
    case 'validarArquivo':
        ob_clean();
        $jason = $controllerObradocumentos->retornaDadosTipoDocumento($_REQUEST);
        echo $jason;
        die;
        break;
    case 'download':
        ob_clean();
        $arqid = $_GET['arqid'];
        if ($arqid) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec(null, null, "par3");
            $file->getDownloadArquivo($arqid);
        }
        die();
        break;
    case 'mostraranalise':
        $otdid = $_GET['otdid'];
        ob_clean();
//        $mObraTipoDocumento = new Par3_Model_ObraTipoDocumento($otdid);
        $mPerguntas = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $mPerguntas->carregarPerguntaDocumentosAnexos($otdid,$_GET['obrid']);
//         echo $mObraTipoDocumento;
        die();
        break;
    case 'listarPendenciasAnalise':
        $otdid = $_GET['otdid'];
        ob_clean();
        $mObra = new Par3_Model_Obra($obrid);
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);

        if((in_array($esd['esdid'],$mAnalise->estadosEdicao()) || in_array($esd['esdid'],$mAnalise->estadoValidacao())) && !array_intersect(pegaPerfilGeral($_SESSION['usucpf']),$mAnalise->perfisEdicao())){
            die();
        }
        $dados = $mAnalise->listarPendenciasPorAba(Par3_Model_ObraAba::DOCUMENTOS_ANEXOS, $_GET['obrid']);
        if(!is_array($dados)){
            die;
        }
        echo simec_json_encode($dados);
        die();
        break;
    case 'salvaranalisedocumento':
        $mAnalise = new Par3_Controller_AnaliseEngenhariaObraDocumentosRespostas();
        ob_clean();
        echo $mAnalise->salvar($_POST);die;
        break;
    default:
        break;
}
$disabled = 'disabled';
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <input type="hidden" name="req" value="salvar"/>
        <div class="ibox">
            <div class="ibox-title">
                <h3 class="center">Documentos Anexo</h3>
                <h3 class="center"><?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
            </div>
            <div class="ibox-content">
                <?php
                $controllerObradocumentos->listaDocumentos($dados);
                ?>
            </div>
            <div class="ibox-footer">
                <div class="row col-md-offset-0">

                </div>
            </div>
        </div>
    </div>
</div>
<br>
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="form" id="form" class="form form-horizontal" enctype="multipart/form-data">
            <input name="obrid" id="obrid" value="" type="hidden">
            <input name="inuid" id="inuid" value="<?=$dados['inuid']?>" type="hidden">
            <input name="otdid" id="otdid" value="" type="hidden">
            <input name="req" id="req" value="salvar" type="hidden">

            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Documentos Anexo - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                    <?php
                    echo $simec->input('arqid', 'Anexo', null, array('type' => 'file', $requidoArquivo));
                    ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                        <button type="button" id="cancelar" data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="salvar"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<!--modal-->
<div class=" float-e-margins animated modal" id="modal-analise" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="form" id="form-analise" class="form form-horizontal">
            <input type="hidden"name="req" value="salvaranalisedocumento">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Análise</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal"></div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                        <button type="button" id="cancelar" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvar-analise" class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script>

    $(function(){
        listarPendenciasAnalise();
        $("[id^=text_panel_]").destroy();
    });

    function listarPendenciasAnalise(){
        $(document).find('[id^="otd_"]').parent('td').parent('tr').removeClass('danger');
        $('#loading').hide();
        $.getJSON(window.location.href+'&req=listarPendenciasAnalise',function(data,value){
            $.each(data,function(key,obj){
                if(obj.aed_pendencia === "S" || obj.aer_corrigido == 'S'){
                    var tr = $(document).find('#otd_'+obj.otdid).parent('td').parent('tr');
                    tr.addClass('danger');
                    var pergunta  = obj.otdanalise_engenharia == ""? "O documento informado está de acordo com as especificações exigidas?":obj.otdanalise_engenharia;
                    tr.attr('title','Clique para visualizar a pendência');
                    var panelPendencia = "<div class='col-lg-12'><div class='panel panel-danger'><div class='panel-heading'>"+pergunta+"</div><div class='panel-body'><div  id='text_panel_"+obj.otdid+"'>"+obj.aed_observacao.replace(/\\/g, "")+"</div></div></div></div>";
                    var trPendencia = '<tr id="pendencia_'+obj.otdid+'"><td colspan="7">'+panelPendencia+'</td></tr>';
                    $(document).find('#otd_'+obj.otdid).parent('td').parent('tr').after(trPendencia).show();
                    var panel = $('#text_panel_'+obj.otdid);
                    $(panel).destroy();
                }
            });
        });
        $("[id^=text_panel_]").destroy();
    }

    $(document).on('change',"[name=aed_pendencia]",function () {
        if ($(this).is(':checked')) {
            $(this).attr('checked', true);
            $(this).parent('label').removeClass('btn-primary').addClass('btn-danger');
            $(this).val('S');
            $(document).find('#bodypendencia').slideDown();
        } else {
            $(this).attr('checked', false);
            $(this).val('N');
            $(this).parent('label').removeClass('btn-danger').addClass('btn-primary');
            $(document).find('#bodypendencia').slideUp();
        }
    }).change();

    function anexarDocumentoObra(id, obrid) {
        $('#obrid').val(obrid);
        $('#otdid').val(id);
        $('#arqid').val('');
        $("#modal").modal("show");
    }


    jQuery(function () {


        jQuery(document).find("#form").on('click', "input[name=indsituacao ][value=I]", function () {
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível.", "warning");
        });

        jQuery('#salvar').click(function (evt) {
            if($('#arqid').val() == '') {
                alert('Adicione um arquivo');
                return false;
            }
            var caminho = window.location.href;
            $('#req').val('validarArquivo');
            var erros = extArquivo = '';
            var action = '&req=validarArquivo&' + $("#form").serialize();
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var dadosValidacao = $.parseJSON(resp);
                    var file = $("#arqid")[0].files[0];

                    if (file === undefined) {
                        erros = "Obrigatório selecionar o arquivo.";
                    } else {

                        if (dadosValidacao.otdqtd_maximo > 0 && dadosValidacao.count >= dadosValidacao.otdqtd_maximo) {
                            erros += "Limite de arquivos excedido para esse tipo de anexo.<br>";
                        }
                        if (dadosValidacao.otdtamanho_maximo > 0 && file.size > (dadosValidacao.otdtamanho_maximo * 1024 * 1024)) {
                            erros += "Esse aquivo não pode ser maior que " + dadosValidacao.otdtamanho_maximo + "Mb<br>";
                        } else if (dadosValidacao.otdtamanho_minimo > 0 && file.size < (dadosValidacao.otdtamanho_minimo * 1024 * 1024)) {
                            erros += "Esse aquivo deve ter no mínimo " + dadosValidacao.otdtamanho_minimo + "Mb<br>";
                        }
                        extArquivo = file.name.substring(file.name.lastIndexOf(".") + 1).toLowerCase();

                        if (dadosValidacao.extdesc != '[null]' && dadosValidacao.extdesc.toLowerCase().lastIndexOf(extArquivo) < 0) {
                            erros += "Essa extenção não é válida para esse tipo de anexo.<br>";
                        }
                    }

                    if (erros = '') {
                        alert(erros);
                        return false;
                    }

                    swal({
                            title: "Confirmar",
                            text: "Deseja <b>Incluir</b> esse Documento?",
                            type: "success",
                            html: true,
                            showCancelButton: true,
                            confirmButtonColor: "#DD6B55",
                            cancelButtonText: "Não",
                            confirmButtonText: "Sim",
                            closeOnConfirm: false
                        }, function (isConfirm) {
                            if (isConfirm) {
                                $("#req").val('salvar');
                                $("#form").submit();
                            }
                            return false;
                        }
                    );

                }
            });
        });
    });

    function modalAjuda(otdid,obrid){
        $('#req').val('validarArquivo');
        var caminho = window.location.href;
        var action = '&req=validarArquivo&otdid=' + otdid + '&obrid=' + obrid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var dadosAjuda = $.parseJSON(resp);
                swal("Ajuda", dadosAjuda.otdajuda);
            }
        });
    }

    function modalAnaliseDocumento(otpid,otdid){
            console.log(otpid +' '+otdid);
        var caminho = window.location.href;
        var action = '&req=mostraranalise&otpid=' + otpid + '&otdid=' + otdid;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
//                var descricaoAnalise = $.parseJSON(resp);
//                console.log(descricaoAnalise.otdajuda);
                $('#modal-analise').find('#conteudo-modal').html(resp);
                $('#modal-analise').modal('show');
            }
        });
        $("[id^=text_panel_]").destroy();
    }


    $('#salvar-analise').click(function (evt) {
        console.log($('#form-analise').serialize());
        evt.preventDefault();
        swal({
            title: "Confirmar",
            text: "Salvar Análise?",
            type: "info",
            html: true,
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim"
        }, function (isConfirm) {
            if (isConfirm) {
                $('#form-analise').submit();
            }
        });
        return false;
    });

    function removerAnexo(id) {
        swal({
                title: "Confirmar",
                text: "Deseja <b>Remover</b> esse Documento?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim"
            }, function (isConfirm) {
                if (isConfirm) {
                    var caminho = window.location.href;
                    var action = '&req=remover&odoid=' + id;
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if(resp == "true"){
//                                 $('#linhaarquivo'+id).hide(800);
                                window.location.href = window.location.href+'<?php echo (isset($_REQUEST['toReturn']))?$_REQUEST['toReturn']:" "?>';
                            }
                        }
                    });
                }
                return false;
            }
        );
    }
    function msgQuantidadeMaxima() {
        swal({
                title: "Atenção",
                html: true,
                text: "Não é possível anexar mais documentos, pois a quantidade máxima de arquivo(s) anexo(s) foi atingida para esse item.",
                type: "warning",
                allowEscapeKey: false
            },
            function() {
            }
        );
    }
</script>