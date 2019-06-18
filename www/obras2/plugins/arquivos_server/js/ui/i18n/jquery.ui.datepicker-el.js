/* Greek (el) initialisation for the jQuery UI date picker plugin. */
/* Written by Alex Cicovic (http://www.alexcicovic.com) */
jQuery(function($){
	$.datepicker.regional['el'] = {
		closeText: 'ÎÎ»ÎµÎ¯ÏÎ¹Î¼Î¿',
		prevText: 'Î ÏÎ¿Î·Î³Î¿ÏÎ¼ÎµÎ½Î¿Ï',
		nextText: 'ÎÏÏÎ¼ÎµÎ½Î¿Ï',
		currentText: 'Î¤ÏÎ­ÏÏÎ½ ÎÎ®Î½Î±Ï',
		monthNames: ['ÎÎ±Î½Î¿ÏÎ¬ÏÎ¹Î¿Ï','Î¦ÎµÎ²ÏÎ¿ÏÎ¬ÏÎ¹Î¿Ï','ÎÎ¬ÏÏÎ¹Î¿Ï','ÎÏÏÎ¯Î»Î¹Î¿Ï','ÎÎ¬Î¹Î¿Ï','ÎÎ¿ÏÎ½Î¹Î¿Ï',
		'ÎÎ¿ÏÎ»Î¹Î¿Ï','ÎÏÎ³Î¿ÏÏÏÎ¿Ï','Î£ÎµÏÏÎ­Î¼Î²ÏÎ¹Î¿Ï','ÎÎºÏÏÎ²ÏÎ¹Î¿Ï','ÎÎ¿Î­Î¼Î²ÏÎ¹Î¿Ï','ÎÎµÎºÎ­Î¼Î²ÏÎ¹Î¿Ï'],
		monthNamesShort: ['ÎÎ±Î½','Î¦ÎµÎ²','ÎÎ±Ï','ÎÏÏ','ÎÎ±Î¹','ÎÎ¿ÏÎ½',
		'ÎÎ¿ÏÎ»','ÎÏÎ³','Î£ÎµÏ','ÎÎºÏ','ÎÎ¿Îµ','ÎÎµÎº'],
		dayNames: ['ÎÏÏÎ¹Î±ÎºÎ®','ÎÎµÏÏÎ­ÏÎ±','Î¤ÏÎ¯ÏÎ·','Î¤ÎµÏÎ¬ÏÏÎ·','Î Î­Î¼ÏÏÎ·','Î Î±ÏÎ±ÏÎºÎµÏÎ®','Î£Î¬Î²Î²Î±ÏÎ¿'],
		dayNamesShort: ['ÎÏÏ','ÎÎµÏ','Î¤ÏÎ¹','Î¤ÎµÏ','Î ÎµÎ¼','Î Î±Ï','Î£Î±Î²'],
		dayNamesMin: ['ÎÏ','ÎÎµ','Î¤Ï','Î¤Îµ','Î Îµ','Î Î±','Î£Î±'],
		weekHeader: 'ÎÎ²Î´',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['el']);
});