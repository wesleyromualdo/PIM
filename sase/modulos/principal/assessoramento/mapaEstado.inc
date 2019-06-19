
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
    <div id="local-mapa">
        <div class='menu-acao' style='float:left; width: 100%;'>
            <span>Mapa:</span>
            <select id="acao-toggle" name="acao" class="btn btn-default">
                <option value="">Selecione</option>
                <option value="1">Adequação</option>
                <option value="2">Lei PNE</option>
                <option value="3">Adesão</option>
            </select>
        </div>
    </div>
    <div style="clear:both"></div>
    <div id="menu" style="display: none; float:left; width: 100%"  class='menu-estados-adequacao'>
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

    jQuery('documento').ready(function(){
        Mapas.inicializar( '#map_canvas' );
        jQuery('#estado').multiselect({
        numberDisplayed: 14,
        id: 'estado'
    });
    
    jQuery("#estado-toggle").click(function(e) {
        e.preventDefault();
        multiselect_toggle(jQuery("#estado"), jQuery(this));
        //definindo ação através do select "Mapa"
        var acao = jQuery("select[name='acao']").val();
        var origemRequisicao1;
        var origemRequisicao2 = origemRequisicao1 = '';
        if(acao == 1){//adequacao
            origemRequisicao1 = 'assessoramento-estado';
            origemRequisicao2 = 'assessoramento-legenda-estado';
        }else if(acao == 2){//lei
            origemRequisicao1 = 'assessoramento-lei-estado';
            origemRequisicao2 = 'assessoramento-legenda-lei-estado';
        }else if(acao == 3){//adesao
            origemRequisicao1 = 'assessoramento-adesao-estado';
            origemRequisicao2 = 'assessoramento-legenda-adesao-estado';
        }

        jQuery('#legendaMapa').css( { display : "inline" } );
            Mapas.buscaEstadoForm( jQuery("#estado"), origemRequisicao1 );
            Mapas.atualizaLegenda( jQuery("#estado"), origemRequisicao2 );
    });
    
    jQuery("select[name=acao]").off('change').on('change', function(e){
       var action = jQuery(this).val();

        jQuery('#legendaMapa').empty().css('display' , 'none');
        //simulando um clique para desmarcar checkbox
        jQuery(":checkbox:checked").click();
        //carregando o mapa sem dados
        Mapas.buscaEstadoForm('','');

        //de acordo com ação selecionada na combo Mapa, popular o mapa com as funcoes Mapas.js/ Mapas.class.inc / MetaDados.class.inc/ SituacaoAssessoramento.class.inc
        if(action !== ''){
            jQuery('.menu-estados-adequacao').css( { display : "inline-block" } );
            jQuery('select[name="estado"]').off('change').on('change', function(e) {
                var action = jQuery('select[name=acao]').val();
                var target = e.target;
                //adequacao
                if(action === '1'){
                    if (jQuery(target).val()) {
                        jQuery('#legendaMapa').empty().css('display' , 'block');
                        Mapas.buscaEstadoForm( this, 'assessoramento-estado' );
                        Mapas.atualizaLegenda( this, 'assessoramento-legenda-estado' );
                        return true;
                    };
                } else if(action === '2'){
                    if (jQuery(target).val()) {
                        jQuery('#legendaMapa').empty().css('display' , 'block');
                        Mapas.buscaEstadoForm( this, 'assessoramento-lei-estado' );
                        Mapas.atualizaLegenda( this, 'assessoramento-legenda-lei-estado' );
                        return true;
                    }
                }else if(action === '3'){
                      if (jQuery(target).val()) {
                          jQuery('#legendaMapa').empty().css('display' , 'block');
                        Mapas.buscaEstadoForm( this, 'assessoramento-adesao-estado' );
                        Mapas.atualizaLegenda( this, 'assessoramento-legenda-adesao-estado' );
                        return true;
                    };
                }
            });

        }else{
            jQuery('#legendaMapa').css( { display : "none" } );
            jQuery('.menu-estados-adequacao').css({ display : "none" });
            location.href = '<?php echo $_SESSION['sisdiretorio']?>.php?modulo=principal/assessoramentoEstado&acao=A&aba=mapa';
        }
    });
});

</script>
<!-- /js -->


