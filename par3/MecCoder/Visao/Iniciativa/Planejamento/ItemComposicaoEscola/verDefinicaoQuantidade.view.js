var _ItensDeComposicao = function () {
    var $escopo = $('.itens-de-composicao-por-escola');
    var _objetoListagem = {};
    this.iniciar = function () {
        var that = this;
        this.carregarEscolas();
        $('.js-definir-quantidade-padrao').on('change', function () {
            var valorPadrao = $(this).val();
            that.aoModificarQuantidadePadrao(valorPadrao);
            $.each($escopo.find('.listagem input[data-val-quantidade]'), function () {
                $(this).val(valorPadrao);
            });
        });
    };
    this.objetoListagem = function (obj) {
        if (obj) {
            _objetoListagem = $().limparObjeto(obj, true);
        } else {
            return _objetoListagem;
        }
    }

    this.aoModificarQuantidadePadrao = function (valorPadrao) {

    };
    this.aoModificarQuantidade = function (valores) {

    };
    this.carregarEscolas = function () {
        var that = this;
        $.ajax('par3.php?modulo=par3/controle/iniciativa/planejamento/item-composicao-escola&acao=A&requisicao=listarEscolas', {
            'type': 'get',
            'dataType': 'json',
            'data': $.param(_JS._POST)
        }).done(function (dados) {
            if (dados.length) {
                that.objetoListagem(dados[0]);
                that.preencherListaEscolas(dados);
            }
        });
    };
    this.preencherListaEscolas = function (dados) {
        var that = this;
        $escopo.find('.js-resultado-pesquisa').show();
        $escopo.find('.listagem').view('iterate', {
            'template': $escopo.find('.templates .item-listagem'),
            'collection': dados,
            'callbackItem': function ($item, dados) {
                $item.find(':input').on('change', function () {
                    that.aoModificarQuantidade(dados);
                });
            }
        });
        $('.js-total-registros').html($escopo.find('.listagem tr').length);
    };
    this.iniciar();
};
if (!Planejamento) {
    var Planejamento = {};
}
$(document).ready(function () {
    Planejamento.ItensComposicao = new _ItensDeComposicao();
});
