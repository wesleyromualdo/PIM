<?php
$sql = " SELECT  ";
?>

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
            <div id="menu" class='menu-acao'>
                <div style='float:left'>Mapa:</div>
                    <select style='float:left;margin-left:30px;' id="acao-toggle" name="acao" onchange="" class="btn btn-default">
                        <option value="">Selecione</option>
                        <option value="<?php echo Mapas::TIPO_ADEQUACAO; ?>">Adequação</option>
                        <option value="<?php echo Mapas::TIPO_LEIPNE; ?>">Lei PNE</option>
                        <option value="<?php echo Mapas::TIPO_ADESAO; ?>">Adesão</option>
                    </select>
            </div>

            <div style="display:none;" id="menu" class='menu-estados-adequacao'>
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
                    <!-- <div class="panel-heading">Mapa</div> -->
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
<!-- /html -->


<!-- js -->
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
                //definindo ação através do select "Mapa"
                var acao = jQuery("select[name='acao']").val();
                var origemRequisicao1 = origemRequisicao2 = '';
                
                if(acao == 1){//adequacao
                    origemRequisicao1 = 'assessoramento';
                    origemRequisicao2 = 'assessoramento-legenda';
                }else if(acao == 2){//lei
                    origemRequisicao1 = 'assessoramento-lei-municipio';
                    origemRequisicao2 = 'assessoramento-lei-municipio';
                }else if(acao == 3){//adesao
                    origemRequisicao1 = 'assessoramento-adesao-municipio';
                    origemRequisicao2 = 'assessoramento-adesao-municipio';
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
                jQuery("#estado-toggle").text("Selecionar Todos");
                //carregando o mapa sem dados
                /*
                Função comentada por não haver necessidade de recarregar novamente o mapa. Estava dando problema no carregamento dos filtros
                Mapas.buscaEstadoForm('','');
                */
               
                //de acordo com ação selecionada na combo Mapa, popular o mapa com as funcoes Mapas.js/ Mapas.class.inc / MetaDados.class.inc/ SituacaoAssessoramento.class.inc
                if(action !== ''){
//                    Mapas.atualizaLegenda('','');
                    jQuery('.menu-estados-adequacao').css( { display : "inline-block" } );
                    jQuery('select[name="estado"]').off('change').on('change', function(e) {
                        var action = jQuery('select[name=acao]').val();
                        var target = e.target;
                        //adequacao
                        if(action === '1'){
                            if (jQuery(target).val()) {
                                jQuery('#legendaMapa').empty().css('display' , 'block');
                                Mapas.buscaEstadoForm( this, 'assessoramento' );
                                Mapas.atualizaLegenda( this, 'assessoramento-legenda' );
                                return true;
                            };
                        } else if(action === '2'){
                            if (jQuery(target).val()) {
                                jQuery('#legendaMapa').empty().css('display' , 'block');
                                Mapas.buscaEstadoForm( this, 'assessoramento-lei-municipio' );
                                Mapas.atualizaLegenda( this, 'assessoramento-lei-municipio' );
                                return true;
                            }
                        }else if(action === '3'){
                              if (jQuery(target).val()) {
                                  jQuery('#legendaMapa').empty().css('display' , 'block');
                                Mapas.buscaEstadoForm( this, 'assessoramento-adesao-municipio' );
                                Mapas.atualizaLegenda( this, 'assessoramento-adesao-municipio' );
                                return true;
                            };
                        }
                    });
                    
                }else{
                    jQuery('#legendaMapa').css( { display : "none" } );
                    jQuery('.menu-estados-adequacao').css({ display : "none" });
                    location.href = '<?php echo $_SESSION['sisdiretorio']?>.php?modulo=principal/assessoramento&acao=A&aba=mapa';
                }
            });
        });
</script>

