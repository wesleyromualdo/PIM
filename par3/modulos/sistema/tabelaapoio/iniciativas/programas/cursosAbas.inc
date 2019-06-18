<?php
//include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'par3/classes/controller/CursosAbas.class.inc';

$controllerCursosAbas = new Par3_Controller_CursosAbas();


function retornaRelatorio($arrPost)
{
    $controllerCursosAbas = new Par3_Controller_CursosAbas();
    $controllerCursosAbas->listar($arrPost);
}

switch ($_REQUEST['req']) {
    case 'salvarCursoAbas':
        ob_clean();
        $resp = $controllerCursosAbas->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagemCursoAbas':
        ob_clean();
        echo $controllerCursosAbas->listar($_REQUEST);
        die;
        break;
    case 'editarCursosAbas':
        ob_clean();
        echo $controllerCursosAbas->form($_REQUEST);
        die;
        break;
    case 'removerCursoAbas':
        ob_clean();
        echo $controllerCursosAbas->remover($_REQUEST);
        die;
        break;
    case 'pesquisarCursoAbas':
        ob_clean();
        echo $controllerCursosAbas->listar($_REQUEST);
        die;
        break;
    case 'atualizarFormCursoAbas':
        ob_clean();
        echo $controllerCursosAbas->form($_REQUEST);
        die;
        break;
    default:
        break;
    case 'recuperarCursoAbas':
        ob_clean();
        echo $controllerCursosAbas->recuperar($_REQUEST);
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>
<?php //echo $simec->title('Tabela Apoio - Programas', 'Sistema / Tabela de Apoio / Cursos/Abas'); ?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_CursosAbas">
                    <i class="fa fa-plus"></i> Incluir Cursos/Abas
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="CursosAbasListagem">
            <?php
            $controllerCursosAbas->listar($_REQUEST);
            ?>
        </div>
    </div>
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_CursosAbas" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="formCursosAbas" id="formCursosAbas" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulocursoabas">Curso/Aba - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modalcursoabas">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarCursosAbas"
                                data-dismiss="modal"
                                class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarCursosAbas"
                                class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>


<script>

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

        jQuery(document).find("#formCursosAbas").on('click',"input[name=cursituacao ][value=I]",function(){
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará mais visível para selecinar", "warning");
        });

        jQuery('#salvarCursosAbas').click(function (evt) {
            evt.preventDefault();
            $('.summernote').each( function() {
                $(this).val($(this).code());
            });
            var caminho = window.location.href;
                    var action = '&req=salvarCursoAbas&' + $("#formCursosAbas").serialize();
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            var res = $.parseJSON(resp);
                            if (!isNaN(parseInt(res)) && isFinite(res)) {
                                $("#modal_CursosAbas").modal("hide");
                                $("#modal_CursosAbas").find("input").val("");
                                msgSuccess(caminho,"Curso/Aba Salvo com Sucesso.");
                                return false;
                            }
                            $.each(res,function(index,erro){//retorna mensagens de erro em json
                                $.each(erro,function(err,errMsg){
                                    if(errMsg < err.length){
                                        return;//se a mensagem for vazia não retorna nada
                                    }
                                    var divFormInput =  $("#modal_CursosAbas").find("[name="+err+"]").parent("div");
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


    $("#btn-incluir").click(function (evt) {
        evt.preventDefault();
        atualizarFormCursoAbas();
        $("#modal_CursosAbas").modal("show");
    });

    function editarCursosAbas(id) {
        var caminho = window.location.href;
        var action = '&req=editarCursosAbas&curid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modalcursoabas").find("input").val("");
                $("#conteudo-modalcursoabas").html(resp);
                $("#modal_titulocursoabas").html("Curso/Aba - Editar");
                $("#modal_CursosAbas").modal("show");
            }
        });
    }
    $("#modal_CursosAbas").on("hidden.bs.modal",function(){
        $("#modal_titulocursoabas").html("Curso/Aba - Incluir");
    });

    function atualizarListagemCursoAbas(params ="") {
        if(params){
            params = '&'+params
        }
        var caminho = window.location.href;
        var action = '&req=atualizarListagemCursoAbas'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#CursosAbasListagem").html(resp);
                return true;
            }
        });
    }

    $("#pesquisar-CursosAbas").click(function (evt) {
        evt.preventDefault();
        var caminho = window.location.href;
        var action = '&req=pesquisarCursoAbas&' + $("#form-filtro").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#CursosAbasListagem").html(resp);
                return true;
            }
        });
    });

    function removerCursosAbas(id) {
        var caminho = window.location.href;
        var action = '&req=recuperarCursoAbas&curid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var cur = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Curso/Aba!",
                    text: "Tem certeza que deseja remover o Curso/Aba: <b>" + cur.curid + " - " + cur.curdsc + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&req=removerCursoAbas&curid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_CursosAbas").modal("hide");
                                $("#modal_CursosAbas").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho,"O Curso/Aba foi removido com sucesso.");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover o Curso/Aba.", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarFormCursoAbas(params = "") {
        if(params){
            params = '&'+params
        }

        var caminho = window.location.href;
        var action = '&req=atualizarFormCursoAbas'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modalcursoabas").html(resp);
                return true;
            }
        });
    }
</script>