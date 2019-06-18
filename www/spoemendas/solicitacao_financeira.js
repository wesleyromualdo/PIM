$(document).ready(function () {
    toggleComboInteressado();
    $('#formulario_exercicio').val('').trigger('chosen:updated');
    $('#formulario_intid').val('').trigger('chosen:updated');
    $($('.tag span')[1]).click(function () {
        $('#solicitacao_sfnvlrsolicitado').val(0);
    });
});

/**
 * Função responsavel por
 */
 function onEmendasNovo() {
    // Apresenta modal e corrige o tamanho dos inputs [fix chosen]
    // Carrega unidade orçamentária através do usuario logado
    $('#modNovaSolicitacao').modal('show');
    $('#modNovaSolicitacao').find('.chosen-container').css('width', '100%');
}

/**
 * Função responsável por apresentar ou não o campo de interessado
 * @author Victor Eduardo Barreto
 * @return void
 */
 function toggleComboInteressado() {
    var obrigatorio = ($("#solicitacao_interessadoObrigatorio").val() || $('#formulario_interessadoObrigatorio').val()) == 'true';
    var $autor = autor.find(':selected').text();
    if ($autor.indexOf('Relator Geral') == 0 ||
        $autor.includes('Com.') ||
        $autor.includes('Bancada')) {
        interessado.prop('required', false).parent().parent().show();
        interessado.addClass('obrigatorio');
    } else {
        interessado.prop('required', false).parent().parent().hide();
        interessado.removeClass('obrigatorio');
        interessado.val('').trigger('chosen:updated');
    }
}

function limpaCampos(campos) {
    campos.forEach(function(cha){
        cha.find('option').remove();
        cha.append('<option value="">Selecione um item</option>');
        cha.trigger('chosen:updated');
    });
}

exercicio.change(function () {
    limpaCampos([unidade, autor, emenda, ptres, gnd, ne]);
    $('#solicitacao_sfnnumeroreferencia').val('');
    $('#solicitacao_sfncodvinculacao_disabled').val('');
    $('#solicitacao_sfnfontedetalhada').val('');
    $('#solicitacao_estuf').val('');
    $("#solicitacao_sfnugexecutora").val('').trigger('chosen:updated');

    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {'acao': 'UO', 'usucpf': solicitacao.usucpf},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, unidade);
        }
    });
    interessado.val('');
    interessado.trigger('chosen:updated');
});

unidade.change(function () {
    limpaCampos([autor, emenda, ptres, gnd, ne]);
    $('#solicitacao_sfnnumeroreferencia').val('');
    $('#solicitacao_sfncodvinculacao_disabled').val('');
    $('#solicitacao_sfnfontedetalhada').val('');
    $('#solicitacao_estuf').val('');
    $("#solicitacao_sfnugexecutora").val('').trigger('chosen:updated');

    if (unidade.val()) {
        $.ajax({
            url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
            type: 'post',
            data: {'acao': 'autor', 'unicod': unidade.val(), 'exercicio': exercicio.val()},
            async: false,
            success: function (dados) {
                validaRetonoPopulaCombo(dados, autor);
                $.ajax({
                    url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
                    type: 'post',
                    data: {'acao': 'UGE', 'unicod': unidade.val()},
                    async: false,
                    success: function (dados) {
                        validaRetonoPopulaCombo(dados, uge);
                    }
                });
            }
        });
    }
    interessado.val('');
    interessado.trigger('chosen:updated');
});

autor.change(function () {
    toggleComboInteressado();
    limpaCampos([emenda, ptres, gnd, ne]);
    $('#solicitacao_sfnnumeroreferencia').val('');
    $('#solicitacao_sfncodvinculacao_disabled').val('');
    $('#solicitacao_sfnfontedetalhada').val('');
    $('#solicitacao_estuf').val('');
    $("#solicitacao_sfnugexecutora").val('').trigger('chosen:updated');

    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {'acao': 'interessado', 'exercicio': exercicio.val()},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, interessado);
        }
    });

    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {'acao': 'emenda', 'autid': autor.val(), 'exercicio': exercicio.val(), 'unicod': unidade.val()},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, emenda);
        }
    });
});

emenda.change(function () {
    limpaCampos([ptres, gnd, ne]);
    $('#solicitacao_sfnnumeroreferencia').val('');
    $('#solicitacao_sfncodvinculacao_disabled').val('');
    $('#solicitacao_sfnfontedetalhada').val('');
    $('#solicitacao_estuf').val('');
    $("#solicitacao_sfnugexecutora").val('').trigger('chosen:updated');

    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {'acao': 'ptres', 'unicod': unidade.val(), 'exercicio': exercicio.val(), 'emeid': emenda.val()},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, ptres);
        }
    });
});

ptres.change(function () {
    limpaCampos([ne]);
    $('#solicitacao_sfnnumeroreferencia').val('');
    $('#solicitacao_sfncodvinculacao_disabled').val('');
    $('#solicitacao_sfnfontedetalhada').val('');
    $('#solicitacao_estuf').val('');
    $("#solicitacao_sfnugexecutora").val('').trigger('chosen:updated');

    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {'acao': 'gnd', 'emenda': emenda.val()},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, gnd);
        }
    });
});

gnd.change(function () {
    limpaCampos([ne]);
    $('#solicitacao_sfnnumeroreferencia').val('');
    $('#solicitacao_sfncodvinculacao_disabled').val('');
    $('#solicitacao_sfnfontedetalhada').val('');
    $('#solicitacao_estuf').val('');
    $("#solicitacao_sfnugexecutora").val('').trigger('chosen:updated');
});

uge.change(function(){
    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {'acao': 'ne', 'ptres': ptres.val(), 'gnd' : gnd.val(), 'exercicio': exercicio.val(), 'uge': uge.val()},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, ne);
        }
    });
});

ne.change(function () {
    if (!$(this).val()) {
        limpaCampos([$('#solicitacao_muncod')]);
        $('#solicitacao_sfnconveniosiafi').val('');
        $('#solicitacao_sfnnumeroreferencia').val('').trigger('chosen:updated');
        $('#solicitacao_sfncodvinculacao_disabled').val('');
        $('#solicitacao_sfnfontedetalhada').val('');
        $('#solicitacao_estuf').val('');
    } else {
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                sfnid: $('#sfnid').val(),
                tgid: $('#tgid').val(),
                sfnnotaempenho: ne.val(),
                ptres:  ptres.val(),
                gnd: gnd.val(),
                exercicio: exercicio.val(),
                emeid: emenda.val(),
                unicod: unidade.val(),
                requisicao: 'carregaDadosNE'
            },
            success: function (res) {
                var data = JSON.parse(res);

                if (data.mensagem.length != 0) {
                    ne.val("").trigger('chosen:updated').change();
                    alert(data.mensagem);
                } else {
                    var valorLimite;

                    if (typeof data.valorlimite == 'string') {
                        valorLimite = data.valorlimite;
                    } else {
                        valorLimite = data.valorlimite.toString();
                    }

                    $('#sfpvalorpedido_hidden').val(mascaraglobal('###.###.###.###.###,##', valorLimite));

                    validaRetonoPopulaCombo(JSON.stringify(data.tgnreferenciaCombo), $('#solicitacao_sfnnumeroreferencia'));
                    $('#solicitacao_estuf').val(data.estuf).change();
                    $('#solicitacao_sfnconveniosiafi').val(data.tgconvsiafi);
                    $('#solicitacao_sfnnumeroreferencia').val(data.tgnreferencia).trigger('chosen:updated');
                    $('#solicitacao_sfncodvinculacao_disabled').val(data.sfncodvinculacao);
                    $('#solicitacao_sfnfontedetalhada').val(data.sfnfontedetalhada);
                }
            }
        });
    }
});

// recupera PTRES apartir dos outros dados do formulário
$('#solicitacao_estuf').change(function () {
    $.ajax({
        url: window.location.href,
        type: 'post',
        data: {'requisicao': 'municipio', 'uf': $('#solicitacao_estuf').val()},
        async: false,
        success: function (dados) {
            validaRetonoPopulaCombo(dados, $('#solicitacao_muncod'), false);
        }
    });
});

function validaRetonoPopulaCombo(retorno, combo, ordenado = true) {
    if ('null' != retorno && 'false' != retorno && retorno) {
        try {
            // verifica se precisa dos dados ordenados pela descrição
            if (ordenado) {
                temp = [];
                orig = [];
                obj = JSON.parse(retorno);
                Object.keys(obj).map(function (val, key) {
                    orig[val] = obj[val];
                    temp[key] = obj[val];
                });
                // ordena os dados em ordem alfabetica
                temp.sort();
                // remove as opções atuais do select
                combo.find('option').remove();
                combo.append('<option value="">Selecione um item</option>');
                // popula os dados no combo
                temp.forEach(function(value , key){
                    let val = $.inArray(value, orig);
                    val = (val == '-1')? value : val;
                    combo.append('<option value=' + val + '>' + value + '</option>')
                });
                //reconstroi o chosen
                combo.trigger('chosen:updated');
            } else {
                dados = JSON.parse(retorno);
                // remove as opções atuais do select
                combo.find('option').remove();
                combo.append('<option value="">Selecione um item</option>');
                //injeta dados no select
                Object.keys(dados).forEach(function (key) {
                    combo.append('<option value=' + key + '>' + dados[key] + '</option>');
                });
                //reconstroi o chosen
                combo.trigger('chosen:updated');
            }
        } catch (e){
            alert('Erro ao tentar obter os dados do campo - ' + combo.parent().parent().find('label').text());
        }
    } else {
        combo.find('option').remove();
        combo.append('<option value="">Selecione um item</option>');
        combo.trigger('chosen:updated');
    }
}

// validação do formulário da modal de edição de valor
$("#btn-mod-edit-pedido").on("click", function (e) {
    var vazio = true;

    $("select", $('#form-mod-edit-pedido')).each(function () {
        if ($(this).attr('required')) {
            if ('' == $(this).val() || null == $(this).val()) {
                $(this).parent().parent().addClass('has-error');
                vazio = false;
            } else {
                $(this).parent().parent().removeClass('has-error');
            }
        }
    });

    if (vazio && verificaLimite()) {
        $('#form-mod-edit-pedido').submit();
    }
});

/**
 * Valida os campos do formulário
 *
 * @returns {boolean}
 */
 function validaCamposEnvioSolicitacao() {
    var errorRequired = false
    , errorOptional = false
    , namespace = 'solicitacao_'
    , inputLabels = []
    , fieldsRequired = [
    'sfnugexecutora'
    , 'sfncodvinculacao_disabled'
    , 'sfnfontedetalhada'
    , 'estuf'
            // ,'muncod'
            // ,'sfnobjetivo'
            // ,'sfnvlrsolicitado'
            ]
            , testOptionField = 0
            , mensagemErro = ''
            ;

            fieldsRequired.forEach(function (el) {
                if (!$("#" + namespace + el).val()) {
                    inputLabels.push($("label[for='" + namespace + el + "']").html());
                    errorRequired = true;
                }
            });

            ['sfncontratorepasse',
            'sfnpropostasiconv',
//        'sfnconvenisiafi',
'tgconvsiafi',
'sfnnumeroreferencia']
.forEach(function (el) {
    if (!$("#" + namespace + el).val()) {
        testOptionField++;
    }
});

if (testOptionField == 4) {
    $fieldOptional = ['Contrato Repasse', 'Proposta no SICONV', 'Convênio SIAFI', 'Nº de Referência'];
    errorOptional = true;
}

if (errorRequired) {
    mensagemErro += 'Os campos: <br> - ' + inputLabels.join('<br> - ') + ' <br>São de preenchimento obrigatórios.<br><br>';
}

if (errorOptional) {
    mensagemErro += 'Pelo menos um dos campos a seguir precisa estar preenchido: <br> - ' + $fieldOptional.join('<br> - ');
}

if ($("#" + namespace + "sfnfontedetalhada").val().length != 10) {
    mensagemErro += '<br>O campo Fonte Detalhada, precisar ter 10 digitos.<br>';
}

if (!$("#solicitacao_sfnnotaempenho").val()) {
    mensagemErro += '<br>O campo Nota de Empenho é de preenchimento obrigatório.<br>';
}

var sfncodvinculacao = $("#solicitacao_sfncodvinculacao_disabled").val();
if (sfncodvinculacao.substring(0, 2) == '01') {
    mensagemErro += "<br>Código de vinculação deve começar com 01.<br>";
}

var sfnugexecutora = $("#solicitacao_sfnugexecutora").val();
if (sfnugexecutora.length != 6) {
    mensagemErro += "<br>O código da UG Executora deve ter 6 números.<br>";
}

var limite = parseFloat($("#sfpvalorpedido_hidden").val().replace(/\./g, '').replace(/,/g, ".")),
solicitado = parseFloat($('#solicitacao_sfpvalorpedido').val().replace(/\./g, '').replace(/,/g, "."));
if (isNaN(limite) || limite == '' || limite == 0) {
    alert("A nota de empenho digitada não possui limite de valor a solicitar");
} else if (solicitado > limite) {
    mensagemErro += "<br>O valor solicitado é maior que o limite.<br>";
}

if (mensagemErro != '') {
    return mensagemErro;
}

return true;
}

/**
 * Função para validar os campos obrigatórios do formulário
 * @author Victor Eduardo Barreto
 * @param {sting} form Identificador do form
 * @param {string} tipoRequired Classe ou propriedade que define o campo como
 * obrigatório Ex: propriedade 'required' ou classe obrigatorio etc...
 * @return {bool} Resultado da validação
 */
function validaCamposObrigatorios(form, tipoRequired) {
    var validado = true;
    $("select", $('#'+form)).each(function () {
        if ($(this).hasClass(tipoRequired) || $(this).attr(tipoRequired)) {
            if ('' == $(this).val() || null == $(this).val()) {
                $(this).parent().parent().addClass('has-error');
                validado = false;
            } else {
                $(this).parent().parent().removeClass('has-error');
            }
        }
    });
    return validado;
}

// validação do formulário da primeira etapa
$("#modalSeguir").on("click", function (e) {
    if (validaCamposObrigatorios('formNovaSolicitacop', 'required')) {
        $('#formNovaSolicitacop').submit();
    }
});

// Validação do formulário de edição (2 etapa)
$('#solicitacao').on('submit', function (e, params) {
    var valueSolic = $('#solicitacao_sfpvalorpedido').val();
    if (isNaN(parseFloat(valueSolic)) && valueSolic != '') {
        bootbox.alert("Campo Valor a solicitar deve conter apenas números.");
        $('#solicitacao_sfpvalorpedido').val('')
        return false;
    }
    var lParams = params || {};
    // monta objeto para o historico de alterações
    objetoHistorico();

    if (!lParams.send) {
        e.preventDefault();

        // validação para enviar solicitação (workflow)
        var valida = true;
        if (typeof $('#enviado_hidden').val() !== 'undefined') {
            if ($('#enviado_hidden').val() == $('#enviado_const_hidden').val()) {
                valida = validaCamposEnvioSolicitacao();
            }
        }

        if (typeof valida === 'string') {
            bootbox.alert(valida);
        } else {
            // valida os campos obrigatórios do formulário
            if (validaCamposObrigatorios('solicitacao', 'obrigatorio')) {
                bootbox.confirm({
                    title: "Alerta!",
                    message: "Após salvar, deseja cadastrar novo pedido com os mesmos dados deste?",
                    buttons: {
                        cancel: {
                            label: 'Não'
                        },
                        confirm: {
                            label: 'Sim'
                        }
                    },
                    callback: function (result) {
                        if (result) {
                            $('#copiarSolicitacao').val(1);
                        } else {
                            $('#copiarSolicitacao').val(0);
                        }
                        if ($('#situacaoAtual').val() != $('#situacaoEnviado').val()) {
                            bootbox.confirm({
                                title: "Alerta!",
                                message: "Após salvar, deseja enviar esta solicitação?",
                                buttons: {
                                    cancel: {
                                        label: 'Não'
                                    },
                                    confirm: {
                                        label: 'Sim'
                                    }
                                },
                                callback: function (result) {
                                    if (result) {
                                        $('#enviarSolicitacao').val(1);
                                    } else {
                                        $('#enviarSolicitacao').val(0);
                                    }
                                    $(e.currentTarget).trigger(e.type, {send: true});
                                    return false;
                                }
                            });
                        } else {
                            $(e.currentTarget).trigger(e.type, {send: true});
                        }
                        return false;
                    }
                });
            }
        }
    }
});

/**
 * Função responsável por limpar os campos
 * @param string form Nome do formulário a ser limpo
 */
 function resetar(form) {
    var $form = $('#' + form);

    $("select", $form).each(function () {
        $(this).val("").trigger('chosen:updated');
    });

}

function infoSolicitacao(sfnid) {
    $.post('spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
           {sfnid: sfnid, acao: 'detalhar-emenda'}, function (data) {
            for (var x in data) {
                $('#mdl' + x).text(data[x]);
            }

            $('#detalheemenda').modal();
        }, 'json');
}

function editarSolicitacao(id) {
    location.href = "spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A" +
    "&sfnid=" + id + "&edit=true";
}

function copySolicitacao(id, msg) {
    if (typeof msg === 'undefined') {
        msg = "Após salvar, deseja cadastrar novo pedido com os mesmos dados deste?";
    }

    bootbox.confirm({
        title: "Alerta!",
        message: msg,
        buttons: {
            cancel: {
                label: 'Não'
            },
            confirm: {
                label: 'Sim'
            }
        },
        callback: function (result) {
            if (result) {
                $.ajax({
                    type: 'POST',
                    data: {
                        sfnid: id,
                        acao: 'copySolicitacao'
                    },
                    success: function (data) {
                        var d = data;
                        try {

                            if (data) {
                                d = parseInt(d);
                                if (isNaN(d)) {
                                    throw data;
                                }
                                location.href = "spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A&sfnid=" + d + "&edit=true";
                            } else {
                                alert("Não existe período aberto para cadastro de novos pedidos");
                            }
                        } catch (e) {
                            alert(e);
                        }
                    }
                });
            }
        }
    });
}

/**
 * Apresenta modal com historico de alteração de solicitacao financeiro
 * @return void
 */
 function historico(sfnid) {
    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A',
        type: 'post',
        data: {acao: 'historico', sfnid: sfnid},
        async: false,
        success: function (dados) {
            if (dados) {
                bootbox.dialog(
                {
                    title: 'Histórico edição',
                    size: 'large',
                    message: dados,
                    buttons: {
                        cancel: {
                            label: 'Fechar',
                            className: 'btn-primary'
                        }
                    }
                });
            }
        }
    });
}

/**
 * Ajax para inativar uma solicitação financeira
 * @param id
 */
 function removerSolicitacao(id) {
    $.ajax({
        data: {'acao': 'podeRemover', sfnid: id},
        type: 'post',
        success: function (result) {
            if (result) {
                removerSolcitacaoAjax(id);
            } else {
                bootbox.alert("Este pedido não pode ser excluído, pois já foi enviado. Em caso de dúvida, entrar em contato com a CGF.");
            }
        }
    });
}

function removerSolcitacaoAjax(id) {
    bootbox.confirm({
        title: "Remover Solicitação",
        message: "Deseja realmente excluir solicitação financeira ?",
        buttons: {
            cancel: {
                label: '<i class="fa fa-times"></i> Cancelar'
            },
            confirm: {
                label: '<i class="fa fa-check"></i> Confirmar'
            }
        },
        callback: function (result) {
            if (result) {
                $.ajax({
                    data: {'acao': 'removerSolicitacao', sfnid: id},
                    type: 'post',
                    success: function (result) {
                        window.location.reload(true);
                    }
                });
            }
        }
    });
}

/**
 * Monta um objeto com todos os dados do form para salvar no historico
 * @return void
 */
 function objetoHistorico() {
    let uge = $('#solicitacao_sfnugexecutora');
    let mun = $('#solicitacao_muncod');
    let nref = $('#solicitacao_sfnnumeroreferencia');
    let nrefoutrs = $('#solicitacao_sfnnumeroreferenciaoutros');
    let maq = $('#solicitacao_sfnmequipamento');
    let grau = $('#solicitacao_sfngrauprioridade');
    let dados = {};

    dados.exercicio = exercicio.val()? exercicio.find(':selected').text(): axexercicio;
    dados.unicod = unidade.val()? unidade.find(':selected').text(): axunidade;
    dados.autid = autor.val()? autor.find(':selected').text(): axautor;
    // dados.sfninteressado = interessado.val()? interessado.find(':selected').text(): axinteressado;
    dados.emeid = emenda.val()? emenda.find(':selected').text(): axemenda;
    dados.ptrid = ptres.val()? ptres.find(':selected').text(): axptres;
    dados.sfngrupodespesa = gnd.val()? gnd.find(':selected').text(): axgnd;
    dados.sfnugexecutora = uge.val()? uge.find(':selected').text() : '';
    dados.sfnnotaempenho = $('#solicitacao_sfnnotaempenho').val();
    dados.sfncodvinculacao_disabled = $('#solicitacao_sfncodvinculacao_disabled').val();
    dados.sfnfontedetalhada = $('#solicitacao_sfnfontedetalhada').val();
    dados.estuf = $('#solicitacao_estuf').val();
    dados.muncod = mun.val()? mun.find(':selected').text() : '';
    dados.sfncontratorepasse = $('#solicitacao_sfncontratorepasse').val();
    dados.sfnpropostasiconv = $('#solicitacao_sfnpropostasiconv').val();
    dados.sfnconveniosiafi = $('#solicitacao_sfnconveniosiafi').val();
    dados.sfnnumeroreferencia = nref.val()? nref.find(':selected').text() : '';
    dados.sfnnumeroreferenciaoutros = nrefoutrs.val();
    dados.sfnmequipamento = maq.val()? maq.find(':selected').text() : '';
    dados.sfnobjetivo = $('#solicitacao_sfnobjetivo').val();
    dados.sfngrauprioridade = grau.val()? grau.find(':selected').text() : '';
    dados.sfpvalorpedido = $('#solicitacao_sfpvalorpedido').val();
    $('#sfhatual').val(JSON.stringify(dados));
}
