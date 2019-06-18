/* Serbian i18n for the jQuery UI date picker plugin. */
/* Written by Dejan DimiÄ. */
jQuery(function($){
	$.datepicker.regional['sr'] = {
		closeText: 'ÐÐ°ÑÐ²Ð¾ÑÐ¸',
		prevText: '&#x3c;',
		nextText: '&#x3e;',
		currentText: 'ÐÐ°Ð½Ð°Ñ',
		monthNames: ['ÐÐ°Ð½ÑÐ°Ñ','Ð¤ÐµÐ±ÑÑÐ°Ñ','ÐÐ°ÑÑ','ÐÐ¿ÑÐ¸Ð»','ÐÐ°Ñ','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ¿ÑÐµÐ¼Ð±Ð°Ñ','ÐÐºÑÐ¾Ð±Ð°Ñ','ÐÐ¾Ð²ÐµÐ¼Ð±Ð°Ñ','ÐÐµÑÐµÐ¼Ð±Ð°Ñ'],
		monthNamesShort: ['ÐÐ°Ð½','Ð¤ÐµÐ±','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ñ','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ¿','ÐÐºÑ','ÐÐ¾Ð²','ÐÐµÑ'],
		dayNames: ['ÐÐµÐ´ÐµÑÐ°','ÐÐ¾Ð½ÐµÐ´ÐµÑÐ°Ðº','Ð£ÑÐ¾ÑÐ°Ðº','Ð¡ÑÐµÐ´Ð°','Ð§ÐµÑÐ²ÑÑÐ°Ðº','ÐÐµÑÐ°Ðº','Ð¡ÑÐ±Ð¾ÑÐ°'],
		dayNamesShort: ['ÐÐµÐ´','ÐÐ¾Ð½','Ð£ÑÐ¾','Ð¡ÑÐµ','Ð§ÐµÑ','ÐÐµÑ','Ð¡ÑÐ±'],
		dayNamesMin: ['ÐÐµ','ÐÐ¾','Ð£Ñ','Ð¡Ñ','Ð§Ðµ','ÐÐµ','Ð¡Ñ'],
		weekHeader: 'Ð¡ÐµÐ´',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['sr']);
});
