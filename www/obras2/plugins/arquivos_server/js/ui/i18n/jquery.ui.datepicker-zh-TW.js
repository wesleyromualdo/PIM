/* Chinese initialisation for the jQuery UI date picker plugin. */
/* Written by Ressol (ressol@gmail.com). */
jQuery(function($){
	$.datepicker.regional['zh-TW'] = {
		closeText: 'éé',
		prevText: '&#x3c;ä¸æ',
		nextText: 'ä¸æ&#x3e;',
		currentText: 'ä»å¤©',
		monthNames: ['ä¸æ','äºæ','ä¸æ','åæ','äºæ','å­æ',
		'ä¸æ','å«æ','ä¹æ','åæ','åä¸æ','åäºæ'],
		monthNamesShort: ['ä¸','äº','ä¸','å','äº','å­',
		'ä¸','å«','ä¹','å','åä¸','åäº'],
		dayNames: ['æææ¥','ææä¸','ææäº','ææä¸','ææå','ææäº','ææå­'],
		dayNamesShort: ['å¨æ¥','å¨ä¸','å¨äº','å¨ä¸','å¨å','å¨äº','å¨å­'],
		dayNamesMin: ['æ¥','ä¸','äº','ä¸','å','äº','å­'],
		weekHeader: 'å¨',
		dateFormat: 'yy/mm/dd',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: 'å¹´'};
	$.datepicker.setDefaults($.datepicker.regional['zh-TW']);
});
