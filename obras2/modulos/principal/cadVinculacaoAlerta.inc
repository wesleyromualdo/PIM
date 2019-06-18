<?php

/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if ($_POST['requisicao'] && $_POST['requisicao'] == 'salvar') {
    $alerta = new AlertaObra();
    $alerta->salvarAlertaObra($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); window.location.href = 'obras2.php?modulo=principal/cadVinculacaoAlerta&acao=A&id_alerta=".$_POST['aid']."';</script>";
    exit;
}

if (!empty($_POST['requisicao']) && 'todos' == $_POST['requisicao']) {

    $alObras = new AlertaObra();
    $metodo = ('marcar' == $_POST['fazer'])?'novo':'removeAlerta';
    foreach ($alObras->retornaListaDeObrasFiltradas($_POST, ['apenasid' => true]) as $obra) {
        $alObras->$metodo($obra['obrid'], $_REQUEST['id_alerta']);
    }
    $alObras->commit();

    header("Location: {$_SERVER['REQUEST_URI']}");
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$habilitado = true;
$alertaObra = new AlertaObra();
$obras = $alertaObra->buscaAlertaObras($_REQUEST['id_alerta']);
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="../includes/tinymce_4/js/tinymce/tinymce.min.js"></script>
<script>
    $(document).ready(function(){
        $('#adicionar_anexo').click(function (e) {
            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_alerta[]" id="arquivo"/></td></tr>').removeClass('anexos-base'));
            e.preventDefault();
        });

        $('.excluir_anexo').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
    });
    function excluir(arqid,tipo){
        if(confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadAlerta&acao=A&excluir=S&arqid='+arqid+'&tipo='+tipo;
        }
    }
</script>
<div class="row">
    <div class="col-md-12">
        <ul class="nav nav-tabs">
            <li>
                <a id="tab_alerta" href="obras2.php?modulo=principal/cadAlerta&acao=A&id_alerta=<?=$_REQUEST['id_alerta']?>">Notificação</a>
            </li>
            <li class="active">
                <a id="tab_vinculacao" href="obras2.php?modulo=principal/cadVinculacaoAlerta&acao=A&id_alerta=<?=$_REQUEST['id_alerta']?>">Vincular Obras</a>
            </li>
            <li>
                <a id="tab_historico" href="obras2.php?modulo=principal/historicoAlerta&acao=A&id_alerta=<?=$_REQUEST['id_alerta']?>">Histórico</a>
            </li>
        </ul>
    </div>
</div>
<?
monta_titulo( 'Vincular Obras', ''); ?>
<div class="row" style="position:static;">
    <form method="post" action="" id="form_notificacao" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="salvar">
        <input type="hidden" name="aid" id="aid" value="<?=$_REQUEST['id_alerta']?>">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:500px!important;" id="obra_lista">
            <tr>
                <td>
                    <input class="btn btn-info" type="button" onclick="abrir_lista();" id="btn_obras" value="Selecionar Obras">
                </td>
            </tr>
            <tr>
                <td class="SubtituloCentro" colspan="2">Obras</td>
            </tr>
            <tr>
                <td class="SubtituloCentro">ID da Obra</td>
                <td class="SubtituloCentro">Obra</td>
            </tr>
            <?php
            foreach($obras as $obra): ?>
                <tr id="obrid_<?=$obra['obrid']?>">
                    <td>
                        <?=$obra['obrid']?>
                        <input type="hidden" name="obrid_in[]" value="<?=$obra['obrid']?>">
                    </td>
                    <td>
                        <?=$obra['obrnome']?>
                    </td>
                </tr>
            <?
            endforeach; ?>
            <tfoot>
                <tr>
                    <td colspan="2" style="text-align:right"><strong>TOTAL: <?=count($obras)?> registros</strong></td>
                </tr>
            </tfoot>
        </table>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td colspan="2" style="text-align: center">
                    <input type="button" value="Voltar" id="voltar" style="cursor:pointer;" />
                    <input type="button" value="Salvar" id="salvar" style="cursor:pointer;" <?=!$habilitado ?>/>
                </td>
            </tr>
        </table>
    </form>
</div>
<script>
    $1_11(document).ready(function () {
        $1_11('select[name="id_assinatura_gestor"]').chosen({
            allow_single_deselect: true
        });
    });

    function abrir_lista() {
        $('#modal-obras #modal-body').html('');
        jQuery("#modal_obras").dialog({
            modal: true,
            width: 1024,
            dialogClass: 'info',
        });

        var url ='obras2.php?modulo=principal/modalObras&acao=A';
        $.post(url,function(html) {
            $('#modal_obras #modal_body').html(html);
        });
    }

    $(document).ready(function(){
        $('#voltar').on('click',function(){
            window.location.href = '?modulo=principal/listaAlerta&acao=A';
        });
        $('#salvar').on('click',function(){
            $('#form_notificacao').submit();
        });
    });
</script>
<!-- Início Modal OBRAS -->
<div id="modal_obras" title="Modal Obras" class="alerta_obras">
    <div id="modal_body">

    </div>
</div>
<!-- FIM Modal OBRAS -->