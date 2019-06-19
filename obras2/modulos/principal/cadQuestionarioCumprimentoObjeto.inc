<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$pflcodsAcesso = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE);
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

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

if($_POST['requisicao'] && $_POST['requisicao'] == 'fotos') {
    $fotosQuestionarioCumprimentoObjeto = new FotosQuestionarioCumprimentoObjeto();
    $fotosQuestionarioCumprimentoObjeto->salvarDados($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); </script>";
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A',$parametros,array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A',$parametros,array());
}

$habilPag = false;

$habilitado = false;
if (possui_perfil($pflcods)) {
    $habilitado = true;
}
if (possui_perfil($pflcodsAcesso)) {
    $habilitado = true;
}

$cumprimentoObjeto = new CumprimentoObjeto();
$estadoWorkflowObra = wf_pegarEstadoAtual($obra->docid);
$coid = $cumprimentoObjeto->verificaExistencia($obrid);
$wfEstadoAtualCumprimento = wf_pegarEstadoAtual($cumprimentoObjeto->docid);


if($estadoWorkflowObra) {
    if($estadoWorkflowObra['esdid'] == ESDID_OBJ_CONCLUIDO || $estadoWorkflowObra['esdid'] == ESDID_OBJ_INACABADA || $estadoWorkflowObra['esdid'] == ESDID_OBJ_CANCELADO) {
        $habilPag = true;

        
        if (!$cumprimentoObjeto->obrasPermitidas($obra->obrid)) {
            echo "<script>alert('No momento, a aba de Cumprimento do Objeto não encontra-se liberado para esta obra.'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A'</script>";
            die;
        }

        $cumprimentoObjetoQuestionario = new CumprimentoObjetoQuestionario();
        $cumprimentoObjetoQuestionario->verificaExistencia($coid,true);

        if($cumprimentoObjetoQuestionario->qrpid) {
            $itpid = $cumprimentoObjetoQuestionario->verificaRespostaItem5();
            $fotosQuestionarioCumprimentoObjeto = new FotosQuestionarioCumprimentoObjeto();
            $fotosQuestionarioCumprimentoObjeto->coqid = $cumprimentoObjetoQuestionario->coqid;
            $arrDadosFotos = $fotosQuestionarioCumprimentoObjeto->importaDados();
        }

        #Pendencias de preenchimento
        $pendencias = $cumprimentoObjetoQuestionario->verificaPendencias();
        $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
        $pendenciasDocumentacao = $cumprimentoObjetoDocumentacao->validaRespostasDocumentacao($obrid);
        if(!$pendenciasDocumentacao) {
            if(!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
                echo "<script>alert('É necessário que não haja pendências na aba Documentação para acessar esta aba.'); window.location.href='obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
                die;
            }
        }
    }



}


print cabecalhoObra($obrid);
$cumprimentoObjeto->criaSubAba($url, $habilitado,$obrid,$wfEstadoAtualCumprimento['esdid']);
monta_titulo('Questionário', '');

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<style>
    fieldset {
        border: 1px solid #c0c0c0!important;
        margin: 0 2px!important;
        padding: 0.35em 0.625em 0.75em!important;
    }
    table p {
        color: #cc0102;
        font-weight: bold;
        font-size: 10px;
        text-align: justify;
    }
</style>
<script>
    $(document).ready(function(){
        $('#adicionar_anexo').click(function (e) {
            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_qco[]" id="arquivo"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao[]" id="arquivo_descricao" style="width:100%;"/></td></tr>').removeClass('anexos-base'));
            e.preventDefault();
        });
        $('.excluir_anexo').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
    });
</script>
<?php
if (!$habilPag):?>
    <div class="col-md-12">
        <center>
        <span style="background: #f00; color: #fff; padding:5px; text-align: center;">
            Esta aba será liberada após a conclusão da obra.
        </span>
        </center>
    </div>
<?php
else:

    if($wfEstadoAtualCumprimento['esdid'] == ESDID_CUMPRIMENTO_DILIGENCIADO) {
        $comentario = wf_pegarComentarioEstadoAtual($cumprimentoObjeto->docid);
        ?>
        <div class="col-md-12" style="">
            <div class="alert alert-danger text-center">
                <?=$comentario?>
            </div>
        </div>
    <?php
    }


    if($cumprimentoObjetoQuestionario->coqid):
        if($pendencias['respostas_zeradas'] == 'S' && $pendencias['resposta_fotos'] == 'N'  && $pendencias['codigo_inep'] !== false):
            $saidaGeral = '<div class="col-md-12"><section class="alert alert-success text-center">O Cumprimento do Objeto esta completamente preenchido. <br /><a style="color: #468847" href="#" alt="Enviar para Validação FNDE" title="Enviar para Validação FNDE" onclick="$(\'#td_acao_2913 a\').click();"><h3>Clique aqui para enviar para Validação do FNDE</h3></a></section></div>';
            $saidaPendencia = '<img src="/imagens/valida1.gif">';
        elseif($pendencias['respostas_zeradas'] == 'N' && $pendencias['codigo_inep'] === false) :
            $saidaPendencia = '<p><img title="" src="/imagens/valida5.gif"> Existem uma ou mais perguntas não respondidas</p>';
        elseif($pendencias['resposta_fotos'] == 'S'):
            if($fotosQuestionarioCumprimentoObjeto->verificaFotos()):
                $saidaGeral = '<div class="col-md-12"><section class="alert alert-success text-center">O Cumprimento do Objeto esta completamente preenchido. <br /><a style="color: #468847" href="#" alt="Enviar para Validação FNDE" title="Enviar para Validação FNDE" onclick="$(\'#td_acao_2913 a\').click();"><h3>Clique aqui para enviar para Validação do FNDE</h3></a></section></div>';
                $saidaPendencia = '<img title="" src="/imagens/valida1.gif">';
            else:
                $saidaPendencia = '<p><img title="" src="/imagens/valida5.gif"> Inserir fotos externas e internas da edificação e de todos seus ambientes no formulário \'Fotos da Conclusão da obra\'.</p>';
            endif;
        endif;

        if($pendencias['codigo_inep'] === false){
            $saidaPendencia = '<p><img title="" src="/imagens/valida5.gif"> O código do INEP inserido no questionário não é válido.</p>';
        }

        if($cumprimentoObjeto->docid) {
            if($wfEstadoAtualCumprimento['esdid'] == ESDID_CUMPRIMENTO_CADASTRAMENTO) {
                echo $saidaGeral;
            }
        }
    endif; ?>
    <div class="row" style="position:static;">
        <div class="col-md-10" style="position:inherit;">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tbody>
                    <tr>
                        <td class="subTituloEsquerda" style="width:15%;">Ação</td>
                        <td><button type="button" class="abrir_questionario" obrid="<?=$obrid?>" coid="<?=$coid?>" qrpid="<?=$cumprimentoObjetoQuestionario->qrpid?>">Abrir Questionário</button></td>
                    </tr>
                    <tr>
                        <td class="subTituloEsquerda">Pendências</td>
                        <td><?=$saidaPendencia;?></td>
                    </tr>
                </tbody>
            </table>

        <?php
        if($itpid == 5577):  ?>
            <form method="post" action="" id="form_foto" enctype="multipart/form-data">
                <input type="hidden" name="requisicao" id="requisicao" value="fotos">
                <input type="hidden" name="coqid" id="coqid" value="<?=$cumprimentoObjetoQuestionario->coqid?>">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Fotos da Conclusão da Obra</label></td></tr>
                    <tr bgcolor="">
                        <td class="subtitulocentro" colspan="2">Fotos externas e internas da edificação e de todos seus ambientes</td>
                    </tr>
                    <tr>
                        <td>
                            <div>
                                <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                    <tr>
                                        <td style="width:5%;">Ação</td>
                                        <td style="width:97px;">Pré-visualizaçao</td>
                                        <td style="">Nome</td>
                                        <td style="width:60%;">Descrição</td>
                                    </tr>
                                    <?
                                    if (!empty($arrDadosFotos)) {
                                        foreach ($arrDadosFotos as $arquivo) {
                                            $filename = APPRAIZ . "arquivos/obras2/" . floor($arquivo['arqid'] / 1000) . "/" . $arquivo['arqid'];
                                            if (file_exists($filename)) {
                                                ?>
                                                <tr class="anexos anexos-base">
                                                    <td class="SubTituloEsquerda" style="">
                                                        <span style=""><img src="/imagens/excluir.gif" alt=""/> <a
                                                                    class="excluir_anexo" href="">Excluir</a></span>
                                                    </td>
                                                    <td class="SubTituloEsquerda">
                                                        <?php


                                                        list($width, $height) = getimagesize($filename);
                                                        // echo $width.' x '.$height;
                                                        $new_width = 120;
                                                        #setando a altura da miniatura
                                                        $new_height = 100;
                                                        $image_p = imagecreatetruecolor($new_width, $new_height);
                                                        $image = imagecreatefromjpeg($filename);
                                                        imagecopyresampled($image_p, $image, 0, 0, 0, 0, $new_width,
                                                            $new_height, $width, $height);
                                                        if (!file_exists(APPRAIZ . "arquivos/obras2/thumbs/")) {
                                                            mkdir(APPRAIZ . "arquivos/obras2/thumbs/");
                                                        }
                                                        if (!file_exists(APPRAIZ . "arquivos/obras2/thumbs/" . floor($arquivo['arqid'] / 1000) . "/")) {
                                                            mkdir(APPRAIZ . "arquivos/obras2/thumbs/" . floor($arquivo['arqid'] / 1000) . "/");
                                                        }
                                                        if (!file_exists(APPRAIZ . "arquivos/obras2/thumbs/" . floor($arquivo['arqid'] / 1000) . "/" . $arquivo['arqid'])) {
                                                            imagejpeg($image_p,
                                                                APPRAIZ . "arquivos/obras2/thumbs/" . floor($arquivo['arqid'] / 1000) . "/" . $arquivo['arqid'],
                                                                50);
                                                        }

                                                        $filename = '';
                                                        $image_p = '';
                                                        $image = '';

                                                        ?>
                                                        <img src='data:image/png;base64,<?= base64_encode(file_get_contents(APPRAIZ . "arquivos/obras2/thumbs/" . floor($arquivo['arqid'] / 1000) . "/" . $arquivo['arqid'])) ?>'
                                                             style="width:97px; height:97px;" alt=""/>
                                                    </td>
                                                    <td class="SubTituloEsquerda">
                                                        <input type="hidden" value="<?= $arquivo['arqid'] ?>"
                                                               name="arquivo_qco[]" id="arquivo"/>
                                                        <a target="_blank"
                                                           href="/obras2/obras2.php?modulo=principal/cadQuestionarioCumprimentoObjeto&acao=A&download=<?= $arquivo['arqid'] ?>">
                                                            <?= $arquivo['arqnome'] ?>.<?= $arquivo['arqextensao'] ?>
                                                        </a>
                                                    </td>
                                                    <td class="SubTituloEsquerda"><input style="width:100%;" type="text"
                                                                                         maxlength="255" size=""
                                                                                         value="<?= $arquivo['arqdescricao'] ?>"
                                                                                         name="arquivo_descricao[]"
                                                                                         id="arquivo_descricao"/></td>
                                                </tr>
                                                <?
                                            }
                                        }
                                    }
                                    ?>
                                    <tr class="anexos">
                                        <td class="SubTituloEsquerda" style=""><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td>
                                        <td class="SubTituloEsquerda"></td>
                                        <td class="SubTituloEsquerda"><input type="file" <?=!$habilitado ? 'disabled' : ''?> name="arquivo_qco[]" id="arquivo"/></td>
                                        <td class="SubTituloEsquerda"><input style="width:100%;" maxlength="255" <?=!$habilitado ? 'disabled' : ''?> type="text" name="arquivo_descricao[]" id="arquivo_descricao"/></td>
                                    </tr>
                                </table>
                            </div>
                        </td>
                    </tr>
                    <tr bgcolor="">
                        <td colspan="2" style="text-align: center">
                            <input type="button" id="adicionar_anexo" value="Adicionar mais uma foto">
                            <input type="button" id="salvar_fotos" value="Salvar Fotos" style="cursor:pointer;" <?=!$habilitado ? 'disabled' : ''?>/>
                        </td>
                    </tr>
                </table>
            </form>
        <?php
        endif; ?>
        </div>
        <div class="col-md-1 pull-right" style="position:inherit;">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td>
                    <?php
                    wf_desenhaBarraNavegacao($cumprimentoObjeto->docid, array('coid' => $cumprimentoObjeto->coid, 'docid' => $cumprimentoObjeto->docid, 'obrid' => $cumprimentoObjeto->obrid)); ?>
                    </td>
                </tr>
            </table>
        </div>
    </div>
<?php
endif;?>
<script>
    $(document).ready(function(){
        $('#div_dialog_workflow').next().css('width','100%');
        $('#salvar_fotos').on('click',function(){
            if(confirm('As fotos selecionadas contemplam as áreas externas e internas da edificação e de todos seus ambientes?')){
                $('#form_foto').submit();
            }
        });
        var popup;
        $('.abrir_questionario').on('click',function(){
            var url = "obras2.php?modulo=principal/popupQuestionarioCumprimento&acao=A" +
                "&obrid=" + $(this).attr('obrid') +
                "&coid=" + $(this).attr('coid') +
                "&qrpid="+ $(this).attr('qrpid');
            popup = window.open(
                url,
                "Solicitação",
                "width=1200,height=600,scrollbars=yes,scrolling=no,resizebled=no"
            );
        });
        var timer = setInterval(function() {
            if(popup == undefined) {
                return;
            }
            if(popup.closed) {
                clearInterval(timer);
                window.location.href = 'obras2.php?modulo=principal/cadQuestionarioCumprimentoObjeto&acao=A'
            }
        }, 1000);
    });
</script>