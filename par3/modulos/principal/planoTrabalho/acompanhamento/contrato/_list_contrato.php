<?php

/**$controllerExecucaoContrato foi instanciado em contrato.php
 * Requisições/'requisicao': contrato.php -> Controller/ExecucaoContrato.class.inc
 *
 */

?>

<style>

    .erro{
        border: 1px solid red !important;
    }

</style>

<div class="ibox float-e-margins">

    <div class="ibox-title text-center">
        <h2><i class="fa fa-list-alt" aria-hidden="true"></i> Lista de Contratos</h2>
    </div>

        <div class="ibox-content" id="listaContratos">
            <div class="row">
                <div class="col-lg-12 pull-left">
                    <b>Legenda:</b>
                    <div class="glyphicon  glyphicon-pencil success"></div> Editar Contrato|
                    <div class="glyphicon glyphicon-download-alt primary"></div> Download do Contrato |
                    <div class="glyphicon glyphicon-usd warning"></div> Solicitar Desembolso
                    <div class="glyphicon glyphicon-trash danger"></div> Inativar Contrato
                </div>
            </div>
            <?php
            //$proid em contrato.php
            $controllerExecucaoContrato->listaContratos($proid);
            ?>


        </div>
</div>

<!-- html da modal de solicitação desembolso se encontra no arquivo contrato.php -->

<script>

    function editarContrato(id){

        $.post(window.location.href, {requisicao: "editarContrato", funcao: 'contrato', ecoid: id}, function (resp) {

            fecharAccordionContrato();
            $('a[href="#add_contrato"]').tab('show');

            var dados = $.parseJSON(resp);

            $('#ecoid').val(id);
            $('#ecocnpj').val(mascaraglobal('##.###.###/####-##',dados.ecocnpj));
            $('#ecorazaosocialfornecedor').val(dados.ecorazaosocialfornecedor);
            $('#ecocpfresponsavel').val(mascaraglobal('###.###.###-##',dados.ecocpfresponsavel));
            $('#econumero').val(dados.econumero);
            $('#ecodta').val(dados.ecodta);
            $('#ecodsc').val(dados.ecodsc);
            $('#valortotal').val(mascaraglobal('###.###.###.###,##', dados.valortotalitens));
            $('#qtdtotal').val(dados.qtdtotalitens);
            $('#listaContratoItens').css('display','block');
            $('#msg_sem_contrato').css('display','none');

            var divArquivo = "<span class='col-lg-2'></span><div class='col-lg-10' style='padding-top: 5px' id='arquivo_" + dados.arqid + "'>" +
                                "<div class='MultiFile-label'></div>" +
                                "<a class='MultiFile-remove' href='#anexo'>" +
                                    "<a onclick='excluirAnexo(" + dados.arqid + ")' class='btn btn-danger btn-xs' title='Excluir'" +
                                    "aria-label='Close'><span aria-hidden='true'>×</span> </a> </a>   " +
                                "<a title='Baixar' href='" + window.location.href + "&reqdownload=download&arqid=" + dados.arqid + "'>" +
                                    "<i class='glyphicon glyphicon-cloud-download btn btn-warning btn-xs'></i></a>" +
                                "<span disabled>" +
                                    "<span class='MultiFile-label' title='Selecionado: " + dados.arqnome + "." + dados.arqextensao + "'>" +
                                        "<span class='MultiFile-title'>" +
                                            "<input type='hidden' name='arquivo-selecionado' id='arquivo-selecionado' value='anterior'>  " +
                                                dados.arqnome + "." + dados.arqextensao +
                                        "</span>" +
                                    "</span>" +
                                "</span>" +
                            "</div>";

            $('#listaAnexo').html(divArquivo);
            $('#arqid').val(dados.arqid); //para excluir o arquivo físico quando o usuário clicar em excluir ou anexar um novo. este campo está no _edit_contrato

            if(dados.ecostatus === 'I'){
                desabilitarInputsContrato();
            }else{
                habilitarInputsContrato();
            }

            if(parseInt(dados.ecoqtd) > 0){
                $('#mensagemitens').css('display','none');
            }else{
                $('#mensagemitens').css('display','block');
            }

        });

    }

    function removerContrato(ecoid){
        swal({
            title: " Excluir",
            text: "Tem certeza que deseja remover o contrato selecionado?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: true,
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $.post(window.location.href, {requisicao: "removerContrato", funcao: 'contrato', ecoid: ecoid}, function (resp) {
                    if(trim(resp) == 'existedesembolso'){
                        swal("Falha", "Existe uma solicitação de desembolso em andamento ou aprovada para este contrato.", "error");
                    }else if(trim(resp) == 'falha'){
                        swal("Falha", "Erro ao excluir, tente novamente mais tarde.", "error");
                        return false;
                    }else{
                        atualizaListaContratos();
                    }
                });
            }
        });
    }

    function downloadArquivoContrato(id, arqid){
        window.location.href = window.location.href+'&reqdownload=download&arqid=' + arqid;
    }

    /* * * FUNÇÕES SOLICITAÇÃO DESEMBOLSO * * */

    function modalSolicitacaoDesembolsoContrato(ecoid){
        $('#modal_titulo_solicitacao').html('Solicitação de desembolso');
        $('#modal_solicitacao').modal();

        $.post(window.location.href, {requisicao: "modalSolicitacaoDesembolso", funcao: 'desembolso', ecoid: ecoid}, function (resp) {
            var dados = $.parseJSON(resp);
            $('#conteudo_modal_solicitacao').html(dados.form);
            $('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);
        });
    }

    function salvarSolicitacaoDesembolsoContrato(ecoid, proid){
        var sdpid = $('#sdpid').val();
        var sdpobservacao = $('#sdpobservacao').val();
        var sdpvalor = $('#sdpvalor').val();
        sdpvalor = moedaParaFloat(sdpvalor);
        var sdpsaldocontrato = moedaParaFloat($('#modalSaldoContrato').text().substr(3));
        var sdpsaldotermo = moedaParaFloat($('#modalSaldoTermo').text().substr(3));

        if(sdpvalor <= 0){
            $('#sdpvalor').addClass('erro');
            $('.erroValorSolicitado').html("<div class='errosdpvalor'><span class='danger'>O valor precisa ser maior que zero</span></div>");
            return false;
        }else{
            $('#sdpvalor').removeClass('erro');
            $('.errosdpvalor').remove();
        }

        $.post(window.location.href, {requisicao: "salvarSolicitacaoDesembolso",
                                        funcao: 'desembolso',
                                        sdpid:sdpid,
                                        ecoid: ecoid,
                                        proid: proid,
                                        sdpobservacao: sdpobservacao,
                                        sdpvalor: sdpvalor,
                                        sdpsaldocontrato: sdpsaldocontrato,
                                        sdpsaldotermo: sdpsaldotermo
                                    }, function (resp) {
            var dados = $.parseJSON(resp);
            $('#conteudo_modal_solicitacao').html(dados.form);
            $('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);
            $('#listaContratos').html(dados.listacontratos);
        });
    }

    function removerSolicitacaoDesembolsoContrato(sdpid, ecoid){
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
                var proid = $('#proididsolicitacao').val();
                $.post(window.location.href, {
                    requisicao: "removerSolicitacaoDesembolso",
                    funcao: 'desembolso',
                    sdpid: sdpid,
                    ecoid: ecoid,
                    proid: proid
                }, function (resp) {
                    if (trim(resp) === 'falha') {
                        swal("Falha", "Erro ao remover solicitação. \nPor favor, tente recarregar a página ou contate o administrador do sistema.", "error");
                        return false;
                    } else {
                        var dados = $.parseJSON(resp);
                        //$('#conteudo_modal_solicitacao').html(dados.form);
                        //$('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);
                        $('#modal_solicitacao').modal('hide');
                        $('#listaContratos').html(dados.listacontratos);
                    }
                });
            }
        });
    }

    //Valor da solicitação de desembolso não pode ser maior que o valor do contrato ou do termo de compromisso
    //O teto será o menor dentre esses 2
    function validarValorSolicitadoContrato(input){
        var saldoContrato = parseFloat(moedaParaFloat($('#modalSaldoContrato').text().substr(3)));
        var saldoTermo = parseFloat(moedaParaFloat($('#modalSaldoTermo').text().substr(3)));
        var valorDigitado = parseFloat(moedaParaFloat($(input).val()));

        if(valorDigitado > saldoContrato || valorDigitado > saldoTermo){
            if(saldoTermo < saldoContrato){
                $(input).val(floatParaMoeda(saldoTermo));
            }else{
                $(input).val(floatParaMoeda(saldoContrato));
            }
        }
    }

    //mostrar/esconder histórico de solicitações, dentro da modal
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

    /* * * FIM SOLICITAÇÃO DESEMBOLSO * * */

</script>