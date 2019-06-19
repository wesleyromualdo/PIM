<?php

/**
 * $controllerExecucaoContrato foi instanciado em contrato.php
 * Requisições/'requisicao': contrato.php -> Controller/ExecucaoContrato.class.inc
*/

//inicializa. Utilizado para controlar a edição de um contrato (tudo via ajax). deve ser inicializado aqui para resetar com o refresh da página.
$_SESSION['idcontrato'] = '';

?>

<style>
    .txtright{
        text-align: right;
    }
    .linha_vermelha>td{
        background-color: #ffcdd2 !important;
    }

</style>



<div class="ibox">

    <div class="ibox-title">
        <h3><i class="fa fa-pencil-square-o" aria-hidden="true"></i> EDITAR/ADICIONAR CONTRATO</h3>
    </div>

    <div class="ibox-content" id="edit_contrato">

        <form method="post" name="formularioContrato" id="formulariocontrato" class="form form-horizontal">

            <input type="hidden" name="ecoid" id="ecoid" value=""/>
            <input type="hidden" name="proid" id="proid" value="<?= $proid ?>"/>
            <input type="hidden" name="arqid" id="arqid" value=""/> <!-- para exclusão do arquivo físico ao salvar o contrato -->

            <div class="form-group">
                <label class="col-lg-2 control-label">Arquivo (pdf)*</label>
                <div class="col-lg-10">
                    <input type="file" id="anexoContrato" placeholder="Arquivo" class="form-control"
                           accept='application/pdf'>
                </div>

                <!-- Mostrar arquivo de contrato anexado ao editar (preenchido via javascript) -->
                <div id="listaAnexo"></div>
            </div>

            <div class="form-group" style="margin-bottom: 0px;">
                <div class="col-lg-4">
                    <div class="form-group">
                        <label class="col-lg-6 control-label">CNPJ *</label>
                        <div class="col-lg-6">
                            <input type="text" id="ecocnpj" name="ecocnpj" class="form-control" maxlength="18" required
                                   onkeyup="this.value=mascaraglobal('##.###.###/####-##',this.value);"
                                   onblur="this.value=mascaraglobal('##.###.###/####-##',this.value);">
                        </div>
                    </div>
                </div>

                <div class="col-lg-6">
                    <div class="form-group">
                        <label class="col-lg-4 control-label">Razão social *</label>
                        <div class="col-lg-6">
                            <input type="text" id="ecorazaosocialfornecedor" name="ecorazaosocialfornecedor" class="form-control" maxlength="18" readonly>
                        </div>
                    </div>
                </div>
            </div>

            <div class="form-group">
                <div class="col-lg-4">
                    <div class="form-group">
                        <label class="col-lg-6 control-label">Número do contrato *</label>
                        <div class="col-lg-6">
                            <input type="text" id="econumero" name="econumero" class="form-control" required onkeyup="this.value=mascaraglobal('########',this.value);">
                        </div>
                    </div>
                </div>

                <div class="col-lg-4">
                    <div class="form-group">
                        <label class="col-lg-6 control-label">Data do contrato *</label>
                        <div class="col-lg-6 date">
                            <div class="input-group">
                                <span class="input-group-addon"><span class="fa fa-calendar"></span></span>
                                <input type="text" id="ecodta" name="ecodta" class="form-control datemask" required>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="col-lg-4">
                    <div class="form-group">
                        <label class="col-lg-6 control-label">CPF Responsável </label>
                        <div class="col-lg-6">
                            <input type="text" id="ecocpfresponsavel" name="ecocpfresponsavel" class="form-control" maxlength="18" required  onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onblur="this.value=mascaraglobal('###.###.###-##',this.value);">
                        </div>
                    </div>
                </div>
            </div>

            <div class="form-group">
                <label class="col-lg-2 control-label">Descrição *</label>
                <div class="col-lg-10">
                    <textarea rows="5" id="ecodsc" name="ecodsc" class="form-control" required></textarea>
                </div>
            </div>

            <div class="form-group">
                <div class="col-lg-12 text-right">
                    <button class="btn btn-white" type="button" title="Resetar dados do formulário" onclick="limparFormContrato();">
                        <i class="fa fa-eraser"></i> Limpar
                    </button>
                    <button class="btn btn-primary" type="button" id="salvarContrato"><i class="fa fa-save"></i> Salvar Contrato</button>
                </div>
            </div>

            <div id="mensagemitens" class="alert alert-danger" style="display: none;">
                Para concluir o cadastro, é necessário adicionar os itens do contrato nas iniciativas abaixo!
            </div>

            <div class="hr-line-dashed"></div>

            <h4>- INICIATIVAS</h4>

            <div class="hr-line-dashed"></div>

            <div id="msg_sem_contrato" class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" style="margin-top:20px;">
                Salve ou selecione o contrato para incluir os itens
            </div>

            <div id="listaContratoItens">

                <div class="form-group">
                    <label class="col-lg-2 col-lg-offset-4 control-label">Quantidade total dos itens:</label>
                    <div class="col-lg-2">
                        <input type="text" id="qtdtotal" class="form-control txtright" readonly value="0">
                    </div>
                    <label class="col-lg-2 control-label">Valor total dos itens:</label>
                    <div class="col-lg-2">
                        <input type="text" id="valortotal" class="form-control txtright" readonly value="0">
                    </div>
                </div>
                <?php

                    $arrIniciativas = $controllerExecucaoContrato->recuperaIniciativas($processo);
                if ($arrIniciativas) {
                    Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['recarregarDiv' => false, 'req' => 'contratolistaitens']);
                } else {
                    ?>

                        <div class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" style="margin-top:20px;">
                            Nenhum item encontrado
                        </div>

                    <?php
                }
                ?>
            </div>

        </form>

    </div>
</div>

<!-- Modal Contratos-->
<div class="ibox float-e-margins animated modal" id="modal_contratos" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo_contrato"></h3>
            </div>
            <div class="ibox-content" id="conteudo_modal_contrato">
                <!-- Conteúdo -->
            </div>
            <div class="ibox-footer center" id="footerModalContrato">
                <button type="submit"
                        data-dismiss="modal"
                        id="btnFecharModalContrato"
                        class="btn btn-default center">
                    <i class="fa fa-times-circle-o"></i> Fechar
                </button>
            </div>
        </div>
    </div>
</div>

<script>

    $(function(){

        //esconder lista de itens
        $('#listaContratoItens').hide();



        $('a[href="#lista_contrato"]').click(function(){
            if($('#ecoid').val() > 0){
                limparFormContrato();
            }
        });

        //retira erro dos campos 'required'
        $('#edit_contrato :input[required]').on('change', function(){
           var formgroup = $(this).closest('.form-group');
           if($(formgroup).hasClass('has-error')){
               if($(this).val() != ''){
                   $(this).closest('.form-group').removeClass('has-error');
                   $(this).parent().find('.msgerro').remove();
               }
           }
        });

        // validação campo data
        $("#ecodta").on('change',function(){
            var newDate = $(this).val();
            if (!moment(newDate, 'DD/MM/YYYY', true).isValid()){
                $(this).val('');
                if($(this).closest('.form-group').hasClass('has-error')){
                    $(this).parent().find('.msgerro').html('<span class="danger">Insira uma data válida</span>');
                }else{
                    $(this).closest('.form-group').addClass('has-error');
                    $(this).after("<div class='msgerro'><span class='danger'>Insira uma data válida</span></div>");
                }
            }
        });

        // Validação campo quantidade do contrato
        $('#ecoqtd').on('change', function(){
           if($(this).val() < 1){
               $(this).val('');
               if($(this).closest('.form-group').hasClass('has-error')){
                   $(this).parent().find('.msgerro').html('<span class="danger">Insira um valor maior que zero</span>');
               }else{
                   $(this).closest('.form-group').addClass('has-error');
                   $(this).after("<div class='msgerro'><span class='danger'>Insira um valor maior que zero</span></div>");
               }
           }
        });

    });

    //serviço cnpj
    $('#ecocnpj').on('change', function () {

        $("#ntfrazaosocialfornecedor").val("");
        $("#ntfuffornecedor").val("");
        $("#contratos_fornecedor").html("");

        var element = $(this);

        if ("" != $('#ecocnpj').val() && 18 != $('#ecocnpj').length) {
            var cnpj = str_replace(['.', ',', '/', '-'], [''], $('#ecocnpj').val());

            $.post(window.location.href, {requisicao: 'getPessoaJuridica', cnpj: cnpj}, function (data) {
                var jsonData = jQuery.parseJSON(data);
                if (jsonData.sucesso == false) {
                    $('#ecocnpj').val("");
                    swal("Alerta", jsonData.mensagem, "warning");
                } else {
                    $("#ecorazaosocialfornecedor").val(jsonData.empresa.no_empresarial_rf);
                    $(element).closest('.form-group').removeClass('has-error');
                }
            });
        }
    });

    //salvar itens
    $('.resultAjax').on('change', '.qtditem', function(){
        calculaValorItem(this);
        salvarItem(this);
    });

    //salvar quantidade destinada (monitoramento)
    $('.resultAjax').on('change', '.qtddestinada', function(){
        salvarQuantidadeDestinada(this);
    });

    //salvar quantidade escolas (dentro da modal)
    $('#conteudo_modal_contrato').on('change', '.qtdescola', function(){
        salvarQuantidadeEscola(this);
    });

    $(document).on('change','[name=escmuncod]',function(){

        var ipiid = $('#modalescolasipiid').val();
        var eciid = $('#modalescolaseciid').val();
        var muncod = $('#escmuncod').val();

        $.post(window.location.href,
                {funcao: 'contrato', requisicao: 'modalEscolas', ipiid: ipiid, eciid: eciid, muncod: muncod},
                function (resp) {
                    $('#conteudo_modal_contrato').html(resp);
                }
        );
    });

    $('#salvarContrato').click(function(){

        var ecoid = $('#ecoid').val();
        var files = $('#anexoContrato')[0].files[0];
        var arqid = $('#arqid').val();
        var proid = $('#proid').val();
        var cnpj = $('#ecocnpj').val();
        var ecorazaosocialfornecedor = $('#ecorazaosocialfornecedor').val();
        var cpfresponsavel = $('#ecocpfresponsavel').val();
        var econumero = $('#econumero').val();
        var ecodata = $('#ecodta').val();
        var ecodsc = $('#ecodsc').val();

        var erro = false;
        var qtdTotal = 0;
        var valorTotal = 0;
        var formData = new FormData();

        $('#edit_contrato :input[required]').each(function(){
            if($(this).val() == '' && $(this).attr('id') != 'ecocpfresponsavel'){
                if(!$(this).closest('.form-group').hasClass('has-error')) {
                    $(this).closest('.form-group').addClass('has-error');
                    $(this).after("<div class='msgerro'><span class='danger'>O campo não pode ser vazio</span></div>");
                }
                erro = true;
            }
        });

        if($('#anexoContrato').val() == '' && $('#listaAnexo').html() == ''){
            swal("Alerta", "Por favor, anexe o arquivo do contrato!", "warning");
            return false;
        }

        if(erro){
            return false;
        }

        formData.append('ecoid', ecoid);
        formData.append('file', files);
        formData.append('ecocnpj', cnpj);
        formData.append('ecorazaosocialfornecedor', ecorazaosocialfornecedor);
        formData.append('ecocpfresponsavel', cpfresponsavel);
        formData.append('econumero', econumero);
        formData.append('ecodta', ecodata);
        formData.append('ecodsc', ecodsc);
        formData.append('requisicao', 'salvarContrato');
        formData.append('funcao', 'contrato'); //para o controle das requisições em contrato.php
        formData.append('arqid', arqid);
        formData.append('proid', proid);

        jQuery.ajax({
            url: window.location.href,
            data: formData,
            cache: false,
            contentType: false,
            processData: false,
            method: 'POST',
            type: 'POST', // For jQuery < 1.9
            success: function(data){
                if(parseInt(data) > 0){

                    //limparFormContrato();
                    $('#ecoid').val(trim(data));
                    swal("Sucesso", "Contrato salvo com Sucesso!", "success");

                    //atualiza lista, sem tirar id do contrato da sessão
                    atualizaListaContratos();

                    //Mostra mensagem de lembrete para usuário preencher os itens do contrato
                    $('#mensagemitens').css('display','block');

                    //mostra lista de itens
                    $('#listaContratoItens').show();

                    //remove mensagem da lista de itens
                    $('#msg_sem_contrato').css('display','none');

                    //remove erros do formulário
                    $('.msgerro').remove();
                    $('.has-error').removeClass('has-error');

                    //se necessário, atualiza lista de contratos na aba de nota fiscal
                    atualizarListaAbaNF();



                }else if (trim(data) === 'filetype'){
                    swal("Falha", "Por favor, escolha arquivos em formato PDF! ", "error");
                }else if (trim(data) === 'falhaexcluirarquivo'){
                    swal("Falha", "Falha ao excluir arquivo. Por favor, atualize a página e tente novamente.", "error");
                }/*else if(trim(data) == 'valorcontrato'){
                    swal("Falha", "O valor do contrato é menor que o valor das solicitações de desembolso aprovadas.", "error");
                }*/else{
                    swal("Falha", "Falha ao anexar Contrato", "error");
                }
            }
        });
    });

    //calcula valor do item pela quantidade digitada
    function calculaValorItem(elem){
        var valorReferencia = parseFloat($(elem).data('vref'));
        var qtdDigitada = parseInt($(elem).val());
        var qtdMax = parseInt($(elem).data('max'));
        if(parseInt(qtdDigitada) > qtdMax){
            qtdDigitada = qtdMax;
            $(elem).val(qtdMax);
        }

        var valorTotal = valorReferencia * qtdDigitada;
        valorTotal = valorTotal.toFixed(2);
        if(Number.isInteger(valorTotal)){
            valorTotal += '00';
        }
        var vlrtemp = valorTotal;
        valorTotal = mascaraglobal('###.###.###,##', valorTotal);
        $(elem).parent().parent().find('.valoritem').val(valorTotal);
        return vlrtemp;
    }

    function formatarValorItem(){
        $('.valoritem').each(function(){$(this).val(mascaraglobal('###.###.###,##',this.value));});
    }

    function salvarItem(element){
        var ecoid = $('#ecoid').val();
        var aicid = $(element).data('aicid');
        var eciqtditens = $(element).val();
        var ecivaloritens = $(element).parent().parent().find('.valoritem').val();
        var eciid = $(element).attr('data-eciid');

        ecivaloritens = moedaParaFloat(ecivaloritens);

        if(eciqtditens == ''){
            return false;
        }

        if(ecoid < 1){
            swal("Alerta", "Por favor, selecione um contrato ou salve um novo!", "warning");
            $(element).val('');
            $(element).parent().parent().find('.valoritem').val('');
            return false;
        }

        $.post(window.location.href, {requisicao: "salvaritem",
                                      funcao: 'contrato',
                                      ecoid: ecoid,
                                      aicid: aicid,
                                      eciqtditens: eciqtditens,
                                      ecivaloritens: ecivaloritens,
                                      eciid: eciid}, function (resp) {

            try {

                var dados = $.parseJSON(resp);

                //atualizar coluna 'Qtd sem contrato'
                var qtdmax = '';
                $(element).parent().parent().find('td:nth-child(5)').text(function () {
                    var qtd = $(element).data('max');
                    qtdmax = qtd - eciqtditens;
                    $(this).text(qtdmax);
                });

                $(element).attr('data-eciid', dados.eciid);
                $(element).parent().parent().find('.qtddestinada').attr('data-eciid', dados.eciid);
                $(element).parent().parent().find('.modalescola').attr('data-eciid', dados.eciid);
                $(element).parent().parent().find('.qtddestinada').attr('data-max', qtdmax);
                $(element).parent().parent().find('.modalescola').attr('data-max', qtdmax);
                $('#valortotal').val(mascaraglobal('###.###.###.###,##', dados.total.valortotal));
                $('#qtdtotal').val(dados.total.qtdtotal);

                atualizaValoresListaContrato(ecoid, dados.total.qtdtotal, dados.total.valortotal);

                $('#mensagemitens').css('display','none');

                //se necessário, atualiza lista de contratos e itens na aba de nota fiscal
                atualizarListaAbaNF();
            }catch(e){
                if(trim(resp) === 'falha'){
                    swal("Alerta", "Falha ao remover o item. Verifique se ele está cadastrado em uma nota fiscal", "error");
                }else{
                    var dados = $.parseJSON(resp);
                    if(dados.erro == 'itemnf'){
                        swal("Alerta", "Este item já está cadastrado em uma nota fiscal.", "error");
                        $(element).val(dados.qtditem);
                        $(element).parent().parent().find('.valoritem').val(mascaraglobal('###.###.###.###,##', dados.vlritem));
                    }
                }
            }

        });

    }

    function fecharAccordionContrato(){
        $('#listaContratoItens .resultAjax').html('');
        $('#listaContratoItens .in').removeClass('in');
        $('#listaContratoItens .contratolistaitens').addClass('collapsed');
    }

    function excluirAnexo(arqid){
        swal({
                title: " Remover Documento Anexo!",
                text: "Tem certeza que deseja remover o Documento Anexo? (Esta ação surtirá efeito apenas ao salvar o contrato)",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            }, function (isConfirm) {
                if (isConfirm) {
                    $('#listaAnexo').html('');
                    $('#arqid').val(arqid);
                }
        });
    }

    function limparFormContrato(){
        $('#ecoid').val('');
        $('#anexoContrato').val('');
        $('#ecocnpj').val('');
        $('#ecorazaosocialfornecedor').val('');
        $('#econumero').val('');
        $('#ecodta').val('');
        $('#ecocpfresponsavel').val('');
        $('#ecodsc').val('');
        $('#listaAnexo').html('');
        $('#valortotal').val('');
        $('#qtdtotal').val('');

        //esconde lista de itens
        $('#listaContratoItens').hide();

        //mostra mensagem de aviso - necessário contrato salvo
        $('#msg_sem_contrato').css('display','block');

        //remove marcações de erro do formulário
        $('#edit_contrato .has-error').removeClass('has-error');
        $('#edit_contrato .msgerro').remove();

        fecharAccordionContrato();
        habilitarInputsContrato();
    }

    //atualiza a lista de contratos na aba Lista
    function atualizaListaContratos(){
        $('#listaContratos').html('');
        $.post(window.location.href, {requisicao: "atualizarListaContratos", funcao: 'contrato'}, function (resp) {
            $('#listaContratos').html(resp);
        });
    }

    //lista todos os contratos que contem alguma quantidade do item na lista
    function modalContratosItem(aicid){
        $.post(window.location.href, {requisicao: "listaContratosItem", funcao: 'contrato', aicid: aicid}, function (resp) {
            $('#conteudo_modal_contrato').html(resp);
            $('#modal_contratos').modal();
        });
    }

    //botão para editar contrato dentro da modal
    function editarContratoModal(id){
        swal({
            title: "Confirmar",
            text: "Tem certeza que deseja editar o contrato selecionado? As informações inseridas na página atual serão perdidas!",
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55",
            confirmButtonText: "Sim",
            cancelButtonText: "Não",
            closeOnCancel: true,
            closeOnConfirm: true
        }, function (isConfirm) {
            if(isConfirm) {
                //função escrita em _list_contrato
                editarContrato(id);
                $('#modal_contratos').modal('hide');
            }
        });
    }

    //preenche todos os itens dentro de um accordion
    function incluirtodos(element){
        var inpid = $(element).data('inpid');
        var ecoid = $('#ecoid').val();
        var item = [];

        $('#resultcontratolistaitens' + inpid + ' .qtditem').each(function(){
            if(!$(this).attr('disabled')) {
                var eciqtditens = $(this).attr('data-max');
                $(this).val(eciqtditens);
                var eciid = $(this).attr('data-eciid');
                var ecivaloritens = calculaValorItem($(this));
                var aicid = $(this).data('aicid');

                //atualizar coluna qtd sem contrato
                var qtdmax = '';
                var element = $(this);
                $(this).parent().parent().find('td:nth-child(5)').text(function(){
                    var qtd = $(element).data('max');
                    qtdmax = qtd-eciqtditens;
                    $(this).text(qtdmax);
                });

                item.push({
                    'aicid': aicid,
                    'ecoid': ecoid,
                    'eciqtditens': eciqtditens,
                    'ecivaloritens': ecivaloritens,
                    'eciid': eciid
                });
            }
        });

        $.post(window.location.href, {requisicao: "salvarArrayItem", funcao: 'contrato', item:item}, function (resp) {
            var dados = $.parseJSON(resp);

            var itens = $('.qtditem');
            var length = dados.length;

            $(itens).each(function(){
               for(var i = 0; i < length; i++){
                   if(dados[i].aicid == $(this).attr('data-aicid') && dados[i].eciqtditens == $(this).val()){
                       $(this).attr('data-eciid', dados[i].eciid);
                       $(this).parent().parent().find('[name="eciqtddestinada[]"]').attr('data-eciid', dados[i].eciid);
                   }
               }
            });
/*
            for(var i = 0; i < length; i++){
                for(var l = 0; i < dados.length; i++){
                    if()
                    $(itens.get(i)).attr('data-eciid', dados[i].eciid);
                }
            }*/

            atualizaValoresListaContrato(ecoid, dados[0].qtdtotal, dados[0].valortotal);

            $('#valortotal').val(mascaraglobal('###.###.###.###,##',dados[0].valortotal));
            $('#qtdtotal').val(dados[0].qtdtotal);
            $('#mensagemitens').css('display','none');

        });

    }

    function atualizaValoresListaContrato(ecoid, qtd, valor){
        $('#linhaContrato' + ecoid).find('td').eq(8).text(qtd); // qtd itens
        $('#linhaContrato' + ecoid).find('td').eq(9).text(mascaraglobal('###.###.###.###,##',valor)); // valor contrato
    }
    /*-*-* FIM FUNÇÕES CONTRATO *-*-*/

    /* * * FUNÇÕES MONITORAMENTO * * */

    function salvarQuantidadeDestinada(element){
        if($(element).attr('data-eciid') < 1){
            swal("Alerta", "Por favor, informe a quantidade de itens antes da destinação", "warning");
            element.value = '';
            return false;
        }
        var eciid = $(element).data('eciid');
        var eciqtddestinada = $(element).val();
        var qtdmax = $(element).parent().parent().find('.qtditem').val();
        if(parseInt(eciqtddestinada) > parseInt(qtdmax)){
            $(element).val('');
            swal("Alerta", "O valor informado não pode ser maior que a quantidade de itens do contrato.", "warning");
            return false;
        }
        $.post(window.location.href, {requisicao: "salvarQuantidadeDestinada", funcao: 'contrato', eciid: eciid, eciqtddestinada: eciqtddestinada}, function (resp) {

            /*if(parseInt(resp) > 0){
             $(element).data('emoid', resp);
             $(element).parent().find('.salvamentoconcluido').html(
             ' <span class="glyphicon glyphicon-ok" style="color:#00b300"></span>'
             )
             }else{
             $(element).parent().find('.salvamentoconcluido').html(
             ' <span class="glyphicon glyphicon-remove" style="color:#b33635"></span>'
             )
             }*/

        });
    }

    //abre modal de escolas quando o item for deste tipo
    function modalEscolas(element){
        if($(element).attr('data-eciid') < 1){
            swal("Alerta", "Por favor, informe a quantidade de itens antes da destinação", "warning");
            return false;
        }

        var titulo = $(element).parent().parent().find('td:nth-child(1)').text();
        var ipiid = $(element).data('ipiid');
        var eciid = $(element).data('eciid');

        $.post(window.location.href, {requisicao: "modalEscolas", funcao: 'contrato', ipiid: ipiid, eciid: eciid}, function (resp) {
            if(trim(resp) === 'idcontrato'){
                swal("Alerta", "Id do contrato não encontrado, por favor, selecione o contrato novamente!", "warning");
                return false;
            }
            $('#modal_titulo_contrato').text(titulo);
            $('#modal_contratos').modal();
            $('#conteudo_modal_contrato').html(resp);
        });
    }

    // atualiza pagina da lista
    function atualizarListaModalEscolas(pagina){
        var aicid = $('#modalescolasipiid').val();
        var eciid = $('#modalescolaseciid').val();
        $.post(window.location.href, {requisicao: "atualizarListaModalEscolas",
                                      funcao: 'contrato',
                                      ipiid: ipiid,
                                      eciid: eciid,
                                      pagina: pagina}, function (resp) {

            $('#conteudo_modal_contrato').html(resp);

        });
    }

    //quantidade monitorada/destinada da modal de escolas
    function salvarQuantidadeEscola(element){
        var eciid = $(element).data('eciid');
        var ipeid = $(element).data('ipeid');
        var emeid = $(element).attr('data-emeid');
        var escid = $(element).attr('data-escid');
        var emeqtd = $(element).val();
        var qtditensmax = $(element).parent().parent().find('.qtditem');

        $.post(window.location.href, {requisicao: "salvarQuantidadeEscola",
                                      funcao: 'contrato',
                                      eciid: eciid,
                                      ipeid: ipeid,
                                      emeqtd: emeqtd,
                                      emeid: emeid,
                                      escid: escid}, function (resp) {

            var dados = jQuery.parseJSON(resp);

            if(dados.erro == 'insert_error'){
                swal('Erro', 'Erro ao salvar quantidade. Favor contatar o administrador do sistema.', 'error');
                return;
            }

            if(parseInt(dados.idmonitoramento) > 0) {
                //atribuir id ao item
                $(element).attr('data-emeid',dados.idmonitoramento);
            }else{
                $(element).attr('data-emeid','');
            }
            $('[data-eciid=' + eciid + '].modalescola').val(dados.total);
            //contar total para atualizar lista de itens
            /*var total = 0;
            $('input[name="emeqtd[]"]').each(function () {
                var valor = $(this).val();
                if (valor > 0) {
                    valor = parseInt(valor);
                    total = total + valor;
                }
            });*/

            /*$.post(window.location.href, {
                requisicao: "salvarQuantidadeDestinada",
                funcao: 'contrato',
                eciid: eciid}, function(resp){
                console.log(resp);
                    $('[data-eciid=' + eciid + '].modalescola').val(resp);
            });*/
        });
    }
    /* * * FIM MONITORAMENTO * * */

    function atualizarListaAbaNF(){
        if($("#contratos_fornecedor").html() != ''){
            limparFormNF();
        }
    }

    function desabilitarInputsContrato(){
        $('#edit_contrato input.form-control').prop('disabled', true);
        $('#ecodsc').prop('disabled', true);
        $('#salvarContrato').hide();
    }
    function habilitarInputsContrato(){
        $('#edit_contrato input.form-control').prop('disabled', false);
        $('#salvarContrato').show();
        $('#ecodsc').prop('disabled', false);
    }

</script>