
// Essa funcao estÃ¡ aqui porque nÃ£o precisa ser duplicada uma vez que
// faz a mesma coisa em todos os lugares em que for chamada
function abreListaSupervisaoFnde( obrid, empid ){
	// window.location.href = 'obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A&obrid='+obrid;
	$('[name=req]').val( 'supervisorFNDE' );
	$('[name=obrid]').val( obrid );
	$('[name=empid]').val( empid );


	$('#formListaObra').submit();
}


function validarPercentual( valor ){
    var inicio = new Number( document.getElementById( 'percentualinicial' ).value );
    var fim    = new Number( document.getElementById( 'percentualfinal' ).value );

    if ( inicio > fim ){
        alert('O valor percentual mínimo é maior do que o máximo');
        if ( fim > 5 ){
            document.getElementById( 'percentualinicial' ).value = fim - 5;
        }else{
            document.getElementById( 'percentualinicial' ).value = 0;
        }
    }
}

/**
 * Função responsável por calcular e retornar a diferença entre duas datas em dias.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @param data1 Data no formato dd/mm/aaaa.
 * @param data2 Data no formato dd/mm/aaaa.
 * @returns {*}
 */
function calcularDiferencaEntreDatas(data1, data2) {

    if (typeof(data1) !== "undefined" && typeof(data2) !== "undefined") {

        if (typeof(data1) === "string" && typeof(data2) === "string") {

            arrData1 = data1.split("\/");
            data1 = arrData1[1] + "/" + arrData1[0] + "/" + arrData1[2];

            arrData2 = data2.split("\/");
            data2 = arrData2[1] + "/" + arrData2[0] + "/" + arrData2[2];

            var dt1 = new Date(data1);
            var dt2 = new Date(data2);

            //var diferenca = Math.abs(dt2.getTime() - dt1.getTime());
            var diferenca = (dt2.getTime() - dt1.getTime());
            var dias = Math.ceil(diferenca / (1000 * 3600 * 24));

            return dias;

        } else {
            return false;
        }

    } else {
        return false;
    }
}

/**
 * Função responsável por converter um número para o formato da moeda brasileira.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @param valor Valor no tipo FLOAT.
 * @returns {string}
 */
function formatarValorMonetario(valor) {
    valor = parseFloat(valor);
    if (isNaN(valor)) {
        return "Erro";
    } else {
        return valor
            .toFixed(2) // O número terá sempre 2 dígitos decimais.
            .replace(".", ",") // Substitui o ponto pela vírgula.
            .replace(/(\d)(?=(\d{3})+(?!\d))/g, "$1."); // Usando o ponto como separador de milhar.
    }
}
