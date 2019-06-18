<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?key=AIzaSyCxb-cN91hx2XcCaBS3v-STxxEUfe0LT9I"></script>
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->

<div style="clear:both"></div>
<div id="container">
    <div id="local-mapa">
        <div id="menu" class="menu-agenda">
            <div style='float:left'>Estados:</div>
            <div style='float:left;margin-left:10px;'>
                <button id="estado-toggle"  class="btn btn-primary">Selecionar Todos</button>
                <select multiple="multiple" id="estado" name="estado" onchange="" class="multiselect">
                    <?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql);
                    foreach ($estados as $key => $value) {
                        echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
                    }
                    ?>
                </select>
            </div>
            <div id="map_canvastxt"></div>
        </div>
        <div class="panel panel-default">
            <div class="panel-body">
                <div id="directionsPanel" style="width: 300px"></div>
                <div id="map_canvas"></div>
            </div>
        </div>
    </div>
    <div id="legendaMapa" style='display:none; padding-top: 80px'></div>
</div>

<div style="clear:both"></div>

<div id="footer"></div>

<script>
    //onchange="javascript:Mapas.buscaEstadoForm( this, 'assessoramento' );Mapas.atualizaLegenda( this, 'assessoramento-legenda' )"
    jQuery('documento').ready(function(){
        Mapas.inicializar( '#map_canvas' );
        jQuery("#estado").multiselect({
            numberDisplayed: 14,
            id: 'estado'
        });
        //quando Selecionar Todos
        jQuery("#estado-toggle").click(function(e) {
            e.preventDefault();
            multiselect_toggle(jQuery("#estado"), jQuery(this));
            jQuery('#legendaMapa').css( { display : "inline" } );
            Mapas.buscaEstadoForm( jQuery("#estado"), 'agenda-trabalho-estado' );
            Mapas.atualizaLegenda( jQuery("#estado"), 'agenda-trabalho-legenda-estado' );
        });


        jQuery('#legendaMapa').empty().css('display' , 'none');
        //simulando um clique para desmarcar checkbox
        jQuery(":checkbox:checked").click();
        jQuery("#estado-toggle").text("Selecionar Todos");
        //carregando o mapa sem dados
        Mapas.buscaEstadoForm('','');

        //de acordo com ação selecionada na combo Mapa, popular o mapa com as funcoes Mapas.js/ Mapas.class.inc / MetaDados.class.inc/ Fichamonitoramento.class.inc
        jQuery('select[name="estado"]').off('change').on('change', function(e) {
            var target = e.target;

            if (jQuery(target).val()) {
                jQuery('#legendaMapa').empty().css('display' , 'block');
                Mapas.buscaEstadoForm( this, 'agenda-trabalho-estado' );
                Mapas.atualizaLegenda( this, 'agenda-trabalho-legenda-estado' );
                return true;
            };
        });
    });
</script>