<?php
/**
 * Sistema Monitoramento de Obras 2.0
 *
 * @author Hemerson Morais
 * Date: 28/06/2017
 *
 */

include_once APPRAIZ . 'par3/classes/controller/Iniciativa_itenscomposicao_grupo.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa_itenscomposicao_grupo.class.inc';

//instanciamento da classe Par3_Controller_Iniciativa
$objControllerIniciativa = new Par3_Controller_Iniciativa();
$mdIniciativa            = new Par3_Model_Iniciativa($_GET['iniid']);
$controllerIniciativa = new Par3_Controller_Iniciativa_itenscomposicao_grupo();

//instanciamento da classe Par3_Controller_ItensComposocao
$objModeltensComposicao = new Par3_Model_ItensComposicao();

//instanciamento da classe Par3_Controller_ItensComposocao
$objModeGruposltens = new Par3_Model_ItensComposicaoGrupos();

$controllerObraTipo = new Par3_Controller_ObraTipo();
$arrCategorias      = $controllerObraTipo->getComboCategoriaObras();
$mdEtapaEnsino      = new Par3_Model_EnsinoEtapa();
$arrEtapasEnsino    = $mdEtapaEnsino->pegarSQLSelectCombo(array(), "SELECT etaid FROM par3.iniciativa_modalidades WHERE iniid = {$_GET['iniid']} AND imostatus = 'A'");
$arrEtapasEnsino    = $mdEtapaEnsino->carregar($arrEtapasEnsino);
$arrEtapasEnsino    = is_array($arrEtapasEnsino)?simec_preparar_array($arrEtapasEnsino):array();
$etapaEnsino        = count($arrEtapasEnsino) == 1 ? $arrEtapasEnsino[0]['codigo'] : null;
$controllerObra     = new Par3_Controller_Obra();

//Controle de Ação
switch ($_REQUEST['req']) {
	case 'atualizaTipoObra';
        ob_clean();
        if($_REQUEST['octid'] && $_REQUEST['etaid']){
            $_REQUEST['otpsituacao']    = 'A';
            $_REQUEST['NOTIN']['otp.otpid'] = "(select otpid from par3.iniciativa_tipo_obra where iniid = {$_GET['iniid']})";
            echo $simec->select('otpid', 'Tipo de Obra', $controllerObra->model->otpid, $controllerObra->getComboTipoObra(), array('required' => 'required', 'class' => 'inteiro'));
        }
        die();
        break;
    case 'salvar':
        ob_clean();
        if($_POST['itcid'] || $_POST['igrid']){
            echo $controllerIniciativa->salvar($_POST);
            die;
            break;
        }else{
            echo $controllerIniciativa->salvarTiposObra($_POST,$_GET['iniid']);
            die;
            break;
        }
    case 'inativar':
        ob_clean();
        $controllerIniciativa->inativar();
        break;
    case 'excluirTipoObra':
        ob_clean();
        $controllerIniciativa->excluirTipoObra($_REQUEST);
        break;
    default:
        $categoria = $controllerIniciativa->buscaCategoria($_GET['iniid']);
        $disabledCategoria = $categoria ? 'disabled' : '';
        break;
}
//Verifica se ICINICATIVA
if (!isset($_GET['iniid'])) {
    $url = "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativas";
    simec_redirecionar($url, 'error', "É necessário incluir a Iniciativa.");
}
?>
<div class="ibox">
    <input type="hidden" id="iniid" name="iniid" value="<?= $_GET['iniid'] ?>">
    <input type="hidden" id="iniobraproinfancia" name="iniobraproinfancia" value="<?= $mdIniciativa->iniobraproinfancia ?>">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Iniciativa - Itens de Composição</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">

            <form method="post" name="formItem" id="formItem" class="form form-horizontal">

                <div class="row">
                    <!-- Itens: Campo de seleção única, Campo obrigatório; -->
                    <div class="col-md-offset-3 col-md-6 center">
                        <div class="form-group">
                            <div class="col-sm-10 center">
                        <?php if($mdIniciativa->intoid != '1'){ ?>
                                <label class="col-sm-4 control-label">
                                    Nome do Item:
                                </label>
                                <div class="col-sm-8 center" style='width: 300px'>
                                    <select name="itcid" id="itcid" class="chosen-select"
                                            data-placeholder="Selecione">
                                        <option></option>
                                        <?php
                                        $itens = $objModeltensComposicao->listaItemComposicao($_GET['iniid']);
                                        if($itens){
                                        foreach ($itens as $iten) {
                                            ?>
                                            <option value="<?= $iten['codigo'] ?>"><?= $iten['descricao'] ?></option>
                                        <?php }} ?>
                                    </select>
                                </div>
                         <?php }else{
                                echo '<input type="hidden" id="itcid" value="" />';
                                echo $simec->select('etaid', 'Etapa de Ensino', $etapaEnsino, $arrEtapasEnsino, array('required' => 'required', 'maxlength' => '1'));
                                echo $simec->select('octid', 'Categoria', $categoria, $arrCategorias, array('required' => 'required', 'maxlength' => '1', $disabledCategoria));

                                if($categoria&&$etapaEnsino){
                                    $_REQUEST['etaid']          = $etapaEnsino;
                                    $_REQUEST['octid']          = $categoria;
                                    $_REQUEST['otpsituacao']    = 'A';
                                    $_REQUEST['NOTIN']['otp.otpid'] = "(SELECT otpid FROM par3.iniciativa_tipo_obra WHERE iniid = {$_GET['iniid']})";
                                    echo $simec->select('otpid', 'Tipo de Obra', $controllerObra->model->otpid, $controllerObra->getComboTipoObra(), array('required' => 'required', 'class' => 'inteiro'));
                                }

                               }?>
                            </div>
                            <div class="col-sm-2 center">
                                <button type="submit" id="salvarItem" class="btn btn-success">
                                    <i class="fa fa-plus"></i> Adicionar
                                </button>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-sm-11 text-center">
                    </div>
                    <div class="row">
                        <div class="col-sm-12 text-center">
                            <p><br></p>
                        </div>
                    </div>
                    <br>
                    <div class="center">
                        <div class="col-sm-12 text-center">
                            <?php
                            $controllerIniciativa->listarItems($_GET['iniid']);
                            ?>
                        </div>
                    </div>
            </form>
        </div>
    </div>
</div>
</div>

<?php if($mdIniciativa->intoid != '1'){ ?>
<div class="ibox-content">
    <div class="row">
        <form name="form-grupo" id="form-grupo" method="post" class="form-horizontal">

            <div class="row">
                <div class="col-md-offset-3 col-md-6 center">
                    <div class="form-group">
                        <label class="col-sm-4 control-label">
                            Grupo de Itens:
                        </label>
                        <div class="col-sm-6">
                            <select name="igrid" id="igrid" class="chosen-select"
                                    data-placeholder="Selecione">
                                <option></option>
                                <?php
                                $dimensoes = $objModeGruposltens->listaGrupo($_GET['iniid']);
                             if($dimensoes){
                                foreach ($dimensoes as $dimensao) {
                                    ?>
                                    <option value="<?= $dimensao['codigo'] ?>"><?= $dimensao['descricao'] ?></option>
                                <?php } }?>
                                <select>
                        </div>
                        <button type="submit" id="salvarGrupo" class="btn btn-success"><i
                                    class="fa fa-plus"></i> Adicionar
                        </button>
                    </div>
                </div>
                <div class="row">
                    <div class="col-sm-12 text-center">
                        <p><br></p>
                    </div>
                </div>
                <div class="center">
                    <div class="col-sm-12 text-center">
                        <?php
                        $controllerIniciativa->listarGrupo($_GET['iniid']);
                        ?>
                    </div>
                </div>
        </form>
    </div>
</div>
<?php } ?>
<div class="ibox-footer">
    <div class="center">
        <a href="par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&iniid=<?= $_GET['iniid'] ?>"
           class="btn btn-primary xls"
           style="width: 110px;">
            <i class="fa fa-mail-reply"></i> Voltar
        </a>


      <button type="button" class="btn btn-success xls"
                style="width: 110px;" id="bt-proximo">
            <i class="fa fa-mail-forward"></i> Próximo

         </button>


    </div>
</div>
<script>

<?php if($mdIniciativa->intoid != '1'){ ?>
var objeto = 'Item de Composição';
<?php }else{?>
var objeto = 'Tipo de Obra';
<?php }?>

	jQuery('#bt-proximo').click(function (evt) {
		var iniid = $('[name=iniid]').val();
		window.location.href = "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=informacaoDemograficaEducacional&iniid=" +iniid;
	});

	jQuery('#octid, #etaid').change(function()
	{
		var etaid = jQuery('#etaid').val();
		var octid = jQuery('#octid').val();
		jQuery('#otpid').parent().parent().remove();
		if(etaid != '' && octid != ''){
    		$.ajax({
                type: "POST",
                url: window.location.href,
                data: '&req=atualizaTipoObra&etaid='+etaid+'&octid='+octid,
                async: false,
                success: function (resp) {
                	jQuery('#octid').parent().parent().after(resp);
                }
            });
		}
	});

    jQuery('#salvarItem').click(function (evt) {

        var itcid = $('#itcid').val();
        var otpid = $('#otpid').val();
        if(itcid == "" && otpid == ''){
            alert('Informe o '+objeto+'!');
            return false;
        }

        evt.preventDefault();
        var iniid = $('#formItem').find('[name=iniid]').val();
        var msg = 'Incluir';
        if (iniid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este "+objeto+"?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
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

    jQuery('#salvarGrupo').click(function (evt) {
        var igrid = $('#igrid').val();


        if(igrid == ""){
            alert('Informe o Grupo!');
            return false;
        }

        evt.preventDefault();
        var iniid = $('#form-grupo').find('[name=iniid]').val();
        var msg = 'Incluir';
        if (iniid) {
            msg = 'Alterar';
        }

        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Grupo ?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
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

    function salvarItem() {

        var itcid = $('#itcid').val();
        var otpid = $('#otpid').val();
        var iniid = $('#iniid').val();

        var caminho = window.location.href;
        var action = '&guia=cadastroIniciativaItensComposicao&req=salvar&itcid=' + itcid + '&iniid=' + iniid + '&otpid=' + otpid;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,

            success: function (resp) {

               if(resp == 'erro'){
                  var msg = "O "+objeto+" selecionado já existe para Iniciativa desejada!"
               }else{
                   var msg = objeto+" Salvo com sucesso"
               }

                var res = 1;
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, msg);
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formItem").find("#" + err).parent("div");
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
    function salvarGrupo() {


        var igrid = $('#igrid').val();
        var iniid = $('#iniid').val();
        var proinf = $('#iniobraproinfancia').val();

        var caminho = window.location.href;
        var action = '&guia=cadastroIniciativaItensComposicao&req=salvar&igrid=' + igrid + '&iniid=' + iniid + '&proinf=' + proinf;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,

            success: function (resp) {
               // alert(resp);
                if(resp == 'erro'){
                    var msg = "O Grupo selecionado já existe para Iniciativa desejada!"
                }else{
                    var msg = "Grupo Salvo com sucesso"
                }
                var res = 1;
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, msg);
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formItem").find("#" + err).parent("div");
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

    function removerItem(id) {
        swal({
                title: " Excluir",
                text: "Tem certeza que deseja excluir o "+objeto+" selecionado?",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {
                     excluirItem(id);
                    $("#formItem").submit();
                }
            });
    }

    function excluirItem(id) {

        var caminho = window.location.href;
        var action = '&guia=cadastroIniciativaItensComposicao&req=inativar&iigid='+id;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = 1;
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, objeto+" Excluido com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formItem").find("#" + err).parent("div");
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

    function removerTipoObra(id)
    {
        swal({
            title: " Excluir",
            text: "Tem certeza que deseja excluir o "+objeto+" selecionado?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }
        , function (isConfirm) {
            if (isConfirm) {
                 excluirTipoObra(id);
                $("#formItem").submit();
            }
        });
    }

    function excluirTipoObra(id) {

        var caminho = window.location.href;
        var action = '&guia=cadastroIniciativaItensComposicao&req=excluirTipoObra&otpid='+id;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = 1;
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, objeto+" Excluido com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formItem").find("#" + err).parent("div");
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

    function removerGrupo(id) {
        swal({
                title: " Excluir",
                text: "Tem certeza que deseja excluir o grupo selecionado?",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {

                    excluirGrupo(id);
                    $("#form-grupo").submit();
                }
            });
    }

    function excluirGrupo(id) {

        var caminho = window.location.href;
        var action = '&guia=cadastroIniciativaItensComposicao&req=inativar&iigid='+id;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = 1;
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, "Grupo Excluido com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formItem").find("#" + err).parent("div");
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

