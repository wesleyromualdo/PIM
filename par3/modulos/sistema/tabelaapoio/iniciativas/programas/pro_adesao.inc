
<?php
require APPRAIZ . 'includes/cabecalho.inc';

global $proadesao;

$controllerProgramaAdesao = new Par3_Controller_ProgramaAdesao();

$prgid = $_GET['prgid'];

switch ($_REQUEST['req']) {

    case 'salvarTermoAdesao':
        ob_clean();
        $resp = $controllerProgramaAdesao->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'removerTermoAdesao':
        ob_clean();
        $controllerProgramaAdesao->remover($_REQUEST);
        break;
    case 'formTermoAdesao':
        ob_clean();
       echo $controllerProgramaAdesao->form($_REQUEST);
       die;
        break;
    case 'editarTermoAdesao':
        ob_clean();
        echo $controllerProgramaAdesao->form($_REQUEST);
        die;
        break;
    case 'recupararTermoAdesao':
        ob_clean();
        echo $controllerProgramaAdesao->recuperar($_REQUEST);
        die;
        break;
    default:
        break;
}
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">

        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-12">
                        <button type="button" class="btn btn-success" id="btn-incluirtermo" data-toggle="modal" data-target="#modal_termoAdesao">
                            <i class="fa fa-plus"></i> Incluir Termo de Adesão
                        </button>
                    </div>
                </div>
            </div>
            <div class="ibox-content">
                <div id="termoAdesaoListagem">
                    <?php echo $controllerProgramaAdesao->listar($_REQUEST)?>
                </div>
            </div>
        </div>




        <!--modal-->
        <div class="ibox float-e-margins animated modal" id="modal_termoAdesao" tabindex="-1" role="dialog"
             aria-hidden="true">
            <div class="modal-dialog modal-lg">
                <form method="post" name="formTermoAdesao" id="formTermoAdesao" class="form form-horizontal">
                    <div class="modal-content" style="overflow: scroll;height: 780px;">
                        <div class="ibox-title">
                            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                        aria-hidden="true">&times;</span></button>
                            <h3 id="modal_titulo">Termo Adesão - Incluir</h3>
                        </div>
                        <div class="ibox-content" id="conteudo-modal">
                            <!-- form aqui -->
                        </div>
                        <div class="ibox-footer">
                            <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                                <button type="submit" id="cancelarTermoAdesao"
                                        data-dismiss="modal"
                                        class="btn btn-default" <?php echo $disabled; ?>>
                                    <i class="fa fa-times-circle-o"></i> Cancelar
                                </button>
                                <button type="submit" id="salvarTermoAdesao"
                                        class="btn btn-success" <?php echo $disabled; ?>>
                                    <i class="fa fa-save"></i> Salvar
                                </button>
                            </div>
                        </div>
                    </div>
                </form>
            </div>
        </div>

    </div>
</div>

<script>

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
                var action = '&req=removerTermoAdesao&pfaid=' + id;
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


jQuery(function () {

    jQuery(document).find("#formTermoAdesao").on('click',"input[name=pfastatus ][value=I]",function(){
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará mais visível para selecionar", "warning");
    });

    jQuery('#salvarTermoAdesao').click(function (evt) {
        evt.preventDefault();
        $('.summernote').each( function() {
            $(this).val($(this).code());
        });
        var caminho = window.location.href;
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
                    msgSuccess(caminho,"Termo de Adesão Salvo com Sucesso.");
                    return false;
                }
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

        return false;
    });
});


    $(document).on('click','#btn-incluirtermo',function(evt){
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
                $("#conteudo-modal").html(resp);
                $("#modal_titulo").html("Termo de Adesão - Incluir");
            }
        });
    });



function editarTermoAdesao(id) {
    var caminho = window.location.href;
    var action = '&req=editarTermoAdesao&pfaid=' + id;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#conteudo-modal").find("input").val("");
            $("#conteudo-modal").html(resp);
            $("#modal_titulo").html("Termo Adesão - Editar");
            $("#modal_termoAdesao").modal("show");
        }
    });
}

    $("#modal_termoAdesao").on("hidden.bs.modal",function(){
        $("#modal_titulo").html("Termo de Adesão - Incluir");
    });




</script>
