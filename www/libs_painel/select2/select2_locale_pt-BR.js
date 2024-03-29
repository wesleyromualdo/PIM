/**
 * Select2 Brazilian Portuguese translation
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['pt-BR'] = {
        formatNoMatches: function () { return "Nenhum resultado encontrado"; },
        formatAjaxError: function () { return "Erro na busca"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "Digite " + (min == 1 ? "" : "mais") + " " + n + " caracter" + (n == 1? "" : "es"); },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "Apague " + n + " caracter" + (n == 1? "" : "es"); },
        formatSelectionTooBig: function (limit) { return "SÃ³ Ã© possÃ­vel selecionar " + limit + " elemento" + (limit == 1 ? "" : "s"); },
        formatLoadMore: function (pageNumber) { return "Carregando mais resultadosâ¦"; },
        formatSearching: function () { return "Buscandoâ¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['pt-BR']);
})(jQuery);
