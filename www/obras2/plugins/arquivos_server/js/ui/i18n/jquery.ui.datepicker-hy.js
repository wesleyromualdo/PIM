/* Armenian(UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Levon Zakaryan (levon.zakaryan@gmail.com)*/
jQuery(function($){
	$.datepicker.regional['hy'] = {
		closeText: 'ÕÕ¡Õ¯Õ¥Õ¬',
		prevText: '&#x3c;ÕÕ¡Õ­.',
		nextText: 'ÕÕ¡Õ».&#x3e;',
		currentText: 'Ô±ÕµÕ½ÖÖ',
		monthNames: ['ÕÕ¸ÖÕ¶Õ¾Õ¡Ö','ÕÕ¥Õ¿ÖÕ¾Õ¡Ö','ÕÕ¡ÖÕ¿','Ô±ÕºÖÕ«Õ¬','ÕÕ¡ÕµÕ«Õ½','ÕÕ¸ÖÕ¶Õ«Õ½',
		'ÕÕ¸ÖÕ¬Õ«Õ½','ÕÕ£Õ¸Õ½Õ¿Õ¸Õ½','ÕÕ¥ÕºÕ¿Õ¥Õ´Õ¢Õ¥Ö','ÕÕ¸Õ¯Õ¿Õ¥Õ´Õ¢Õ¥Ö','ÕÕ¸ÕµÕ¥Õ´Õ¢Õ¥Ö','Ô´Õ¥Õ¯Õ¿Õ¥Õ´Õ¢Õ¥Ö'],
		monthNamesShort: ['ÕÕ¸ÖÕ¶Õ¾','ÕÕ¥Õ¿Ö','ÕÕ¡ÖÕ¿','Ô±ÕºÖ','ÕÕ¡ÕµÕ«Õ½','ÕÕ¸ÖÕ¶Õ«Õ½',
		'ÕÕ¸ÖÕ¬','ÕÕ£Õ½','ÕÕ¥Õº','ÕÕ¸Õ¯','ÕÕ¸Õµ','Ô´Õ¥Õ¯'],
		dayNames: ['Õ¯Õ«ÖÕ¡Õ¯Õ«','Õ¥Õ¯Õ¸ÖÕ·Õ¡Õ¢Õ©Õ«','Õ¥ÖÕ¥ÖÕ·Õ¡Õ¢Õ©Õ«','Õ¹Õ¸ÖÕ¥ÖÕ·Õ¡Õ¢Õ©Õ«','Õ°Õ«Õ¶Õ£Õ·Õ¡Õ¢Õ©Õ«','Õ¸ÖÖÕ¢Õ¡Õ©','Õ·Õ¡Õ¢Õ¡Õ©'],
		dayNamesShort: ['Õ¯Õ«Ö','Õ¥ÖÕ¯','Õ¥ÖÖ','Õ¹ÖÖ','Õ°Õ¶Õ£','Õ¸ÖÖÕ¢','Õ·Õ¢Õ©'],
		dayNamesMin: ['Õ¯Õ«Ö','Õ¥ÖÕ¯','Õ¥ÖÖ','Õ¹ÖÖ','Õ°Õ¶Õ£','Õ¸ÖÖÕ¢','Õ·Õ¢Õ©'],
		weekHeader: 'ÕÔ²Õ',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['hy']);
});