/* Estonian initialisation for the jQuery UI date picker plugin. */
/* Written by Mart SÃµmermaa (mrts.pydev at gmail com). */
jQuery(function($){
	$.datepicker.regional['et'] = {
		closeText: 'Sulge',
		prevText: 'Eelnev',
		nextText: 'JÃ¤rgnev',
		currentText: 'TÃ¤na',
		monthNames: ['Jaanuar','Veebruar','MÃ¤rts','Aprill','Mai','Juuni',
		'Juuli','August','September','Oktoober','November','Detsember'],
		monthNamesShort: ['Jaan', 'Veebr', 'MÃ¤rts', 'Apr', 'Mai', 'Juuni',
		'Juuli', 'Aug', 'Sept', 'Okt', 'Nov', 'Dets'],
		dayNames: ['PÃ¼hapÃ¤ev', 'EsmaspÃ¤ev', 'TeisipÃ¤ev', 'KolmapÃ¤ev', 'NeljapÃ¤ev', 'Reede', 'LaupÃ¤ev'],
		dayNamesShort: ['PÃ¼hap', 'Esmasp', 'Teisip', 'Kolmap', 'Neljap', 'Reede', 'Laup'],
		dayNamesMin: ['P','E','T','K','N','R','L'],
		weekHeader: 'nÃ¤d',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['et']);
}); 