/* Macedonian i18n for the jQuery UI date picker plugin. */
/* Written by Stojce Slavkovski. */
jQuery(function($){
	$.datepicker.regional['mk'] = {
		closeText: 'ÐÐ°ÑÐ²Ð¾ÑÐ¸',
		prevText: '&#x3C;',
		nextText: '&#x3E;',
		currentText: 'ÐÐµÐ½ÐµÑ',
		monthNames: ['ÐÐ°Ð½ÑÐ°ÑÐ¸','Ð¤ÐµÐ²ÑÑÐ°ÑÐ¸','ÐÐ°ÑÑ','ÐÐ¿ÑÐ¸Ð»','ÐÐ°Ñ','ÐÑÐ½Ð¸',
		'ÐÑÐ»Ð¸','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ¿ÑÐµÐ¼Ð²ÑÐ¸','ÐÐºÑÐ¾Ð¼Ð²ÑÐ¸','ÐÐ¾ÐµÐ¼Ð²ÑÐ¸','ÐÐµÐºÐµÐ¼Ð²ÑÐ¸'],
		monthNamesShort: ['ÐÐ°Ð½','Ð¤ÐµÐ²','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ñ','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ¿','ÐÐºÑ','ÐÐ¾Ðµ','ÐÐµÐº'],
		dayNames: ['ÐÐµÐ´ÐµÐ»Ð°','ÐÐ¾Ð½ÐµÐ´ÐµÐ»Ð½Ð¸Ðº','ÐÑÐ¾ÑÐ½Ð¸Ðº','Ð¡ÑÐµÐ´Ð°','Ð§ÐµÑÐ²ÑÑÐ¾Ðº','ÐÐµÑÐ¾Ðº','Ð¡Ð°Ð±Ð¾ÑÐ°'],
		dayNamesShort: ['ÐÐµÐ´','ÐÐ¾Ð½','ÐÑÐ¾','Ð¡ÑÐµ','Ð§ÐµÑ','ÐÐµÑ','Ð¡Ð°Ð±'],
		dayNamesMin: ['ÐÐµ','ÐÐ¾','ÐÑ','Ð¡Ñ','Ð§Ðµ','ÐÐµ','Ð¡Ð°'],
		weekHeader: 'Ð¡ÐµÐ´',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['mk']);
});
