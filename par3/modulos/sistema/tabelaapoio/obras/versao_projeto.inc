
<input type="hidden" name="ovpid" value="<?php echo $mVersaoProjeto->ovpid; ?>">
<div class="form-group">
    <div class="col-lg-offset-3 col-md-9" id="inputgroupovpid">
        <div class="input-group  col-sm-10" style="margin-left: 30px;">
            <input type="hidden" id="ovpid">
            <input type="hidden" id="otpid" name="otpid" value="<?php echo $_REQUEST['otpid']?>">
            <input type="text" class="form-control" placeholder="Adicione novo VersaoProjeto" id="ovpdsc" name="ovpdsc"
                   value="<?php echo $mVersaoProjeto->ovpdsc; ?>">
            <span class="input-group-btn">
                <button type="button" class="btn btn-primary" id="salvarVersaoProjeto" title="Salvar VersaoProjeto">
                    <i class="fa fa-check"></i>
                </button>
                <button type="button" class="btn btn-default" id="cancelarVersaoProjeto" title="Cancelar">
                    <i class="fa fa-times"></i>
                </button>
            </span>
        </div>
        <div class="input-group col-sm-6" style="margin-left: 30px;">
            <input id="ovpmetragem" class="form-control" placeholder="informe a metragem da obra em m²" name="ovpmetragem" value="<?php echo $mVersaoProjeto->ovpmetragem; ?>">
            <span class="input-group-addon"><b>M²</b></span>
        </div>
    </div>
</div>
<div class="col-md-offset-5 col-lg-offset-5">
    <span class="help-block m-b-none" style="display: none" id="msgSalvarVersaoProjeto"></span>
    <br>
</div>
<script>

    $('#cancelarVersaoProjeto').click(function () {
        $('#formVersaoProjeto').fadeOut();
        $('#btn-enviar').removeAttr('disabled');
        $('#input_metragem').show();
        atualizarMetragemProjeto();
    });

    $('#salvarVersaoProjeto').click(function (evt) {
        evt.preventDefault();
        swal({
                title: "Confirmar",
                text: "Deseja <b>Salvar</b> esta Versão de Projeto?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarVersaoProjeto();
                    $('#input_metragem').show();
                    atualizarMetragemProjeto();
                }
                return false;
            }
        );
    });

    function salvarVersaoProjeto() {
        var caminho = window.location.href;
        var formVersaoProjeto = $("#formVersaoProjeto").serialize();
        var action = '&requisicao=salvarVersaoProjeto&' + formVersaoProjeto;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    swal("Sucesso.", "Versão de Projeto salva com sucesso.", "success");
                    $('#btn-enviar').removeAttr('disabled');
                    $('#formVersaoProjeto').fadeOut();
                    $('#btnVersaoProjeto').addClass();
                    atualizarListagemVersaoProjeto(formVersaoProjeto);
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#formVersaoProjeto").find("#inputgroupovpid");
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

    function atualizarListagemVersaoProjeto(params = '') {

        if(params){
            params = '&' + params;
        }
        var caminho = window.location.href;
        var action = '&requisicao=atualizarListagemVersaoProjeto'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (form) {
                $('#selectVerProjeto').html(form);
                return false;
            }
        });
    }

    $(function(){
        $("#ovpmetragem").inputmask('numeric');
    });
</script>