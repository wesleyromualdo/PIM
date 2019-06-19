<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 03/11/2015
 * Time: 10:08
 */

ob_start();
global $db;
$usucpf = $_SESSION['usucpf'];
$diretoria = 2;

// --------------- Dependências../../sase/classes/AvaliadorEducacional.class.inc
include_once APPRAIZ . 'sase/classes/RelatorioAvaliadorRedeae.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";

switch($_REQUEST['acao']){
    case 'novo':
        $url = validaUrlPerfil('PlanoTrabalho', null, $diretoria);
        echo "<script>window.location.href = '{$url}';</script>";
        exit;

    case 'apagar_relatorio':
        $ravid = $_POST['ravid'];
        $rel = new RelatorioAvaliadorRedeae();
        $rel->carregarPorId($ravid);
        if(!$rel->apagaRelatorio()){
            echo "Erro ao apagar o relatório.";
        } else {
            echo "Relatório apagado com sucesso.";
            echo "<script>location.reload();</script>";
        }
        exit;
}

$rel = new RelatorioAvaliadorRedeae();
$rel->arAtributos['usucpf'] = $usucpf;
$per = $rel->retornaPermissao();
$data = new Data();

include APPRAIZ . 'includes/cabecalho.inc';
$titulo = "Plano de Trabalho do Coordenador";
?>
<link rel="stylesheet" href="<?= APPRAIZ ?>www/sase/css/estilo.css"/>
<link rel="stylesheet" href="<?= APPRAIZ ?>www/includes/JsLibrary/date/displaycalendar/displayCalendar.css"/>
<link rel="stylesheet" href="<?= APPRAIZ ?>www/library/bootstrap-3.0.0/css/bootstrap.min-simec.css"/>

<script src="<?= APPRAIZ ?>www/includes/funcoes.js"></script>
<script src="<?= APPRAIZ ?>www/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>
    function submitForm(request){
        if (request !== 'undefined'){
            jQuery('[name=acao]').val(request);
            jQuery('[name=formCadastroLista]').submit();
        } else {
            alert('Ação não informada.');
        }
    }

    function editRelatorio(ravid){
        window.location.href = 'sase.php?modulo=principal/planodecarreira/relatorios/relPlanoTrabCoor&acao=A&ravid=' + ravid;
    }

    function apagarRelatorio(ravid){
        if(confirm("Deseja apagar este relatório?")) {
            jQuery.ajax({
                url: '',
                type: 'POST',
                data: {acao: 'apagar_relatorio', ravid: ravid},
                success: function (e) {
                    alert(e);
                }
            });
        }
    }

    function downloadRelatorio(ravid){
        window.location.href = 'sase.php?modulo=relatorio/relPlanoTrabCoor&acao=A&ravid=' + ravid;
    }
</script>

<div id='cabecalhoSistema'>
    <center><h3><?=$titulo?></h3></center>
</div>

<div id="container">
    <div class="row">
        <div class="col-md-12">
            <form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
                <input type="hidden" id="requisicao" name="requisicao" value="" />
                <input name="acao" value="carregar" type="hidden"/>
                <input name="ravid" value="" type="hidden"/>
                <input name="usucpf" value="<?=@$usucpf?>" type="hidden"/>

                <div class="col-md-offset-2 col-md-8">
                    <div class="well">
                        <fieldset>
                            <div class="form-group">
                                <label for="mes" class="col-md-4 control-label">Mês:</label>
                                <div class="col-md-8">
                                    <select name="mes" id="mes" class="form-control">
                                        <option value="1">Janeiro</option>
                                        <option value="2">Fevereiro</option>
                                        <option value="3">Março</option>
                                        <option value="4">Abril</option>
                                        <option value="5">Maio</option>
                                        <option value="6">Junho</option>
                                        <option value="7">Julho</option>
                                        <option value="8">Agosto</option>
                                        <option value="9">Setembro</option>
                                        <option value="10">Outubro</option>
                                        <option value="11">Novembro</option>
                                        <option value="12">Dezembro</option>
                                    </select>
                                </div>
                            </div>

                            <div class="form-group">
                                <label class="col-md-4 control-label">Período do relatorio:</label>
                                <div class="col-md-8">
                                    <table>
                                        <tr>
                                            <td><input id="ratdata1periodo" name="ratdata1periodo" value="<?= $_REQUEST['ratdata1periodo'] ?>" type="text" class="campoData form-control" placeholder=""></td>
                                            <td>à</td>
                                            <td><input id="ratdata2periodo" name="ratdata2periodo" value="<?= $_REQUEST['ratdata2periodo'] ?>" type="text" class="campoData form-control" placeholder=""></td>
                                        </tr>
                                    </table>
                                </div>
                            </div>

                            <div class="text-right">
                                <button title="Salvar" class="btn btn-primary" type="button" id="btnPesquisar" onclick="submitForm('novo')">
                                    Novo
                                </button>
                                <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="submitForm('pesquisar')">Pesquisar
                                </button>
<!--                                <button title="Salvar" class="btn btn-warning" type="button" id="btnPesquisar" onclick="submitForm('exportar-excel')">Exportar XLS-->
<!--                                </button>-->
                            </div>
                        </fieldset>
                    </div>
                </div>
            </form>
            <div>
                <?php $rel->recuperarListagem('rel', 2, 2); ?>
            </div>
        </div>

    </div>
</div>
<script>
    $(document).ready(function(){
        $('.campoCpf').mask("999.999.999-99");
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();

        $('.obrigatorio').append('<span style="color: red;">*</span>');

    });
</script>