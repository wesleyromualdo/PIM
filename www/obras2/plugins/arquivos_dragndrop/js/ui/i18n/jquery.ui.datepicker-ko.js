/* Korean initialisation for the jQuery calendar extension. */
/* Written by DaeKwon Kang (ncrash.dk@gmail.com), Edited by Genie. */
jQuery(function($){
	$.datepicker.regional['ko'] = {
		closeText: 'ë«ê¸°',
		prevText: 'ì´ì ë¬',
		nextText: 'ë¤ìë¬',
		currentText: 'ì¤ë',
		monthNames: ['1ì','2ì','3ì','4ì','5ì','6ì',
		'7ì','8ì','9ì','10ì','11ì','12ì'],
		monthNamesShort: ['1ì','2ì','3ì','4ì','5ì','6ì',
		'7ì','8ì','9ì','10ì','11ì','12ì'],
		dayNames: ['ì¼ìì¼','ììì¼','íìì¼','ììì¼','ëª©ìì¼','ê¸ìì¼','í ìì¼'],
		dayNamesShort: ['ì¼','ì','í','ì','ëª©','ê¸','í '],
		dayNamesMin: ['ì¼','ì','í','ì','ëª©','ê¸','í '],
		weekHeader: 'Wk',
		dateFormat: 'yy-mm-dd',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: 'ë'};
	$.datepicker.setDefaults($.datepicker.regional['ko']);
});