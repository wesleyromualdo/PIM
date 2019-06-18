/**
 * Select2 Traditional Chinese translation
 */
(function ($) {
    "use strict";
    $.fn.select2.locales['zh-TW'] = {
        formatNoMatches: function () { return "æ²ææ¾å°ç¸ç¬¦çé ç®"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "è«åè¼¸å¥" + n + "åå­å";},
        formatInputTooLong: function (input, max) { var n = input.length - max; return "è«åªæ" + n + "åå­å";},
        formatSelectionTooBig: function (limit) { return "ä½ åªè½é¸ææå¤" + limit + "é "; },
        formatLoadMore: function (pageNumber) { return "è¼å¥ä¸­â¦"; },
        formatSearching: function () { return "æå°ä¸­â¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['zh-TW']);
})(jQuery);
