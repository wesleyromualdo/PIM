/**
 * Select2 Bulgarian translation.
 * 
 * @author  Lubomir Vikev <lubomirvikev@gmail.com>
 * @author  Uriy Efremochkin <efremochkin@uriy.me>
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['bg'] = {
        formatNoMatches: function () { return "ÐÑÐ¼Ð° Ð½Ð°Ð¼ÐµÑÐµÐ½Ð¸ ÑÑÐ²Ð¿Ð°Ð´ÐµÐ½Ð¸Ñ"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "ÐÐ¾Ð»Ñ Ð²ÑÐ²ÐµÐ´ÐµÑÐµ Ð¾ÑÐµ " + n + " ÑÐ¸Ð¼Ð²Ð¾Ð»" + (n > 1 ? "Ð°" : ""); },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "ÐÐ¾Ð»Ñ Ð²ÑÐ²ÐµÐ´ÐµÑÐµ Ñ " + n + " Ð¿Ð¾-Ð¼Ð°Ð»ÐºÐ¾ ÑÐ¸Ð¼Ð²Ð¾Ð»" + (n > 1 ? "Ð°" : ""); },
        formatSelectionTooBig: function (limit) { return "ÐÐ¾Ð¶ÐµÑÐµ Ð´Ð° Ð½Ð°Ð¿ÑÐ°Ð²Ð¸ÑÐµ Ð´Ð¾ " + limit + (limit > 1 ? " Ð¸Ð·Ð±Ð¾ÑÐ°" : " Ð¸Ð·Ð±Ð¾Ñ"); },
        formatLoadMore: function (pageNumber) { return "ÐÐ°ÑÐµÐ¶Ð´Ð°Ñ ÑÐµ Ð¾ÑÐµâ¦"; },
        formatSearching: function () { return "Ð¢ÑÑÑÐµÐ½Ðµâ¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['bg']);
})(jQuery);
