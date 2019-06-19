var sql = '';
var sqlDetalhe = '';
/**
 * Definição do Relatório de Escola
 */
var RelatorioEscola = {
    /**
     * Número da última página requisitada
     * @type Number
     */
    pagina: null,
    /**
     * Dados serializados do filtro a ser executado
     * @type String
     */
    filtro: null,
    /**
     * Registro de exemplo que serve como base para extração de dados da tela
     * @type Object
     */
    registro: null,
    /**
     * Número total de registros encontrados com o filtro executado
     * @type Number
     */
    totalRegistros: null,
    sql: null,
    /**
     * Método inicializador da tela
     */
    iniciar: function () {
        this.popularDadosIniciais();
        this.registrarEventos();
        this.atualizarColunasRelatorio($());
        this.atualizarResponsabilidade();
    },
    popularDadosIniciais: function () {
        $('input[type="radio"][value="3"]').trigger('click');
    },
    /**
     * Eventos existentes na tela
     */
    registrarEventos: function () {
        /**
         * Indice com os eventos que serão inicializados
         */
        indiceDeEventos = function () {
            disparoDeBuscaAutomatica();
            disparoDeColunas();
            disparoDaPesquisa();
            disparoDeVisualizarRegistro();
            disparoDeVisualizarMapa();
            disparoComSaidaParaExcel();
            disparoComSaidaParaJson();
            disparoSeletoresDeEscopoDoRegistro();
            disparoDeBuscaNoResultado();
            disparoDeSelecaoDaUF();
            disparoDeClickNoMunicipio();
            disparoDeFocoNaDescricaoDoMunicipio();
            disparoDeMaisRegistros();
            disparoDeFiltroNaEscola();
            disparoMostrarEsconderSql();
            disparoMostrarEsconderSqlDetalhe();
            disparoMostrarEsconderInputPlanejamento();
            disparoAlternarEsferas();
        };
        /**
         * Dispara a busca automática com a rolagem de tela
         */
        disparoDeBuscaAutomatica = function () {
            $(window).scroll(function () {
                if (RelatorioEscola.pagina) {
                    if ($('.js-busca-no-resultado').val()) {
                        return;
                    }

                    var device = detectZoom.device();
                    if (device <= 0.999 || device >= 1.001) {
                        $('.js-mais-registros').show();
                    } else {
                        $('.js-mais-registros').hide();
                        if ($(window).scrollTop() >= $(document).height() - $(window).height()) {
                            RelatorioEscola.pagina++;
                            RelatorioEscola.pesquisarEscolas();
                        }
                    }
                }
            });
        };
        /**
         * Dispara a busca de mais escolas
         */
        disparoDeMaisRegistros = function () {
            $('.js-mais-registros').on('click', function () {
                RelatorioEscola.pagina++;
                RelatorioEscola.pesquisarEscolas();
            });
        };
        /**
         * Dispara a busca nos resultados visíveis da tela
         */
        disparoDeBuscaNoResultado = function () {
            $('.js-busca-no-resultado').on('change', function () {
                var val = $(this).val();
                $('.listagem tr')[val ? 'hide' : 'show' ]();
                $('.listagem td.matched').removeClass('matched');
                $('.listagem').parents('table:first').addClass('table-striped')
                if (val) {
                    $('.listagem').parents('table:first').removeClass('table-striped')
                    var re = new RegExp(val, 'i');
                    $.each($('.listagem td').filter(function () {
                        var res = $(this).css('display') != 'none' && re.test($(this).text());
                        if (res) {
                            $(this).addClass('matched');
                        }
                        return res;
                    }), function () {
                        $(this).parent().show();
                    });
                }
            });
        };
        /**
         * Dispara a apresentação de colunas
         */
        disparoDeColunas = function () {
            $('.js-seletor-colunas').on('change', function () {
                RelatorioEscola.atualizarColunasRelatorio($(this).find(':selected'));
            });
        };
        /**
         * Dispara a pesquisa direta ao enviar o formulário
         */
        disparoDaPesquisa = function () {
            $('form[name="pesquisa"]').on('submit', function (event) {
                if ($('input[name="tipoSaida"]').val() == 'json') {
                    $('.listagem').html('');
                    RelatorioEscola.filtro = $('form[name="pesquisa"]').serialize();
                    RelatorioEscola.pagina = 1;
                    RelatorioEscola.pesquisarEscolas();
                    event.preventDefault();
                    event.stopPropagation();
                }
            });
        };
        /**
         * Dispara a apresentação do modal com os detalhes do registro
         */
        disparoDeVisualizarRegistro = function () {
            $('.js-visualizar-registro').on('click', function () {
                RelatorioEscola.visualizarDadosDaEscola($(this)
                        .parents('.item-listagem:first')
                        .view('extract', RelatorioEscola.registro));
            });

        };
        /**
         * Dispara a apresentação do mapa nos detalhes do registro
         */
        disparoDeVisualizarMapa = function () {
            $('.js-visualizar-mapa').on('click', function () {
                var coordenadas = $(this).data('coordenadas').split(' / ');
                $("#map_canvas").toggle();
                setTimeout(function () {
                    RelatorioEscola.apresentarMapa(coordenadas[1], coordenadas[0]);
                }, 10);
            });
        };
        /**
         * Dispara a busca com saída para excel
         */
        disparoComSaidaParaExcel = function () {
            $(".xls").on("click", function () {
                $('input[name="tipoSaida"]').val('excel');
                $("#formulario").submit();
            });
        };
        /**
         * Dispara a busca com saída para json
         */
        disparoComSaidaParaJson = function () {
            $(".buscar").on("click", function () {
                $('input[name="tipoSaida"]').val('json');
                $("#formulario").submit();
            });
        };
        /**
         * Dispara os eventos de mudança de escopo do registro
         */
        disparoSeletoresDeEscopoDoRegistro = function () {
            $('.js-nav-block').on('click', function () {
                $($(this).data('scope')).hide();
                $($(this).data('target')).show();
                $(this).parent().children().removeClass('active');
                $(this).addClass('active');
            }).first().click();

        };
        /**
         * Dispara os eventos ao selecionar uma Unidade Federativa
         */
        disparoDeSelecaoDaUF = function () {
            $('.js-uf').change(function () {
                if (!RelatorioEscola.responsabilidadeMunicipal()) {
                    if ($(".js-seletor-esfera").val() == 1) {
                        // console.log($(".js-seletor-esfera").val());
                        return;
                    } else {
                        if (!Array.isArray($(this).val())) {
                            RelatorioEscola.listarMunicipios();
                        } else {
                            RelatorioEscola.carregarMunicipioAnalise($('.js-uf').val(), $('.js-municipio').val());
                        }
                    }
                }
            });
        };
        /**
         * Dispara os eventos no click do input do municipio
         */
        disparoDeClickNoMunicipio = function () {
            $('.js-linha-municipio').one('click', function () {
                if (!RelatorioEscola.responsabilidadeMunicipal()) {
                    if (Array.isArray($('.js-uf').val())) {
                        return;
                    }
                    RelatorioEscola.listarMunicipios();
                }
            });
        };
        /**
         * Dispara os eventos no foco do municipio
         */
        disparoDeFocoNaDescricaoDoMunicipio = function () {
            $('.js-municipio').on('focus', function () {
                if (!RelatorioEscola.responsabilidadeMunicipal()) {
                    if (Array.isArray($('.js-uf').val())) {
                        return;
                    }
                    RelatorioEscola.listarMunicipios();
                }
            });
        };
        disparoDeFiltroNaEscola = function () {
            $.each(['item','processo','termo', 'situacaoPlanejamento'],function(i,campo){
                $('.js-escola-' + campo).on('change', function () {
                    RelatorioEscola.filtrarItensDestinados();
                });
            });
        };

        disparoMostrarEsconderSql = function() {
            $('#btn-sql').on('click',function() {
                $('#panel-sql').slideToggle();
            });
        };

        disparoMostrarEsconderSqlDetalhe = function() {
            $('#btn-detalhe-sql').on('click',function() {
                $('#panel-detalhe-sql').slideToggle();
            });
        };

        disparoMostrarEsconderInputPlanejamento = function() {
            $("[name='sfiltro[planejamento]']").on('change',function() {
                if($(this).val() == 1) {
                    $('.form-inpid').show();
                }else {
                    $('.form-inpid').hide();
                }
            });
        };

        disparoAlternarEsferas = function() {
            $("[name='sfiltro[itrid]']").on('change',function() {
                if($(this).val() == 1) {
                    $('.js-linha-municipio').hide();
                }else {
                    $('.js-linha-municipio').show();
                }
            });
        };

        // Executa a inicialização dos eventos
        indiceDeEventos();
    },
    /**
     * Preenche os dados da listagem das escolas
     * @param {Array} dados
     */
    preencherDadosListagem: function (dados) {
        if (dados.length) {
            $('.js-resultado-pesquisa').show();
            $('.listagem').view('iterate', {
                'template': $('.templates .item-listagem'),
                'collection': dados,
                'callbackItem': function ($item, dados) {
                }
            });
        } else {
        }
    },
    /**
     * Preenche painel de visualização do SQL da pesquisa
     * @param {String} sql
     */
    preencherPainelSql: function (sql) {
        if (sql) {
            $('#panel-sql').html(sql);
        } else {
        }
    },

    mostarBotaoSql: function () {
        $('#btn-sql').show();
    },

    mostarBotaoSqlDetalhe: function () {
        $('#btn-detalhe-sql').show();
    },

    /**
     * Preenche painel de visualização do SQL da pesquisa ao acessar o detalhamento da escola
     * @param {String} sql
     */
    preencherPainelSqlDetalhe: function (sql) {
        if (sql) {
            $('#panel-detalhe-sql').html(sql);
        } else {
        }
    },
    /**
     * Atualiza a tela de filtro de acordo com a responsabilidade do usuário
     */
    atualizarResponsabilidade: function () {
        setTimeout(function () {
            if (RelatorioEscola.responsabilidadeNacional()) {
                return;
            }
            if (RelatorioEscola.responsabilidadeMunicipal()) {
                $('.js-esfera').parents('form-group').hide();
                $('.js-municipio').next().hide();
                $.each(_JS.responsabilidade.municipios, function (i, municipio) {
                    var desc = municipio.mundescricao + ' - ' + municipio.estuf;
                    $('.js-municipio').next().after($('<span>').html(desc + ', '));
                    $('.js-municipio').append($('<option>').attr('selected', 'selected').val(municipio.muncod).html(desc));
                });
                $('input[name="sfiltro[itrid]"][value="2"]').attr('checked', 'checked').parents('.form-group:first').hide();
            }
            if (RelatorioEscola.responsabilidadeEstadual()) {
                $('.js-uf').html('');

                if(_JS.responsabilidade.estados.length == 1) {
                    var estado = _JS.responsabilidade.estados[0];
                    var uf     = $('.js-uf');
                    uf.append($('<option>').attr('selected', 'selected').val(estado.codigo).html(estado.descricao));
                    uf.prop('disabled',true);
                    uf.after(`<input type='hidden' name='sfiltro[estuf][]' value='${estado.codigo}' />`);
                    RelatorioEscola.carregarMunicipioAnalise(uf.val(), $('.js-municipio').val());
                }else{
                    $.each(_JS.responsabilidade.estados, function (i, estado) {
                        // $('.js-uf').next().after($('<span>').html(estado.descricao + ', '));
                        $('.js-uf').append($('<option>').attr('selected', 'selected').val(estado.codigo).html(estado.descricao));
                    });
                }
                // $('input[name="sfiltro[itrid]"][value="1"]').attr('checked', 'checked').parents('.form-group:first').hide();
                $('.js-municipio').parents('.form-group:first').show();
                $('.js-uf').parents('.form-group:first').show();
            }
            if (RelatorioEscola.responsabilidadeEstadual() && RelatorioEscola.responsabilidadeMunicipal()) {
                $('input[name="sfiltro[itrid]"][value="0"]').attr('checked', 'checked').parents('.form-group:first').show();
            }
        }, 100);
    },
    /**
     * Atualiza a tela com o total de registros encontrados para a pesquisa
     * @param {Object} resposta
     */
    atualizarTotalDeRegistros: function (resposta) {
        if (resposta.totalRegistros) {
            $('.js-resultado-pesquisa').assign(resposta);
            RelatorioEscola.totalRegistros = parseInt(resposta.totalRegistros);
        }
    },
    /**
     * Executa a requisição da pesquisa paginada de escolas 
     */
    pesquisarEscolas: function () {
        $.ajax({
            url: $('form[name="pesquisa"]').attr('action') + '&pagina=' + RelatorioEscola.pagina,
            data: RelatorioEscola.filtro,
            dataType: 'json',
            type: 'post'
        }).done(function (result) {
            if (result && !result.error) {
                if (result.dados.length) {
                    RelatorioEscola.registro = result.dados[0];
                }
                if(result.hasOwnProperty('sql')) {
                    RelatorioEscola.mostarBotaoSql();
                }
                if (result.sql) {
                    sql = result.sql;
                }
                RelatorioEscola.preencherPainelSql(sql);
                RelatorioEscola.atualizarTotalDeRegistros(result);
                RelatorioEscola.preencherDadosListagem(result.dados);
            }
        });
    },
    /**
     * Preenche a listagem de itens destinados na apresentação detalhada da escola
     * @param {Array} dados
     */
    preencherItensDestinados: function (dados) {
        // console.log(dados);
        if (dados.length) {
            $('.js-resultado-itens-destinados').show();
            $('.listagem-destinados').view('iterate', {
                'template': $('.templates .item-listagem-destinados'),
                'collection': dados,
                'callbackItem': function ($item, dados) {
                    $item.view('assign', {
                        'valor': Moeda.formatar(dados.valor),
                        'valorTotal': Moeda.formatar(dados.valorTotal),
                        'tipoassistencia': formatarAssistencia(dados.tipoassistencia,dados.anaid),
                    });
                },
                'cleanBeforeInit': true
            });
        }
    },
    /**
     * Preenche os combos da tela de filtro da escola selecionada
     * @param {Array} dados
     */
    preencherFiltroEscola: function (dados) {
        var reset = function(select){
            select.empty();
            select.trigger('chosen:updated');
            select.append(new Option('', ''));
            return select;
        };
        $.each(['item','processo','termo','situacaoPlanejamento'],function(i,campo){
            var idx = {};
            $campo = reset($('.js-escola-' + campo));
            $.each(dados,function(i,dado){
                if(!idx[dado[campo]]){
                    $campo.append(new Option(dado[campo],dado[campo]));
                }
                idx[dado[campo]] = true;
            });
            $campo.trigger('chosen:updated');
        });
    },
    filtrarItensDestinados: function(){
        var in_array = function(valor, ar){
            res = false;
            $.each(ar,function(i,v){
                if(v == valor){
                    res = true;
                    return false;
                }
            });
            return res;
        }
        var filtro = {};
        $.each(['item','processo','termo', 'situacaoPlanejamento'],function(i,campo){
            var valores = $('.js-escola-' + campo).val();
            if(valores){
                filtro[campo] = valores;
            }
        });
        $.each($('.listagem-destinados tr'),function(a,linha){
            try{
                $linha = $(linha);
                var criterio = true;
                $.each(filtro,function(campo,selecionados){
                    if(!in_array($linha.find('td.js-texto-' + campo).text(), selecionados)){
                        criterio = false;
                        return false;
                    }
                });
                $linha[criterio ? 'show' : 'hide']();
            }catch (e){}
        });
    },
    /**
     * Executa a requisição de leitura dos itens destinados da escola
     * @param {type} escid
     */
    lerItensDestinados: function (escid) {
        $.ajax({
            url: "?modulo=par3/controle/relatorio-escola&acao=A&requisicao=lerItensDestinados",
            data: {"escid": escid},
            dataType: 'json',
            type: 'post'
        }).done(function (result) {
            if (result && !result.error) {
                if(result.hasOwnProperty('sql')) {
                    RelatorioEscola.mostarBotaoSqlDetalhe();
                }
                if (result.sql) {
                    sqlDetalhe = result.sql;
                }
                RelatorioEscola.preencherPainelSqlDetalhe(sqlDetalhe);
                RelatorioEscola.preencherFiltroEscola(result.dados);
                RelatorioEscola.preencherItensDestinados(result.dados);
            }
        });
        setTimeout(function () {
            let popover =  $('[data-toggle="popover"]');
            popover.popover({

            });
            popover.mouseleave(function(e) {
                e.stopPropagation();
                $(document).find('.popover').remove();
            });
        },1000);
    },
    /**
     * Atualiza a tela com as colunas marcadas para serem apresentadas
     * @param {jQuery} $marcadas
     */
    atualizarColunasRelatorio: function ($marcadas) {
        $('.js-col-extra').hide();
        $.each($marcadas, function () {
            $('.js-col-extra.js-col-' + $(this).val()).show();
        });
    },
    /**
     * Atualiza a tela de apresentação da escola com os dados detalhados da escola
     * @param {Object} dados
     */
    visualizarDadosDaEscola: function (dados) {
        $("#map_canvas").hide();
        $('.js-registro').view('assign', dados);
        $('.js-modal-registro').modal();
        $('.js-visualizar-mapa')[dados.coordenadas ? 'show' : 'hide']();
        $('.js-resultado-itens-destinados').hide();
        RelatorioEscola.lerItensDestinados(dados.escid);
    },
    /**
     * Executa a criação e atualização do mapa de acordo com as coordenadas
     * @param {float} lng
     * @param {float} lat
     */
    apresentarMapa: function (lng, lat) {
        lat = lat || -15.799940193919426;
        lng = lng || -47.861337661743164;
        var latlng = new google.maps.LatLng(lat, lng),
                image = 'http://www.google.com/intl/en_us/mapfiles/ms/micons/blue-dot.png';
        //zoomControl: true,
        //zoomControlOptions = google.maps.ZoomControlStyle.LARGE,

        var mapOptions = {
            center: new google.maps.LatLng(lat, lng),
            zoom: 15,
            mapTypeId: google.maps.MapTypeId.ROADMAP,
            panControl: true,
            panControlOptions: {
                position: google.maps.ControlPosition.TOP_RIGHT
            },
            zoomControl: true,
            zoomControlOptions: {
                style: google.maps.ZoomControlStyle.LARGE,
                position: google.maps.ControlPosition.TOP_left
            }
        },
                map = new google.maps.Map(document.getElementById('map_canvas'), mapOptions),
                marker = new google.maps.Marker({
                    position: latlng,
                    map: map,
                    icon: image
                });

        var input = document.getElementById('campodeprocurar');
        var autocomplete = new google.maps.places.Autocomplete(input, {
            types: ["geocode"]
        });

        autocomplete.bindTo('bounds', map);

        var infowindow = new google.maps.InfoWindow();

        google.maps.event.addListener(autocomplete, 'place_changed', function (event) {
            infowindow.close();
            var place = autocomplete.getPlace();
            if (place.geometry.viewport) {
                map.fitBounds(place.geometry.viewport);
            } else {
                map.setCenter(place.geometry.location);
                map.setZoom(10);
            }
            moveMarker(place.name, place.geometry.location);
            $('#obrlatitude').val(place.geometry.location.lat());
            $('#obrlongitude').val(place.geometry.location.lng());
        });

        google.maps.event.addListener(map, 'click', function (event) {
            $('#obrlatitude').val(event.latLng.lat());
            $('#obrlongitude').val(event.latLng.lng());
            infowindow.close();
            var geocoder = new google.maps.Geocoder();
            geocoder.geocode({
                "latLng": event.latLng
            }, function (results, status) {
                if (status == google.maps.GeocoderStatus.OK) {
                    var lat = results[0].geometry.location.lat(),
                            lng = results[0].geometry.location.lng(),
                            placeName = results[0].address_components[0].long_name,
                            latlng = new google.maps.LatLng(lat, lng);
                    moveMarker(placeName, latlng);
                    $("#campodeprocurar").val(results[0].formatted_address);
                }
            });
        });

        function moveMarker(placeName, latlng) {
            marker.setIcon(image);
            marker.setPosition(latlng);
            infowindow.setContent(placeName);
//                 infowindow.open(map, marker);
        }
    },
    /**
     * Executa a requisição da lista de municípios e atualiza o seletor
     */
    listarMunicipios: function () {
        var options = $('.js-municipio');
        options.empty();
        options.trigger('chosen:updated');
        options.append(new Option('', ''));
        $.ajax({
            type: "GET",
            url: window.location.href,
            data: {requisicao: 'lerMunicipios'},
            success: function (retorno) {
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function () {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.trigger('chosen:updated');
            }
        });
    },
    /**
     * Executa a requisição da lista de municípios e atualiza o seletor
     * @param {string} estuf
     * @param {string} muncod
     */
    carregarMunicipioAnalise: function (estuf, muncod) {
        if (estuf != '' && $('.js-seletor-esfera :checked').val() != 1) {
            var options = $('.js-municipio');
            options.empty();
            options.trigger('chosen:updated');
            if (!Array.isArray(estuf)) {
                return;
            }
            $('#loading').hide();
            options.append(new Option('', ''));
            $.ajax({
                type: "GET",
                url: window.location.href,
                data: {requisicao: 'lerMunicipios', estuf: estuf},
                success: function (retorno) {
                    if (retorno !== 'false') {
                        options.append(new Option('', ''));
                        $.each(JSON.parse(retorno), function () {
                            options.append(new Option(this.descricao, this.codigo));
                        });
                        options.focus();
                        options.val(muncod);
                        options.trigger('chosen:updated');
                    }
                }
            });
        }
    },
    /**
     * Retorna se o usuário possui responsabilidade em esfera nacional
     * @returns {Boolean}
     */
    responsabilidadeNacional: function () {
        return !this.responsabilidadeEstadual() && !this.responsabilidadeMunicipal();
    },
    /**
     * Retorna se o usuário possui responsabilidade em esfera estadual
     * @returns {Boolean}
     */
    responsabilidadeEstadual: function () {
        return _JS.responsabilidade.estados && _JS.responsabilidade.estados.length;
    },
    /**
     * Retorna se o usuário possui responsabilidade em esfera municipal
     * @returns {Boolean}
     */
    responsabilidadeMunicipal: function () {
        return _JS.responsabilidade.municipios && _JS.responsabilidade.municipios.length;
    }
};
/**
 * Inicializa a tela
 */
$(document).ready(function () {
    RelatorioEscola.iniciar();
});
// popOverEmendas($anaid)
var formatarAssistencia = function(assistencia,anaid) {

    if(assistencia === 'Emendas') {
        var content = $.ajax({
            url: "?modulo=par3/controle/relatorio-escola&acao=A&requisicao=detalheAssistencia",
            data: {"anaid": anaid},
            dataType: 'json',
            type: 'post',
            async: false
        }).responseText;
        content = JSON.parse(content);
        if(!content) {
            return assistencia;
        }
        var html = `<p><b>Código: </b>${content.emecod}</p>`;
        html += `<p><b>Parlamentar: </b>${content.parlamentar}</p>`;
        return `<span
                data-toggle="popover"
                data-anaid="${anaid}"
                class="js-popup-assistencia"
                data-html="true"
                title="Emenda"
                data-trigger="hover"
                data-placement="left"
                data-content="${html}"
        >${assistencia}
        </span>`;
    }
    return assistencia;
};