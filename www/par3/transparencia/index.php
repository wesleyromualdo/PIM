<?php

include_once('configuracao.php');
include_once('cabecalho.inc');
?>

<style>
    #map_canvas{
        width:500px;
        height:570px;
        position:relative;
        margin:0 auto;
        background: transparent !important;
    }

    #page-wrapper{
        margin: 60px 0 0 0 !important;
        min-height: 0 !important;
    }

    .main-text{
        color: #FFF;
        font-size: 20px;
        padding: 133px 0 0 0;
        min-width: 475px;
    }
    
    #link-e-sic:hover {
    	text-decoration: underline;
    }
</style>

<script type="text/javascript" src="../../library/bootstrap-3.0.0/js/bootstrap.min.js" charset="utf-8"></script>
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false&v=3.24"></script>
<script type="text/javascript" src="js/infobox.js"></script>
<script type="text/javascript" src="../../includes/gmaps/gmaps.js"></script>

<script>
    function abrirObras(estuf) {
        window.open('lista.php?estuf='+estuf, '_self');
    }
</script>

<div class="row" style="margin-top: 15px;">
    <div class="col-lg-4 col-md-4 col-md-offset-2 main-text">
        <p style="padding: 10px 0 0 25px; min-width: 475px">
            A participação da sociedade na gestão pública é um direito assegurado pela Constituição Federal, permitindo que os cidadãos fiscalizem de forma permanente a aplicação dos recursos públicos.
        </p>

        <p style="padding: 10px 0 0 25px; min-width: 475px">
            O acesso público ao Módulo Obras 2.0 possibilita <br />ao cidadão acompanhar a execução dos recursos públicos transferidos pelo FNDE destinados à construção de creches, escolas e quadras poliesportiva.
        </p>

        <p style="padding: 10px 0 0 25px; min-width: 475px">* Para acessar a obra de seu município, localize no mapa o seu estado (UF).</p>
    </div>
    <div class="col-lg-4 col-md-4" style="min-width: 500px;">
        <div id="containerPais">
            <div id="map_canvas"></div>
        </div><br>
        
    </div>
</div>

<div class="row" style="margin: 10px;">
    <div class="col-lg-12 col-md-12">
        <p style="font-size: 16px; color:#FFC600; text-align: center">
            <a id="link-e-sic" target="_blank" style="color:#FFC600; font-weight: bold;" href="https://esic.cgu.gov.br/sistema/site/index.html">e-SIC - Sistema Eletrônico do Serviço de Informação ao Cidadão</a><br>
            Central de Atendimento ao Cidadão: 0800-616161.<br>
            Ouvidoria do FNDE: <a href="mailto:ouvidoria@fnde.gov.br" style="color:#FFC600;">ouvidoria@fnde.gov.br</a>
        </p>
    </div>
</div>
 
<script>
    var Mapa = Mapas;
    jQuery('documento').ready(function() {
        Mapa.estilo = 'externo_blank_pais';
        Mapa.inicializar( '#map_canvas' );
        Mapa.buscaEstadoForm( '#uf', 'obras-fnde' );
    });
</script>

