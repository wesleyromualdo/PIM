<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
$mInto = new Par3_Model_IniciativaTiposObjeto();
$rsInto = simec_preparar_array($mInto->recuperarTodos('intoid as codigo,intodsc as descricao ', array("intosituacao = 'A' "), 'intodsc ASC'));

//Recupera o grupo para edição
$controllerItemComposicaoGrupos = new Par3_Controller_ItensComposicaoGrupos();
$igr = $controllerItemComposicaoGrupos->recuperar();

//instancia tipo de iniciativas objeto
$grupoTipoObj = new Par3_Model_ItensComposicaoGruposTiposObjeto();

//instancia grupo item
$controllerGrupoItem = new Par3_Controller_ItensComposicaoGruposItens();
if($_REQUEST['reqgrupoform'] == 'salvar'){
    ob_clean();
    $resp = $controllerItemComposicaoGrupos->salvar($_REQUEST);
    echo simec_json_encode($resp);
    die;
}
$igrid = $igr->igrid ? $igr->igrid : " ";

switch($_REQUEST['reqvincularitem']){
    case 'formvincularitens':
        ob_clean();
//        $git = $controllerItemComposicaoGrupos->recuperar();
        require 'formvincularitens.inc';die;
        break;
    case 'vinculargrupoitens':
        ob_clean();
        $resp = $controllerGrupoItem->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'recuperar':
        ob_clean();
        $resp = $controllerGrupoItem->recuperar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerGrupoItem->listarItens($_REQUEST);die;
        break;
    case 'mediatotal':
        ob_clean();
        $resp = $controllerGrupoItem->getMediaTotal($_REQUEST['igrid']);
        echo simec_json_encode($resp);
        die;
        break;
    case 'desvincular':
        ob_clean();
        $resp = $controllerGrupoItem->desvincular($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
}
?>
    <div class="ibox">
        <div class="ibox-title">
            <a id="listagem-grupoitens"><i class="fa fa-arrow-left"></i> voltar para a pesquisa</a>
            <h3 class="center"><?php echo $igr->igrid?'Editar - ':'Incluir - '; ?>Grupo Itens de Composição</h3>
        </div>
        <form method="post" name="formGrupoItem" id="formGrupoItem" class="form form-horizontal">

        <div class="ibox-content" id="conteudo-form">
            <div class="form-group">
                <label class="col-lg-3 control-label">Código:</label>
                <div class="col-lg-2">
                    <input type="text" disabled="" value="<?php echo $igr->igrid; ?>" class="form-control">
                </div>
            </div>
            <input type="hidden" name="igrid" value="<?php echo $igr->igrid; ?>">
            <?php
            echo $simec->input('igrnome', 'Nome', $igr->igrnome, array('maxlength' => '255', 'required', 'title' => 'Informe um nome para um grupo de Item'));
            echo $simec->textarea('igrdsc', 'Descrição', $igr->igrdsc, array('required', 'title' => 'Informe uma descrição para o Grupo do Item de Composição.'));
            echo $simec->select('intoid[]', 'Tipo de Objeto', $grupoTipoObj->recuperarPorGrupo($igr->igrid), $rsInto, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
            echo $simec->radio('igrsituacao', 'Situação', ($igr->igrsituacao ? $igr->igrsituacao : "A"), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
            echo $simec->input('igridsigarp', 'Id Sigarp', $igr->igridsigarp, array('type' => 'number'),array('input-size' => '2'));
            ?>
        </div>
        <div class="ibox-footer">
            <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                <button type="submit" id="cancelar-grupoitem" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar </button>
                <button type="submit" id="salvarGrupoItem" class="btn btn-success"><i class="fa fa-plus-square-o"></i> Salvar </button>
                <?php if($igr->igrid): ?>
                <button type="button" class="btn btn-primary" id="novo-grupoitem"><i class="fa fa-plus"></i> Incluir Novo Grupo de Item </button>
                <?php endif; ?>
            </div>
        </div>
        </form>
    </div>
    <?php if($igr->igrid): ?>
    <div class="ibox">
        <div class="ibox-title">
            <h3 class="center">Itens Vinculados</h3>
            <button class="btn btn-success" id="vincular-item" data-toggle="modal" data-target="#modal_vincularitem"><i class="fa fa-plus"></i>&nbsp;Vincular Itens</button>
        </div>
            <div class="ibox-content" >
            <?php $controllerGrupoItem->listar($_REQUEST); ?>
            </div>
            <div class="ibox-footer"></div>
        </form>
    </div>

    <!--modal-->
    <div class="ibox float-e-margins animated modal" id="modal_vincularitem" tabindex="-1" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <form method="post" name="formGrupoItem" id="formGrupoItem" class="form form-horizontal">
                <div class="modal-content">
                    <div class="ibox-title">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                        <h3 id="modal_titulo" align="center">Vincular Itens de Composição</h3>
                    </div>
                    <div class="ibox-content" id="conteudo-vincularitem" ></div>
                    <div class="ibox-footer">
                        <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                            <button type="submit" id="cancelarvinculo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i>Cancelar</button>
                            <button type="submit" id="vinculargrupoitem" class="btn btn-success"><i class="fa fa-plus-square-o"></i> Salvar</button>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
    <?php endif; ?>
<script>
    <?php
    /**
     */
    if($igr->igrid): ?>
    $(document).ready(function(){
            var caminho = window.location.href;
            var action = '&reqvincularitem=mediatotal';
            $.ajax({
                type: "GET",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var valor = $.parseJSON(resp);
                    $('#grupositenstable').find('tfoot').prepend('<tr><td style="text-align:right" colspan="7"><strong>Subtotal: '+valor.total+'</strong></td></tr>');
                }
            });
        });
    <?php endif; ?>

    $(document).on('click', '#listagem-grupoitens', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens');
    });

    $(document).on('click', '#cancelar-grupoitem', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens');
    });

    $(document).on('click', '#novo-grupoitem', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens&submenu=form');
    });

    jQuery('#salvarGrupoItem').click(function (evt) {
        evt.preventDefault();
        var igrid = $('#conteudo-form').find('[name=igrid]').val();
        var msg = 'Incluir';
        if (igrid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Grupo de Itens?",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarGrupo();
                }
                return false;
            }
        );
    });

    function salvarGrupo() {
        var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens&submenu=form';
        var action = '&reqgrupoform=salvar&' + $("#formGrupoItem").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho+'&igrid='+parseInt(res),"Grupo de Item Salvo com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#conteudo-form").find("#" + err).parent("div");
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

    $('#vincular-item').click(function(){
        carregarFormVincularItem(<?php echo $igr->igrid; ?>);
    });

    function carregarFormVincularItem(id = null) {
        var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens&submenu=form';
        var requisicao = '&reqvincularitem=formvincularitens';
        var gitid = '';
        if (id != null) {
            var igrid = '&igrid=' + id;
        }
        $('#conteudo-vincularitem').load(caminho + requisicao + igrid);
    }

    function removerGrupoItem(id)
    {
        var caminho = window.location.href;
        var action = 'reqvincularitem=recuperar&gitid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var git = $.parseJSON(resp);
                console.log(git);
                swal({
                    title: "Remover Item Vinculado",
                    text: "Tem certeza que deseja Desvincular o Item<br/><center><b>" + git.itcdsc +" ?</b></center>",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&reqvincularitem=desvincular&gitid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                if (resp != 'erro') {
                                    msgSuccess(caminho,"Item desvinculado com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao desvincular Item.", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    jQuery('#vinculargrupoitem').click(function (evt) {
        evt.preventDefault();
        swal({
                title: "Confirmar",
                text: "Deseja <b>Vincular</b> estes Items?",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    vincularGrupoItens();
                }
                return false;
            }
        );
    });

    function vincularGrupoItens(id = null)
    {
        var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens&submenu=form';
        var action = '&reqvincularitem=vinculargrupoitens'+'&igrid='+'<?php echo $igrid ; ?>&'+$('#listagemitens form').serialize();

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho+'&igrid='+'<?php echo $igrid; ?>',"Itens Vinculados com Sucesso!");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#conteudo-vincularitem").find("#" + err);
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                    });
                });
                return false;
            }
        });
        return false;
    }
</script>