/**
 * Select2 Japanese translation.
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['ja'] = {
        formatNoMatches: function () { return "è©²å½ãªã"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "å¾" + n + "æå­å¥ãã¦ãã ãã"; },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "æ¤ç´¢æå­åã" + n + "æå­é·ããã¾ã"; },
        formatSelectionTooBig: function (limit) { return "æå¤ã§" + limit + "é ç®ã¾ã§ããé¸æã§ãã¾ãã"; },
        formatLoadMore: function (pageNumber) { return "èª­è¾¼ä¸­ï½¥ï½¥ï½¥"; },
        formatSearching: function () { return "æ¤ç´¢ä¸­ï½¥ï½¥ï½¥"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['ja']);
})(jQuery);
