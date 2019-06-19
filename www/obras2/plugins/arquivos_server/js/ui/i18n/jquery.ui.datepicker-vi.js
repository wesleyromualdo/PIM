/* Vietnamese initialisation for the jQuery UI date picker plugin. */
/* Translated by Le Thanh Huy (lthanhhuy@cit.ctu.edu.vn). */
jQuery(function($){
	$.datepicker.regional['vi'] = {
		closeText: 'ÄÃ³ng',
		prevText: '&#x3c;TrÆ°á»c',
		nextText: 'Tiáº¿p&#x3e;',
		currentText: 'HÃ´m nay',
		monthNames: ['ThÃ¡ng Má»t', 'ThÃ¡ng Hai', 'ThÃ¡ng Ba', 'ThÃ¡ng TÆ°', 'ThÃ¡ng NÄm', 'ThÃ¡ng SÃ¡u',
		'ThÃ¡ng Báº£y', 'ThÃ¡ng TÃ¡m', 'ThÃ¡ng ChÃ­n', 'ThÃ¡ng MÆ°á»i', 'ThÃ¡ng MÆ°á»i Má»t', 'ThÃ¡ng MÆ°á»i Hai'],
		monthNamesShort: ['ThÃ¡ng 1', 'ThÃ¡ng 2', 'ThÃ¡ng 3', 'ThÃ¡ng 4', 'ThÃ¡ng 5', 'ThÃ¡ng 6',
		'ThÃ¡ng 7', 'ThÃ¡ng 8', 'ThÃ¡ng 9', 'ThÃ¡ng 10', 'ThÃ¡ng 11', 'ThÃ¡ng 12'],
		dayNames: ['Chá»§ Nháº­t', 'Thá»© Hai', 'Thá»© Ba', 'Thá»© TÆ°', 'Thá»© NÄm', 'Thá»© SÃ¡u', 'Thá»© Báº£y'],
		dayNamesShort: ['CN', 'T2', 'T3', 'T4', 'T5', 'T6', 'T7'],
		dayNamesMin: ['CN', 'T2', 'T3', 'T4', 'T5', 'T6', 'T7'],
		weekHeader: 'Tu',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['vi']);
});
