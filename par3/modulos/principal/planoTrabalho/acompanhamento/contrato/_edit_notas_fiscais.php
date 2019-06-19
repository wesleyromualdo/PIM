<?php

//Requisições: contrato.php -> Controller/ExecucaoNotaFiscal.class.inc

switch ($_REQUEST['requisicao']) {
    /* Detalhamento de itens não será utilizado agora.
     * case 'modaldetalhamentoitem':
        ob_clean();
        $controllerExecucaoNotaFiscal->modalDetalhamentoItem($_REQUEST['encid'], $_REQUEST['inuid']);
        exit;
        break;
    case 'salvarDetalhamentoItem':
        ob_clean();
        $controllerExecucaoNotaFiscal->salvarDetalhamentoItem($_REQUEST);
        exit;
        break;
    case 'removerDetalhamentoItem':
        ob_clean();
        $controllerExecucaoNotaFiscal->removerDetalhamentoItem($_REQUEST['edvid'], $_REQUEST['encid']);
        exit;
        break;
    case 'atualizarListaDetalhamento':
        ob_clean();
        $controllerExecucaoNotaFiscal->listaDetalhamentoItens($_REQUEST['encid'], $_REQUEST['pagina']);
        exit;
        break;
    case 'downloadFotoDetalhamento':
        ob_clean();
        $file = new FilesSimec("execucao_detalhamento_veiculo", null, "par3");
        $file->getDownloadArquivo($_GET['arqid']);
        break;*/
}

?>

<div class="ibox">
    <div class="ibox-title">
        <h3><i class="fa fa-pencil-square-o" aria-hidden="true"></i> EDITAR/ADICIONAR NOTAS FISCAIS</h3>
    </div>

    <div class="ibox-content" id="edit_nf">

        <div id="msgvalornota" class="alert alert-warning" style="display: none;">
            O valor da nota está diferente do valor informado nos itens!
        </div>

        <form method="post" name="edit_nf" id="edit_nf" class="form form-horizontal form_salvar_nota">
            <input type="hidden" name="ntfid" id="ntfid" value=""/>
            <input type="hidden" name="proid" id="proid" value="<?= $proid ?>"/>
            <input type="hidden" name="arqid" id="arqid" value=""/>

            <div class="form-group">
                <label class="col-lg-2 control-label">Arquivo</label>
                <div class="col-lg-10">
                    <input type="file" placeholder="Arquivo" class="form-control" name="arquivo" id="arquivo" accept='application/pdf'>
                </div>
                <div id="listaAnexoNotaFiscal"></div>
            </div>

            <div class="form-group">
                <div class="col-lg-3" style="margin-left: 8.2%;" id="combontfcnpjfornecedor">
                    <?php
                    #variável proid recuperada no arquivo contrato.php
                    $arrCnpj = $controllerExecucaoNotaFiscal->comboCnpjFornecedor($proid);
                    echo $simec->select('ntfcnpjfornecedor', 'CNPJ Fornecedor', null, $arrCnpj, array(), array('label-size' => '4', 'input-size' => '8'));
                    ?>
                </div>
                <label class="col-lg-1 control-label">Razão social</label>
                <div class="col-lg-3">
                    <input type="text" class="form-control" id="ntfrazaosocialfornecedor" name="ntfrazaosocialfornecedor" readOnly="readonly">
                </div>
                <label class="col-lg-1 control-label">UF</label>
                <div class="col-lg-3">
                    <input type="text" class="form-control disabled" name="ntfuffornecedor" id="ntfuffornecedor" readOnly="readonly">
                </div>
            </div>

            <div class="col-lg-4">
                <div class="form-group">
                    <label class="col-lg-6 control-label">Número da nota</label>
                    <div class="col-lg-6">
                        <input type="text" class="form-control" name="ntfnumeronotafiscal" id="ntfnumeronotafiscal" required readOnly="readonly" onkeyup="this.value=mascaraglobal('########',this.value);">
                    </div>
                </div>
            </div>

            <div class="col-lg-4">
                <div class="form-group">
                    <label class="col-lg-3 control-label">Data da nota</label>
                    <div class="col-lg-9 date">
                        <div class="input-group">
                            <span class="input-group-addon"><span class="fa fa-calendar"></span></span>
                            <input name="ntfdata" type="text" class="datemask form-control" id="ntfdata" required readOnly="readonly" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" onblur="this.value=mascaraglobal('##/##/####',this.value);">
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-lg-4">
                <div class="form-group">
                    <label class="col-lg-3 control-label">Valor da nota</label>
                    <div class="col-lg-9">
                        <input type="text" name="ntfvlrnota" id="ntfvlrnota" class="form-control txtright" required readOnly="readonly" onkeyup="this.value=mascaraglobal('###.###.###.###,##',this.value);" onblur="this.value=mascaraglobal('###.###.###.###,##',this.value);">
                    </div>
                </div>
            </div>

            <div class="form-group">
                <div class="col-lg-12 text-right">
                    <button class="btn btn-white" type="button" title="voltar a paginar anterior" onclick="limparFormNF()"><i class="fa fa-eraser"></i> Limpar</button>
                    <button class="btn btn-primary" type="button" id="salvarNotaFiscal"><i class="fa fa-save"></i> Salvar Nota</button>
                </div>
            </div>

            <div class="hr-line-dashed" style="clear: both;"></div>

            <h4>- CONTRATOS</h4>

            <div class="hr-line-dashed"></div>

            <div id="contratos_fornecedor"></div>

        </form>
    </div>
</div>


<!-- Modal Detalhamento -->
<div id="modal-form" class="modal fade" aria-hidden="true">
    <div id="modal_detalhamento" class="modal fade" aria-hidden="true">
        <div class="modal-content" id="dv-form-large">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h1 id="modal_titulo_detalhamento"> Detalhamento do Serviço / Item </h1>
            </div>
            <div class="ibox-content" id="conteudo_modal_detalhamento">

            </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="submit"
                            data-dismiss="modal"
                            class="btn btn-default center">
                        <i class="fa fa-times-circle-o"></i> Fechar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>


<script>
    $(function() {

        //carrega cnpj dos contratos ao clicar na aba 'notas fiscais'
        $('a[href="#notas_fiscais"]').on('click', function(){
            $.post(window.location.href, {requisicao: "carregarComboCnpjContratos", funcao: 'notasfiscais'}, function (resp) {
                $('#combontfcnpjfornecedor').html(resp);
            });
        });

        //remove mensagem de erro para campos preenchidos
        $('#edit_nf :input[required]').on('change', function(){
            var formgroup = $(this).closest('.form-group');
            if($(formgroup).hasClass('has-error')){
                if($(this).val() != ''){
                    $(this).closest('.form-group').removeClass('has-error');
                    $(this).parent().find('.msgerro').remove();
                }
            }
        });

        //mensagem de erro para dada inválida
        $("#ntfdata").change(function(){
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
    });

    //limpa o formulário ao clicar na lista de notas fiscais
    $('a[href="#lista_notas_fiscais"]').click(function(){
        if($('#ntfid').val() > 0){
            limparFormNF();
        }
    });

    //função que coloca mascara de moeda nos inputs da lista
    function formataValorItensNF(){
        $('input[name^="vlritensnota"]').each(function(){$(this).val(mascaraglobal('###.###.###,##',this.value));});
    }

    //salvar item
    $(document).on('change', 'input[name^="qtditensnota"]', function(i,v) {
        var quantidadeInformada = parseInt($(this).val());
        var valorReferencia = $(this).data("ipivalorreferencia");
        var quantidadeTotalRestante = parseInt($(this).attr("data-restantes"));

        var name = $(this).attr('name');
        var indice = name.match(/\[(\d+)\]/)[1];

        var ntfid = $('#ntfid').val();
        var encid = $(this).attr('data-encid');
        var element = $(this);

        var quantidadeTotalItensNota = 0;
        var valorTotalSelecaoItens = 0;

        $('input[name="vlritensnota['+indice+']').val("0,00");

        if (quantidadeInformada > quantidadeTotalRestante) {
            swal("Alerta", "O valor informado não pode ser maior que a quantidade de itens cadastrada.", "warning");
            $(this).val("");
            return;
        } else {
            var valorTotalCalculado = floatParaMoeda((quantidadeInformada * valorReferencia)); //função criada em contrato.php
            $('input[name="vlritensnota['+indice+']').val(valorTotalCalculado);
        }

        //calcula quantidade e valor total
        $('input[name^="qtditensnota"]').each(function(i, v) {

            var quantidadeItensNota = $(this).val();

            if ("" != quantidadeItensNota) {
                var name = $(this).attr('name');
                var eciid = name.match(/\[(\d+)\]/)[1];
                var valorTotalItensPago = moedaParaFloat($('input[name="vlritensnota['+eciid+']').val()); //moedaparafloat: contrato.php
                quantidadeTotalItensNota = parseInt(quantidadeTotalItensNota) + parseInt(quantidadeItensNota);
                valorTotalSelecaoItens = (parseFloat(valorTotalSelecaoItens)+parseFloat(valorTotalItensPago)).toFixed(2);
            }
        });

        valorTotalCalculado = moedaParaFloat(valorTotalCalculado);

        //salvar item
        $.post(window.location.href, {requisicao: "salvaritemnf", funcao: 'notasfiscais', encid: encid, eciid: indice, ntfid: ntfid, encqtditens: quantidadeInformada, encvaloritens: valorTotalCalculado, qtdtotal: quantidadeTotalItensNota}, function (resp) {

            var dados = $.parseJSON(resp);

            //atualizar id do item
            $(element).attr('data-encid',dados.encid);

            //atualizar campo valor total dos itens
            $('#valortotalitensnf').val(floatParaMoeda(dados.valortotalnf)); //floatparamoeda: contrato.php

            //atualizar coluna 'Itens sem pagamento'
            var qtdmax = '';
            $(element).parent().parent().find('td:nth-child(3)').text(function(){
                var qtd = $(element).attr('data-restantes');
                qtdmax = qtd - quantidadeInformada;
                $(this).text(qtdmax);
            });

            //mensagem da diferença de valor da nota para valor dos itens informados
            var ntfvlrnota = moedaParaFloat($('#ntfvlrnota').val());
            if (parseFloat(parseFloat(ntfvlrnota).toFixed(2)) !== parseFloat(parseFloat(valorTotalSelecaoItens).toFixed(2))) {
                $('#msgvalornota').css('display','block');
            }else{
                $('#msgvalornota').css('display','none');
            }

            //botão de detalhamento - aparece apenas para itens do tipo Veículo (intoid = 4)
            //if(dados.tipoitem == 4){
                //$(element).parent().parent().find('td:nth-child(9)').html('<button class="visualizar_item btn btn-success btn-sm" type="button" onclick="detalharItem('+dados.encid+')"> Detalhar </button>');
            //}

        });

    });

    $(document).on('change', '#ntfcnpjfornecedor', function () {

        if($('#ntfid').val() != ''){
            return false;
        }

        $("#ntfrazaosocialfornecedor").val("");
        $("#ntfuffornecedor").val("");
        $("#contratos_fornecedor").html("");

        if ("" != $('#ntfcnpjfornecedor').val() && 18 != $('#ntfcnpjfornecedor').length) {
            var cnpj = str_replace(['.', ',', '/', '-'], [''], $('#ntfcnpjfornecedor').val());

            $.post(window.location.href, {requisicao: 'getPessoaJuridica', cnpj: cnpj}, function (data) {
                var jsonData = jQuery.parseJSON(data);
                if (jsonData.sucesso == false) {
                    $('#ntfcnpjfornecedor').val("");
                    swal("Alerta", jsonData.mensagem, "warning");
                } else {
//                    listarContratosPorFornecedor(cnpj);
                    $("#ntfrazaosocialfornecedor").val(jsonData.empresa.no_empresarial_rf);
                    $("#ntfuffornecedor").val(jsonData.empresa.sg_uf);
                    $("#ntfnumeronotafiscal").removeAttr('readOnly');
                    $("#ntfdata").removeAttr('readOnly');
                    $("#ntfvlrnota").removeAttr('readOnly');
                }
            });
        }
    });

    //salvar nota fiscal
    $("#salvarNotaFiscal").click(function(event) {
        event.preventDefault();

        var notaFiscalContratoItens = [];

        var files = $('#arquivo')[0].files[0];
        var ntfid = $('#ntfid').val();
        var proid = $('#proid').val();
        var ntfcnpjfornecedor = $('#ntfcnpjfornecedor').val();
        var ntfrazaosocialfornecedor = $('#ntfrazaosocialfornecedor').val();
        var ntfuffornecedor = $('#ntfuffornecedor').val();
        var ntfnumeronotafiscal = $('#ntfnumeronotafiscal').val();
        var ntfdata = $('#ntfdata').val();
        var ntfvlrnota = moedaParaFloat($('#ntfvlrnota').val());
        var quantidadeTotalItensNota = 0;
        var arqid = $('#arqid').val();
        var formData = new FormData();
        var erro = false;
        var campoValorTotal = $('#valortotalitensnf');

        $('form.form_salvar_nota :input[required]').each(function(){
            if($(this).val() == ''){
                if(!$(this).closest('.form-group').hasClass('has-error')) {
                    $(this).closest('.form-group').addClass('has-error');
                    $(this).after("<div class='msgerro'><span class='danger'>O campo não pode ser vazio</span></div>");
                }
                erro = true;
            }
        });

        if($('#arquivo').val() == '' && $('#listaAnexoNotaFiscal').html() == ''){
            swal("Alerta", "Por favor, anexe o arquivo do contrato!", "warning");
            return false;
        }

        if (erro) {
            return false;
        }

        if(campoValorTotal.length > 0) {
            if (ntfvlrnota !== moedaParaFloat(campoValorTotal.val())) {
                $('#msgvalornota').css('display', 'block');
            } else {
                $('#msgvalornota').css('display', 'none');
            }
        }

        var contratoItens = {'contratoItens': notaFiscalContratoItens};

        formData.append('file', files);
        formData.append('ntfid', ntfid);
        formData.append('ntfcnpjfornecedor', ntfcnpjfornecedor);
        formData.append('ntfrazaosocialfornecedor', ntfrazaosocialfornecedor);
        formData.append('ntfuffornecedor', ntfuffornecedor);
        formData.append('ntfnumeronotafiscal', ntfnumeronotafiscal);
        formData.append('ntfdata', ntfdata);
        formData.append('ntfvlrnota', ntfvlrnota);
        formData.append('requisicao', 'salvarNotaFiscal');
        formData.append('funcao', 'notasfiscais');

        jQuery.ajax({
            url: window.location.href,
            data: formData,
            cache: false,
            contentType: false,
            processData: false,
            method: 'POST',
            type: 'POST',
            success: function(ntfid){
                if(parseInt(ntfid) > 0) {
                    swal("Sucesso", "Nota fiscal foi cadastrada com sucesso!", "success");
                    $('#ntfid').val(ntfid);
                    listarContratosPorFornecedor(ntfcnpjfornecedor, ntfid);
                    atualizarListaNF();
                } else if (trim(ntfid) === 'filetype') {
                    swal("Falha", "Por favor, escolha arquivos em formato PDF! ", "error");
                } else {
                    swal("Falha", "Falha ao cadastrar nota fiscal", "error");
                }
            }
        });
    });

    //preenche todos os itens dentro de um accordion
    function incluirTodosItensNF(element){
        var ecoid = $(element).attr('data-ecoid');
        var ntfid = $('#ntfid').val();
        var item = [];
        var valortotal = 0;

        $('#resultlistaItensContratoNotaFiscal' + ecoid + ' input[name^="qtditensnota"]').each(function(){
            if(!$(this).attr('disabled')) {
                var encqtditens = $(this).attr('data-restantes');
                $(this).val(encqtditens);
                var name = $(this).attr('name');
                var eciid = name.match(/\[(\d+)\]/)[1];
                var encid = $(this).attr('data-encid');
                var encvaloritens = $(this).attr('data-ipivalorreferencia') * encqtditens;
                $(this).parent().parent().find('input[name^="vlritensnota"]').val(mascaraglobal('###.###.###.###,##',encvaloritens.toFixed(2)));
                valortotal += encvaloritens;
                //atualizar coluna qtd sem contrato
                $(this).parent().parent().find('td:nth-child(4)').text('0');

                item.push({
                    'ntfid': ntfid,
                    'eciid': eciid,
                    'encqtditens': encqtditens,
                    'encvaloritens': encvaloritens,
                    'encid': encid
                });
            }
        });

        $.post(window.location.href, {requisicao: "salvarArrayItemNF", funcao: 'notasfiscais', item:item}, function (resp) {

            if(trim(resp) === 'sucesso') {
                $('#valortotalitensnf').val(floatParaMoeda(valortotal));

                if (valortotal != moedaParaFloat($('#ntfvlrnota').val())) {
                    $('#msgvalornota').css('display', 'block');
                } else {
                    $('#msgvalornota').css('display', 'none');
                }
            }else{
                swal("Falha", "Falha ao salvar itens", "error");
            }


        });

    }

    //utilizado também no arquivo _edit_contrato
    function limparFormNF() {
        $('#arquivo').val('');
        $('#ntfid').val('');
        $("#ntfcnpjfornecedor").val('').prop('disabled', false).trigger("chosen:updated");
        $('#ntfrazaosocialfornecedor').val('');
        $('#ntfuffornecedor').val('');
        $('#ntfnumeronotafiscal').val('').attr('readonly','readonly');
        $('#ntfdata').val('').attr('readonly','readonly');
        $('#ntfvlrnota').val('').attr('readonly','readonly');
        $("#contratos_fornecedor").html("");
        $('#listaAnexoNotaFiscal').html('');
        $('#msgvalornota').css('display','none');

        //remove marcações de erro do formulário
        $('#edit_nf .has-error').removeClass('has-error');
        $('#edit_nf .msgerro').remove();

        habilitarInputs();
    }

    function listarContratosPorFornecedor(cnpj, ntfid = null) {
        $.post(window.location.href, {requisicao: 'listarContratosPorFornecedor', funcao: 'notasfiscais', cnpj: cnpj, ntfid: ntfid}, function (data) {
            $("#contratos_fornecedor").html(data);
        });
    }

    function excluirDocumentoNF(arqid){
        swal({
            title: " Remover Documento Anexo!",
            text: "Tem certeza que deseja remover o Documento Anexo? (Esta ação surtirá efeito apenas ao salvar a nota fiscal)",
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

</script>
