/**
 * Select2 Chinese translation
 */
(function ($) {
    "use strict";
    $.fn.select2.locales['zh-CN'] = {
        formatNoMatches: function () { return "æ²¡ææ¾å°å¹éé¡¹"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "è¯·åè¾å¥" + n + "ä¸ªå­ç¬¦";},
        formatInputTooLong: function (input, max) { var n = input.length - max; return "è¯·å æ" + n + "ä¸ªå­ç¬¦";},
        formatSelectionTooBig: function (limit) { return "ä½ åªè½éæ©æå¤" + limit + "é¡¹"; },
        formatLoadMore: function (pageNumber) { return "å è½½ç»æä¸­â¦"; },
        formatSearching: function () { return "æç´¢ä¸­â¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['zh-CN']);
})(jQuery);
