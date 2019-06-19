<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_GET['download']){
    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema = 'obras2';

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">
        window.close();
        </script>');
}

if($_REQUEST['excluir'] == 'S') {
    $assinaturaGestor = new AssinaturaGestor();
    $assinaturaGestor->excluir($_REQUEST['arqid']);
    echo"<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadAssinaturaGestor&acao=A&id='+{$_REQUEST['id']};</script>";
    exit;
}

if($_POST['requisicao'] && $_POST['requisicao'] == 'salvar') {
    $assinaturaGestor = new AssinaturaGestor();
    $assinaturaGestor->salvarAssinatura($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); window.location.href = 'obras2.php?modulo=principal/listaAssinaturaGestor&acao=A';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$habilitado = true;
$assinaturaGestor = new AssinaturaGestor();
$assinatura = $assinaturaGestor->buscaAssinatura($_REQUEST['id']);
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="../includes/tinymce_4/js/tinymce/tinymce.min.js"></script>

<!--<script src="//cdn.tinymce.com/4/tinymce.min.js"></script>
<script>tinymce.init({ selector:'textarea' });</script>-->

<script>
    tinymce.init({ selector:'textarea[name=descricao]' });
    $(document).ready(function(){
    });
    function excluir(arqid,tipo){
        if(confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadAlerta&acao=A&excluir=S&arqid='+arqid+'&tipo='+tipo;
        }
    }
</script>
<?php
monta_titulo( 'Assinatura de Gestor', '');
?>

<div class="row" style="position:static;">
    <form method="post" action="" id="form_assinatura" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="salvar">
        <input type="hidden" name="agid" id="id" value="<?=$assinatura['agid']?>">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubtituloDireita" style="width:20%;">Nome</td>
                <td style="width:80%;">
                    <input type="text" style="width:100%;" value="<?= $assinatura['nome']?>" name="nome" <?=!$habilitado ? 'disabled' : ''?> maxlength="255">
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita" style="width:20%;">Descrição</td>
                <td style="width:80%;">
                    <textarea style="width:100%;" name="descricao" rows="5" <?=!$habilitado ? 'disabled' : ''?>><?= $assinatura['descricao']?></textarea>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita" style="width:20%;">Assinatura</td>
                <td style="width:80%;">
                    <?
                    if($assinatura['arqid']): ?>
                        <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr class="anexos anexos-base">
                                <td class="SubTituloEsquerda" style="">
                                    <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="link" onclick="excluirArquivo('<?=$assinatura['arqid']?>');">Excluir</a></span>
                                </td>
                                <td class="SubTituloEsquerda">
                                    <img src='data:image/png;base64,<?=base64_encode(file_get_contents(APPRAIZ."arquivos/obras2/".floor($assinatura['arqid']/1000)."/".$assinatura['arqid']))?>' style="width:97px; height:97px;" alt=""/>
                                </td>
                                <td class="SubTituloEsquerda">
                                    <input type="hidden" value="<?=$assinatura['arqid']?>" name="arqid" id="arquivo"/>
                                    <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadAssinaturaGestor&acao=A&download=<?=$assinatura['arqid']?>">
                                        <?=$assinatura['arqnome']?>.<?=$assinatura['arqextensao']?>
                                    </a>
                                </td>
                            </tr>
                        </table>
                    <?
                    else: ?>
                    <input type="file" name="arqid">
                    <?
                    endif; ?>
                </td>
            </tr>
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
    function excluirArquivo(arqid){
        if(arqid) {
            if(confirm('Você deseja realmente excluir este arquivo?')) {
                window.location = '?modulo=principal/cadAssinaturaGestor&acao=A&excluir=S&arqid='+arqid;
            }
        }
    }

    $(document).ready(function(){
        $('#voltar').on('click',function(){
            window.location.href = '?modulo=principal/listaAssinaturaGestor&acao=A';
        });

        $('#salvar').on('click',function(){
            if($('[name=nome]').val() == '') {
                alert('Campo \'Nome\' é de preenchimento obrigatório.');
                return false;
            }
            if(tinyMCE.activeEditor.getContent() == '') {
                alert('Campo \'Descrição\' é de preenchimento obrigatório.');
                return false;
            }
            if($('[name=arqid]').val() == '') {
                alert('Campo \'Assinatura\' é de preenchimento obrigatório.');
                return false;
            }

            $('#form_assinatura').submit();
        });
    });
</script>