//tirar os espa�os das extremidades do valor passado (utilizada pela mascaraglobal)
function trim(valor) {
    valor += '';
    for (i = 0; i < valor.length; i++) {
        if (valor.substr(i, 1) != " ") {
            valor = valor.substr(i);
            break;
        }
        if (i == valor.length - 1) {
            valor = "";
        }
    }
    for (i = valor.length - 1; i >= 0; i--) {
        if (valor.substr(i, 1) != " ") {
            valor = valor.substr(0, i + 1);
            break;
        }
    }
    return valor;
}

/*
 objetivo: mascarar de acordo com a mascara passada
 caracteres: # - caracter a ser mascarado
 | - separador de mascaras
 modos (exemplos):
 mascara simples: "###-####"	                 mascara utilizando a mascara passada
 mascara composta: "###-####|####-####"       mascara de acordo com o tamanho (length) do valor passado
 mascara din�mica: "[###.]###,##"             multiplica o valor entre colchetes de acordo com o length do valor para que a mascara seja din�mica ex: ###.###.###.###,##
 utilizar no onkeyup do objeto
 ex: onkeyup="this.value = mascara_global('#####-###',this.value);"
 tratar o maxlength do objeto na p�gina (a fun��o n�o trata isso)
 */
function mascaraglobal(mascara, valor) {

    var mascara_utilizar;
    var mascara_limpa;
    var temp;
    var i;
    var j;
    var caracter;
    var separador;
    var dif;
    var validar;
    var mult;
    var ret;
    var tam;
    var tvalor;
    var valorm;
    var masct;
    tvalor = "";
    ret = "";
    caracter = "#";
    separador = "|";
    mascara_utilizar = "";
    valor = trim(valor);
    if (valor == "")return valor;
    temp = mascara.split(separador);
    dif = 1000;

    valorm = valor;
    //tirando mascara do valor j� existente
    for (i = 0; i < valor.length; i++) {
        if (!isNaN(valor.substr(i, 1))) {
            tvalor = tvalor + valor.substr(i, 1);
        }
    }
    valor = tvalor;

    //formatar mascara dinamica
    for (i = 0; i < temp.length; i++) {
        mult = "";
        validar = 0;
        for (j = 0; j < temp[i].length; j++) {
            if (temp[i].substr(j, 1) == "]") {
                temp[i] = temp[i].substr(j + 1);
                break;
            }
            if (validar == 1) mult = mult + temp[i].substr(j, 1);
            if (temp[i].substr(j, 1) == "[") validar = 1;
        }
        for (j = 0; j < valor.length; j++) {
            temp[i] = mult + temp[i];
        }
    }


    //verificar qual mascara utilizar
    if (temp.length == 1) {
        mascara_utilizar = temp[0];
        mascara_limpa = "";
        for (j = 0; j < mascara_utilizar.length; j++) {
            if (mascara_utilizar.substr(j, 1) == caracter) {
                mascara_limpa = mascara_limpa + caracter;
            }
        }
        tam = mascara_limpa.length;
    } else {
        //limpar caracteres diferente do caracter da m�scara
        for (i = 0; i < temp.length; i++) {
            mascara_limpa = "";
            for (j = 0; j < temp[i].length; j++) {
                if (temp[i].substr(j, 1) == caracter) {
                    mascara_limpa = mascara_limpa + caracter;
                }
            }

            if (valor.length > mascara_limpa.length) {
                if (dif > (valor.length - mascara_limpa.length)) {
                    dif = valor.length - mascara_limpa.length;
                    mascara_utilizar = temp[i];
                    tam = mascara_limpa.length;
                }
            } else if (valor.length < mascara_limpa.length) {
                if (dif > (mascara_limpa.length - valor.length)) {
                    dif = mascara_limpa.length - valor.length;
                    mascara_utilizar = temp[i];
                    tam = mascara_limpa.length;
                }
            } else {
                mascara_utilizar = temp[i];
                tam = mascara_limpa.length;
                break;
            }
        }
    }

    //validar tamanho da mascara de acordo com o tamanho do valor
    if (valor.length > tam) {
        valor = valor.substr(0, tam);
    } else if (valor.length < tam) {
        masct = "";
        j = valor.length;
        for (i = mascara_utilizar.length - 1; i >= 0; i--) {
            if (j == 0)
                break;
            if (mascara_utilizar.substr(i, 1) == caracter) {
                j--;
            }
            masct = mascara_utilizar.substr(i, 1) + masct;
        }
        mascara_utilizar = masct;
    }

    //mascarar
    j = mascara_utilizar.length - 1;
    for (i = valor.length - 1; i >= 0; i--) {
        if (mascara_utilizar.substr(j, 1) != caracter) {
            ret = mascara_utilizar.substr(j, 1) + ret;
            j--;
        }
        ret = valor.substr(i, 1) + ret;
        j--;
    }
    return ret;
}

function dataBancoFormata(data) {
    if (data) {
        var dtH = (data).split(" ");
        var dataf = (dtH[0]).split("-");
        var data = dataf[2] + "/" + dataf[1] + "/" + dataf[0];
    }
    return data;
}