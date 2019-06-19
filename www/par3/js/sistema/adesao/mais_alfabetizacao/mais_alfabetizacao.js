$(document).ready(function () {

    $('#adpano_ciclo').change(function () {
        var inuid = $('#inuid').val();
        var prgid = $('#prgid').val();

        var ano = $(this).val();
        var link = 'par3.php?modulo=principal/adesao/termo&acao=A';
        var campos = '&prgid=' + prgid + '&inuid=' + inuid + '&adpano_ciclo=' + ano;
        window.location.href = link + campos;
    });

    $('#modal-form').on('hidden.bs.modal', function () {
        var esdid = $("#esdid_atual").val();
        if ($('#grupoPopUp').val() != 'N' && ((esdid != 1815) && (esdid != 1817))) {
            if (!confirm('Você está saindo da tela de vinculação de escolas, caso não tenha salvado, as informações preenchidas serão desconsideradas. Deseja continuar?')) {
                $('#modal-form').modal();
            }
        }
    });

    $('#formulario-coordenador').submit(function () {

        if (validateEmail($('#cmaemail').val())) {
            return true;
        }
        else {
            alert('E-mail inválido. Insira um endereço de E-mail válido');
        }
        return false;
    });

    var abaAtual = $("#aba_atual").val();

    if (abaAtual != '') {
        if (abaAtual == 2) {
            $("#tab-2").addClass("active");
            $("#li2").addClass("active");
        } else if (abaAtual == 3) {
            $("#tab-3").addClass("active");
            $("#li3").addClass("active");
        } else if (abaAtual == 4) {
            $("#tab-4").addClass("active");
            $("#li4").addClass("active");
        }
        $("#tab-1").removeClass("active");
        $("#li1").removeClass("active");
    }

    $('.btn-xls').hide();
    $('.btn-query').hide();

    $('.ibox').on('change', 'input[name=cmacpf]', function () {
        if (!validar_cpf($(this).val())) {
            alert("CPF inválido!\nFavor informar um cpf válido!");
            $(this).val('');
            $(this).parent().parent().find('input[name=cmanome]').val('');
            $(this).parent().parent().find("label").html('');
            return false;
        }
        var param = new Array();
        param.push({name: 'requisicao', value: 'verifica_cpf'},
            {name: 'cpf', value: $(this).val()});

        var t = $(this);

        $.ajax({
            type: "POST",
            dataType: "json",
            url: window.location.href,
            data: param,
            success: function (data) {

                if (data[0].origem == "instrumentounidade_entidade") {
                    var locais = [];
                    for (i = 0; data.length > i; i++) {
                        locais.push(data[i].inudescricao);
                    }

                    var lista_locais = locais.join(',');

                    swal({
                            title: "Você tem certeza?",
                            text: "O nutricionista <b>(" + data[0].usunome + ")</b> selecionado está vinculado aos município(s): <b>" + lista_locais + "</b>. Deseja fazer o cadastro dele em um novo município!",
                            type: "warning", showCancelButton: true,
                            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                            closeOnConfirm: "on",
                            cancelButtonText: "Cancelar",
                            html: true
                        },
                        function (isConfirm) {
                            if (isConfirm) {
                                t.closest(".ibox-content").find('input[name=cmanome]').val(data[0].usunome);
                                t.closest(".ibox-content").find('input[name=cmaemail]').val(data[0].cmaemail);
                            }
                            else {
                                t.closest(".ibox-content").find('input[name=cmacpf]').val("");
                                t.closest(".ibox-content").find('input[name=cmaemail]').val("");
                                t.closest(".ibox-content").find('input[name=cmanome]').val("");
                            }
                        });

                }
                if (data[0].origem == "receita") {
                    t.closest(".ibox-content").find('input[name=cmanome]').val(data[0].usunome);
                    t.closest(".ibox-content").find('input[name=cmaemail]').val("");
                    t.closest(".ibox-content").find('input[name=cmatelefone]').val("");
                }
                if (data[0].origem == "seguranca") {
                    t.closest(".ibox-content").find('input[name=cmanome]').val(data[0].usunome);
                    t.closest(".ibox-content").find('input[name=cmaemail]').val(data[0].cmaemail);
                    t.closest(".ibox-content").find('input[name=cmatelefone]').val("");
                }
            },
            error: function (data) {
                t.closest(".ibox-content").find('input[name=cmanome]').val("");
                t.closest(".ibox-content").find('input[name=cmaemail]').val("");
                t.closest(".ibox-content").find('input[name=cmatelefone]').val("");
            }
        });
    });
});

function validateEmail($email) {
    var emailReg = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
    return emailReg.test($email);
}

function gerarPDFSinteseAlfabetizacao() {
    $('#conteudo_pdf').val($("#conteudo_sintese").html());

    $("#formulario-pdf-sintese").submit();

}

function submeteFormEscolasAlfabetizacao() {
    var submete = true;
    if ($("#formulario_escolas input:checkbox:checked").length == 0) {
        alert('Selecione no mínimo uma escola.');
        submete = false;
    }
    if (submete) {
        $("#formulario_escolas").submit();
    }
}

function filtraPesquisaAlfabetizacao(inuid, adpid, adpciclo_ano, grupo, prgid, tipoPesquisa) {

    var continua = true;
    var titulo = 'Vincular Escolas';

    if (continua) {
        var emamunicipio = ($("#emamunicipio").val()) ? $("#emamunicipio").val() : '';
        var emarede = ($("#emarede").val()) ? $("#emarede").val() : '';
        var emacod = ($("#emacod").val()) ? $("#emacod").val() : '';
        var emanome = ($("#emanome").val()) ? $("#emanome").val() : '';
        var checado = ($("#checado").val()) ? $("#checado").val() : '';
        var carga_horaria = ($("#carga_horaria").val()) ? $("#carga_horaria").val() : '';
        var ordenacao = ($('#ordenacao').val()) ? $('#ordenacao').val() : '';
        var adpano_ciclo = $('#adpano_ciclo').val();


        if (tipoPesquisa == 'xls') {
            link = 'par3.php?modulo=principal/adesao/termos/mais_alfabetizacao/2018/popup_alfabetizacao&acao=A' + '&grupo=' + '&grupo=' + grupo +
                '&inuid=' + inuid + '&adpid=' + adpid + '&adpano_ciclo=' + adpano_ciclo + '&prgid=' + prgid +
                '&emamunicipio=' + emamunicipio + '&emarede=' + emarede + '&emacod=' + emacod + '&emanome=' + emanome +
                '&tipopesquisa=' + tipoPesquisa + '&carga_horaria=' + carga_horaria + '&checado=' + checado + '&ordenacao=' + ordenacao;
            location.href = link;
            return false;
        }
        else {
            $.ajax({
                type: "POST",
                url: 'par3.php?modulo=principal/adesao/termos/mais_alfabetizacao/2018/popup_alfabetizacao&acao=A',
                data: '&grupo=' + grupo + '&inuid=' + inuid + '&adpid=' + adpid +
                '&adpano_ciclo=' + adpano_ciclo + '&prgid=' + prgid + '&emamunicipio=' + emamunicipio +
                '&emarede=' + emarede + '&emacod=' + emacod + '&emanome=' + emanome + '&tipopesquisa=' + tipoPesquisa +
                '&carga_horaria=' + carga_horaria + '&checado=' + checado + '&ordenacao=' + ordenacao,
                async: false,
                success: function (resp) {
                    $('#html_modal-form').html(resp);
                    $('#html_modal-title').html(titulo);
                    $('#modal-form').modal();
                }
            });
        }
    }
}

function retornaTooltipPorGrupo(grupo) {
    var title = new Array();
    title['1'] = 'Escolas com Índice de Desenvolvimento da Educação Básica (IDEB) inferior a 4.4 nos anos iniciais e inferior a 3.0 nos anos finais, concomitantemente.';
    title['2'] = 'Escolas com IDEB inferior a 4.4 nos anos iniciais OU IDEB inferior a 3.0 nos anos finais.';
    title['3'] = 'Escolas com mais de 50% dos alunos oriundos de famílias beneficiárias do Programa Bolsa Família e que não se enquadrarem nos critérios anteriores.';

    return title[grupo];
}

function insereExcecaoEscolasAlfabetizacao(inuid, adpid, prgid) {

    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/termos/mais_alfabetizacao/2018/popupIncluirEscolasExcecao&acao=A',
        data: '&inuid=' + inuid + '&adpid=' + adpid + '&prgid=' + prgid,
        async: false,
        success: function (resp) {
            $('#html_modal-form2').html(resp);
            $('#html_modal-title2').html('Solicitação de inclusão de escolas na adesão');
            $('#modal-form2').modal();
        }
    });
}

function popupEscolasAlfabetizacao(inuid, adpid, carga_horaria, prgid) {

    var titulo = 'Vincular Escolas';

    var adpano_ciclo = $('#adpano_ciclo').val();
    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/termos/mais_alfabetizacao/2018/popup_alfabetizacao&acao=A',
        data: '&carga_horaria=' + carga_horaria + '&inuid=' + inuid + '&adpid=' + adpid + '&prgid=' + prgid + '&adpano_ciclo=' + adpano_ciclo,
        async: false,
        success: function (resp) {
            $('#html_modal-form').html(resp);
            $('#html_modal-title').html(titulo);
            $('#modal-form').modal();
        }
    });
}

function descadastrarCoordenadorAlfabetizacao(link) {
    if (confirm('Tem certeza que deseja remover o Coordenador?')) {
        location.href = link;
    }
}

function confirmaAdesaoAlfabetizacao(decisao, link) {
    if (decisao == 'C') {
        if (confirm('Tem certeza que deseja voltar para cadastramento?')) {
            location.href = link;
        }
    }
    if (decisao == 'E') {
        if (confirm('Após confirmação de envio, não será possível realizar alterações no seu processo de adesão ao Programa. Confirma a ação?')) {
            location.href = link;
        }

    }
    if (decisao == 'S') {
        location.href = link;
    }
    if (decisao == 'N') {
        //@todo apagar os dados dos formulários
        if (confirm('Ao escolher a opção de não aceitar a adesão, caso já tenha respondido a algum formulário, os dados serão perdidos. Confirma a ação?')) {
            location.href = link;
        }
    }
}

function fMasc(objeto,mascara) {
    obj=objeto
    masc=mascara
    setTimeout("fMascEx()",1)
}
function fMascEx() {
    obj.value=masc(obj.value)
}
function mTel(tel) {
    tel=tel.replace(/\D/g,"")
    tel=tel.replace(/^(\d)/,"($1")
    tel=tel.replace(/(.{3})(\d)/,"$1)$2")
    if(tel.length == 9) {
        tel=tel.replace(/(.{1})$/,"-$1")
    } else if (tel.length == 10) {
        tel=tel.replace(/(.{2})$/,"-$1")
    } else if (tel.length == 11) {
        tel=tel.replace(/(.{3})$/,"-$1")
    } else if (tel.length == 12) {
        tel=tel.replace(/(.{4})$/,"-$1")
    } else if (tel.length > 12) {
        tel=tel.replace(/(.{4})$/,"-$1")
    }
    return tel;
}

function mCPF(cpf){
    cpf=cpf.replace(/\D/g,"")
    cpf=cpf.replace(/(\d{3})(\d)/,"$1.$2")
    cpf=cpf.replace(/(\d{3})(\d)/,"$1.$2")
    cpf=cpf.replace(/(\d{3})(\d{1,2})$/,"$1-$2")
    return cpf
}