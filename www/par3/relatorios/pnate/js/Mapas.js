/**
 * Objeto para lidar com mapas com Gmaps e Google Maps
 * 
 * @author Sï¿½vio Resende <savio@savioresende.com.br>
 * @link http://hpneo.github.io/gmaps Documentation of Gmaps
 */
var Mapas = {

	versao: 'beta',
	poligonos: [],
	fundoBrasil: '',
	idTag: '',
	eTeste: false, // encapsulamento da requisicao ajax para teste jasmine
	poligonosDesenhados: [],
	posLegenda: '', // acao a ser executada apos a aplicacao da legenda
	posDesenhoPoligono: '', // acao a ser executada apos o desenho do poligono
	estilo: 'default',
	orgid: '',
	popup: null,
	poligonosCentro: [],
	marcadores: [],
	casa: 0,

	// centros dos estados para as siglas (nï¿½o ï¿½ o centro real)
	centroEstados: [
		[ "AC", -11.615191, -70.726764 ],
		[ "AL", -11.458018, -36.478033 ],
		[ "AP", 1.311648, -53.146021 ],
		[ "AM", -4.070592, -65.601665 ],
		[ "BA", -13.725678, -42.310651 ],
		[ "CE", -5.967653, -40.387462 ],
		[ "DF", -16.342755, -49.362535 ],
		[ "ES", -21.396513, -40.909884 ],
		[ "GO", -17.710158, -51.289962 ],
		[ "MA", -5.618566, -46.027267 ],
		[ "MT", -12.256661, -57.013594 ],
		[ "MS", -21.893160, -55.651290 ],
		[ "MG", -19.939395, -46.203048 ],
		[ "PR", -26.209254, -52.443282 ],
		[ "PB", -8.353385, -37.564680 ],
		[ "PA", -5.924623, -52.772872 ],
		[ "PE", -9.670559, -37.039962 ],
		[ "PI", -7.582897, -43.401534 ],
		[ "RJ", -24.496119, -43.461959 ],
		[ "RN", -6.619917, -36.794655 ],
		[ "RS", -30.470374, -54.874377 ],
		[ "RO", -11.665184, -65.119690 ],
		[ "RR", 1.838791, -63.260181 ],
		[ "SC", -28.590937, -51.576237 ],
		[ "SE", -12.687295, -37.875421 ],
		[ "SP", -23.927472, -49.663507 ],
		[ "TO", -11.755162, -50.060382 ]
	],
	buscaMunicipioSuporte: [],

	/**
	 * Mï¿½todo que tem o objetivo de mostrar o caminho 
	 * sequencial da execuï¿½ï¿½o do JS, para que se possa
	 * debugar melhor. O resultado ï¿½ apresentado no console.
	 */
	mostraPosicao: function( parte ){
	},

	/**
	 * Inicializa mapa
	 *
	 * @param idTag - id do tag html
	 */
	inicializar: function( idTag ){
		this.mostraPosicao( 'inicializar' );

		var that = this;
		this.idTag = idTag;

		switch( this.estilo ) {
			case 'externo_blank':

				var maxZoomValue = that.trataZoom();
				var zoomInicio = that.trataZoomInicial();

			    this.map = new GMaps({
					div: idTag,
					mapTypeId: google.maps.MapTypeId.SATELLITE,
					lat: -14.689881, 
					lng: -52.373047,
					zoom: zoomInicio,
					zoomControl: true
				});
			    break;
			case 'externo_blank_pais':

				this.map = new GMaps({
					div: idTag,
					mapTypeId: google.maps.MapTypeId.SATELLITE,
					lat: -15.389881, 
					lng: -54.373047,
					zoom: 4,
					zoomControl: true,
					scrollwheel: false,
					draggable: false,
					disableDoubleClickZoom: true,
					disableDefaultUI: true
				});
				setTimeout(function(){
					that.aplicaTituloEstados();
				},500);
				break;
			default:
				this.map = new GMaps({
					div: idTag,
					lat: -15.689881, 
					lng: -52.373047,
					zoom: 4,
					mapTypeId: google.maps.MapTypeId.ROADMAP
				});
		}

		this.poligonosDesenhados = [];
	},

	/**
	 * Tratamento de Zoom de acordo com o estado
	 */
	trataZoom: function(){
		this.mostraPosicao( 'trataZoom' );

		switch( jQuery('#uf').val() ){
			case 'SE':
			case 'AL':
			case 'PE':
			case 'PB':
			case 'RN':
			case 'SP':
				return 9;
				break;
			case 'BA':
			case 'MA':
			case 'MG':
			case 'MS':
			case 'PA':
			case 'PI':
				return 8;
				break;
			case 'AM':
			case 'MT':
			case 'TO':
				return 7;
				break;
			default:
				return 8
				break;
		}
	},

	/**
	 * Tratamento de Zoom de acordo com o estado
	 */
	trataZoomInicial: function(){
		this.mostraPosicao( 'trataZoomInicial' );

		switch( jQuery('#uf').val() ){
			case 'SE':
			case 'AL':
			case 'PE':
			case 'PB':
			case 'RN':
			case 'SP':
				return 9;
				break;
			case 'BA':
			case 'MA':
			case 'MG':
			case 'MS':
			case 'PA':
			case 'PI':
				return 8;
				break;
			case 'AM':
			case 'MT':
			case 'TO':
				return 7;
				break;
			case 'RR':
				return 6;
				break;
			default:
				return 8
				break;
		}
	},


	/**
	 * Busca gerada por um form de multiselect para mostrar no mapa multiplos estados
	 *
	 * @param estuf - id do tag html
	 */
	buscaEstadoForm: function( estuf, origemRequisicao ){
		this.mostraPosicao( 'buscaEstadoForm' );

        var load = false;
        if (origemRequisicao.indexOf('externo') == -1){
            load = false;
        } else {
            load = true;
        }

		var selecionados = jQuery(estuf).val();
		this.origemRequisicao = origemRequisicao;
		//if( selecionados == null ){
		//	this.poligonos.poligono = '';
		//	this.atualizaMapa();
		//}else{
			var requisicao = {
				estado: selecionados,
				municipio: false,
                load: load
			};

			this.getEstadoPoligono( requisicao );
		//}
	},

	/**
	 * Resgata poligono de estados passados
	 *
	 * Formato aceito para @param chamada:
	 *     var chamada = { estado: [], municipio: [] };
	 *	ou
	 *	   var chamada = { false, municipio: [] };
	 * obs.: lembrando que "[]" ï¿½ array js, e "{}" ï¿½ objeto.
	 */
	getEstadoPoligono: function( chamada ){
		this.mostraPosicao( 'getEstadoPoligono' );

		var that = this;
		this.chamada = chamada;

		var datad = new Date();
		var nomeTemporario = datad.getDay() + '' + datad.getMonth() + '' + datad.getYear() + '' + datad.getHours() + '' + datad.getMinutes() + '' + datad.getSeconds() + '' + datad.getMilliseconds();
		jQuery( this.idTag+'txt' ).append( "<div style='position:absolute;line-height:34px;z-index:8;' id='"+nomeTemporario+"'><img style='width:20px;margin-right:5px;' src='../imagens/carregando.gif'/>Carregando...</div>" );

		// encapsulamento da requisicao ajax para teste jasmine
		if( this.eTeste == true ){
			this.poligonos = {cor:'#BBD8E9',poligono:this.requisicao};
		}else{
			var origensDasRequisicoes = this.origensDasRequisicoes();
            if (chamada['load']) {
                $.blockUI({
                    message: $('#loading'),
                    css: {
                        backgroundColor: 'transparent',
                        border: 'none',
                        color: '#fff'
                    }
                });
            }

			jQuery.ajax({
				url: origensDasRequisicoes.url,
				type: 'POST',
				data: {chamadoMapas:'montaEstado',params:chamada,retorno:'EstadosSelecionados',origemRequisicao:this.origemRequisicao},
				success: function( resposta ){
					jQuery( '#'+nomeTemporario ).remove();
                    var a = resposta;
					that.requisicao = resposta;
					that.poligonos = {cor:'#FFF',poligono:a};
					that.desenhaPoligonos();
					that.atualizaPosicaoLegenda();
                    if (chamada['load']) {
                        $.unblockUI();
                    }
				}
			});
		}

	},

	/**
	 * Atualiza mapa com as configuraï¿½ï¿½es aplicadas
	 */
	atualizaMapa: function(){

		this.mostraPosicao( 'atualizaMapa' );
		this.desenhaPoligonos();
	},

	desenhaPoligonos: function(){
		this.mostraPosicao( 'desenhaPoligonos' );

		var that = this;
		var bordaTexto = 'text-shadow: 1px 0 0 #000, -1px 0 0 #000, 0 1px 0 #000, 0 -1px 0 #000, 1px 1px #000, -1px -1px 0 #000, 1px -1px 0 #000, -1px 1px 0 #000;';

		if( this.poligonos.poligono != "" ){
			var poli = JSON.parse(this.poligonos.poligono);
		}

		// retira todos os poligonos do mapa e destrï¿½i os mesmos
		var i;

		for (i in that.poligonosDesenhados){
			//that.poligonosDesenhados[i].setMap( null );
			 that.poligonosDesenhados[i] = undefined;
		}

		if(this.origemRequisicao == 'obras-fnde'){
			this.aplicaFundoBrasil();
		}

		if( this.poligonos.poligono != "" ){
            var gEstuf, acaoMapa;

			jQuery.each( poli, function( index, value ){

				if( value == null ){
					return;
				}
				
				if( value.cor ){ 
					var cor = value.cor; 
				}else{ 
					var cor = that.poligonos.cor;
				}
				
                cor = '#3A8D42';

				var im;
				for (im in value.poli.coordinates) { // LOOP MULTIPOLIGONO
					if( typeof value.poli.coordinates[im][0] != 'undefined' ) {
                        var i;
                        for (var i in value.poli.coordinates[im][0]) {
                            value.poli.coordinates[im][0][i] = new google.maps.LatLng(value.poli.coordinates[im][0][i][1], value.poli.coordinates[im][0][i][0]);
                        }



                        that.poligonosDesenhados[that.casa] = new google.maps.Polygon({
                            mundescricao: value.mundescricao,
                            estdescricao: value.estdescricao,
                            muncod: value.muncod,
                            estuf: value.estuf,
                            cor: cor,
                            situacao: value.situacao,
                            paths: value.poli.coordinates[im][0],
                            useGeoJSON: true,
                            strokeColor: '#273A4A',
                            strokeColor: '#FFF',
                            strokeOpacity: 1,
                            strokeWeight: 1,
                            fillColor: cor,
                            // fillOpacity: 0.6,
                            fillOpacity: 1,
                            zIndex: 6,
                            map: that.map.map
                        });


                        infowindow = new google.maps.InfoWindow();
                        google.maps.event.addListener(that.poligonosDesenhados[that.casa], 'click', function (event) {
                            abrirObras(this.estuf);
                        });
                        google.maps.event.addListener(that.poligonosDesenhados[that.casa], 'mouseover', function () {
                            var corAtual = this.fillColor;
                            this.setOptions({fillColorBkp: corAtual});
                            this.setOptions({fillColor: "#4eba58"});
                            //this.setOptions({fillColor: "#8ce394"});
                            that.clearTextArea();
                        });
                        google.maps.event.addListener(that.poligonosDesenhados[that.casa], 'mouseout', function () {
                            var corAtual = this.fillColorBkp;
                            this.setOptions({fillColor: corAtual});
                            that.clearTextArea();
                        });

                        that.casa++;
                    }

				} // LOOP MULTIPOLIGONO /

			});
		}

		this.posDesenhoPoligonoAcao();
	},

	/**
	 * Atualiza com chamada a mostragem atual
	 */
	posDesenhoPoligonoAcao: function(){
		this.mostraPosicao( 'posDesenhoPoligono' );

		if( this.posDesenhoPoligono != '' ){
			eval(''+this.posDesenhoPoligono+'()');
		}
	},

	/**
	 * Atualiza com chamada a mostragem atual
	 */
	sincronizaMapaComServidor: function(){
		this.mostraPosicao( 'sincronizaMapaComServidor' );

		this.getEstadoPoligono( this.chamada );
	},

	/**
	 * Determina informaï¿½ï¿½es necessï¿½rias para atender as diferentes origens das requisicoes
	 */
	 origensDasRequisicoes: function(){
		this.mostraPosicao( 'origensDasRequisicoes' );

	 	var that = this;

	 	switch( this.origemRequisicao ){

            case 'obras-fnde':
                return {url:'index.php'};
                break;

	 		// ---------------------------------------- buscas padrao
	 		case 'assessoramento-estado': 
	 		case 'assessoramento':
	 		case 'assessoramento-legenda-estado':
	 		case 'assessoramento-legenda':
	 			return {url:'sase.php?modulo=principal/assessoramento&acao=A'};
	 			break;
	 		case 'questoespontuais':
	 		case 'questoespontuais-legenda':
	 			return {url:'sase.php?modulo=principal/questoespontuais&acao=A'};
	 			break;
	 		case 'organizacoesterritoriais':
	 		case 'organizacoesterritoriais-legenda':
	 			that.orgid = jQuery('#orgao').val();
	 			return {url:'sase.php?modulo=principal/organizacoesterritoriais&acao=A'};
	 			break;
            case 'planocarreira-municipio':
            case 'planocarreira-municipio-legenda':
                return {url: 'sase.php?modulo=principal/planodecarreira&acao=A'};
                break;
            case 'planocarreira-estado':
            case 'planocarreira-estado-legenda':
                return {url: 'sase.php?modulo=principal/planodecarreiraEstado&acao=A'};
                break;
            case 'acompanhamento-pne-municipio':
                return {url: 'sase.php?modulo=principal/acompanhamentopne/municipio&acao=A'};
                break;
            case 'acompanhamento-pne-estado':
                return {url: 'sase.php?modulo=principal/acompanhamentopne/estado&acao=A'};
                break;
	 		// ---------------------------------------- buscas sase_mapas
	 		case 'assessoramento-externo':
	 		case 'assessoramento-legenda-externo':
	 		case 'questoespontuais-externo':
	 		case 'questoespontuais-legenda-externo':
	 		case 'organizacoesterritoriais-externo':
	 			return {url:'sase_mapas.php'};
	 			break;
	 		// ---------------------------------------- buscas sase_mapas
	 		case 'pais-externo':
	 		case 'pais-legenda-externo':
            case 'municipio-externo':
            case 'municipio-legenda-externo':
	 			return {url:'sase_pais.php'};
	 			break;
            // ---------------------------------------- buscas sase_mapa_planodecarreira
            case 'planocarreira-estado-externo':
            case 'planocarreira-estado-legenda-externo':
                return {url:'sase_mapa_planodecarreira.php'};
                break;
	 	}
	},

	/**
	  * Atualiza as legendas apresentadas para o que ï¿½ apresentado no mapa apenas com suas quantidades
	  */
	atualizaLegenda: function( estuf, origemRequisicao ){
		this.mostraPosicao( 'atualizaLegenda' );

		var that = this;
		var selecionados = jQuery(estuf).val();

		if(typeof selecionados === 'string'){ selecionados = [selecionados]; }

		this.origemRequisicao = origemRequisicao;

		this.chamada = {
			estados: selecionados
		};

        if(origemRequisicao == 'planocarreira-municipio-legenda'
            || origemRequisicao == 'planocarreira-estado-legenda'
            || origemRequisicao == 'planocarreira-estado-legenda-externo'){
            this.chamada.tpdid = jQuery('#tpdid').val();
        }

		if( this.orgid != '' ){ this.chamada.orgid = this.orgid; }

		var origensDasRequisicoes = this.origensDasRequisicoes();
		jQuery.ajax({
			url: origensDasRequisicoes.url,
			type: 'POST',
			data: {chamadoMapas:'legenda',params:this.chamada,retorno:'legenda',origemRequisicao:this.origemRequisicao},
			success: function( resposta ){
				jQuery('#legendaMapa').html( resposta );
				that.acaoPosAplicacaoLegenda();
			}
		});
	},

	atualizaPosicaoLegenda: function(){
		this.mostraPosicao( 'atualizaPosicaoLegenda' );

		switch( this.origemRequisicao ){
	 		case 'assessoramento-externo':
	 		case 'assessoramento-legenda-externo':
	 		case 'questoespontuais-externo':
	 		case 'questoespontuais-legenda-externo':
	 		case 'organizacoesterritoriais-externo':
            case 'planocarreira-municipio':
            case 'planocarreira-estado':
	 			jQuery('#legendaMapa').appendTo( jQuery('#map_canvas') );
	 			break;
	 	}
	 },

	clearTextArea: function(){
		this.mostraPosicao( 'clearTextArea' );

		jQuery( '.txts' ).remove();
	},

	acaoPosAplicacaoLegenda: function(){
		this.mostraPosicao( 'acaoPosAplicacaoLegenda' );

		if( this.posLegenda != '' ){
			eval(''+this.posLegenda+'()');
		}
	},

	verificaSeCentroPermaneceNoBound: function(){
		this.mostraPosicao( 'verificaSeCentroPermaneceNoBound' );

		var that = this;
		var map = this.map.map;

		that.allowedBounds = map.getBounds();
		google.maps.event.addListenerOnce(map,'idle',function() {
			// var zoomAtual = map.getZoom();

			// map.setZoom( zoomAtual + 1 );

			that.allowedBounds = map.getBounds();

			// map.setZoom( zoomAtual );
		});

		google.maps.event.addListener(map,'center_changed',function() { 
			checkBounds( that.allowedBounds ); 
		});

		function checkBounds() {    
		    if(! that.allowedBounds.contains(map.getCenter())) {
		      var C = map.getCenter();
		      var X = C.lng();
		      var Y = C.lat();

		      var AmaxX = that.allowedBounds.getNorthEast().lng();
		      var AmaxY = that.allowedBounds.getNorthEast().lat();
		      var AminX = that.allowedBounds.getSouthWest().lng();
		      var AminY = that.allowedBounds.getSouthWest().lat();

		      if (X < AminX) {X = AminX;}
		      if (X > AmaxX) {X = AmaxX;}
		      if (Y < AminY) {Y = AminY;}
		      if (Y > AmaxY) {Y = AmaxY;}

		      map.setCenter(new google.maps.LatLng(Y,X));
		    }
		}
	},

	/**
	 * Aplica imagens para Siglas dos Estados
	 */
	aplicaTituloEstados: function(){
		this.mostraPosicao( 'aplicaTituloEstados' );

		setMarkers(this.map.map, this.centroEstados);

		function setMarkers(map, locations) {
		  var shape = {
		      coords: [1, 1, 1, 20, 18, 20, 18 , 1],
		      type: 'poly'
		  };
		  for (var i = 0; i < locations.length; i++) {
		    var uf = locations[i];
		    var myLatLng = new google.maps.LatLng(uf[1], uf[2]);
		    var marker = new google.maps.Marker({
		        position: myLatLng,
		        map: map,
		        icon: {
		        	url: 'texto_nome_entidade_territorio.php?estuf='+uf[0],
		        	size: new google.maps.Size(60, 32),
				    origin: new google.maps.Point(0,0),
				    anchor: new google.maps.Point(0, 32)
		        },
		        shape: shape,
		        title: uf[0],
		        zIndex: 4
		    });
		  }
		}

	},

	/**
	 * Aplica imagem para fundo 
	 */
	aplicaFundoBrasil: function(){
		this.mostraPosicao( 'aplicaFundoBrasil' );

		var that = this;

		var path = [
			new google.maps.LatLng(8.183668,-75.689105), // nordeste 
			new google.maps.LatLng(-38.655990,-75.689105), // sudeste
			new google.maps.LatLng(-38.655990,-32.534811), // sudoeste
			new google.maps.LatLng(8.183668,-32.534811)  // noroeste
		];
		
		this.fundoBrasil = new google.maps.Polygon({
			paths: path,
			strokeColor: 'transparent',
			strokeOpacity: 1,
			strokeWeight: 0,
			fillColor: 'transparent',
			fillOpacity: 1,
			map: that.map.map
		});

	},

	aplicaComponente: function( estuf, componente, origemRequisicao){
		this.mostraPosicao( 'aplicaComponente' );

		var that = this;
		this.origemRequisicao = origemRequisicao;

		estuf = jQuery(estuf).val();

		var origensDasRequisicoes = that.origensDasRequisicoes();
		jQuery.ajax({
			url: origensDasRequisicoes.url,
			type: 'POST',
			data: {chamadoMapas:componente,params:estuf,origemRequisicao:this.origemRequisicao},
			success: function( resposta ){
				jQuery('#componentesMapa').append( resposta );
			}
		});

	},

	/**
	 * mï¿½todo para atender componente de busca de municipios
	 */
	buscaMunicipio: function( muncod ){
		this.mostraPosicao( 'buscaMunicipio' );

		var chave = null;
		jQuery.each(this.poligonosDesenhados, function( index, value ){
			if( value.muncod == muncod ){
				chave = index;
			}
		});

		var that = this;

		var contornoInicial = this.poligonosDesenhados[ chave ].strokeWeight;
		if( this.buscaMunicipioSuporte[chave] === undefined ){
			this.buscaMunicipioSuporte.push( [chave,contornoInicial] );
		}
		var atualizaContornosAnteriores = function(){
			jQuery.each( that.buscaMunicipioSuporte, function( index, value){
				that.poligonosDesenhados[ value[0] ].setOptions({strokeWeight: value[1]});
			} );
		}
		atualizaContornosAnteriores();

		var contornoFinal = 4;
		this.poligonosDesenhados[ chave ].setOptions({strokeWeight: contornoFinal});


		// ################################################################## POSICAO DO MAPA
		var polMunicipio = JSON.parse(this.poligonos.poligono);
		
		// that.map.removeMarkers();
		var removeMarcadores = function(){
			for (var i = that.marcadores.length - 1; i >= 0; i--) {
				that.marcadores[i].setMap(null);
			};
		}
		removeMarcadores();
		
		jQuery.each(polMunicipio, function( key, value ){
			if( key != 'boundsSimplificado' ){
				if( value.muncod == muncod ){
					
					var poligono = value.poli.coordinates[0][0];
					var boundbox = new google.maps.LatLngBounds();
					for ( var i = 0; i < poligono.length; i++ ){
						boundbox.extend(new google.maps.LatLng(poligono[i][1],poligono[i][0]));
						
						if( i == poligono.length-1 ){
							that.map.map.setCenter(boundbox.getCenter());
							that.map.map.fitBounds(boundbox);

							var centro = boundbox.getCenter();
							that.marcadores.push( new google.maps.Marker({
								position: new google.maps.LatLng( centro.lat(), centro.lng() ),
								map: that.map.map,
								icon: '../imagens/sase/direction.png'
							}) );
						}
					}

				}
			}
		});
	}


};



