<?php

$controllerSolicitacaoDesembolso = new Par3_Controller_SolicitacaoDesembolso();


switch($_POST['requisicao']) {
    case 'modalSolicitacaoDesembolso':
        ob_clean();
        $controllerSolicitacaoDesembolso->modalSolicitacaoDesembolso($_POST['ecoid'], $_POST['sdpid'], $_POST['edlid']);
        exit;
        break;
    case 'removerSolicitacaoDesembolso':
        ob_clean();
        $controllerSolicitacaoDesembolso->removerSolicitacaoDesembolso($_POST);
        exit;
        break;
    case 'salvarSolicitacaoDesembolso':
        ob_clean();
        $controllerSolicitacaoDesembolso->salvarSolicitacaoDesembolso($_POST);
        exit;
        break;
}

switch ($_GET['requisicao']){
    case 'downloadArquivoContrato':
        ob_clean();
        $file = new FilesSimec("execucao_contrato", null, "par3");
        $file->getDownloadArquivo($_GET['arqid']);
        break;
}

$inuid = $_GET['inuid'];
$url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=solicitacaoDesembolso';
global $db;
$db->cria_aba( 58014, $url , '&inuid=' . $inuid, array());

?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Lista de solicitações de desembolso</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form_solicitacoes" id="form_solicitacoes" class="form-horizontal" method="post">
                <div class="row">
                    <div class="col-lg-6">
                        <input type="hidden" name="inuid" value="<?php echo $_GET['inuid']; ?>"/>
                        <input type="hidden" name="requisicao" id="requisicao" value="pesquisar"/>
                        <?php
                        echo $simec->input('dotnumero', 'Número do Termo', $_POST['dotnumero'], array('onkeyup'=>"this.value=mascaraglobal('##########',this.value);", 'autofocus'),array('input-size' => '4'));
                        echo $simec->input('econumero', 'Número do Contrato', $_POST['econumero'], array('onkeyup'=>"this.value=mascaraglobal('###########',this.value);"),array('input-size' => '4'));
                        echo $simec->input('sdpcod', 'Código da solicitação', $_POST['sdpcod'], array('onkeyup'=>"this.value=mascaraglobal('###########',this.value);"),array('input-size' => '4'));
                        echo $simec->input('obrid', 'ID da obra', $_POST['obrid'], array('onkeyup'=>"this.value=mascaraglobal('###########',this.value);"),array('input-size' => '4'));
                        echo $simec->select('esdid_analise', 'Situação', $_POST['esdid_analise'], $controllerSolicitacaoDesembolso->sqlSituacaoAnalise(), array('maxlength' => '255'), array());
                        ?>
                    </div>
                    <div class="col-lg-6">

                        <?php
                        $arrOptions =  array('O' => 'Obras', 'P' => 'PAR','T' => 'Todos');
                        echo $simec->radio('tipo', 'Tipo',($_POST['tipo']?:'T'), $arrOptions, array(), array('input-size' => '4','label-size' => '3'));

                        $anosprocesso = "SELECT DISTINCT substring(pronumeroprocesso,12,4) as codigo,  substring(pronumeroprocesso,12,4) as descricao FROM par3.processo WHERE substring(pronumeroprocesso,12,4) <> '' AND prostatus = 'A' ";
                        echo $simec->select('anoprocesso', 'Ano', $_POST['anoprocesso'], $anosprocesso, array(),array('input-size' => '4'));

                        echo $simec->data('sdpdtinicio', 'De', $_POST['sdpdtinicio'], array(), array('input-size' => 4));
                        echo $simec->data('sdpdtfim', 'Até', $_POST['sdpdtfim'], array(), array('input-size' => 4));

                        ?>

                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-info btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
                    <button type="button" class="btn btn-default-bright" id="btn-limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
            <!-- Listagem  -->

            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">

                    <!-- Legenda -->
                    <div class="row" style="float: left;">
                        <div class="col-lg-12 pull-left">
                            <b>Legenda:</b>
                            <div class="fa fa-info-circle" style="color: #64a0e8;"></div> Passe o mouse para visualizar mais informações do pagamento
                        </div>
                    </div>

                    <div class="table-responsive" >
                        <div>
                            <div id="testeSQL">
                                <br style="clear:both">
                                <?php
                                if($_POST['requisicao'] == 'pesquisar'){
                                    echo $controllerSolicitacaoDesembolso->listarSolicitacoes($_POST);
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


<div class="ibox float-e-margins animated modal" id="modal_solicitacao" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo_solicitacao"></h3>
            </div>
            <div class="ibox-content" id="conteudo_modal_solicitacao">
                <!-- Conteúdo -->
            </div>
            <div class="ibox-footer center" id="footerModalSolicitacao">
                <button type="submit"
                        data-dismiss="modal"
                        id="btnFecharModalSolicitacao"
                        class="btn btn-default center">
                    <i class="fa fa-times-circle-o"></i> Fechar
                </button>
            </div>
        </div>
    </div>
</div>

<script>

    $('.dropup').hover(function(e){
        $('.dropup').removeClass('open'); //fecha qualquer outro drop up
        $(this).addClass('open')}
    );

    jQuery(document).keypress(function(e) {
        if(e.which == 13){
            jQuery('.btn-pesquisar').click();
        }
    });

    /*$(window).on('beforeunload',function(){
        if($('#requisicao').val() != 'excel'){
            $('.loading-dialog-par3').show();
        }
    });*/
    $(window).on('click','#btn-pesquisar #limpar',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).load(function(){
        $('.loading-dialog-par3').hide();
    });

    $("#btn-limpar").click(function () {
        var params = {};
        var parser = document.createElement('a');
        parser.href = window.location.href;
        var query = parser.search.substring(1);
        var vars = query.split('&');
        for (var i = 0; i < vars.length; i++) {
            var pair = vars[i].split('=');
            params[pair[0]] = decodeURIComponent(pair[1]);
        }

        window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=solicitacaoDesembolso&inuid=' + params.inuid;
    });

    $("#btn-pesquisar").click(function () {
        jQuery(this).button('loading');
        jQuery('#form_solicitacoes').submit();
    });

    //histórico de solicitações, dentro da modal
    function modalSolicitacaoDesembolso(ecoid, sdpid, edlid){
        $('#modal_titulo_solicitacao').html('Solicitação de desembolso');
        $('#modal_solicitacao').modal();

        $.post(window.location.href, {requisicao: "modalSolicitacaoDesembolso", ecoid: ecoid, sdpid: sdpid, edlid:edlid}, function (resp) {
            var dados = $.parseJSON(resp);
            $('#conteudo_modal_solicitacao').html(dados.form);
            $('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);
        });
    }

    function salvarSolicitacaoDesembolso(ecoid){
        var sdpid = $('#sdpid').val();
        var sdpobservacao = $('#sdpobservacao').val();
        var sdpvalor = $('#sdpvalor').val();
        sdpvalor = moedaParaFloat(sdpvalor);
        if(sdpvalor <= 0){
            $('#sdpvalor').addClass('erro');
            $('.erroValorSolicitado').html("<div class='errosdpvalor'><span class='danger'>O valor precisa ser maior que zero</span></div>");
            return false;
        }else{
            $('#sdpvalor').removeClass('erro');
            $('.errosdpvalor').remove();
        }

        $.post(window.location.href, {requisicao: "salvarSolicitacaoDesembolso",
            sdpid: sdpid,
            ecoid: ecoid,
            sdpobservacao: sdpobservacao,
            sdpvalor: sdpvalor}, function (resp) {

            swal({
                title: "Sucesso!",
                text: "A solicitação foi salva com sucesso",
                icon: "success"
            }, function() {
                window.location.reload();
            });
//            var dados = $.parseJSON(resp);
            /*$('#conteudo_modal_solicitacao').html(dados.form);
            $('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);*/

        });
    }

    function salvarSolicitacaoDesembolsoContrato(ecoid, proid){
        var sdpid = $('#sdpid').val();
        var sdpobservacao = $('#sdpobservacao').val();
        var sdpvalor = $('#sdpvalor').val();
        sdpvalor = moedaParaFloat(sdpvalor);
        if(sdpvalor <= 0){
            $('#sdpvalor').addClass('erro');
            $('.erroValorSolicitado').html("<div class='errosdpvalor'><span class='danger'>O valor precisa ser maior que zero</span></div>");
            return false;
        }else{
            $('#sdpvalor').removeClass('erro');
            $('.errosdpvalor').remove();
        }

        $.post(window.location.href, {requisicao: "salvarSolicitacaoDesembolso",
            sdpid:sdpid,
            ecoid: ecoid,
            proid: proid,
            sdpobservacao: sdpobservacao,
            sdpvalor: sdpvalor}, function (resp) {
            swal({
                title: "Sucesso!",
                text: "A solicitação foi salva com sucesso",
                icon: "success"
            }, function() {
                window.location.reload();
            });
        });
    }

    function salvarSolicitacaoDesembolsoDocumento(edlid, proid){
        var sdpid = $('#sdpid').val();
        var sdpobservacao = $('#sdpobservacao').val();
        var sdpvalor = $('#sdpvalor').val();
        sdpvalor = moedaParaFloat(sdpvalor);
        if(sdpvalor <= 0){
            $('#sdpvalor').addClass('erro');
            $('.erroValorSolicitado').html("<div class='errosdpvalor'><span class='danger'>O valor precisa ser maior que zero</span></div>");
            return false;
        }else{
            $('#sdpvalor').removeClass('erro');
            $('.errosdpvalor').remove();
        }

        $.post(window.location.href, {requisicao: "salvarSolicitacaoDesembolso",
            sdpid:sdpid,
            edlid: edlid,
            proid: proid,
            sdpobservacao: sdpobservacao,
            sdpvalor: sdpvalor}, function (resp) {
            swal({
                title: "Sucesso!",
                text: "A solicitação foi salva com sucesso",
                icon: "success"
            }, function() {
                window.location.reload();
            });
        });
    }

    function removerSolicitacaoDesembolso(sdpid, proid = null){
        swal({
            title: " Excluir",
            text: "Tem certeza que deseja remover esta solicitação?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: true,
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $.post(window.location.href, {
                    requisicao: "removerSolicitacaoDesembolso",
                    sdpid: sdpid
                }, function (resp) {
                    if (trim(resp) === 'falha') {
                        swal("Falha", "Erro ao remover solicitação. Contate o administrador do sistema.", "error");
                        return false;
                    } else {
                        window.location.reload();
                    }
                });
            }
        });
    }

    function validarValorSolicitado(input){
        var saldo = moedaParaFloat($('#modalSaldoContrato').text().substr(3));
        var valorDigitado = moedaParaFloat($(input).val());
        if(parseFloat(valorDigitado) > parseFloat(saldo)){
            $(input).val(floatParaMoeda(saldo));
        }
    }

    //histórico de solicitações, dentro da modal
    $(document).on('click', '#btn-historicosolicitacoes', function () {
        $('#historicosolicitacoes').slideToggle();
        //chevron up/down
        $('#chevron').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $('#chevron').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#chevron').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    function acaoDownload(id, arqid){
        window.location.href = window.location.href+'&requisicao=downloadArquivoContrato&arqid=' + arqid;
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