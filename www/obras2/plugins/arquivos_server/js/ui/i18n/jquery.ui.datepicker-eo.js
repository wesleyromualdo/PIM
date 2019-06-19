/* Esperanto initialisation for the jQuery UI date picker plugin. */
/* Written by Olivier M. (olivierweb@ifrance.com). */
jQuery(function($){
	$.datepicker.regional['eo'] = {
		closeText: 'Fermi',
		prevText: '&lt;Anta',
		nextText: 'Sekv&gt;',
		currentText: 'Nuna',
		monthNames: ['Januaro','Februaro','Marto','Aprilo','Majo','Junio',
		'Julio','AÅ­gusto','Septembro','Oktobro','Novembro','Decembro'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Maj','Jun',
		'Jul','AÅ­g','Sep','Okt','Nov','Dec'],
		dayNames: ['DimanÄo','Lundo','Mardo','Merkredo','Ä´aÅ­do','Vendredo','Sabato'],
		dayNamesShort: ['Dim','Lun','Mar','Mer','Ä´aÅ­','Ven','Sab'],
		dayNamesMin: ['Di','Lu','Ma','Me','Ä´a','Ve','Sa'],
		weekHeader: 'Sb',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['eo']);
});
