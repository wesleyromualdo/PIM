/* Slovak initialisation for the jQuery UI date picker plugin. */
/* Written by Vojtech Rinik (vojto@hmm.sk). */
jQuery(function($){
	$.datepicker.regional['sk'] = {
		closeText: 'ZavrieÅ¥',
		prevText: '&#x3c;PredchÃ¡dzajÃºci',
		nextText: 'NasledujÃºci&#x3e;',
		currentText: 'Dnes',
		monthNames: ['JanuÃ¡r','FebruÃ¡r','Marec','AprÃ­l','MÃ¡j','JÃºn',
		'JÃºl','August','September','OktÃ³ber','November','December'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','MÃ¡j','JÃºn',
		'JÃºl','Aug','Sep','Okt','Nov','Dec'],
		dayNames: ['NedeÄ¾a','Pondelok','Utorok','Streda','Å tvrtok','Piatok','Sobota'],
		dayNamesShort: ['Ned','Pon','Uto','Str','Å tv','Pia','Sob'],
		dayNamesMin: ['Ne','Po','Ut','St','Å t','Pia','So'],
		weekHeader: 'Ty',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['sk']);
});
