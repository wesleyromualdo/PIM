/* Faroese initialisation for the jQuery UI date picker plugin */
/* Written by Sverri Mohr Olsen, sverrimo@gmail.com */
jQuery(function($){
	$.datepicker.regional['fo'] = {
		closeText: 'Lat aftur',
		prevText: '&#x3c;Fyrra',
		nextText: 'NÃ¦sta&#x3e;',
		currentText: 'Ã dag',
		monthNames: ['Januar','Februar','Mars','AprÃ­l','Mei','Juni',
		'Juli','August','September','Oktober','November','Desember'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Mei','Jun',
		'Jul','Aug','Sep','Okt','Nov','Des'],
		dayNames: ['Sunnudagur','MÃ¡nadagur','TÃ½sdagur','Mikudagur','HÃ³sdagur','FrÃ­ggjadagur','Leyardagur'],
		dayNamesShort: ['Sun','MÃ¡n','TÃ½s','Mik','HÃ³s','FrÃ­','Ley'],
		dayNamesMin: ['Su','MÃ¡','TÃ½','Mi','HÃ³','Fr','Le'],
		weekHeader: 'Vk',
		dateFormat: 'dd-mm-yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['fo']);
});
