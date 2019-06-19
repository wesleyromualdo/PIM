<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


if ($_GET['download']) {
    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema = 'obras2';

    $file = new FilesSimec(null, null, $eschema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">
        window.close();
        </script>');
}

if ($_REQUEST['excluir'] == 'S') {
    $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
    $cumprimentoObjetoConclusao->excluir($_REQUEST['arqid'], $_REQUEST['tipo']);
    echo "<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadConclusaoCumprimentoObjeto&acao=A';</script>";
    exit;
}

if ($_POST['requisicao'] && $_POST['requisicao'] == 'salvar') {
    $alerta = new Alerta();
    $id = $alerta->salvarAlerta($_POST);
    $alertaArquivo = new AlertaArquivo();
    $alertaArquivo->salvarAlertaArquivo($_POST, $id);
    $alertaPerfil = new AlertaPerfil();
    $alertaPerfil->salvarAlertaPerfil($_POST, $id);
    echo "<script> alert('Registro salvo com sucesso!'); window.location.href = 'obras2.php?modulo=principal/cadAlerta&acao=A&id_alerta={$id}';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$habilitado = true;
$alerta = new Alerta($_REQUEST['id_alerta']);
$alertaArquivo = new AlertaArquivo();
$arrArquivos = $alertaArquivo->buscaArquivosPorAlerta($_REQUEST['id_alerta']);
$alertaPerfil = new AlertaPerfil();
$perfis = $alertaPerfil->buscaPerfisPorAlerta($_REQUEST['id_alerta']);

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="../includes/tinymce_4/js/tinymce/tinymce.min.js"></script>
<script>
    tinymce.init({selector: 'textarea[name=mensagem]'});
    $(document).ready(function () {
        $('#adicionar_anexo').click(function (e) {
            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_alerta[]" id="arquivo"/></td></tr>').removeClass('anexos-base'));
            e.preventDefault();
        });

        $('.excluir_anexo').live('click', function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
    });

    function excluir(arqid, tipo) {
        if (confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadAlerta&acao=A&excluir=S&arqid=' + arqid + '&tipo=' + tipo;
        }
    }
</script>
<?php
if ($_REQUEST['id_alerta']) : ?>
    <div class="row">
        <div class="col-md-12">
            <ul class="nav nav-tabs">
                <li class="active">
                    <a id="tab_alerta"
                       href="obras2.php?modulo=principal/cadAlerta&acao=A&id_alerta=<?= $_REQUEST['id_alerta'] ?>">Notificação</a>
                </li>
                <li>
                    <a id="tab_vinculacao"
                       href="obras2.php?modulo=principal/cadVinculacaoAlerta&acao=A&id_alerta=<?= $_REQUEST['id_alerta'] ?>">Vincular
                        Obras</a>
                </li>
                <li>
                    <a id="tab_historico"
                       href="obras2.php?modulo=principal/historicoAlerta&acao=A&id_alerta=<?= $_REQUEST['id_alerta'] ?>">Histórico</a>
                </li>
            </ul>
        </div>
    </div>
    <?php
endif;
monta_titulo('Notificação', '');
?>
<div class="row" style="position:static;">
    <form method="post" action="" id="form_alerta" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="salvar">
        <input type="hidden" name="aid" id="id" value="<?= $alerta->aid ?>">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubtituloDireita" style="width:20%;">Título</td>
                <td style="width:80%;">
                    <input type="text" style="width:100%;" value="<?= $alerta->titulo ?>"
                           name="titulo" <?= !$habilitado ? 'disabled' : '' ?> maxlength="255">
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Mensagem</td>
                <td>
                    <textarea name="mensagem" style="width:100%;"
                              rows="5" <?= !$habilitado ? 'disabled' : '' ?>><?= $alerta->mensagem ?></textarea>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Entrar em Contato com</td>
                <td>
                    <input type="text" style="width:100%;" value="<?= $alerta->contato ?>"
                           name="contato" <?= !$habilitado ? 'disabled' : '' ?> maxlength="255">
                </td>
            </tr>
            <?php
            if ($_SESSION['usucpf'] == '' || $_SESSION['usucpf'] == '') : ?>
                <tr>
                    <td class="SubtituloDireita">Condição</td>
                    <td>
                        <input type="text" style="width:100%;" value="<?= $alerta->condicao ?>"
                               name="condicao" <?= !$habilitado ? 'disabled' : '' ?> maxlength="255">
                    </td>
                </tr>
                <tr>
                    <td class="SubtituloDireita">Mensagem Condição</td>
                    <td>
                        <input type="text" style="width:100%;" value="<?= $alerta->msg_condicao ?>"
                               name="msg_condicao" <?= !$habilitado ? 'disabled' : '' ?> maxlength="255">
                    </td>
                </tr>
                <tr>
                    <td class="SubtituloDireita">Pos Ação</td>
                    <td>
                        <input type="text" style="width:100%;" value="<?= $alerta->pos_acao ?>"
                               name="pos_acao" <?= !$habilitado ? 'disabled' : '' ?> maxlength="255">
                    </td>
                </tr>
                <?php
            endif; ?>
            <tr>
                <td class="SubtituloDireita">Cabeçalho</td>
                <td>
                    <label><input
                                type="radio" <?= $alerta->cabecalho == 'I' ? 'checked' : '' ?> <?= !$alerta->cabecalho ? 'checked' : '' ?>
                                name="cabecalho" <?= !$habilitado ? 'disabled' : '' ?> value="I"> Inativo</label>
                    <label><input type="radio" <?= $alerta->cabecalho == 'A' ? 'checked' : '' ?>
                                  name="cabecalho" <?= !$habilitado ? 'disabled' : '' ?> value="A"> Ativo</label>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Registro de Atividade</td>
                <td>
                    <label><input
                                type="radio" <?= $alerta->registro_atividade == 'I' ? 'checked' : '' ?> <?= !$alerta->registro_atividade ? 'checked' : '' ?>
                                name="registro_atividade" <?= !$habilitado ? 'disabled' : '' ?> value="I">
                        Inativo</label>
                    <label><input type="radio" <?= $alerta->registro_atividade == 'A' ? 'checked' : '' ?>
                                  name="registro_atividade" <?= !$habilitado ? 'disabled' : '' ?> value="A">
                        Ativo</label>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Assinatura Gestor</td>
                <td>
                    <select name="agid" class="chosen-select" data-placeholder="Selecione" style="width:50%;">
                        <option value="NULL">Selecione uma Opção</option>
                        <?php
                        $sql = "SELECT agid AS codigo, nome AS descricao FROM obras2.assinatura_gestor ORDER BY nome";
                        global $db;
                        $arDados = $db->carregar($sql);
                        if ($arDados) {
                            foreach ($arDados as $key) : ?>
                                <option value="<?= $key['codigo'] ?>" <?= (isset($alerta->agid) && $alerta->agid == $key['codigo']) ? "selected='selected'" : '' ?>>
                                    <?= $key["descricao"] ?>
                                </option>
                                <?php
                            endforeach;
                        } ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Vincular Perfil(is)</td>
                <td>
                    <select name="pflcod[]" class="chosen-select" multiple data-placeholder="Selecione"
                            style="width:50%;">
                        <option value="NULL">Selecione uma Opção</option>
                        <?php
                        $sql = "SELECT pflcod AS codigo, pfldsc AS descricao FROM seguranca.perfil WHERE sisid = 147 ORDER BY 2";
                        global $db;
                        $arDados = $db->carregar($sql);
                        if ($arDados) {
                            foreach ($arDados as $key) {
                                if (!empty($perfis)) {
                                    ?>
                                    <option value="<?= $key['codigo'] ?>" <?= (isset($perfis) && in_array(
                                        $key['codigo'],
                                        $perfis
                                    )) ? "selected='selected'" : '' ?>>
                                        <?= $key["descricao"] ?>
                                    </option>
                                    <?php
                                } else { ?>
                                    <option value="<?= $key['codigo'] ?>">
                                        <?= $key["descricao"] ?>
                                    </option>
                                <?php }
                            }
                        }
                        ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Arquivos</td>
                <td>
                    <div>
                        <input type="button" id="adicionar_anexo" value="Adicionar mais um arquivo"><br>
                        <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela"
                               cellpadding="3" cellspacing="1">
                            <tr>
                                <td style="width:10%;">Ação</td>
                                <td style="">Nome</td>
                            </tr>
                            <?php
                            if (!empty($arrArquivos)) :
                                foreach ($arrArquivos as $arquivo) : ?>
                                    <tr class="anexos anexos-base">
                                        <td class="SubTituloEsquerda" style="">
                                            <span style=""><img src="/imagens/excluir.gif" alt=""/> <a
                                                        class="excluir_anexo" href="">Excluir</a></span>
                                        </td>
                                        <td class="SubTituloEsquerda">
                                            <input type="hidden" value="<?= $arquivo['arqid'] ?>"
                                                   name="arquivo_alerta[]" id="arquivo"/>
                                            <a target="_blank"
                                               href="/obras2/obras2.php?modulo=principal/cadAlerta&acao=A&download=<?= $arquivo['arqid'] ?>">
                                                <?= $arquivo['arqnome'] ?>
                                            </a>
                                        </td>
                                    </tr>
                                    <?php
                                endforeach;
                            endif;
                            ?>
                            <tr class="anexos">
                                <td class="SubTituloEsquerda" style=""><span><img src="/imagens/excluir.gif" alt=""/> <a
                                                class="excluir_anexo" href="">Excluir</a></span></td>
                                <td class="SubTituloEsquerda"><input type="file" <?= !$habilitado ? 'disabled' : '' ?>
                                                                     name="arquivo_alerta[]" id="arquivo"/></td>
                            </tr>
                        </table>
                    </div>
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: center">
                    <input type="button" value="Voltar" id="voltar" style="cursor:pointer;"/>
                    <input type="button" value="Salvar" id="salvar" style="cursor:pointer;" <?= !$habilitado ?>/>
                </td>
            </tr>
        </table>
    </form>
</div>
<script>
    $1_11(document).ready(function () {
        $1_11('select[name="pflcod[]"]').chosen();
        $1_11('select[name="agid"]').chosen({
            allow_single_deselect: true
        });
    });
    $(document).ready(function () {

        $('#voltar').on('click', function () {
            window.location.href = '?modulo=principal/listaAlerta&acao=A';
        });
        $('#salvar').on('click', function () {
            /*if($('[name=aprovacao]:checked').val() == undefined) {
                alert('Selecione o campo de rádio: Aprovado, Reprovado ou Aprovado Parcialmente.');
                return false;
            } else if ($('[name=aprovacao]:checked').val() == 'A') {
                var valor = replaceAll(replaceAll($('[name=valordevolucao]').val(),'.',''), ',', '.');
                valor = parseFloat(valor).toFixed(2);
                if(valor > 0) {
                    alert('Campo \'Valor a Devolver ao Erário Público\' nao pode ser maior que zero quando \'Aprovado 100% - Objetivo Cumprido\'.');
                    return false;
                }
            }

            if($('[name=valordevolucao]').val() != undefined) {
                if($('[name=valordevolucao]').val() == '') {
                    alert('Campo \'Valor a Devolver ao Erário Público\' é de preenchimento obrigatório.');
                    return false;
                }
            }*/

            $('#form_alerta').submit();
        });
    });
</script>
<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }
</style>
