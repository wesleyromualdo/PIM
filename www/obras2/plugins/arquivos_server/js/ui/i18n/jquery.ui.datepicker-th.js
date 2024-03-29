/* Thai initialisation for the jQuery UI date picker plugin. */
/* Written by pipo (pipo@sixhead.com). */
jQuery(function($){
	$.datepicker.regional['th'] = {
		closeText: 'à¸à¸´à¸',
		prevText: '&laquo;&nbsp;à¸¢à¹à¸­à¸',
		nextText: 'à¸à¸±à¸à¹à¸&nbsp;&raquo;',
		currentText: 'à¸§à¸±à¸à¸à¸µà¹',
		monthNames: ['à¸¡à¸à¸£à¸²à¸à¸¡','à¸à¸¸à¸¡à¸ à¸²à¸à¸±à¸à¸à¹','à¸¡à¸µà¸à¸²à¸à¸¡','à¹à¸¡à¸©à¸²à¸¢à¸','à¸à¸¤à¸©à¸ à¸²à¸à¸¡','à¸¡à¸´à¸à¸¸à¸à¸²à¸¢à¸',
		'à¸à¸£à¸à¸à¸²à¸à¸¡','à¸ªà¸´à¸à¸«à¸²à¸à¸¡','à¸à¸±à¸à¸¢à¸²à¸¢à¸','à¸à¸¸à¸¥à¸²à¸à¸¡','à¸à¸¤à¸¨à¸à¸´à¸à¸²à¸¢à¸','à¸à¸±à¸à¸§à¸²à¸à¸¡'],
		monthNamesShort: ['à¸¡.à¸.','à¸.à¸.','à¸¡à¸µ.à¸.','à¹à¸¡.à¸¢.','à¸.à¸.','à¸¡à¸´.à¸¢.',
		'à¸.à¸.','à¸ª.à¸.','à¸.à¸¢.','à¸.à¸.','à¸.à¸¢.','à¸.à¸.'],
		dayNames: ['à¸­à¸²à¸à¸´à¸à¸¢à¹','à¸à¸±à¸à¸à¸£à¹','à¸­à¸±à¸à¸à¸²à¸£','à¸à¸¸à¸','à¸à¸¤à¸«à¸±à¸ªà¸à¸à¸µ','à¸¨à¸¸à¸à¸£à¹','à¹à¸ªà¸²à¸£à¹'],
		dayNamesShort: ['à¸­à¸².','à¸.','à¸­.','à¸.','à¸à¸¤.','à¸¨.','à¸ª.'],
		dayNamesMin: ['à¸­à¸².','à¸.','à¸­.','à¸.','à¸à¸¤.','à¸¨.','à¸ª.'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['th']);
});