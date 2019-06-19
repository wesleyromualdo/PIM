function salvarAreaRelacionada() {
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas';
    var action  = '&reqarearelacionada=salvar&' + $("#form-arearelacionada").serialize();
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            var res = $.parseJSON(resp);
            if (!isNaN(parseInt(res)) && isFinite(res)) {
                msgSuccess(caminho+"&irrid=","Area Relacionada Salva com sucesso");
                return false;
            }
            swal.close();
            $.each(res, function (index, erro) {//retorna mensagens de erro em json
                $.each(erro, function (err, errMsg) {
                    if (errMsg < err.length) {
                        return;//se a mensagem for vazia não retorna nada
                    }
                    var divFormInput = $("#form-arearelacionada").find("#" + err).parent("div");
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

function vicularProgramas() {
    var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas';
    var action  = '&req=salvar&' + $("#form-arearelacionada").serialize();
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            var res = $.parseJSON(resp);
            if (!isNaN(parseInt(res)) && isFinite(res)) {
                var iarid = $('#iarid').val();
                if(iarid){
                    iarid = '&iarid='+iarid;
                }
                msgSuccess(caminho+iarid,"Programa Vinculado com sucesso.");
                pesquisarAreaRelacionada();
                return false;
            }
            swal.close();
            $.each(res, function (index, erro) {//retorna mensagens de erro em json
                $.each(erro, function (err, errMsg) {
                    if (errMsg < err.length) {
                        return;//se a mensagem for vazia não retorna nada
                    }
                    var divFormInput = $("#form-arearelacionada").find("#" + err).parent("div");
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

function desvincularPrograma(id) {
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas';
    var action  = 'req=recuperarprograma&iapid=' + id;
    $.ajax({
        type: "GET",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            var iap = $.parseJSON(resp);
            swal({
                title: "Desvincular Programa",
                text: "Tem certeza que deseja Desvincular o Programa<br/><center><b>"+iap.prgabreviatura+'<br/>'+iap.prgdsc+" ?</br></center>",
                html: true,
                type: "warning",
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas';
                    var action = '&req=desvincular&iapid=' + id;
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            if (resp != 'erro') {
                                swal('Sucesso','Programa desvinculado com sucesso','success');
                                pesquisarAreaRelacionada();
                                return false;
                            }
                            swal("Erro.", "Ocorreu um erro ao desvincular Item.", "error");
                        }
                    });
                }
            });
        }
    });
}

function pesquisarAreaRelacionada() {
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas';
    var action  = '&req=pesquisar&' + $("#form-arearelacionada").serialize();
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#listaprogramasvinculados").html(resp);
            return true;
        }
    });
}

function vincularEstadoDocumento(esdid,iapid){
    var caminho = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas';
    var action  = '&req=marcaropcao&iapid=' +iapid+'&esdid='+esdid;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            var res = $.parseJSON(resp);
            if (!isNaN(parseInt(res)) && isFinite(res)) {
                return false;
            }
        }
    });
    return false;
}