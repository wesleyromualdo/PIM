<?php
include_once 'programas/_funcoes_maismedicos.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . 'includes/cabecalho.inc';

$termoAceite = $db->pegaUm("SELECT tcrid FROM maismedicomec.termocompromissoresidencia WHERE prgid = {$_REQUEST['prgid']} AND muncod = '{$muncod}' AND tcraceite = 'A'");
if(!$termoAceite){
    $termo = $db->pegaLinha("SELECT tcrid, tcraceite FROM maismedicomec.termocompromissoresidencia WHERE prgid = {$_REQUEST['prgid']} AND muncod = '{$muncod}'");
    $termoAceite = ($termo['tcraceite'] == 'N') ? $termo['tcrid'] : $termoAceite;
}

if ($_REQUEST['requisicao']) {
    if ($_REQUEST['requisicao'] == 'downloadArquivoMantenedoras') {
        downloadArquivoMantenedoras($_REQUEST['arqid']); #DOWNLOAD ARQUIVOS
    } else if ($_REQUEST['requisicao'] == 'excluirArquivoMetas') {
        excluirArquivoMetas($_REQUEST['arqid']);
    } else if ($_REQUEST['requisicao'] == 'verificarQuantidadeArquivos') {
        verificarQuantidadeArquivos($_REQUEST['tpaid'], $_REQUEST['mntid']);
    } else if ($_REQUEST['requisicao'] == 'inserir') {
        inserirAnalise($_REQUEST);
    }
}
#pega o nome do municipio
$municipio = $controleUnidade->pegarMunicipio($_REQUEST['inuid']);

if ($_REQUEST['requisicao'] == 'excluir') {
    header('content-type: text/html; charset=utf-8');
    excluirDoc($_REQUEST['arquivo']);
    exibirListaDoc();
    exit();
}

if ($_REQUEST['requisicao'] == 'cadastrarAnalise') {
    $retornoInsert = salvarArquivosAdesao($_POST, $_FILES);

    if ($retornoInsert == 'S') {
        $alerta = "Dados Salvos com sucesso.";
    } else {
        $alerta = "Ocorreu um erro na hora de salvar o arquivo.";
    }
}

if ($_REQUEST['requisicao'] == 'excluiAnalise') {
    $retornoExclusao = deletarArquivosAdesao($_POST);

    if ($retornoExclusao == 'S') {
        $alerta = "Dados excluidos com sucesso.";
    } else {
        $alerta = "Ocorreu um erro na hora de excluir o arquivo.";
    }
}

if ($_GET['download'] == 'S') {
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo "<script>window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A';</script>";
    exit;
}

if ($_SESSION['par']['rqmid']) {
    $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);
}
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
?>

<style type="text/css">
    .btn-file {
        position: relative;
        overflow: hidden;
    }

    .btn-file input[type=file] {
        position: absolute;
        top: 0;
        right: 0;
        min-width: 100%;
        min-height: 100%;
        font-size: 100px;
        text-align: right;
        filter: alpha(opacity=0);
        opacity: 0;
        background: red;
        cursor: inherit;
        display: block;
    }

    input[readonly] {
        background-color: white !important;
        cursor: text !important;
    }

    fieldset {
        border: 0px !important;
    }

    .note-toolbar {
        border-top: 1px solid !important;
        border-top-color: #696969 !important;

        border-right: 1px solid !important;
        border-right-color: #696969 !important;

        border-left: 1px solid !important;
        border-left-color: #696969 !important;
    }

    .ibox-title {
        background-color: #F8F8FF;
    }

    .ibox-content {
        background-color: #F8F8FF;
    }
</style>

<script type="text/javascript" language="javascript">
    $(document).ready(function () {
        <?php if ($alerta != ''){ ?>
        alert('<?php echo $alerta ?>');
        <?php  } ?>

        $('.btn-file :file').on('fileselect', function (event, numFiles, label) {
            var input = $(this).parents('.input-group').find(':text'),
                log = numFiles > 1 ? numFiles + ' files selected' : label;

            if (input.length) {
                input.val(log);
            } else {
                if (log) alert(log);
            }
        });
    });

    function apagaDocumento(id) {
        var txt;
        var r = confirm("Tem Certeza que deseja apagar o arquivo?");
        if (r == true) {
            $('[name=documento]').val(id);
            document.getElementById("formularioExcluirAnalise").submit();
        } else {
            alert('o registro não foi apagado');
        }
    }

    $(document).on('change', '.btn-file :file', function () {
        var input = $(this),
            numFiles = input.get(0).files ? input.get(0).files.length : 1,
            label = input.val().replace(/\\/g, '/').replace(/.*\//, '');
        input.trigger('fileselect', [numFiles, label]);
    });

    function validarDocumentoAnalise() {
        if ($('[name=arquivoAnalise]').val() == '') {
            alert('O campo "Arquivo" é obrigatório!');
            $('[name=arquivoAnalise]').focus();
            return false;
        }

        if ($('[name=arqdescricaoAnalise]').val() == '') {
            alert('O campo "Descrição" é obrigatório!');
            $('[name=arqdescricao]').focus();
            return false;
        }
        document.getElementById("formularioDocumentosAnalise").submit();
        return true;
    }
</script>

<form id="formularioExcluirAnalise" method="POST" name="formularioExcluirAnalise">
    <input type="hidden" id="documento" name="documento" value="">
    <input type="hidden" id="requisicao" name="requisicao" value="excluiAnalise"/>
</form>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3> Documentos de Adesão de <b><?php echo ucfirst($municipio); ?></b></h3>
            </div>
        </div>
        <form id="formularioDocumentosAnalise" method="POST" name="formularioDocumentosAnalise"
              enctype="multipart/form-data">
            <input type="hidden" name="inuid" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
            <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>

            <div class="ibox-content">
                <input type="hidden" id="tipoArquivo" name="tipoArquivo" value="29"/>
                <input type="hidden" id="tcrid" name="tcrid" value="<?php echo $termoAceite; ?>"/>
                <input type="hidden" id="requisicao" name="requisicao" value="cadastrarAnalise"/>
                <div class="form-group">
                    <div class="form-group">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">
                            Anexar Arquivo:
                            <span class="campo-obrigatorio" title="Campo obrigatório">*</span>
                        </label>
                        <div class="col-sm-9 col-md-9 col-lg-9">
                            <div class="input-group">
                                <span class="input-group-btn">
                                    <span class="btn btn-primary btn-file">
                                        Buscar Arquivo <input type="file" id="arquivoAnalise" name="arquivoAnalise"
                                                              style="display:">
                                    </span>
                                </span>
                                <input type="text" class="form-control" readonly>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="form-group">
                    <label for="exampleInputFile"></label>
                    <?php echo $simec->textarea('arqdescricaoAnalise', 'Descrição', '', array('required', 'rows' => '3', 'placeHolder' => 'Descrição', 'maxlengh' => 1000)); ?>
                </div>
                <div class="clearfix"></div>
            </div>

            <div class="clearfix"></div>

            <div class="row">
                <div class="col-lg-4 text-left">&nbsp;</div>
                <div class="col-lg-4 text-center">
                    <?php if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta()) : ?>
                        <?php $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabledForm); ?>
                        <button class="btn btn-success salvar"
                                onclick="validarDocumentoAnalise()"><i
                                class="fa fa-save"></i> Salvar Documento
                        </button>
                    <?php endif; ?>
                </div>
                <div class="col-lg-4 text-right">&nbsp;</div>
            </div>
        </form>

        <div class="clearfix"></div>

        <div id="div_documento"><?php exibirListaDoc('29', $termoAceite); ?></div>
    </div>
</div>

<br> <br>

<script type="text/javascript" language="javascript">
    <?php if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){  ?>
    jQuery("input,select,textarea").attr('disabled', 'disabled');
    <?php } ?>
</script>
