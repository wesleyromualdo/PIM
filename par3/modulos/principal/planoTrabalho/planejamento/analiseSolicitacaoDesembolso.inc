<?php

$controllerAnaliseDesembolso = new Par3_Controller_AnaliseSolicitacaoDesembolso();

switch ($_POST['requisicao']) {
    case 'modalAnaliseDesembolso':
//        ob_clean();
        $controllerAnaliseDesembolso->modalAnaliseDesembolso($_POST['sdpid']);
        exit;
    case 'salvarAnaliseDesembolso':
        ob_clean();
        parse_str($_POST['dados'], $dados);
        $controllerAnaliseDesembolso->salvarAnaliseDesembolso($dados);
        exit;
    case 'redirecionamentoObraPar3':
        global $db;

        $sql = "SELECT emp.orgid, o.empid
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento emp ON emp.empid = o.empid
                WHERE o.obrid = {$_POST['obrid']}";
        $dadosObra = $db->pegaLinha($sql);

        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $_SESSION['obras2']['orgid'] = $dadosObra['orgid'];
        $_SESSION['obras2']['empid'] = $dadosObra['empid'];

        simec_redirecionar('/obras2/obras2.php?modulo=principal/desembolso&acao=A&obrid='.$_POST['obrid']);
}
switch ($_GET['requisicao']) {
    case 'downloadDocumento':
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec();
        try {
            $file->getDownloadArquivo($_GET['arqid']);
        } catch (Exception $e) {
            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/planejamento/analiseSolicitacaoDesembolso&acao=A');
        }
        exit;
        break;
}

require APPRAIZ.'includes/cabecalho.inc';
$url = 'par3.php?modulo=principal/planoTrabalho/planejamento/analiseSolicitacaoDesembolso&acao=A';
echo $simec->tab(criaAbaPar(), $url);

$modelIniciativa = new Par3_Model_IniciativaPlanejamento();
$arrEstado = $modelIniciativa->getEstadoMunicipioPorIniciativa();
$arrMunicipio = $modelIniciativa->getEstadoMunicipioPorIniciativa(true);

// Salvar requisição para que a lista continue a mesma depois de tramitar workflow ou salvar análise das solicitações
/*if ($_REQUEST['requisicao_form'] == 'limpar') { //resetar form
    $keys = array_keys($_POST);
    $_POST = array_fill_keys($keys, '');
    unset($_SESSION['postanalisedocumentos']);
}

if ($_POST['requisicao_form'] == 'filtrar') {
    $_SESSION['postanalisedocumentos'] = $_POST;
}

if (isset($_SESSION['postanalisedocumentos'])) {
    $_POST = $_SESSION['postanalisedocumentos'];
}*/

?>

<style>
    .icon-flipped {
        transform: scaleX(1) !important;
        -moz-transform: scaleX(1) !important;
        -webkit-transform: scaleX(1) !important;
        -ms-transform: scaleX(1) !important;
    }
</style>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista de Solicitação de Desembolso</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form_solicitacoes" id="form_solicitacoes" class="form-horizontal" method="post">

                <input type="hidden" name="obrid"  id="obrid"   value=""/>

                <div class="row">
                    <div class="col-lg-6">
                        <input type="hidden" name="requisicao_form" id="requisicao_form" value="pesquisar"/>
                        <?php
                        echo $simec->input('pronumeroprocesso', 'Processo', $_POST['pronumeroprocesso'], array('onkeyup'=>"this.value=mascaraglobal('#####.######/####-##',this.value);", 'maxlength' => 21, 'autofocus'), array('input-size' => '4'));
                        echo $simec->input('dotnumero', 'Número do Termo', $_POST['dotnumero'], array('onkeyup'=>"this.value=mascaraglobal('###########',this.value);", 'maxlength' => 12), array('input-size' => '4'));
                        echo $simec->input('sdpid', 'Código da solicitação', $_POST['sdpid'], array('onkeyup'=>"this.value=mascaraglobal('###########',this.value);"), array('input-size' => '4'));
                        echo $simec->input('econumero', 'Número do Contrato', $_POST['econumero'], array('onkeyup'=>"this.value=mascaraglobal('###########',this.value);"), array('input-size' => '4'));
                        echo $simec->select('esdid_analise', 'Situação', $_POST['esdid_analise'], $controllerAnaliseDesembolso->sqlSituacaoAnalise(), array('maxlength' => '255'), ['input-size' => '6']);

                        $_POST['contratosemsolicitacao'] = $_POST['contratosemsolicitacao'] ?: 'N';
                        echo $simec->radio('contratosemsolicitacao', 'Contrato sem solicitação', $_POST['contratosemsolicitacao'], array('S' => 'Sim', 'N' => 'Não'));

                        ?>

                    </div>
                    <div class="col-lg-6">

                        <?php

                        if($_POST['itrid'] == '') $_POST['itrid'] = 0;

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
                                echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array()); ?>
                            </div>
                        </div>
                        <div class="form-group ">
                            <label for="dataini" class="col-sm-3 col-md-3 col-lg-3 control-label">Data da Solicitação: </label>
                            <div class="row">
                                <div class="col-lg-5" style="text-align: center !important;">
                                    <?php
                                    $sdpdtinicio = $_POST['sdpdtinicio'] ? formata_data_sql($_POST['sdpdtinicio']) : '';
                                    echo $simec->data('sdpdtinicio', 'De', $sdpdtinicio, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                                <div class="col-lg-5" style="text-align: left !important; margin-left: -75.5px">
                                    <?php
                                    $sdpdtfim = $_POST['sdpdtfim'] ? formata_data_sql($_POST['sdpdtfim']) : '';
                                    echo $simec->data('sdpdtfim', 'Até', $sdpdtfim, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="dataini" class="col-sm-3 col-md-3 col-lg-3 control-label">Data de inclusão do contrato: </label>
                            <div class="row">
                                <div class="col-lg-5" style="text-align: center !important;">
                                    <?php
                                    $ecodtinicio = $_POST['ecodtinicio'] ? formata_data_sql($_POST['ecodtinicio']) : '';
                                    echo $simec->data('ecodtinicio', 'De', $ecodtinicio, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                                <div class="col-lg-5" style="text-align: left !important; margin-left: -75.5px">
                                    <?php
                                    $ecodtfim = $_POST['ecodtfim'] ? formata_data_sql($_POST['ecodtfim']) : '';
                                    echo $simec->data('ecodtfim', 'Até', $ecodtfim, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="dataini" class="col-sm-3 col-md-3 col-lg-3 control-label">Vigência do termo: </label>
                            <div class="row">
                                <div class="col-lg-5" style="text-align: center !important;">
                                    <?php
                                    $dotdtinicio = $_POST['dotdatainiciovigencia'] ? formata_data_sql($_POST['dotdatainiciovigencia']) : '';
                                    echo $simec->data('dotdatainiciovigencia', 'De', $dotdtinicio, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                                <div class="col-lg-5" style="text-align: left !important; margin-left: -75.5px">
                                    <?php
                                    $dotdtfim = $_POST['dotdatafimvigencia'] ? formata_data_sql($_POST['dotdatafimvigencia']) : '';
                                    echo $simec->data('dotdatafimvigencia', 'Até', $dotdtfim, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                            </div>
                        </div>

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
                                if ($_POST['requisicao_form'] == 'pesquisar') {

                                    echo $controllerAnaliseDesembolso->listarSolicitacoesAnalise($_POST);

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

<div class="ibox float-e-margins animated modal" id="modal-form-large" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
<!--                <h3 id="modal_titulo_solicitacao"></h3>-->
                <h3 id="modal_titulo"> Analisar Solicitação de Desembolso </h3>
            </div>
            <div class="ibox-content" id="conteudo_modal_desembolso">
                <!-- Conteúdo -->
            </div>
            <div class="ibox-footer center" id="footerModalSolicitacao">
                <button type="submit"
                        data-dismiss="modal"
                        class="btn btn-default">
                    <i class="fa fa-times-circle-o"></i> Cancelar
                </button>
                <button type="button" id="btn_salvaranalise" onclick="salvarAnaliseDesembolso()"
                        class="btn btn-success">
                    <i class="fa fa-plus-square-o"></i> Salvar
                </button>
            </div>
        </div>
    </div>
</div>


<!--<div id="modal-form" class="modal fade" aria-hidden="true">
    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="modal-content" id="dv-form-large">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                        aria-hidden="true">&times;</span></button>
                <h1 id="modal_titulo"> Analisar Solicitação de Desembolso </h1>
            </div>
            <div class="" id="conteudo_modal_desembolso">

            </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="submit"
                            data-dismiss="modal"
                            class="btn btn-default">
                        <i class="fa fa-times-circle-o"></i> Cancelar
                    </button>
                    <button type="button" id="btn_salvaranalise" onclick="salvarAnaliseDesembolso()"
                            class="btn btn-success">
                        <i class="fa fa-plus-square-o"></i> Salvar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>-->

<script>
    jQuery(function ($) {
        "use strict";

        jQuery(document).keypress(function(e) {
            if(e.which == 13){
                jQuery('.btn-pesquisar').click();
            }
        });

        $(window).on('click','#btn-pesquisar #limpar',function(){
            $('.loading-dialog-par3').show();
        });
        $(window).load(function(){
            $('.loading-dialog-par3').hide();
        });

        if(jQuery('[name="estuf"]').val() != '') {
            carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
        }

        jQuery('select[name=estuf]').change(function(){
            carregarMunicipio(this.value);
        });

        $("#form_analise_desembolso_parecer" ).validate({
            rules: {
                asdparecer:{
                    required:true
                }
            },messages: {
                asdparecer:{
                    required:'Por favor, preencha o parecer!'
                }
            }
        });

    });

    //mostra/esconde ibox
    $("#conteudo_modal_desembolso").on('click', '#titulo-dados-solicitacao', function () {
        $('#div-dados-solicitacao').slideToggle();
        //chevron up/down
        $('#i-dados-solicitacao').toggleClass(function () {
            if ($('#i-dados-solicitacao').is(".fa-chevron-down")) {
                $('#i-dados-solicitacao').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-dados-solicitacao').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    $('#div-dados-solicitacao').slideToggle();

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').val('');
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });


    $("#btn-limpar").click(function () {
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento/analiseSolicitacaoDesembolso&acao=A&requisicao_form=limpar';
    });

    $("#btn-pesquisar").click(function () {
        $btn = jQuery(this).button('loading');
        jQuery('#form_solicitacoes').submit();
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
        window.location.href = window.location.href+'&requisicao=downloadDocumento&arqid=' + arqid;
    }

    function modalAnaliseDesembolso(sdpid){
        $('#modal-form-large').modal();

        $.post(window.location.href, {requisicao: "modalAnaliseDesembolso", sdpid: sdpid}, function (resp) {
            $('#conteudo_modal_desembolso').html(resp);
            if($('#readonly').val() == 'readonly'){
                $('#btn_salvaranalise').hide();
            }else{
                $('#btn_salvaranalise').show();
            }
        });

    }

    function salvarAnaliseDesembolso(){
        if(!$('#form_analise_desembolso_parecer').valid()){
            return;
        }

        var dados = $('#form_analise_desembolso_parecer').serialize();
        var valoraprovado = $('#asdvaloraprovado').val();

        dados += '&asdvaloraprovado=' + valoraprovado;

        $.post(window.location.href, {requisicao: "salvarAnaliseDesembolso", dados: dados}, function (resp) {

            if(resp == 'erro'){
                swal('Erro', 'Erro ao salvar parecer, tente novamente mais tarde', 'error');
            }else{
                swal('Sucesso', 'Parecer salvo com sucesso!', 'success');
                $('#conteudo_modal_desembolso').html(resp);
            }
        });

        //todo ajax
//        $('#form_analise_desembolso_parecer').submit();
    }

    function acompanhamentoDesembolsoObra( dotid, obrid ){
        $('#form_solicitacoes #obrid').val(obrid);
        $('#requisicao').val('redirecionamentoObraPar3');
        $('#form_solicitacoes').submit();
    }

    function calculaValorPorPorcentagem(pc, v_solicitacao){
        var valor = pc * v_solicitacao / 100;

        if(valor > v_solicitacao){
            valor = v_solicitacao;
            $('#asdpercaprovado').val(100.00);
        }

        $('#asdvaloraprovado').val(floatParaMoeda(valor));
    }

    function calculaPorcentagemPeloValor(valor, v_solicitacao){
        var valor = moedaParaFloat(valor) ;

        if(valor == '' || isNaN(valor)){
            $('#asdpercaprovado').val(0.00);
            return;
        }

        var pc = valor * 100 / v_solicitacao;
        if(pc > 100){
            $('#asdvaloraprovado').val(v_solicitacao);
            pc = 100.00;
        }
        $('#asdpercaprovado').val(pc.toFixed(2));
    }

    function moedaParaFloat(numero) {
        var num = numero;
        num = num.replace('.', '');
        num = num.replace('.', '');
        num = num.replace('.', '');
        num = num.replace('.', '');
        num = num.replace(',', '.');
        return parseFloat(num).toFixed(2);
    }

    function floatParaMoeda(num) {
        var num = parseFloat(num).toFixed(2);
        x = 0;
        if (num < 0) {
            num = Math.abs(num);
            x = 1;
        }
        if (isNaN(num))
            num = "0";
        cents = Math.floor((num * 100 + 0.5) % 100);

        num = Math.floor((num * 100 + 0.5) / 100).toString();

        if (cents < 10)
            cents = "0" + cents;
        for (var i = 0; i < Math.floor((num.length - (1 + i)) / 3); i++)
            num = num.substring(0, num.length - (4 * i + 3)) + '.'
                + num.substring(num.length - (4 * i + 3));
        ret = num + ',' + cents;
        if (x == 1)
            ret = ' - ' + ret;
        return ret;
    }

    function redirecionarPagamentos(id, proid, tipo){
        window.location.href = 'par3.php?modulo=principal/orcamento/solicitarPagamento&acao=A&proid=' + proid + '&tipo=' + tipo;
    }


</script>
