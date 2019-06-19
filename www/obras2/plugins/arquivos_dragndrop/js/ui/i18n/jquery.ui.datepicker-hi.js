/* Hindi initialisation for the jQuery UI date picker plugin. */
/* Written by Michael Dawart. */
jQuery(function($){
	$.datepicker.regional['hi'] = {
		closeText: 'à¤¬à¤à¤¦',
		prevText: 'à¤ªà¤¿à¤à¤²à¤¾',
		nextText: 'à¤à¤à¤²à¤¾',
		currentText: 'à¤à¤',
		monthNames: ['à¤à¤¨à¤µà¤°à¥ ','à¤«à¤°à¤µà¤°à¥','à¤®à¤¾à¤°à¥à¤','à¤à¤ªà¥à¤°à¥à¤²','à¤®à¤','à¤à¥à¤¨',
		'à¤à¥à¤²à¤¾à¤','à¤à¤à¤¸à¥à¤¤ ','à¤¸à¤¿à¤¤à¤®à¥à¤¬à¤°','à¤à¤à¥à¤à¥à¤¬à¤°','à¤¨à¤µà¤®à¥à¤¬à¤°','à¤¦à¤¿à¤¸à¤®à¥à¤¬à¤°'],
		monthNamesShort: ['à¤à¤¨', 'à¤«à¤°', 'à¤®à¤¾à¤°à¥à¤', 'à¤à¤ªà¥à¤°à¥à¤²', 'à¤®à¤', 'à¤à¥à¤¨',
		'à¤à¥à¤²à¤¾à¤', 'à¤à¤', 'à¤¸à¤¿à¤¤', 'à¤à¤à¥à¤', 'à¤¨à¤µ', 'à¤¦à¤¿'],
		dayNames: ['à¤°à¤µà¤¿à¤µà¤¾à¤°', 'à¤¸à¥à¤®à¤µà¤¾à¤°', 'à¤®à¤à¤à¤²à¤µà¤¾à¤°', 'à¤¬à¥à¤§à¤µà¤¾à¤°', 'à¤à¥à¤°à¥à¤µà¤¾à¤°', 'à¤¶à¥à¤à¥à¤°à¤µà¤¾à¤°', 'à¤¶à¤¨à¤¿à¤µà¤¾à¤°'],
		dayNamesShort: ['à¤°à¤µà¤¿', 'à¤¸à¥à¤®', 'à¤®à¤à¤à¤²', 'à¤¬à¥à¤§', 'à¤à¥à¤°à¥', 'à¤¶à¥à¤à¥à¤°', 'à¤¶à¤¨à¤¿'],
		dayNamesMin: ['à¤°à¤µà¤¿', 'à¤¸à¥à¤®', 'à¤®à¤à¤à¤²', 'à¤¬à¥à¤§', 'à¤à¥à¤°à¥', 'à¤¶à¥à¤à¥à¤°', 'à¤¶à¤¨à¤¿'],
		weekHeader: 'à¤¹à¤«à¥à¤¤à¤¾',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['hi']);
});