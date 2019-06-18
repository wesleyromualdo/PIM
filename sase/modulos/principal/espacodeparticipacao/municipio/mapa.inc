<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 07/06/2016
 * Time: 10:25
 */
global $db;

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . "includes/library/simec/Listagem.php";
include APPRAIZ . 'includes/cabecalho.inc';
if($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'carrega_municipio':
            ob_clean();
            $estuf = implode("','", $_REQUEST['estuf']);
            $sql = "select muncod, mundescricao from territorios.municipio where estuf in ('{$estuf}')";
            $res = $db->carregar($sql);
            $opts = "";
            foreach ($res as $r) {
                $opts .= <<<HTML
                    <option value="{$r['muncod']}">{$r['mundescricao']}</option>
HTML;
            }
            echo <<<HTML
                <select name="muncod[]" class="form-control" id="muncod" multiple="multiple">
                    {$opts}
                </select>
HTML;
            exit;
    }
}

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Espaço de Participação';


?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css">
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->

<style>
    td, th{
        height: 10px;
        padding: 0px !important;
    }
    td  {
        background-color: #FFFFFF;
    }
    #divMapa{
        margin-bottom: 0px !important;
    }
    #map_canvas{
        border: none !important;
        height: 470px !important;
    }
    #legendaMapa{
        padding-top: 0px !important;
    }
    #map_canvastxt{
        float: right;
    }
    #divLegenda{
        margin-top: 10px;
    }
    .txts{
        position: inherit !important;
        float: right !important;
        background-color: transparent !important;
    }
    .btn-todos{
        margin-bottom: 3px;
        padding: 0px 12px !important;
    }
    .btn-primary{
        height: 21px;
        width: 51px;
        padding: 0 !important;
    }
    #local-mapa{
        width: 100% !important;
    }

    /* Tabela */
    #tb_render {
        border-spacing: 0;
        border: 2px solid black;
    }

    #tb_render tbody,
    #tb_render thead { display: block; }

    #tb_render thead tr th {
        height: 30px;
        line-height: 30px;
    }

    #tb_render tbody {
        height: 100px;
        overflow-y: auto;
        overflow-x: hidden;
    }

    #tb_render tbody { border-top: 2px solid black; }

    #tb_render tbody td, #tb_render thead th {
        border-right: 1px solid black;
    }

    #tb_render tbody td:last-child, #tb_render thead th:last-child {
        border-right: none;
    }

    #directionsPanel{
        width: 100%;
        margin-bottom: 10px;
    }

    #divTituloMeta{
        margin-bottom: 5px;
        font-size: 19px;
        font-weight: bold;
        color: #1f6b9f;
    }

    #divMeta{
        font-size: 14px;
        font-weight: bold;
    }

    #divTituloIndicador{
        display: none;
        margin-bottom: 0px !important;
        border-bottom-right-radius: 0px !important;
        border-bottom-left-radius: 0px !important;
        padding-top: 10px !important;
        padding-bottom: 10px !important;
        font-weight: bold;
    }

    /*Exemplo*/
    table.scroll {
        width: 100%;
    }

    table.scroll tbody,
    table.scroll thead { display: block; }

    thead tr th {
        height: 30px;
        line-height: 30px;
    }

    table.scroll tbody {
        height: 200px;
        width: 100%;
        overflow-y: auto;
        overflow-x: hidden;
    }

    table.scroll tbody td {
        min-width: 65px;
    }

    #btn-load, #btnxls{
        height: 30px;
    }

    .well fieldset legend {
        margin-bottom: 10px !important;
    }

    #panel-mapa{
        margin-bottom: 0px !important;
    }
</style>

<script>
    function filtraMunicipios(){
        estuf = '0';
        var est = jQuery('#estuf').val();
        if(Array.isArray(est)) {
            if (est[0] == "") {
                if (est.length > 1) {
                    est.shift();
                    estuf = est;
                }
            } else {
                estuf = est;
            }
        }

        jQuery.ajax({
            url:'',
            type:'POST',
            data:{acao:'carrega_municipio',estuf:estuf},
            success: function( resposta ){
                jQuery('#divFiltroMunicipio .inputarea').html( resposta );
                jQuery('#muncod').multiselect('enable');
                jQuery('#muncod').multiselect({
                    numberDisplayed: 1,
                    id: 'muncod'
                });
            }
        });
    }
</script>
<div id="container">
    <div class="row">
        <div class="col-lg-2">
            <div class="well">
                <fieldset>
                    <legend>Filtros</legend>

                    <div class="form-group">
                        <label for="tpMapa" class="col-lg-12">Tipo</label>
                        <div class="col-lg-12">
                            <select name="tpmapa" id="tpmapa" class="form-control">
                                <option value="f">Forum</option>
                                <option value="c">Conselho</option>
                                <option value="s">Sistema</option>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroEstado">
                        <label for="filtroEstado" class="col-lg-12">
                            Estado:
                            <!--                                <input type="button" value="Todos" onclick="toggleSelect(jQuery('#estuf')); carregaMapa(); filtraMunicipios(); carregarTabela();" class="btn btn-primary"/>-->
                        </label>
                        <div class="col-lg-12" style="margin-top: 5px;">
                            <select name="estuf[]" id="estuf" class="form-control" multiple="multiple">
                                <?php
                                $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                $estados = $db->carregar($sql);
                                foreach( $estados as $chave => $estado ){ ?>
                                    <option <?=($estado['estuf']==$_POST['estuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <div class="col-lg-12" align="center">
                            <button type="button" class="btn btn-primary" id="btn-load" title="Carregar mapa"><i class="fa fa-search"></i></button>
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>

        <div class="col-lg-8">
            <div class="well">
                <fieldset>
                    <legend>Mapa:<div id="map_canvastxt"></div></legend>

                    <div class="form-group" id="divMapa">
                        <div class="row">
                            <div id="local-mapa" class="col-lg-12 col-sm-12">
                                <div class="panel panel-default" id="panel-mapa">
                                    <div class="panel-body">
                                        <div id="map_canvas"></div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>

                </fieldset>
            </div>
        </div>

        <div class="col-lg-2">
            <div class="well">
                <fieldset>
                    <legend>Legenda</legend>
                    <div class="panel panel-default" style="height: 117px; padding-left: 10px;">
                        <div id="legendaMapa" class="col-lg-12"></div>
                    </div>
                </fieldset>
            </div>
        </div>
    </div>
</div>
<script>
    jQuery('documento').ready(function(){
        Mapas.inicializar( '#map_canvas' );
        Mapas.atualizaLegenda(jQuery("#estuf"), 'espacodeparticipacao-municipio-legenda' );

        jQuery('#estuf').multiselect({
            numberDisplayed: 1,
            id: 'estuf'
        });

        jQuery('#muncod').multiselect({
            numberDisplayed: 1,
            id: 'muncod'
        });

        jQuery('#btn-load').click(function(){
            Mapas.buscaEstadoForm(jQuery('#estuf'), 'espacodeparticipacao-municipio');
            Mapas.atualizaLegenda(jQuery("#estuf"), 'espacodeparticipacao-municipio-legenda' );
        });

    });
</script>
