/* Azerbaijani (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Jamil Najafov (necefov33@gmail.com). */
jQuery(function($) {
	$.datepicker.regional['az'] = {
		closeText: 'BaÄla',
		prevText: '&#x3c;Geri',
		nextText: 'Ä°rÉli&#x3e;',
		currentText: 'BugÃ¼n',
		monthNames: ['Yanvar','Fevral','Mart','Aprel','May','Ä°yun',
		'Ä°yul','Avqust','Sentyabr','Oktyabr','Noyabr','Dekabr'],
		monthNamesShort: ['Yan','Fev','Mar','Apr','May','Ä°yun',
		'Ä°yul','Avq','Sen','Okt','Noy','Dek'],
		dayNames: ['Bazar','Bazar ertÉsi','ÃÉrÅÉnbÉ axÅamÄ±','ÃÉrÅÉnbÉ','CÃ¼mÉ axÅamÄ±','CÃ¼mÉ','ÅÉnbÉ'],
		dayNamesShort: ['B','Be','Ãa','Ã','Ca','C','Å'],
		dayNamesMin: ['B','B','Ã','Ð¡','Ã','C','Å'],
		weekHeader: 'Hf',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['az']);
});