/**
 * Select2 Persian translation.
 * 
 * Author: Ali Choopan <choopan@arsh.co>
 * Author: Ebrahim Byagowi <ebrahim@gnu.org>
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['fa'] = {
        formatMatches: function (matches) { return matches + " ÙØªÛØ¬Ù ÙÙØ¬ÙØ¯ Ø§Ø³ØªØ Ú©ÙÛØ¯ÙØ§Û Ø¬ÙØª Ø¨Ø§ÙØ§ Ù Ù¾Ø§ÛÛÙ Ø±Ø§ Ø¨Ø±Ø§Û Ú¯Ø´ØªÙ Ø§Ø³ØªÙØ§Ø¯Ù Ú©ÙÛØ¯."; },
        formatNoMatches: function () { return "ÙØªÛØ¬ÙâØ§Û ÛØ§ÙØª ÙØ´Ø¯."; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "ÙØ·ÙØ§Ù " + n + " ÙÙÛØ³Ù Ø¨ÛØ´ØªØ± ÙØ§Ø±Ø¯ ÙÙØ§ÛÛØ¯"; },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "ÙØ·ÙØ§Ù " + n + " ÙÙÛØ³Ù Ø±Ø§ Ø­Ø°Ù Ú©ÙÛØ¯."; },
        formatSelectionTooBig: function (limit) { return "Ø´ÙØ§ ÙÙØ· ÙÛâØªÙØ§ÙÛØ¯ " + limit + " ÙÙØ±Ø¯ Ø±Ø§ Ø§ÙØªØ®Ø§Ø¨ Ú©ÙÛØ¯"; },
        formatLoadMore: function (pageNumber) { return "Ø¯Ø± Ø­Ø§Ù Ø¨Ø§Ø±Ú¯ÛØ±Û ÙÙØ§Ø±Ø¯ Ø¨ÛØ´ØªØ±â¦"; },
        formatSearching: function () { return "Ø¯Ø± Ø­Ø§Ù Ø¬Ø³ØªØ¬Ùâ¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['fa']);
})(jQuery);
