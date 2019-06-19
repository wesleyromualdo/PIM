/**
 * Varre formulário em busca de campos que precisam de tratamento
 * especial. O campo do tipo 'combo_popup' tem seus itens todos
 * selecionados.
 *
 * @return void
 */
function prepara_formulario() {
    var quantidade = document.forms.length;
    var quantidade_elementos = 0;
    var elemento = null;
    var j = 0;
    for (var i = 0; i < quantidade; i++) {
        quantidade_elementos = document.forms[i].elements.length;

        for (j = 0; j < quantidade_elementos; j++) {
            elemento = document.forms[i].elements[j];
            if (elemento.getAttribute('tipo') == 'combo_popup') {
                selectAllOptions(elemento);
            }
        }
    }
}


/*
 * Classe   ValidacaoAutomatica
 * A classe possibilita a valida��o autom�tica dos campos do formul�rio
 *
 * @author  Felipe Tarchiani Ceravolo Chiavicatti
 * @since   16/01/2015
 * @exemple
 * @link
 *
 * Lista de todos os validadores at� o momento:
 * 		addValidaObrigatorio = function ( funcaoFlag ); - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaTamanho = function ( tamanhoString, funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaTamanhoMinimo = function ( tamanhoString, funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaTamanhoMaximo = function ( tamanhoString, funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaMascaraNumerica = function ( mascara, funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaData = function ( funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaMesAno = function ( funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaExpressaoNumerica = function (operadorRelacional, numero, funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaCnpj = function ( funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 * 		addValidaCpf = function ( funcaoFlag ) - Para aplica��o correta, leia a documenta��o do m�todo;
 *
 * 		addValidaExterno = function ( funcaoValida ) - Para aplica��o correta, leia a documenta��o do m�todo;
 *
 */
function ValidacaoAutomatica(){
	// Esta vari�vel armazena os campos e suas configura��es de valida��o
	var camposForm 		 = new Object();
	// Este �ndice armazena os atributos de fun��o
	camposForm.param    = new Array();
	// Este �ndice armazena o atributo do item a ser validado
	camposForm.attr    = new Array();
	// Este �ndice armazena o valor (estuf, muncod) do atributo (id, name) a ser considerado na valida��o
	camposForm.valueAttr  = new Array();
	// Este �ndice armazena a label (Estado, Munic�pio) do campo (estuf, muncod) a ser considerado na valida��o
	camposForm.labelAttr  = new Array();
	// Este �ndice armazena a fun��o de retorno booleano que dir� se a fun��o de valida��o deve ser checada
	camposForm.funcaoFlag = new Array();
	// Este �ndice armazena o nome das fun��es de valida��o do campo
	camposForm.funcaoValidacao = new Array();

	// Mensagem Padr�o
	var msgPadrao = new Object();
	// Flag de verifica��o de existencia de MSG
	msgPadrao.flagMsg = false;
	// Mensagem de valida��o de campo obrigat�rio
	msgPadrao.validaObrigatorio 	   		= new Object();
	msgPadrao.validaObrigatorio.msg    		= 'Os campos abaixo são de preenchimento obrigatório:';
	msgPadrao.validaObrigatorio.attr   		= new Array();
	msgPadrao.validaObrigatorio.msgUnitaria = false;
	msgPadrao.validaObrigatorio.compilada 	= false;
	// Mensagem de valida��o de tamanho de campo
	msgPadrao.validaTamanho 	   		= new Object();
	msgPadrao.validaTamanho.msg   		= 'O campo <campo> deve possuir o tamanho de <tamanho> caractere(s).';
	msgPadrao.validaTamanho.attr  		= new Array();
	msgPadrao.validaTamanho.msgUnitaria = true;
	// Mensagem de valida��o de tamanho de campo m�nimo
	msgPadrao.validaTamanhoMinimo 	   		  = new Object();
	msgPadrao.validaTamanhoMinimo.msg   	  = 'O campo <campo> deve possuir o tamanho mínimo de <tamanho> caractere(s).';
	msgPadrao.validaTamanhoMinimo.attr  	  = new Array();
	msgPadrao.validaTamanhoMinimo.msgUnitaria = true;
	// Mensagem de valida��o de tamanho de campo m�ximo
	msgPadrao.validaTamanhoMaximo 	   		  = new Object();
	msgPadrao.validaTamanhoMaximo.msg   	  = 'O campo <campo> deve possuir o tamanho máximo de <tamanho> caractere(s).';
	msgPadrao.validaTamanhoMaximo.attr  	  = new Array();
	msgPadrao.validaTamanhoMaximo.msgUnitaria = true;
	// Mensagem de valida��o de campo num�rico
	msgPadrao.validaMascaraNumerica 	   		= new Object();
	msgPadrao.validaMascaraNumerica.msg   	  	= 'O campo <campo> deve possuir o formato <mascara>';
	msgPadrao.validaMascaraNumerica.attr  	  	= new Array();
	msgPadrao.validaMascaraNumerica.msgUnitaria = true;
	// Mensagem de valida��o de campo data
	msgPadrao.validaData 	   		 = new Object();
	msgPadrao.validaData.msg   	  	 = 'Os campos abaixo devem ser preenchidos com uma data válida e no formato correto: ##/##/####';
	msgPadrao.validaData.attr  	  	 = new Array();
	msgPadrao.validaData.msgUnitaria = false;
	// Mensagem de valida��o de campo M�s/Ano
	msgPadrao.validaMesAno 	   		 	= new Object();
	msgPadrao.validaMesAno.msg   	  	= 'Os campos abaixo devem ser preenchidos com um mês/ano válidos e no formato correto: ##/####';
	msgPadrao.validaMesAno.attr  	  	= new Array();
	msgPadrao.validaMesAno.msgUnitaria 	= false;
	// Mensagem de valida��o de express�o l�gica
	msgPadrao.validaExpressaoNumerica 	   		  = new Object();
	msgPadrao.validaExpressaoNumerica.msg   	  = 'O campo <campo> deve ser preenchido com um valor <expressao><numero>';
	msgPadrao.validaExpressaoNumerica.attr  	  = new Array();
	msgPadrao.validaExpressaoNumerica.msgUnitaria = true;
	// Mensagem de valida��o de CNPJ
	msgPadrao.validaCnpj 	   		  	= new Object();
	msgPadrao.validaCnpj.msg   	  		= 'Os campos abaixo devem ser preenchidos com um CNPJ válido:';
	msgPadrao.validaCnpj.attr  	  	 	= new Array();
	msgPadrao.validaCnpj.msgUnitaria 	= false;
	// Mensagem de valida��o de CPF
	msgPadrao.validaCpf 	   		  	= new Object();
	msgPadrao.validaCpf.msg   	  		= 'Os campos abaixo devem ser preenchidos com um CPF válido:';
	msgPadrao.validaCpf.attr  	  	 	= new Array();
	msgPadrao.validaCpf.msgUnitaria 	= false;
	// Mensagem de valida��o de campo externo
	msgPadrao.validaExterno 	       	= new Object();
	msgPadrao.validaExterno.msg        	= 'Outras valida��es:';
	msgPadrao.validaExterno.attr       	= new Array();
	msgPadrao.validaExterno.msgUnitaria = false;

	/**
	 * function addCampo
	 * @description Adiciona um campo do formul�rio ao validador
	 * @param 		string attr 	 - Compreende ao nome do atributo (id, name, type) do campo que ser� usado para as valida��es
	 * @param 		string valueAttr - Compreende ao valor (estuf, muncod) do atributo (id, name, type) do campo que ser� usado para as valida��es
	 * @param 		string labelAttr - Compreende �  label do campo que ser� exibido nas mensagens de valida��o
	 * @return 		object this
	 * @author 		Felipe Chiavicatti
	 * @since 		16/01/2015
	 */
	this.addCampo = function ( attr, valueAttr, labelAttr ){
		camposForm.attr.push( attr );
		
		camposForm.valueAttr.push('"' + valueAttr + '"');
		camposForm.labelAttr.push( labelAttr );

		camposForm.funcaoFlag.push( new Array() );
		camposForm.param.push( new Array() );
		camposForm.funcaoValidacao.push( new Array() );

		return this;
	}

	/**
	 * function addValidaObrigatorio
	 * @description Valida se o campo est� vazio.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o
	 * 										 ser� aplicada; caso retorne FALSE a valida��o N�O ser� aplicada. Par�metro obrigat�rio.
	 * @return 		object 	 	this
	 * @author 		Felipe Chiavicatti
	 * @since 		16/01/2015
	 */
	this.addValidaObrigatorio = function ( funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaObrigatorio'] = funcaoFlag;
		camposForm.funcaoValidacao[ ind ].push('validaObrigatorio');

		return this;
	}

	function validaObrigatorio( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];

		var valueCampo 	= '';
		if ( jQuery('[' + attr + '=' + valueAttr + ']').attr('type') == 'radio' || jQuery('[' + attr + '=' + valueAttr + ']').attr('type') == 'checkbox'){
			valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']:checked').val();
			valueCampo 	= jQuery.trim( valueCampo );
		}else{
			valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
			valueCampo 	= jQuery.trim( valueCampo );
		}

		// Valida se o campo est� vazio
		if ( valueCampo == '' ){
			msgPadrao.flagMsg = true;
			msgPadrao.validaObrigatorio.attr.push( (labelAttr ? labelAttr : attr) );
		}
	}

	/**
	 * function addValidaTamanho
	 * @description Valida o tamanho EXATO de caracteres que o campo deve conter.
	 * @param 		integer		tamanhoString - Define o tamanho exato que a string deve conter. Par�metro obrigat�rio.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		14/04/2015
	 */
	this.addValidaTamanho = function ( tamanhoString, funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaTamanho'] = funcaoFlag;
		camposForm.param[ ind ]['validaTamanho'] 	  = tamanhoString;
		camposForm.funcaoValidacao[ ind ].push('validaTamanho');

		return this;
	}

	function validaTamanho( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
		var param 	  = camposForm.param[indice]['validaTamanho'];

		var valueCampo 	= '';
		valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 	= jQuery.trim( valueCampo );

		// Valida se o campo est� vazio
		if ( valueCampo.length < param ){
			msgPadrao.flagMsg = true;
			var msg = msgPadrao.validaTamanho.msg.replace("<tamanho>", param)
												 .replace('<campo>', (labelAttr ? labelAttr : attr));
			msgPadrao.validaTamanho.attr.push( msg );
		}
	}

	/**
	 * function addValidaTamanhoMinimo
	 * @description Valida o tamanho M�?NIMO de caracteres que o campo deve conter.
	 * @param 		integer		tamanhoString - Define o tamanho m�nimo que a string deve conter. Par�metro obrigat�rio.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		16/04/2015
	 */
	this.addValidaTamanhoMinimo = function ( tamanhoString, funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaTamanhoMinimo'] = funcaoFlag;
		camposForm.param[ ind ]['validaTamanhoMinimo'] 	  = tamanhoString;
		camposForm.funcaoValidacao[ ind ].push('validaTamanhoMinimo');

		return this;
	}

	function validaTamanhoMinimo( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
		var param 	  = camposForm.param[indice]['validaTamanhoMinimo'];

		var valueCampo 	= '';
		valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 	= jQuery.trim( valueCampo );

		// Valida se o campo est� vazio
		if ( valueCampo.length < param ){
			msgPadrao.flagMsg = true;
			var msg = msgPadrao.validaTamanhoMinimo.msg.replace("<tamanho>", param)
													   .replace('<campo>', (labelAttr ? labelAttr : attr));
			msgPadrao.validaTamanhoMinimo.attr.push( msg );
		}
	}

	/**
	 * function addValidaTamanhoMaximo
	 * @description Valida o tamanho M�?XIMO de caracteres que o campo deve conter.
	 * @param 		integer		tamanhoString - Define o tamanho m�ximo que a string deve conter. Par�metro obrigat�rio.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		16/04/2015
	 */
	this.addValidaTamanhoMaximo = function ( tamanhoString, funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaTamanhoMaximo'] = funcaoFlag;
		camposForm.param[ ind ]['validaTamanhoMaximo'] 	  = tamanhoString;
		camposForm.funcaoValidacao[ ind ].push('validaTamanhoMaximo');

		return this;
	}

	function validaTamanhoMaximo( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
		var param 	  = camposForm.param[indice]['validaTamanhoMaximo'];

		var valueCampo 	= '';
		valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 	= jQuery.trim( valueCampo );

		// Valida se o campo est� vazio
		if ( valueCampo.length > param ){
			msgPadrao.flagMsg = true;
			var msg = msgPadrao.validaTamanhoMaximo.msg.replace("<tamanho>", param)
													   .replace('<campo>', (labelAttr ? labelAttr : attr));
			msgPadrao.validaTamanhoMaximo.attr.push( msg );
		}
	}

	/**
	 * function addValidaMascaraNumerica
	 * @description Valida se o campo condiz com a mêscara passada.
	 * @param 		string		mascara - Define a m�scara que deve ser usada no preenchimento do campo (Ex.: ##/##/#### ou #####-### ou ########## etc)
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		17/04/2015
	 */
	this.addValidaMascaraNumerica = function ( mascara, funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaMascaraNumerica'] = funcaoFlag;
		camposForm.param[ ind ]['validaMascaraNumerica'] 	  = mascara;
		camposForm.funcaoValidacao[ ind ].push('validaMascaraNumerica');

		return this;
	}

	function validaMascaraNumerica( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
		var mascara   = camposForm.param[indice]['validaMascaraNumerica'];

		var valueCampo 	= '';
		valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 	= jQuery.trim( valueCampo );

		// Verifica se o valor do campo tem o mesmo tamanho da m�scara
		if ( valueCampo.length != mascara.length ){
			msgPadrao.flagMsg = true;
			var msg = msgPadrao.validaMascaraNumerica.msg.replace("<mascara>", mascara)
													     .replace('<campo>', (labelAttr ? labelAttr : attr));
			msgPadrao.validaMascaraNumerica.attr.push( msg );
			return false;
		}

		var stringValue   = '';
		var stringMascara = '';
		for (var i=0; i < mascara.length; i++){
			stringValue   = valueCampo[i] || '';
			stringMascara = mascara[i] || '';

			if ( (stringMascara == '#' && isNaN(stringValue)) || (stringMascara != '#' && stringMascara != stringValue) ){
				msgPadrao.flagMsg = true;
				var msg = msgPadrao.validaMascaraNumerica.msg.replace("<mascara>", mascara)
														   	 .replace('<campo>', (labelAttr ? labelAttr : attr));
				msgPadrao.validaMascaraNumerica.attr.push( msg );
				break;
			}
		}

	}

	/**
	 * function addValidaData
	 * @description Valida se a data informada no campo � valida.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		17/04/2015
	 */
	this.addValidaData = function ( funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaData'] = funcaoFlag;
//		camposForm.param[ ind ]['validaData'] 	  = mascara;
		camposForm.funcaoValidacao[ ind ].push('validaData');

		return this;
	}

	function validaData( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
//		var mascara   = camposForm.param[indice]['validaData'];

		var valueCampo 	= '';
		valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 	= jQuery.trim( valueCampo );

		var day   = valueCampo.substring(0,2);
		var month = valueCampo.substring(3,5);
		var year  = valueCampo.substring(6,10);

		if ( day.length != 2 || month.length != 2 || year.length != 4 || isNaN(day) || isNaN(month) || isNaN(year) ){
			msgPadrao.flagMsg = true;
			msgPadrao.validaData.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
		}

		if( (month==01) || (month==03) || (month==05) || (month==07) || (month==08) || (month==10) || (month==12) )    {//mes com 31 dias
			if( (day < 01) || (day > 31) ){
				msgPadrao.flagMsg = true;
				msgPadrao.validaData.attr.push( (labelAttr ? labelAttr : attr) );
			}
		} else if( (month==04) || (month==06) || (month==09) || (month==11) ){//mes com 30 dias
			if( (day < 01) || (day > 30) ){
				msgPadrao.flagMsg = true;
				msgPadrao.validaData.attr.push( (labelAttr ? labelAttr : attr) );
			}
		} else if( (month==02) ){//February and leap year
			if( (year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) ) ){
				if( (day < 01) || (day > 29) ){
					msgPadrao.flagMsg = true;
					msgPadrao.validaData.attr.push( (labelAttr ? labelAttr : attr) );
				}
			} else {
				if( (day < 01) || (day > 28) ){
					msgPadrao.flagMsg = true;
					msgPadrao.validaData.attr.push( (labelAttr ? labelAttr : attr) );
				}
			}
		} else if ( month < 1 || month > 12 ){
			msgPadrao.flagMsg = true;
			msgPadrao.validaData.attr.push( (labelAttr ? labelAttr : attr) );
		}
	}

	/**
	 * function addValidaMesAno
	 * @description Valida se o M�s/Ano informado no campo � valido.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		17/04/2015
	 */
	this.addValidaMesAno = function ( funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaMesAno'] = funcaoFlag;
//		camposForm.param[ ind ]['validaMesAno'] 	  = mascara;
		camposForm.funcaoValidacao[ ind ].push('validaMesAno');

		return this;
	}

	function validaMesAno( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
//		var mascara   = camposForm.param[indice]['validaMesAno'];

		var valueCampo 	= '';
		valueCampo 	= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 	= jQuery.trim( valueCampo );

		var day   = '01'; //valueCampo.substring(0,2);
		var month = valueCampo.substring(0,2);
		var year  = valueCampo.substring(3,7);

		if ( day.length != 2 || month.length != 2 || year.length != 4 || isNaN(month) || isNaN(year) || (month < 1 || month > 12)){
			msgPadrao.flagMsg = true;
			msgPadrao.validaMesAno.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
		}
	}

	/**
	 * function addValidaCnpj
	 * @description Valida se o CNPJ informado no campo � valido.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		20/04/2015
	 */
	this.addValidaCnpj = function ( funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaCnpj'] = funcaoFlag;
		camposForm.funcaoValidacao[ ind ].push('validaCnpj');

		return this;
	}

	function validaCnpj( camposForm, indice ){
		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];

		var valueCampo 	= '';
		valueCampo 		= jQuery('[' + attr + '=' + valueAttr + ']').val();
		var cnpj 		= jQuery.trim( valueCampo );
	    cnpj 			= cnpj.replace(/[^\d]+/g,'');

	    if(cnpj == ''){
//			msgPadrao.flagMsg = true;
//			msgPadrao.validaMesAno.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    };

	    if (cnpj.length != 14){
			msgPadrao.flagMsg = true;
			msgPadrao.validaCnpj.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    }

	    // Elimina CNPJs invalidos conhecidos
	    if (cnpj == "00000000000000" ||
	        cnpj == "11111111111111" ||
	        cnpj == "22222222222222" ||
	        cnpj == "33333333333333" ||
	        cnpj == "44444444444444" ||
	        cnpj == "55555555555555" ||
	        cnpj == "66666666666666" ||
	        cnpj == "77777777777777" ||
	        cnpj == "88888888888888" ||
	        cnpj == "99999999999999"){

			msgPadrao.flagMsg = true;
			msgPadrao.validaCnpj.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    }

	    // Valida DVs
	    tamanho = cnpj.length - 2
	    numeros = cnpj.substring(0,tamanho);
	    digitos = cnpj.substring(tamanho);
	    soma = 0;
	    pos = tamanho - 7;
	    for (i = tamanho; i >= 1; i--) {
	      soma += numeros.charAt(tamanho - i) * pos--;
	      if (pos < 2)
	            pos = 9;
	    }
	    resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
	    if (resultado != digitos.charAt(0)){
			msgPadrao.flagMsg = true;
			msgPadrao.validaCnpj.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    }

	    tamanho = tamanho + 1;
	    numeros = cnpj.substring(0,tamanho);
	    soma = 0;
	    pos = tamanho - 7;
	    for (i = tamanho; i >= 1; i--) {
	      soma += numeros.charAt(tamanho - i) * pos--;
	      if (pos < 2)
	            pos = 9;
	    }
	    resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
	    if (resultado != digitos.charAt(1)){
			msgPadrao.flagMsg = true;
			msgPadrao.validaCnpj.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    }
	}


	/**
	 * function addValidaCpf
	 * @description Valida se o CPF informado no campo � valido.
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		20/04/2015
	 */
	this.addValidaCpf = function ( funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaCpf'] = funcaoFlag;
		camposForm.funcaoValidacao[ ind ].push('validaCpf');

		return this;
	}

	function validaCpf( camposForm, indice ){
		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];

		var valueCampo 	= '';
		valueCampo 		= jQuery('[' + attr + '=' + valueAttr + ']').val();
		var strCPF 		= jQuery.trim( valueCampo );
		strCPF 			= strCPF.replace(/[^\d]+/g,'');

	    var Soma;
	    var Resto;
	    Soma = 0;

	    if (strCPF == ""){
//	    	msgPadrao.flagMsg = true;
//	    	msgPadrao.validaCpf.attr.push( (labelAttr ? labelAttr : attr) );
	    	return false;
	    }

		if (strCPF == "00000000000"){
			msgPadrao.flagMsg = true;
			msgPadrao.validaCpf.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
		}

		for (i=1; i<=9; i++){
			Soma = Soma + parseInt(strCPF.substring(i-1, i)) * (11 - i);
		}
		Resto = (Soma * 10) % 11;

	    if ((Resto == 10) || (Resto == 11)){
	    	Resto = 0;
	    }

	    if (Resto != parseInt(strCPF.substring(9, 10)) ){
			msgPadrao.flagMsg = true;
			msgPadrao.validaCpf.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    }

		Soma = 0;
	    for (i = 1; i <= 10; i++){
	    	Soma = Soma + parseInt(strCPF.substring(i-1, i)) * (12 - i);
	    }
	    Resto = (Soma * 10) % 11;

	    if ((Resto == 10) || (Resto == 11)){
	    	Resto = 0;
	    }
	    if (Resto != parseInt(strCPF.substring(10, 11) ) ){
			msgPadrao.flagMsg = true;
			msgPadrao.validaCpf.attr.push( (labelAttr ? labelAttr : attr) );
			return false;
	    }
	}


	/**
	 * function addValidaExpressaoNumerica
	 * @description Valida se a express�o n�ºmerica informada aplicada ao valor do campo � valida ( Ex.: numeroCampo (10) operadorRelacional (>) numeroPassado (8)) <=> (10 > 8) )
	 * @param 		string		operadorRelacional - Define um operador relacional ( = | <> | > | >= | < | <= ) para ser aplicado na express�o.
	 * @param 		numeric		numero - Define um valor para ser aplicado na express�o. Pode ser passado o elemento do DOM para valida��es din�micas, ou seja:
	 * 									 .addValidaExpressaoNumerica('=', document.getElementById('exemplo') ).
	 * @param 		function	funcaoFlag - Define uma fun��o condicional de retorno booleano. Caso retorne TRUE, a valida��o ser� aplicada; caso retorne
	 * 										 FALSE a valida��o N�O ser� aplicada. Par�metro opcional.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		20/04/2015
	 */
	this.addValidaExpressaoNumerica = function (operadorRelacional, numero, funcaoFlag ){
		funcaoFlag = funcaoFlag || '';
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoFlag[ ind ]['validaExpressaoNumerica'] 				= funcaoFlag;
		camposForm.param[ ind ]['validaExpressaoNumerica'] 	 					= new Object();
		camposForm.param[ ind ]['validaExpressaoNumerica'].numero 				= numero;
		camposForm.param[ ind ]['validaExpressaoNumerica'].operadorRelacional 	= operadorRelacional;
		camposForm.funcaoValidacao[ ind ].push('validaExpressaoNumerica');

		return this;
	}

	function validaExpressaoNumerica( camposForm, indice ){

		var attr 	  = camposForm.attr[indice];
		var	valueAttr = camposForm.valueAttr[indice];
		var	labelAttr = camposForm.labelAttr[indice];
		var param     = camposForm.param[indice]['validaExpressaoNumerica'];

		if (typeof param.numero == 'object'){
			var numero = param.numero.value;
		}else{
			var numero = param.numero;
		}

		var valueCampo 	= '';
		valueCampo 		= jQuery('[' + attr + '=' + valueAttr + ']').val();
		valueCampo 		= jQuery.trim( valueCampo );

		var calculo = new Calculo();

		var txtOperador 	= '';
		var returnExpressao = true;
		switch ( param.operadorRelacional ){
			case '<>':
				txtOperador 	= 'diferente de ';
				returnExpressao = calculo.comparar(valueCampo, numero, '<>');
				break;
			case '=':
				txtOperador 	= 'igual a ';
				returnExpressao = calculo.comparar(valueCampo, numero, '=');
				break;
			case '>':
				txtOperador 	= 'maior do que ';
				returnExpressao = calculo.comparar(valueCampo, numero, '>');
				break;
			case '>=':
				txtOperador 	= 'maior ou igual a ';
				returnExpressao = calculo.comparar(valueCampo, numero, '>=');
				break;
			case '<':
				txtOperador 	= 'menor do que ';
				returnExpressao = calculo.comparar(valueCampo, numero, '<');
				break;
			case '<=':
				txtOperador 	= 'menor ou igual a ';
				returnExpressao = calculo.comparar(valueCampo, numero, '<=');
				break;
		}

		if ( returnExpressao == false ){
			msgPadrao.flagMsg = true;
			var msg = msgPadrao.validaExpressaoNumerica.msg.replace("<numero>", numero)
														   .replace('<expressao>', txtOperador)
													   	   .replace('<campo>', (labelAttr ? labelAttr : attr));
			msgPadrao.validaExpressaoNumerica.attr.push( msg );
		}
	}

	/**
	 * function addValidaExterno
	 * @description Define um validador indicado pelo desenvolvedor, ou seja, que ser� constru�do pelo desenvolvedor.
	 * @param 		function	funcaoValida - Define uma fun��o que ser� aplicada como um validador. Caso retorne TRUE, a valida��o passa; caso retorne 'STRING',
	 * 										   essa ser� usada como mensagem de erro do validador.
	 * @return 		object		this
	 * @author 		Felipe Chiavicatti
	 * @since 		16/01/2015
	 */
	this.addValidaExterno = function ( funcaoValida ){
		var ind = camposForm.attr.length;
		ind--;

		camposForm.funcaoValidacao[ ind ].push( funcaoValida );

		return this;
	}

	this.enviarFormulario = function ( idForm, hiddenCtrl, msgHiddenCtrl, ajaxType ){

		limpaMsg();

		hiddenCtrl 		= hiddenCtrl || '';
		msgHiddenCtrl 	= msgHiddenCtrl || 'salvar';
		ajaxType 		= ajaxType || false;
		for (var i=0; i < camposForm.attr.length; i++){
			var attr 	  = camposForm.attr[i];
			var	valueAttr = camposForm.valueAttr[i];
			var	labelAttr = camposForm.labelAttr[i];

			var resultadoFuncao = '';

			if ( jQuery('[' + attr + '=' + valueAttr + ']')[0] == undefined ){
				continue;
//				alert( 'FALHA NA VALIDA��O!\nCampo de formul�rio n�o encontrado:\nAtributo:' + attr + '\nValor atributo:' + valueAttr + '\nLabel:' + labelAttr );
//				return false;
			}
			for (var a=0; a<camposForm.funcaoValidacao[i].length; a++){
				var funcaoValidacao = camposForm.funcaoValidacao[i][a];
				if ( typeof funcaoValidacao == 'function' ){
					resultadoFuncao = funcaoValidacao();

					if ( resultadoFuncao != true ){
						msgPadrao.flagMsg = true;
						msgPadrao.validaExterno.attr.push( resultadoFuncao );
					}
				}else{
					var flagValida = true;
					if ( typeof camposForm.funcaoFlag[i][funcaoValidacao] == 'function' ){
						flagValida = camposForm.funcaoFlag[i][funcaoValidacao]();
					}

					if ( flagValida ){
						switch ( funcaoValidacao ){
							case 'validaObrigatorio':
								validaObrigatorio( camposForm, i );
								break;
							case 'validaTamanho':
								validaTamanho( camposForm, i );
								break;
							case 'validaTamanhoMinimo':
								validaTamanhoMinimo( camposForm, i );
								break;
							case 'validaTamanhoMaximo':
								validaTamanhoMaximo( camposForm, i );
								break;
							case 'validaMascaraNumerica':
								validaMascaraNumerica( camposForm, i );
								break;
							case 'validaData':
								validaData( camposForm, i );
								break;
							case 'validaMesAno':
								validaMesAno( camposForm, i );
								break;
							case 'validaExpressaoNumerica':
								validaExpressaoNumerica( camposForm, i );
								break;
							case 'validaCnpj':
								validaCnpj( camposForm, i );
								break;
							case 'validaCpf':
								validaCpf( camposForm, i );
								break;
						}
					}
				}
			}

		}
		
		if ( msgPadrao.flagMsg == true ){
			if ( typeof swal != 'undefined' ){
				swal({title	: "Validação de Formulário", 
					  text	: trataMsgValidacao(), 
					  type	: "warning",
					  html	: true});
			}else{
				alert( trataMsgValidacaoAlert() );
			}
			return false;
		}else{

			if ( hiddenCtrl ){
				jQuery('#' + hiddenCtrl).val(msgHiddenCtrl);
			}

			prepara_formulario();

			if (ajaxType == false) {
				sigBeforeunload = function() {};
				jQuery('#' + idForm).submit();
			} else {
				return true;
			}
		}
	}

	function trataMsgValidacao(){
		var i;
		var msg = '';

		for ( i in msgPadrao ){
			if( typeof msgPadrao[i] == 'object' && typeof msgPadrao[i].attr == 'object' && msgPadrao[i].attr.length && msgPadrao[i].msgUnitaria != true ){
				msg += ( msg == '' ? msgPadrao[i].msg + '<br>' : '<br>' + msgPadrao[i].msg + '<br>');
				msg += '<div style="margin-left:20px;">' + msgPadrao[i].attr.join('<br>') + '<br> </div>';
			}else if ( typeof msgPadrao[i] == 'object' && typeof msgPadrao[i].attr == 'object' && msgPadrao[i].attr.length ){
				console.log(msgPadrao[i]);
				msg += '<br>' + msgPadrao[i].attr.join('<br>') + '<br>';
			}
		}

		return '<div style="text-align:left;">' + msg + '</div>';
	}
	
	function trataMsgValidacaoAlert(){
		var i;
		var msg = '';

		for ( i in msgPadrao ){
			if( typeof msgPadrao[i] == 'object' && typeof msgPadrao[i].attr == 'object' && msgPadrao[i].attr.length && msgPadrao[i].msgUnitaria != true ){
				msg += ( msg == '' ? msgPadrao[i].msg + '\n' : '\n' + msgPadrao[i].msg + '\n');
				msg += msgPadrao[i].attr.join('\n') + '\n';
			}else if ( typeof msgPadrao[i] == 'object' && typeof msgPadrao[i].attr == 'object' && msgPadrao[i].attr.length ){
				msg += '\n' + msgPadrao[i].attr.join('\n') + '\n';
			}
		}

		return msg;
	}
	
	function limpaMsg(){
		msgPadrao.flagMsg = false;

		for ( i in msgPadrao ){
			if(typeof msgPadrao[i] == 'object'){
				msgPadrao[i].attr = new Array();
			}
		}
	}
}
