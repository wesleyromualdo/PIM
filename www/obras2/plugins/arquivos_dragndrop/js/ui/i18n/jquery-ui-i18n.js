/*! jQuery UI - v1.8.20 - 2012-04-30
* https://github.com/jquery/jquery-ui
* Includes: jquery.ui.datepicker-af.js, jquery.ui.datepicker-ar-DZ.js, jquery.ui.datepicker-ar.js, jquery.ui.datepicker-az.js, jquery.ui.datepicker-bg.js, jquery.ui.datepicker-bs.js, jquery.ui.datepicker-ca.js, jquery.ui.datepicker-cs.js, jquery.ui.datepicker-cy-GB.js, jquery.ui.datepicker-da.js, jquery.ui.datepicker-de.js, jquery.ui.datepicker-el.js, jquery.ui.datepicker-en-AU.js, jquery.ui.datepicker-en-GB.js, jquery.ui.datepicker-en-NZ.js, jquery.ui.datepicker-eo.js, jquery.ui.datepicker-es.js, jquery.ui.datepicker-et.js, jquery.ui.datepicker-eu.js, jquery.ui.datepicker-fa.js, jquery.ui.datepicker-fi.js, jquery.ui.datepicker-fo.js, jquery.ui.datepicker-fr-CH.js, jquery.ui.datepicker-fr.js, jquery.ui.datepicker-ge.js, jquery.ui.datepicker-gl.js, jquery.ui.datepicker-he.js, jquery.ui.datepicker-hi.js, jquery.ui.datepicker-hr.js, jquery.ui.datepicker-hu.js, jquery.ui.datepicker-hy.js, jquery.ui.datepicker-id.js, jquery.ui.datepicker-is.js, jquery.ui.datepicker-it.js, jquery.ui.datepicker-ja.js, jquery.ui.datepicker-kk.js, jquery.ui.datepicker-km.js, jquery.ui.datepicker-ko.js, jquery.ui.datepicker-lb.js, jquery.ui.datepicker-lt.js, jquery.ui.datepicker-lv.js, jquery.ui.datepicker-mk.js, jquery.ui.datepicker-ml.js, jquery.ui.datepicker-ms.js, jquery.ui.datepicker-nl-BE.js, jquery.ui.datepicker-nl.js, jquery.ui.datepicker-no.js, jquery.ui.datepicker-pl.js, jquery.ui.datepicker-pt-BR.js, jquery.ui.datepicker-pt.js, jquery.ui.datepicker-rm.js, jquery.ui.datepicker-ro.js, jquery.ui.datepicker-ru.js, jquery.ui.datepicker-sk.js, jquery.ui.datepicker-sl.js, jquery.ui.datepicker-sq.js, jquery.ui.datepicker-sr-SR.js, jquery.ui.datepicker-sr.js, jquery.ui.datepicker-sv.js, jquery.ui.datepicker-ta.js, jquery.ui.datepicker-th.js, jquery.ui.datepicker-tj.js, jquery.ui.datepicker-tr.js, jquery.ui.datepicker-uk.js, jquery.ui.datepicker-vi.js, jquery.ui.datepicker-zh-CN.js, jquery.ui.datepicker-zh-HK.js, jquery.ui.datepicker-zh-TW.js
* Copyright (c) 2012 AUTHORS.txt; Licensed MIT, GPL */

/* Afrikaans initialisation for the jQuery UI date picker plugin. */
/* Written by Renier Pretorius. */
jQuery(function($){
	$.datepicker.regional['af'] = {
		closeText: 'Selekteer',
		prevText: 'Vorige',
		nextText: 'Volgende',
		currentText: 'Vandag',
		monthNames: ['Januarie','Februarie','Maart','April','Mei','Junie',
		'Julie','Augustus','September','Oktober','November','Desember'],
		monthNamesShort: ['Jan', 'Feb', 'Mrt', 'Apr', 'Mei', 'Jun',
		'Jul', 'Aug', 'Sep', 'Okt', 'Nov', 'Des'],
		dayNames: ['Sondag', 'Maandag', 'Dinsdag', 'Woensdag', 'Donderdag', 'Vrydag', 'Saterdag'],
		dayNamesShort: ['Son', 'Maa', 'Din', 'Woe', 'Don', 'Vry', 'Sat'],
		dayNamesMin: ['So','Ma','Di','Wo','Do','Vr','Sa'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['af']);
});

/* Algerian Arabic Translation for jQuery UI date picker plugin. (can be used for Tunisia)*/
/* Mohamed Cherif BOUCHELAGHEM -- cherifbouchelaghem@yahoo.fr */

jQuery(function($){
	$.datepicker.regional['ar-DZ'] = {
		closeText: 'Ø¥ØºÙØ§Ù',
		prevText: '&#x3c;Ø§ÙØ³Ø§Ø¨Ù',
		nextText: 'Ø§ÙØªØ§ÙÙ&#x3e;',
		currentText: 'Ø§ÙÙÙÙ',
		monthNames: ['Ø¬Ø§ÙÙÙ', 'ÙÙÙØ±Ù', 'ÙØ§Ø±Ø³', 'Ø£ÙØ±ÙÙ', 'ÙØ§Ù', 'Ø¬ÙØ§Ù',
		'Ø¬ÙÙÙÙØ©', 'Ø£ÙØª', 'Ø³Ø¨ØªÙØ¨Ø±','Ø£ÙØªÙØ¨Ø±', 'ÙÙÙÙØ¨Ø±', 'Ø¯ÙØ³ÙØ¨Ø±'],
		monthNamesShort: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12'],
		dayNames: ['Ø§ÙØ£Ø­Ø¯', 'Ø§ÙØ§Ø«ÙÙÙ', 'Ø§ÙØ«ÙØ§Ø«Ø§Ø¡', 'Ø§ÙØ£Ø±Ø¨Ø¹Ø§Ø¡', 'Ø§ÙØ®ÙÙØ³', 'Ø§ÙØ¬ÙØ¹Ø©', 'Ø§ÙØ³Ø¨Øª'],
		dayNamesShort: ['Ø§ÙØ£Ø­Ø¯', 'Ø§ÙØ§Ø«ÙÙÙ', 'Ø§ÙØ«ÙØ§Ø«Ø§Ø¡', 'Ø§ÙØ£Ø±Ø¨Ø¹Ø§Ø¡', 'Ø§ÙØ®ÙÙØ³', 'Ø§ÙØ¬ÙØ¹Ø©', 'Ø§ÙØ³Ø¨Øª'],
		dayNamesMin: ['Ø§ÙØ£Ø­Ø¯', 'Ø§ÙØ§Ø«ÙÙÙ', 'Ø§ÙØ«ÙØ§Ø«Ø§Ø¡', 'Ø§ÙØ£Ø±Ø¨Ø¹Ø§Ø¡', 'Ø§ÙØ®ÙÙØ³', 'Ø§ÙØ¬ÙØ¹Ø©', 'Ø§ÙØ³Ø¨Øª'],
		weekHeader: 'Ø£Ø³Ø¨ÙØ¹',
		dateFormat: 'dd/mm/yy',
		firstDay: 6,
  		isRTL: true,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ar-DZ']);
});

/* Arabic Translation for jQuery UI date picker plugin. */
/* Khaled Alhourani -- me@khaledalhourani.com */
/* NOTE: monthNames are the original months names and they are the Arabic names, not the new months name ÙØ¨Ø±Ø§ÙØ± - ÙÙØ§ÙØ± and there isn't any Arabic roots for these months */
jQuery(function($){
	$.datepicker.regional['ar'] = {
		closeText: 'Ø¥ØºÙØ§Ù',
		prevText: '&#x3c;Ø§ÙØ³Ø§Ø¨Ù',
		nextText: 'Ø§ÙØªØ§ÙÙ&#x3e;',
		currentText: 'Ø§ÙÙÙÙ',
		monthNames: ['ÙØ§ÙÙÙ Ø§ÙØ«Ø§ÙÙ', 'Ø´Ø¨Ø§Ø·', 'Ø¢Ø°Ø§Ø±', 'ÙÙØ³Ø§Ù', 'ÙØ§ÙÙ', 'Ø­Ø²ÙØ±Ø§Ù',
		'ØªÙÙØ²', 'Ø¢Ø¨', 'Ø£ÙÙÙÙ',	'ØªØ´Ø±ÙÙ Ø§ÙØ£ÙÙ', 'ØªØ´Ø±ÙÙ Ø§ÙØ«Ø§ÙÙ', 'ÙØ§ÙÙÙ Ø§ÙØ£ÙÙ'],
		monthNamesShort: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12'],
		dayNames: ['Ø§ÙØ£Ø­Ø¯', 'Ø§ÙØ§Ø«ÙÙÙ', 'Ø§ÙØ«ÙØ§Ø«Ø§Ø¡', 'Ø§ÙØ£Ø±Ø¨Ø¹Ø§Ø¡', 'Ø§ÙØ®ÙÙØ³', 'Ø§ÙØ¬ÙØ¹Ø©', 'Ø§ÙØ³Ø¨Øª'],
		dayNamesShort: ['Ø§ÙØ£Ø­Ø¯', 'Ø§ÙØ§Ø«ÙÙÙ', 'Ø§ÙØ«ÙØ§Ø«Ø§Ø¡', 'Ø§ÙØ£Ø±Ø¨Ø¹Ø§Ø¡', 'Ø§ÙØ®ÙÙØ³', 'Ø§ÙØ¬ÙØ¹Ø©', 'Ø§ÙØ³Ø¨Øª'],
		dayNamesMin: ['Ø§ÙØ£Ø­Ø¯', 'Ø§ÙØ§Ø«ÙÙÙ', 'Ø§ÙØ«ÙØ§Ø«Ø§Ø¡', 'Ø§ÙØ£Ø±Ø¨Ø¹Ø§Ø¡', 'Ø§ÙØ®ÙÙØ³', 'Ø§ÙØ¬ÙØ¹Ø©', 'Ø§ÙØ³Ø¨Øª'],
		weekHeader: 'Ø£Ø³Ø¨ÙØ¹',
		dateFormat: 'dd/mm/yy',
		firstDay: 6,
  		isRTL: true,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ar']);
});
/* Azerbaijani (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Jamil Najafov (necefov33@gmail.com). */
jQuery(function($) {
	$.datepicker.regional['az'] = {
		closeText: 'BaÄla',
		prevText: '&#x3c;Geri',
		nextText: 'Ä°rÉli&#x3e;',
		currentText: 'BugÃ¼n',
		monthNames: ['Yanvar','Fevral','Mart','Aprel','May','Ä°yun',
		'Ä°yul','Avqust','Sentyabr','Oktyabr','Noyabr','Dekabr'],
		monthNamesShort: ['Yan','Fev','Mar','Apr','May','Ä°yun',
		'Ä°yul','Avq','Sen','Okt','Noy','Dek'],
		dayNames: ['Bazar','Bazar ertÉsi','ÃÉrÅÉnbÉ axÅamÄ±','ÃÉrÅÉnbÉ','CÃ¼mÉ axÅamÄ±','CÃ¼mÉ','ÅÉnbÉ'],
		dayNamesShort: ['B','Be','Ãa','Ã','Ca','C','Å'],
		dayNamesMin: ['B','B','Ã','Ð¡','Ã','C','Å'],
		weekHeader: 'Hf',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['az']);
});
/* Bulgarian initialisation for the jQuery UI date picker plugin. */
/* Written by Stoyan Kyosev (http://svest.org). */
jQuery(function($){
    $.datepicker.regional['bg'] = {
        closeText: 'Ð·Ð°ÑÐ²Ð¾ÑÐ¸',
        prevText: '&#x3c;Ð½Ð°Ð·Ð°Ð´',
        nextText: 'Ð½Ð°Ð¿ÑÐµÐ´&#x3e;',
		nextBigText: '&#x3e;&#x3e;',
        currentText: 'Ð´Ð½ÐµÑ',
        monthNames: ['Ð¯Ð½ÑÐ°ÑÐ¸','Ð¤ÐµÐ²ÑÑÐ°ÑÐ¸','ÐÐ°ÑÑ','ÐÐ¿ÑÐ¸Ð»','ÐÐ°Ð¹','Ð®Ð½Ð¸',
        'Ð®Ð»Ð¸','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ¿ÑÐµÐ¼Ð²ÑÐ¸','ÐÐºÑÐ¾Ð¼Ð²ÑÐ¸','ÐÐ¾ÐµÐ¼Ð²ÑÐ¸','ÐÐµÐºÐµÐ¼Ð²ÑÐ¸'],
        monthNamesShort: ['Ð¯Ð½Ñ','Ð¤ÐµÐ²','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ð¹','Ð®Ð½Ð¸',
        'Ð®Ð»Ð¸','ÐÐ²Ð³','Ð¡ÐµÐ¿','ÐÐºÑ','ÐÐ¾Ð²','ÐÐµÐº'],
        dayNames: ['ÐÐµÐ´ÐµÐ»Ñ','ÐÐ¾Ð½ÐµÐ´ÐµÐ»Ð½Ð¸Ðº','ÐÑÐ¾ÑÐ½Ð¸Ðº','Ð¡ÑÑÐ´Ð°','Ð§ÐµÑÐ²ÑÑÑÑÐº','ÐÐµÑÑÐº','Ð¡ÑÐ±Ð¾ÑÐ°'],
        dayNamesShort: ['ÐÐµÐ´','ÐÐ¾Ð½','ÐÑÐ¾','Ð¡ÑÑ','Ð§ÐµÑ','ÐÐµÑ','Ð¡ÑÐ±'],
        dayNamesMin: ['ÐÐµ','ÐÐ¾','ÐÑ','Ð¡Ñ','Ð§Ðµ','ÐÐµ','Ð¡Ñ'],
		weekHeader: 'Wk',
        dateFormat: 'dd.mm.yy',
		firstDay: 1,
        isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
    $.datepicker.setDefaults($.datepicker.regional['bg']);
});

/* Bosnian i18n for the jQuery UI date picker plugin. */
/* Written by Kenan Konjo. */
jQuery(function($){
	$.datepicker.regional['bs'] = {
		closeText: 'Zatvori', 
		prevText: '&#x3c;', 
		nextText: '&#x3e;', 
		currentText: 'Danas', 
		monthNames: ['Januar','Februar','Mart','April','Maj','Juni',
		'Juli','August','Septembar','Oktobar','Novembar','Decembar'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Maj','Jun',
		'Jul','Aug','Sep','Okt','Nov','Dec'],
		dayNames: ['Nedelja','Ponedeljak','Utorak','Srijeda','Äetvrtak','Petak','Subota'],
		dayNamesShort: ['Ned','Pon','Uto','Sri','Äet','Pet','Sub'],
		dayNamesMin: ['Ne','Po','Ut','Sr','Äe','Pe','Su'],
		weekHeader: 'Wk',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['bs']);
});
/* InicialitzaciÃ³ en catalÃ  per a l'extenciÃ³ 'calendar' per jQuery. */
/* Writers: (joan.leon@gmail.com). */
jQuery(function($){
	$.datepicker.regional['ca'] = {
		closeText: 'Tancar',
		prevText: '&#x3c;Ant',
		nextText: 'Seg&#x3e;',
		currentText: 'Avui',
		monthNames: ['Gener','Febrer','Mar&ccedil;','Abril','Maig','Juny',
		'Juliol','Agost','Setembre','Octubre','Novembre','Desembre'],
		monthNamesShort: ['Gen','Feb','Mar','Abr','Mai','Jun',
		'Jul','Ago','Set','Oct','Nov','Des'],
		dayNames: ['Diumenge','Dilluns','Dimarts','Dimecres','Dijous','Divendres','Dissabte'],
		dayNamesShort: ['Dug','Dln','Dmt','Dmc','Djs','Dvn','Dsb'],
		dayNamesMin: ['Dg','Dl','Dt','Dc','Dj','Dv','Ds'],
		weekHeader: 'Sm',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ca']);
});
/* Czech initialisation for the jQuery UI date picker plugin. */
/* Written by Tomas Muller (tomas@tomas-muller.net). */
jQuery(function($){
	$.datepicker.regional['cs'] = {
		closeText: 'ZavÅÃ­t',
		prevText: '&#x3c;DÅÃ­ve',
		nextText: 'PozdÄji&#x3e;',
		currentText: 'NynÃ­',
		monthNames: ['leden','Ãºnor','bÅezen','duben','kvÄten','Äerven',
        'Äervenec','srpen','zÃ¡ÅÃ­','ÅÃ­jen','listopad','prosinec'],
		monthNamesShort: ['led','Ãºno','bÅe','dub','kvÄ','Äer',
		'Ävc','srp','zÃ¡Å','ÅÃ­j','lis','pro'],
		dayNames: ['nedÄle', 'pondÄlÃ­', 'ÃºterÃ½', 'stÅeda', 'Ätvrtek', 'pÃ¡tek', 'sobota'],
		dayNamesShort: ['ne', 'po', 'Ãºt', 'st', 'Ät', 'pÃ¡', 'so'],
		dayNamesMin: ['ne','po','Ãºt','st','Ät','pÃ¡','so'],
		weekHeader: 'TÃ½d',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['cs']);
});

/* Welsh/UK initialisation for the jQuery UI date picker plugin. */
/* Written by William Griffiths. */
jQuery(function($){
	$.datepicker.regional['cy-GB'] = {
		closeText: 'Done',
		prevText: 'Prev',
		nextText: 'Next',
		currentText: 'Today',
		monthNames: ['Ionawr','Chwefror','Mawrth','Ebrill','Mai','Mehefin',
		'Gorffennaf','Awst','Medi','Hydref','Tachwedd','Rhagfyr'],
		monthNamesShort: ['Ion', 'Chw', 'Maw', 'Ebr', 'Mai', 'Meh',
		'Gor', 'Aws', 'Med', 'Hyd', 'Tac', 'Rha'],
		dayNames: ['Dydd Sul', 'Dydd Llun', 'Dydd Mawrth', 'Dydd Mercher', 'Dydd Iau', 'Dydd Gwener', 'Dydd Sadwrn'],
		dayNamesShort: ['Sul', 'Llu', 'Maw', 'Mer', 'Iau', 'Gwe', 'Sad'],
		dayNamesMin: ['Su','Ll','Ma','Me','Ia','Gw','Sa'],
		weekHeader: 'Wy',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['cy-GB']);
});
/* Danish initialisation for the jQuery UI date picker plugin. */
/* Written by Jan Christensen ( deletestuff@gmail.com). */
jQuery(function($){
    $.datepicker.regional['da'] = {
		closeText: 'Luk',
        prevText: '&#x3c;Forrige',
		nextText: 'NÃ¦ste&#x3e;',
		currentText: 'Idag',
        monthNames: ['Januar','Februar','Marts','April','Maj','Juni',
        'Juli','August','September','Oktober','November','December'],
        monthNamesShort: ['Jan','Feb','Mar','Apr','Maj','Jun',
        'Jul','Aug','Sep','Okt','Nov','Dec'],
		dayNames: ['SÃ¸ndag','Mandag','Tirsdag','Onsdag','Torsdag','Fredag','LÃ¸rdag'],
		dayNamesShort: ['SÃ¸n','Man','Tir','Ons','Tor','Fre','LÃ¸r'],
		dayNamesMin: ['SÃ¸','Ma','Ti','On','To','Fr','LÃ¸'],
		weekHeader: 'Uge',
        dateFormat: 'dd-mm-yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
    $.datepicker.setDefaults($.datepicker.regional['da']);
});

/* German initialisation for the jQuery UI date picker plugin. */
/* Written by Milian Wolff (mail@milianw.de). */
jQuery(function($){
	$.datepicker.regional['de'] = {
		closeText: 'schlieÃen',
		prevText: '&#x3c;zurÃ¼ck',
		nextText: 'Vor&#x3e;',
		currentText: 'heute',
		monthNames: ['Januar','Februar','MÃ¤rz','April','Mai','Juni',
		'Juli','August','September','Oktober','November','Dezember'],
		monthNamesShort: ['Jan','Feb','MÃ¤r','Apr','Mai','Jun',
		'Jul','Aug','Sep','Okt','Nov','Dez'],
		dayNames: ['Sonntag','Montag','Dienstag','Mittwoch','Donnerstag','Freitag','Samstag'],
		dayNamesShort: ['So','Mo','Di','Mi','Do','Fr','Sa'],
		dayNamesMin: ['So','Mo','Di','Mi','Do','Fr','Sa'],
		weekHeader: 'KW',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['de']);
});

/* Greek (el) initialisation for the jQuery UI date picker plugin. */
/* Written by Alex Cicovic (http://www.alexcicovic.com) */
jQuery(function($){
	$.datepicker.regional['el'] = {
		closeText: 'ÎÎ»ÎµÎ¯ÏÎ¹Î¼Î¿',
		prevText: 'Î ÏÎ¿Î·Î³Î¿ÏÎ¼ÎµÎ½Î¿Ï',
		nextText: 'ÎÏÏÎ¼ÎµÎ½Î¿Ï',
		currentText: 'Î¤ÏÎ­ÏÏÎ½ ÎÎ®Î½Î±Ï',
		monthNames: ['ÎÎ±Î½Î¿ÏÎ¬ÏÎ¹Î¿Ï','Î¦ÎµÎ²ÏÎ¿ÏÎ¬ÏÎ¹Î¿Ï','ÎÎ¬ÏÏÎ¹Î¿Ï','ÎÏÏÎ¯Î»Î¹Î¿Ï','ÎÎ¬Î¹Î¿Ï','ÎÎ¿ÏÎ½Î¹Î¿Ï',
		'ÎÎ¿ÏÎ»Î¹Î¿Ï','ÎÏÎ³Î¿ÏÏÏÎ¿Ï','Î£ÎµÏÏÎ­Î¼Î²ÏÎ¹Î¿Ï','ÎÎºÏÏÎ²ÏÎ¹Î¿Ï','ÎÎ¿Î­Î¼Î²ÏÎ¹Î¿Ï','ÎÎµÎºÎ­Î¼Î²ÏÎ¹Î¿Ï'],
		monthNamesShort: ['ÎÎ±Î½','Î¦ÎµÎ²','ÎÎ±Ï','ÎÏÏ','ÎÎ±Î¹','ÎÎ¿ÏÎ½',
		'ÎÎ¿ÏÎ»','ÎÏÎ³','Î£ÎµÏ','ÎÎºÏ','ÎÎ¿Îµ','ÎÎµÎº'],
		dayNames: ['ÎÏÏÎ¹Î±ÎºÎ®','ÎÎµÏÏÎ­ÏÎ±','Î¤ÏÎ¯ÏÎ·','Î¤ÎµÏÎ¬ÏÏÎ·','Î Î­Î¼ÏÏÎ·','Î Î±ÏÎ±ÏÎºÎµÏÎ®','Î£Î¬Î²Î²Î±ÏÎ¿'],
		dayNamesShort: ['ÎÏÏ','ÎÎµÏ','Î¤ÏÎ¹','Î¤ÎµÏ','Î ÎµÎ¼','Î Î±Ï','Î£Î±Î²'],
		dayNamesMin: ['ÎÏ','ÎÎµ','Î¤Ï','Î¤Îµ','Î Îµ','Î Î±','Î£Î±'],
		weekHeader: 'ÎÎ²Î´',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['el']);
});
/* English/Australia initialisation for the jQuery UI date picker plugin. */
/* Based on the en-GB initialisation. */
jQuery(function($){
	$.datepicker.regional['en-AU'] = {
		closeText: 'Done',
		prevText: 'Prev',
		nextText: 'Next',
		currentText: 'Today',
		monthNames: ['January','February','March','April','May','June',
		'July','August','September','October','November','December'],
		monthNamesShort: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
		'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
		dayNames: ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'],
		dayNamesShort: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
		dayNamesMin: ['Su','Mo','Tu','We','Th','Fr','Sa'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['en-AU']);
});

/* English/UK initialisation for the jQuery UI date picker plugin. */
/* Written by Stuart. */
jQuery(function($){
	$.datepicker.regional['en-GB'] = {
		closeText: 'Done',
		prevText: 'Prev',
		nextText: 'Next',
		currentText: 'Today',
		monthNames: ['January','February','March','April','May','June',
		'July','August','September','October','November','December'],
		monthNamesShort: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
		'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
		dayNames: ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'],
		dayNamesShort: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
		dayNamesMin: ['Su','Mo','Tu','We','Th','Fr','Sa'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['en-GB']);
});

/* English/New Zealand initialisation for the jQuery UI date picker plugin. */
/* Based on the en-GB initialisation. */
jQuery(function($){
	$.datepicker.regional['en-NZ'] = {
		closeText: 'Done',
		prevText: 'Prev',
		nextText: 'Next',
		currentText: 'Today',
		monthNames: ['January','February','March','April','May','June',
		'July','August','September','October','November','December'],
		monthNamesShort: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
		'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
		dayNames: ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'],
		dayNamesShort: ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
		dayNamesMin: ['Su','Mo','Tu','We','Th','Fr','Sa'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['en-NZ']);
});

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

/* InicializaciÃ³n en espaÃ±ol para la extensiÃ³n 'UI date picker' para jQuery. */
/* Traducido por Vester (xvester@gmail.com). */
jQuery(function($){
	$.datepicker.regional['es'] = {
		closeText: 'Cerrar',
		prevText: '&#x3c;Ant',
		nextText: 'Sig&#x3e;',
		currentText: 'Hoy',
		monthNames: ['Enero','Febrero','Marzo','Abril','Mayo','Junio',
		'Julio','Agosto','Septiembre','Octubre','Noviembre','Diciembre'],
		monthNamesShort: ['Ene','Feb','Mar','Abr','May','Jun',
		'Jul','Ago','Sep','Oct','Nov','Dic'],
		dayNames: ['Domingo','Lunes','Martes','Mi&eacute;rcoles','Jueves','Viernes','S&aacute;bado'],
		dayNamesShort: ['Dom','Lun','Mar','Mi&eacute;','Juv','Vie','S&aacute;b'],
		dayNamesMin: ['Do','Lu','Ma','Mi','Ju','Vi','S&aacute;'],
		weekHeader: 'Sm',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['es']);
});
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
/* Euskarako oinarria 'UI date picker' jquery-ko extentsioarentzat */
/* Karrikas-ek itzulia (karrikas@karrikas.com) */
jQuery(function($){
	$.datepicker.regional['eu'] = {
		closeText: 'Egina',
		prevText: '&#x3c;Aur',
		nextText: 'Hur&#x3e;',
		currentText: 'Gaur',
		monthNames: ['Urtarrila','Otsaila','Martxoa','Apirila','Maiatza','Ekaina',
		'Uztaila','Abuztua','Iraila','Urria','Azaroa','Abendua'],
		monthNamesShort: ['Urt','Ots','Mar','Api','Mai','Eka',
		'Uzt','Abu','Ira','Urr','Aza','Abe'],
		dayNames: ['Igandea','Astelehena','Asteartea','Asteazkena','Osteguna','Ostirala','Larunbata'],
		dayNamesShort: ['Iga','Ast','Ast','Ast','Ost','Ost','Lar'],
		dayNamesMin: ['Ig','As','As','As','Os','Os','La'],
		weekHeader: 'Wk',
		dateFormat: 'yy/mm/dd',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['eu']);
});
/* Persian (Farsi) Translation for the jQuery UI date picker plugin. */
/* Javad Mowlanezhad -- jmowla@gmail.com */
/* Jalali calendar should supported soon! (Its implemented but I have to test it) */
jQuery(function($) {
	$.datepicker.regional['fa'] = {
		closeText: 'Ø¨Ø³ØªÙ',
		prevText: '&#x3C;ÙØ¨ÙÛ',
		nextText: 'Ø¨Ø¹Ø¯Û&#x3E;',
		currentText: 'Ø§ÙØ±ÙØ²',
		monthNames: [
			'ÙØ±ÙØ±Ø¯ÙÙ',
			'Ø§Ø±Ø¯ÙØ¨ÙØ´Øª',
			'Ø®Ø±Ø¯Ø§Ø¯',
			'ØªÙØ±',
			'ÙØ±Ø¯Ø§Ø¯',
			'Ø´ÙØ±ÙÙØ±',
			'ÙÙØ±',
			'Ø¢Ø¨Ø§Ù',
			'Ø¢Ø°Ø±',
			'Ø¯Û',
			'Ø¨ÙÙÙ',
			'Ø§Ø³ÙÙØ¯'
		],
		monthNamesShort: ['1','2','3','4','5','6','7','8','9','10','11','12'],
		dayNames: [
			'ÙÚ©Ø´ÙØ¨Ù',
			'Ø¯ÙØ´ÙØ¨Ù',
			'Ø³ÙâØ´ÙØ¨Ù',
			'ÚÙØ§Ø±Ø´ÙØ¨Ù',
			'Ù¾ÙØ¬Ø´ÙØ¨Ù',
			'Ø¬ÙØ¹Ù',
			'Ø´ÙØ¨Ù'
		],
		dayNamesShort: [
			'Û',
			'Ø¯',
			'Ø³',
			'Ú',
			'Ù¾',
			'Ø¬', 
			'Ø´'
		],
		dayNamesMin: [
			'Û',
			'Ø¯',
			'Ø³',
			'Ú',
			'Ù¾',
			'Ø¬', 
			'Ø´'
		],
		weekHeader: 'ÙÙ',
		dateFormat: 'yy/mm/dd',
		firstDay: 6,
		isRTL: true,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['fa']);
});
/* Finnish initialisation for the jQuery UI date picker plugin. */
/* Written by Harri KilpiÃ¶ (harrikilpio@gmail.com). */
jQuery(function($){
	$.datepicker.regional['fi'] = {
		closeText: 'Sulje',
		prevText: '&#xAB;Edellinen',
		nextText: 'Seuraava&#xBB;',
		currentText: 'T&#xE4;n&#xE4;&#xE4;n',
		monthNames: ['Tammikuu','Helmikuu','Maaliskuu','Huhtikuu','Toukokuu','Kes&#xE4;kuu',
		'Hein&#xE4;kuu','Elokuu','Syyskuu','Lokakuu','Marraskuu','Joulukuu'],
		monthNamesShort: ['Tammi','Helmi','Maalis','Huhti','Touko','Kes&#xE4;',
		'Hein&#xE4;','Elo','Syys','Loka','Marras','Joulu'],
		dayNamesShort: ['Su','Ma','Ti','Ke','To','Pe','La'],
		dayNames: ['Sunnuntai','Maanantai','Tiistai','Keskiviikko','Torstai','Perjantai','Lauantai'],
		dayNamesMin: ['Su','Ma','Ti','Ke','To','Pe','La'],
		weekHeader: 'Vk',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['fi']);
});

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
/* French initialisation for the jQuery UI date picker plugin. */
/* Written by Keith Wood (kbwood{at}iinet.com.au),
              StÃ©phane Nahmani (sholby@sholby.net),
              StÃ©phane Raimbault <stephane.raimbault@gmail.com> */
jQuery(function($){
	$.datepicker.regional['fr'] = {
		closeText: 'Fermer',
		prevText: 'PrÃ©cÃ©dent',
		nextText: 'Suivant',
		currentText: 'Aujourd\'hui',
		monthNames: ['Janvier','FÃ©vrier','Mars','Avril','Mai','Juin',
		'Juillet','AoÃ»t','Septembre','Octobre','Novembre','DÃ©cembre'],
		monthNamesShort: ['Janv.','FÃ©vr.','Mars','Avril','Mai','Juin',
		'Juil.','AoÃ»t','Sept.','Oct.','Nov.','DÃ©c.'],
		dayNames: ['Dimanche','Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi'],
		dayNamesShort: ['Dim.','Lun.','Mar.','Mer.','Jeu.','Ven.','Sam.'],
		dayNamesMin: ['D','L','M','M','J','V','S'],
		weekHeader: 'Sem.',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['fr']);
});

/* Georgian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Lado Lomidze (lado.lomidze@gmail.com). */
jQuery(function($){
	$.datepicker.regional['ge'] = {
		closeText: 'ááá®á£á áá',
		prevText: '&#x3c; á¬ááá',
		nextText: 'á¨áááááá &#x3e;',
		currentText: 'áá¦áá¡',
		monthNames: ['áááááá á','ááááá áááá','ááá á¢á','ááá ááá','áááá¡á','ááááá¡á', 'ááááá¡á','ááááá¡á¢á','á¡áá¥á¢ááááá á','áá¥á¢ááááá á','ááááááá á','áááááááá á'],
		monthNamesShort: ['ááá','ááá','ááá ','ááá ','ááá','ááá', 'ááá','ááá','á¡áá¥','áá¥á¢','ááá','ááá'],
		dayNames: ['áááá á','áá á¨ááááá','á¡ááá¨ááááá','ááá®á¨ááááá','á®á£áá¨ááááá','ááá áá¡áááá','á¨ááááá'],
		dayNamesShort: ['áá','áá á¨','á¡áá','ááá®','á®á£á','ááá ','á¨áá'],
		dayNamesMin: ['áá','áá á¨','á¡áá','ááá®','á®á£á','ááá ','á¨áá'],
		weekHeader: 'áááá á',
		dateFormat: 'dd-mm-yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ge']);
});

/* Galician localization for 'UI date picker' jQuery extension. */
/* Translated by Jorge Barreiro <yortx.barry@gmail.com>. */
jQuery(function($){
	$.datepicker.regional['gl'] = {
		closeText: 'Pechar',
		prevText: '&#x3c;Ant',
		nextText: 'Seg&#x3e;',
		currentText: 'Hoxe',
		monthNames: ['Xaneiro','Febreiro','Marzo','Abril','Maio','XuÃ±o',
		'Xullo','Agosto','Setembro','Outubro','Novembro','Decembro'],
		monthNamesShort: ['Xan','Feb','Mar','Abr','Mai','XuÃ±',
		'Xul','Ago','Set','Out','Nov','Dec'],
		dayNames: ['Domingo','Luns','Martes','M&eacute;rcores','Xoves','Venres','S&aacute;bado'],
		dayNamesShort: ['Dom','Lun','Mar','M&eacute;r','Xov','Ven','S&aacute;b'],
		dayNamesMin: ['Do','Lu','Ma','M&eacute;','Xo','Ve','S&aacute;'],
		weekHeader: 'Sm',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['gl']);
});
/* Hebrew initialisation for the UI Datepicker extension. */
/* Written by Amir Hardon (ahardon at gmail dot com). */
jQuery(function($){
	$.datepicker.regional['he'] = {
		closeText: '×¡×××¨',
		prevText: '&#x3c;××§×××',
		nextText: '×××&#x3e;',
		currentText: '××××',
		monthNames: ['×× ×××¨','×¤××¨×××¨','××¨×¥','××¤×¨××','×××','××× ×',
		'××××','×××××¡×','×¡×¤××××¨','×××§××××¨','× ×××××¨','××¦×××¨'],
		monthNamesShort: ['×× ×','×¤××¨','××¨×¥','××¤×¨','×××','××× ×',
		'××××','×××','×¡×¤×','×××§','× ××','××¦×'],
		dayNames: ['×¨××©××','×©× ×','×©×××©×','×¨×××¢×','××××©×','×©××©×','×©××ª'],
		dayNamesShort: ['×\'','×\'','×\'','×\'','×\'','×\'','×©××ª'],
		dayNamesMin: ['×\'','×\'','×\'','×\'','×\'','×\'','×©××ª'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: true,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['he']);
});

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

/* Croatian i18n for the jQuery UI date picker plugin. */
/* Written by Vjekoslav Nesek. */
jQuery(function($){
	$.datepicker.regional['hr'] = {
		closeText: 'Zatvori',
		prevText: '&#x3c;',
		nextText: '&#x3e;',
		currentText: 'Danas',
		monthNames: ['SijeÄanj','VeljaÄa','OÅ¾ujak','Travanj','Svibanj','Lipanj',
		'Srpanj','Kolovoz','Rujan','Listopad','Studeni','Prosinac'],
		monthNamesShort: ['Sij','Velj','OÅ¾u','Tra','Svi','Lip',
		'Srp','Kol','Ruj','Lis','Stu','Pro'],
		dayNames: ['Nedjelja','Ponedjeljak','Utorak','Srijeda','Äetvrtak','Petak','Subota'],
		dayNamesShort: ['Ned','Pon','Uto','Sri','Äet','Pet','Sub'],
		dayNamesMin: ['Ne','Po','Ut','Sr','Äe','Pe','Su'],
		weekHeader: 'Tje',
		dateFormat: 'dd.mm.yy.',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['hr']);
});
/* Hungarian initialisation for the jQuery UI date picker plugin. */
/* Written by Istvan Karaszi (jquery@spam.raszi.hu). */
jQuery(function($){
	$.datepicker.regional['hu'] = {
		closeText: 'bezÃ¡r',
		prevText: 'vissza',
		nextText: 'elÅre',
		currentText: 'ma',
		monthNames: ['JanuÃ¡r', 'FebruÃ¡r', 'MÃ¡rcius', 'Ãprilis', 'MÃ¡jus', 'JÃºnius',
		'JÃºlius', 'Augusztus', 'Szeptember', 'OktÃ³ber', 'November', 'December'],
		monthNamesShort: ['Jan', 'Feb', 'MÃ¡r', 'Ãpr', 'MÃ¡j', 'JÃºn',
		'JÃºl', 'Aug', 'Szep', 'Okt', 'Nov', 'Dec'],
		dayNames: ['VasÃ¡rnap', 'HÃ©tfÅ', 'Kedd', 'Szerda', 'CsÃ¼tÃ¶rtÃ¶k', 'PÃ©ntek', 'Szombat'],
		dayNamesShort: ['Vas', 'HÃ©t', 'Ked', 'Sze', 'CsÃ¼', 'PÃ©n', 'Szo'],
		dayNamesMin: ['V', 'H', 'K', 'Sze', 'Cs', 'P', 'Szo'],
		weekHeader: 'HÃ©t',
		dateFormat: 'yy.mm.dd.',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['hu']);
});

/* Armenian(UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Levon Zakaryan (levon.zakaryan@gmail.com)*/
jQuery(function($){
	$.datepicker.regional['hy'] = {
		closeText: 'ÕÕ¡Õ¯Õ¥Õ¬',
		prevText: '&#x3c;ÕÕ¡Õ­.',
		nextText: 'ÕÕ¡Õ».&#x3e;',
		currentText: 'Ô±ÕµÕ½ÖÖ',
		monthNames: ['ÕÕ¸ÖÕ¶Õ¾Õ¡Ö','ÕÕ¥Õ¿ÖÕ¾Õ¡Ö','ÕÕ¡ÖÕ¿','Ô±ÕºÖÕ«Õ¬','ÕÕ¡ÕµÕ«Õ½','ÕÕ¸ÖÕ¶Õ«Õ½',
		'ÕÕ¸ÖÕ¬Õ«Õ½','ÕÕ£Õ¸Õ½Õ¿Õ¸Õ½','ÕÕ¥ÕºÕ¿Õ¥Õ´Õ¢Õ¥Ö','ÕÕ¸Õ¯Õ¿Õ¥Õ´Õ¢Õ¥Ö','ÕÕ¸ÕµÕ¥Õ´Õ¢Õ¥Ö','Ô´Õ¥Õ¯Õ¿Õ¥Õ´Õ¢Õ¥Ö'],
		monthNamesShort: ['ÕÕ¸ÖÕ¶Õ¾','ÕÕ¥Õ¿Ö','ÕÕ¡ÖÕ¿','Ô±ÕºÖ','ÕÕ¡ÕµÕ«Õ½','ÕÕ¸ÖÕ¶Õ«Õ½',
		'ÕÕ¸ÖÕ¬','ÕÕ£Õ½','ÕÕ¥Õº','ÕÕ¸Õ¯','ÕÕ¸Õµ','Ô´Õ¥Õ¯'],
		dayNames: ['Õ¯Õ«ÖÕ¡Õ¯Õ«','Õ¥Õ¯Õ¸ÖÕ·Õ¡Õ¢Õ©Õ«','Õ¥ÖÕ¥ÖÕ·Õ¡Õ¢Õ©Õ«','Õ¹Õ¸ÖÕ¥ÖÕ·Õ¡Õ¢Õ©Õ«','Õ°Õ«Õ¶Õ£Õ·Õ¡Õ¢Õ©Õ«','Õ¸ÖÖÕ¢Õ¡Õ©','Õ·Õ¡Õ¢Õ¡Õ©'],
		dayNamesShort: ['Õ¯Õ«Ö','Õ¥ÖÕ¯','Õ¥ÖÖ','Õ¹ÖÖ','Õ°Õ¶Õ£','Õ¸ÖÖÕ¢','Õ·Õ¢Õ©'],
		dayNamesMin: ['Õ¯Õ«Ö','Õ¥ÖÕ¯','Õ¥ÖÖ','Õ¹ÖÖ','Õ°Õ¶Õ£','Õ¸ÖÖÕ¢','Õ·Õ¢Õ©'],
		weekHeader: 'ÕÔ²Õ',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['hy']);
});
/* Indonesian initialisation for the jQuery UI date picker plugin. */
/* Written by Deden Fathurahman (dedenf@gmail.com). */
jQuery(function($){
	$.datepicker.regional['id'] = {
		closeText: 'Tutup',
		prevText: '&#x3c;mundur',
		nextText: 'maju&#x3e;',
		currentText: 'hari ini',
		monthNames: ['Januari','Februari','Maret','April','Mei','Juni',
		'Juli','Agustus','September','Oktober','Nopember','Desember'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Mei','Jun',
		'Jul','Agus','Sep','Okt','Nop','Des'],
		dayNames: ['Minggu','Senin','Selasa','Rabu','Kamis','Jumat','Sabtu'],
		dayNamesShort: ['Min','Sen','Sel','Rab','kam','Jum','Sab'],
		dayNamesMin: ['Mg','Sn','Sl','Rb','Km','jm','Sb'],
		weekHeader: 'Mg',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['id']);
});
/* Icelandic initialisation for the jQuery UI date picker plugin. */
/* Written by Haukur H. Thorsson (haukur@eskill.is). */
jQuery(function($){
	$.datepicker.regional['is'] = {
		closeText: 'Loka',
		prevText: '&#x3c; Fyrri',
		nextText: 'N&aelig;sti &#x3e;',
		currentText: '&Iacute; dag',
		monthNames: ['Jan&uacute;ar','Febr&uacute;ar','Mars','Apr&iacute;l','Ma&iacute','J&uacute;n&iacute;',
		'J&uacute;l&iacute;','&Aacute;g&uacute;st','September','Okt&oacute;ber','N&oacute;vember','Desember'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Ma&iacute;','J&uacute;n',
		'J&uacute;l','&Aacute;g&uacute;','Sep','Okt','N&oacute;v','Des'],
		dayNames: ['Sunnudagur','M&aacute;nudagur','&THORN;ri&eth;judagur','Mi&eth;vikudagur','Fimmtudagur','F&ouml;studagur','Laugardagur'],
		dayNamesShort: ['Sun','M&aacute;n','&THORN;ri','Mi&eth;','Fim','F&ouml;s','Lau'],
		dayNamesMin: ['Su','M&aacute;','&THORN;r','Mi','Fi','F&ouml;','La'],
		weekHeader: 'Vika',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['is']);
});
/* Italian initialisation for the jQuery UI date picker plugin. */
/* Written by Antonello Pasella (antonello.pasella@gmail.com). */
jQuery(function($){
	$.datepicker.regional['it'] = {
		closeText: 'Chiudi',
		prevText: '&#x3c;Prec',
		nextText: 'Succ&#x3e;',
		currentText: 'Oggi',
		monthNames: ['Gennaio','Febbraio','Marzo','Aprile','Maggio','Giugno',
			'Luglio','Agosto','Settembre','Ottobre','Novembre','Dicembre'],
		monthNamesShort: ['Gen','Feb','Mar','Apr','Mag','Giu',
			'Lug','Ago','Set','Ott','Nov','Dic'],
		dayNames: ['Domenica','Luned&#236','Marted&#236','Mercoled&#236','Gioved&#236','Venerd&#236','Sabato'],
		dayNamesShort: ['Dom','Lun','Mar','Mer','Gio','Ven','Sab'],
		dayNamesMin: ['Do','Lu','Ma','Me','Gi','Ve','Sa'],
		weekHeader: 'Sm',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['it']);
});

/* Japanese initialisation for the jQuery UI date picker plugin. */
/* Written by Kentaro SATO (kentaro@ranvis.com). */
jQuery(function($){
	$.datepicker.regional['ja'] = {
		closeText: 'éãã',
		prevText: '&#x3c;å',
		nextText: 'æ¬¡&#x3e;',
		currentText: 'ä»æ¥',
		monthNames: ['1æ','2æ','3æ','4æ','5æ','6æ',
		'7æ','8æ','9æ','10æ','11æ','12æ'],
		monthNamesShort: ['1æ','2æ','3æ','4æ','5æ','6æ',
		'7æ','8æ','9æ','10æ','11æ','12æ'],
		dayNames: ['æ¥ææ¥','æææ¥','ç«ææ¥','æ°´ææ¥','æ¨ææ¥','éææ¥','åææ¥'],
		dayNamesShort: ['æ¥','æ','ç«','æ°´','æ¨','é','å'],
		dayNamesMin: ['æ¥','æ','ç«','æ°´','æ¨','é','å'],
		weekHeader: 'é±',
		dateFormat: 'yy/mm/dd',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: 'å¹´'};
	$.datepicker.setDefaults($.datepicker.regional['ja']);
});
/* Kazakh (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Dmitriy Karasyov (dmitriy.karasyov@gmail.com). */
jQuery(function($){
	$.datepicker.regional['kk'] = {
		closeText: 'ÐÐ°Ð±Ñ',
		prevText: '&#x3c;ÐÐ»Ð´ÑÒ£ÒÑ',
		nextText: 'ÐÐµÐ»ÐµÑÑ&#x3e;',
		currentText: 'ÐÒ¯Ð³ÑÐ½',
		monthNames: ['ÒÐ°Ò£ÑÐ°Ñ','ÐÒÐ¿Ð°Ð½','ÐÐ°ÑÑÑÐ·','Ð¡ÓÑÑÑ','ÐÐ°Ð¼ÑÑ','ÐÐ°ÑÑÑÐ¼',
		'Ð¨ÑÐ»Ð´Ðµ','Ð¢Ð°Ð¼ÑÐ·','ÒÑÑÐºÒ¯Ð¹ÐµÐº','ÒÐ°Ð·Ð°Ð½','ÒÐ°ÑÐ°ÑÐ°','ÐÐµÐ»ÑÐ¾ÒÑÐ°Ð½'],
		monthNamesShort: ['ÒÐ°Ò£','ÐÒÐ¿','ÐÐ°Ñ','Ð¡ÓÑ','ÐÐ°Ð¼','ÐÐ°Ñ',
		'Ð¨ÑÐ»','Ð¢Ð°Ð¼','ÒÑÑ','ÒÐ°Ð·','ÒÐ°Ñ','ÐÐµÐ»'],
		dayNames: ['ÐÐµÐºÑÐµÐ½Ð±Ñ','ÐÒ¯Ð¹ÑÐµÐ½Ð±Ñ','Ð¡ÐµÐ¹ÑÐµÐ½Ð±Ñ','Ð¡ÓÑÑÐµÐ½Ð±Ñ','ÐÐµÐ¹ÑÐµÐ½Ð±Ñ','ÐÒ±Ð¼Ð°','Ð¡ÐµÐ½Ð±Ñ'],
		dayNamesShort: ['Ð¶ÐºÑ','Ð´ÑÐ½','ÑÑÐ½','ÑÑÑ','Ð±ÑÐ½','Ð¶Ð¼Ð°','ÑÐ½Ð±'],
		dayNamesMin: ['ÐÐº','ÐÑ','Ð¡Ñ','Ð¡Ñ','ÐÑ','ÐÐ¼','Ð¡Ð½'],
		weekHeader: 'ÐÐµ',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['kk']);
});

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

/* Korean initialisation for the jQuery calendar extension. */
/* Written by DaeKwon Kang (ncrash.dk@gmail.com), Edited by Genie. */
jQuery(function($){
	$.datepicker.regional['ko'] = {
		closeText: 'ë«ê¸°',
		prevText: 'ì´ì ë¬',
		nextText: 'ë¤ìë¬',
		currentText: 'ì¤ë',
		monthNames: ['1ì','2ì','3ì','4ì','5ì','6ì',
		'7ì','8ì','9ì','10ì','11ì','12ì'],
		monthNamesShort: ['1ì','2ì','3ì','4ì','5ì','6ì',
		'7ì','8ì','9ì','10ì','11ì','12ì'],
		dayNames: ['ì¼ìì¼','ììì¼','íìì¼','ììì¼','ëª©ìì¼','ê¸ìì¼','í ìì¼'],
		dayNamesShort: ['ì¼','ì','í','ì','ëª©','ê¸','í '],
		dayNamesMin: ['ì¼','ì','í','ì','ëª©','ê¸','í '],
		weekHeader: 'Wk',
		dateFormat: 'yy-mm-dd',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: 'ë'};
	$.datepicker.setDefaults($.datepicker.regional['ko']);
});
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

/* Lithuanian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* @author Arturas Paleicikas <arturas@avalon.lt> */
jQuery(function($){
	$.datepicker.regional['lt'] = {
		closeText: 'UÅ¾daryti',
		prevText: '&#x3c;Atgal',
		nextText: 'Pirmyn&#x3e;',
		currentText: 'Å iandien',
		monthNames: ['Sausis','Vasaris','Kovas','Balandis','GeguÅ¾Ä','BirÅ¾elis',
		'Liepa','RugpjÅ«tis','RugsÄjis','Spalis','Lapkritis','Gruodis'],
		monthNamesShort: ['Sau','Vas','Kov','Bal','Geg','Bir',
		'Lie','Rugp','Rugs','Spa','Lap','Gru'],
		dayNames: ['sekmadienis','pirmadienis','antradienis','treÄiadienis','ketvirtadienis','penktadienis','Å¡eÅ¡tadienis'],
		dayNamesShort: ['sek','pir','ant','tre','ket','pen','Å¡eÅ¡'],
		dayNamesMin: ['Se','Pr','An','Tr','Ke','Pe','Å e'],
		weekHeader: 'Wk',
		dateFormat: 'yy-mm-dd',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['lt']);
});
/* Latvian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* @author Arturas Paleicikas <arturas.paleicikas@metasite.net> */
jQuery(function($){
	$.datepicker.regional['lv'] = {
		closeText: 'AizvÄrt',
		prevText: 'Iepr',
		nextText: 'NÄka',
		currentText: 'Å odien',
		monthNames: ['JanvÄris','FebruÄris','Marts','AprÄ«lis','Maijs','JÅ«nijs',
		'JÅ«lijs','Augusts','Septembris','Oktobris','Novembris','Decembris'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Mai','JÅ«n',
		'JÅ«l','Aug','Sep','Okt','Nov','Dec'],
		dayNames: ['svÄtdiena','pirmdiena','otrdiena','treÅ¡diena','ceturtdiena','piektdiena','sestdiena'],
		dayNamesShort: ['svt','prm','otr','tre','ctr','pkt','sst'],
		dayNamesMin: ['Sv','Pr','Ot','Tr','Ct','Pk','Ss'],
		weekHeader: 'Nav',
		dateFormat: 'dd-mm-yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['lv']);
});
/* Macedonian i18n for the jQuery UI date picker plugin. */
/* Written by Stojce Slavkovski. */
jQuery(function($){
	$.datepicker.regional['mk'] = {
		closeText: 'ÐÐ°ÑÐ²Ð¾ÑÐ¸',
		prevText: '&#x3C;',
		nextText: '&#x3E;',
		currentText: 'ÐÐµÐ½ÐµÑ',
		monthNames: ['ÐÐ°Ð½ÑÐ°ÑÐ¸','Ð¤ÐµÐ²ÑÑÐ°ÑÐ¸','ÐÐ°ÑÑ','ÐÐ¿ÑÐ¸Ð»','ÐÐ°Ñ','ÐÑÐ½Ð¸',
		'ÐÑÐ»Ð¸','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ¿ÑÐµÐ¼Ð²ÑÐ¸','ÐÐºÑÐ¾Ð¼Ð²ÑÐ¸','ÐÐ¾ÐµÐ¼Ð²ÑÐ¸','ÐÐµÐºÐµÐ¼Ð²ÑÐ¸'],
		monthNamesShort: ['ÐÐ°Ð½','Ð¤ÐµÐ²','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ñ','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ¿','ÐÐºÑ','ÐÐ¾Ðµ','ÐÐµÐº'],
		dayNames: ['ÐÐµÐ´ÐµÐ»Ð°','ÐÐ¾Ð½ÐµÐ´ÐµÐ»Ð½Ð¸Ðº','ÐÑÐ¾ÑÐ½Ð¸Ðº','Ð¡ÑÐµÐ´Ð°','Ð§ÐµÑÐ²ÑÑÐ¾Ðº','ÐÐµÑÐ¾Ðº','Ð¡Ð°Ð±Ð¾ÑÐ°'],
		dayNamesShort: ['ÐÐµÐ´','ÐÐ¾Ð½','ÐÑÐ¾','Ð¡ÑÐµ','Ð§ÐµÑ','ÐÐµÑ','Ð¡Ð°Ð±'],
		dayNamesMin: ['ÐÐµ','ÐÐ¾','ÐÑ','Ð¡Ñ','Ð§Ðµ','ÐÐµ','Ð¡Ð°'],
		weekHeader: 'Ð¡ÐµÐ´',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['mk']);
});

/* Malayalam (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Saji Nediyanchath (saji89@gmail.com). */
jQuery(function($){
	$.datepicker.regional['ml'] = {
		closeText: 'à´¶à´°à´¿',
		prevText: 'à´®àµà´¨àµà´¨à´¤àµà´¤àµ',  
		nextText: 'à´à´àµà´¤àµà´¤à´¤àµ ',
		currentText: 'à´à´¨àµà´¨àµ',
		monthNames: ['à´à´¨àµà´µà´°à´¿','à´«àµà´¬àµà´°àµà´µà´°à´¿','à´®à´¾à´°àµâà´àµà´àµ','à´à´ªàµà´°à´¿à´²àµâ','à´®àµà´¯àµ','à´àµà´£àµâ',
		'à´àµà´²àµ','à´à´à´¸àµà´±àµà´±àµ','à´¸àµà´ªàµà´±àµà´±à´à´¬à´°àµâ','à´à´àµà´àµà´¬à´°àµâ','à´¨à´µà´à´¬à´°àµâ','à´¡à´¿à´¸à´à´¬à´°àµâ'],
		monthNamesShort: ['à´à´¨àµ', 'à´«àµà´¬àµ', 'à´®à´¾à´°àµâ', 'à´à´ªàµà´°à´¿', 'à´®àµà´¯àµ', 'à´àµà´£àµâ',
		'à´àµà´²à´¾', 'à´à´', 'à´¸àµà´ªàµ', 'à´à´àµà´àµ', 'à´¨à´µà´', 'à´¡à´¿à´¸'],
		dayNames: ['à´à´¾à´¯à´°àµâ', 'à´¤à´¿à´àµà´à´³àµâ', 'à´àµà´µàµà´µ', 'à´¬àµà´§à´¨àµâ', 'à´µàµà´¯à´¾à´´à´', 'à´µàµà´³àµà´³à´¿', 'à´¶à´¨à´¿'],
		dayNamesShort: ['à´à´¾à´¯', 'à´¤à´¿à´àµà´', 'à´àµà´µàµà´µ', 'à´¬àµà´§', 'à´µàµà´¯à´¾à´´à´', 'à´µàµà´³àµà´³à´¿', 'à´¶à´¨à´¿'],
		dayNamesMin: ['à´à´¾','à´¤à´¿','à´àµ','à´¬àµ','à´µàµà´¯à´¾','à´µàµ','à´¶'],
		weekHeader: 'à´',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ml']);
});

/* Malaysian initialisation for the jQuery UI date picker plugin. */
/* Written by Mohd Nawawi Mohamad Jamili (nawawi@ronggeng.net). */
jQuery(function($){
	$.datepicker.regional['ms'] = {
		closeText: 'Tutup',
		prevText: '&#x3c;Sebelum',
		nextText: 'Selepas&#x3e;',
		currentText: 'hari ini',
		monthNames: ['Januari','Februari','Mac','April','Mei','Jun',
		'Julai','Ogos','September','Oktober','November','Disember'],
		monthNamesShort: ['Jan','Feb','Mac','Apr','Mei','Jun',
		'Jul','Ogo','Sep','Okt','Nov','Dis'],
		dayNames: ['Ahad','Isnin','Selasa','Rabu','Khamis','Jumaat','Sabtu'],
		dayNamesShort: ['Aha','Isn','Sel','Rab','kha','Jum','Sab'],
		dayNamesMin: ['Ah','Is','Se','Ra','Kh','Ju','Sa'],
		weekHeader: 'Mg',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ms']);
});
/* Dutch (Belgium) initialisation for the jQuery UI date picker plugin. */
/* David De Sloovere @DavidDeSloovere */
jQuery(function($){
	$.datepicker.regional['nl-BE'] = {
		closeText: 'Sluiten',
		prevText: 'â',
		nextText: 'â',
		currentText: 'Vandaag',
		monthNames: ['januari', 'februari', 'maart', 'april', 'mei', 'juni',
		'juli', 'augustus', 'september', 'oktober', 'november', 'december'],
		monthNamesShort: ['jan', 'feb', 'mrt', 'apr', 'mei', 'jun',
		'jul', 'aug', 'sep', 'okt', 'nov', 'dec'],
		dayNames: ['zondag', 'maandag', 'dinsdag', 'woensdag', 'donderdag', 'vrijdag', 'zaterdag'],
		dayNamesShort: ['zon', 'maa', 'din', 'woe', 'don', 'vri', 'zat'],
		dayNamesMin: ['zo', 'ma', 'di', 'wo', 'do', 'vr', 'za'],
		weekHeader: 'Wk',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['nl-BE']);
});

/* Dutch (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Mathias Bynens <http://mathiasbynens.be/> */
jQuery(function($){
	$.datepicker.regional.nl = {
		closeText: 'Sluiten',
		prevText: 'â',
		nextText: 'â',
		currentText: 'Vandaag',
		monthNames: ['januari', 'februari', 'maart', 'april', 'mei', 'juni',
		'juli', 'augustus', 'september', 'oktober', 'november', 'december'],
		monthNamesShort: ['jan', 'feb', 'mrt', 'apr', 'mei', 'jun',
		'jul', 'aug', 'sep', 'okt', 'nov', 'dec'],
		dayNames: ['zondag', 'maandag', 'dinsdag', 'woensdag', 'donderdag', 'vrijdag', 'zaterdag'],
		dayNamesShort: ['zon', 'maa', 'din', 'woe', 'don', 'vri', 'zat'],
		dayNamesMin: ['zo', 'ma', 'di', 'wo', 'do', 'vr', 'za'],
		weekHeader: 'Wk',
		dateFormat: 'dd-mm-yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional.nl);
});
/* Norwegian initialisation for the jQuery UI date picker plugin. */
/* Written by Naimdjon Takhirov (naimdjon@gmail.com). */

jQuery(function($){
  $.datepicker.regional['no'] = {
    closeText: 'Lukk',
    prevText: '&laquo;Forrige',
    nextText: 'Neste&raquo;',
    currentText: 'I dag',
    monthNames: ['januar','februar','mars','april','mai','juni','juli','august','september','oktober','november','desember'],
    monthNamesShort: ['jan','feb','mar','apr','mai','jun','jul','aug','sep','okt','nov','des'],
    dayNamesShort: ['sÃ¸n','man','tir','ons','tor','fre','lÃ¸r'],
    dayNames: ['sÃ¸ndag','mandag','tirsdag','onsdag','torsdag','fredag','lÃ¸rdag'],
    dayNamesMin: ['sÃ¸','ma','ti','on','to','fr','lÃ¸'],
    weekHeader: 'Uke',
    dateFormat: 'dd.mm.yy',
    firstDay: 1,
    isRTL: false,
    showMonthAfterYear: false,
    yearSuffix: ''
  };
  $.datepicker.setDefaults($.datepicker.regional['no']);
});

/* Polish initialisation for the jQuery UI date picker plugin. */
/* Written by Jacek Wysocki (jacek.wysocki@gmail.com). */
jQuery(function($){
	$.datepicker.regional['pl'] = {
		closeText: 'Zamknij',
		prevText: '&#x3c;Poprzedni',
		nextText: 'NastÄpny&#x3e;',
		currentText: 'DziÅ',
		monthNames: ['StyczeÅ','Luty','Marzec','KwiecieÅ','Maj','Czerwiec',
		'Lipiec','SierpieÅ','WrzesieÅ','PaÅºdziernik','Listopad','GrudzieÅ'],
		monthNamesShort: ['Sty','Lu','Mar','Kw','Maj','Cze',
		'Lip','Sie','Wrz','Pa','Lis','Gru'],
		dayNames: ['Niedziela','PoniedziaÅek','Wtorek','Åroda','Czwartek','PiÄtek','Sobota'],
		dayNamesShort: ['Nie','Pn','Wt','År','Czw','Pt','So'],
		dayNamesMin: ['N','Pn','Wt','År','Cz','Pt','So'],
		weekHeader: 'Tydz',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['pl']);
});

/* Brazilian initialisation for the jQuery UI date picker plugin. */
/* Written by Leonildo Costa Silva (leocsilva@gmail.com). */
jQuery(function($){
	$.datepicker.regional['pt-BR'] = {
		closeText: 'Fechar',
		prevText: '&#x3c;Anterior',
		nextText: 'Pr&oacute;ximo&#x3e;',
		currentText: 'Hoje',
		monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho',
		'Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
		monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun',
		'Jul','Ago','Set','Out','Nov','Dez'],
		dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
		dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','S&aacute;b'],
		dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','S&aacute;b'],
		weekHeader: 'Sm',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['pt-BR']);
});
/* Portuguese initialisation for the jQuery UI date picker plugin. */
jQuery(function($){
	$.datepicker.regional['pt'] = {
		closeText: 'Fechar',
		prevText: '&#x3c;Anterior',
		nextText: 'Seguinte',
		currentText: 'Hoje',
		monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho',
		'Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
		monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun',
		'Jul','Ago','Set','Out','Nov','Dez'],
		dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
		dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','S&aacute;b'],
		dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','S&aacute;b'],
		weekHeader: 'Sem',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['pt']);
});
/* Romansh initialisation for the jQuery UI date picker plugin. */
/* Written by Yvonne Gienal (yvonne.gienal@educa.ch). */
jQuery(function($){
	$.datepicker.regional['rm'] = {
		closeText: 'Serrar',
		prevText: '&#x3c;Suandant',
		nextText: 'Precedent&#x3e;',
		currentText: 'Actual',
		monthNames: ['Schaner','Favrer','Mars','Avrigl','Matg','Zercladur', 'Fanadur','Avust','Settember','October','November','December'],
		monthNamesShort: ['Scha','Fev','Mar','Avr','Matg','Zer', 'Fan','Avu','Sett','Oct','Nov','Dec'],
		dayNames: ['Dumengia','Glindesdi','Mardi','Mesemna','Gievgia','Venderdi','Sonda'],
		dayNamesShort: ['Dum','Gli','Mar','Mes','Gie','Ven','Som'],
		dayNamesMin: ['Du','Gl','Ma','Me','Gi','Ve','So'],
		weekHeader: 'emna',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['rm']);
});

/* Romanian initialisation for the jQuery UI date picker plugin.
 *
 * Written by Edmond L. (ll_edmond@walla.com)
 * and Ionut G. Stan (ionut.g.stan@gmail.com)
 */
jQuery(function($){
	$.datepicker.regional['ro'] = {
		closeText: 'Ãnchide',
		prevText: '&laquo; Luna precedentÄ',
		nextText: 'Luna urmÄtoare &raquo;',
		currentText: 'Azi',
		monthNames: ['Ianuarie','Februarie','Martie','Aprilie','Mai','Iunie',
		'Iulie','August','Septembrie','Octombrie','Noiembrie','Decembrie'],
		monthNamesShort: ['Ian', 'Feb', 'Mar', 'Apr', 'Mai', 'Iun',
		'Iul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
		dayNames: ['DuminicÄ', 'Luni', 'MarÅ£i', 'Miercuri', 'Joi', 'Vineri', 'SÃ¢mbÄtÄ'],
		dayNamesShort: ['Dum', 'Lun', 'Mar', 'Mie', 'Joi', 'Vin', 'SÃ¢m'],
		dayNamesMin: ['Du','Lu','Ma','Mi','Jo','Vi','SÃ¢'],
		weekHeader: 'SÄpt',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ro']);
});

/* Russian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Andrew Stromnov (stromnov@gmail.com). */
jQuery(function($){
	$.datepicker.regional['ru'] = {
		closeText: 'ÐÐ°ÐºÑÑÑÑ',
		prevText: '&#x3c;ÐÑÐµÐ´',
		nextText: 'Ð¡Ð»ÐµÐ´&#x3e;',
		currentText: 'Ð¡ÐµÐ³Ð¾Ð´Ð½Ñ',
		monthNames: ['Ð¯Ð½Ð²Ð°ÑÑ','Ð¤ÐµÐ²ÑÐ°Ð»Ñ','ÐÐ°ÑÑ','ÐÐ¿ÑÐµÐ»Ñ','ÐÐ°Ð¹','ÐÑÐ½Ñ',
		'ÐÑÐ»Ñ','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ½ÑÑÐ±ÑÑ','ÐÐºÑÑÐ±ÑÑ','ÐÐ¾ÑÐ±ÑÑ','ÐÐµÐºÐ°Ð±ÑÑ'],
		monthNamesShort: ['Ð¯Ð½Ð²','Ð¤ÐµÐ²','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ð¹','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ½','ÐÐºÑ','ÐÐ¾Ñ','ÐÐµÐº'],
		dayNames: ['Ð²Ð¾ÑÐºÑÐµÑÐµÐ½ÑÐµ','Ð¿Ð¾Ð½ÐµÐ´ÐµÐ»ÑÐ½Ð¸Ðº','Ð²ÑÐ¾ÑÐ½Ð¸Ðº','ÑÑÐµÐ´Ð°','ÑÐµÑÐ²ÐµÑÐ³','Ð¿ÑÑÐ½Ð¸ÑÐ°','ÑÑÐ±Ð±Ð¾ÑÐ°'],
		dayNamesShort: ['Ð²ÑÐº','Ð¿Ð½Ð´','Ð²ÑÑ','ÑÑÐ´','ÑÑÐ²','Ð¿ÑÐ½','ÑÐ±Ñ'],
		dayNamesMin: ['ÐÑ','ÐÐ½','ÐÑ','Ð¡Ñ','Ð§Ñ','ÐÑ','Ð¡Ð±'],
		weekHeader: 'ÐÐµÐ´',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ru']);
});
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

/* Slovenian initialisation for the jQuery UI date picker plugin. */
/* Written by Jaka Jancar (jaka@kubje.org). */
/* c = &#x10D;, s = &#x161; z = &#x17E; C = &#x10C; S = &#x160; Z = &#x17D; */
jQuery(function($){
	$.datepicker.regional['sl'] = {
		closeText: 'Zapri',
		prevText: '&lt;Prej&#x161;nji',
		nextText: 'Naslednji&gt;',
		currentText: 'Trenutni',
		monthNames: ['Januar','Februar','Marec','April','Maj','Junij',
		'Julij','Avgust','September','Oktober','November','December'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Maj','Jun',
		'Jul','Avg','Sep','Okt','Nov','Dec'],
		dayNames: ['Nedelja','Ponedeljek','Torek','Sreda','&#x10C;etrtek','Petek','Sobota'],
		dayNamesShort: ['Ned','Pon','Tor','Sre','&#x10C;et','Pet','Sob'],
		dayNamesMin: ['Ne','Po','To','Sr','&#x10C;e','Pe','So'],
		weekHeader: 'Teden',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['sl']);
});

/* Albanian initialisation for the jQuery UI date picker plugin. */
/* Written by Flakron Bytyqi (flakron@gmail.com). */
jQuery(function($){
	$.datepicker.regional['sq'] = {
		closeText: 'mbylle',
		prevText: '&#x3c;mbrapa',
		nextText: 'PÃ«rpara&#x3e;',
		currentText: 'sot',
		monthNames: ['Janar','Shkurt','Mars','Prill','Maj','Qershor',
		'Korrik','Gusht','Shtator','Tetor','NÃ«ntor','Dhjetor'],
		monthNamesShort: ['Jan','Shk','Mar','Pri','Maj','Qer',
		'Kor','Gus','Sht','Tet','NÃ«n','Dhj'],
		dayNames: ['E Diel','E HÃ«nÃ«','E MartÃ«','E MÃ«rkurÃ«','E Enjte','E Premte','E Shtune'],
		dayNamesShort: ['Di','HÃ«','Ma','MÃ«','En','Pr','Sh'],
		dayNamesMin: ['Di','HÃ«','Ma','MÃ«','En','Pr','Sh'],
		weekHeader: 'Ja',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['sq']);
});

/* Serbian i18n for the jQuery UI date picker plugin. */
/* Written by Dejan DimiÄ. */
jQuery(function($){
	$.datepicker.regional['sr-SR'] = {
		closeText: 'Zatvori',
		prevText: '&#x3c;',
		nextText: '&#x3e;',
		currentText: 'Danas',
		monthNames: ['Januar','Februar','Mart','April','Maj','Jun',
		'Jul','Avgust','Septembar','Oktobar','Novembar','Decembar'],
		monthNamesShort: ['Jan','Feb','Mar','Apr','Maj','Jun',
		'Jul','Avg','Sep','Okt','Nov','Dec'],
		dayNames: ['Nedelja','Ponedeljak','Utorak','Sreda','Äetvrtak','Petak','Subota'],
		dayNamesShort: ['Ned','Pon','Uto','Sre','Äet','Pet','Sub'],
		dayNamesMin: ['Ne','Po','Ut','Sr','Äe','Pe','Su'],
		weekHeader: 'Sed',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['sr-SR']);
});

/* Serbian i18n for the jQuery UI date picker plugin. */
/* Written by Dejan DimiÄ. */
jQuery(function($){
	$.datepicker.regional['sr'] = {
		closeText: 'ÐÐ°ÑÐ²Ð¾ÑÐ¸',
		prevText: '&#x3c;',
		nextText: '&#x3e;',
		currentText: 'ÐÐ°Ð½Ð°Ñ',
		monthNames: ['ÐÐ°Ð½ÑÐ°Ñ','Ð¤ÐµÐ±ÑÑÐ°Ñ','ÐÐ°ÑÑ','ÐÐ¿ÑÐ¸Ð»','ÐÐ°Ñ','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ¿ÑÐµÐ¼Ð±Ð°Ñ','ÐÐºÑÐ¾Ð±Ð°Ñ','ÐÐ¾Ð²ÐµÐ¼Ð±Ð°Ñ','ÐÐµÑÐµÐ¼Ð±Ð°Ñ'],
		monthNamesShort: ['ÐÐ°Ð½','Ð¤ÐµÐ±','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ñ','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ¿','ÐÐºÑ','ÐÐ¾Ð²','ÐÐµÑ'],
		dayNames: ['ÐÐµÐ´ÐµÑÐ°','ÐÐ¾Ð½ÐµÐ´ÐµÑÐ°Ðº','Ð£ÑÐ¾ÑÐ°Ðº','Ð¡ÑÐµÐ´Ð°','Ð§ÐµÑÐ²ÑÑÐ°Ðº','ÐÐµÑÐ°Ðº','Ð¡ÑÐ±Ð¾ÑÐ°'],
		dayNamesShort: ['ÐÐµÐ´','ÐÐ¾Ð½','Ð£ÑÐ¾','Ð¡ÑÐµ','Ð§ÐµÑ','ÐÐµÑ','Ð¡ÑÐ±'],
		dayNamesMin: ['ÐÐµ','ÐÐ¾','Ð£Ñ','Ð¡Ñ','Ð§Ðµ','ÐÐµ','Ð¡Ñ'],
		weekHeader: 'Ð¡ÐµÐ´',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['sr']);
});

/* Swedish initialisation for the jQuery UI date picker plugin. */
/* Written by Anders Ekdahl ( anders@nomadiz.se). */
jQuery(function($){
    $.datepicker.regional['sv'] = {
		closeText: 'StÃ¤ng',
        prevText: '&laquo;FÃ¶rra',
		nextText: 'NÃ¤sta&raquo;',
		currentText: 'Idag',
        monthNames: ['Januari','Februari','Mars','April','Maj','Juni',
        'Juli','Augusti','September','Oktober','November','December'],
        monthNamesShort: ['Jan','Feb','Mar','Apr','Maj','Jun',
        'Jul','Aug','Sep','Okt','Nov','Dec'],
		dayNamesShort: ['SÃ¶n','MÃ¥n','Tis','Ons','Tor','Fre','LÃ¶r'],
		dayNames: ['SÃ¶ndag','MÃ¥ndag','Tisdag','Onsdag','Torsdag','Fredag','LÃ¶rdag'],
		dayNamesMin: ['SÃ¶','MÃ¥','Ti','On','To','Fr','LÃ¶'],
		weekHeader: 'Ve',
        dateFormat: 'yy-mm-dd',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
    $.datepicker.setDefaults($.datepicker.regional['sv']);
});

/* Tamil (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by S A Sureshkumar (saskumar@live.com). */
jQuery(function($){
	$.datepicker.regional['ta'] = {
		closeText: 'à®®à¯à®à¯',
		prevText: 'à®®à¯à®©à¯à®©à¯à®¯à®¤à¯',
		nextText: 'à®à®à¯à®¤à¯à®¤à®¤à¯',
		currentText: 'à®à®©à¯à®±à¯',
		monthNames: ['à®¤à¯','à®®à®¾à®à®¿','à®ªà®à¯à®à¯à®©à®¿','à®à®¿à®¤à¯à®¤à®¿à®°à¯','à®µà¯à®à®¾à®à®¿','à®à®©à®¿',
		'à®à®à®¿','à®à®µà®£à®¿','à®ªà¯à®°à®à¯à®à®¾à®à®¿','à®à®ªà¯à®ªà®à®¿','à®à®¾à®°à¯à®¤à¯à®¤à®¿à®à¯','à®®à®¾à®°à¯à®à®´à®¿'],
		monthNamesShort: ['à®¤à¯','à®®à®¾à®à®¿','à®ªà®à¯','à®à®¿à®¤à¯','à®µà¯à®à®¾','à®à®©à®¿',
		'à®à®à®¿','à®à®µ','à®ªà¯à®°','à®à®ªà¯','à®à®¾à®°à¯','à®®à®¾à®°à¯'],
		dayNames: ['à®à®¾à®¯à®¿à®±à¯à®±à¯à®à¯à®à®¿à®´à®®à¯','à®¤à®¿à®à¯à®à®à¯à®à®¿à®´à®®à¯','à®à¯à®µà¯à®µà®¾à®¯à¯à®à¯à®à®¿à®´à®®à¯','à®ªà¯à®¤à®©à¯à®à®¿à®´à®®à¯','à®µà®¿à®¯à®¾à®´à®à¯à®à®¿à®´à®®à¯','à®µà¯à®³à¯à®³à®¿à®à¯à®à®¿à®´à®®à¯','à®à®©à®¿à®à¯à®à®¿à®´à®®à¯'],
		dayNamesShort: ['à®à®¾à®¯à®¿à®±à¯','à®¤à®¿à®à¯à®à®³à¯','à®à¯à®µà¯à®µà®¾à®¯à¯','à®ªà¯à®¤à®©à¯','à®µà®¿à®¯à®¾à®´à®©à¯','à®µà¯à®³à¯à®³à®¿','à®à®©à®¿'],
		dayNamesMin: ['à®à®¾','à®¤à®¿','à®à¯','à®ªà¯','à®µà®¿','à®µà¯','à®'],
		weekHeader: 'ÐÐµ',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['ta']);
});

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
/* Tajiki (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Abdurahmon Saidov (saidovab@gmail.com). */
jQuery(function($){
	$.datepicker.regional['tj'] = {
		closeText: 'ÐÐ´Ð¾Ð¼Ð°',
		prevText: '&#x3c;ÒÐ°ÑÐ¾',
		nextText: 'ÐÐµÑ&#x3e;',
		currentText: 'ÐÐ¼ÑÓ¯Ð·',
		monthNames: ['Ð¯Ð½Ð²Ð°Ñ','Ð¤ÐµÐ²ÑÐ°Ð»','ÐÐ°ÑÑ','ÐÐ¿ÑÐµÐ»','ÐÐ°Ð¹','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³ÑÑÑ','Ð¡ÐµÐ½ÑÑÐ±Ñ','ÐÐºÑÑÐ±Ñ','ÐÐ¾ÑÐ±Ñ','ÐÐµÐºÐ°Ð±Ñ'],
		monthNamesShort: ['Ð¯Ð½Ð²','Ð¤ÐµÐ²','ÐÐ°Ñ','ÐÐ¿Ñ','ÐÐ°Ð¹','ÐÑÐ½',
		'ÐÑÐ»','ÐÐ²Ð³','Ð¡ÐµÐ½','ÐÐºÑ','ÐÐ¾Ñ','ÐÐµÐº'],
		dayNames: ['ÑÐºÑÐ°Ð½Ð±Ðµ','Ð´ÑÑÐ°Ð½Ð±Ðµ','ÑÐµÑÐ°Ð½Ð±Ðµ','ÑÐ¾ÑÑÐ°Ð½Ð±Ðµ','Ð¿Ð°Ð½Ò·ÑÐ°Ð½Ð±Ðµ','Ò·ÑÐ¼ÑÐ°','ÑÐ°Ð½Ð±Ðµ'],
		dayNamesShort: ['ÑÐºÑ','Ð´ÑÑ','ÑÐµÑ','ÑÐ¾Ñ','Ð¿Ð°Ð½','Ò·ÑÐ¼','ÑÐ°Ð½'],
		dayNamesMin: ['Ð¯Ðº','ÐÑ','Ð¡Ñ','Ð§Ñ','ÐÑ','Ò¶Ð¼','Ð¨Ð½'],
		weekHeader: 'Ð¥Ñ',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['tj']);
});
/* Turkish initialisation for the jQuery UI date picker plugin. */
/* Written by Izzet Emre Erkan (kara@karalamalar.net). */
jQuery(function($){
	$.datepicker.regional['tr'] = {
		closeText: 'kapat',
		prevText: '&#x3c;geri',
		nextText: 'ileri&#x3e',
		currentText: 'bugÃ¼n',
		monthNames: ['Ocak','Åubat','Mart','Nisan','MayÄ±s','Haziran',
		'Temmuz','AÄustos','EylÃ¼l','Ekim','KasÄ±m','AralÄ±k'],
		monthNamesShort: ['Oca','Åub','Mar','Nis','May','Haz',
		'Tem','AÄu','Eyl','Eki','Kas','Ara'],
		dayNames: ['Pazar','Pazartesi','SalÄ±','ÃarÅamba','PerÅembe','Cuma','Cumartesi'],
		dayNamesShort: ['Pz','Pt','Sa','Ãa','Pe','Cu','Ct'],
		dayNamesMin: ['Pz','Pt','Sa','Ãa','Pe','Cu','Ct'],
		weekHeader: 'Hf',
		dateFormat: 'dd.mm.yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['tr']);
});
/* Ukrainian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Maxim Drogobitskiy (maxdao@gmail.com). */
/* Corrected by Igor Milla (igor.fsp.milla@gmail.com). */
jQuery(function($){
	$.datepicker.regional['uk'] = {
		closeText: 'ÐÐ°ÐºÑÐ¸ÑÐ¸',
		prevText: '&#x3c;',
		nextText: '&#x3e;',
		currentText: 'Ð¡ÑÐ¾Ð³Ð¾Ð´Ð½Ñ',
		monthNames: ['Ð¡ÑÑÐµÐ½Ñ','ÐÑÑÐ¸Ð¹','ÐÐµÑÐµÐ·ÐµÐ½Ñ','ÐÐ²ÑÑÐµÐ½Ñ','Ð¢ÑÐ°Ð²ÐµÐ½Ñ','Ð§ÐµÑÐ²ÐµÐ½Ñ',
		'ÐÐ¸Ð¿ÐµÐ½Ñ','Ð¡ÐµÑÐ¿ÐµÐ½Ñ','ÐÐµÑÐµÑÐµÐ½Ñ','ÐÐ¾Ð²ÑÐµÐ½Ñ','ÐÐ¸ÑÑÐ¾Ð¿Ð°Ð´','ÐÑÑÐ´ÐµÐ½Ñ'],
		monthNamesShort: ['Ð¡ÑÑ','ÐÑÑ','ÐÐµÑ','ÐÐ²Ñ','Ð¢ÑÐ°','Ð§ÐµÑ',
		'ÐÐ¸Ð¿','Ð¡ÐµÑ','ÐÐµÑ','ÐÐ¾Ð²','ÐÐ¸Ñ','ÐÑÑ'],
		dayNames: ['Ð½ÐµÐ´ÑÐ»Ñ','Ð¿Ð¾Ð½ÐµÐ´ÑÐ»Ð¾Ðº','Ð²ÑÐ²ÑÐ¾ÑÐ¾Ðº','ÑÐµÑÐµÐ´Ð°','ÑÐµÑÐ²ÐµÑ','Ð¿âÑÑÐ½Ð¸ÑÑ','ÑÑÐ±Ð¾ÑÐ°'],
		dayNamesShort: ['Ð½ÐµÐ´','Ð¿Ð½Ð´','Ð²ÑÐ²','ÑÑÐ´','ÑÑÐ²','Ð¿ÑÐ½','ÑÐ±Ñ'],
		dayNamesMin: ['ÐÐ´','ÐÐ½','ÐÑ','Ð¡Ñ','Ð§Ñ','ÐÑ','Ð¡Ð±'],
		weekHeader: 'Ð¢Ð¸Ð¶',
		dateFormat: 'dd/mm/yy',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['uk']);
});
/* Vietnamese initialisation for the jQuery UI date picker plugin. */
/* Translated by Le Thanh Huy (lthanhhuy@cit.ctu.edu.vn). */
jQuery(function($){
	$.datepicker.regional['vi'] = {
		closeText: 'ÄÃ³ng',
		prevText: '&#x3c;TrÆ°á»c',
		nextText: 'Tiáº¿p&#x3e;',
		currentText: 'HÃ´m nay',
		monthNames: ['ThÃ¡ng Má»t', 'ThÃ¡ng Hai', 'ThÃ¡ng Ba', 'ThÃ¡ng TÆ°', 'ThÃ¡ng NÄm', 'ThÃ¡ng SÃ¡u',
		'ThÃ¡ng Báº£y', 'ThÃ¡ng TÃ¡m', 'ThÃ¡ng ChÃ­n', 'ThÃ¡ng MÆ°á»i', 'ThÃ¡ng MÆ°á»i Má»t', 'ThÃ¡ng MÆ°á»i Hai'],
		monthNamesShort: ['ThÃ¡ng 1', 'ThÃ¡ng 2', 'ThÃ¡ng 3', 'ThÃ¡ng 4', 'ThÃ¡ng 5', 'ThÃ¡ng 6',
		'ThÃ¡ng 7', 'ThÃ¡ng 8', 'ThÃ¡ng 9', 'ThÃ¡ng 10', 'ThÃ¡ng 11', 'ThÃ¡ng 12'],
		dayNames: ['Chá»§ Nháº­t', 'Thá»© Hai', 'Thá»© Ba', 'Thá»© TÆ°', 'Thá»© NÄm', 'Thá»© SÃ¡u', 'Thá»© Báº£y'],
		dayNamesShort: ['CN', 'T2', 'T3', 'T4', 'T5', 'T6', 'T7'],
		dayNamesMin: ['CN', 'T2', 'T3', 'T4', 'T5', 'T6', 'T7'],
		weekHeader: 'Tu',
		dateFormat: 'dd/mm/yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: ''};
	$.datepicker.setDefaults($.datepicker.regional['vi']);
});

/* Chinese initialisation for the jQuery UI date picker plugin. */
/* Written by Cloudream (cloudream@gmail.com). */
jQuery(function($){
	$.datepicker.regional['zh-CN'] = {
		closeText: 'å³é­',
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
		dateFormat: 'yy-mm-dd',
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: 'å¹´'};
	$.datepicker.setDefaults($.datepicker.regional['zh-CN']);
});

/* Chinese initialisation for the jQuery UI date picker plugin. */
/* Written by SCCY (samuelcychan@gmail.com). */
jQuery(function($){
	$.datepicker.regional['zh-HK'] = {
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
		dateFormat: 'dd-mm-yy',
		firstDay: 0,
		isRTL: false,
		showMonthAfterYear: true,
		yearSuffix: 'å¹´'};
	$.datepicker.setDefaults($.datepicker.regional['zh-HK']);
});

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
