/* Luxembourgish initialisation for the jQuery UI date picker plugin. */
/* Written by Michel Weimerskirch <michel@weimerskirch.net> */
jQuery(function($){
	$.datepicker.regional['lb'] = {
		closeText: 'FÃ¤erdeg',
		prevText: 'ZrÃ©ck',
		nextText: 'Weider',
		currentText: 'Haut',
		monthNames: ['Januar','Februar','MÃ¤erz','AbrÃ«ll','Mee','Juni',
		'Juli','August','September','Oktober','November','Dezember'],
		monthNamesShort: ['Jan', 'Feb', 'MÃ¤e', 'Abr', 'Mee', 'Jun',
		'Jul', 'Aug', 'Sep', 'Okt', 'Nov', 'Dez'],
		dayNames: ['Sonndeg', 'MÃ©indeg', 'DÃ«nschdeg', 'MÃ«ttwoch', 'Donneschdeg', 'Freideg', 'Samschdeg'],
		dayNamesShort: ['Son', 'MÃ©i', 'DÃ«n', 'MÃ«t', 'Don', 'Fre', 'Sam'],
		dayNamesMin: ['So','MÃ©','DÃ«','MÃ«','Do','Fr','Sa'],
		weekHeader: 'W',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['lb']);
});
