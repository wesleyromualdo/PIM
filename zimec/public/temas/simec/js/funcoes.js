semValor = function(val) {
    return !val;
};
comValor = function(val) {
    return val;
};
define = function() {
};
String.prototype.repeat = function(count) {
    if (count < 1) return '';
    var result = '', pattern = this.valueOf();
    while (count > 1) {
        if (count & 1) result += pattern;
        count >>= 1, pattern += pattern;
    }
    return result + pattern;
};
String.prototype.trim = function() {
    return this.ltrim().rtrim().toString();
};
String.prototype.ltrim = function() {
    return new String(this.replace(eval("/^\ */"), '')).toString();
};
String.prototype.rtrim = function() {
    return new String(this.replace(eval("/\ *$/"), '')).toString();
};
String.prototype.strReplace = function(strAntiga, strNova) {
    return new String(this.replace(eval("/" + strAntiga + "/g"), new String(strNova)));
};
String.prototype.ucFirst = function() {
    return this.charAt(0).toUpperCase() + this.substr(1);
};
String.prototype.lcFirst = function() {
    return this.charAt(0).toLowerCase() + this.substr(1);
};
/**
 * função para fazer CamelCase();
 */
String.prototype.CamelCase = function(tipo, dash) {
    dash = '_' || dash;
    palavra = this.strReplace(' ', dash);
    arPalavra = palavra.split(dash);
    if (arPalavra.length > 1) {
        palavra = this.strReplace(' ', dash).toLowerCase().retiraAcentos();
        arPalavra = palavra.split(dash);
        primeira = arPalavra.shift();
        palavraFim = '';
        for (i in arPalavra) {
            palavraFim += arPalavra[i].ucFirst();
        }
    } else {
        primeira = this;
        palavraFim = '';
    }
    if (tipo !== 'lower')
        primeira = primeira.ucFirst();
    return primeira + palavraFim;
};
String.prototype.camel2dash = function() {
    return this.replace(/([a-z])([A-Z])/g, '$1-$2').toLowerCase();
}
/**
 * função para fazer upperCamelCase();
 */
String.prototype.upperCamelCase = function(dash) {
    return this.CamelCase('upper', dash);
};
/**
 * função para fazer lowerCamelCase();
 */
String.prototype.lowerCamelCase = function(dash) {
    return this.CamelCase('lower', dash);
};
/**
 * função para fazer lowerCamelCase();
 */
String.prototype.lowerCamelCase = function() {
    palavra = this.upperCamelCase();
    return palavra.charAt(0).toLowerCase() + palavra.substr(1);
};
/**
 * funcao para retirar os acentos da string
 */
String.prototype.retiraAcentos = function() {
    str = this;
    stA = new String('çàèìòùâêîôûäëïöüáéíóúãĩõũÇÀÈÌÒÙÂÊÎÔÛÄËÏÖÜÁÉÍÓÚÃĨÕŨ');
    stB = new String('caeiouaeiouaeiouaeiouaiouCAEIOUAEIOUAEIOUAEIOUAIOU');
    for (i in stA) {
        str = str.strReplace(stA.charAt(i), stB.charAt(i));
    }
    return str;
};
String.prototype.retiraEspeciais = function() {
    return this.replace(/[-[\]{}()*+?%&@!?¨:;'"<>/=\\^$|#\b]/g, "");
};
String.prototype.retiraEspeciaisCustom = function () {
    return this.replace(/[[\]{}()+?%@!?¨:;'"<>=\\^$|#\b]/g, "");
};
String.prototype.makeLowerUnderLine = function() {
    return this.toLowerCase().retiraAcentos().strReplace('[^a-zA-Z0-9_]', '_');
};
String.prototype.makeUpperUnderLine = function() {
    return this.toUpperCase().retiraAcentos().strReplace('[^a-zA-Z0-9_]', '_');
};
String.prototype.extenso = function(c){
    var ex = [
        ["zero", "um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove", "dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezessete", "dezoito", "dezenove"],
        ["dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa"],
        ["cem", "cento", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos"],
        ["mil", "milhão", "bilhão", "trilhão", "quadrilhão", "quintilhão", "sextilhão", "setilhão", "octilhão", "nonilhão", "decilhão", "undecilhão", "dodecilhão", "tredecilhão", "quatrodecilhão", "quindecilhão", "sedecilhão", "septendecilhão", "octencilhão", "nonencilhão"]
    ];
    var a, n, v, i, n = this.replace(c ? /[^,\d]/g : /\D/g, "").split(","), e = " e ", $ = "real", d = "centavo", sl;
    for(var f = n.length - 1, l, j = -1, r = [], s = [], t = ""; ++j <= f; s = []){
        j && (n[j] = (("." + n[j]) * 1).toFixed(2).slice(2));
        if(!(a = (v = n[j]).slice((l = v.length) % 3).match(/\d{3}/g), v = l % 3 ? [v.slice(0, l % 3)] : [], v = a ? v.concat(a) : v).length) continue;
        for(a = -1, l = v.length; ++a < l; t = ""){
            if(!(i = v[a] * 1)) continue;
            i % 100 < 20 && (t += ex[0][i % 100]) ||
            i % 100 + 1 && (t += ex[1][(i % 100 / 10 >> 0) - 1] + (i % 10 ? e + ex[0][i % 10] : ""));
            s.push((i < 100 ? t : !(i % 100) ? ex[2][i == 100 ? 0 : i / 100 >> 0] : (ex[2][i / 100 >> 0] + e + t)) +
                ((t = l - a - 2) > -1 ? " " + (i > 1 && t > 0 ? ex[3][t].replace("?o", "?es") : ex[3][t]) : ""));
        }
        a = ((sl = s.length) > 1 ? (a = s.pop(), s.join(" ") + e + a) : s.join("") || ((!j && (n[j + 1] * 1 > 0) || r.length) ? "" : ex[0][0]));
        a && r.push(a + (c ? (" " + (v.join("") * 1 > 1 ? j ? d + "s" : (/0{6,}$/.test(n[0]) ? "de " : "") + $.replace("l", "is") : j ? d : $)) : ""));
    }
    return r.join(e);
};
/**
 * Função equivalente ao number_format do PHP
 * @param {Number} number
 * @param {String} decimals
 * @param {String} dec_point
 * @param {String} thousands_sep
 * @returns {String}
 */
function number_format(number, decimals, dec_point, thousands_sep) {
    number = (number + '').replace(/[^0-9+\-Ee.]/g, '');
    var n = !isFinite(+number) ? 0 : +number,
        prec = !isFinite(+decimals) ? 0 : Math.abs(decimals),
        sep = (typeof thousands_sep === 'undefined') ? ',' : thousands_sep,
        dec = (typeof dec_point === 'undefined') ? '.' : dec_point,
        s = '',
        toFixedFix = function(n, prec) {
            var k = Math.pow(10, prec);
            return '' + Math.round(n * k) / k;
        };
    // Fix for IE parseFloat(0.55).toFixed(0) = 0;
    s = (prec ? toFixedFix(n, prec) : '' + Math.round(n)).split('.');
    if (s[0].length > 3) {
        s[0] = s[0].replace(/\B(?=(?:\d{3})+(?!\d))/g, sep);
    }
    if ((s[1] || '').length < prec) {
        s[1] = s[1] || '';
        s[1] += new Array(prec - s[1].length + 1).join('0');
    }
    return s.join(dec);
}

/**
 * Função simuladora da sprintf da linguagem C
 */
function sprintf() {
    try {
        if (!arguments || arguments.length < 1 || !RegExp) {
            return;
        }
        var str = arguments[0];
        var re = /([^%]*)%('.|0|\x20)?(-)?(\d+)?(\.\d+)?(%|b|c|d|u|f|o|s|x|X)(.*)/;
        var a = b = [], numSubstitutions = 0, numMatches = 0;
        while (a = re.exec(str)) {
            var leftpart = a[1], pPad = a[2], pJustify = a[3], pMinLength = a[4];
            var pPrecision = a[5], pType = a[6], rightPart = a[7];
            numMatches++;
            if (pType == '%') {
                subst = '%';
            } else {
                numSubstitutions++;
                if (numSubstitutions >= arguments.length) {
                    alert('Error! Not enough function arguments (' +
                        (arguments.length - 1) + ', excluding the string)\n' +
                        'for the number of substitution parameters in string (' +
                        numSubstitutions + ' so far).');
                }
                var param = arguments[numSubstitutions];
                var pad = '';
                if (pPad && pPad.substr(0, 1) == "'") {
                    pad = leftpart.substr(1, 1);
                } else if (pPad) {
                    pad = pPad;
                }
                var justifyRight = true;
                if (pJustify && pJustify === "-")
                    justifyRight = false;
                var minLength = -1;
                if (pMinLength)
                    minLength = parseInt(pMinLength);
                var precision = -1;
                if (pPrecision && pType == 'f') {
                    precision = parseInt(pPrecision.substring(1));
                }
                var subst = param;
                switch (pType) {
                    case 'b':
                        subst = parseInt(param).toString(2);
                        break;
                    case 'c':
                        subst = String.fromCharCode(parseInt(param));
                        break;
                    case 'd':
                        subst = parseInt(param) ? parseInt(param) : 0;
                        break;
                    case 'u':
                        subst = Math.abs(param);
                        break;
                    case 'f':
                        subst = (precision > -1) ?
                            Math.round(parseFloat(param) * Math.pow(10, precision)) /
                            Math.pow(10, precision) : parseFloat(param);
                        break;
                    case 'o':
                        subst = parseInt(param).toString(8);
                        break;
                    case 's':
                        subst = param;
                        break;
                    case 'x':
                        subst = ('' + parseInt(param).toString(16)).toLowerCase();
                        break;
                    case 'X':
                        subst = ('' + parseInt(param).toString(16)).toUpperCase();
                        break;
                }
                var padLeft = minLength - subst.toString().length;
                if (padLeft > 0) {
                    var arrTmp = new Array(padLeft + 1);
                    var padding = arrTmp.join(pad ? pad : " ");
                } else {
                    var padding = "";
                }
            }
            str = leftpart + padding + subst + rightPart;
        }
        return str;
    }
    catch (e) {
        alert(e);
    }
}

/**
 * Função que retorna as chaves de um objeto
 * @param Object input
 * @param Boolean boolean
 * @returns Array
 */
function arrayKeys(input, notNull) {
    var output = new Array();
    var counter = 0;
    if (notNull) {
        for (i in input) {
            if (parseInt(i)) {
                output[counter++] = i;
            }
        }
    } else {
        for (i in input) {
            output[counter++] = i;
        }
    }

    return output;
}

/**
 * Converte uma string monetária para float
 * @param {String} price Valor monetário
 * @returns {Float}
 */
function price_to_float(price) {
    return parseFloat(price.replace('.', '').replace(',', '').replace('R$ ', '')) / 100;
}
/**
 * Converte um número para valor monetário
 * @param {Float} valor
 * @returns {String}
 */
function moeda(valor) {
    return 'R$ ' + number_format(valor, 2, ',', '.');
}
/**
 * Objeto de tratamento para númericos
 * @type Object
 */
var Numerico = {
    /**
     * Formata um numérico para string numérica
     * @param {Float} valor
     * @returns {String}
     */
    formatar: function(valor) {
        return number_format(valor, 2, ',', '.');
    },
    /**
     * Converte uma string numérica para um Float
     * @param {String} valor
     * @returns {Float}
     */
    desformatar: function(valor) {
        return parseFloat(valor.replace('R$ ', '').replace(/\./g, '').replace(',', '.').replace('(', '-').replace(')', ''));
    }
};
/**
 * Objeto de tratamento de moeda
 * @type Object
 */
var Moeda = {
    /**
     * Formata um numérico para string numérica
     * @param Float valor
     * @returns String
     */
    formatar: function(valor) {
        if (valor >= 0) {
            return 'R$ ' + Numerico.formatar(valor);
        } else {
            return '(R$ ' + Numerico.formatar(valor) + ')';
        }
    },
    /**
     * Converte uma string numérica para um Float
     * @param String valor
     * @returns Float
     */
    desformatar: function(valor) {
        return Numerico.desformatar(valor);
    },
    /**
     * Formata um numérico para string numérica
     * @param Float valor
     * @returns String
     */
    mascarar: function(valor) {
        return Moeda.formatar(valor);
    },
    /**
     * Converte uma string numérica para um Float
     * @param String valor
     * @returns Float
     */
    desmascarar: function(valor) {
        return Numerico.desformatar(valor);
    }
};
/**
 * Objeto de tratamento de data
 * @type Object
 */
Data = {
    formatar: function(valor) {
        return valor.split('-').reverse().join('/');
    }
};
/**
 * Objeto de tratamento de documento CPF
 * @type Object
 */
CPF = {
    formatar: function(valor) {
        var reg = /(\d{0,3})(\d{0,3})(\d{0,3})(\d{0,2})/;
        valor.replace(reg, "$1.$2.$3-$4");
        return valor;
    }
};
/**
 * Objeto de tratamento de documento CNPJ
 * @type Object
 */
CNPJ = {
    formatar: function(valor) {
        var reg = /(\d{0,2})(\d{0,3})(\d{0,3})(\d{0,4})(\d{0,2})/;
        return valor.replace(reg, "$1.$2.$3/$4-$5");
    }
};
/**
 * Objeto de tratamento de documento INSS
 * @type Object
 */
INSS = {
    formatar: function(valor) {
        var reg = /(\d{0,2})(\d{0,3})(\d{0,5})(\d{0,2})/;
        return valor.replace(reg, "$1.$2.$3/$4");
    }
};
/**
 * Objeto de tratamento de documento PIS
 * @type Object
 */
PIS = {
    formatar: function(valor) {
        var reg = /(\d{0,3})(\d{0,5})(\d{0,2})(\d{0,1})/;
        return valor.replace(reg, "$1.$2.$3-$4");
    }
};
/**
 * Objeto de tratamento de documento CPF/CNPJ
 * @type Object
 */
Documento = {
    formatar: function(valor) {
        if (valor.length == 11) {
            return CPF.formatar(valor);
        } else {
            return CNPJ.formatar(valor);
        }
    }
};
refresh = function() {
    switch(true){
        case Controle.getContexto().is(':visible'):
            if (Controle.action) {
                Sistema.tela(Controle.action);
                Sistema.alternar();
            }
            break;
        case $('.manual-sistema .tela-manual').is(':visible'):
            if (Controle.action) {
                Sistema.alternar('manual');
            }
            break;
    }

};
/**
 * Retorna uma string da data no formato solicitado
 * @returns {String}
 */
Date.prototype.getData = function(formato) {
    formato = formato || 'd/m/Y';
    var mes = this.getMonth() + 1;
    mes = (mes >= 10 ? mes : '0' + mes);
    var dia = this.getDate();
    dia = (dia < 10 ? '0' + dia : dia)
    switch (formato) {
        case 'Y/m/d':
            return this.getFullYear() + '/' + mes + '/' + dia;
        case 'd/m/Y':
            return dia + '/' + mes + '/' + this.getFullYear();
        case 'm/d/Y':
            return mes + '/' + dia + '/' + this.getFullYear();
        default:
            return '';
    }
};
Date.prototype.getDecimalseconds = function() {
    var a = this.getMilliseconds();
    if (a > 100) {
        return parseInt(a / 100);
    }
    return 0;
};
Date.prototype.getDataHora = function(formato) {
    var h = this.getHours();
    var m = this.getMinutes();
    return this.getData(formato) + ' ' + (h < 10 ? '0' + h : h) + ':' + (m < 10 ? '0' + m : m);
};

Date.prototype.getDataHoraSegundo = function(formato) {
    var s = this.getSeconds();
    return this.getDataHora(formato) + ':' + (s < 10 ? '0' + s : s);
};
Date.prototype.getDataHoraSegundoDecimos = function(formato) {
    return this.getDataHoraSegundo(formato) + '.' + this.getDecimalseconds();
};
Date.prototype.getHoraMinuto = function() {
    var h = this.getHours();
    var m = this.getMinutes();
    return (h < 10 ? '0' + h : h) + ':' + (m < 10 ? '0' + m : m);
};

Date.prototype.getHoraMinutoSegundo = function() {
    var s = this.getSeconds();
    return this.getHoraMinuto() + ':' + (s < 10 ? '0' + s : s);
};
Date.prototype.getHoraMinutoSegundoDecimos = function() {
    return this.getHoraMinutoSegundo() + '.' + this.getDecimalseconds();
};
Date.prototype.getMinuto = function() {
    var m = this.getMinutes();
    return (m < 10 ? '0' + m : m);
};

Date.prototype.getMinutoSegundo = function() {
    var s = this.getSeconds();
    return this.getMinuto() + ':' + (s < 10 ? '0' + s : s);
};
Date.prototype.getMinutoSegundoDecimos = function() {
    return this.getMinutoSegundo() + '.' + this.getDecimalseconds();
};

/**
 *
 * @param {String} formato
 * @returns {Date}
 */
String.prototype.toDate = function(formato) {
    var formato = formato || 'd/m/Y';
    var d = m = Y = h = i = s = 0;
    switch (formato) {
        case 'Y/m/d':
            var match = this.match(/^(\d{4})\/(\d{2})\/(\d{2})(|\s(0[0-9]|1[0-9]||2[0-3])\:([0-5][0-9])(|\:([0-5][0-9])))$/);
            if (!match) {
                return new Date(0, 0, 0, 0, 0, 0);
            }
            Y = match[1] ? match[1] : 0;
            m = match[2] ? match[2] : 0;
            d = match[3] ? match[3] : 0;
            break;
        case 'd/m/Y':
            var match = this.match(/^(\d{2})\/(\d{2})\/(\d{4})(|\s(0[0-9]|1[0-9]||2[0-3])\:([0-5][0-9])(|\:([0-5][0-9])))$/);
            if (!match) {
                return new Date(0, 0, 0, 0, 0, 0);
            }
            Y = match[3] ? match[3] : 0;
            m = match[2] ? match[2] : 0;
            d = match[1] ? match[1] : 0;
            break;
        case 'm/d/Y':
            var match = this.match(/^(\d{2})\/(\d{2})\/(\d{4})(|\s(0[0-9]|1[0-9]||2[0-3])\:([0-5][0-9])(|\:([0-5][0-9])))$/);
            if (!match) {
                return new Date(0, 0, 0, 0, 0, 0);
            }
            Y = match[3] ? match[3] : 0;
            m = match[1] ? match[1] : 0;
            d = match[2] ? match[2] : 0;
            break;
    }
    var h=0;
    var i=0;
    var s=0;
    if(match[5]){
        h = match[5] ? match[5] : 0;
        i = match[6] ? match[6] : 0;
        s = match[8] ? match[8] : 0;
    }

    return new Date(parseInt(new Number(Y)), parseInt(new Number(m)) - 1, parseInt(new Number(d)), parseInt(new Number(h)), parseInt(new Number(i)), parseInt(new Number(s)));
};


function valida_inss(inss) {
    //INSS: 99.999.99999/99
    var tmp = inss.replace(/[^\d]+/g, '');
    if(tmp.length<12){
        return false;
    }
    return true;
}

function valida_pis(pis) {
    var ftap = "3298765432";
    var total = 0;
    var i;
    var resto = 0;
    var numPIS = 0;
    var strResto = "";

    validaPis = function(val) {
        var pis = val.replace(/[^\d]+/g, '');
        if (pis == '' || pis.length != 11) {
            return false;
        }
        total = 0;
        resto = 0;
        numPIS = 0;
        strResto = "";
        numPIS = pis;
        if (numPIS == "" || numPIS == null) {
            return false;
        }
        for (i = 0; i <= 9; i++) {
            resultado = (numPIS.slice(i, i + 1)) * (ftap.slice(i, i + 1));
            total = total + resultado;
        }
        resto = (total % 11);
        if (resto != 0) {
            resto = 11 - resto;
        }
        if (resto == 10 || resto == 11) {
            strResto = resto + "";
            resto = strResto.slice(1, 2);
        }
        if (resto != (numPIS.slice(10, 11))) {
            return false;
        }
        return true;
    };
    return validaPis(pis);
}
;

function valida_cnpj(cnpj) {
    cnpj = cnpj.replace('.', '').replace('/', '').replace('-', '').replace('.', '');
    var numeros, digitos, soma, i, resultado, pos, tamanho, digitos_iguais;
    digitos_iguais = 1;
    if (cnpj.length !== 14) {
        return false;
    }

    for (i = 0; i < cnpj.length - 1; i++) {
        if (cnpj.charAt(i) != cnpj.charAt(i + 1) || cnpj.charAt(i + 1) == 0 || cnpj.charAt(i + 1) == 1 || cnpj.charAt(i + 1) == 9) {
            digitos_iguais = 0;
            break;
        }
    }
    if (!digitos_iguais) {
        tamanho = cnpj.length - 2;
        numeros = cnpj.substring(0, tamanho);
        digitos = cnpj.substring(tamanho);
        soma = 0;
        pos = tamanho - 7;

        for (i = tamanho; i >= 1; i--) {
            soma += numeros.charAt(tamanho - i) * pos--;
            if (pos < 2) {
                pos = 9;
            }
        }
        resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
        if (cnpj != '99999999999999' && cnpj != '11111111111111' && resultado != digitos.charAt(0)) {
            return false;
        }
        tamanho = tamanho + 1;
        numeros = cnpj.substring(0, tamanho);
        soma = 0;
        pos = tamanho - 7;
        for (i = tamanho; i >= 1; i--) {
            soma += numeros.charAt(tamanho - i) * pos--;
            if (pos < 2) {
                pos = 9;
            }
        }
        resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
        if (cnpj != '99999999999999' && cnpj != '11111111111111' && resultado != digitos.charAt(1)) {
            return false;
        }
        return true;
    } else {
        return false;
    }
}

function valida_cpf(cpf) {
    cpf = (cpf.replace('.', '').replace('.', '').replace('-', ''));
    var numeros, digitos, soma, i, resultado, digitos_iguais;
    digitos_iguais = 1;
    if (cpf.length < 11)
        return false;
    for (i = 0; i < cpf.length - 1; i++) {
        if (cpf.charAt(i) != cpf.charAt(i + 1))
        //if (cpf.charAt(i) != cpf.charAt(i + 1) || cpf.charAt(i + 1) == 1 || cpf.charAt(i) == 0 || cpf.charAt(i + 1) == 9)
        {
            digitos_iguais = 0;
            break;
        }
    }
    if (!digitos_iguais) {
        numeros = cpf.substring(0, 9);
        digitos = cpf.substring(9);
        soma = 0;
        for (i = 10; i > 1; i--)
            soma += numeros.charAt(10 - i) * i;
        resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
        if (resultado != digitos.charAt(0))
            return false;
        numeros = cpf.substring(0, 10);
        soma = 0;
        for (i = 11; i > 1; i--)
            soma += numeros.charAt(11 - i) * i;
        resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
        if (resultado != digitos.charAt(1))
            return false;
        return true;
    } else {
        return false;
    }
}

function valida_conta(_input) {
    // implementar validacao de banco
    return true;
}

function valida_agencia(valor) {
    var sNum = valor.replace("X",0).replace(/[^\d]+/g, '');
    if (sNum) {
        var /*sNum = sNum.val().replace('-', ''), */
            multiplicador = 6,
            arrValues = new Array();
        for (var i = 0; i < sNum.length; i++) {
            var dg = sNum.charAt(i) * multiplicador;
            arrValues[i] = dg;
            multiplicador++;
        }

        var d1 = arrValues[0],
            d2 = arrValues[1],
            d3 = arrValues[2],
            d4 = arrValues[3],
            dv = sNum.charAt(4);

        var resto = ((d1 + d2 + d3 + d4) % 11),newResto;
        if (resto == 10) {
            newResto = '0';
        } else {
            newResto = resto;
        }

        if (newResto != dv) {
            return false;
        }

        if (sNum == '00000') {
            return false;
        }

        return true;
    }

    // implementar validacao de agencia
    return true;
}


campoPreenchido = function() {
    return $(this).val() !== '';
};

function sleep(milliseconds) {
    var start = new Date().getTime();
    for (var i = 0; i < 1e7; i++) {
        if ((new Date().getTime() - start) > milliseconds) {
            break;
        }
    }
};

function formatFileSize(bytes) {
    if (typeof bytes !== 'number') {
        return '';
    }
    if (bytes >= 1000000000) {
        return (bytes / 1000000000).toFixed(2) + ' GB';
    }
    if (bytes >= 1000000) {
        return (bytes / 1000000).toFixed(2) + ' MB';
    }
    return (bytes / 1000).toFixed(2) + ' KB';
};

function xor(a,b){
    return (a && !b) || (!a && b);
}

function loadScript(url, className) {
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = url;
    if(className) {
        script.setAttribute('class', className);
    }
    document.body.appendChild(script);
}

// Gerar um id único
function guid() {
    function s4() {
        return Math.floor((1 + Math.random()) * 0x10000)
            .toString(16)
            .substring(1);
    }
    return s4() + s4() + '-' + s4() + '-' + s4() + '-' +
        s4() + '-' + s4() + s4() + s4();
}

function fontColor(hex){
    var hexToRgb = function (hex) {
        var bigint = parseInt(hex, 16);
        var r = (bigint >> 16) & 255;
        var g = (bigint >> 8) & 255;
        var b = bigint & 255;

        return [r, g, b];
    }

    var rgb = hexToRgb(hex.substring(1,7));
    var res = {
        'lightness' : Math.floor((Math.max(rgb[0], rgb[1], rgb[2]) + Math.min(rgb[0], rgb[1], rgb[2])) / 2),
        'average' : Math.floor((rgb[0] + rgb[1] + rgb[2]) / 3),
        'luminosity' : Math.floor((0.21 * rgb[0]) + (0.71 * rgb[1]) + (0.07 * rgb[2]))
    };
    var r = {
        'lightness': (res.lightness > 127 ? '#000000' : '#FFFFFF'),
        'average': (res.average > 127 ? '#000000' : '#FFFFFF'),
        'luminosity': (res.luminosity > 127 ? '#000000' : '#FFFFFF')
    };
    return r;
}