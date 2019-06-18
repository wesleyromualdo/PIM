/* Tajiki (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Abdurahmon Saidov (saidovab@gmail.com). */
jQuery(function($){
	$.datepicker.regional['tj'] = {
		closeText: 'ÐÐ´Ð¾Ð¼Ð°',
		prevText: '&#x3c;ÒÐ°ÑÐ¾',
		nextText: 'ÐÐµÑ&#x3e;',
		currentText: 'ÐÐ¼ÑÓ¯Ð·',
		monthNames: ['Ð¯Ð½Ð²Ð°Ñ','Ð¤ÐµÐ²ÑÐ°Ð»','ÐÐ°ÑÑ','ÐÐ¿ÑÐµÐ»','ÐÐ°Ð¹','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ½ÑÑÐ±Ñ','ÐÐºÑÑÐ±Ñ','ÐÐ¾ÑÐ±Ñ','ÐÐµÐºÐ°Ð±Ñ'],
		monthNamesShort: ['Ð¯Ð½Ð²','Ð¤ÐµÐ²','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ð¹','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ½','ÐÐºÑ','ÐÐ¾Ñ','ÐÐµÐº'],
		dayNames: ['ÑÐºÑÐ°Ð½Ð±Ðµ','Ð´ÑÑÐ°Ð½Ð±Ðµ','ÑÐµÑÐ°Ð½Ð±Ðµ','ÑÐ¾ÑÑÐ°Ð½Ð±Ðµ','Ð¿Ð°Ð½Ò·ÑÐ°Ð½Ð±Ðµ','Ò·ÑÐ¼ÑÐ°','ÑÐ°Ð½Ð±Ðµ'],
		dayNamesShort: ['ÑÐºÑ','Ð´ÑÑ','ÑÐµÑ','ÑÐ¾Ñ','Ð¿Ð°Ð½','Ò·ÑÐ¼','ÑÐ°Ð½'],
		dayNamesMin: ['Ð¯Ðº','ÐÑ','Ð¡Ñ','Ð§Ñ','ÐÑ','Ò¶Ð¼','Ð¨Ð½'],
		weekHeader: 'Ð¥Ñ',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['tj']);
});