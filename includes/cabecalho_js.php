<script language="JavaScript">
	function setpfl() {
		document.getElementById('setperfil2').submit();
	}

	function setpfl1() {
		document.getElementById('setperfil').submit();
	}

	function abrirsistema( sisid ) {
		location.href = "../muda_sistema.php?sisid=" + sisid;
	}

	function abrir_popup_mensagem()
	{
		w = window.open( '../geral/popup_mensagem.php', 'mensagens', 'width=780,height=400,scrollbars=yes,menubar=no,toolbar=no,statusbar=no' );
		w.focus();
	}

    function changeSystem(system)
    {
        location.href = "../muda_sistema.php?sisid=" + system;
    }
    function exibeThemas()
    {
    	var div = document.getElementById('menu_theme');
		if (div.style.display == 'none')
    		div.style.display = '';
    	else
    		div.style.display = 'none';
    }

    function alteraTema(){
    	document.getElementById('formTheme').submit();
    }



    	var goleftimage='../imagens/back.gif'
	var gorightimage='../imagens/foward.gif'
	//configure menu width (in px):
	var myWidth;
	
	if( typeof( window.innerWidth ) == 'number' ) {
		//Non-IE
		myWidth = window.innerWidth - 70 ;
	} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
		//IE 6+ in 'standards compliant mode'
		myWidth = document.documentElement.clientWidth - 50;
	} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
		//IE 4 compatible
		myWidth = document.body.clientWidth - 50 ;
	}
	var menuwidth = myWidth;
	var menuheight=18
	var scrolldir="normal"
	var scrollspeed=10
	var menucontents='<table id="tb_menu" border="0" cellspacing="0" cellpadding="0"><tr>';
	
	
	<? foreach ( $sistemas as $key => $sistema ): ?>
		<? extract( $sistema ); ?>
		<? if ( $sisid == $_SESSION['sisid'] && $key == 0){ ?>
			menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_left.png"></td>';
		<? }elseif ( $sisid != $_SESSION['sisid'] && $key == 0){ ?>
			menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_left_off.png"></td>';
		<? }elseif ( $sisid == $_SESSION['sisid'] && $key != 0 ){ ?>
			menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_left_off.png"></td>';
		<? }elseif ( $sisid != $_SESSION['sisid'] && $key != 0 && $sistemas[$key - 1]['sisid'] == $_SESSION['sisid']){ ?>
			menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_right_off.png"></td>';
		<? }elseif ( $sisid != $_SESSION['sisid'] && $key != 0 ){ ?>
			menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_off.png"></td>';
	<? } ?>

	<? if ( $sisid == $_SESSION['sisid']){ ?>
		menucontents = menucontents + '<td nowrap class="td_on" onclick="abrirsistema(<?= $sisid ?>)" title="<?= $sisdsc ?>"><a href="javascript:abrirsistema(<?= $sisid ?>)" ><?= $sisabrev ?></a></td>';
	<? } else{ ?>
		menucontents = menucontents + '<td nowrap class="td_off" onclick="abrirsistema(<?= $sisid ?>)" title="<?= $sisdsc ?>"><a href="javascript:abrirsistema(<?= $sisid ?>)" ><?= $sisabrev ?></a></td>';
	<? } ?>

	<? if ( $sisid == $_SESSION['sisid'] && $key == (count($sistemas) - 1) ){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_right.png"></td>';
	<? } elseif ( $sisid != $_SESSION['sisid'] && $key == (count($sistemas) - 1) ){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_right_off.png"></td>';
	<? } ?>
	<? endforeach; ?>



menucontents = menucontents + '</tr></table>';

if( screen.width >= 800 && screen.width < 1024 ){
	var qtdAbas = 50;
	q}else if( screen.width >= 1024 && screen.width < 1300 ) {
	
	var qtdAbas = 80;
	}else if( screen.width >= 1300 && screen.width < 1600 ) {
	
	var qtdAbas = 100;
	}else if( screen.width >= 1600 && screen.width < 1900 ) {
	
	var qtdAbas = 120;
	}else if( screen.width >= 1900 && screen.width < 3000 ) {
	
	var qtdAbas = 150;
	}else if( screen.width >= 3000 ) {
	var qtdAbas = 200;
	} else {
	var qtdAbas = 20;
}


	var iedom=document.all||document.getElementById
	var leftdircode='onmousedown="moveleft('+qtdAbas+')"'
	var rightdircode='onmousedown="moveright('+qtdAbas+')"'
	if (scrolldir=="reverse"){
	var tempswap=leftdircode
	leftdircode=rightdircode
	rightdircode=tempswap
	}
	
	if (iedom)
	document.write('<span id="temp" style="visibility:hidden;position:absolute;top:-100;left:-5000">'+menucontents+'</span>')
	
	var actualwidth='';
	var cross_scroll, ns_scroll
	var loadedyes=0
	
	function fillup(){
		if (iedom){
			cross_scroll=document.getElementById? document.getElementById("test2") : document.all.test2
			cross_scroll.innerHTML=menucontents;
			actualwidth=document.all? parseInt(cross_scroll.offsetWidth) : parseInt(document.getElementById("temp").offsetWidth);
		}else if (document.layers){
			ns_scroll=document.ns_scrollmenu.document.ns_scrollmenu2
			ns_scroll.document.write(menucontents)
			ns_scroll.document.close()
			actualwidth=ns_scroll.document.width
		}
		loadedyes=1
	}

	function moveleft(i){
		if (loadedyes){
			if (iedom&&parseInt(cross_scroll.style.left)>(menuwidth-actualwidth)){
				cross_scroll.style.left=parseInt(cross_scroll.style.left)-scrollspeed+"px"
				}else if (document.layers&&ns_scroll.left>(menuwidth-actualwidth)) {
					ns_scroll.left-=scrollspeed
				}
		}
		if(i > 0) {
			lefttime=setTimeout("moveleft("+(i-1)+")",10)
		}
		
	}
	function moveright(i){
		if (loadedyes){
			if (iedom&&parseInt(cross_scroll.style.left)<0)
				cross_scroll.style.left=parseInt(cross_scroll.style.left)+scrollspeed+"px"
				else if (document.layers&&ns_scroll.left<0)
				ns_scroll.left+=scrollspeed
		}
		if(i > 0) {
			righttime=setTimeout("moveright("+(i-1)+")",10);
		}
	}
	if (iedom||document.layers){
	with (document){
		document.write('<table border="0" class="bg_linha_cinza" cellspacing="0 cellpadding="0" width="'+menuwidth+'" height="'+menuheight+'"><tr>')
		document.write('<td valign="top" id="dirleft" width="10"><table border="0" cellspacing="0" cellpadding="0"><tr><td style="cursor: pointer;background-color:#e6ebed;color:#285e78; border: 1px solid #555555;" '+rightdircode+'>&nbsp;«&nbsp;</td></tr></table></td>')
		document.write('<td width="'+menuwidth+'" height="'+menuheight+'" id="flutua1" class="bg_linha_cinza" valign="top">')
		if (iedom){
			document.write('<div id="flutua2" style="position:relative;width:'+menuwidth+';height:18;overflow:hidden;">')
			document.write('<div id="test2" style="position:absolute;left:0;top:0">')
			document.write('</div></div>')
			}else if (document.layers){
				document.write('<ilayer width='+menuwidth+' height='+menuheight+' name="ns_scrollmenu">')
				document.write('<layer name="ns_scrollmenu2" left=0 top=0></layer></ilayer>')
			}
			document.write('</td>')
			document.write('<td valign="top" class="bg_linha_cinza"  id="dirright"><table class="bg_linha_cinza"  border="0" cellspacing="0" cellpadding="0"><tr><td style="cursor: pointer;background-color:#e6ebed;color:#285e78; border: 1px solid #555555;" '+leftdircode+'>&nbsp;')
			document.write('»&nbsp;</td></tr></table>')
			document.write('</td></tr></table>')
	}
	}
fillup();


// Procedimento para ajustar a aba selecionada selecionado
	var tabela1 = document.getElementById("tb_menu");
	var countchar = 0;

	for(i = 1; i < tabela1.rows[0].cells.length; i=i+2) {
		var text = tabela1.rows[0].cells[i].innerHTML;
		countchar = countchar + text.length;

		if(tabela1.rows[0].cells[i].className == 'td_on') {
			var sel = countchar + (countchar/1.1); // padronização : 15 caracteres = 90px
		}
	}
	if(sel > menuwidth) {
		cross_scroll.style.left = (menuwidth + 10) - sel;
	}
	
	// Caso seja IE, utilizar o tamanho estimado
	if(actualwidth == 0) {
		actualwidth = (countchar*6 - 30);
	}
	if((countchar*6) < menuwidth) {
		document.getElementById("dirleft").style.display = 'none';
		document.getElementById("dirright").style.display = 'none';
	}


	window.onresize = redimensionarMenu;

	function redimensionarMenu() {
		var myWidth;
		if( typeof( window.innerWidth ) == 'number' ) {
			//Non-IE
			myWidth = window.innerWidth;
		} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
			//IE 6+ in 'standards compliant mode'
			myWidth = document.documentElement.clientWidth;
		} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
			//IE 4 compatible
			myWidth = document.body.clientWidth;
		}
		if(parseInt(cross_scroll.style.left) < 0) {
			cross_scroll.style.left = (myWidth - 80 - menuwidth) + parseInt(cross_scroll.style.left);
		}
		
		menuwidth = myWidth - 80;

		if((countchar*6) < menuwidth) {
			document.getElementById("dirleft").style.display = 'none';
			document.getElementById("dirright").style.display = 'none';
		} else {
			document.getElementById("dirleft").style.display = '';
			document.getElementById("dirright").style.display = '';
		}

		document.getElementById('flutua1').style.width = myWidth - 80;
		document.getElementById('flutua2').style.width = myWidth - 80;
	}
	var minutos=<? echo floor((MAXONLINETIME/60)); ?>;
	var seconds=<? echo floor(MAXONLINETIME%60); ?>;
	var campo = document.getElementById("cronometro");
	var campo_div = document.getElementById("cronometro_div");

	function startCountdown(){
		if (seconds<=0){
		    seconds=60;
			minutos-=1;
		}
			if (minutos<=-1){
			   seconds=0;
			   seconds+=1;
			   campo.innerHTML="";
			   campo_div.innerHTML="Sessão expirada!";
			location.href = "../../logout.php";
			} else{
			seconds-=1
			if(seconds < 10) {
				seconds = "0" + seconds;
			}
			   campo.innerHTML = " " + minutos+"min"+seconds+"s";
			   setTimeout("startCountdown()",1000);
			
			}
			startCountdown();
			function simularUsuario(){
				var horizontal = 550;
				var vertical   = 320;
			
				var res_ver = screen.height;
				var res_hor = screen.width;
			
				var pos_ver_fin = (parseInt(res_ver) - parseInt(vertical) )/2;
				var pos_hor_fin = (parseInt(res_hor) - parseInt(horizontal) )/2;
			
				return window.open("../includes/simularUsuario.php","Simular Usuário","width="+horizontal+",height="+vertical+",top="+pos_ver_fin+",left="+pos_hor_fin);
			}
        
        // Pega o conteudo da divAmbiente com os dados que serão exibidos na tela
        divAmbiente = document.getElementById('divAmbiente').innerHTML;
        
        // Verifica se a div do menu fixo existe
        if( document.getElementById('menubackmenu1') ){
            // Insere os dados de ambiente com formatação na barra de menu para ficar sempre visivel
            document.getElementById('menubackmenu1').innerHTML = divAmbiente;
        }			
</script>