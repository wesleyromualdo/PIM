<?php
$controllerObraTipo = new Par3_Controller_ObraTipo();
$controllerObraTipo->model->popularDadosObjeto($_POST);

switch ($_REQUEST['requisicao']) {
    case 'imprimir':
        ob_clean();
        $controllerObraTipo->listar($_REQUEST);
        die();
        break;
    case 'xls':
        ob_clean();
        $controllerObraTipo->listar($_GET);
        header("Content-Disposition: attachment; filename=Relatorio_TipoObra.xls");
        die;
        break;
    case 'inativar':
        ob_clean();
        echo $controllerObraTipo->remover($_POST);
        die();
        break;
    default:
        break;
}
?>

<style>
    .navbar-listagem {
        margin-top: 0px;
        float: left;
        width: 500px;
    }

</style>

<div class="ibox" id="painelProgramas">

    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Tipo de Obra</h2>
                <h3 class="center">Pesquisar</h3>
            </div>
        </div>
    </div>

    <div class="ibox-content">
        <form method="post" name="form-filtro-tipoObra" id="form-filtro-tipoObra" class="form form-horizontal">
            <div class="row">

                <div class="col-md-offset-2 col-md-8 center">
<!--                     <input type="hidden" name="req" id="req" value="pesquisar"/> -->

                    <?php echo $simec->input('otpdsc', 'Descrição', $controllerObraTipo->model->otpdsc, array('maxlength' => '255')); ?>

                    <div class="ibox float-e-margins">

                        <div class="ibox-content" id="avancado" style="display: none;">
                            <?php
                            echo $simec->input('otpid', 'Código', $controllerObraTipo->model->otpid, array('class' => 'inteiro'), array('input-size' => 2));

                            echo $simec->input('otpalunosatendidos', 'Alunos Atendidos', $controllerObraTipo->model->otpalunosatendidos, array('class' => 'inteiro'), array('input-size' => 2));
                            echo $simec->radio('otpsituacao', 'Situação', $controllerObraTipo->model->otpsituacao, array('A' => 'Ativo', 'I' => 'Inativo'), array());

                            echo $simec->select('otpesfera', 'Esfera', $controllerObraTipo->model->otpesfera, array('' => ' .: Selecione :. ', 'E' => 'Estadual', 'M' => 'Municipal', 'T' => 'Todos'), array('maxlength' => '1'));
                            echo $simec->select('octid', 'Categoria', $controllerObraTipo->model->octid, $controllerObraTipo->getComboCategoriaObras(), array('maxlength' => '1'));
                            echo $simec->select('tpoid', 'Tipologia Obras 2.0', $controllerObraTipo->model->tpoid, $controllerObraTipo->getComboTipologiaObras2(), array('maxlength' => '1'));
                            ?>
                        </div>

                        <div class="ibox-title">
                            <div class="ibox-tools">
                                <a id="btn-avancado">
                                    Opções Avançadas <i id="chevron" class="fa fa-chevron-down"></i>
                                </a>
                            </div>
                        </div>
                    </div>
                </div>

            </div>
            <div class="center">
                <div class="center">

                    <button type="submit" class="btn btn-primary" id="pesquisar-tipoobra">
                        <i class="fa fa-search"></i> Pesquisar
                    </button>

                    <button type="button" class="btn btn-success" id="btn-incluir"
                            onclick="window.location.href='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=tipo_obra' ">
                        <i class="fa fa-plus"></i> Incluir
                    </button>
                    |
                    <button type="button" class="btn btn-primary xls" style="width:110px;" id="gerarExcel"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                    <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                </div>

            </div>
        </form>
    </div>

    <div class="ibox-content">
        <?php echo $controllerObraTipo->listar($_POST); ?>
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
                    Relatório Programas<br>
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
    function editar(otpid) {
        window.location = "/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=tipo_obra&otpid=" + otpid;
    }

    function remover(id) {
        var cod = $('a[href*="' + id + '"]').closest('tr').find('td').eq(2).text();
        var descricao = $('a[href*="' + id + '"]').closest('tr').find('td').eq(3).text();
        var confirmado = false;
        swal({
            title: "Remover Tipo de Obra",
            text: "Tem certeza que deseja remover o Tipo de Obra: <br> <b>" + cod + " - " + descricao + " </b>?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false

        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action = '&requisicao=inativar&otpid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        console.log(resp);
                        if (resp != 'erro') {
                            msgSuccess(caminho, "Tipo Obra removido com sucesso");
                            return false;
                        }
                        swal("Erro.", "Ocorreu um erro ao remover o Tipo Obra.", "error");
                    }
                });
            }
        });
    }

    $(function () {
        $("#gerarExcel").click(function () {
            window.location.assign(window.location.href + "&requisicao=xls&" + jQuery('#form-filtro-tipoObra').serialize() + '&' + $('.form-listagem').serialize());
        });

        $("#imprimir").click(function () {
            var caminho = window.location.href;
            //var action = '&requisicao=imprimir&' + $("#form-filtro-tipoObra").serialize() + '&' + $('.form-listagem').serialize();
            var action = '&requisicao=imprimir&' + $("#form-filtro-tipoObra").serialize()
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
                    $('.navbar').remove();
                   	table.css('width','100%')
                   	table.css('border-collapse','collapse');
                   	container.find(".popover,button,.modal").remove();
                   	container.find("table,td,th").css('border','1px solid black');
                   	container.find('tr:even').css('background-color', 'silver');
                   	th.css('height','50px');
                   	td.css('text-align','center');
                    var w = window.open();
                    w.document.write($('#div-impressao').html());
                    w.document.title = 'Relatório - Tipo de Objeto';
                    w.print();
                    w.close();
                    $("#impressao-content").html("");
                    //atualizarListagem($("#form-filtro-tipoObra").serialize()+'&'+$('.form-listagem').serialize());
                    return true;
                }
            });
        });

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
    });
</script>