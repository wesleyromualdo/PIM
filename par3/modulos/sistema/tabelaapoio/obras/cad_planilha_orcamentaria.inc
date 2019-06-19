<?php

$_REQUEST['ovpid'] = explode('_', $_REQUEST['ovpid']);
$_REQUEST['ovpid'] = $_REQUEST['ovpid'][0];

$_REQUEST['tplid'] = explode('_', $_REQUEST['tplid']);
$_REQUEST['tplid'] = $_REQUEST['tplid'][0];

if(!is_array($_REQUEST['estuf'])) $_REQUEST['estuf'] = explode(',', $_REQUEST['estuf']);
$_REQUEST['estuf'] = $_REQUEST['estuf'][1] != '' ? $_REQUEST['estuf'] : $_REQUEST['estuf'][0];
$_REQUEST['estuf'] = $_REQUEST['estuf'] ? $_REQUEST['estuf'] : 'ND';

$ctrObraTipo = new Par3_Controller_ObraTipo($_REQUEST['otpid']);

$ctrObraTipoVersaoProjeto   = new Par3_Controller_ObraTipoVersaoProjeto();
$arrVerProj                 = $ctrObraTipoVersaoProjeto->pegarSelectCombo($_REQUEST['otpid']);

$ctrObraVersaoPlanilha      = new Par3_Controller_ObraTipoVersaoPlanilha();
$arrVerPlan                 = $ctrObraVersaoPlanilha->pegarSelectCombo($_REQUEST['ovpid']);

$mdEstado   = new Territorios_Model_Estado();
$sqlEstado  = $mdEstado->pegarSQLSelect($_REQUEST);

$mdMunicipio = new Territorios_Model_Municipio();
$sqlMunicipio = $mdMunicipio->pegarSQLSelect($_REQUEST['estuf']);

$arrTipoRegiao = array(
    'N' => 'Nacional',
    'E' => 'Estadual',
    'M' => 'Municipal'
);

function retornaRelatorio($arrPost)
{
    $ctrObraTipoVersaoProjeto = new Par3_ctr_ObraTipoVersaoProjeto();
    $ctrObraTipoVersaoProjeto->lista($arrPost);
}

switch ($_REQUEST['requisicao']) {
    /* Funções de Projeto*/
    case 'recuperar':
        ob_clean();
        echo $ctrObraTipoVersaoProjeto->recuperar($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $ctrObraTipoVersaoProjeto->remover($_REQUEST);
        die;
        break;
    case 'recuperarVersaoProjeto':
        ob_clean();
        $mVersaoProjeto = $ctrObraTipoVersaoProjeto->recuperar($_REQUEST['otpid']);
        require_once 'versao_projeto.inc';
        die;
        break;
    case 'recuperarmetragem':
        ob_clean();
        $mVersaoProjeto = $ctrObraTipoVersaoProjeto->recuperar($_REQUEST['ovpid']);
        echo $mVersaoProjeto->ovpmetragem;die;
        break;
    case 'download':
        ob_clean();
        $arqid = $_GET['arqid'];
        if ( $arqid ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file 		  = new FilesSimec(null, null, "par3");
            $file->getDownloadArquivo($arqid);
        }
        die();
        break;
    case 'listarArquivos':
        ob_clean();
        $arrArquivos = $ctrObraVersaoPlanilha->listarArquivos($_POST['tplid']);
        echo json_encode($arrArquivos);die;
        break;
    case 'salvarVersaoProjeto':
        ob_clean();
        $resp = $ctrObraTipoVersaoProjeto->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagemVersaoProjeto':
        $ovpid = isset($_REQUEST['ovpid'])?$_REQUEST['ovpid']:'';
        ob_clean();
        echo $simec->select('ovpid', '', $ovpid, $arrVerProj,'',array('input-size'=>12)) ;
        die;
        break;
    /* Funções de Projeto*/
    case 'recuperarPlanilha':
        ob_clean();
        echo $ctrObraVersaoPlanilha->recuperar($_REQUEST);
        die;
        break;
    case 'removerPlanilha':
        ob_clean();
        echo $ctrObraVersaoPlanilha->remover($_REQUEST);
        die;
        break;
    case 'detalhar':
        ob_clean();
        $_REQUEST['ovpid'] = explode('_', $_REQUEST['dados'][0]);
        $_REQUEST['ovpid'] = $_REQUEST['ovpid'][0];
        echo $ctrObraVersaoPlanilha->lista($_REQUEST);
        die;
        break;
    /* Funções de Itens*/
    case 'listarItens':
        ob_clean();
        $_REQUEST['tplid'] = explode('_', $_REQUEST['dados'][0]);
        $_REQUEST['tplid'] = $_REQUEST['tplid'][0];
        $ctrObraItemPlanilha = new Par3_Controller_ObraTipoItemPlanilha();
        echo $ctrObraItemPlanilha->lista($_REQUEST);
        die;
        break;
    case 'listarValorItens':
        ob_clean();
        $_REQUEST['oipid'] = explode('_', $_REQUEST['dados'][0]);
        $_REQUEST['oipid'] = $_REQUEST['oipid'][0];
        $ctrObraItemValores = new Par3_Controller_ObraTipoItemPlanilhaValores();
        echo $ctrObraItemValores->lista($_REQUEST);
        die;
        break;
    case 'atualizaMunicipios':
        ob_clean();
        echo $simec->select("muncod[{$_REQUEST['linha']}][]", '', '', $sqlMunicipio,array(),array('input-size'=>12));
        die;
        break;
    case 'adicionarLinhaArquivo':
        ob_clean();
        require_once 'linhaArquivoPlanilha.inc';
        die;
        break;
    case 'validaPlanilha':
        ob_clean();
        $_REQUEST['ovpid'] = $_REQUEST['ovpid_salvar'];
        $resp = $ctrObraVersaoPlanilha->validarInput($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'salvarPlanilha':
        ob_clean();
        $ctrObraVersaoPlanilha->salvar($_REQUEST);
        simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cad_planilha_orcamentaria&otpid='.$_REQUEST['otpid'], 'success');
        die;
        break;
    default:
        break;
}
global $simec;
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-2">
                <a href="par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=planilha_orcamentaria"><i class="fa fa-arrow-left" aria-hidden="true"></i>voltar para a pesquisa</a>
            </div>
            <div class="col-md-8">
                <h3 class="center">Planilha Orçamentária</h3>
            </div>
            <div class="col-md-2">
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-offset-2 col-md-8 center">
                <div class="form-group ">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Tipo de Obra: </label>
                    <div class="col-sm-5 col-md-5 col-lg-5 ">
                        <h4>
                            <?php echo $ctrObraTipo->getDescricao()?>
                        </h4>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-12 center">

                <div class="form-group ">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Versão de Projeto:* </label>
                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                        <form class="input-group" style="width: 100%" id="selectVersaoProjeto" >
                            <input type="hidden" name="otpid" value="<?php echo $_REQUEST['otpid']?>"/>
                            <div id="selectVerProjeto" class="col-sm-11 ">
                                <?php echo $simec->select('ovpid', '', '', $arrVerProj,array('required'),array('input-size'=>12)) ; ?>
                            </div>
                            <div class="col-sm-1">
                                <span class="input-group-btn" >
                                    <button type="button" class="btn btn-success" id="btnVersaoProjeto" style="margin-left:-30px;" title="">
                                             <i id="iconeAcaoVersaoProjeto" class="fa fa-plus"></i>
                                    </button>
                                </span>
                            </div>
                        </form>
                    </div>
                </div>

                <div class="form-group " id="input_metragem">
                    <br><br><br>
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Metragem da Obra: </label>
                    <div class="col-sm-8">
                        <div class="input-group col-sm-7" style="margin-left: 30px;">
                            <input id="ovpmetragem_selecionada" class="form-control" disabled="disabled" placeholder="informe a metragem da obra em m²" value="" required="required">
                            <span class="input-group-addon"><b>M²</b></span>
                        </div>
                    </div>
                </div>

                <form style="display: none;" id="formVersaoProjeto"></form>
                <form class="input-group" style="width: 100%" id="formPlanilha"  method="post" enctype="multipart/form-data">
                    <input type="hidden" name="ovpid_salvar"/>
                    <input type="hidden" name="requisicao" id="requisicao"/>
<br><br>
                    <div class="form-group ">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Versão de Planilha:* </label>
                        <div class="col-sm-8">
                            <div class="form-group ">
                                <div class="col-sm-12" style="margin-left: 15px;" >
                                    <input type="text" class="form-control" placeholder="Adicione nova Versao de Planilha" id="tpldsc" name="tpldsc" value="" required="required">
                                </div>
                            </div>
                        </div>
                    </div>

<br><br><br>
                    <div class="form-group ">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Tipo de Região:* </label>
                        <div class="col-sm-9 col-md-9 col-lg-9 ">
                            <div id="selectVerProjeto" class="col-sm-11 ">
                                <?php echo $simec->select('tpltipo_regiao', '', '', $arrTipoRegiao,array('required'),array('input-size'=>12)) ; ?>
                            </div>
                        </div>
                    </div>
<br><br><br>
                    <div class="form-group">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Formato do Arquivo: </label>
                        <div class="col-sm-9 col-md-9 col-lg-9" style="text-align:left;">
                            <div style="margin-left:30px" >
                                <h4>Deve ser uma planilha .csv (separado por ';') e possuir 7 colunas com as respectivas informações:</h4>
                                <h5>
                                1 - Código ordinal do item separado por pontos (Ex.: 1.1.21)<br>
                                2 - Descrição do item (NÃO PODE CONTER PONTO E VIRGULA ';' )<br>
                                3 - O código da unidade de medida (Ex.: Se for Unidade então deve estar assim: 2)<br>
                                4 - Quantidade em formato brasileiro (Ex.: 1.112,23)<br>
                                5 - Se a quantidade pode ser editada (S para editável e N para não editável)<br>
                                6 - Valor unitário do item contendo somente números no formato brasileiro (Ex.: 1.123,23)<br>
                                7 - Se o valor unitário pode ser editado (S para editável e N para não editável)<br>
                                </h5>
                                <h5>Observação: Ao incluir arquivos para a esfera estadual, é necessário informar a(s) UF(s) da planilha anexada.</h5>
                                <h5>Observação*: Para salvar o arquivo .csv separado por ';', basta salva-lo utilizando o OpenOffice (Ou LibreOffice) Calc, clicando em 'salvar como', escolhendo a extensão .csv, marcar a opção 'Editar as configurações do filtro', clicar em 'Salvar', escolher o ';' no campo 'Delimitador de campo:' e apertar 'OK'.</h5>
                            </div>
                        </div>
                    </div>
<br><br><br>
                    <div class="form-group row">
                        <label for="" class="col-sm-3 control-label" style="text-align: right">
                            Arquivo:*
                            <span class="input-group-btn" style="display:none" id="addArquivo">
                                <button type="button" class="btn btn-success" id="btnAddArquivo" style="margin-left:0px;" title="Novo Arquivo">
                                         <i class="fa fa-plus"></i>
                                </button>
                            </span>
                        </label>
                        <div class="col-sm-9">
                            <div class="row" id="linha_1">
                                <div class="ibox-content">
                                    <div class="row">
                                        <div class="col-lg-12">
                                            <div class="form-group">
                                                <div class="col-lg-12">
                                                    <input type=file name="arqid_1" id="arqid_1" style="margin-left:60px" required="required" class="input-arqid"/>
                                                </div>
                                            </div>
                                        </div>
                                    </div><br>
                                    <div class="row">
                                        <div id="selectEstuf_1" linha="1" class="col-sm-4" style="display:none;margin-left:40px">
                                            <?php echo $simec->select('estuf[1][]', '', '', $sqlEstado,array(),array('input-size'=>12)) ; ?>
                                        </div>
                                        <div id="selectMuncod_1" linha="1" class="col-sm-5"" style="display:none">
                                            <?php echo $simec->select('muncod[1][]', '', '', $sqlMunicipio,array(),array('input-size'=>12)) ; ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </form>
<br>
            </div>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-success" id="btn-enviar" >
                    <i class="fa fa-save"></i> Enviar
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="desdobramentoListagem">
            <?php
            $ctrObraTipoVersaoProjeto->lista($_REQUEST);
            ?>
        </div>
    </div>
</div>


<div class="ibox float-e-margins animated modal" id="modal_download" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo" align="center">Download Planilhas</h3>
            </div>
            <div class="ibox-content" id="conteudo-modal"></div>
            <div class="ibox-footer">
                <div class="row center">
                    <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default">
                        <i class="fa fa-times-circle-o"></i> Cancelar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>
<script>

    $('.input-arqid').change(function () {
        var linha = $(this).attr('linha');
        $('#upload-file-info-'+linha).html($(this).val());
    });

    $(document).on('click','#excluir',function () {
        var linha = $(this).attr('linha');
        $('#linha_'+linha).remove();
    });

    $(document).on('change','#tpltipo_regiao',function () {
        var tipo_regiao = $(this).val();
        if(tipo_regiao == 'N'){
            $('#addArquivo').hide();
            $('[id*="selectEstuf_"]').hide();
            $('[id*="selectMuncod_"]').hide();
            $('[id*="linha_"][id!="linha_1"]').remove();
            $('[id*="selectEstuf_"]').removeAttr('required');
            $('[id*="selectMuncod_"]').removeAttr('required');
        }
        if(tipo_regiao == 'E'){
            $('#addArquivo').show();
            $('[id*="selectEstuf_"]').show();
            $('[id*="selectMuncod_"]').hide();
            $('[id*="selectEstuf_"]').attr('required', 'required');
            $('[id*="selectMuncod_"]').removeAttr('required');
        }
        if(tipo_regiao == 'M'){
            $('#addArquivo').show();
            $('[id*="selectEstuf_"]').show();
            $('[id*="selectMuncod_"]').show();
            $('[id*="selectEstuf_"]').attr('required', 'required');
            $('[id*="selectMuncod_"]').attr('required', 'required');
        }
    }).change();

    $(document).on('change','[name*="estuf"]',function () {
        var linha   = $(this).parent().parent().parent().attr('linha');
        var estuf   = $(this).val();
    	var caminho = window.location.href;
        var action  = '&requisicao=atualizaMunicipios&estuf=' + estuf + '&linha=' + linha;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#selectMuncod_'+linha).html(resp);
            }
        });
    }).change();

    $(document).on('click','#btnAddArquivo',function () {
        var tipo_regiao = $('#tpltipo_regiao').val();
        var ultima      = $('.input-arqid').size();
        var linha       = parseFloat(ultima)+1;
    	var caminho     = window.location.href;
        var action      = '&requisicao=adicionarLinhaArquivo&linha=' + linha + '&tipo_regiao=' + tipo_regiao;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#linha_'+ultima).after(resp);
            }
        });
    }).change();


    $(document).on('change','#ovpid',function () {
    	resp = $(this).val();
        if (!isNaN(parseInt(resp)) && isFinite(resp)) {
            $('#iconeAcaoVersaoProjeto').attr('class', 'fa fa-pencil');
            $('#iconeAcaoVersaoProjeto').attr('title', 'Alterar Tipo de Desdobramento');
            $('#iconeAcaoVersaoPlanilha').show();
        }
        $('#iconeAcaoVersaoProjeto').attr('class', 'fa fa-plus');
        $('#iconeAcaoVersaoProjeto').attr('title', 'Novo Tipo de Desdobramento');
        $('#iconeAcaoVersaoPlanilha').hide();
        $('[name="ovpid_salvar"]').val($('#ovpid').val());
    }).change();

    $('#btnVersaoProjeto').click(function (evt) {
        evt.preventDefault();
        $('#formVersaoProjeto').fadeIn();
        $('#btn-enviar').attr('disabled', 'disabled');
        var selectTipo = $('#selectVersaoProjeto').serialize();
        console.log(selectTipo);
        var caminho = window.location.href;
        var action = '&requisicao=recuperarVersaoProjeto&' + selectTipo;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#formVersaoProjeto').html(resp);
                $('#input_metragem').hide();
            }
        });
    });

    $(document).on('change ready','#ovpid',function(){
        $('#ovpmetragem_selecionada').show();
        atualizarMetragemProjeto();
    });

    function atualizarMetragemProjeto()
    {
        var caminho = window.location.href;
        var action = '&requisicao=recuperarmetragem&ovpid=' + $(document).find('#ovpid').val();
        $.ajax({
            type: "get",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $(document).find('#loading').hide();
                console.log(resp);
                $('#ovpmetragem_selecionada').val(resp);
            }
        });
    }

    $('#btn-enviar').click(function (evt) {
        console.log($('#ovpmetragem').val());
    	$('div[id*="div-erro"]').remove();
    	$('#requisicao').val('validaPlanilha');
        var caminho = window.location.href;
        var data    = '&' + $("#formPlanilha").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: data,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                var erroArquivo = false;
                $('[name*="arqid"]').each(function(){
                    if(!($(this).val() != '')){
                    	var divFormInput = $(this).parent('div');
                        divFormInput.closest('.form-group').addClass('has-error');
                        divFormInput.append("<div id='div-erro-" + $(this).attr('name') + "'><span class='danger'> O campo não pode ser vazio </span></div>");
                        erroArquivo = true;
                    }
                });
                if (resp == 'false' && !erroArquivo) {
                	$('#requisicao').val('salvarPlanilha');
                	$("#formPlanilha").submit()
                    return false;
                }else if(resp != 'false'){
                    swal.close();
                    $.each(res, function (index, erro) {//retorna mensagens de erro em json
                        $.each(erro, function (err, errMsg) {
                            if (errMsg < err.length) {
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput = $("#" + err).parent('div');
                            divFormInput.closest('.form-group').addClass('has-error');
                            divFormInput.append("<div id='div-erro-" + err + "'></div>");
                            $("#div-erro-" + err).html("<span class='danger'>" + errMsg + "</span>");
                        });
                    });
                }
                return false;
            }
        });
    });














    jQuery('#salvarDesdobramento').click(function (evt) {
        evt.preventDefault();
        var ovpid = $('#conteudo-modal').find('[name=ovpid]').val();
        var msg = 'Incluir';
        if (ovpid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Desdobramento?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarDesdobramento();
                }
                return false;
            }
        );
    });

    function salvarDesdobramento() {
        var caminho = window.location.href;
        var action = '&requisicao=salvarDesdobramento&' + $("#formDesdobramento").serialize()+'&'+$('#selectDesdobramentoTipo').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    $("#modal_desdobramento").modal("hide");
                    $("#modal_desdobramento").find("input").val("");
                    msgSuccess(caminho,"Desdobramento salvo com sucesso");
                    //swal("Sucesso.", "Desdobramento salvo com sucesso.", "success");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        if(err == 'tipid'){
                            var tipidlabel = $("#formDesdobramento").find("#tipidlabel");
                            tipidlabel.addClass('has-error');
                            tipidlabel.append("<div id='div-" + err + "'></div>");
                            $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                            tipidlabel.show();
                        }
                        var divFormInput = $("#modal_desdobramento").find("input[name=" + err + "]").parent("div");
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

    $("#pesquisar-desdobramento").click(function (evt) {
        evt.preventDefault();
        var caminho = window.location.href;
        var action = '&requisicao=pesquisar&' + $("#form-filtro").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#desdobramentoListagem").html(resp);
                return true;
            }
        });
    });

    function removerPlanilha(id) {
        var caminho = window.location.href;
        var action = '&requisicao=recuperarPlanilha&tplid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var res = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Planilha",
                    text: "Tem certeza que deseja remover: <b>" + res.tplid + " - " + res.tpldsc + "</b> ?<br> Ao confirmar a operação, a versão de planilha e seu conteudo não poderá ser utilizado em novas obras.",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&requisicao=removerPlanilha&tplid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_desdobramento").modal("hide");
                                $("#modal_desdobramento").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho,"Versão de Planilha <b>\"" + res.tplid + " - " + res.tpldsc + "\"</b> removida com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover a Versão de Planilha. <b>" + res.tplid + " - " + res.tpldsc + "</b>", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    function remover(id) {
        var caminho = window.location.href;
        var action = '&requisicao=recuperar&ovpid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var res = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    title: "Remover Projeto",
                    text: "Tem certeza que deseja remover: <b>" + res.ovpid + " - " + res.ovpdsc + "</b> ?<br> Ao confirmar a operação, a versão de projeto e seu conteudo não poderá ser utilizado em novas obras.",
                    html: true,
                    type: "warning",
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = window.location.href;
                        var action = '&requisicao=remover&ovpid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                                $("#modal_desdobramento").modal("hide");
                                $("#modal_desdobramento").find("input").val("");
                                if (resp != 'erro') {
                                    msgSuccess(caminho,"Versão de Projeto <b>\"" + res.ovpid + " - " + res.ovpdsc + "\"</b> removida com sucesso");
                                    return false;
                                }
                                swal("Erro.", "Ocorreu um erro ao remover a Versão de Projeto. <b>" + res.ovpid + " - " + res.ovpdsc + "</b>", "error");
                            }
                        });
                    }
                })
            }
        });
    }

    jQuery("#limpar").click(function(){

        $('#desdsc').val('');
        $('#ovpid').val('');

       $("#tipodesc").val('').trigger('chosen:updated');

        $("#form-filtro").find('#dessituacao1').removeAttr("checked");
        $("#form-filtro").find('#dessituacao2').removeAttr("checked");
        $("#form-filtro").find('#dessituacao3').prop('checked',true);

    });

    function listarArquivos(t,tplid,tpldsc)
    {
        $('#modal_download').find('#conteudo-modal').html('');
        $('#modal_download').find('#modal_titulo').html('');
        $('#modal_download').find('#modal_titulo').html('Planilhas da Versão: <b>'+tpldsc+'</b>');
        $('#modal_download').modal();
        var links = '';
        var caminho = window.location.href;
        var action = '&requisicao=listarArquivos&tplid=' + tplid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var arquivos = JSON.parse(resp);
                links += '<table class="table table-bordered table-striped">';
                links += '<tr class="info">';
                links += '<td><b>Arquivo</b></td>';
                links += '<td><b>Opção</b></td>';
                links += '</tr>';
                $.each(arquivos,function(index,value){
                    links +='<tr>';
                    links +="<td class=\"col-lg-9\"><b>"+value.arqnome+'.'+value.arqextensao+"</b></td>";
                    links +="<td class='col-lg-1'><button class='btn btn-download' onclick=\"downloadArquivo("+value.arqid+")\"><i class='fa fa-download'></i></button></td>";
                    links +="</tr>";
                });
                links +='</table>';
            }
        });
        $('#modal_download').find('#conteudo-modal').html(links);

    }

    function downloadArquivo(arqid)
    {
        window.location.href = window.location.href +'&requisicao=download&arqid='+arqid;
    }
</script>