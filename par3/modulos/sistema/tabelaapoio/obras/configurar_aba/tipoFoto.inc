<div class="form-group">
    <div class="col-md-5">
        <span class="help-block m-b-none" style="display: none" id="msgSalvarTipoFoto"></span>
        <br>
    </div>
    <div class="col-md-6" id="inputgroupatfid">
        <div class="col-md-12" id="inputgroupatfid">
            <div class="input-group">
                <input type="hidden" id="atfid" name="atfid" value="<?php echo $_REQUEST['atfid']?>">
                <input type="text" class="form-control" placeholder="Adicione Novo Tipo de Foto" id="atfdsc" name="atfdsc"
                       value="<?php echo $mTipoFoto->atfdsc; ?>">
                <span class="input-group-btn">
                    <button type="button" class="btn btn-primary" id="salvarTipoFoto" title="Salvar TipoFoto">
                        <i class="fa fa-check"></i>
                    </button>
                    <button type="button" class="btn btn-default" id="cancelarTipoFoto" title="Cancelar">
                        <i class="fa fa-times"></i>
                    </button>
                </span>
            </div>
        </div>
    </div>
<script>

    $('#cancelarTipoFoto').click(function () {
        $('#formTipoFoto').fadeOut();
        $('#salvarConf').removeAttr('disabled');
    });

    $('#salvarTipoFoto').click(function (evt) {
        evt.preventDefault();
        $('#salvarConf').removeAttr('disabled');
        swal({
                title: "Confirmar",
                text: "Deseja <b>Salvar</b> este tipo de foto?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarTipoFoto();
                }
                return false;
            }
        );
    });

    function salvarTipoFoto() {
        var caminho = window.location.href;
        var formTipoFoto = $("#formTipoFoto").serialize();
        var action = '&requisicao=salvarTipoFoto&' + formTipoFoto;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    swal("Sucesso.", "Tipo de foto salvo com sucesso.", "success");
                    $('#btn-enviar').removeAttr('disabled');
                    $('#formTipoFoto').fadeOut();
                    $('#btnTipoFoto').addClass();
                    atualizarListagemTipoFoto(formTipoFoto);
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formTipoFoto").find("#inputgroupatfid");
                        divFormInput.addClass('has-error');
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                    });
                });
                return false;
            }
        });
        return false;
    }

    function atualizarListagemTipoFoto(params = '') {

        if(params){
            params = '&' + params;
        }
        var caminho = window.location.href;
        var action = '&requisicao=atualizarListagemTipoFoto'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (form) {
                $('#divSelectTipoFoto').html(form);
                return false;
            }
        });
    }
</script>