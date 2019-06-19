function form(id = null) {
    var iarid = '';
    if (id) {
        iarid = '&iarid=' + id;
        $("#modal_titulo").html("Área - Editar");
    }

    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
    var action = '&req=form' + iarid;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#conteudo-modal").html(resp);
            $("#modal_area").modal("show");
            $('#salvarArea').removeAttr('disabled');
        }
    });
}

function salvarArea() {
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
    var action = '&req=salvarArea&' + $("#formArea").serialize()+'&'+$('#selectUnidadeOrcamentaria').serialize();
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            var res = $.parseJSON(resp);
            if (!isNaN(parseInt(res)) && isFinite(res)) {
                $("#modal_area").modal("hide");
                $("#modal_area").find("input").val("");
                swal({
                    title: "Sucesso.",
                    text: "Área salva com sucesso.",
                    type: "success",
                    },
                    function(isConfirm){
                        if(isConfirm){
                            window.location.href = window.location.href;
                        }
                    });
                return false;
            }
            swal.close();
            $.each(res, function (index, erro) {//retorna mensagens de erro em json
                $.each(erro, function (err, errMsg) {
                    if (errMsg < err.length) {
                        return;//se a mensagem for vazia não retorna nada
                    }
                    if(err == 'iuoid'){
                        var iuoidlabel = $("#formArea").find("#iuoidlabel");
                        iuoidlabel.addClass('has-error');
                        iuoidlabel.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                        iuoidlabel.show();
                    }
                    var divFormInput = $("#modal_area").find("input[name=" + err + "]").parent("div");
                    divFormInput.closest('.form-group').addClass('has-error');
                    divFormInput.append("<div id='div-" + err + "'></div>");
                    $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                });
            });
            return false;
        }
    });
    return false;
}

function removerArea(id) {
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
    var action  = '&req=recuperar&iarid=' + id;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            var des = $.parseJSON(resp);
            var confirmado = false;
            swal({
                title: "Remover Area",
                text: "Tem certeza que deseja remover: <b>" + des.iarid + " - " + des.iardsc + "</b> ?",
                html: true,
                type: "warning",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
                    var action  = '&req=removerArea&iarid=' + id;
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if (!isNaN(parseInt(resp)) && isFinite(resp)) {
                                $("#modal_area").modal("hide");
                                $("#modal_area").find("input").val("");
                                swal("Sucesso.", "Area foi removido com sucesso.", "success");
                                atualizarListagemArea();
                                return false;
                            }
                            swal("Erro.",resp, "error");
                            return false;
                        }
                    });
                }
            })
        }
    });
}

function editarArea(id) {
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
    var action  = '&req=form&iarid=' + id;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#conteudo-modal").find("input").val("");
            $("#conteudo-modal").html(resp);
            $("#modal_titulo").html("Area - Editar");
            $("#modal_area").modal("show");
        }
    });
}

function atualizarListagemArea(params = '') {
    if (params) {
        params = '&' + params;
    }
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areas';
    var action  = '&req=atualizarListagemArea' + params;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (form) {
            $('#areaListagem').html(form);
            return false;
        }
    });
}