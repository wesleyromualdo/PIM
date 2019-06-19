
<!-- Requisições em contrato.php
controller/model: ExecucaoDocumentosLicitacao.class.inc
-->

<!-- Lista de Itens por Iniciativa -->
<div class="ibox float-e-margins">

    <div class="ibox-title text-center">
        <h2><i class="fa fa-list-alt" aria-hidden="true"></i> Lista de Iniciativas</h2>
    </div>

    <div id="listaIniciativas">
        <?php
        $arrIniciativas = $controllerDocumentoLicitacao->model->recuperarIniciativas($proid);

        if ($arrIniciativas) {
            Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['recarregarDiv' => false, 'req' => 'carregalistadcumentos', 'msgtooltip' => 'Clique para abrir os itens da iniciativa']);
        } else {
            echo '<div id="vazio" style="margin:20px auto; float:none;"
                         class="alert alert-info col-md-4 col-md-offset-6 text-center nenhum-registro">Nenhum item foi encontrado
                  </div>
                  ';
        }
        ?>
    </div>

    <?php if ($arrIniciativas) { ?>
        <div class="ibox-title text-center">
            <h2><i class="fa fa-file" aria-hidden="true"></i> Anexar Documentos</h2>
        </div>

        <div class="center" id="btnModalTR">
            <div style="width: 600px; margin: 0 auto;">
                <?php
                $sql = "SELECT
                                doaid as codigo,
                                doadescricao as descricao
                        FROM par3.tipo_documento_anexo
                        WHERE
                          doastatus = 'A'
                          AND doasituacao = 'A'
                        ORDER BY doadescricao
                ";

                echo $simec->select('doaid', 'Tipo de Documento', null, $sql);
                ?>
            </div>
            <button class=" visualizar_item btn btn-success" type="button" onclick="modalDocumento()"> Anexar Documento </button>
            <br><br>
        </div>

    <?php } ?>

<!-- Lista de Documentos -->

    <div class="ibox-title text-center">
        <h2><i class="fa fa-list-alt" aria-hidden="true"></i> Lista de Documentos</h2>
    </div>

    <div id="listaDocumentosLicitacao">
        <?php
        $controllerDocumentoLicitacao->listarDocumentos($_REQUEST['proid']);
        ?>
    </div>

</div>

<!-- Modal Anexar Documentos -->
<div class="ibox float-e-margins animated modal" id="modal_documentos_licitacao" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <form method="post" name="formDocumentoLicitacao" id="formDocumentoLicitacao" class="form form-horizontal" enctype="multipart/form-data">
            <div class="modal-content" >
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo"></h3>
                </div>
                <div class="ibox-content" id="conteudo_modal_doc">
                    <!-- Conteúdo -->
                </div>
                <div class="ibox-footer">
                    <div class="center">
                        <button type="submit" id="cancelarDocumentoAnexo"
                                data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="confirmarSalvar"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>


<script>

    $(document).on('change', '[name="aicid[]"]', function(){
        var campoValor = $('#edlvalordocumento');
        var valorAtual = $(campoValor).val();
        valorAtual = moedaParaFloat(valorAtual);
        var valorAlterado = $(this).attr('data-total');
        var valorTotal = 0;

        if($(this).is(':checked')){
           valorTotal = parseFloat(valorAtual) + parseFloat(valorAlterado);
           $(campoValor).val(floatParaMoeda(valorTotal));
       }else{
           valorTotal = parseFloat(valorAtual) - parseFloat(valorAlterado);
           $(campoValor).val(floatParaMoeda(valorTotal));
       }
    });

    $(document).on('change', '.checkall', function(){
        var valorDocumento = 0.00;
        $('input:checkbox:checked:not(.checkall)').each(function () {
            valorDocumento = parseFloat(valorDocumento) + parseFloat($(this).attr('data-total'));
        });
        $('#edlvalordocumento').val(floatParaMoeda(valorDocumento));
    });

    function modalAdesao(iasid, aicid){
        $.post(window.location.href, {requisicao: 'dadosAdesao', funcao: 'documentoslicitacao', iasid: iasid, aicid:aicid }, function (resp) {

            if($('#confirmarSalvar').css('display') != 'none'){
                $('#confirmarSalvar').hide();
            }

            var dados = $.parseJSON(resp);

            $('#modal_titulo').html('Item: ' + dados.item);
            $('#cancelarDocumentoAnexo').html('<i class="fa fa-times-circle-o"></i> Fechar');
            $('#conteudo_modal_doc').html(dados.dados);

            $('#modal_documentos_licitacao').modal();

        });
    }

    function modalDocumento(edlid = null){

        var doaid = $('#doaid').val();

        if(doaid === "" && edlid == null){
            swal("Atenção", "Por favor, selecione um tipo de documento!", "warning");
            return;
        }

        $.post(window.location.href, {requisicao: "modalDocumento", funcao: 'documentoslicitacao', doaid: doaid, edlid: edlid }, function (resp) {

            var dados = $.parseJSON(resp);
            $('#cancelarDocumentoAnexo').text('Cancelar');
            if($('#confirmarSalvar').css('display') == 'none'){
                $('#confirmarSalvar').show();
            }
            $('#confirmarSalvar').attr('onclick','salvarDocumento(' + edlid + ')')
                                 .html('Salvar Documento');
            $('#modal_titulo').html('Anexar Documento - ' + dados.nomeDocumento);
            $('#conteudo_modal_doc').html(dados.html);
            $('#modal_documentos_licitacao').modal();

        });

    }

    function salvarDocumento(id){

        var anexo = $('#anexo').val();
        var descricao = $('#descricao').val();
        var doaid = $('#doaid').val();
        var arqidExclusao = $('#arqid_licitacao').val();
        var edicao = $('#edicaoDocumento').val();
        if(edicao == 'true'){
            doaid = $('#doaidmodal').val();
        }
        var detalharitens = $('#detalharitens').val();
        var desembolso = $('#desembolso').val();
        var edlcnpjfornecedor = $('#edlcnpjfornecedor').val();
        var edlvalordocumento = $('#edlvalordocumento');
        if($(edlvalordocumento).length > 0){
            edlvalordocumento = parseFloat(moedaParaFloat($(edlvalordocumento).val()));
        }else{
            edlvalordocumento = null;
        }

        if((anexo == '' && ($('#listaAnexoLicitacao').html() == '' || $('#listaAnexoLicitacao').html() === undefined))
            || descricao == ''){
            swal("Atenção", "Os campos Arquivo e Descrição devem ser preenchidos!", "warning");
            return;
        }

        if(desembolso == 'true' && !(edlvalordocumento > 0)){
            swal("Atenção", "Por favor, preencha o valor do documento!", "warning");
            return;
        }

        var formData = new FormData();
        if(detalharitens == 'true') {
            var valorDocumento = 0;
            //recupera todos os checkboxs selecionados na página, transformando em array
            var aicid = $.map($('input:checkbox:checked:not(.checkall)'), function (e, i) {
                valorDocumento = parseFloat(valorDocumento) + parseFloat($(e).attr('data-total'));
                return +e.value
            });
            if (aicid.length < 1 && edicao == '') {
                swal("Atenção", "Por favor, selecione pelo menos um item para salvar este documento!", "warning");
                return;
            }

            formData.append('aicid', aicid);

            if(desembolso == 'true' && valorDocumento > 0){
                $('#edlvalordocumento').val(floatParaMoeda(valorDocumento));
            }
        }

        formData.append('file',$('#anexo')[0].files[0]);
        formData.append('descricao',$('#descricao').val());
        formData.append('requisicao', 'salvarDocumento');
        formData.append('funcao', 'documentoslicitacao');
        formData.append('edlid', id);
        formData.append('arqid', arqidExclusao);
        formData.append('doaid', doaid);
        formData.append('edlcnpjfornecedor', edlcnpjfornecedor);
        if(valorDocumento > 0){
            formData.append('edlvalordocumento', valorDocumento);
        }else{
            formData.append('edlvalordocumento', edlvalordocumento);
        }

        jQuery.ajax({
            url: window.location.href,
            data: formData,
            cache: false,
            contentType: false,
            processData: false,
            method: 'POST',
            type: 'POST', // For jQuery < 1.9
            success: function(data){

                if(data.trim() == 'sucesso'){
                    swal("Sucesso", "O documento foi anexado com Sucesso!", "success");
                    $.post(window.location.href, {requisicao: "atualizarListagensDocumento", funcao: 'documentoslicitacao'}, function (resp) {

                        var dados = $.parseJSON(resp);

                        $('#listaIniciativas').html(dados.iniciativas);
                        $('#listaDocumentosLicitacao').html(dados.documentos);

                    });
                }else if (data.trim() === 'filetype'){
                    swal("Falha", "Por favor, os arquivos devem ter as mesmas extensões cadastradas para este tipo de documento. ", "error");
                }else{
                    swal("Falha", "Falha ao salvar Documento", "error");
                }

                $('#modal_documentos_licitacao').modal('hide');
            }
        });

    }

    function modalAnalise(idDoc){
        $.post(window.location.href, {requisicao: "modalAnalise", funcao: 'documentoslicitacao', edlid: idDoc}, function (resp) {

            $('#modal_titulo').html('Análise do Documento');
            $('#conteudo_modal_doc').html(resp);

            if($('#edit').val() == 'false'){
                $('#confirmarSalvar').hide();
                $('#cancelarDocumentoAnexo').text('Fechar');
            }else{
                $('#confirmarSalvar').show();
                $('#confirmarSalvar').attr('onclick','salvarAnalise()').html('Salvar Análise');
                $('#confirmarSalvar').text('Salvar Análise');
                $('#cancelarDocumentoAnexo').text('Cancelar');
            }
            
            $('#modal_documentos_licitacao').modal();
            ajustarTamanhoTextarea();

        });

    }

    function salvarAnalise(){
        var eadparecer = $('#eadparecer').val();
        var eadproponente = $('#eadproponente').val();
        var eadproposta = $('#eadproposta').val();
        var eadprojeto  = $('#eadprojeto ').val();
        var eadobjetivo = $('#eadobjetivo').val();
        var eadjustificativa = $('#eadjustificativa').val();
        var eadvalores = $('#eadvalores').val();
        var eadcabiveis = $('#eadcabiveis').val();
//        var acao = $('#acaoanalise').val();
        var edlid = $('#edlid').val();
        var eadid = $('#eadid').val();
        var esdid = $('#esdid').val();
        var docid = $('#docid').val();

        if(eadparecer == ''){
            swal("Atenção", "Por favor, escreva o parecer!", "warning");
            return false;
        }

        jQuery.ajax({
            url: window.location.href,
            data: {
                requisicao: 'salvarAnalise',
                funcao: 'documentoslicitacao',
                edlid: edlid,
                eadparecer: eadparecer,
                eadproponente: eadproponente,
                eadproposta: eadproposta,
                eadprojeto: eadprojeto,
                eadobjetivo: eadobjetivo,
                eadjustificativa: eadjustificativa,
                eadvalores: eadvalores,
                eadcabiveis: eadcabiveis,
                esdid: esdid,
                eadid: eadid,
                docid: docid
            },
            method: 'POST',
            success: function (data) {

                var dados = $.parseJSON(data);

                if (dados.result == 'sucesso') {
                    swal("Sucesso", "Análise Salva com sucesso!", "success");

                    //atualizar lista de iniciativas
                   // $('#listaIniciativas').html('');
                   // $('#listaDocumentosLicitacao').html('');
                    //$.post(window.location.href, {requisicao: "atualizarListagensDocumento"}, function (resp) {

                        //var dados = $.parseJSON(resp);
                    $('#modal_documentos_licitacao').modal('hide');
                    $('#listaDocumentosLicitacao').html(dados.listaDocumentos.documentos);

                } else {
                    swal("Falha", "Falha ao salvar análise.", "error");
                }

//                $('#modal_documentos_licitacao').modal('hide');
            }
        });
    }

    function visualizarDocumentosItem(aicid){
        $.post(window.location.href, {requisicao: "visualizarDocumentosItem", funcao: 'documentoslicitacao', aicid: aicid}, function (resp) {
            $('#confirmarSalvar').hide();
            $('#conteudo_modal_doc').html(resp);
            $('#modal_documentos_licitacao').modal();
        });
    }

    function editarDocModal(edlid){
        $('#modal_contratos').modal('hide');
        modalDocumento(edlid);
    }

    //id colocado como argumento para que a função funcione a partir da ação da lista
    function downloadArqLicitacao(id, arqid){
        window.location.href = window.location.href+'&reqdownload=download&arqid=' + arqid;
    }

    function excluirAnexoLicitacao(arqid){
        swal({
            title: " Remover Documento Anexo!",
            text: "Tem certeza que deseja remover o Documento Anexo? (Esta ação surtirá efeito apenas ao salvar)",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $('#listaAnexoLicitacao').html('');
            }
        });
    }

    function removerDcumentoLicitacao(edlid) {
        swal({
            title: " Excluir",
            text: "Tem certeza que deseja excluir o item selecionado?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $.post(window.location.href, {requisicao: "desativarDocumentoLicitacao", funcao: 'documentoslicitacao', edlid: edlid}, function (resp) {
                    if(resp === 'falha'){
                        swal("Falha", "Erro ao excluir, tente novamente mais tarde.", "error");
                        return;
                    }else{
                        var dados = $.parseJSON(resp);

                        $('#listaIniciativas').html(dados.iniciativas);
                        $('#listaDocumentosLicitacao').html(dados.documentos);
                    }
                });
            }
        });
    }

    function ajustarTamanhoTextarea(element){

        if(element != null){
            element.style.height = "1px";
            element.style.height = (25+element.scrollHeight)+"px";
        }else{
            $('#modal_documentos_licitacao textarea').each(function(){
                this.style.height = "1px";
                this.style.height = (25+this.scrollHeight)+"px";
            });
        }
    }

    /* * * FUNÇÕES SOLICITAÇÃO DESEMBOLSO * * */

    function modalSolicitacaoDesembolsoDocumentos(edlid){
        $('#modal_titulo_solicitacao').html('Solicitação de desembolso');
        $('#modal_solicitacao').modal();

        $.post(window.location.href, {requisicao: "modalSolicitacaoDesembolso", funcao: 'desembolso', edlid: edlid}, function (resp) {
            var dados = $.parseJSON(resp);
            $('#conteudo_modal_solicitacao').html(dados.form);
            $('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);
        });
    }

    function salvarSolicitacaoDesembolsoDocumento(edlid, proid){
        var sdpid = $('#sdpid').val();
        var sdpobservacao = $('#sdpobservacao').val();
        var sdpvalor = $('#sdpvalor').val();
        sdpvalor = moedaParaFloat(sdpvalor);
//        var sdpsaldotermo = moedaParaFloat($('#modalSaldoTermo').text().substr(3));
        if(sdpvalor <= 0 || sdpvalor == 'NaN'){
            $('#sdpvalor').addClass('erro');
            $('.erroValorSolicitado').html("<div class='errosdpvalor'><span class='danger'>O valor precisa ser maior que zero</span></div>");
            return false;
        }else{
            $('#sdpvalor').removeClass('erro');
            $('.errosdpvalor').remove();
        }

        $.post(window.location.href, {
            requisicao: "salvarSolicitacaoDesembolso",
            funcao: 'desembolso',
            sdpid:sdpid,
            edlid: edlid,
            proid: proid,
            sdpobservacao: sdpobservacao,
            sdpvalor: sdpvalor
//            sdpsaldotermo: sdpsaldotermo
        }, function (resp) {
            var dados = $.parseJSON(resp);
            $('#conteudo_modal_solicitacao').html(dados.form);
            $('#footerModalSolicitacao').html(dados.botaoFechar + dados.botaoSalvar + dados.botaoExcluir);
            $('#listaDocumentosLicitacao').html(dados.listadocumentos);
        });
    }

    function removerSolicitacaoDesembolsoDocumento(sdpid, edlid){
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
                var dotid = $('#dotidsolicitacao').val();
                $.post(window.location.href, {
                    requisicao: "removerSolicitacaoDesembolso",
                    funcao: 'desembolso',
                    sdpid: sdpid,
                    edlid: edlid,
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
                        $('#listaDocumentosLicitacao').html(dados.listadocumentos);
                    }
                });
            }
        });
    }

    //Valor da solicitação não pode ser maior que o valor do contrato ou do termo de compromisso
    //O teto será o menor dentre esses 2
    function validarValorSolicitadoDocumento(input){
        var valorDocumento = parseFloat(moedaParaFloat($('#modalValordocumento').text().substr(3)));
        var saldoTermo = parseFloat(moedaParaFloat($('#modalSaldoTermo').text().substr(3)));
        var valorDigitado = parseFloat(moedaParaFloat($(input).val()));

        if(valorDigitado > valorDocumento || valorDigitado > saldoTermo){
            if(saldoTermo < valorDocumento){
                $(input).val(floatParaMoeda(saldoTermo));
            }else{
                $(input).val(floatParaMoeda(valorDocumento));
            }
        }
    }
    //histórico de solicitações, dentro da modal
    /*$(document).on('click', '#btn-historicosolicitacoes', function () {
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
    });*/

    /* * * FIM SOLICITAÇÃO DESEMBOLSO * * */

</script>
