<?php
include_once 'programas/_funcoes_maismedicos.php';

#pega o nome do municipio
$municipio = $controleUnidade->pegarMunicipio($_REQUEST['inuid']);

$termoAceite = $db->pegaUm("SELECT tcrid FROM maismedicomec.termocompromissoresidencia WHERE prgid = {$_REQUEST['prgid']} AND muncod = '{$muncod}' AND tcraceite = 'A'");
if(!$termoAceite){
    $termo = $db->pegaLinha("SELECT tcrid, tcraceite FROM maismedicomec.termocompromissoresidencia WHERE prgid = {$_REQUEST['prgid']} AND muncod = '{$muncod}'");
    $termoAceite = ($termo['tcraceite'] == 'N') ? $termo['tcrid'] : $termoAceite;
}

$somenteLeitura = false;

if($_REQUEST['prgid']) {
    $modelProadesao = new Par3_Model_Proadesao();
    $proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($_REQUEST['prgid']);
    $dtFinal = (int)str_replace('-', '', $proadesaoDados['pfadatafinal']);
    $dtAtual = (int)str_replace('-', '', date('Y-m-d'));
    $somenteLeitura = ($dtFinal > 0 && ($dtAtual > $dtFinal)) ? true : false;
}


if ($_REQUEST['requisicao'] == 'excluir') {
    header('content-type: text/html; charset=utf-8');
    excluirDoc($_REQUEST['arquivo']);
    exibirListaDoc();
    exit();
}

if ($_REQUEST['requisicao'] == 'downloadArquivo') {
    downloadArquivo($_REQUEST['arqid']); #DOWNLOAD ARQUIVOS
}


if ($_REQUEST['requisicao'] == 'cadastrarDoc') {
    salvarArquivosAdesao($_POST, $_FILES);
}

if ($_REQUEST['requisicao'] == 'excluiDocumento') {
    deletarArquivosAdesao($_POST);
}

if ($_GET['download'] == 'S') {
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo "<script>window.location.href = 'par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=documentos';</script>";
    exit;
}

if ($_SESSION['par']['rqmid']) {
    $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);
}

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
?>

<script type="text/javascript" language="javascript">

    $(document).ready(function () {

        function verificaExtensaoArquivo(fname) {
            var re = /(\.pdf)$/i;
            if (!re.exec(fname)) {
                return false;
            }
            return true;
        }

        //CONTROLA A EXTENSÃO DOS ARQUIVOS E OS RESPECTIVOS NOMES QUE APARECEM NO CAMPO DE TEXTO
        $(':file').on('change', function () {
            var erro = false;
            var names = [];
            if ($('#arquivoDoc').val() != '') {
                for (var i = 0; i < $(this).get(0).files.length; ++i) {
                    nome = $(this).get(0).files[i].name.toLowerCase();

                    if (verificaExtensaoArquivo(nome)) {
                        names.push($(this).get(0).files[i].name);
                    } else {
                        erro = true;
                    }
                }
                $(this).parents('.input-group').find(':text').val(names);

                if (erro) {
                    $('[name=arquivoDoc]').val('');
                    alert("Apenas arquivos .pdf serão incluídos!");
                }
            } else {
                alert('Selecione o tipo do arquivo');
            }
        });


        <?php if ($_SESSION['maismedicomec']['msg'] != ''){ ?>
        alert('<?php echo $_SESSION['maismedicomec']['msg']; ?>');
        <?php unset($_SESSION['maismedicomec']['msg']); } ?>

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
            $("#requisicao").val('excluiDocumento');
            $("#formularioDocumentos").submit();
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


    function downloadDocumento(id, arqid) {
        var pathArray = location.search.substring(1).split('&');
        window.location.href = 'par3.php?modulo=principal/adesao/termo&acao=A&prgid=<?php echo $_REQUEST['prgid']; ?>&inuid=<?php echo $_REQUEST['inuid']; ?>&aba=documentos&requisicao=downloadArquivo&arqid=' + arqid;
    }

    function validarDocumentoDoc() {
        var erro = 0;

        if ($('[name=arquivoDoc]').val() == '') {
            alert('O campo "Arquivo" é obrigatório!');
            $('[name=arquivoDoc]').focus();
            erro = 1;
            return false;
        }
        if ($('[name=tpaid]').val() == '') {
            alert('O campo "Tipo de Arquivo" é obrigatório!');
            $('[name=tpaid]').focus();
            erro = 1;
            return false;
        }
        if ($('[name=arqdescricaoDoc]').val() == '') {
            alert('O campo "Descrição" é obrigatório!');
            $('[name=arqdescricao]').focus();
            erro = 1;
            return false;
        }

        if (erro == 0) {
            $("#requisicao").val('cadastrarDoc');
            $("#formularioDocumentos").submit();
        }
    }

</script>

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

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3> DOCUMENTOS DE ADESÃO DE - <b><?php echo strtoupper($municipio); ?></b></h3>
            </div>
        </div>

        <form id="formularioDocumentos" method="POST" name="formularioDocumentos"
              enctype="multipart/form-data">
            <input type="hidden" name="inuid" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
            <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
            <input type="hidden" name="tcrid" id="tcrid" value="<?php echo $termoAceite; ?>"/>
            <input type="hidden" id="aba" name="aba" value="documentos"/>
            <input type="hidden" id="documento" name="documento" value=""/>
            <input type="hidden" id="tipoArquivo" name="tipoArquivo" value="28"/>
            <input type="hidden" id="requisicao" name="requisicao" value=""/>

            <div class="ibox-content">

                <div class="form-group">
                    <div class="form-group">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">
                            Anexar Arquivo:
                            <span class="campo-obrigatorio" title="Campo obrigatório">*</span>
                        </label>
                        <div class="col-sm-9 col-md-9 col-lg-9">
                            <div class="input-group">
                                <span class="input-group-btn">
                                    <span class="btn btn-primary btn-file">Buscar Arquivo
                                        <input type="file" id="arquivoDoc" name="arquivoDoc"
                                               style="display:inline">
                                    </span>
                                </span>
                                <input type="text" class="form-control" readonly>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="clearfix">&emsp;</div>
                <div class="form-group">
                    <?php
                    $sqlDocumento = "
                            SELECT  tpaid AS codigo,  
                                    tpadsc AS descricao 
                            FROM par.tipoarquivo 
                            ORDER BY tpadsc
                        ";
                    echo $simec->select('tpaid', 'Tipo de Arquivo', '', $sqlDocumento, array('data-placeholder' => 'Selecione...', 'required'));
                    ?>
                </div>
                <div class="form-group">
                    <label for="exampleInputFile"></label>
                    <?php echo $simec->textarea('arqdescricaoDoc', 'Descrição', '', array('required', 'rows' => '3', 'placeHolder' => 'Descrição', 'maxlengh' => 1000)); ?>
                </div>
                <div class="clearfix"></div>
            </div>

            <div class="clearfix"></div>

            <div class="row">
                <div class="col-lg-4">&nbsp;</div>
                <div class="col-lg-4 text-center">
                    <?php
                    if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta() && !$somenteLeitura) {
                        $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabledForm);
                        ?>
                        <button class="btn btn-success salvar" onclick="validarDocumentoDoc()">
                            <i class="fa fa-save"></i> Salvar Documento
                        </button>
                        <?php
                    }
                    ?>
                </div>
                <div class="col-lg-4">&nbsp;</div>
            </div>
        </form>

        <div>&ensp;</div>
        <div id="div_documento"><?php exibirListaDoc('28', $termoAceite); ?></div>
        <div>&ensp;</div>
        <div>&ensp;</div>

    </div>
</div>
<script type="text/javascript" language="javascript">
    <?php if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) || $somenteLeitura ){  ?>
        jQuery("input,select,textarea").not('#chosen-sistema').attr('disabled', 'disabled');
        jQuery('a[title="Apagar"]').parent().remove();
        jQuery('input').removeAttr('readonly');
        setTimeout(function(){
            jQuery('.chosen-disabled)').children().css('background-color','#EEEEEE');
        },200);
    <?php } ?>
</script>
