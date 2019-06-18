<?php
$sitcodigo = '';
$numeroata = '';
$estados = array();
if ($itd->itdid) {

    if($itd->sprid){
        //Recupera Código Pregão SIGARP
        $mPregao = $db->recuperar("SELECT * FROM par.sigarppregao WHERE sprid = $itd->sprid");
        $sitcodigo = $mPregao['sprnupregao'];
    }

    if($itd->atpid){
        //Recupera Número da Ata
        $mAta = new Par3_Model_ItensComposicaoAtaPregao($itd->atpid);
        $numeroata = $mAta->atpnumeroata;
    }

    //Recupera lista de Estados
    $ide = new Par3_Model_ItensComposicaoDetalhamentoEstado();
    $estados = $ide->recuperarPorItemDetalhamento($itd->itdid);
}
$mUf = new Par3_Model_Estado();
$rsUf = simec_preparar_array($mUf->recuperarTodos('estuf as codigo,estdescricao as descricao ', null, 'estuf ASC'));
global $db;

if($_REQUEST['requisicao'] == 'carregarDadosPregao'){
    $sprid = $_POST['dados'][0];
    ob_clean();
    $itd = new Par3_Controller_ItensComposicaoDetalhamento();
    $itd->listarDadosPregao($sprid);
    die;
}

 $valorMascara = ($itd->itdvalor == null ) ? "onkeyup" : "onclick";
?>
<div class="ibox" id="painelItens">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Incluir - Detalhamento do Item</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <form name="form-itemDetalhamento" id="form-itemDetalhamento" class="form-horizontal">
            <?php if($itd->itdid): ?>
            <div class="form-group">
                <label class="col-lg-3 control-label">Código:</label>
                <div class="col-lg-1">
                    <input type="text" disabled="" value="<?php echo $itd->itdid; ?>" class="form-control">
                </div>
            </div>
            <?php endif; ?>
            <input type="hidden" name="itcid" value="<?php echo $_GET['itcid']; ?>"/>
            <input type="hidden" name="itdid" value="<?php echo $itd->itdid; ?>"/>
            <?php echo $simec->radio('itdpregao', 'Pregão', ($itd->itdid ? $itd->itdpregao : "N"), array('S' => 'Sim', 'N' => 'Não'), array('', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline')); ?>
            <div id="div-pregao" style="display: none;">
                <?php echo $simec->radio('itdpregaopublicado', 'Pregão Publicado', ($itd->itdid ? $itd->itdpregaopublicado : "N"), array('S' => 'Sim', 'N' => 'Não'), array('', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline')); ?>
            </div>
            <div id="div-pregaopublicado" style="display: none;">
                <div class="form-group">
                    <label class="col-lg-3 control-label">Nº Pregão:*</label>
                    <div class="col-lg-2">
                        <input type="text" disabled value="<?php echo $sitcodigo; ?>" id="numeropregao"
                               class="form-control">
                        <input type="hidden" value="<?php echo $itd->sprid; ?>" name="sprid" id="sprid"/>
                    </div>
                    <button class="btn btn-success" id="btn-buscarPregao">Buscar Pregão</button>
                </div>

                <div class="form-group">
                    <label class="col-lg-3 control-label">Ata:</label>
                    <div class="col-lg-2">
                        <input type="text" disabled="" value="<?php echo $numeroata; ?>" id="numeroata"
                               class="form-control">
                        <input type="hidden" value="<?php echo $itd->atpid; ?>" name="atpid" id="atpid"/>
                    </div>
                    <button class="btn btn-success" id="btn-buscarAta">Buscar Ata</button>
                </div>
            </div>
            <?php
            echo $simec->input('itdvalor', 'Valor de Referência', $itd->itdvalor, array('maxlength' => '255', 'required', 'type' => 'text', 'title' => 'Informe uma descrição', $valorMascara => 'this.value=mascaraglobal("###.###.###,##",this.value);'));
//             echo $simec->periodo('itdperiodo', 'Período', array($itd->itdperiodoinicio, $itd->itdperiodofim), array('required', $disablePeriodo, 'title' => 'Informe o Período de Vigência do Item'), $attribs = array(), $config = array());
            ?>
            <div class="form-group ">
                <label for="teste" class="col-sm-3 col-md-3 col-lg-3 control-label">Período: *</label>
                <div class="col-sm-3">
                    <?php echo $simec->data('itdperiodoinicio', '', $itd->itdperiodoinicio, array(), array('input-size'=>12)); ?>
                </div>
                <div class="col-sm-1">
                    <span class="input-group-addon" style="border-width: 0px !important;padding: 10px  !important; background-color: #fff;">a</span>
                </div>
                <div class="col-sm-3">
                    <?php echo $simec->data('itdperiodofim', '', $itd->itdperiodofim, array(), array('input-size'=>12)); ?>
                </div>
                <div style="clear:both"></div>
            </div>
            <div class="row">
                <div class=" form-group">
                    <div class="col-sm-3 col-md-3 col-lg-3" style="margin-left: 20px;" id="itdperiodo" name="itdperiodo"></div>
                </div>
            </div>
            <?php
//            echo '<div class="form-group"><label class="col-lg-3 control-label">UF:*</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
//            echo inputCombo("estuf[]", $rsUf, $estados, 'estuf', array('multiple' => 'multiple', 'titulo' => 'Selecione'));
//            echo '</div></div>';
            echo $simec->select('estuf[]', 'UF', $estados, $rsUf, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
            ;?>
            <div class="row">
                <div class=" form-group">
                    <div class="col-sm-3 col-md-3 col-lg-3" style="margin-left: 20px;" id="estuf" name="estuf"></div>
                </div>
            </div>
            <?php
            echo $simec->radio('itdsituacao', 'Situação', ($itd->itdsituacao ? $itd->itdsituacao : "A"), array('A' => 'Ativo', 'I' => 'Inativo'), array('', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
            ?>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" class="btn btn-default" id="novo-detalhamentoitem"><i class="fa fa-times"></i>Cancelar</button>
                    <button type="button" class="btn btn-success" id="salvar-detalhamentoitem"><i class="fa fa-check"></i>Salvar</button>
                    <?php //if($itd->itdid):?>
                    <!--<button type="button" class="btn btn-primary" id="novo-detalhamentoitem"><i class="fa fa-plus"></i>Novo Detalhamento</button>-->
                    <?php //endif; ?>
                </div>
            </div>
        </form>
    </div>
</div>

<script>
    $('#itdvalor').click();
    $('#estuf').change(function(){
            $('#estuf').trigger($.Event("click", { ctrlKey: true }));
    });
    $('#estuf').prepend(new Option('Todos', 'TODOS', 'selected'));
    <?php if($itd->itdperiodoinicio && $itd->itdperiodofim): ?>
    $('[name=itdperiodoinicio]').val('<?php echo formata_data($itd->itdperiodoinicio)?>');
    $('[name=itdperiodofim]').val('<?php echo formata_data($itd->itdperiodofim)?>');
    <?php endif; ?>

    <?php if($itd->itdpregao == 'S'): ?>
    $('#div-pregao').show();
    <?php endif; ?>

    <?php if($itd->itdpregaopublicado == 'S'): ?>
    $('#div-pregaopublicado').show();
    <?php endif; ?>

    $('#itdpregao-S').click(function () {
        $('#div-pregao').show();
    });

    $('#itdpregao-N').click(function () {
        $('#div-pregao').hide();
        $('#div-pregaopublicado').hide();
    });

    $('#itdpregaopublicado-S').click(function () {
        $('#div-pregaopublicado').show();
    });

    $('#itdpregaopublicado-N').click(function () {
        $('#div-pregaopublicado').hide();
        $('#numeropregao').val("");
        $('[name=sprid]').val("");
        $('#numeroata').val("");
        $('[name=atpid]').val("");
    });

    $('#btn-buscarPregao').click(function (evt) {
        evt.preventDefault();
        carregarPregao();
    });

    $('#btn-buscarAta').click(function (evt) {
        evt.preventDefault();
        carregarAta();
    });

    function carregarPregao() {
        var caminho = window.location.href;
        var action = '&reqdetalhamento=listarpregao';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#modal_titulo').html('<center>Buscar Pregão</center>');
                $('#conteudo-modal').html(resp);
                $('#modal_detalhamento').modal('show');
            }
        });
        return false;
    }

    function carregarAta() {
        var caminho = window.location.href;
        var action = '&reqdetalhamento=listarata';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#modal_titulo').html('<center>Buscar Ata</center>');
                $('#conteudo-modal').html(resp);
                $('#modal_detalhamento').modal('show');
            }
        });
        return false;
    }

    function adicionarPregao(id) {
        $('#modal_detalhamento').modal('hide');
        var caminho = window.location.href;
        var action = '&reqdetalhamento=getpregaocodigo&sprid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#numeropregao').val(resp);
            }
        });
        $('[name=sprid]').val(id);
    }

    function adicionarAta(id) {
        $('#modal_detalhamento').modal('hide');
        var caminho = window.location.href;
        var action = '&reqdetalhamento=getatacodigo&atpid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#numeroata').val(resp);
            }
        });
        $('[name=atpid]').val(id);
    }

    $('#novo-detalhamentoitem').click(function () {
        var caminho = window.location.href;
        var action = '&reqdetalhamento=recuperar';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#div-itemdetalhamento').html(resp);
            }
        });
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

    function editarItemDetalhamento(id) {
        var caminho = window.location.href;
        var action = '&reqdetalhamento=recuperar&itdid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#div-itemdetalhamento').html(resp);
            }
        });
    }


</script>
