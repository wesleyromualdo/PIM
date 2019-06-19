/* Hebrew initialisation for the UI Datepicker extension. */
/* Written by Amir Hardon (ahardon at gmail dot com). */
jQuery(function($){
	$.datepicker.regional['he'] = {
		closeText: '×¡×××¨',
		prevText: '&#x3c;××§×××',
		nextText: '×××&#x3e;',
		currentText: '××××',
		monthNames: ['×× ×××¨','×¤××¨×××¨','××¨×¥','××¤×¨××','×××','××× ×',
		'××××','×××××¡×','×¡×¤××××¨','×××§××××¨','× ×××××¨','××¦×××¨'],
		monthNamesShort: ['×× ×','×¤××¨','××¨×¥','××¤×¨','×××','××× ×',
		'××××','×××','×¡×¤×','×××§','× ××','××¦×'],
		dayNames: ['×¨××©××','×©× ×','×©×××©×','×¨×××¢×','××××©×','×©××©×','×©××ª'],
		dayNamesShort: ['×\'','×\'','×\'','×\'','×\'','×\'','×©××ª'],
		dayNamesMin: ['×\'','×\'','×\'','×\'','×\'','×\'','×©××ª'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: true,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['he']);
});
