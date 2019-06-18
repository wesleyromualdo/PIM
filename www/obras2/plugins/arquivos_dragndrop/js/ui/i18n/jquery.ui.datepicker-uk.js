/* Ukrainian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Maxim Drogobitskiy (maxdao@gmail.com). */
/* Corrected by Igor Milla (igor.fsp.milla@gmail.com). */
jQuery(function($){
	$.datepicker.regional['uk'] = {
		closeText: 'ÐÐ°ÐºÑÐ¸ÑÐ¸',
		prevText: '&#x3c;',
		nextText: '&#x3e;',
		currentText: 'Ð¡ÑÐ¾Ð³Ð¾Ð´Ð½Ñ',
		monthNames: ['Ð¡ÑÑÐµÐ½Ñ','ÐÑÑÐ¸Ð¹','ÐÐµÑÐµÐ·ÐµÐ½Ñ','ÐÐ²ÑÑÐµÐ½Ñ','Ð¢ÑÐ°Ð²ÐµÐ½Ñ','Ð§ÐµÑÐ²ÐµÐ½Ñ',
		'ÐÐ¸Ð¿ÐµÐ½Ñ','Ð¡ÐµÑÐ¿ÐµÐ½Ñ','ÐÐµÑÐµÑÐµÐ½Ñ','ÐÐ¾Ð²ÑÐµÐ½Ñ','ÐÐ¸ÑÑÐ¾Ð¿Ð°Ð´','ÐÑÑÐ´ÐµÐ½Ñ'],
		monthNamesShort: ['Ð¡ÑÑ','ÐÑÑ','ÐÐµÑ','ÐÐ²Ñ','Ð¢ÑÐ°','Ð§ÐµÑ',
		'ÐÐ¸Ð¿','Ð¡ÐµÑ','ÐÐµÑ','ÐÐ¾Ð²','ÐÐ¸Ñ','ÐÑÑ'],
		dayNames: ['Ð½ÐµÐ´ÑÐ»Ñ','Ð¿Ð¾Ð½ÐµÐ´ÑÐ»Ð¾Ðº','Ð²ÑÐ²ÑÐ¾ÑÐ¾Ðº','ÑÐµÑÐµÐ´Ð°','ÑÐµÑÐ²ÐµÑ','Ð¿âÑÑÐ½Ð¸ÑÑ','ÑÑÐ±Ð¾ÑÐ°'],
		dayNamesShort: ['Ð½ÐµÐ´','Ð¿Ð½Ð´','Ð²ÑÐ²','ÑÑÐ´','ÑÑÐ²','Ð¿ÑÐ½','ÑÐ±Ñ'],
		dayNamesMin: ['ÐÐ´','ÐÐ½','ÐÑ','Ð¡Ñ','Ð§Ñ','ÐÑ','Ð¡Ð±'],
		weekHeader: 'Ð¢Ð¸Ð¶',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['uk']);
});