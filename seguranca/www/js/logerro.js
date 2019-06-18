function popup_center(a,b,w,h,scroll){
    // a=endereco ; b=titulo
    var winl = (screen.width-w)/2;
    var wint = (screen.height-h)/2;
    var settings  ='height='+h+',';
    settings +='width='+w+',';
    settings +='top='+wint+',';
    settings +='left='+winl+',';
    settings +='scrollbars='+scroll+',';
    settings +='resizable=no,';
    settings +='titlebar=no,';
    settings +='toolbar=no,';
    settings +='location=no,';
    settings +='directories=no,';
    settings +='status=yes,';
    settings +='menubar=no';
    NewWin2 = window.open(a,b,settings);
    if(parseInt(navigator.appVersion) >= 4){NewWin2.window.focus();}
}

function detalhaErro( errid ){
    popup_center('/seguranca/seguranca.php?modulo=principal/logFalhaInesperada&acao=A&errid='+errid,'janelaDetalheErro','1024','768','1');
}