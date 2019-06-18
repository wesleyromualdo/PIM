/* Kazakh (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Dmitriy Karasyov (dmitriy.karasyov@gmail.com). */
jQuery(function($){
	$.datepicker.regional['kk'] = {
		closeText: 'ÐÐ°Ð±Ñ',
		prevText: '&#x3c;ÐÐ»Ð´ÑÒ£ÒÑ',
		nextText: 'ÐÐµÐ»ÐµÑÑ&#x3e;',
		currentText: 'ÐÒ¯Ð³ÑÐ½',
		monthNames: ['ÒÐ°Ò£ÑÐ°Ñ','ÐÒÐ¿Ð°Ð½','ÐÐ°ÑÑÑÐ·','Ð¡ÓÑÑÑ','ÐÐ°Ð¼ÑÑ','ÐÐ°ÑÑÑÐ¼',
		'Ð¨ÑÐ»Ð´Ðµ','Ð¢Ð°Ð¼ÑÐ·','ÒÑÑÐºÒ¯Ð¹ÐµÐº','ÒÐ°Ð·Ð°Ð½','ÒÐ°ÑÐ°ÑÐ°','ÐÐµÐ»ÑÐ¾ÒÑÐ°Ð½'],
		monthNamesShort: ['ÒÐ°Ò£','ÐÒÐ¿','ÐÐ°Ñ','Ð¡ÓÑ','ÐÐ°Ð¼','ÐÐ°Ñ',
		'Ð¨ÑÐ»','Ð¢Ð°Ð¼','ÒÑÑ','ÒÐ°Ð·','ÒÐ°Ñ','ÐÐµÐ»'],
		dayNames: ['ÐÐµÐºÑÐµÐ½Ð±Ñ','ÐÒ¯Ð¹ÑÐµÐ½Ð±Ñ','Ð¡ÐµÐ¹ÑÐµÐ½Ð±Ñ','Ð¡ÓÑÑÐµÐ½Ð±Ñ','ÐÐµÐ¹ÑÐµÐ½Ð±Ñ','ÐÒ±Ð¼Ð°','Ð¡ÐµÐ½Ð±Ñ'],
		dayNamesShort: ['Ð¶ÐºÑ','Ð´ÑÐ½','ÑÑÐ½','ÑÑÑ','Ð±ÑÐ½','Ð¶Ð¼Ð°','ÑÐ½Ð±'],
		dayNamesMin: ['ÐÐº','ÐÑ','Ð¡Ñ','Ð¡Ñ','ÐÑ','ÐÐ¼','Ð¡Ð½'],
		weekHeader: 'ÐÐµ',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['kk']);
});
