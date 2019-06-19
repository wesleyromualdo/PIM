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
        var apta = $("#apta").val();
        if(apta == 'true'){
            if ($('#grupoPopUp').val() != 'N' && ((esdid != 2180) && (esdid != 2181))) {
                if (!confirm('Você está saindo da tela de vinculação de escolas, caso não tenha salvado, as informações preenchidas serão desconsideradas. Deseja continuar?')) {
                    $('#modal-form').modal();
                }
            }
        }
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

});

function validateEmail($email) {
    var emailReg = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
    return emailReg.test($email);
}

function gerarPDFSinteseEscolaAcessivel() {
    $('#conteudo_pdf').val($("#conteudo_sintese").html());

    $("#formulario-pdf-sintese").submit();

}

function submeteFormEscolaAcessivel() {
    var submete = true;
    if ($("#formulario_escolas input:checkbox:checked").length == 0) {
        alert('Selecione no mínimo uma escola.');
        submete = false;
    }
    if (submete) {
        $(".salvar_escolas").attr("disabled", true);
        $("#formulario_escolas").submit();
    }
}

function filtraPesquisaEscolaAcessivel(inuid, adpid, grupo, prgid, tipoPesquisa) {

    var continua = true;
    var titulo = 'Vincular Escolas';

    if (continua) {
        var eeamunicipio = ($("#eeamunicipio").val()) ? $("#eeamunicipio").val() : '';
        var eearede = ($("#eearede").val()) ? $("#eearede").val() : '';
        var eeacod = ($("#eeacod").val()) ? $("#eeacod").val() : '';
        var eeanome = ($("#eeanome").val()) ? $("#eeanome").val() : '';
        var checado = ($("#checado").val()) ? $("#checado").val() : '';
        var apta = ($("#apta").val()) ? $("#apta").val() : '';
        var ordenacao = ($('#ordenacao').val()) ? $('#ordenacao').val() : '';
        var adpano_ciclo = $('#adpano_ciclo').val();
        
        if($('#selecionada').is(':checked')) {
            var eeaselecionada = $("#selecionada").val();
        }
        if($('#naoselecionada').is(':checked')) {
            var eeaselecionada = $("#naoselecionada").val();
        }
        
        if(apta == 'true'){
            $('#exibe_valor_programa').show();
            $('#exibe_filtro_escola_selecionada').show();
        } else {
            setTimeout(function(){
               $('#exibe_valor_programa').hide();
               $('#exibe_filtro_escola_selecionada').hide();
            }, 200);
        }

        if (tipoPesquisa == 'xls') {
            link = 'par3.php?modulo=principal/adesao/termos/escola_acessivel/popup_escola_acessivel&acao=A' + '&grupo=' + '&grupo=' + grupo +
                '&inuid=' + inuid + '&adpid=' + adpid + '&adpano_ciclo=' + adpano_ciclo + '&prgid=' + prgid +
                '&eeamunicipio=' + eeamunicipio + '&eearede=' + eearede + '&eeacod=' + eeacod + '&eeanome=' + eeanome +
                '&tipopesquisa=' + tipoPesquisa + 
                '&apta=' + apta + 
                '&checado=' + checado + '&ordenacao=' + ordenacao + '&eeaselecionada=' + eeaselecionada;
            location.href = link;
            return false;
        }
        else {
            $.ajax({
                type: "POST",
                url: 'par3.php?modulo=principal/adesao/termos/escola_acessivel/popup_escola_acessivel&acao=A',
                data: '&grupo=' + grupo + '&inuid=' + inuid + '&adpid=' + adpid +
                '&adpano_ciclo=' + adpano_ciclo + '&prgid=' + prgid + '&eeamunicipio=' + eeamunicipio +
                '&eearede=' + eearede + '&eeacod=' + eeacod + '&eeanome=' + eeanome + '&tipopesquisa=' + tipoPesquisa +
                '&apta=' + apta + 
                '&checado=' + checado + '&ordenacao=' + ordenacao + '&eeaselecionada=' + eeaselecionada,
                async: false,
                success: function (resp) {
//                    console.log('filtraPesquisaEscolaAcessivel:', resp);
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

function insereExcecaoEscolaAcessivel(inuid, adpid, prgid) {

    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/termos/escola_acessivel/popupIncluirEscolasExcecao&acao=A',
        data: '&inuid=' + inuid + '&adpid=' + adpid + '&prgid=' + prgid,
        async: false,
        success: function (resp) {
            $('#html_modal-form2').html(resp);
            $('#html_modal-title2').html('Solicitação de inclusão de escolas na adesão');
            $('#modal-form2').modal();
        }
    });
}

function popupEscolaAcessivel(inuid, adpid, apta, prgid) {
        
    var titulo = '';
    
    var adpano_ciclo = $('#adpano_ciclo').val();
    
    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/termos/escola_acessivel/popup_escola_acessivel&acao=A',
        data: '&apta=' + apta + '&inuid=' + inuid + '&adpid=' + adpid + '&prgid=' + prgid + '&adpano_ciclo=' + adpano_ciclo,
        async: true,
        beforeSend: function () {
            $('.loading-dialog-par3').show();
        },
        success: function (resp) {
            
            if(apta == 'true'){
                titulo = 'Vincular escolas aptas';
                $('#exibe_valor_programa').show();
                $('#exibe_filtro_escola_selecionada').show();
                $('#btn-desmarcar-todos').show();
                
            } else {
                titulo = 'Escolas não aptas';
                setTimeout(function(){
                   $('#exibe_valor_programa').hide();
                   $('#exibe_filtro_escola_selecionada').hide();
                   $('#btn-desmarcar-todos').hide();
                }, 200);
            }
            
            $('#html_modal-form').html(resp);
            $('#html_modal-title').html(titulo);
            $('#modal-form').modal();
            $('.loading-dialog-par3').hide();
        },
        complete : function() {
            $('.loading-dialog-par3').hide();
        },
        error : function () {
            $('.loading-dialog-par3').hide();
            $(document).find('#loading').hide();
            alert('Ocorreu um erro ao carregar as escolas.');
        }
    });
}

function confirmaAdesaoEscolaAcessivel(decisao, link) {
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

function pegarValorDestinado(elem){
    //console.log(elem);
    //var dadosForm = $('#formulario_escolas').serialize();
    var itemChecado = $(elem).attr("id");    
    var valor_saldo_disponivel = $('#valor_saldo_disponivel').val();
    var valor_disponivel_uf = $('#valor_disponivel_uf').val();
    var valor_disponivel_mun = $('#valor_disponivel_mun').val();
    //var escola_checada = $(elem).attr("name");
    //console.log(escola_checada);    
    var array_escola_checada = [];
    $('.escola_input:checked').each(function() {
        array_escola_checada.push($(this).val());
    });
    //console.log(array_escola_checada);
    
    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/termos/escola_acessivel/popup_escola_acessivel&acao=A',
        data: {'requisicao': 'valor_escola_checada', 'escola_checada': array_escola_checada, 'valor_saldo_disponivel': valor_saldo_disponivel, 'valor_disponivel_uf': valor_disponivel_uf, 'valor_disponivel_mun' : valor_disponivel_mun},
        async: true,
        dataType: 'html',
        beforeSend: function () {
            $('.loading-dialog-par3').show();
        },
        success: function (resp) {
            //console.log(resp);   
            if(resp !== 'false'){
               $('#saldo-total-programa-formatado').html('R$ ' + resp);
               $('#valor_saldo_disponivel').val(resp);
            } else {                
                swal({
                    title: "Saldo indisponível!",
                    text: "Saldo disponível não é suficiente para contemplar a escola selecionada!",
                    type: "warning", 
//                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55", 
                    confirmButtonText: "Ok",
                    closeOnConfirm: "on",
//                    cancelButtonText: "Cancelar",
                    html: true
                },
                function (isConfirm) {
                    
                });
                
                $('#'+itemChecado).prop('checked', false);
            }
        },
        complete : function() {
            $('.loading-dialog-par3').hide();
        },
        error : function () {
            $('.loading-dialog-par3').hide();
            $(document).find('#loading').hide();
            alert('Ocorreu um erro ao marcar o item.');
        }
    });
    
}