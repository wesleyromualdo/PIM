
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->


<div style="clear:both"></div>

<div id="container">
    <div style="clear:both"></div>
    <div id="menu" style="float:left; width: 100%"  class='menu-estados'>
        <div style='float:left;margin-left:10px;'>
        <span>Estados:</span>
            <button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
            <select multiple="multiple" id="estado" name="estado" class="multiselect">
                <?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql); 
                    foreach ($estados as $key => $value) {
                        echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
                }?>

            </select>
        </div>
        <div id="map_canvastxt"></div>
    </div>
</div>
<div id="container">
    <div class="panel panel-default" style="width: 80%; float: left;">
         <!--<div class="panel-heading">Mapa</div>--> 
        <div class="panel-body">
            <div id="directionsPanel" style="width: 300px"></div>
            <div id="map_canvas"></div>
        </div>
    </div>
    <div id="legendaMapa" style='display:none; width:20%; padding-top:0px;text-align:left;'>
</div>


<div style="clear:both"></div>

<div id="footer"></div>
<!-- /html -->


<!-- js -->
<script>
    //onchange="javascript:Mapas.buscaEstadoForm( this, 'monitoramento' );Mapas.atualizaLegenda( this, 'monitoramento-legenda' )" 
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
                Mapas.buscaEstadoForm( jQuery("#estado"), 'monitoramento-estado' );
                Mapas.atualizaLegenda( jQuery("#estado"), 'monitoramento-legenda-estado' );
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
                            Mapas.buscaEstadoForm( this, 'monitoramento-estado' );
                            Mapas.atualizaLegenda( this, 'monitoramento-legenda-estado' );
                            return true;
                        };
                });
        });
</script>
<!-- /js -->


