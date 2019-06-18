/**
 * Select2 Georgian (Kartuli) translation.
 * 
 * Author: Dimitri Kurashvili dimakura@gmail.com
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['ka'] = {
        formatNoMatches: function () { return "ááá  áááá«áááá"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "ááá®ááá á¨ááá§ááááá ááááá " + n + " á¡áááááá"; },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "ááá®ááá á¬áá¨áááá " + n + " á¡áááááá"; },
        formatSelectionTooBig: function (limit) { return "áá¥ááá á¨áááá«áááá áá®áááá " + limit + " á©áááá¬áá áá¡ ááááá¨ááá"; },
        formatLoadMore: function (pageNumber) { return "á¨áááááá¡ á©áá¢ááá áááâ¦"; },
        formatSearching: function () { return "á«ááááâ¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['ka']);
})(jQuery);
