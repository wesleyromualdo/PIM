<?php
include_once APPRAIZ . 'par3/classes/controller/IniciativaProgramasTermoAdesao.class.inc';

$cotrollerTermoAdesao = new Par3_Controller_IniciativaProgramasTermoAdesao();

switch ($_REQUEST['req']) {
    case 'salvarTermoAdesao':
        ob_clean();
        $resp = $cotrollerTermoAdesao->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagemTermoAdesao':
        ob_clean();
        echo $cotrollerTermoAdesao->listar($_REQUEST);
        die;
        break;
    case 'formTermoAdesao':
        ob_clean();
        echo $cotrollerTermoAdesao->form($_REQUEST);
        die;
        break;
    case 'removerTermoAdesao':
        $cotrollerTermoAdesao->remover($_REQUEST);
        break;
    case 'recuperarTermoAdesao':
        ob_clean();
        echo $cotrollerTermoAdesao->recuperar($_REQUEST);
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <button type="button" class="btn btn-success" id="btnIncluirTermoAdesao" data-toggle="modal" data-target="#modal_termoAdesao">
                    <i class="fa fa-plus"></i> Incluir Termo Adesão
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="termoAdesaoListagem">
            <?php
                 $cotrollerTermoAdesao->listar($_REQUEST);
            ?>
        </div>
    </div>
</div>
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_termoAdesao" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="formTermoAdesao" id="formTermoAdesao" class="form form-horizontal">
            <div class="modal-content animated">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                    <h3 id="modal_tituloTermoAdesao">Termo de Adesão - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudoModalTermo">
                <!--formulário carrega aqui-->
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="button" id="cancelarDescIniciativa"
                                data-dismiss="modal" class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="salvarTermoAdesao" class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    jQuery(document).find("#formTermoAdesao").on('click', "[name=tersituacao ][value=I]", function () {
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para ser selecionado posteriormente", "warning");
    });

    $(document).on('click','#btnIncluirTermoAdesao',function(evt){
        evt.preventDefault();
        $('.summernote').each( function() {
            $(this).val($(this).code());
        });
        var caminho = window.location.href;
        var action = '&req=formTermoAdesao';
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudoModalTermo").html(resp);
                $("#modal_tituloTermoAdesao").html("Termo de Adesão - Incluir");
                $("#modal_termoAdesao").modal("show");
            }
        });
    });

    //mostra/esconde ibox
    $(document).on('click',"#btnMensagensTermo",function () {
        $('#mensagensTermo').slideToggle();
        //chevron up/down
        $('#chevronTermo').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $('#chevronTermo').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#chevronTermo').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //mostra/esconde ibox
    $(document).on('click',"#btnOutrosTermo",function () {
        $('#outrosTermo').slideToggle();
        //chevron up/down
        $('#chevronOutrosTermo').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $('#chevronOutrosTermo').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#chevronOutrosTermo').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
$(function () {
    jQuery('#salvarTermoAdesao').click(function (evt) {
        $('.summernote').each( function() {
            $(this).val($(this).code());
        });
        console.log($("#formTermoAdesao").serialize());
        evt.preventDefault();
        var caminho = window.location.href;
//        $('#formTermoAdesao').isValid(function (isValid) {
//            if (isValid) {
                var action = '&req=salvarTermoAdesao&' + $("#formTermoAdesao").serialize();
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);
                        if (!isNaN(parseInt(res)) && isFinite(res)) {
                            $("#modal_termoAdesao").modal("hide");
                            $("#modal_termoAdesao").find("input").val("");
                            msgSuccess(caminho,"Termo de Adesão salvo com sucesso");
                            return false;
                        }
                        $('#outrosTermo').slideDown();
                        $('#chevronOutrosTermo').removeClass('fa-chevron-down');
                        $('#chevronOutrosTermo').addClass('fa-chevron-up');
                        $.each(res,function(index,erro){//retorna mensagens de erro em json
                            $.each(erro,function(err,errMsg){
                                if(errMsg < err.length){
                                    return;//se a mensagem for vazia não retorna nada
                                }
                                var divFormInput =  $("#modal_termoAdesao").find("[name="+err+"]").parent("div");
                                divFormInput.closest('.form-group').addClass('has-error');
                                divFormInput.append("<div id='div-"+err+"'></div>");
                                $("#div-"+err).html("<span class='danger'>"+errMsg+"</span>");
                            });
                        });
                        return false;
                    }
                });
//            }
//            return false;
//        });
    });
});

function removerTermoAdesao(id) {
var caminho = window.location.href;
        var confirmado = false;
        swal({
            title: "Remover Termo de Adesão!",
            text: "Tem certeza que deseja remover termo de Adesão?",
            type: "warning",
            html: true,
            showCancelButton: true,
            confirmButtonColor: "#DD6B55",
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action = '&req=removerTermoAdesao&terid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        $("#modal_ciclo").modal("hide");
                        $("#modal_ciclo").find("input").val("");
                        if (resp != 'erro') {
                            msgSuccess(caminho,"Termo de Adesão removido com sucesso");
                            return false;
                        }
                        swal("Erro.", "Ocorreu um erro ao remover Termo de Adesão.", "error");
                    }
                });
            }
        })
}

function editarTermoAdesao(id) {
    $('.summernote').each( function() {
        $(this).val($(this).code());
    });
    var caminho = window.location.href;
    var action = '&req=formTermoAdesao&terid=' + id;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#conteudoModalTermo").find("input").val("");
            $("#conteudoModalTermo").html(resp);
            $("#modal_tituloTermoAdesao").html("Termo de Adesão - Editar");
            $("#modal_termoAdesao").modal("show");
        }
    });
}

function atualizarListagemTermoAdesao(params ="") {
    if(params){
        params = '&'+params
    }
    var caminho = window.location.href;
    var action = '&req=atualizarListagemTermoAdesao'+params;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#termoAdesaoListagem").html(resp);
            return true;
        }
    });
}
</script>