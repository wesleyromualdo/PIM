<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
include_once APPRAIZ . 'par3/classes/controller/ItensComposicao.class.inc';
$item = new Par3_Model_ItensComposicao($_REQUEST['itcid']);

$mInto = new Par3_Model_IniciativaTiposObjeto();
$rsInto = simec_preparar_array($mInto->recuperarTodos('intoid as codigo,intodsc as descricao ', array("intosituacao = 'A' "), 'intodsc ASC'));

$mCtd = new Par3_Model_CategoriaDespesa();
$mUni = new Par3_Model_UnidadeMedida();
$mIto = new Par3_Model_ItensComposicaoTiposObjeto();
$sqlCtd = $mCtd->selectCombo();
$sqlUni = $mUni->selectCombo();

switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $controllerItem = new Par3_Controller_ItensComposicao();
        $resp = $controllerItem->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'recuperar':
        break;
    case 'remover':
        break;
    default;
}
?>
<div class="ibox" id="painelItens">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <a id="listagem-itens"><i class="fa fa-arrow-left"></i> voltar para a pesquisa</a>
                <h3 class="center">Item de Composição - <?php echo $item->itcid ? 'Alterar' : 'Incluir'; ?></h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <form name="form-item" id="form-item" class="form-horizontal">
            <div class="row">
                <div class="form-group">
                    <label class="col-lg-3 control-label">Código:</label>
                    <div class="col-lg-2">
                        <input type="text" disabled="" value="<?php echo $item->itcid; ?>" class="form-control">
                    </div>
                </div>
                <input type="hidden" name="itcid" value="<?php echo $item->itcid; ?>"/>
                <?php
                global $simec;
                echo $simec->input('itcdsc', 'Nome', $item->itcdsc, array('maxlength' => '255', 'required', '', 'title' => 'Informe uma descrição'));
                echo $simec->textarea('itcespecificacao', 'Especificação Técnica', $item->itcespecificacao, array('required', 'title' => 'Informe uma descrição para a Especificação téncnica.'));
                echo $simec->select('intoid[]', 'Tipo de Objeto', $mIto->recuperarPorItem($item->itcid), $rsInto, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
                echo $simec->select('ctdid', 'Categoria de Despesa', $item->ctdid, $sqlCtd, array('maxlength' => '255', 'required', 'title' => 'Selecione uma Categoria de Despesa'));//,array('class' => 'col-md-3')
                echo $simec->select('uniid', 'Unidade de Medida', $item->uniid, $sqlUni, array('maxlength' => '255', 'required', 'title' => 'Selecione uma Unidade de Medida'));//,array('class' => 'col-md-3')
                echo $simec->radio('itcsituacao', 'Situação', ($item->itcsituacao ? $item->itcsituacao : "A"), array('A' => 'Ativo', 'I' => 'Inativo'), array('', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                echo $simec->input('itcidsigarp', 'Id Sigarp', $item->itcidsigarp, array('type' => 'number'),array('input-size' => '2'));
                ?>
            </div>
        </form>
    </div>
    <div class="ibox-footer">
        <div class="center">
            <button type="button" class="btn btn-default" id="cancelar-item"><i class="fa fa-times"></i> Cancelar
            </button>
            <button type="button" class="btn btn-success" id="salvar-item"><i class="fa fa-check"></i> Salvar</button>
            <?php if($item->itcid): ?>
            <button type="button" class="btn btn-primary" id="novo-item"><i class="fa fa-plus"></i> Incluir Novo Item
            </button>
            <?php endif; ?>
        </div>
    </div>
</div>

<?php if($item->itcid): ?>
<?php require_once 'detalhamento.inc'; ?>
<?php endif; ?>
<script>
    $(document).find("#form-item").on('click', "input[name=itcsituacao ][value=I]", function () {
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
    });

    $(document).on('click', '#listagem-itens', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=itens');
    });

    $(document).on('click', '#cancelar-item', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=itens');
    });

    $(document).on('click', '#novo-item', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=itens&submenu=form');
    });

    $(document).on('click', '#salvar-item', function (evt) {
        evt.preventDefault();
        var inprgid = $('#form-item').find('[name=itcid]').val();
        var msg = 'Incluir';
        if (inprgid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Item de Composição?",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarItem();
                }
                return false;
            }
        );
    });
    function salvarItem() {
        var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=itens&submenu=form';
        var action = '&req=salvar&' + $("#form-item").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho+'&itcid='+parseInt(res), "Item salvo com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#form-item").find("#" + err).parent("div");
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
</script>
