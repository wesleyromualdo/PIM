/* Georgian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Lado Lomidze (lado.lomidze@gmail.com). */
jQuery(function($){
	$.datepicker.regional['ge'] = {
		closeText: 'ááá®á£á áá',
		prevText: '&#x3c; á¬ááá',
		nextText: 'á¨áááááá &#x3e;',
		currentText: 'áá¦áá¡',
		monthNames: ['áááááá á','ááááá áááá','ááá á¢á','ááá ááá','áááá¡á','ááááá¡á', 'ááááá¡á','ááááá¡á¢á','á¡áá¥á¢ááááá á','áá¥á¢ááááá á','ááááááá á','áááááááá á'],
		monthNamesShort: ['ááá','ááá','ááá ','ááá ','ááá','ááá', 'ááá','ááá','á¡áá¥','áá¥á¢','ááá','ááá'],
		dayNames: ['áááá á','áá á¨ááááá','á¡ááá¨ááááá','ááá®á¨ááááá','á®á£áá¨ááááá','ááá áá¡áááá','á¨ááááá'],
		dayNamesShort: ['áá','áá á¨','á¡áá','ááá®','á®á£á','ááá ','á¨áá'],
		dayNamesMin: ['áá','áá á¨','á¡áá','ááá®','á®á£á','ááá ','á¨áá'],
		weekHeader: 'áááá á',
		dateFormat: 'dd-mm-yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ge']);
});
