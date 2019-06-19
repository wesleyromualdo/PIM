<script type="text/javascript">
$(document).ready(function(){

  $('.pesquisar').click(function(){
    var op = $('#op').val();
                $('#op').val('pesquisar');
                $('#xls').val('0');
                $('#xlsparalizada').val('0');
    $('#formListaObra').submit();
                $('#op').val(op);
  });

  $('.exportarxls').click(function(){
    $('#xls').val('1');
        $('#xlsparalizada').val('0');
    $('#formListaObra').submit();
  });
    $('.ver-maps').click(function(){
        $('#mapa').val('1');
        $('#formListaObra').submit();
  });
    $('.exportarxlsparalizadas').click(function(){
    $('#xls').val('1');
                $('#xlsparalizada').val('1');
    $('#formListaObra').submit();
  });

});


function exibeBuscaAvancada(visivel){
	if ( visivel == true ){
		$('.tr_busca_avancada').fadeIn(800);
		$('#labelBuscaAvancada').fadeOut(0);
		$('#abreBuscaAvancada').val('t');
	}else{
		$('.tr_busca_avancada').fadeOut(0);
		$('#labelBuscaAvancada').fadeIn(0);
		$('#abreBuscaAvancada').val('f');
	}
}

function carregarMunicipio( estuf ){
        var td	= $('#td_municipio');
        if ( estuf != '' ){
                var url = location.href;
                $.ajax({
                        url  		 : url,
                        type 		 : 'post',
                        data 		 : {ajax  : 'municipio',
                                                  estuf : estuf},
                        dataType   : "html",
                        async		 : false,
                        beforeSend : function (){
                              divCarregando();
                              td.find('select option:first').attr('selected', true);
                        },
                        error 	 : function (){
                              divCarregado();
                        },
                        success	 : function ( data ){
                              td.html( data );
                              divCarregado();
                        }
              });
        }else{
                td.find('select option:first').attr('selected', true);
                td.find('select').attr('selected', true)
                                                 .attr('disabled', true);
        }
}

function alterarEmp( empid ){
	location.href = '?modulo=principal/cadEmpreendimento&acao=A&empid=' + empid;
}

function abreVistoriaEmpresa( empid, sosid, sueid ){
	location.href = '?modulo=principal/listaEmpreendimentoEmpresa&acao=A&op=abreVistoria&empid=' + empid + '&sosid=' + sosid + '&sueid=' + sueid;
}

function abreObraEmpresa( obrid){
	window.open('?modulo=principal/cadObra&acao=A&visualizar=1&obrid='+obrid,
			'ObraSupervisaoDetalhe',
			"height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
}

function abreEmpreendimentoEmpresa( empid ){
	window.open('?modulo=principal/listaObrasEmpreendimento&acao=A&empid='+empid,
			'ObraSupervisaoDetalhe',
			"height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
}

function imprimirLaudo( sueid , empid ){
	return windowOpen( '?modulo=principal/popupImpressaoLaudo&acao=A&sueid=' + sueid + '&empid=' + empid,'blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function imprimirQuestionarioRespondido(sueid, sosid){
	return windowOpen( '?modulo=principal/cadVistoriaEmpresaImpressaoPreenchido&acao=A&sueid='+sueid+'&sosid='+sosid,'blank',
			   'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}


function abrirConsultaEmpresa( empid ){
	return windowOpen( '?modulo=principal/selecionaObjeto&acao=V&empid=' + empid, 'blank',
			   'height=900,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

</script>