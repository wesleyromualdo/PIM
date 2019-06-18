/**
* Select2 Hebrew translation.
*
* Author: Yakir Sitbon <http://www.yakirs.net/>
*/
(function ($) {
    "use strict";

    $.fn.select2.locales['he'] = {
        formatNoMatches: function () { return "×× × ××¦×× ××ª××××ª"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "× × ××××× ×¢×× " + n + " ×ª×××× × ××¡×¤××"; },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "× × ××××× ×¤×××ª " + n + " ×ª××××"; },
        formatSelectionTooBig: function (limit) { return "× ××ª× ×××××¨ " + limit + " ×¤×¨××××"; },
        formatLoadMore: function (pageNumber) { return "×××¢× ×ª××¦×××ª × ××¡×¤××ªâ¦"; },
        formatSearching: function () { return "×××¤×©â¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['he']);
})(jQuery);
