<?php
/**
 *
 */
include "config.inc";
/**
 *
 */

function __autoload($class_name) {
    $arCaminho = array(
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "obras2/classes/modelo/",
        APPRAIZ . "obras/classe/modelo/"
    );

    foreach($arCaminho as $caminho){
        $arquivo = $caminho . $class_name . '.class.inc';
        if ( file_exists( $arquivo ) ){
            require_once( $arquivo );
            break;
        }
    }
}

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "www/obras2/_funcoes.php";
include_once APPRAIZ . "www/obras2/_funcoes_obras_par.php";
include_once APPRAIZ . "www/obras2/_constantes.php";

if(!isset($_SESSION['obras2']['obrid']))
    exit;

$obrid = $_SESSION['obras2']['obrid'];
$obra = new Obras($obrid);
$endereco = new Endereco($obra->endid);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>jQuery.noConflict();</script>
<?
if($endereco->medlongitude){ ?>
    <div id="map-canvas"></div>
<?php } ?>


<script src="<?=GMAPS_API?>v=3.exp"></script>
<script>

    function coordToDec(coord){

        var match, dec, s, d, m,direcao;
        match = coord.split(".");

        d = match[0];
        m = match[1];
        s = match[2];
        direcao = match[3];
        //alert(direcao);
        dec = Math.sign(d) * (Math.abs(d) + (m / 60.0) + (s / 3600.0));
        if(direcao == 'S' || direcao == 'W' || direcao !== 'undefined') {
            dec = dec * -1;
        }

        return dec;

    }



    function initialize() {
        <?php
        if(empty($endereco->medlatitude))
        {
        $municipios = $db->pegaLinha("select  CASE WHEN (length (munmedlat)=8) THEN
								                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
								                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
								                ELSE
								                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
								                END
								            ELSE
								                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
								                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
								                ELSE
								                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
								               END
								            END  as latitude, (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1) as longitude  from territorios.municipio where muncod ='{$endereco->muncod}'");

        ?>
        var myLatlng = new google.maps.LatLng(<?php echo $municipios['latitude']?>,<?php echo $municipios['longitude']?>);
        <?php
        }else{
         ?>
        var myLatlng = new google.maps.LatLng(coordToDec("<?php echo simec_trim($endereco->medlatitude)?>"),coordToDec("<?php echo simec_trim($endereco->medlongitude)?>"));


        <?php } ?>
        var mapOptions = {
            zoom: 12,
            center: myLatlng
        };


        map = new google.maps.Map(document.getElementById('map-canvas'),
            mapOptions);

        var marker = new google.maps.Marker({
            position: myLatlng,
            map: map,
            title: 'Obra!'
        });

    }

    google.maps.event.addDomListener(window, 'load', initialize);


</script>

<style>
    *{
        margin: 0;
        padding: 0;
    }
    #map-canvas {
        height: 500px;
        width: 500px;
        margin: 0px;
        padding: 0px
    }
</style>