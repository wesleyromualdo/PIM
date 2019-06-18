/**
 * Select2 <Language> translation.
 * 
 * Author: bigmihail <bigmihail@bigmir.net>
 */
(function ($) {
    "use strict";

    $.extend($.fn.select2.defaults, {
        formatNoMatches: function () { return "ÐÑÑÐ¾Ð³Ð¾ Ð½Ðµ Ð·Ð½Ð°Ð¹Ð´ÐµÐ½Ð¾"; },
        formatInputTooShort: function (input, min) { var n = min - input.length, s = ["", "Ð¸", "ÑÐ²"], p = [2,0,1,1,1,2]; return "ÐÐ²ÐµÐ´ÑÑÑ Ð±ÑÐ»Ñ Ð»Ð°ÑÐºÐ° ÑÐµ " + n + " ÑÐ¸Ð¼Ð²Ð¾Ð»" + s[ (n%100>4 && n%100<=20)? 2 : p[Math.min(n%10, 5)] ]; },
        formatInputTooLong: function (input, max) { var n = input.length - max, s = ["", "Ð¸", "ÑÐ²"], p = [2,0,1,1,1,2]; return "ÐÐ²ÐµÐ´ÑÑÑ Ð±ÑÐ»Ñ Ð»Ð°ÑÐºÐ° Ð½Ð° " + n + " ÑÐ¸Ð¼Ð²Ð¾Ð»" + s[ (n%100>4 && n%100<=20)? 2 : p[Math.min(n%10, 5)] ] + " Ð¼ÐµÐ½ÑÐµ"; },
        formatSelectionTooBig: function (limit) {var s = ["", "Ð¸", "ÑÐ²"], p = [2,0,1,1,1,2];  return "ÐÐ¸ Ð¼Ð¾Ð¶ÐµÑÐµ Ð²Ð¸Ð±ÑÐ°ÑÐ¸ Ð»Ð¸ÑÐµ " + limit + " ÐµÐ»ÐµÐ¼ÐµÐ½Ñ" + s[ (limit%100>4 && limit%100<=20)? 2 : p[Math.min(limit%10, 5)] ]; },
        formatLoadMore: function (pageNumber) { return "ÐÐ°Ð²Ð°Ð½ÑÐ°Ð¶ÐµÐ½Ð½Ñ Ð´Ð°Ð½Ð¸Ñ..."; },
        formatSearching: function () { return "ÐÐ¾ÑÑÐº..."; }
    });
})(jQuery);
