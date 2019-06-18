<?php

$obrid = (int)$_GET['obrid'];
$inuid = (int)$_GET['inuid'];
$inpid = (int)$_GET['inpid'];
$oftid = (int)$_GET['oftid'];

if (empty($inuid)) {
    echo 'Não possui inuid! ';
    exit;
}

$controllerObra                 = new Par3_Controller_Obra($inuid, $obrid);
$controllerObrasFotosTerreno    = new Par3_Controller_ObraFotosTerreno($_GET['oftid']);
$qtdFotos                       = $controllerObrasFotosTerreno->retornaQtdFotos($_REQUEST);

$mdObra             = new Par3_Model_Obra($_GET['obrid']);

$mdConfiguracaoAba  = new Par3_Model_ObraConfiguracaoAba();
$arrConf            = $mdConfiguracaoAba
                        ->recuperarTodos
                        (
                        '*',
                            array("otpid = ".$mdObra->otpid, "ocastatus = 'A'", "oabid = 4"),
                        'oabid'
                        );
$conf               = $arrConf[0];
 
switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerObrasFotosTerreno->salvar($_REQUEST);
        die;
        break;
    case 'remover':
        $controllerObrasFotosTerreno->remover($_REQUEST);
        break;
    case 'pesquisar':
        break;
    case 'download':
        ob_clean();
        $arqid = $_GET['arqid'];
        if ($arqid) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec(null, null, "par3");
            $file->getDownloadArquivo($arqid);
        }
        die();
        break;
    default:
        break;
}

$disabled = 'disabled';

$disabledQtd = '';
if ($qtdFotos >= $conf['ocaqtd_maximo']) {
    $disabledQtd = 'disabled';
}

require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';



$mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
$pendenciaDadosBasicos     = $mpergunta
                                ->habilitarCamposEdicaoPorAbaOrdem(
                                        Par3_Model_ObraAba::FOTOS_TERRENO, 1, $_GET['obrid']);
//*******************************************
?>

<form method="post" name="form-foto-terreno" id="form-foto-terreno" class="form form-horizontal"
      enctype="multipart/form-data">

    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-12">
                    <h2 class="center"> Foto do Terreno </h2>
                    <h3 class="center">
                        <?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$_GET['obrid']}") ?>
                    </h3>
                    <h3 class="center">
                        <?=
                            (empty($oftid) ?
                                'Cadastrar' :
                                "Pré-Obra ID: {$controllerObra->model->obrid} / ID Foto: {$oftid}")
                        ?>
                    </h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">

            <input name="req" id="req" value="salvar" type="hidden">
            <input name="obrid" id="obrid" value="<?= $obrid ?>" type="hidden">
            <input name="inpid" id="inpid" value="<?= $inpid ?>" type="hidden">
            <input name="oftid" id="oftid" value="<?= $oftid ?>" type="hidden">


            <?php
            if ($oftid) {
                echo $simec->input(
                        'oftdsc',
                        'Descrição',
                        $controllerObrasFotosTerreno->model->oftdsc,
                        array('maxlength' => '255')
                );
                echo $simec->
                select(
                        'otfid',
                        'Tipo Foto',
                        $controllerObrasFotosTerreno->model->otfid,
                        $controllerObrasFotosTerreno->getComboTipoFoto($obrid),
                        array('class' => 'inteiro')
                );
                echo $simec->input(
                        'arqid',
                        'Anexo',
                        null,
                        array('type' => 'file', 'style' => 'height: auto;' )
                );
            } else {
                echo $simec->input(
                        'oftdsc',
                        'Descrição',
                        $controllerObrasFotosTerreno->model->oftdsc,
                        array('required' => 'required', 'maxlength' => '255', $disabledQtd)
                );
                echo $simec->select(
                        'otfid',
                        'Tipo Foto',
                        $controllerObrasFotosTerreno->model->otfid,
                        $controllerObrasFotosTerreno->getComboTipoFoto($obrid),
                        array('required' => 'required', 'class' => 'inteiro', $disabledQtd)
                );
                echo $simec->input(
                        'arqid',
                        'Anexo',
                        null,
                        array(
                                'type' => 'file',
                            'style' => 'height: auto;',
                            $requidoArquivo,
                            $disabledQtd,
                            $pendenciaDadosBasicos)
                );
            }

            ?>

        </div>

        <div class="ibox-footer">
            <div class="row col-md-offset-3">
            <?php if ($disabledQtd != '') {?>
                <button
                        type="button"
                        class="btn btn-danger"
                        title="Número máximo de fotos atingido."
                        onclick="swal('Erro','Número máximo de fotos atingido.', 'error')">
                    <i class="fa fa-save"></i> Enviar Foto</button>
            <?php } else {?>
                <?php if (!$pendenciaDadosBasicos) :?>
                <button type="submit" class="btn btn-success"><i class="fa fa-save"></i> Enviar Foto</button>
                <?php endif;?>
            <?php }?>
                |
                <button type="submit" class="btn btn-info pesquisar"><i class="fa fa-search"></i> Pesquisar Foto</button>
            </div>
        </div>

        <div class="ibox-content">
            <?php
            $mpergunta->carregarPergunta(Par3_Model_ObraAba::FOTOS_TERRENO, 1, $_GET['obrid']);
            ?>
            <div id="Listagem">
                <?php
                if ($_POST['req'] == 'pesquisar') {
                    echo $controllerObrasFotosTerreno->listaImagens($_POST);
                } elseif (empty($oftid)) {
                    $controllerObrasFotosTerreno->listaImagens($_GET);
                }
                ?>
            </div>
        </div>
    </div>
</form>

<!--modal-->
<div class="ibox float-e-margins animated modal" role="dialog" aria-hidden="true" id="modal-img">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo" class="center"></h3>
                <div id="div-img" align="center"></div>
            </div>
            <div class="ibox-footer" align="center">

            </div>
        </div>
    </div>
</div>

<script>
    $(function () {
        $('.abremodal').on('click',function() {
            // $('#div-img').html('');
            $('#modal-img').find('.ibox-footer').html('');
            $('.img-arqid').hide();
            var arqid = $(this).attr('data-arqid');
            $('#modal-img').modal();
            var descricao = $('#descricao-'+arqid).html();

            //se a imagem já tiver sido redenrizada não carrega de novo
            if($('#img-arqid'+arqid).length) {
                $('#img-arqid'+arqid).show();
                $('#modal-img').find('.ibox-footer').html(descricao);
                $('#modal-img').modal();
                return;
            }

            $('.loading-dialog-par3').show();
            var img = $('<img>');
            img.attr('src','/slideshow/slideshow/verimagem.php?&_sisarquivo=par3&arqid='+arqid);
            img.css('height: auto');
            img.addClass('img-responsive img-thumbnail center img-arqid');
            img.attr('id','img-arqid'+arqid);
            $('#div-img').append(img);
            $('#modal-img').find('.ibox-footer').html(descricao);
            $('#modal-img').modal();
            $(img).load(function() {
                $('.loading-dialog-par3').hide();
            });
        });

       
       $('input').keypress(function (e) {
            var code = null;
            code = (e.keyCode ? e.keyCode : e.which);
            return (code == 13) ? false : true;
       });
   

        $('.abremodal').on('click', function () {
            var id = $(this).data('oftid');
            $("#modalFoto"+id).modal('show');
        });

        $('.editar-foto').on('click', function () {
            //event.preventDefault();
            var id = $(this).data('oftid');
            var caminho = window.location.href;
            var action = '&edit=editar&oftid=' + id;
            window.location = caminho + action + '<?php echo $helperUrlToReturn->renderComplementoUrl();?>'
        })

        $('.excluir-foto').on('click', function () {
            //event.preventDefault();
            var id = $(this).data('oftid');
            var caminho = window.location.href;
            var action = '&req=recuperar&indid=' + id +'<?php echo $helperUrlToReturn->renderComplementoUrl();?>';
            swal({
                title: "Remover Foto do Terreno",
                text: "Tem certeza que deseja remover esta foto?",
                html: true,
                type: "warning",
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {

                if (isConfirm) {
                    var caminho = window.location.href;
                    var action = '&req=remover&oftid=' + id ;
                    window.location = caminho + action + '<?php echo $helperUrlToReturn->renderComplementoUrl();?>'
                }
            })
        });

        $(".pesquisar").click(function (evt) {
            evt.preventDefault();
            $('#req').val('pesquisar');
            var caminho = window.location.href;
            var action = '&req=pesquisar&' + $("#form-foto-terreno").serialize();
            window.location = caminho + action + '<?php echo $helperUrlToReturn->renderComplementoUrl();?>'
        });

    });
</script>
