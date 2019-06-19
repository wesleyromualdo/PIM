<?php

$controllerTipoDocumentoAnexo = new Par3_Controller_TipoDocumentoAnexo();
$controllerPesquisa = new Par3_Controller_PesquisaDocumento();

$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);

require APPRAIZ . 'includes/cabecalho.inc';

$url = 'par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarDocumento&acao=A';
echo $simec->tab(criaAbaPar(), $url);

switch ($_REQUEST['requisicao']) {
    case 'downloaddocumento':
        $controllerPesquisa->downloadDocumento($_REQUEST['arqid']);
        exit;
        break;
    case 'modalAnaliseDocumento':
        ob_clean();
        $controllerPesquisa->modalAnaliseDocumento($_REQUEST);
        exit;
    case 'salvarAnalise':
        ob_clean();
        $controllerPesquisa->salvarAnalise($_REQUEST);
        exit;
    default:
        break;
}

if ($_REQUEST['requisicao_form'] == 'limpar') { //resetar form
    $keys = array_keys($_POST);
    $_POST = array_fill_keys($keys, '');
    unset($_SESSION['postanalisedocumentos']);
}


if ($_POST['requisicao_form'] == 'filtrar') {
    $_SESSION['postanalisedocumentos'] = $_POST;
}

if (isset($_SESSION['postanalisedocumentos'])) {
    $_POST = $_SESSION['postanalisedocumentos'];
}

?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Pesquisa de Termos e Documentos para Análise</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form_documentos" id="form_documentos" class="form-horizontal" method="post">
                <div class="row">
                    <div class="col-lg-6">
                        <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
                        <input type="hidden" name="requisicao_form" id="requisicao_form" value="filtrar"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $inp->inpid; ?>"/>

                        <?php
                            $filtro = simec_htmlentities($_POST['numeroprocesso']);
                            $numeroprocesso = $filtro;

                            $config = array();
                            $attribs = array('maxlength' => '255',
                                'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                                'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);");

                            echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, array('input-size' => '4'));
                            echo $simec->input('dotnumero', 'Número do Termo', $_POST['dotnumero'], array(), array('input-size' => '4'));
                            echo $simec->select('doaid', 'Tipo de Documento', $_POST['doaid'], $controllerTipoDocumentoAnexo->sqlComboDocumentosAnalise());
                            echo $simec->select('iniid', 'Iniciativa', $_POST['iniid'], $controllerPesquisa->sqlFormListaIniciativas());
                            echo $simec->select('intoid', 'Tipo de Objeto', $_POST['intoid'], $controllerPesquisa->sqlFormListaTipoObjeto());

                        ?>
                    </div>
                    <div class="col-lg-6">
                        <?php

                            if($_POST['itrid'] == '') $_POST['itrid'] = 0;
                            if($_POST['analisado'] == '') $_POST['analisado'] = 'T';

                            echo $simec->radio('itrid', 'Esfera', $_POST['itrid'], array(1 => 'Estadual', 2 => 'Municipal', 0 => 'Todos'));
                        ?>
                        <div class="row">
                            <div class="col-lg-5" style="margin-left:55.5px;">
                                <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array('input-size' => '6', 'label-size' => '5')); ?>
                            </div>
                            <div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                                if ($_REQUEST['estuf']) {
                                    $municipio = new Territorios_Model_Municipio();
                                    $arrMunicipio = simec_preparar_array($municipio->lista(array('muncod as codigo', 'mundescricao||\' - \'||estuf as descricao'), array("estuf = '{$_REQUEST['estuf']}'")));
                                }
                                echo $simec->select('muncod[]', 'Município', $_POST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array()); ?>
                            </div>
                        </div>

                        <?php
                            /*echo $simec->select('estuf', 'Estado', $_POST['estuf'], $arrEstado, array('maxlength' => '255'), array());
                            echo $simec->select('muncod', 'Município', $_POST['muncod'], $arrMunicipio, array('maxlength' => '255'), array());*/
                        echo $simec->radio('analisado', 'Documento analisado', $_POST['analisado'], array('S' => 'Sim', 'N' => 'Não', 'T' => 'Todos'), array(), array());
                        echo $simec->select('esdid_analise', 'Situação da Análise', $_POST['esdid_analise'], $controllerPesquisa->sqlSituacaoAnalise(), array('maxlength' => '255'), array());
                        ?>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-info btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button> |
                    <button type="button" class="btn btn-default-bright" id="btn-limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
            <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                    <div class="table-responsive" >
                        <div>
                            <div id="testeSQL">
                                <?php
//                                ver($_GET,  $_POST, $_REQUEST);
                                if ($_POST['requisicao_form'] == 'filtrar') {
                                    echo $controllerPesquisa->listarDocumentosAnalise($_POST);
                                }
                                ?>
                            </div>
                        </div>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
            <!-- END Listagem  -->

        </div>
    </div>

</div>

<div id="modal-form" class="modal fade" aria-hidden="true">
    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="modal-content" id="dv-form-large">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                        aria-hidden="true">&times;</span></button>
                <h1 id="modal_titulo"> Analisar Documento </h1>
            </div>
            <div class="" id="conteudo_modal_doc">
                <!-- Conteúdo -->
            </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="submit"
                            data-dismiss="modal"
                            class="btn btn-default">
                        <i class="fa fa-times-circle-o"></i> Cancelar
                    </button>
                    <button type="button" onclick="salvarAnalise()"
                            class="btn btn-success">
                        <i class="fa fa-plus-square-o"></i> Salvar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    jQuery(function ($) {
        "use strict";

        $("#form_analise_desembolso_parecer" ).validate({
            rules: {
                eadparecer:{
                    required:true
                }
            },messages: {
                eadparecer:{
                    required:'Por favor, preencha o parecer!'
                }
            }
        });

        if(jQuery('[name="estuf"]').val() != '') {
            carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
        }

        jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
            //renderizeMunicipio();
        });

        jQuery('select[name=estuf]').change(function(){
            carregarMunicipio(this.value);
            carregarEscolas();
        });

        jQuery('select[name=muncod]').change(function(){
            carregarEscolas(this.value);
        });

    });

    //mostra/esconde ibox
    $("#conteudo_modal_doc").on('click', '#btn-itens-composicao', function () {
        $('#div-itens-composicao').slideToggle();
        //chevron up/down
        $('#i-itens-composicao').toggleClass(function () {
            if ($('#i-itens-composicao').is(".fa-chevron-down")) {
                $('#i-itens-composicao').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-itens-composicao').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    $('#div-itens-composicao').slideToggle();


    $("#btn-limpar").click(function () {
        $('#requisicao_form').val('limpar');
        console.log($('#requisicao_form').val());
        $('#form_documentos').submit();
//        window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarDocumento&acao=A';
    });

    $("#btn-pesquisar").click(function () {
        $btn = jQuery(this).button('loading');
        jQuery('#form_documentos').submit();
    });

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').val('');
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });

    function carregarMunicipio(estuf, muncod) {
        if(estuf != '') {
            var options = jQuery('#muncod');
            options.empty();
            options.append(new Option('', ''));
            jQuery.post('', 'requisicao=carregaMunicipios&estuf='+estuf, function(retorno) {
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.mundescricao, this.muncod));
                });
                options.focus();
                options.val(muncod);
                options.trigger('chosen:updated');
            });
        }
    }

    function acaoDownload(id, arqid){
        window.location.href = window.location.href+'&requisicao=downloaddocumento&arqid=' + arqid;
    }

    function modalAnaliseDocumento(edlid){
        $('#modal-form-large').modal();
        $('#modal-form-large').trigger('focus');

        $.post(window.location.href, {requisicao: "modalAnaliseDocumento", edlid: edlid}, function (resp) {
            $('#conteudo_modal_doc').html(resp);
        });

    }

    function salvarAnalise(){
        if(!$('#form_analise_documentos_parecer').valid()){
            return;
        }

        var dados = $('#form_analise_documentos_parecer').serialize();
        $.post(window.location.href, {requisicao: "salvarAnalise", dados: dados}, function (resp) {
            if(resp == 'falha') {
                alert('Falha ao salvar análise');
            }else{
                $('#barranavegacaoworkflow').html(resp);
            }

        });
    }



</script>
