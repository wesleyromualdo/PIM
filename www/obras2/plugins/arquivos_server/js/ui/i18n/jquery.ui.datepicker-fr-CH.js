/* Swiss-French initialisation for the jQuery UI date picker plugin. */
/* Written Martin Voelkle (martin.voelkle@e-tc.ch). */
jQuery(function($){
	$.datepicker.regional['fr-CH'] = {
		closeText: 'Fermer',
		prevText: '&#x3c;PrÃ©c',
		nextText: 'Suiv&#x3e;',
		currentText: 'Courant',
		monthNames: ['Janvier','FÃ©vrier','Mars','Avril','Mai','Juin',
		'Juillet','AoÃ»t','Septembre','Octobre','Novembre','DÃ©cembre'],
		monthNamesShort: ['Jan','FÃ©v','Mar','Avr','Mai','Jun',
		'Jul','AoÃ»','Sep','Oct','Nov','DÃ©c'],
		dayNames: ['Dimanche','Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi'],
		dayNamesShort: ['Dim','Lun','Mar','Mer','Jeu','Ven','Sam'],
		dayNamesMin: ['Di','Lu','Ma','Me','Je','Ve','Sa'],
		weekHeader: 'Sm',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['fr-CH']);
});