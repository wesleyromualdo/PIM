<?php

include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';

$controllerIniciativaFonteRecurso = new Par3_Controller_IniciativaFonteRecurso();

switch ($_REQUEST['requisicao']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaFonteRecurso->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'alterarSitucaao':
        ob_clean();
        $resp = $controllerIniciativaFonteRecurso->alteraSituacao($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'editarFonteRecurso':
        ob_clean();
        $resp = $controllerIniciativaFonteRecurso->recuperar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'removerFonteRecurso':
        ob_clean();
        echo $controllerIniciativaFonteRecurso->inativar($_REQUEST);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaFonteRecurso->listar($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        echo $controllerIniciativaFonteRecurso->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Fonte_Recurso.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        echo $controllerIniciativaFonteRecurso->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Fonte_Recurso.pdf");
        die;
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

$modelPlanoInterno = new Par3_Model_IniciativaPlanoInterno();
//$sqlPTRES = $modelPlanoInterno->pegarSQLSelectCombo();
$sqlPTRES = "SELECT  inplintid as codigo, i.inplintdsc||' ( '||i.inplintptres||' ) '||pp.prgdsc as descricao FROM par3.iniciativa_plano_interno i 
			INNER JOIN par3.programa pp ON pp.prgid = i.inplinprog
			WHERE inplintsituacao = 'A' 
			ORDER BY i.inplintptres";

global $simec;
?>
<style>
    .switchery {
        background-color: #ed5565 ;
    }
    .navbar-listagem {
        margin-top: 0px;
        float: right;
        width: 500px;
    }
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Fonte de Recurso</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form method="post" name="formFonteRecurso" id="formFonteRecurso" class="form form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div>
                            <input type="hidden" name="ifrid" id="ifrid" value="<?= $_GET['ifrid']; ?>">
                            <?php
                            echo $simec->select('inplintid', 'Plano Interno (PTRES)', $_GET['inplintid'], $sqlPTRES, array('maxlength' => '255', 'required'), array('input-size' => '8'));
                            ?>
                        </div>
                        <!--filtros-->
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código
                                da Fonte: *</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="ifrcodigofonte" id="ifrcodigofonte" maxlength="255"
                                       class="form-control"
                                       value="<?php echo ($_GET['ifrcodigofonte']); ?>"/>
                            </div>
                        </div>
                        <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Descrição
                                da Fonte: *</label>
                            <div class="col-sm-7 col-md-8">
                                <input type="text" name="ifrdescicaofonte" id="ifrdescicaofonte" maxlength="255"
                                       class="form-control"
                                       value="<?php echo ($_GET['ifrdescicaofonte']); ?>"/>
                            </div>
                        </div>
                        <div>
                            <div id="nivel-div">
                                <?php
                                echo $simec->select('ifrano', 'Ano', $_GET['ifrano'], pegaAnoAFrente(5), array('maxlength' => '255', 'required'), array('input-size' => '8'));
                                ?>
                            </div>

                        </div>
                        <br>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-fonteRecurso"><i class="fa fa-search"></i>
                    Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-salvarFonteRecruso">
                    <i class="fa fa-save"></i> Salvar
                </button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                            class="fa fa-file-excel-o"></i> Gerar Excel
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i
                            class="glyphicon glyphicon-print"></i> Imprimir
                </button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>

            </div>
        </div>
        <br>
        <br>


    </div>

    <div class="ibox-content">
        <div id="fonteRecursoListagem">
            <?php
            $controllerIniciativaFonteRecurso->listar($_REQUEST);
            ?>
        </div>
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
                    Relatório Modalidades<br>
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
<br>

<script>


    function alterasituacao(id, situacao) {

        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=alterarSitucaao&ifrid=' + id + '&situacao=' + situacao;

        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                msgSuccess(caminho, "Situação Alterada Com Sucesso!");
                return true;

            }
        });


    }
    jQuery(document).find("#formModalidade").on('click', "input[name=modsituacao ][value=I]", function () {
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
    });


    jQuery("#limpar").click(function () {

        $('#formFonteRecurso').resetForm();
        $("#ifrid").val('');
        $("#inplintid").val('').trigger("chosen:updated");
        $("#ifrano").val('').trigger("chosen:updated");
        location.reload();



    });


    jQuery('#btn-salvarFonteRecruso').click(function (evt) {
        evt.preventDefault();

        var ptres = $('#inplintid').val();
        var ano = $('#ifrano').val();
        var descricaoFonte = $('#ifrdescicaofonte').val();
        var codigoFonte = $('#ifrcodigofonte').val();



        if (ptres == "") {

            alert('O Campo PTRES É Obrigatório!')
            return false;

        }
        if (codigoFonte == "") {

            alert('O Campo Código Da Fonte É Obrigatório!')
            return false;

        }
        if (descricaoFonte == "") {

            alert('O Campo Descrição Da Fonte É Obrigatório!')
            return false;

        }
        if (ano == "") {

            alert('O Campo Ano É Obrigatório!')
            return false;

        }

        var ifrid = $('#formFonteRecurso').find('[name=ifrid]').val();
        var msg = 'Incluir';
        if (ifrid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> esta Fonte de Recurso?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarFonteRecurso();
                }
                return false;
            }
        );
    });

    function salvarFonteRecurso() {


        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=salvar&' + $("#formFonteRecurso").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                console.log(resp);
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho, "Fonte de Recurso Salva com Sucesso!");
                    return false;
                }
                swal("Erro.", "Existe Fonte de Recurso  Ultilizando o mesmo  PTRES ,Código e Ano.", "error");

                return false;
            }
        });
        return false;
    }

    $("#pesquisar-fonteRecurso").click(function (evt) {
        evt.preventDefault();
        var caminho = window.location.href;
        var action = 'requisicao=pesquisar&' + $("#formFonteRecurso").serialize();
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#fonteRecursoListagem").html(resp);
                return true;
            }
        });
    });


    function removerFonteRecurso(id) {
        var confirmado = false;
        swal({
            title: "Remover Fonte de Recurso",
            text: "Tem certeza que deseja remover essa Fonte de Recurso?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = '<?php echo $urlGuia;?>';
                var action = '&requisicao=removerFonteRecurso&ifrid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        if (resp != 'erro') {
                            msgSuccess(caminho, "Fonte de Recurso removida com sucesso");
                            return false;
                        }
                        swal("Erro.", "Ocorreu um erro ao remover a Fonte de Recurso.", "error");
                    }
                });
            }
        })
    }


    function editarFonteRecurso(id) {

        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=editarFonteRecurso&ifrid=' + id;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {

                var resp = JSON.parse(resp);
                $('#ifrid').val(resp['ifrid']);
                $("#inplintid").val(resp['inplintid']).trigger("chosen:updated");
                $('#ifrcodigofonte').val(resp['ifrcodigofonte']);
                $('#ifrdescicaofonte').val(resp['ifrdescicaofonte']);
                $("#ifrano").val(resp['ifrano']).trigger("chosen:updated");

            }
        });
    }


    $(".xls").on("click", function () {
        window.location.assign(window.location.href + "&requisicao=xls&" + jQuery('#formFonteRecurso').serialize() + '&' + $('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=imprimir&' + $("#formFonteRecurso").serialize() + '&' + $('.form-listagem').serialize();
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
                w.document.title = 'Relatório - Fonte Recurso';
                w.print();
                w.close();
                $("#impressao-content").html("");
               // atualizarListagem($("#formFonteRecurso").serialize() + '&' + $('.form-listagem').serialize());
                return true;
            }
        });
    });


</script>