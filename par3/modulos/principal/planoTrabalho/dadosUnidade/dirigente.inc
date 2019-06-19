<?php

$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$mItr = new Par3_Controller_InstrumentoUnidade();
$nomeItr = $mItr->pegarNomeEntidade($_REQUEST['inuid']);
switch ($_REQUEST['req']) {
    case 'pesquisar':
        echo $controllerInstrumentoUnidadeEntidade->listaDirigenteMunicipioAtivos($_REQUEST);
        echo $controllerInstrumentoUnidadeEntidade->listaDirigenteMunicipio($_REQUEST, false);
        die;
        break;
    case 'pdf':
        ob_clean();
        echo $controllerInstrumentoUnidadeEntidade->listaDirigenteMunicipio($_REQUEST, true);
        die;
        break;
    case 'xls':
        ob_clean();
        header("Content-Disposition: attachment; filename=Dirigentes".$nomeItr.".xls");
        echo $controllerInstrumentoUnidadeEntidade->listaDirigenteMunicipio($_REQUEST, true);
        die;
        break;
    case 'visualizar':
    case 'visualizarRemover':
        $renderDirigente = new Par3_Controller_Entidade();
        $objEntidade = new Par3_Model_InstrumentoUnidadeEntidade($_REQUEST['entid']);
        $arrEsconde = array('enttelefonefax' => 'N');
        $renderDirigente->formPessoaFisica('disabled', $objEntidade, null, $arrEsconde);
        echo '<div class="ibox-title"><div class="row"><div class="col-md-12"><h3>Informações Adicionais</h3></div></div></div>';
        $renderDirigente->formDirigente('disabled', $objEntidade, null);
        if ($_REQUEST['req'] == 'visualizarRemover')
            echo $simec->data('entdt_fim_mandato', 'Fim do mandato', '', array('required'));
        die;
        break;
    case 'remover':
        $arrAtual['entid'] = $_REQUEST['entid'];
        $arrAtual['entusucpfinativacao'] = $_SESSION['usucpf'];
        $arrAtual['entdtinativacao'] = date("d/m/Y h:i:s");
        $arrAtual['entdt_fim_mandato'] = $_REQUEST['entdt_fim_mandato'];
        $arrAtual['entstatus'] = 'I';

        $modelEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $modelEntidade->popularDadosObjeto($arrAtual);
        $modelEntidade->salvar();
        $modelEntidade->commit();

        $modelUsuariosistema = new Seguranca_Model_Usuariosistema();

        $entcpf = limpar_numero($_REQUEST['entcpf']);

        $modelUsuariosistema->bloqueiaUsuario($entcpf, SISID_PAR3);
        $modelUsuariosistema->bloqueiaUsuario($entcpf, SISID_OBRAS2);
        $modelUsuariosistema->bloqueiaUsuario($entcpf, SISID_PAR);

        $modelUsuariosistema->commit();
        die;
        break;
    case 'desbloquear':

        $modelUsuariosistema = new Seguranca_Model_Usuariosistema();

        $modelUsuario = new Seguranca_Model_Usuario();

        $entcpf = limpar_numero($_REQUEST['entcpf']);

        $modelUsuariosistema->ativaUsuario($entcpf, SISID_PAR3);
        $modelUsuariosistema->ativaUsuario($entcpf, SISID_OBRAS2);
        $modelUsuariosistema->ativaUsuario($entcpf, SISID_PAR);
        $modelUsuariosistema->commit();

        $modelUsuario->ativarUsuario($entcpf);
        $modelUsuario->commit();

        die;
        break;
    case 'inserir':
        $controllerInstrumentoUnidadeEntidade->salvarInformacoesDirigenteMunicipal($_REQUEST);

        $modelUsuariosistema = new Seguranca_Model_Usuariosistema();

        $modelUsuario = new Seguranca_Model_Usuario();

        $entcpf = limpar_numero($_REQUEST['entcpf']);

        $modelUsuariosistema->ativaUsuario($entcpf, SISID_PAR3);
        $modelUsuariosistema->ativaUsuario($entcpf, SISID_OBRAS2);
        $modelUsuariosistema->ativaUsuario($entcpf, SISID_PAR);
        $modelUsuariosistema->commit();

        $modelUsuario->ativarUsuario($entcpf);
        $modelUsuario->commit();

        die;
        break;
    default:
        break;
}

global $simec;

$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;

$modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
if($modelInstrumentoUnidadeEntidade->pegarEntidAtivoPorTipo($inuid, $tenid)) {
    $disableIncluir = 'disabled';
}
?>
<script type="text/javascript" src="js/sistema/tabelaapoio/iniciativas/areasrelacionadas.js"></script>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Dirigente Municipal de Educação</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <form name="form-filtro" id="form-filtro" class="form-horizontal">
            <div class="row">
                <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>
                <input type="hidden" name="entid" id="entid"/>
                <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
                <input type="hidden" name="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <?php echo $simec->input('entnome', 'Nome', $entnome, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->cpf('entcpf', 'CPF', $entcpf,array(),array('input-size' => '8')); ?>
                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Status:</label>
                            <div class="col-md-5 col-lg-5">
                                <div class="radio radio-default radio-inline ">
                                    <input type="radio" name="entstatus" id="entstatus1" value="A">
                                    <label for="entstatus1">Ativo</label>
                                </div>
                                <div class="radio radio-default radio-inline ">
                                    <input type="radio" name="entstatus" id="entstatus2" value="I">
                                    <label for="entstatus2">Inativo</label>
                                </div>
                                <div class="radio radio-default radio-inline">
                                    <input type="radio" name="entstatus" id="entstatus3" checked="checked"
                                           value="">
                                    <label for="entstatus3">Todos</label>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

            </div>
            <br>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" class="btn btn-primary" id="pesquisar-dirigente"><i class="fa fa-search"></i>Pesquisar
                    </button>
                    <button type="button" class="btn btn-success" id="btn-incluir" <?php echo $disableIncluir;?>><i class="fa fa-plus"></i> Incluir
                    </button>
                    |
                    <button type="button" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                    <button type="button" class="btn btn-warning" style="width:110px;" id="pdf"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                    |
                    <button type="reset" class="btn btn-success" id="limpar-formfiltro"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
        </form>
    </div>
    <div class="ibox-content">
        <div id="dirigenteListagem">
            <?php
            echo $controllerInstrumentoUnidadeEntidade->listaDirigenteMunicipioAtivos($_REQUEST);
            echo $controllerInstrumentoUnidadeEntidade->listaDirigenteMunicipio($_REQUEST);
            ?>
        </div>
        <div>
            <?php
            $arrParamHistorico = array();
            $arrParamHistorico['inuid'] = $inuid;
            $arrParamHistorico['tenid'] = $tenid;
            $arrParamHistorico['booCPF'] = true;
            $controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
        </div>
    </div>
</div>

<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_dirigente_visualizar" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="form_modal" id="form_modal" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </button>
                    <h3 id="modal_titulo">Dirigente - Visualizar</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5" id="modal_botoes">

                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório Dirigentes - <?php echo $nomeItr ?><br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content">
    </div>
</div>
<script>
    function editarDirigente(id) {
        window.location.assign(window.location.href + "&submenu=form&entid=" + id + '&inuid=' + $('#inuid').val());
    }

    function visuaizarDirigente(id) {
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        var action = '&req=visualizar&entid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#modal_titulo").html('Dirigente - Visualizar');
                $("#modal_botoes").html('<button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>');
                $("#conteudo-modal").html(resp);
                verificaCurso();
                $("#modal_dirigente_visualizar").modal("show");

            }
        });
    }

    function removerDirigente(id) {
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        var action = '&req=visualizarRemover&entid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#modal_titulo").html('Dirigente - Inativar');
                $("#modal_botoes").html('<button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>' +
                    ' <button type="button" class="btn btn-danger" id="btn-incluir" onclick="inativarDirigente();"> Inativar </button>');
                $("#conteudo-modal").html(resp);
                verificaCurso();
                $("#modal_dirigente_visualizar").modal("show");
            }
        });
    }

    function inativarDirigente() {
        var obData = new Data();

        if ($("#entdt_fim_mandato").val() == '' || !validaData(document.form_modal.entdt_fim_mandato)) {
            alert('Preenchimento obrigatório do campo Fim do mandato.');
            return false;
        } else if ($("#entdt_inicio_mandato").val() != '') {
            if (!obData.comparaData($("#entdt_fim_mandato").val(), $("#entdt_inicio_mandato").val(), ">")) {
                alert("A Data do Fim do mandato deve ser superior a data da nomeação.");
                return false;
            }
        }

        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        var action = '&req=remover&' + $("#form_modal").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#modal_dirigente_visualizar").modal("hide");
                alert(" Operação realizada com sucesso!");
                $("#btn-incluir").removeAttr('disabled');
                $("#pesquisar-dirigente").trigger("click");
            }
        });
    }

    function desbloquearDirigente(id) {
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        var action = '&req=desbloquear&entcpf=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                alert(" Operação realizada com sucesso!");
                $("#pesquisar-dirigente").trigger("click");
            }
        });
    }


    $(".xls").on("click", function () {
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#pdf").on("click", function () {
        var caminho = window.location.href;
        var action = '&req=pdf&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#impressao-content").html(resp);
                var container = $("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                table.css('width', '100%')
                table.css('border-collapse', 'collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border', '1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height', '50px');
                td.css('text-align', 'center');
                var w = window.open();
                w.document.write($('#div-impressao').html());
                w.document.title = "Relatório - <?php echo $nomeItr;?> - Dirigentes";
                w.print();
                w.close();
                $("#impressao-content").html("");
                return true;
            }
        });
    });

    $("#limpar-formfiltro").click(function (evt) {
        $("#entcpf").val('');
        $("#entnome").val('');
        evt.preventDefault();
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#dirigenteListagem").html(resp);
                return true;
            }
        });
    });

    $("#pesquisar-dirigente").click(function (evt) {
        evt.preventDefault();
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A';
        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#dirigenteListagem").html(resp);
                return true;
            }
        });
    });
    $("#btn-incluir").click(function () {
        window.location.assign(window.location.href + "&submenu=form&inuid=" + $('#inuid').val());
    });

    function verificaCurso() {
        if ($('#entcursomec-t:checked').val() == 't') {
            $('#entcursomecdescricao').closest('.form-group').removeClass('hidden');
            $('#entcursomecdescricao').attr('required', 'required');
        } else {
            $('#entcursomecdescricao').closest('.form-group').addClass('hidden');
            $('#entcursomecdescricao').val('');
            $('#entcursomecdescricao').removeAttr('required');
        }
    }
</script>