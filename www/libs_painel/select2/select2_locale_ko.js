/**
 * Select2 Korean translation.
 * 
 * @author  Swen Mun <longfinfunnel@gmail.com>
 */
(function ($) {
    "use strict";

    $.fn.select2.locales['ko'] = {
        formatNoMatches: function () { return "ê²°ê³¼ ìì"; },
        formatInputTooShort: function (input, min) { var n = min - input.length; return "ëë¬´ ì§§ìµëë¤. "+n+"ê¸ì ë ìë ¥í´ì£¼ì¸ì."; },
        formatInputTooLong: function (input, max) { var n = input.length - max; return "ëë¬´ ê¹ëë¤. "+n+"ê¸ì ì§ìì£¼ì¸ì."; },
        formatSelectionTooBig: function (limit) { return "ìµë "+limit+"ê°ê¹ì§ë§ ì ííì¤ ì ììµëë¤."; },
        formatLoadMore: function (pageNumber) { return "ë¶ë¬ì¤ë ì¤â¦"; },
        formatSearching: function () { return "ê²ì ì¤â¦"; }
    };

    $.extend($.fn.select2.defaults, $.fn.select2.locales['ko']);
})(jQuery);
