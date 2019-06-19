<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 15:16
 */
global $db;

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . "includes/library/simec/Listagem.php";

include APPRAIZ . 'includes/cabecalho.inc';

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Acompanhamento PNE';

if($_POST['acao']){
    switch($_POST['acao']){
        case 'carrega_legenda':
            ob_clean();
            echo getLegendaAcompanhamentoPne($_POST['subid']);
            exit;

        case 'carrega_tabela':
            ob_clean();
            $selapresentacao = $_POST['selapresentacao'];
            carregaDadosMonitoramentoPNE();
            exit;

        case 'carrega_ano':
            ob_clean();
            $subid = $_POST['subid'];

            $paeano = $db->carregar("select paeano from sase.pneanoexibicaodial where subiddial = {$subid} and paetipo = 'E'");
            $pneano = $paeano[0]['paeano'];

            echo $pneano;
            exit;

        case 'carrega_dados_meta':
            ob_clean();
            $metid = $_POST['metid'];
            $sql = "select metchamada, mettitulo from sase.meta where metid = {$metid}";
            $res = $db->carregar($sql);
            echo simec_json_encode($res[0]);
            exit;

        case 'carrega_dados_indicador':
            ob_clean();
            $subid = $_POST['subid'];
            $sql = "select subtitulo from sase.submetadial where subiddial = {$subid}";
            $res = $db->carregar($sql);
            echo $res[0]['subtitulo'];
            exit;
    }
}

// Filtra os municipios
if( $_GET['filtroAjax'] && isset($_GET['estuf']) ){
    ob_clean();
    $sql = "select muncod, mundescricao from territorios.municipio";

    if ($_GET['estuf'] && $_GET['estuf'] != '0'){
        if(is_array($_GET['estuf'])) {
            $sql .= " where estuf in ('" . implode("','", $_GET['estuf']) . "') ";
        }
    }
    $sql .= " order by mundescricao asc ";
    $municipios = $db->carregar($sql);
    ?>
    <select name="muncod" class="form-control" id="muncod" multiple="multiple" onchange="carregarTabela()">
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>
            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
    <?php die();}


// Filtra os indicadores


if( $_GET['filtroAjax'] && $_REQUEST['metid'] ){
    ob_clean(); $indicadores =
    $sql = "SELECT subiddial as codigo, subtitulo as descricao FROM sase.submetadial";
    if( $_REQUEST['metid'] ) $sql .= " where metid = '" . $_REQUEST['metid'] . "' ";
    $sql .= " order by subtitulo asc ";
    $indicadores = $db->carregar($sql);
    ?>

    <select name="subid" class="form-control" id="subid" onchange="carregaDadosIndicador(); carregaMapa(); carregaAno();">
        <option value="">Filtrar indicador:</option>
        <?php
        foreach( $indicadores as $chave => $indicador ){ ?>
            <option value="<?=$indicador['codigo']?>"><?=$indicador['descricao']?></option>
        <?php }?>
    </select>
    <?php die();}

?>
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
        console.log(estuf);
        jQuery.ajax({
            url:'',
            type:'GET',
            data:{filtroAjax:true,estuf:estuf},
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
    function filtraIndicadores(metid){
        jQuery.ajax({
            url:'',
            type:'GET',
            data:{filtroAjax:true,metid:metid},
            success: function( resposta ){
                jQuery('#divFiltroIndicador .inputarea').html( resposta );
                jQuery('#estuf').multiselect('enable');
                jQuery('#estuf').multiselect({
                    numberDisplayed: 1,
                    id: 'estuf'
                });
                jQuery('#todosMuncod').removeAttr('disabled');
            }
        });
    }

    function carregaLegenda(){
        subid = jQuery('#subid').val();
        jQuery.ajax({
            utl: '',
            type: 'POST',
            data: {acao: 'carrega_legenda', subid:subid},
            success: function(data){
                console.log(data);
                jQuery('#divLegenda').html(data);
            }
        });
    }

    function carregaAno(){
        subid = jQuery('#subid').val();
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                acao: 'carrega_ano',
                subid: subid
            },
            success: function(data){
                var ano = parseInt(data);
                console.log(data);
                jQuery('#pneano').val(ano);
            }
        });
    }

    function carregaMapa(){
        var valor = $('#selapresentacao').val();
        var tipo = '';
        if(valor != 'M'){
            $('#divFiltroMunicipio').hide();
        } else {
            $('#divFiltroMunicipio').show();
        }
        switch(valor){
            case 'M':
            default:
                var tipo = 'acompanhamento-pne-municipio';
                break;
            case 'E':
                var tipo = 'acompanhamento-pne-estado';
                break;
        }
        Mapas.buscaEstadoForm( jQuery("#estuf"), tipo );
    }

    function carregarTabela(todos, pagina){
        var mun = jQuery('#muncod').val();
        var est = jQuery('#estuf').val();
        subid  = jQuery('#subid').val();
        selapresentacao = jQuery('#selapresentacao').val();

        todos = typeof todos === 'undefined' ? false : true;
        pagina = typeof pagina === 'undefined' ? false : pagina;
        muncod = '0';
        estuf = '0';
//        if(est != null && mun != null) {
//            if (Array.isArray(mun)) {

                if (Array.isArray(mun) && mun[0] == "") {
                    if (mun.length > 1) {
                        mun.shift();
                        muncod = mun;
                    }
                } else {
                    muncod = mun;
                }

                if (est[0] == "") {
                    if (est.length > 1) {
                        est.shift();
                        estuf = est;
                    }
                } else {
                    estuf = est;
                }

                metid = jQuery('#metid').val();
                var dados = {
                    acao: 'carrega_tabela',
                    subid: subid,
                    muncod: muncod,
                    estuf: estuf,
                    metid: metid,
                    selapresentacao: selapresentacao
                };

                if (pagina) {
                    dados['listagem[p]'] = pagina;
                }

                //        carregaLegenda();
                //        Mapas.atualizaLegenda( jQuery("#estuf"), 'acompanhamento-pne-municipio-legenda' )
                jQuery.ajax({
                    url: '',
                    type: 'POST',
                    data: dados,
                    success: function (dados) {
                        jQuery('#divTabela').html(dados);
                        jQuery('#tb_render').removeAttr('class');
                        resizeTable();
                        carregaLegenda();
                        //                console.log(d)
                    }
                });
//            }
//        }
    }

    function carregaScrollbar(){
        $('#tb_render').find('tbody').css('height', '200px');
        $('#tb_render').find('tbody').scroll();
    }

    function toggleSelect(sel){
        if (multiselect_selected(sel)) {
            //sel.multiselect('deselect_all');
            multiselect_deselectAll(sel);
//            $btn.text("Selecionar Todos");
        } else {
//            sel.multiselect('selectAll', false);
            multiselect_selectAll(sel);
//            $btn.text("Desselecionar Todos");
        }
    }

    function carretaDadosMeta(){
        metid  = jQuery('#metid').val();
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                acao: 'carrega_dados_meta',
                metid: metid
            },
            success: function(data){
                var d = JSON.parse(data);
                jQuery('#divTituloMeta').html('Meta '+d['metchamada']);
                jQuery('#divMeta').html(d['mettitulo']);
            }
        });
    }

    function carregaDadosIndicador(){
        subid = jQuery('#subid').val();
        console.log(subid);
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                acao: 'carrega_dados_indicador',
                subid: subid
            },
            success: function(data){
                console.log(data);
                jQuery('#divTituloIndicador').html(data);
                jQuery('#divTituloIndicador').show();
            }
        });
    }

</script>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?key=AIzaSyCxb-cN91hx2XcCaBS3v-STxxEUfe0LT9I"></script>
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
        height: 428px !important;
    }
    #legendaMapa{
        padding-top: 0px !important;
    }
    #map_canvastxt{
        float: none;
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

</style>

<div id="container">
    <form class="form-horizontal" name="formMapa" id="formMapa" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>

        <div class="row">
            <div class="col-lg-2">
                <div class="well">
                    <fieldset>
                        <legend>Filtros</legend>

                        <div class="form-group">
                            <label for="filtroEstado" class="col-lg-12">Meta:</label>
                            <div class="col-lg-12">
                                <select name="metid" id="metid" class="form-control" onchange="javascript:if(this.value!=''){carretaDadosMeta(); filtraIndicadores(this.value);}">
                                    <option value="">Filtrar meta</option>
                                    <?php
                                    $sql = "SELECT metid as codigo, 'Meta '||metid as descricao FROM sase.meta ORDER BY metid";
                                    $estados = $db->carregar($sql);
                                    foreach( $estados as $chave => $estado ){ ?>
                                        <option value="<?=$estado['codigo']?>"><?=$estado['descricao']?></option>
                                    <?php }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroIndicador">
                            <label for="filtroMunicipio" class="col-lg-12">Indicador:</label>
                            <div class="col-lg-12 inputarea">
                                <?php $dis = ''; if(!$_POST['metid']){ $dis = 'disabled'; } ?>
                                <select name="subid" <?= $dis ?> class="form-control" id="subid" onchange="carregaDadosIndicador(); carregaMapa(); carregaAno();">
                                    <option value="">Filtrar indicador</option>
                                </select>
                            </div>
                        </div>

                        <div class="form-group">
                            <?php
                            $anos = array(2010, 2025);
                            $a = $anos[0];
                            $opts = '';
                            ?>
                            <label for="filtroEstado" class="col-lg-12">Ano Previsto:</label>
                            <div class="col-lg-12" id="divAno">
                                <select name="pneano" id="pneano" class="form-control" onchange="carregaMapa()">
                                    <option value="">Selecione</option>
                                    <?php
                                    while($a <= $anos[1]){
                                        $sel = $dadosMun['anoprevisto'] == $a ? 'selected' : '';
                                        $opts .= "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                        $a++;
                                    }
                                    echo $opts;
                                    ?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="filtroEstado" class="col-lg-12">Apresentação:</label>
                            <div class="col-lg-12" id="divAno">
                                <select name="selapresentacao" id="selapresentacao" class="form-control">
                                    <option value="M">Municipal</option>
                                    <option value="E">Estadual</option>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroEstado">
                            <label for="filtroEstado" class="col-lg-12">
                                Estado:
<!--                                <input type="button" value="Todos" onclick="toggleSelect(jQuery('#estuf')); carregaMapa(); filtraMunicipios(); carregarTabela();" class="btn btn-primary"/>-->
                            </label>
                            <div class="col-lg-12" style="margin-top: 5px;">
                                <select name="estuf" id="estuf" class="form-control" multiple="multiple" disabled onchange="javascript:carregaMapa(); filtraMunicipios(); carregarTabela();">
                                    <?php
                                    $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                    $estados = $db->carregar($sql);
                                    foreach( $estados as $chave => $estado ){ ?>
                                        <option <?=($estado['estuf']==$_POST['estuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                    <?php }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroMunicipio">
                            <label for="filtroMunicipio" class="col-lg-12">
                                Município:
<!--                                <input type="button" value="Todos" onclick="toggleSelect(jQuery('#muncod')); carregarTabela();" class="btn btn-primary"/>-->
                            </label>
                            <div class="col-lg-12 inputarea"  style="margin-top: 5px;">
                                <?php $dis = ''; if(!$_POST['estuf']){ $dis = 'disabled'; } ?>
                                <select name="muncod" <?= $dis ?> class="form-control" id="muncod" multiple="multiple" onchange="carregarTabela()">
                                </select>
                            </div>
                        </div>

                    </fieldset>
                </div>
            </div>

            <div class="col-lg-10">
                <div class="well">
                    <fieldset>
                        <legend><div id="map_canvastxt"><?= $titulo ?></div></legend>
                        <div class="form-group" id="divMapa">
                            <div class="row">
                                <div id="local-mapa" class="col-lg-12 col-sm-12">
                                    <div class="panel panel-default">
                                        <div class="panel-body">
                                            <div id="directionsPanel">
                                                <div id="divTituloMeta"></div>
                                                <div id="divMeta"></div>
                                            </div>
                                            <div id="divTituloIndicador" class="well"></div>
                                            <div id="map_canvas"></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-lg-12 col-sm-12">
                                    <div id="divLegenda" class="col-lg-8 col-sm-8 col-lg-offset-2"></div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-lg-12 col-sm-12">
                                    <?php
                                    $anos = array(2015, 2025);
                                    ?>
                                    <div id="divTabela">
                                    </div>
                                </div>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </div>

    </form>
</div>
<script>
    jQuery('documento').ready(function(){
        Mapas.inicializar( '#map_canvas' );
        jQuery('#estuf').multiselect({
            numberDisplayed: 1,
            id: 'estuf'
        });
        jQuery('#muncod').multiselect({
            numberDisplayed: 1,
            id: 'muncod'
        });

        $('#selapresentacao').change(function(){
            carregaMapa();
            carregarTabela();
        });

        // Change the selector if needed
        var $table = $('table.scroll'),
            $bodyCells = $table.find('tbody tr:first').children(),
            colWidth;

// Adjust the width of thead cells when window resizes
        $(window).resize(function() {
            // Get the tbody columns width array
            colWidth = $bodyCells.map(function() {
                return $(this).width();
            }).get();

            // Set the width of thead columns
            $table.find('thead tr').children().each(function(i, v) {
                $(v).width(colWidth[i]);
            });
        }).resize(); // Trigger resize handler
    });

function resizeTable(){
    var $table = $('table.scroll'),
        $bodyCells = $table.find('tbody tr').children(),
        colWidth;

// Adjust the width of thead cells when window resizes
    $(window).resize(function() {
        // Get the tbody columns width array
        colWidth = $bodyCells.map(function() {
            return $(this).width();
        }).get();
        console.log(colWidth);

        // Set the width of thead columns
        $table.find('thead tr').children().each(function(i, v) {
            $(v).width(colWidth[i]);
        });
    }).resize();
}

    delegatePaginacao = function()
    {
        $('body').on('click', '.container-listing li[class="pgd-item"]:not(".disabled")', function(){
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            // -- Submetendo o formulário
            carregarTabela(true, novaPagina);
        });
    };
</script>