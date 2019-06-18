<?php
if (!isset($_REQUEST['estuf']) || $_REQUEST['estuf'] == '') {
    echo ' <div class="col-md-12">
                    <div class="well">
                       <fieldset>
                           Não é possivel cadastrar dados na aba Agenda. Selecione um município na aba Lista.
                       </fieldset>
                    </div>
               </div>';
    die();
}

include_once APPRAIZ . 'sase/classes/Model/Agendacomissaocoordestado.class.inc';

$cadastro = new Agendacomissaocoordestado();


if($_POST['ajaxeditarEtapa']){

    $isTrue = $cadastro->editarAgendaEstado($_POST);
    die();
}
if ($_POST['action'] != '') {
    switch ($_POST['action']) {
        case 'salvarEtapa':

            $isTrue = $cadastro->salvarAgendaEstado($_POST);
            if ($isTrue == 1) {
                $msn = "Operação realizada com sucesso!";
            } elseif ($isTrue == 2) {
                $msn = "Data Inválida!";
            } else {
                $msn = "Ocorreu um erro ao executar operação!";
            }
            $success = "<script>
                                alert('{$msn}');
                                location.href='sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=edit_estado&estuf={$_GET['estuf']}';
                            </script>";
            die($success);


    }
}


if ($_REQUEST['request'] == 'excluirItemAgendaTrabalho') {
    $cadastro->excluirItemAgendaTrabalho($_REQUEST['aceid']);
    $success = "<script>
                                alert('Operação realizada com sucesso!');
                                    location.href='sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=edit_estado&estuf={$_GET['estuf']}';
                            </script>";

    $_REQUEST['request'] = 'excluido';
    die($success);
}

function redir($url = null, $msg = null)
{
    $script .= '<script>';
    if (!empty($msg))
        $script .= '	alert(\'' . $msg . '\');';

    if (!empty($url))
        $script .= '	location.href=\'' . $url . '\';';
    else
        $script .= '	history.go(-1);';

    $script .= '</script>';
    die($script);
}



global $db;
$sql = "select count(estuf)  from territorios.estado WHERE estuf = '{$_GET[estuf]}'";

$estufesiste = $db->pegaum($sql);

if($estufesiste  ==  '' ) {
    $alerta2 = "<script> alert('O codigo do estado esta incorreto!');
              location.href= 'sase.php?modulo=principal/agendamento_trab_estado&acao=A';
        </script>";
    echo $alerta2;

}
$ano_passado =  (date("Y")-1);
?>
<style>
    @media (min-width: 568px) {
        .modal-xl {
            width: 70%;
            max-width: 1200px;
        }
    }
</style>
<script>

    function salvaEtapa() {
        var etapa = $('#etapa').val();
        var eacid = etapa;
        var estuf = $('#estuf').val();
        var aceacao = trim($('#aceacao').val());
        var aceresponsavel = trim($('#aceresponsavel').val());
        var aceprazo = $('#aceprazo').val();
        var aceobservacao = trim($('#aceobservacao').val());


        var msg = [];

        if (etapa == 0) {
            msg.push('Campo Etapa é obrigatório');
        }

        if (aceacao == '') {
            msg.push('Campo Ação é obrigatório');
        }
        if (aceresponsavel == '') {
            msg.push('Campo Responsável é obrigatório');
        }
        if (aceprazo == '') {
            msg.push('Campo Previsão de Execução é obrigatório');
        }
//        if (aceobservacao == '') {
//            msg.push('Campo Observações é obrigatório');
//        }


        if (msg.length > 0) {
            bootbox.alert('Os seguintes campos são obrigatórios e devem ser preenchidos:<br /><br /><ul><li>' + msg.join(';</li><li>') + '.</li></ul>');
            return false;
        }

        $('#formulario_').submit();


    }


    function editarEtapa(id) {
        var etapa = $('#etapa_'+id).val();
        var eacid = etapa;
        var estuf = $('#estuf_'+id).val();
        var aceid = $('#aceid_'+id).val();
        var aceacao = trim($('#aceacao_'+id).val());
        var aceresponsavel = trim($('#aceresponsavel_'+id).val());
        var aceprazo = $('#aceprazo_'+id).val();
        var aceobservacao = trim($('#aceobservacao_'+id).val());
        var aceconcluida = trim($('#aceconcluida_'+id).val());


        var datainscricao = aceprazo.substring(6, 11);
        var msg = [];
        if(<?=$ano_passado?> != datainscricao){

            if (etapa == 0) {
                msg.push('Campo Etapa é obrigatório');
            }

            if (aceacao == '') {
                msg.push('Campo Ação é obrigatório');
            }
            if (aceresponsavel == '') {
                msg.push('Campo Responsável é obrigatório');
            }
            if (aceprazo == '') {
                msg.push('Campo Previsão de Execução é obrigatório');
            }
            if (aceobservacao == '') {
                msg.push('Campo Observações é obrigatório');
            }


            if (msg.length > 0) {
                bootbox.alert('Os seguintes campos são obrigatórios e devem ser preenchidos:<br /><br /><ul><li>' + msg.join(';</li><li>') + '.</li></ul>');
                return false;
            }
        }
       $.ajax({
            url: 'sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=edit_estado&estuf=<?=$_GET['estuf']?>',
            type: 'POST',
            cache: false,
            data: { eacid: eacid,
                    estuf: estuf,
                    aceid: aceid,
                    aceacao: aceacao,
                    aceresponsavel: aceresponsavel,
                    aceprazo: aceprazo,
                    aceobservacao: aceobservacao,
                    aceconcluida: aceconcluida,
                    ajaxeditarEtapa: true},
            success: function (response) {
                $('#edtarAtividade_'+id).modal('hide');
               // alert("AGENDA DE TRABALHO DA COMISSÃO COORDENADORA ALTERADO COM SUCESSO!");
                location.reload();
            }
        });


    }


</script>

<br>

<div id="container">
    <div class="row">
        <div class="col-md-12">
            <?php
            $alerta = '';
            if($_SESSION['msg']) {
                $alerta = '<script> alert("'.$_SESSION[msg].'")</script>';
                echo $alerta;
                unset($_SESSION['msg']);
            }
            ?>

            <div id="formularioFiltros">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title text-center">
                            <b>CADASTRO DAS ETAPAS</b>
                        </h3>
                    </div>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <button type="button" class="btn btn-primary " data-toggle="modal" data-target="#cadastrarAcao">
                            <span class="glyphicon glyphicon-plus"></span> CADASTRAR AÇÃO
                        </button>
                        <!-- Modal -->
                        <div class="modal fade" id="cadastrarAcao" tabindex="-1" role="dialog"
                             aria-labelledby="cadastrarAcao">
                            <div class="modal-dialog  modal-xl" role="document">
                                <div class="modal-content">
                                    <div class="modal-header">
                                        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                                            <span aria-hidden="true">&times;</span></button>
                                        <h4 class="modal-title text-center" id="myModalLabel">CADASTRAR AÇÃO DA
                                            ETAPA</h4>
                                    </div>
                                    <div class="modal-body">
                                        <form name="formulario_" id="formulario_" class="form-horizontal" method="POST">
                                            <div class="form-group">
                                                <label class="col-md-2 control-label" for="etapa">Etapa:</label>
                                                <div class="col-md-10">
                                                    <select id="etapa" name="etapa" class="form-control">
                                                        <option value="0">Selecione</option>
                                                        <option value="1"> I. ETAPA - Organizar o Trabalho</option>
                                                        <option value="2"> II. ETAPA - Estudar o Plano</option>
                                                        <option value="3"> III. ETAPA - Monitorar continuamente as metas
                                                            e estratégias
                                                        </option>
                                                        <option value="4"> IV. ETAPA - Avaliar periodicamente o plano
                                                        </option>
                                                    </select>
                                                </div>
                                            </div>
                                            <input type="hidden" id="estuf" name="estuf"
                                                   value="<?= $_REQUEST['estuf'] ?>"/>
                                            <input type="hidden" id="action" name="action" value="salvarEtapa"/>
                                            <div class="form-group">
                                                <label class="col-md-2 control-label" for="aceacao">Ação:</label>
                                                <div class="col-md-10">
                                                    <textarea class="form-control" cols="40" id="aceacao"
                                                              name="aceacao"></textarea>
                                                </div>
                                            </div>


                                            <div class="form-group">
                                                <label class="col-md-2 control-label"
                                                       for="aceresponsavel">Responsável:</label>
                                                <div class="col-md-10">
                                                    <textarea class="form-control" id="aceresponsavel"
                                                              name="aceresponsavel"></textarea>
                                                </div>
                                            </div>
                                            <div class="form-group">
                                                <label class="col-md-2 control-label" for="aceprazo">Previsão de
                                                    Execução:</label>
                                                <div class="col-md-10">
                                                    <input id="aceprazo" maxlength="0" name="aceprazo" width="200px"
                                                           type="text" placeholder=""
                                                           class="form-control  campoData2 input-md">

                                                </div>
                                            </div>


                                            <div class="form-group">
                                                <label class="col-md-2 control-label"
                                                       for="aceobservacao">Observações:</label>
                                                <div class="col-md-10">
                                                    <textarea class="form-control" id="aceobservacao"
                                                              name="aceobservacao"></textarea>
                                                </div>
                                            </div>


                                    </div>
                                    <div class="modal-footer">
                                        <button type="button" class="btn btn-default" data-dismiss="modal">Cancelar
                                        </button>
                                        <button type="button" class="btn btn-primary" onclick="salvaEtapa()"><span
                                                    class="glyphicon glyphicon-plus"></span> Salvar
                                        </button>
                                    </div>
                                    </form>
                                </div>
                            </div>
                        </div>
                        <!--  -->
                    </fieldset>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>I. ETAPA - Organizar o Trabalho</legend>
                        <div class="form-group">
                            <div class="col-lg-12 col-md-12">
                                <?php echo $cadastro->ListaTrabalhosAgendados($_REQUEST['estuf'], 1); ?>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>II. ETAPA - Estudar o Plano</legend>
                        <div class="form-group">
                            <div class="col-lg-12 col-md-12">
                                <?php echo $cadastro->ListaTrabalhosAgendados($_REQUEST['estuf'], 2); ?>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>III. ETAPA - Monitorar continuamente as metas e estratégias</legend>
                        <div class="form-group">
                            <div class="col-lg-12 col-md-12">
                                <?php echo $cadastro->ListaTrabalhosAgendados($_REQUEST['estuf'], 3); ?>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>IV. ETAPA - Avaliar periodicamente o plano</legend>
                        <div class="form-group">
                            <div class="col-lg-12 col-md-12">
                                <?php echo $cadastro->ListaTrabalhosAgendados($_REQUEST['estuf'], 4); ?>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </div>

        <div class="col-lg-12">
            <div class="well" align="center">
                <button class="btn btn-primary" id="btn-historico">Histórico</i></button>
            </div>
        </div>
    </div>
</div>

<div class="modal fade modalAditivo" id="modalAditivo" tabindex="-1" role="dialog" aria-labelledby="modalAditivo"
     aria-hidden="true">
    <div class="modal-dialog" style="width:80%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span>
                    <span class="sr-only">Fechar</span>
                </button>
                <h4 class="modal-title">Histórico Agenda de trabalho da comissão coordenadora</h4>
            </div>
            <div class="modal-body" id="modal-body">
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>I. Etapa - Organizar o Trabalho</legend>
                            <?php echo $cadastro->dadosHistorico($_REQUEST['estuf'], 1); ?>
                        </fieldset>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>II. Etapa - Estudar o Plano</legend>
                            <?php echo $cadastro->dadosHistorico($_REQUEST['estuf'], 2); ?>
                        </fieldset>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>III. Etapa - Monitorar continuamente as metas e estratégias</legend>
                            <?php echo $cadastro->dadosHistorico($_REQUEST['estuf'], 3); ?>
                        </fieldset>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>IV. Etapa - Avaliar periodicamente o plano</legend>
                            <?php echo $cadastro->dadosHistorico($_REQUEST['estuf'], 4); ?>
                        </fieldset>
                    </div>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>


<script>
    jQuery('documento').ready(function () {
        $("#btn-historico").on("click", function () {
            $('#modalAditivo').modal('show');
        });

        $('.campoData2').datepicker();
    });

    function podeAlterar(str, id) {
        if (str == 's') {
            $('.frm-disabled' + id).attr('readonly', false);
            $('.btnExcluir' + id).show();
        } else {
            $('.frm-disabled' + id).attr('readonly', true);
            $('.btnExcluir' + id).hide();
        }
    }

    function limparAtividade(id) {
        textoConfimacao = 'Deseja excluir o registro?';
        bootbox.confirm(textoConfimacao, function (result) {
            if (result) {
                $('.frmExcluir_' + id).val('');
            }
        });
    }

    function excluirAtividade(aceid) {
        textoConfimacao = 'Deseja excluir o registro?';
        bootbox.confirm(textoConfimacao, function (result) {
            if (result) {
                url = location.href + '&request=excluirItemAgendaTrabalho&aceid=' + aceid;
                location.href = url;
            }
        });
    }

    function excluirLinha(el) {
        textoConfimacao = 'Deseja excluir a linha registro?';
        bootbox.confirm(textoConfimacao, function (result) {
            if (result) {
                $('#lineExcluir_' + $(el).attr('frm')).remove();
            }
        });
    }
</script>
<!--<div id="footer"></div>-->