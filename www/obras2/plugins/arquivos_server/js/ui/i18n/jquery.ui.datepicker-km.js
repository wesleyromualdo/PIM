/* Khmer initialisation for the jQuery calendar extension. */
/* Written by Chandara Om (chandara.teacher@gmail.com). */
jQuery(function($){
	$.datepicker.regional['km'] = {
		closeText: 'áááá¾âáá½á',
		prevText: 'áá»á',
		nextText: 'ááááá¶áá',
		currentText: 'ááááâááá',
		monthNames: ['áááá¶','áá»áááá','áá¸áá¶','áááá¶','á§ááá¶','áá·áá»áá¶',
		'áááááá¶','áá¸á á¶','ááááá¶','áá»áá¶','áá·áááá·áá¶','áááá¼'],
		monthNamesShort: ['áááá¶','áá»áááá','áá¸áá¶','áááá¶','á§ááá¶','áá·áá»áá¶',
		'áááááá¶','áá¸á á¶','ááááá¶','áá»áá¶','áá·áááá·áá¶','áááá¼'],
		dayNames: ['á¢á¶áá·ááá', 'áááá', 'á¢áááá¶á', 'áá»á', 'áááá ááááá·á', 'áá»ááá', 'áááá'],
		dayNamesShort: ['á¢á¶', 'á', 'á¢', 'áá»', 'áááá ', 'áá»', 'áá'],
		dayNamesMin: ['á¢á¶', 'á', 'á¢', 'áá»', 'áááá ', 'áá»', 'áá'],
		weekHeader: 'ááááá¶á á',
		dateFormat: 'dd-mm-yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['km']);
});
