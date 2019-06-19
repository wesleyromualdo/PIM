

<div class="ibox float-e-margins">
    <div class="ibox-title text-center">
        <h2>
            <i class="fa fa-list-alt" aria-hidden="true"></i> Lista de Notas Fiscais
        </h2>
    </div>

    <div class="ibox-content" id="listaNotasFiscais">


        <div id="listagemNotasFiscais">

            <?php
                // $controllerExecucaoNotaFiscal em contrato.php
                $controllerExecucaoNotaFiscal->listaNotasFiscais($proid);
            ?>

        </div>
    </div>
</div>

<script>

    function editarNotaFiscal(id){

        $.post(window.location.href, {requisicao: 'editarNotaFiscal', funcao: 'notasfiscais', ntfid: id}, function (resp) {

            $('a[href="#add_nota_fiscal"]').tab('show');

            var dados = $.parseJSON(resp);

            if(dados.ntfstatus == 'I'){
                desabilitarInputsNF();
            }else{
                habilitarInputsNF();
            }

            $('#ntfid').val(dados.ntfid);
            $('#ntfcnpjfornecedor').val(dados.ntfcnpjfornecedor).trigger("chosen:updated");
            $('#ntfnumeronotafiscal').val(dados.ntfnumeronotafiscal).attr('readonly',false);
            $('#ntfdata').val(dados.ntfdata).attr('readonly',false);
            $('#ntfrazaosocialfornecedor').val(dados.ntfrazaosocialfornecedor);
            $('#ntfuffornecedor').val(dados.ntfuffornecedor);
            $('#ntfvlrnota').val(mascaraglobal('###.###.###.###,##',dados.ntfvlrnota)).attr('readonly',false);

            var divArquivo = "<span class='col-lg-2'></span><div class='col-lg-10' style='padding-top: 5px' id='arquivo_" + dados.arqid + "'>" +
                "<div class='MultiFile-label'></div>" +
                "<a class='MultiFile-remove' href='#anexo'>" +
                "<a onclick='excluirDocumentoNF(" + dados.arqid + ")' class='btn btn-danger btn-xs' title='Excluir'" +
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

            $('#listaAnexoNotaFiscal').html(divArquivo);
            $('#arqid').val(dados.arqid);

            //verifica valor dos itens com falor da nota
            if(dados.valortotalitens != dados.ntfvlrnota){
                $('#msgvalornota').css('display','block');
            }else{
                $('#msgvalornota').css('display','none');
            }

            $("#contratos_fornecedor").html(dados.listaContratos);
        });
    }

    function removerNotaFiscal(ntfid){
        swal({
            title: " Excluir",
            text: "Tem certeza que deseja remover a nota selecionada?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
                $.post(window.location.href, {requisicao: "removerNotaFiscal",funcao: 'notasfiscais', ntfid: ntfid}, function (resp) {
                    if(trim(resp) === 'falha'){
                        swal("Falha", "Erro ao excluir, tente novamente mais tarde.", "error");
                        return false;
                    }else{
                        atualizarListaNF();
                    }
                });
            }
        });
    }

    function atualizarListaNF(){
        $.post(window.location.href, {requisicao: 'listarNotasFiscais', funcao: 'notasfiscais'}, function (data) {
            $("#listagemNotasFiscais").html(data);
        });
    }

    function downloadNotaFiscal(id, arqid){
        window.location.href = window.location.href+'&reqdownload=download&arqid=' + arqid;
    }

    function desabilitarInputsNF(){
        $('#edit_nf input.form-control').prop('disabled',true);
        $('#ntfcnpjfornecedor').prop('disabled', 'disabled').trigger("chosen:updated");
        $('#salvarNotaFiscal').hide();
    }
    function habilitarInputsNF(){
        $('#edit_nf #arquivo').prop('disabled',false);
        $('#ntfnumeronotafiscal').prop('disabled',false);
        $('#ntfdata').prop('disabled',false);
        $('#ntfvlrnota').prop('disabled',false);
        $('#salvarNotaFiscal').show();
    }
</script>