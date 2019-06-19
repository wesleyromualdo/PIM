<?php
$controllerItemDetalhamento = new Par3_Controller_ItensComposicaoDetalhamento();
switch ($_REQUEST['reqdetalhamento']) {
    case 'salvar':
        unset($itd);
        ob_clean();
        $resp = $controllerItemDetalhamento->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'listarpregao':
        ob_clean();
        echo $controllerItemDetalhamento->listarPregao();
        die;
        break;
    case 'listarata':
        ob_clean();
        echo $controllerItemDetalhamento->listarAta();
        die;
        break;
    case 'recuperar':
        $itd = $controllerItemDetalhamento->recuperar($_REQUEST);
        ob_clean();
        require 'formDetalhamento.inc';
        die;
        break;
    case 'recuperardetalhamento':
        $itd = $controllerItemDetalhamento->recuperar($_REQUEST);
        ob_clean();
        echo $itd;
        die;
        break;
    case 'remover':
        ob_clean();
        $resp = $controllerItemDetalhamento->remover($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'getpregaocodigo':
        $sprid = $_REQUEST['sprid'];
        //Recupera Código Pregão SIGARP
        $mPregao = $db->recuperar("SELECT * FROM par.sigarppregao WHERE sprid = $sprid");
        ob_clean();
        echo $mPregao['sprnupregao'];
        die;
        break;
    case 'getatacodigo':
        $mAta = new Par3_Model_ItensComposicaoAtaPregao($_REQUEST['atpid']);
        ob_clean();
        echo $mAta->atpnumeroata;
        die;
        break;
    default:
        break;
}
?>

<div id="div-itemdetalhamento">
    <?php if ($item->itcid): ?>
        <?php require 'formDetalhamento.inc'; ?>
    <?php endif; ?>
</div>
<div class="ibox" id="painelItens">
    <div class="ibox-content">
        <div id="itensDetalhamentos">
            <?php $controllerItemDetalhamento->listar($_REQUEST); ?>
        </div>
    </div>
</div>
<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_detalhamento" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formDetalhamento" id="formDetalhamento" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo"></h3>
                </div>
                <div class="ibox-content" id="conteudo-modal" style="height:500px;overflow-x: scroll;">
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    $(document).on('click', '#salvar-detalhamentoitem', function (evt) {
        evt.preventDefault();
        var inprgid = $('#form-itemDetalhamento').find('[name=itdid]').val();
        var msg = 'Incluir';
        if (inprgid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Detalhamento de Item de Composição?",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarItemDetalhamento();
                }
                return false;
            }
        );
    });

    function salvarItemDetalhamento() {
        var caminho = window.location.href;
        var action = '&reqdetalhamento=salvar&' + $("#form-itemDetalhamento").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, "Detalhamento do Item salvo com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#form-itemDetalhamento").find("#" + err).parent("div");
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

    function removerItemDetalhamento(id) {
        var caminho = window.location.href;
        var action = 'reqdetalhamento=recuperardetalhamento&itdid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var itd = $.parseJSON(resp);
                swal({
                    title: "Remover Item de Composição",
                    text: "Tem certeza que deseja remover o Detalhamento <br> Código: <b>" + itd.itdid + "</b> ?",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&reqdetalhamento=remover&itdid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (resp != 'erro') {
                                    msgSuccess(caminho,"Detalhamento do Item de Composição removido com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover o Detalhamento do Item de Composição.", "error");
                            }
                        });
                    }
                })
            }
        });
    }
</script>