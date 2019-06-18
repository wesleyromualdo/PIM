/**
 * Select2 Arabic translation.
 *
 * Author: Adel KEDJOUR <adel@kedjour.com>
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['ar'] = {
        formatNoMatches: function () { return "ÙÙ ÙØªÙ Ø§ÙØ¹Ø«ÙØ± Ø¹ÙÙ ÙØ·Ø§Ø¨ÙØ§Øª"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; if (n == 1){ return "Ø§ÙØ±Ø¬Ø§Ø¡ Ø¥Ø¯Ø®Ø§Ù Ø­Ø±Ù ÙØ§Ø­Ø¯ Ø¹ÙÙ Ø§ÙØ£ÙØ«Ø±"; } return n == 2 ? "Ø§ÙØ±Ø¬Ø§Ø¡ Ø¥Ø¯Ø®Ø§Ù Ø­Ø±ÙÙÙ Ø¹ÙÙ Ø§ÙØ£ÙØ«Ø±" : "Ø§ÙØ±Ø¬Ø§Ø¡ Ø¥Ø¯Ø®Ø§Ù " + n + " Ø¹ÙÙ Ø§ÙØ£ÙØ«Ø±"; },
        formatInputTooLong: function (input, max) { var n = input.length - max; if (n == 1){ return "Ø§ÙØ±Ø¬Ø§Ø¡ Ø¥Ø¯Ø®Ø§Ù Ø­Ø±Ù ÙØ§Ø­Ø¯ Ø¹ÙÙ Ø§ÙØ£ÙÙ"; } return n == 2 ? "Ø§ÙØ±Ø¬Ø§Ø¡ Ø¥Ø¯Ø®Ø§Ù Ø­Ø±ÙÙÙ Ø¹ÙÙ Ø§ÙØ£ÙÙ" : "Ø§ÙØ±Ø¬Ø§Ø¡ Ø¥Ø¯Ø®Ø§Ù " + n + " Ø¹ÙÙ Ø§ÙØ£ÙÙ "; },
        formatSelectionTooBig: function (limit) { if (limit == 1){ return "ÙÙÙÙÙ Ø£Ù ØªØ®ØªØ§Ø± Ø¥Ø®ØªÙØ§Ø± ÙØ§Ø­Ø¯ ÙÙØ·"; } return limit == 2 ? "ÙÙÙÙÙ Ø£Ù ØªØ®ØªØ§Ø± Ø¥Ø®ØªÙØ§Ø±ÙÙ ÙÙØ·" : "ÙÙÙÙÙ Ø£Ù ØªØ®ØªØ§Ø± " + limit + " Ø¥Ø®ØªÙØ§Ø±Ø§Øª ÙÙØ·"; },
        formatLoadMore: function (pageNumber) { return "ØªØ­ÙÙÙ Ø§ÙÙØ²ÙØ¯ ÙÙ Ø§ÙÙØªØ§Ø¦Ø¬â¦"; },
        formatSearching: function () { return "Ø§ÙØ¨Ø­Ø«â¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['ar']);
})(jQuery);
