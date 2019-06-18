/**
 * Select2 Romanian translation.
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['ro'] = {
        formatNoMatches: function () { return "Nu a fost gÄsit nimic"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "VÄ rugÄm sÄ introduceÈi incÄ " + n + " caracter" + (n == 1 ? "" : "e"); },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "VÄ rugÄm sÄ introduceÈi mai puÈin de " + n + " caracter" + (n == 1? "" : "e"); },
        formatSelectionTooBig: function (limit) { return "AveÈi voie sÄ selectaÈi cel mult " + limit + " element" + (limit == 1 ? "" : "e"); },
        formatLoadMore: function (pageNumber) { return "Se Ã®ncarcÄâ¦"; },
        formatSearching: function () { return "CÄutareâ¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['ro']);
})(jQuery);
